/********************************************************
 *
 * Structure for ext_data_fetch_event() filling.
 *
 * Do not edit - automatically generated.
 */

#ifndef __GUARD_H101_SOFCORRM_EXT_H101_SOFCORRV_H__
#define __GUARD_H101_SOFCORRM_EXT_H101_SOFCORRV_H__

#ifndef __CINT__
# include <stdint.h>
#else
/* For CINT (old version trouble with stdint.h): */
# ifndef uint32_t
typedef unsigned int uint32_t;
typedef          int  int32_t;
# endif
#endif
#ifndef EXT_STRUCT_CTRL
# define EXT_STRUCT_CTRL(x)
#endif

/********************************************************
 *
 * Plain structure (layout as ntuple/root file):
 */

typedef struct EXT_STR_h101_sofcorrm_t
{
  /* RAW */
  uint32_t SOFCORRV_TRM /* [1,1] */;
  uint32_t SOFCORRV_TRMI[1 EXT_STRUCT_CTRL(SOFCORRV_TRM)] /* [1,1] */;
  uint32_t SOFCORRV_TRME[1 EXT_STRUCT_CTRL(SOFCORRV_TRM)] /* [1,100] */;
  uint32_t SOFCORRV_TR /* [0,100] */;
  uint32_t SOFCORRV_TRv[100 EXT_STRUCT_CTRL(SOFCORRV_TR)] /* [0,65535] */;
  uint32_t SOFCORRV_TTM /* [1,1] */;
  uint32_t SOFCORRV_TTMI[1 EXT_STRUCT_CTRL(SOFCORRV_TTM)] /* [1,1] */;
  uint32_t SOFCORRV_TTME[1 EXT_STRUCT_CTRL(SOFCORRV_TTM)] /* [1,100] */;
  uint32_t SOFCORRV_TT /* [0,100] */;
  uint32_t SOFCORRV_TTv[100 EXT_STRUCT_CTRL(SOFCORRV_TT)] /* [0,65535] */;

} EXT_STR_h101_sofcorrm;

/********************************************************
 *
 * Structure with multiple levels of arrays (partially)
 * recovered (recommended):
 */

typedef struct EXT_STR_h101_sofcorrm_onion_t
{
  /* RAW */
  uint32_t SOFCORRV_TRM;
  uint32_t SOFCORRV_TRMI[1 /* SOFCORRV_TRM */];
  uint32_t SOFCORRV_TRME[1 /* SOFCORRV_TRM */];
  uint32_t SOFCORRV_TR;
  uint32_t SOFCORRV_TRv[100 /* SOFCORRV_TR */];
  uint32_t SOFCORRV_TTM;
  uint32_t SOFCORRV_TTMI[1 /* SOFCORRV_TTM */];
  uint32_t SOFCORRV_TTME[1 /* SOFCORRV_TTM */];
  uint32_t SOFCORRV_TT;
  uint32_t SOFCORRV_TTv[100 /* SOFCORRV_TT */];

} EXT_STR_h101_sofcorrm_onion;

/*******************************************************/

#define EXT_STR_h101_sofcorrm_ITEMS_INFO(ok,si,offset,struct_t,printerr) do { \
  ok = 1; \
  /* RAW */ \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRM,                    UINT32,\
                    "SOFCORRV_TRM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRMI,                   UINT32,\
                    "SOFCORRV_TRMI",                   "SOFCORRV_TRM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRME,                   UINT32,\
                    "SOFCORRV_TRME",                   "SOFCORRV_TRM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TR,                     UINT32,\
                    "SOFCORRV_TR",100); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TRv,                    UINT32,\
                    "SOFCORRV_TRv",                    "SOFCORRV_TR"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TTM,                    UINT32,\
                    "SOFCORRV_TTM",1); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TTMI,                   UINT32,\
                    "SOFCORRV_TTMI",                   "SOFCORRV_TTM"); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TTME,                   UINT32,\
                    "SOFCORRV_TTME",                   "SOFCORRV_TTM"); \
  EXT_STR_ITEM_INFO_LIM(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TT,                     UINT32,\
                    "SOFCORRV_TT",100); \
  EXT_STR_ITEM_INFO_ZZP(ok,si,offset,struct_t,printerr,\
                     SOFCORRV_TTv,                    UINT32,\
                    "SOFCORRV_TTv",                    "SOFCORRV_TT"); \
  \
} while (0);

#endif/*__GUARD_H101_SOFCORRM_EXT_H101_SOFCORRV_H__*/

/*******************************************************/
