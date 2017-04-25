/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0.  If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 1997 - July 2008 CWI, August 2008 - 2016 MonetDB B.V.
 */

#ifndef MAL_PRELUDE
#define MAL_PRELUDE
#include "opt_support.h"

/* cf., gdk/gdk.mx */
#define DEBUGoptimizers     if (GDKdebug & GRPoptimizers)

mal_export  str abortRef;
mal_export  str affectedRowsRef;
mal_export  str aggrRef;
mal_export  str alarmRef;
mal_export  str algebraRef;
mal_export	str andRef;
mal_export  str appendidxRef;
mal_export  str appendRef;
mal_export  str assertRef;
mal_export  str attachRef;
mal_export  str alter_seqRef;
mal_export  str alter_schemaRef;
mal_export  str alter_viewRef;
mal_export  str alter_tableRef;
mal_export  str alter_constraintRef;
mal_export  str alter_typeRef;
mal_export  str alter_userRef;
mal_export  str alter_roleRef;
mal_export  str alter_userRef;
mal_export  str alter_indexRef;
mal_export  str alter_functionRef;
mal_export  str alter_triggerRef;
mal_export  str alter_add_tableRef;
mal_export  str alter_del_tableRef;
mal_export  str alter_set_tableRef;
mal_export  str avgRef;
mal_export  str arrayRef;
mal_export  str batRef;
mal_export  str batalgebraRef;
mal_export  str basketRef;
mal_export  str batcalcRef;
mal_export  str batstrRef;
mal_export  str batmtimeRef;
mal_export  str batmmathRef;
mal_export  str batxmlRef;
mal_export  str batsqlRef;
mal_export  str betweenRef;
mal_export  str betweensymmetricRef;
mal_export  str blockRef;
mal_export  str bbpRef;
mal_export  str tidRef;
mal_export  str dateRef;
mal_export  str deltaRef;
mal_export  str subdeltaRef;
mal_export  str projectdeltaRef;
mal_export  str binddbatRef;
mal_export  str bindidxRef;
mal_export  str bindRef;
mal_export  str emptybindRef;
mal_export	str emptybindidxRef;
mal_export  str bpmRef;
mal_export  str bstreamRef;
mal_export  str calcRef;
mal_export  str clear_tableRef;
mal_export  str closeRef;
mal_export  str columnRef;
mal_export  str commitRef;
mal_export  str columnBindRef;
mal_export  str connectRef;
mal_export  str countRef;
mal_export  str subcountRef;
mal_export  str copyRef;
mal_export  str copy_fromRef;
mal_export  str export_tableRef;
mal_export  str count_no_nilRef;
mal_export  str crossRef;
mal_export  str createRef;
mal_export  str create_seqRef;
mal_export  str create_schemaRef;
mal_export  str create_viewRef;
mal_export  str create_tableRef;
mal_export  str create_constraintRef;
mal_export  str create_typeRef;
mal_export  str create_userRef;
mal_export  str create_roleRef;
mal_export  str create_userRef;
mal_export  str create_indexRef;
mal_export  str create_functionRef;
mal_export  str create_triggerRef;
mal_export  str dataflowRef;
mal_export  str dblRef;
mal_export  str defineRef;
mal_export  str deleteRef;
mal_export  str differenceRef;
mal_export  str intersectRef;
mal_export  str drop_seqRef;
mal_export  str drop_schemaRef;
mal_export  str drop_viewRef;
mal_export  str drop_tableRef;
mal_export  str drop_constraintRef;
mal_export  str drop_typeRef;
mal_export  str drop_userRef;
mal_export  str drop_roleRef;
mal_export  str drop_userRef;
mal_export  str drop_indexRef;
mal_export  str drop_functionRef;
mal_export  str drop_triggerRef;
mal_export  str subdiffRef;
mal_export  str diffRef;
mal_export  str subinterRef;
mal_export  str mergecandRef;
mal_export  str mergepackRef;
mal_export  str intersectcandRef;
mal_export  str eqRef;
mal_export  str disconnectRef;
mal_export  str evalRef;
mal_export  str execRef;
mal_export  str expandRef;
mal_export  str errorRef;
mal_export  str exportOperationRef;
mal_export  str findRef;
mal_export  str finishRef;
mal_export  str firstnRef;
mal_export  str getRef;
mal_export  str getTraceRef;
mal_export  str generatorRef;
mal_export  str grantRef;
mal_export  str grant_rolesRef;
mal_export  str grant_functionRef;
mal_export  str groupRef;
mal_export  str groupdoneRef;
mal_export  str subgroupRef;
mal_export  str subgroupdoneRef;
mal_export  str groupbyRef;
mal_export  str hgeRef;
mal_export  str hashRef;
mal_export  str identityRef;
mal_export  str ifthenelseRef;
mal_export  str inplaceRef;
mal_export  str intRef;
mal_export  str ioRef;
mal_export  str iteratorRef;
mal_export  str projectionpathRef;
mal_export  str jitRef;
mal_export  str jsonRef;
mal_export  str joinRef;
mal_export  str semijoinRef;
mal_export  str leftjoinRef;
mal_export  str antijoinRef;
mal_export  str bandjoinRef;
mal_export  str rangejoinRef;
mal_export  str thetajoinRef;
mal_export  str languageRef;
mal_export  str projectionRef;
mal_export  str likeselectRef;
mal_export  str ilikeselectRef;
mal_export  str likeuselectRef;
mal_export  str ilikeuselectRef;
mal_export  str listRef;
mal_export  str lockRef;
mal_export  str lookupRef;
mal_export  str malRef;
mal_export  str batmalRef;
mal_export  str mapiRef;
mal_export  str markRef;
mal_export  str mtimeRef;
mal_export  str matRef;
mal_export  str max_no_nilRef;
mal_export  str maxRef;
mal_export  str submaxRef;
mal_export  str submedianRef;
mal_export  str mdbRef;
mal_export  str min_no_nilRef;
mal_export  str minRef;
mal_export  str subminRef;
mal_export  str mirrorRef;
mal_export  str mitosisRef;
mal_export  str mkeyRef;
mal_export  str mmathRef;
mal_export  str multiplexRef;
mal_export  str manifoldRef;
mal_export  str multicolumnRef;
mal_export  str mvcRef;
mal_export  str newRef;
mal_export  str notRef;
mal_export  str nextRef;
mal_export  str oidRef;
mal_export  str oltpRef;
mal_export  str openRef;
mal_export  str optimizerRef;
mal_export  str parametersRef;
mal_export  str packRef;
mal_export  str pack2Ref;
mal_export  str passRef;
mal_export  str partitionRef;
mal_export  str pcreRef;
mal_export  str pinRef;
mal_export  str plusRef;
mal_export  str minusRef;
mal_export  str mulRef;
mal_export  str divRef;
mal_export  str printRef;
mal_export  str preludeRef;
mal_export  str prodRef;
mal_export  str subprodRef;
mal_export  str postludeRef;
mal_export  str profilerRef;
mal_export  str projectRef;
mal_export  str putRef;
mal_export  str querylogRef;
mal_export  str queryRef;
mal_export  str rapiRef;
mal_export  str batrapiRef;
mal_export  str pyapiRef;
mal_export  str batpyapiRef;
mal_export  str pyapimapRef;
mal_export  str pyapi3Ref;
mal_export  str batpyapi3Ref;
mal_export  str pyapi3mapRef;
mal_export  str subeval_aggrRef;
mal_export  str rankRef;
mal_export  str dense_rankRef;
mal_export	str raiseRef;
mal_export  str reconnectRef;
mal_export  str refineRef;
mal_export  str registerRef;
mal_export  str releaseRef;
mal_export  str remapRef;
mal_export  str remoteRef;
mal_export  str rename_userRef;
mal_export  str replaceRef;
mal_export  str replicatorRef;
mal_export  str resultSetRef;
mal_export  str reuseRef;
mal_export  str revoke_rolesRef;
mal_export  str revoke_functionRef;
mal_export  str revokeRef;
mal_export  str row_numberRef;
mal_export  str rpcRef;
mal_export  str rsColumnRef;
mal_export  str schedulerRef;
mal_export  str selectNotNilRef;
mal_export  str seriesRef;
mal_export  str semaRef;
mal_export  str setAccessRef;
mal_export  str setVariableRef;
mal_export  str setWriteModeRef;
mal_export  str sinkRef;
mal_export  str sliceRef;
mal_export  str subsliceRef;
mal_export  str singleRef;
mal_export  str sortRef;
mal_export  str sortReverseRef;
mal_export  str sqlRef;
mal_export  str sqlcatalogRef;
mal_export  str streamsRef;
mal_export  str startRef;
mal_export  str starttraceRef;
mal_export  str stopRef;
mal_export  str stoptraceRef;
mal_export  str strRef;
mal_export  str sumRef;
mal_export  str subsumRef;
mal_export  str subavgRef;
mal_export  str sortRef;
mal_export  str cqueryRef;
mal_export  str timestampRef;
mal_export	str tumbleRef;
mal_export  str takeRef;
mal_export  str transactionRef;
mal_export	str transaction_beginRef;
mal_export	str transaction_releaseRef;
mal_export	str transaction_commitRef;
mal_export	str transaction_abortRef;
mal_export	str transaction_rollbackRef;
mal_export  str not_uniqueRef;
mal_export  str sampleRef;
mal_export  str uniqueRef;
mal_export  str subuniformRef;
mal_export  str unpackRef;
mal_export  str unpinRef;
mal_export  str unlockRef;
mal_export  str updateRef;
mal_export  str selectRef;
mal_export  str thetaselectRef;
mal_export  str likeselectRef;
mal_export  str likethetaselectRef;
mal_export  str ilikeselectRef;
mal_export  str ilikethetaselectRef;
mal_export  str likeRef;
mal_export  str ilikeRef;
mal_export  str not_likeRef;
mal_export  str not_ilikeRef;
mal_export  str userRef;
mal_export  str vectorRef;
mal_export  str zero_or_oneRef;
#endif
