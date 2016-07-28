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
 * Copyright August 2008-2014 MonetDB B.V.
 * All Rights Reserved.
 */

/* (c) M. Kersten
 * Pass relevant algebra operations through the mosaic library.
 * It should only look at direct use of persistent readonly columns.
 * This can be recognized by observing the various components being accessed.
 */
#include "monetdb_config.h"
#include "opt_mosaic.h"
#include "mosaic.h"
#include "mtime.h"

/* #define _DEBUG_MOSAIC_*/

static int OPTmosaicType(MalBlkPtr mb, InstrPtr pci, int idx)
{	int type;
	switch(type = getBatType( getArgType(mb,pci,idx))){
	case TYPE_bte:
	case TYPE_bit:
	case TYPE_sht:
	case TYPE_int:
	case TYPE_lng:
#ifdef HAVE_HGE
	case TYPE_hge:
#endif
	case TYPE_oid:
	case TYPE_flt:
	case TYPE_dbl:
	case TYPE_str:
		return 1;
	default:
		if( type == TYPE_date)
			return 1;
		if( type == TYPE_daytime)
			return 1;
		if( type == TYPE_timestamp)
			return 1;
	}
	return 0;
}

int 
OPTmosaicImplementation(Client cntxt, MalBlkPtr mb, MalStkPtr stk, InstrPtr pci)
{
	InstrPtr p,q, *old;
    int limit,i,j, k, target=0;
	signed char *check;

	check = GDKzalloc(mb->vsize);
	if ( check == NULL)
		return 0;

#ifdef _DEBUG_MOSAIC_
	mnstr_printf(cntxt->fdout,"#mosaic implementation\n");
    printFunction(cntxt->fdout,mb,0,LIST_MAL_ALL);
#endif
	limit = mb->stop;
	old = mb->stmt;
	if ( newMalBlkStmt(mb, mb->ssize) < 0)
		return 0;

	(void) cntxt;
	(void) pci;
	(void) stk;

	// pre-scan to identify all potentially compressed columns
    for( i=1; i < limit; i++){
        p = old[i];
        if ( getModuleId(p) == sqlRef && getFunctionId(p) == bindRef && getVarConstant(mb,getArg(p,5)).val.ival == 0 && OPTmosaicType(mb,p,0)){
				check[getArg(p,0)] = 1;
		} else
        if ( getModuleId(p) == sqlRef && getFunctionId(p) == bindRef && getVarConstant(mb,getArg(p,5)).val.ival != 0){
			// locate the corresponding base column and turn it off
			for( k= i-1; k>0; k--){
				q = old[k];
				if ( getArg(q,2) == getArg(p,2) && getArg(q,3) == getArg(p,3) && getArg(q,4) == getArg(p,4))
					check[getArg(q,0)] = 0;
			}
		} else
        if ( getModuleId(p) == algebraRef && (getFunctionId(p) == subselectRef || getFunctionId(p) == thetasubselectRef) && check[getArg(p,1)] ) 
                /* ok */;
		else
        if ( getModuleId(p) == algebraRef && getFunctionId(p) == projectionRef && check[getArg(p,2)])
                /* ok */;
		else
        if ( getModuleId(p) == algebraRef && getFunctionId(p) == subjoinRef && (check[getArg(p,2)] || check[getArg(p,1)]) && p->argc ==8)
                /* ok */;
		else
		if ( p->token == ASSIGNsymbol)
			for( j=0; j < p->retc; j++)
				check[getArg(p,j)]= check[getArg(p,p->retc+j)];
		else
		// mark all that needs decompression
		for(j= p->retc; j<p->argc; j++)
		if( check[getArg(p,j)] )
			check[getArg(p,j)] = -1;
    }
	// actual conversion
    for( i=0; i < limit; i++){
        p = old[i];
        if ( getModuleId(p) == sqlRef && getFunctionId(p) == bindRef && getVarConstant(mb,getArg(p,5)).val.ival == 0 && check[getArg(p,0)]< 0){
			//decompress before use such that it can be used properly
			pushInstruction(mb,p);
			j=  getArg(p,0);
			check[getArg(p,0)] = 0;

			q = newStmt(mb,mosaicRef,decompressRef);
			setVarType(mb,getArg(q,0), getVarType(mb,getArg(p,0)));
			setVarUDFtype(mb,getArg(q,0));

			getArg(p,0) = getArg(q,0);
			q = pushArgument(mb,q,getArg(q,0));
			getArg(q,0) = j;
			p= 0;
		} 
		else
        if ( getModuleId(p) == algebraRef && (getFunctionId(p) == subselectRef || getFunctionId(p) == thetasubselectRef) && check[getArg(p,1)] != 0)
                setModuleId(p, mosaicRef);
		else
        if ( getModuleId(p) == algebraRef && getFunctionId(p) == projectionRef && check[getArg(p,2)] != 0)
                setModuleId(p, mosaicRef);
		 else
        if ( getModuleId(p) == algebraRef && getFunctionId(p) == subjoinRef && (check[getArg(p,2)] || check[getArg(p,1)] != 0) && p->argc ==8)
                setModuleId(p, mosaicRef);
		if( p )
			pushInstruction(mb,p);
    }
	GDKfree(old);
	GDKfree(check);
#ifdef _DEBUG_MOSAIC_
    printFunction(cntxt->fdout,mb,0,LIST_MAL_ALL);
#endif
	return target == 0;
}