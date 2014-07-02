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
 * Copyright August 2008-2013 MonetDB B.V.
 * All Rights Reserved.
 */

/*
 * @a Minh-Duc Pham
 *
 */
#include "monetdb_config.h"
#include "mal_exception.h"
#include <gdk.h>
#include <rdf.h>
#include <rdftypes.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>

/*
 * Get substring of a string
 * NOTE: The position starts from 1 (not from 0)
 * */

char* substring(char *string, int position, int length) 
{
	char *pointer;
	int c;

	pointer = GDKmalloc(length+1);

	if (pointer == NULL)
	{
		throw(RDF, "rdf_shredder.substring", "Memory allocation failed!");
	}

	for (c = 0 ; c < position -1 ; c++) 
		string++; 

	for (c = 0 ; c < length ; c++)
	{
		*(pointer+c) = *string;      
		string++;   
	}

	*(pointer+c) = '\0';

	return pointer;
}


/* 
	This function returns 1 if architecture 
	is little endian, 0 in case of big endian.
*/

int is_little_endian(void)
{
	unsigned int x = 1;
	char *c = (char*) &x;
	return (int)*c;
}

char isInt(char *input, int len){
	
	int 	i = 0;
	//int	len = strlen(input);
	//printf("... Checking value %s with len %d \n", input, len);
	if (len > 11) return 0;

	if (input[0] != '-' && input[0] != '+' && isdigit(input[0]) == 0)
		return 0; 	

	for(i = 1; i < len; i++)
	{
		if(isdigit(input[i]) == 0){ // May also check ispunct(string[i]) != 0 
			//printf("NOT A DIGIT \n");
			break;
		}
	}
	//printf("i is %d \n",i);
	if(i == len)
		return 1;
	else
		return 0;
}


char isDouble(char *input, int len){
	
	int 	i = 0;
	int	numE = 0; 	//number of E's
	//int	len = strlen(input);
	//printf("... Checking value %s with len %d \n", input, len);
	if (input[0] != '-' && input[0] != '+' && isdigit(input[0]) == 0)
		return 0; 	

	for(i = 1; i < len; i++)
	{
		if(isdigit(input[i]) == 0 && input[i] != '.' 
		   && input[i] != 'e' && input[i] != 'E' 
		   && input[i] != '-' && input[i] != '+'){ // May also check ispunct(string[i]) != 0 
			//printf("NOT A DIGIT \n");
			break;
		}
		if (input[i] == 'e' || input[i] == 'E')
			numE++;
	}

	if (numE > 1) return 0; 

	//printf("i is %d \n",i);
	if(i == len)
		return 1;
	else
		return 0;
}

int getIntFromRDFString(str input){
	int i; 
	int ret; 
	char *tmpStr; 

	assert(input[0] == '\"');
	
	//Find the second quote
	i = 1; 
	while (input[i] != '\"'){
		i++; 
	}
	//input[i] = '\0';
	//input++;
	tmpStr = substring(input, 2, i - 1);
	//printf("INT: Input after extraction %s \n", tmpStr);	
	ret = atoi(tmpStr);
	//printf("return value: %d \n",ret); 
	GDKfree(tmpStr); 
	return ret; 
}

double getDoubleFromRDFString(str input){
	int i; 
	double ret; 
	char *tmpStr; 

	assert(input[0] == '\"');
	
	//Find the second quote
	i = 1; 
	while (input[i] != '\"'){
		i++; 
	}
	tmpStr = substring(input, 2, i - 1);
	ret = atof(tmpStr); 
	//printf("return value: %f \n",ret); 
	GDKfree(tmpStr);
	return ret; 
}

str getDateTimeFromRDFString(str input){

	int i; 
	char *tmpStr; 
	assert(input[0] == '\"');
	
	//Find the second quote
	i = 1; 
	while (input[i] != '\"'){
		i++; 
	}
	tmpStr = substring(input, 2, i - 1);

	return tmpStr; 
}

