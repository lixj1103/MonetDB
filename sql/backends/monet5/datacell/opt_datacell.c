/*
 * The contents of this file are subject to the MonetDB Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.monetdb.org/Legal/MonetDBLicense
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is the MonetDB Database System.
 *
 * The Initial Developer of the Original Code is CWI.
 * Portions created by CWI are Copyright (C) 1997-July 2008 CWI.
 * Copyright August 2008-2012 MonetDB B.V.
 * All Rights Reserved.
 */

/*
 * @a M. Kersten
 * @- Datacell optimizer
 * Assume simple queries . Clear out all non-datacell schema related sql statements, except
 * for the bare minimum.
 */
/*
 * We keep a flow dependency table to detect.
 */
#include "monetdb_config.h"
#include "opt_datacell.h"
#include "opt_deadcode.h"
#include "mal_interpreter.h"    /* for showErrors() */
#include "mal_builder.h"
#include "basket.h"
#include "sql_optimizer.h"
#include "opt_statistics.h"
#include "opt_dataflow.h"

int
OPTdatacellImplementation(Client cntxt, MalBlkPtr mb, MalStkPtr stk, InstrPtr pci)
{
	int actions = 0, fnd, mvc = 0;
	int bskt, i, j, k, limit, /*vlimit,*/ slimit;
	InstrPtr r, p, qq, *old;
	str col;
	int maxbasket = 128, m = 0, a = 0;
	char *tables[128] = { NULL };
	char *appends[128] = { NULL };
	InstrPtr q[128], qa[128] = { NULL };
	lng clk /*,t*/;
	int *alias;
	char buf[BUFSIZ];

	(void) pci;

	OPTDEBUGdatacell {
		mnstr_printf(cntxt->fdout, "#Datacell optimizer started\n");
		printFunction(cntxt->fdout, mb, stk, LIST_MAL_STMT);
	} else
		(void) stk;

	old = mb->stmt;
	limit = mb->stop;
	slimit = mb->ssize;
	/*vlimit = mb->vtop;*/
	if (newMalBlkStmt(mb, slimit) < 0)
		return 0;

	alias = (int *) GDKzalloc(mb->vtop * 2 * sizeof(int));
	if (alias == 0)
		return 0;
	removeDataflow(old, limit);

	pushInstruction(mb, old[0]);
	newFcnCall(mb, sqlRef, putName("transaction", 11));
	for (i = 1; i < limit; i++)
		if (old[i]) {
			p = old[i];

			if (getModuleId(p) == datacellRef && getFunctionId(p) == putName("window", 6) &&
				isVarConstant(mb, getArg(p, 1)) && isVarConstant(mb, getArg(p, 2)) && isVarConstant(mb, getArg(p, 3))) {
				/* let's move the window to the start of the block  when it consists of constants*/
				pushInstruction(mb, p);
				for (j = mb->stop - 1; j > 1; j--)
					mb->stmt[j] = mb->stmt[j - 1];
				mb->stmt[j] = p;
				continue;
			}
			if (getModuleId(p) == datacellRef && (getFunctionId(p) == putName("threshold", 9) || getFunctionId(p) == putName("beat", 4)) &&
				isVarConstant(mb, getArg(p, 1)) && isVarConstant(mb, getArg(p, 2))) {
				/* let's move the threshold/beat to the start of the block  when it consists of constants*/
				pushInstruction(mb, p);
				for (j = mb->stop - 1; j > 1; j--)
					mb->stmt[j] = mb->stmt[j - 1];
				mb->stmt[j] = p;
				continue;
			}

			if (p->token == ENDsymbol) {
				/* a good place to commit the SQL transaction */
				for (j = 0; j < a; j++)
					pushInstruction(mb, qa[j]);
				/* catch any exception left behind */
				r = newAssignment(mb);
				j = getArg(r, 0) = newVariable(mb, GDKstrdup("SQLexception"), TYPE_str);
				setVarUDFtype(mb, j);
				r->barrier = CATCHsymbol;
				r = newAssignment(mb);
				getArg(r, 0) = j;
				r->barrier = EXITsymbol;
				r = newAssignment(mb);
				j = getArg(r, 0) = newVariable(mb, GDKstrdup("MALexception"), TYPE_str);
				setVarUDFtype(mb, j);
				r->barrier = CATCHsymbol;
				r = newAssignment(mb);
				getArg(r, 0) = j;
				r->barrier = EXITsymbol;

				(void) newFcnCall(mb, sqlRef, commitRef);
				break;
			}
			if (getModuleId(p) == sqlRef && getFunctionId(p) == putName("affectedRows", 12)) {
				freeInstruction(p);
				continue;
			}

			if (getModuleId(p) == sqlRef && getFunctionId(p) == mvcRef)
				mvc = getArg(p, 0);

			if (getModuleId(p) == sqlRef && (getFunctionId(p) == bindRef || getFunctionId(p) == binddbatRef)) {
				snprintf(buf, BUFSIZ, "%s.%s", getVarConstant(mb, getArg(p, 2)).val.sval, getVarConstant(mb, getArg(p, 3)).val.sval);
				col = getVarConstant(mb, getArg(p, 4)).val.sval;
				bskt = BSKTlocate(buf);
				if (bskt) {
					for (j = 0; j < m; j++)
						if (strcmp(buf, tables[j]) == 0)
							break;
					if (j == m) {
						if (m == maxbasket)
							return 0;
						/* grab the basket tables by swapping the BATs used for the SQL table */
						q[m] = BSKTgrabInstruction(mb, buf);
						tables[m++] = GDKstrdup(buf);
						actions = 1;
					}
				} else {
					pushInstruction(mb, p);
					continue;
				}

				fnd = 0;
				if (bskt && getFunctionId(p) == bindRef && getVarConstant(mb, getArg(p, p->argc - 1)).val.ival == 0) {
					/* only the primary BAT is used and inject the call to empty the basket. */
					for (j = 0; fnd == 0 && j < baskets[bskt].colcount; j++)
						if (strcmp(baskets[bskt].cols[j], col) == 0) {
							for (k = 0; k < m; k++)
								if (strcmp(buf, tables[k]) == 0) {
									alias[getArg(p, 0)] = getArg(q[k], j);
									fnd = 1;
									break;
								}
							break;
						}

					if (fnd == 0){
						for (j = 0; j < p->argc; j++)
							if (alias[getArg(p, j)])
								getArg(p, j) = alias[getArg(p, j)];
						pushInstruction(mb, p);
					} else
						freeInstruction(p);
					continue;
				} else
				if (bskt) {
					wrd rows = 0;
					ValRecord vr;
					/* zap all expression arguments */
					getModuleId(p) = batRef;
					getFunctionId(p) = newRef;
					p->argc = p->retc= 1;
					p = pushType(mb, p, TYPE_oid);
					p = pushType(mb, p, getTailType(getVarType(mb,getArg(p,0))));
					varSetProp(mb, p->argv[0], rowsProp, op_eq, VALset(&vr, TYPE_wrd, &rows));
				}
			}

			for (j = 0; j < p->argc; j++)
				if (alias[getArg(p, j)])
					getArg(p, j) = alias[getArg(p, j)];

			if (getModuleId(p) == sqlRef && getFunctionId(p) == appendRef) {
				/* the appends come in multiple steps.
				   The first initializes an basket update statement,
				   which is triggered when we commit the transaction.
				 */
				snprintf(buf, BUFSIZ, "%s.%s", getVarConstant(mb, getArg(p, 2)).val.sval, getVarConstant(mb, getArg(p, 3)).val.sval);
				col = getVarConstant(mb, getArg(p, 4)).val.sval;
				bskt = BSKTlocate(buf);
				if (bskt) {
					for (j = 0; j < a; j++)
						if (strcmp(buf, appends[j]) == 0)
							break;
					if (j == a) {
						if (a == maxbasket)
							return 0;
						/* grab the basket tables instruction */
						qa[a] = BSKTupdateInstruction(mb, buf);
						appends[a++] = GDKstrdup(buf);
						actions = 1;
					}
					fnd = 0;
					for (k = 0; fnd == 0 && k < baskets[bskt].colcount; k++)
						if (strcmp(baskets[bskt].cols[k], col) == 0) {
							fnd++;
							break;
						}

					if (fnd) {
						/* upgrade single values to a BAT */
						clrFunction(p);
						if (!isaBatType(getVarType(mb, getArg(p, 5)))) {
							qq= newInstruction(mb, ASSIGNsymbol);
							getModuleId(qq) = sqlRef;
							getFunctionId(qq) = singleRef;
							getArg(qq, 0) = getArg(qa[j], k + 2);
							getArg(qq, 1) = getArg(p, 5);
							qq->argc = 2;
							p = qq;
						} else {
							qq= newAssignment(mb);
							getArg(qq, 0) = getArg(qa[j], k + 2);
							getArg(qq, 1) = getArg(p, 5);
							qq->argc = 2;
							alias[getArg(p,0)] = getArg(p,1);
							p->argc = 2;
							mvc = getArg(p, 0);
						}
					}
				} else {
					if ( alias[mvc] )
						getArg(p, 1) = alias[mvc];
					mvc = getArg(p, 0);
				}
			}
			pushInstruction(mb, p);
		}

    /* take the remainder as is */
    for (; i<limit; i++)
        if (old[i])
            pushInstruction(mb,old[i]);
	(void) stk;
	(void) pci;

	if (actions)
	{
		clk = GDKusec();
		optimizerCheck(cntxt, mb, "optimizer.datacell", 1, /*t =*/ (GDKusec() - clk), OPT_CHECK_ALL);
		addtoMalBlkHistory(mb, "datacell");
	}
	GDKfree(alias);
	for (j = 0; j < m; j++)
		GDKfree(tables[j]);
	return actions;
}

