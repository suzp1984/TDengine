/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TD_QUERY_H_
#define _TD_QUERY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tarray.h"
#include "thash.h"
#include "tlog.h"

typedef SVgroupListRspMsg SVgroupListInfo;

typedef struct STableComInfo {
  uint8_t numOfTags;      // the number of tags in schema
  uint8_t precision;      // the number of precision
  int16_t numOfColumns;   // the number of columns
  int32_t rowSize;        // row size of the schema
} STableComInfo;

/*
 * ASSERT(sizeof(SCTableMeta) == 24)
 * ASSERT(tableType == TSDB_CHILD_TABLE)
 * The cached child table meta info. For each child table, 24 bytes are required to keep the essential table info.
 */
typedef struct SCTableMeta {
  int32_t  vgId:24;
  int8_t   tableType;
  uint64_t uid;
  uint64_t suid;
} SCTableMeta;

/*
 * Note that the first 24 bytes of STableMeta are identical to SCTableMeta, it is safe to cast a STableMeta to be a SCTableMeta.
 */
typedef struct STableMeta {
  //BEGIN: KEEP THIS PART SAME WITH SCTableMeta
  int32_t        vgId:24;
  int8_t         tableType;
  uint64_t       uid;
  uint64_t       suid;
  //END: KEEP THIS PART SAME WITH SCTableMeta
  
  // if the table is TSDB_CHILD_TABLE, the following information is acquired from the corresponding super table meta info
  int16_t        sversion;
  int16_t        tversion;
  STableComInfo  tableInfo;
  SSchema        schema[];
} STableMeta;


typedef struct SDBVgroupInfo {
  int32_t   vgVersion;  
  int8_t    hashMethod;
  SHashObj *vgInfo;  //key:vgId, value:SVgroupInfo
} SDBVgroupInfo;

typedef struct SUseDbOutput {
  char db[TSDB_FULL_DB_NAME_LEN];
  SDBVgroupInfo dbVgroup;
} SUseDbOutput;

typedef struct STableMetaOutput {
  int32_t     metaNum;
  char        ctbFname[TSDB_TABLE_FNAME_LEN];
  char        tbFname[TSDB_TABLE_FNAME_LEN];  
  SCTableMeta ctbMeta;
  STableMeta *tbMeta;
} STableMetaOutput;

extern int32_t (*queryBuildMsg[TSDB_MSG_TYPE_MAX])(void* input, char **msg, int32_t msgSize, int32_t *msgLen);
extern int32_t (*queryProcessMsgRsp[TSDB_MSG_TYPE_MAX])(void* output, char *msg, int32_t msgSize);

extern void msgInit();


extern int32_t qDebugFlag;

#define qFatal(...)  do { if (qDebugFlag & DEBUG_FATAL) { taosPrintLog("QRY FATAL ", qDebugFlag, __VA_ARGS__); }} while(0)
#define qError(...)  do { if (qDebugFlag & DEBUG_ERROR) { taosPrintLog("QRY ERROR ", qDebugFlag, __VA_ARGS__); }} while(0)
#define qWarn(...)   do { if (qDebugFlag & DEBUG_WARN)  { taosPrintLog("QRY WARN ", qDebugFlag, __VA_ARGS__); }}  while(0)
#define qInfo(...)   do { if (qDebugFlag & DEBUG_INFO)  { taosPrintLog("QRY ", qDebugFlag, __VA_ARGS__); }} while(0)
#define qDebug(...)  do { if (qDebugFlag & DEBUG_DEBUG) { taosPrintLog("QRY ", qDebugFlag, __VA_ARGS__); }} while(0)
#define qTrace(...)  do { if (qDebugFlag & DEBUG_TRACE) { taosPrintLog("QRY ", qDebugFlag, __VA_ARGS__); }} while(0)
#define qDebugL(...) do { if (qDebugFlag & DEBUG_DEBUG) { taosPrintLongString("QRY ", qDebugFlag, __VA_ARGS__); }} while(0)


#ifdef __cplusplus
}
#endif

#endif /*_TD_QUERY_H_*/