static 
char rdfcastToIntFromDouble(ValPtr srcPtr, ValPtr dstPtr){
	double srcdbl; 
	int  dstint; 
	srcdbl =  srcPtr->val.dval;
	dstint = (int) srcdbl; 
	if (dstint == srcdbl){
		VALset(dstPtr,TYPE_int, &dstint);		
		return 1; 		
	}
	else return 2; 
}



/*
 * Note: Input is a quoted string 
 * Get the first quoted part of the string 
 * e.g., "abc"@en ===>  abc
 * Check whether it can be converted to an integer or not
 * */

static 
char rdfcastToIntFromString(ValPtr srcPtr, ValPtr dstPtr){
	int i; 
	int dstint; 
	char *tmpStr; 
	int const MAX_INT_LEN = 9; 
	char *input = srcPtr->val.sval; 

	assert(input[0] == '\"');

	if (srcPtr->len < 3) return 0;
	
	if (input[1] != '-' && isdigit(input[1]) == 0)
		return 0; 	

	//Find the second quote
	i = 2; 
	while (input[i] != '\"'){
		if (isdigit(input[i]) == 0) return 0;
		i++; 
	}

	if (i > (MAX_INT_LEN + 1)) return 0;

	tmpStr = substring(input, 2, i - 1);
	//printf("INT: Input after extraction %s \n", tmpStr);	
	dstint = atoi(tmpStr);
	GDKfree(tmpStr); 
	VALset(dstPtr,TYPE_int, &dstint);
	return 1; 
}


static 
char rdfcastToDoubleFromString(ValPtr srcPtr, ValPtr dstPtr){
	int i; 
	int	numdot = 0; 
	double dstdbl; 
	char *tmpStr; 
	int const MAX_DOUBLE_LEN = 23; 
	char *input = srcPtr->val.sval; 
	
	assert(input[0] == '\"');

	if (srcPtr->len < 3) return 0;
	
	if (input[1] != '-' && isdigit(input[1]) == 0)
		return 0; 	

	//if (ispunct(input[1])) return 0; 
	if (ispunct(input[1]) == '.') return 0; 

	//Find the second quote
	i = 2; 
	while (input[i] != '\"'){
		//if (isdigit(input[i]) == 0 && !ispunct(input[i])) return 0;
		if (isdigit(input[i]) == 0 && (input[i] != '.' )) 
			return 0;

		//if (ispunct(input[i])) numdot += 1; 
		if (input[i] == '.') numdot += 1; 

		i++; 
	}
	
	if (numdot > 1)	return 0; 

	if (i > (MAX_DOUBLE_LEN + 1)) return 0;

	tmpStr = substring(input, 2, i - 1);
	dstdbl = atof(tmpStr);
	//printf("Float: Input after extraction %s ==> %f \n", tmpStr, dstdbl);	
	GDKfree(tmpStr); 
	VALset(dstPtr,TYPE_dbl, &dstdbl);
	return 1; 
}

/**
 * Castable Table
 * 	src:	URI	STRING	DATETIME	DOUBLE	INT
 * dst: 	
 * URI				
 * STRING		Y	Y		Y	Y	
 * DATETIME			
 * DOUBLE		V				Y
 * INT			V			V	
 *
 * Y: Castable
 * V: need to be verified
 * */

 /* Return: 
 * 0: Cannot cast
 * 1: Cast without changing the value
 * 2: Castable, but the dest value is not the same as the src value 
 */