/* #define _DEBUG_OPTIMIZER_*/

str OPTdatacell(Client cntxt, MalBlkPtr mb, MalStkPtr stk, InstrPtr p)
{
	str modnme;
	str fcnnme;
	str msg = MAL_SUCCEED;
	Symbol s = NULL;
	lng t = 0, clk = GDKusec();
	int actions = 0;

	optimizerInit();
	if (p)
		removeInstruction(mb, p);
	OPTDEBUGdatacell mnstr_printf(cntxt->fdout, "=APPLY OPTIMIZER datacell\n");
	if (p && p->argc > 1) {
		if (getArgType(mb, p, 1) != TYPE_str ||
			getArgType(mb, p, 2) != TYPE_str ||
			!isVarConstant(mb, getArg(p, 1)) ||
			!isVarConstant(mb, getArg(p, 2))
			) {
			throw(MAL, "optimizer.datacell", ILLARG_CONSTANTS);
		}
		if (stk != 0) {
			modnme = *(str *) getArgReference(stk, p, 1);
			fcnnme = *(str *) getArgReference(stk, p, 2);
		} else {
			modnme = getArgDefault(mb, p, 1);
			fcnnme = getArgDefault(mb, p, 2);
		}
		s = findSymbol(cntxt->nspace, putName(modnme, strlen(modnme)), putName(fcnnme, strlen(fcnnme)));

		if (s == NULL) {
			char buf[1024];
			snprintf(buf, 1024, "%s.%s", modnme, fcnnme);
			throw(MAL, "optimizer.datacell", RUNTIME_OBJECT_UNDEFINED ":%s", buf);
		}
		mb = s->def;
		stk = 0;
	}
	if (mb->errors) {
		/* when we have errors, we still want to see them */
		addtoMalBlkHistory(mb, "datacell");
		return MAL_SUCCEED;
	}
	actions = OPTdatacellImplementation(cntxt, mb, stk, p);
	addOptimizers(cntxt, mb);
	if (msg == MAL_SUCCEED)
		msg = optimizeMALBlock(cntxt, mb);
	if (msg == MAL_SUCCEED)
		msg = optimizerCheck(cntxt, mb, "optimizer.datacell", actions, t = (GDKusec() - clk), OPT_CHECK_ALL);
	OPTDEBUGdatacell {
		mnstr_printf(cntxt->fdout, "=FINISHED datacell %d\n", actions);
		printFunction(cntxt->fdout, mb, 0, LIST_MAL_STMT | LIST_MAPI);
	}
	DEBUGoptimizers
		mnstr_printf(cntxt->fdout, "#opt_reduce: " LLFMT " ms\n", t);
	QOTupdateStatistics("datacell", actions, t);
	addtoMalBlkHistory(mb, "datacell");
	return msg;
}
