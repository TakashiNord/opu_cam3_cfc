/*=============================================================================
   WARNING!!  This file is overwritten by the UIStyler each time the Styler 
   file is saved.
  
  
        Filename:  opu_cam3_dlg.h
  
        This file was generated by the NX User Interface Styler
        Created by: ChudoyakovEB68
        Version: NX 4
              Date: 01-24- 8
              Time: 10:42
  
   This include file is overwritten each time the UIStyler dialog is
   saved.  Any modifications to this file will be lost.
==============================================================================*/
 
 
#ifndef OPU_CAM3_DLG_H_INCLUDED
#define OPU_CAM3_DLG_H_INCLUDED
 
#include <uf.h> 
#include <uf_defs.h>
#include <uf_styler.h> 


#ifdef __cplusplus
extern "C" {
#endif


/*------------------ UIStyler Dialog Definitions  ------------------- */
/* The following values are definitions into your UIStyler dialog.    */
/* These values will allow you to modify existing objects within your */
/* dialog.   They work directly with the NX Open API,                 */
/* UF_STYLER_ask_value, UF_STYLER_ask_values, and UF_STYLER_set_value.*/
/*------------------------------------------------------------------- */
 
#define SL_CORNER_CONTROL_METHOD       ("CORNER_CONTROL_METHOD")
#define SL_SEP_1                       ("SEP_1")
#define SL_CORNER_CIR_FEED_COMP        ("CORNER_CIR_FEED_COMP")
#define SL_CIR_FEED_COMP               ("CIR_FEED_COMP")
#define SL_CIR_FEED_COMP_MAXIMUM       ("CIR_FEED_COMP_MAXIMUM")
#define SL_CIR_FEED_COMP_MINIMUM       ("CIR_FEED_COMP_MINIMUM")
#define SL_SEP_6                       ("SEP_6")
#define SL_CORNER_FILLET_TYPE          ("CORNER_FILLET_TYPE")
#define SL_CORNER_FILLET_PATH_RADIUS   ("CORNER_FILLET_PATH_RADIUS")
#define SL_MINIMUM_CORNER_FILLET_RADIUS ("MINIMUM_CORNER_FILLET_RADIUS")
#define SL_SEP_10                      ("SEP_10")
#define SL_CORNER_SLOWDOWN_STATUS      ("CORNER_SLOWDOWN_STATUS")
#define SL_CORNER_SLOWDOWN_LENGTH_METHOD ("CORNER_SLOWDOWN_LENGTH_METHOD")
#define SL_CORNER_PERCENT_TOOL         ("CORNER_PERCENT_TOOL")
#define SL_CORNER_TL_DIAM_PERCENT      ("CORNER_TL_DIAM_PERCENT")
#define SL_CORNER_NUM_STEPS            ("CORNER_NUM_STEPS")
#define SL_SEP_16                      ("SEP_16")
#define SL_LAB_CORNER_ANGLE            ("LAB_CORNER_ANGLE")
#define SL_CORNER_MIN_ANGLE            ("CORNER_MIN_ANGLE")
#define SL_CORNER_MAX_ANGLE            ("CORNER_MAX_ANGLE")
#define SL_DIALOG_OBJECT_COUNT         ( 20 )
 

/*---------------- UIStyler Callback Prototypes --------------- */
/* The following function prototypes define the callbacks       */
/* specified in your UIStyler built dialog.  You are REQUIRED to*/
/* create the associated function for each prototype.  You must */
/* use the same function name and parameter list when creating  */
/* your callback function.                                      */
/*------------------------------------------------------------- */

int SL_ok_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data);

int SL_apply_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data);

int SL_cancel_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data);

int SL_convex_corner_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data);

int SL_circular_feed_rate_comp_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data);

int SL_real_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data);

int SL_fillets_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data);

int SL_slowdowns_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data);

int SL_length_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data);




#ifdef __cplusplus
}
#endif



#endif /* OPU_CAM3_DLG_H_INCLUDED */