char rdfcast(ObjectType srcT, ObjectType dstT, ValPtr srcPtr, ValPtr dstPtr){
	switch(dstT){
		case URI: 
			return 0; 	// Do not convert any thing to URI
		case STRING: 
			switch(srcT){
				case INTEGER:
					dstPtr->val.sval = NULL;
					(*BATatoms[TYPE_int].atomToStr)(&dstPtr->val.sval,
							&dstPtr->len, &srcPtr->val.ival);
					dstPtr->vtype = TYPE_str;
					//printf("Cast a int value %d to string value %s  len = %d \n", srcPtr->val.ival, dstPtr->val.sval, dstPtr->len); 
					return 1; 
				case DOUBLE: 					
					dstPtr->val.sval = NULL;
					(*BATatoms[TYPE_dbl].atomToStr)(&dstPtr->val.sval,
							&dstPtr->len, &srcPtr->val.dval);
					dstPtr->vtype = TYPE_str;
					return 1; 
				case DATETIME: 
					dstPtr->vtype = TYPE_str; 
					dstPtr->len = srcPtr->len; 
					dstPtr->val.sval = GDKstrdup(srcPtr->val.sval);
					return 1; 
				case STRING:	
					dstPtr->vtype = TYPE_str; 
					dstPtr->len = srcPtr->len; 
					dstPtr->val.sval = GDKstrdup(srcPtr->val.sval);
					return 1; 
				default: 
					return 0; 
			}
		case DATETIME: 
			return 0; 
		case DOUBLE: 
			switch(srcT){
				case INTEGER: 
					dstPtr->vtype = TYPE_dbl; 
					dstPtr->val.dval = (double) srcPtr->val.ival;
					return 1; 
				case STRING: 
					return rdfcastToDoubleFromString(srcPtr, dstPtr);
				default: 
					return 0; 
			}

		case INTEGER: 
			switch(srcT){
				case DOUBLE: 
					return rdfcastToIntFromDouble(srcPtr, dstPtr); 
				case STRING: 
					return rdfcastToIntFromString(srcPtr, dstPtr); 	
				default: 
					return 0;
			}
		default:
			return 0; 
			
	}	
}


void 
encodeValueInOid(ValPtr vrPtrRealValue, ObjectType objType, BUN* bun){

	int positiveInt = 0; 

	*bun = 0; 

	/* Add the type here by changing 3 bits at position 60, 61, 62 of oid */
	*bun |= (BUN)objType << (sizeof(BUN)*8 - 4);
	
	switch(objType){
		case INTEGER: 
			if (vrPtrRealValue->val.ival < 0){
				positiveInt = (-1) * vrPtrRealValue->val.ival;				
				*bun |= (BUN) 1 << (sizeof(BUN)*8 - 5);	
			}
			else {
				positiveInt = vrPtrRealValue->val.ival;
			}

			*bun |= (BUN) positiveInt;
			break;

		case DOUBLE:
			{
			double tmp = vrPtrRealValue->val.dval;
			BUN *tmpoid = (BUN *) & tmp;
			*tmpoid = *tmpoid >> 4;
			*bun |= *tmpoid; 
			}
			break;
		case DATETIME: 
			break;
		default:
			return; 

	}
}


void 
decodeValueFromOid(BUN bun, ObjectType objType, ValPtr vrPtrRealValue){
	BUN realval = 0; 
	int sign = 0; 
	int ival = 0; 
	double *realdbl = NULL; 
	
	//printf("Decode value from oid: "BUNFMT "\n",bun);
	switch(objType){
		case INTEGER: 
			sign = (int)((bun >> (sizeof(BUN)*8 - 5)) & 0x01);	//Get the sign bit
			//printf("Sign bit from "BUNFMT " is %d \n",bun, sign);
			realval = bun & (~((BUN) 0x1F << (sizeof(BUN)*8 - 5)));	//Get the real value 
			if (sign == 1) ival = 0 - realval ; 
			else	ival = realval; 
			VALset(vrPtrRealValue,TYPE_int, &ival);
			break;	
		case DOUBLE:
			realval = bun << 4; 
			realdbl = (double *) &realval;
			VALset(vrPtrRealValue,TYPE_dbl, realdbl);
			break;
		case DATETIME: 
			break; 
		default:
			printf("The objecttype %d cannot be handled\n",objType);
			break; 
	
	}
}


/*
static
char isIntWithQuote(char *input, int len){	// "123123" ==> INT
	
	int 	i;
	if (len < 3) return 0;
	for(i = 1; i < len-1; i++)
	{
		if(isdigit(input[i]) == 0){ // May also check ispunct(string[i]) != 0 
			//printf("NOT A DIGIT \n");
			break;
		}
	}
	//printf("i is %d \n",i);
	if(i == len)
		return 1;
	else
		return 0;
}
*/

