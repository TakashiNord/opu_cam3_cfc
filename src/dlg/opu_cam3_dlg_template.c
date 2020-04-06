

/*=============================================================================
   WARNING!!  This file is overwritten by the UIStyler each time the Styler 
   file is saved.
  
  
        Filename:  opu_cam3_dlg_template.c
  
        This file was generated by the NX User Interface Styler
        Created by: ChudoyakovEB68
        Version: NX 4
              Date: 01-24- 8
              Time: 10:42
  
   This template file is overwritten each time the UIStyler dialog is
   saved.  Any modifications to this file will be lost.
==============================================================================*/
 


/*==============================================================================
   Purpose:  This TEMPLATE file contains C source and static structures to      
   guide you in the construction of your NX Open application dialog.            
   The generation of your dialog file (.dlg extension) is the first step towards
   dialog construction within Unigraphics.  You must now create a UGOpen        
   application that utilizes this file (.dlg).                                  
                                                                                
   The information in this file provides you with the following:                
                                                                                
   1.  Help on the use of the functions, UF_MB_add_styler_actions and           
       UF_STYLER_create_dialog in your NX Open application.  These functions    
       will load and display your UIStyler dialog in Unigraphics.               
                                                                                
       An example of the function, UF_MB_add_styler_actions to associate your   
       dialog to the menubar is shown below (Search on Example 1).              
                                                                                
       An example of a invoking a dialog from a callback utilizing              
       UF_STYLER_create_dialog is also shown below (Search for Example 2).      
                                                                                
       An example of a user exit utilizing UF_STYLER_create_dialog is also      
       shown below (Search for Example 3).                                      
                                                                                
   2.  The callback structure: --- SL_cbs ---               
       This structure is VERY important if you have callbacks associated with   
       your dialog.  It correlates the dialog items in your dialog with the     
       callback functions you must supply.  You should not modify this          
       structure since it MUST match up to the information stored in your       
       dialog file (.dlg).  Any attempt to do so will cause an error while      
       constructing your dialog.   If you wish to modify the association of     
       your callbacks to your dialog, please reload your dialog file (.dlg) into
       the UIStyler and regenerate your files.                                  
       You do not need to be concerned about this structure, simply pass it as  
       an argument to the function, UF_STYLER_create_dialog along with your     
       dialog file (.dlg).
       
       Example 1 displays the actual call you may make for this particular      
       dialog.                                                                  
                                                                                
   3.  The empty callback functions (stubs) associated with your dialog items   
       have also been placed in this file.  These empty functions have been     
       created simply to start you along with your coding requirements.         
       The function name, argument list and possible return values have already 
       been provided for you.                                                   
                                                                                
NOTE:  Each callback must be wrappered with the functions UF_initialize()       
       and UF_terminate().                                                      
                                                                                
==============================================================================*/



/* These include files are needed for the following template code.            */
#include <stdio.h> 
#include <uf.h>
#include <uf_defs.h>
#include <uf_exit.h>
#include <uf_ui.h>
#include <uf_styler.h>
#include <uf_mb.h> 
#include <opu_cam3_dlg.h>

/* The following definition defines the number of callback entries */
/* in the callback structure:                                      */
/* UF_STYLER_callback_info_t SL_cbs */
#define SL_CB_COUNT ( 17 + 1 ) /* Add 1 for the terminator */
 
/*--------------------------------------------------------------------------
The following structure defines the callback entries used by the       
styler file.  This structure MUST be passed into the user function,    
UF_STYLER_create_dialog along with SL_CB_COUNT.                       
--------------------------------------------------------------------------*/
static UF_STYLER_callback_info_t SL_cbs[SL_CB_COUNT] = 
{
 {UF_STYLER_DIALOG_INDEX, UF_STYLER_OK_CB           , 0, SL_ok_cb},
 {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB        , 0, SL_apply_cb},
 {UF_STYLER_DIALOG_INDEX, UF_STYLER_CANCEL_CB       , 0, SL_cancel_cb},
 {SL_CORNER_CONTROL_METHOD, UF_STYLER_ACTIVATE_CB     , 0, SL_convex_corner_cb},
 {SL_CORNER_CIR_FEED_COMP, UF_STYLER_VALUE_CHANGED_CB, 0, SL_circular_feed_rate_comp_cb},
 {SL_CIR_FEED_COMP_MAXIMUM, UF_STYLER_ACTIVATE_CB     , 0, SL_real_cb},
 {SL_CIR_FEED_COMP_MINIMUM, UF_STYLER_ACTIVATE_CB     , 0, SL_real_cb},
 {SL_CORNER_FILLET_TYPE , UF_STYLER_ACTIVATE_CB     , 0, SL_fillets_cb},
 {SL_CORNER_FILLET_PATH_RADIUS, UF_STYLER_ACTIVATE_CB     , 0, SL_real_cb},
 {SL_MINIMUM_CORNER_FILLET_RADIUS, UF_STYLER_ACTIVATE_CB     , 0, SL_real_cb},
 {SL_CORNER_SLOWDOWN_STATUS, UF_STYLER_VALUE_CHANGED_CB, 0, SL_slowdowns_cb},
 {SL_CORNER_SLOWDOWN_LENGTH_METHOD, UF_STYLER_ACTIVATE_CB     , 0, SL_length_cb},
 {SL_CORNER_PERCENT_TOOL, UF_STYLER_ACTIVATE_CB     , 0, SL_real_cb},
 {SL_CORNER_TL_DIAM_PERCENT, UF_STYLER_ACTIVATE_CB     , 0, SL_real_cb},
 {SL_CORNER_NUM_STEPS   , UF_STYLER_ACTIVATE_CB     , 0, SL_real_cb},
 {SL_CORNER_MIN_ANGLE   , UF_STYLER_ACTIVATE_CB     , 0, SL_real_cb},
 {SL_CORNER_MAX_ANGLE   , UF_STYLER_ACTIVATE_CB     , 0, SL_real_cb},
 {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }
};



/*--------------------------------------------------------------------------
UF_MB_styler_actions_t contains 4 fields.  These are defined as follows:
 
Field 1 : the name of your dialog that you wish to display.
Field 2 : any client data you wish to pass to your callbacks.
Field 3 : your callback structure.
Field 4 : flag to inform menubar of your dialog location.  This flag MUST  
          match the resource set in your dialog!  Do NOT ASSUME that changing 
          this field will update the location of your dialog.  Please use the 
          UIStyler to indicate the position of your dialog.
--------------------------------------------------------------------------*/
static UF_MB_styler_actions_t actions[] = {
    { "opu_cam3_dlg.dlg",  NULL,   SL_cbs,  UF_MB_STYLER_IS_NOT_TOP },
    { NULL,  NULL,  NULL,  0 } /* This is a NULL terminated list */
};








/*-------DIALOG CREATION FROM A USER EXIT HELP Example --------
To create this dialog from a user exit, you must invoke a     
call to the NX Open API, UF_STYLER_create_dialog.  An example 
is shown below.                                               

All dialog files must be located in 
      $UGII_USER_DIR/application or 
      $UGII_SITE_DIR/application or 
      $UGII_VENDOR_DIR/application directory

1) Remove the conditional definitions:
   #ifdef DISPLAY_FROM_USER_EXIT 
   #endif DISPLAY_FROM_USER_EXIT 
2) Add a user exit to the function name below, for example, ufusr.
3) Consider how your shared library will be unloaded.  Take a look
   at the generated function ufusr_ask_unload.
--------------------------------------------------------------*/

extern void <enter a valid user exit here> (char *param, int *retcode, int rlen)
{
    int  response   = 0;
    int  error_code = 0;
 
    if ( ( UF_initialize() ) != 0 ) 
           return;

    if ( ( error_code = UF_STYLER_create_dialog ( "opu_cam3_dlg.dlg",
           SL_cbs,      /* Callbacks from dialog */
           SL_CB_COUNT, /* number of callbacks*/
           NULL,        /* This is your client data */
           &response ) ) != 0 )
    {
          char fail_message[133];

          /* Get the user function fail message based on the fail code.*/
          UF_get_fail_message(error_code, fail_message);
          UF_UI_set_status (fail_message);
          printf ( "%s\n", fail_message ); 
    }


    UF_terminate();                             
    return;
}




/*--------------------------------------------------------------------------
This function specifies how a shared image is unloaded from memory          
within Unigraphics. This function gives you the capability to unload an     
internal NX Open application or user  exit from Unigraphics.  You can       
specify any one of the three constants as a return value to determine       
the type of unload to perform:  immediately after user function             
execution, via an unload selection dialog, or when Unigraphics terminates   
terminates.  If you choose UF_UNLOAD_SEL_DIALOG, then you have the          
option to unload your image by selecting  File->Utilities->Unload Shared    
Image. 

NOTE:  A program which associates NX Open applications with the menubar     
MUST NOT use this option since it will UNLOAD your NX Open application image
--------
from the menubar.
--------------------------------------------------------------------------*/

extern int ufusr_ask_unload (void)
{
     /* unload immediately after application exits*/
     return ( UF_UNLOAD_IMMEDIATELY );

     /*via the unload selection dialog... */
     /*return ( UF_UNLOAD_SEL_DIALOG );   */
     /*when UG terminates...              */
     /*return ( UF_UNLOAD_UG_TERMINATE ); */
}



/*--------------------------------------------------------------------------
You have the option of coding the cleanup routine to perform any housekeeping
chores that may need to be performed.  If you code the cleanup routine, it is
automatically called by Unigraphics.
--------------------------------------------------------------------------*/
extern void ufusr_cleanup (void)
{
    return;
}






/*-------------------------------------------------------------------------*/
/*---------------------- UIStyler Callback Functions ----------------------*/
/*-------------------------------------------------------------------------*/

/* -------------------------------------------------------------------------
 * Callback Name: SL_ok_cb
 * This is a callback function associated with an action taken from a
 * UIStyler object. 
 *
 * Input: dialog_id   -   The dialog id indicate which dialog this callback
 *                        is associated with.  The dialog id is a dynamic,
 *                        unique id and should not be stored.  It is
 *                        strictly for the use in the NX Open API:
 *                               UF_STYLER_ask_value(s) 
 *                               UF_STYLER_set_value   
 *        client_data -   Client data is user defined data associated
 *                        with your dialog.  Client data may be bound
 *                        to your dialog with UF_MB_add_styler_actions
 *                        or UF_STYLER_create_dialog.                 
 *        callback_data - This structure pointer contains information
 *                        specific to the UIStyler Object type that  
 *                        invoked this callback and the callback type.
 * -----------------------------------------------------------------------*/
int SL_ok_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data)
{
     /* Make sure User Function is available. */  
     if ( UF_initialize() != 0) 
          return ( UF_UI_CB_CONTINUE_DIALOG );

     /* ---- Enter your callback code here ----- */

     UF_terminate ();

    /* Callback acknowledged, terminate dialog             */
    /* It is STRONGLY recommended that you exit your       */
    /* callback with UF_UI_CB_EXIT_DIALOG in a ok callback.*/
    /* return ( UF_UI_CB_EXIT_DIALOG );                    */
    return (UF_UI_CB_EXIT_DIALOG);                           

}


/* -------------------------------------------------------------------------
 * Callback Name: SL_apply_cb
 * This is a callback function associated with an action taken from a
 * UIStyler object. 
 *
 * Input: dialog_id   -   The dialog id indicate which dialog this callback
 *                        is associated with.  The dialog id is a dynamic,
 *                        unique id and should not be stored.  It is
 *                        strictly for the use in the NX Open API:
 *                               UF_STYLER_ask_value(s) 
 *                               UF_STYLER_set_value   
 *        client_data -   Client data is user defined data associated
 *                        with your dialog.  Client data may be bound
 *                        to your dialog with UF_MB_add_styler_actions
 *                        or UF_STYLER_create_dialog.                 
 *        callback_data - This structure pointer contains information
 *                        specific to the UIStyler Object type that  
 *                        invoked this callback and the callback type.
 * -----------------------------------------------------------------------*/
int SL_apply_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data)
{
     /* Make sure User Function is available. */  
     if ( UF_initialize() != 0) 
          return ( UF_UI_CB_CONTINUE_DIALOG );

     /* ---- Enter your callback code here ----- */

     UF_terminate ();

    /* Callback acknowledged, do not terminate dialog                 */
    /* A return value of UF_UI_CB_EXIT_DIALOG will not be accepted    */
    /* for this callback type.  You must respond to your apply button.*/
    return (UF_UI_CB_CONTINUE_DIALOG); 

}


/* -------------------------------------------------------------------------
 * Callback Name: SL_cancel_cb
 * This is a callback function associated with an action taken from a
 * UIStyler object. 
 *
 * Input: dialog_id   -   The dialog id indicate which dialog this callback
 *                        is associated with.  The dialog id is a dynamic,
 *                        unique id and should not be stored.  It is
 *                        strictly for the use in the NX Open API:
 *                               UF_STYLER_ask_value(s) 
 *                               UF_STYLER_set_value   
 *        client_data -   Client data is user defined data associated
 *                        with your dialog.  Client data may be bound
 *                        to your dialog with UF_MB_add_styler_actions
 *                        or UF_STYLER_create_dialog.                 
 *        callback_data - This structure pointer contains information
 *                        specific to the UIStyler Object type that  
 *                        invoked this callback and the callback type.
 * -----------------------------------------------------------------------*/
int SL_cancel_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data)
{
     /* Make sure User Function is available. */  
     if ( UF_initialize() != 0) 
          return ( UF_UI_CB_CONTINUE_DIALOG );

     /* ---- Enter your callback code here ----- */

     UF_terminate ();

    /* Callback acknowledged, terminate dialog             */
    /* It is STRONGLY recommended that you exit your       */
    /* callback with UF_UI_CB_EXIT_DIALOG in a cancel call */
    /* back rather than UF_UI_CB_CONTINUE_DIALOG.          */
    return ( UF_UI_CB_EXIT_DIALOG );                       

}


/* -------------------------------------------------------------------------
 * Callback Name: SL_convex_corner_cb
 * This is a callback function associated with an action taken from a
 * UIStyler object. 
 *
 * Input: dialog_id   -   The dialog id indicate which dialog this callback
 *                        is associated with.  The dialog id is a dynamic,
 *                        unique id and should not be stored.  It is
 *                        strictly for the use in the NX Open API:
 *                               UF_STYLER_ask_value(s) 
 *                               UF_STYLER_set_value   
 *        client_data -   Client data is user defined data associated
 *                        with your dialog.  Client data may be bound
 *                        to your dialog with UF_MB_add_styler_actions
 *                        or UF_STYLER_create_dialog.                 
 *        callback_data - This structure pointer contains information
 *                        specific to the UIStyler Object type that  
 *                        invoked this callback and the callback type.
 * -----------------------------------------------------------------------*/
int SL_convex_corner_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data)
{
     /* Make sure User Function is available. */  
     if ( UF_initialize() != 0) 
          return ( UF_UI_CB_CONTINUE_DIALOG );

     /* ---- Enter your callback code here ----- */

     UF_terminate ();

    /* Callback acknowledged, do not terminate dialog */
    return (UF_UI_CB_CONTINUE_DIALOG); 
    
    /* or Callback acknowledged, terminate dialog.    */
    /* return ( UF_UI_CB_EXIT_DIALOG );               */

}


/* -------------------------------------------------------------------------
 * Callback Name: SL_circular_feed_rate_comp_cb
 * This is a callback function associated with an action taken from a
 * UIStyler object. 
 *
 * Input: dialog_id   -   The dialog id indicate which dialog this callback
 *                        is associated with.  The dialog id is a dynamic,
 *                        unique id and should not be stored.  It is
 *                        strictly for the use in the NX Open API:
 *                               UF_STYLER_ask_value(s) 
 *                               UF_STYLER_set_value   
 *        client_data -   Client data is user defined data associated
 *                        with your dialog.  Client data may be bound
 *                        to your dialog with UF_MB_add_styler_actions
 *                        or UF_STYLER_create_dialog.                 
 *        callback_data - This structure pointer contains information
 *                        specific to the UIStyler Object type that  
 *                        invoked this callback and the callback type.
 * -----------------------------------------------------------------------*/
int SL_circular_feed_rate_comp_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data)
{
     /* Make sure User Function is available. */  
     if ( UF_initialize() != 0) 
          return ( UF_UI_CB_CONTINUE_DIALOG );

     /* ---- Enter your callback code here ----- */

     UF_terminate ();

    /* Callback acknowledged, do not terminate dialog */
    return (UF_UI_CB_CONTINUE_DIALOG); 
    
    /* or Callback acknowledged, terminate dialog.  */
    /* return ( UF_UI_CB_EXIT_DIALOG );             */

}


/* -------------------------------------------------------------------------
 * Callback Name: SL_real_cb
 * This is a callback function associated with an action taken from a
 * UIStyler object. 
 *
 * Input: dialog_id   -   The dialog id indicate which dialog this callback
 *                        is associated with.  The dialog id is a dynamic,
 *                        unique id and should not be stored.  It is
 *                        strictly for the use in the NX Open API:
 *                               UF_STYLER_ask_value(s) 
 *                               UF_STYLER_set_value   
 *        client_data -   Client data is user defined data associated
 *                        with your dialog.  Client data may be bound
 *                        to your dialog with UF_MB_add_styler_actions
 *                        or UF_STYLER_create_dialog.                 
 *        callback_data - This structure pointer contains information
 *                        specific to the UIStyler Object type that  
 *                        invoked this callback and the callback type.
 * -----------------------------------------------------------------------*/
int SL_real_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data)
{
     /* Make sure User Function is available. */  
     if ( UF_initialize() != 0) 
          return ( UF_UI_CB_CONTINUE_DIALOG );

     /* ---- Enter your callback code here ----- */

     UF_terminate ();

    /* Callback acknowledged, do not terminate dialog */
    return (UF_UI_CB_CONTINUE_DIALOG); 
    
    /* or Callback acknowledged, terminate dialog.    */
    /* return ( UF_UI_CB_EXIT_DIALOG );               */

}


/* -------------------------------------------------------------------------
 * Callback Name: SL_fillets_cb
 * This is a callback function associated with an action taken from a
 * UIStyler object. 
 *
 * Input: dialog_id   -   The dialog id indicate which dialog this callback
 *                        is associated with.  The dialog id is a dynamic,
 *                        unique id and should not be stored.  It is
 *                        strictly for the use in the NX Open API:
 *                               UF_STYLER_ask_value(s) 
 *                               UF_STYLER_set_value   
 *        client_data -   Client data is user defined data associated
 *                        with your dialog.  Client data may be bound
 *                        to your dialog with UF_MB_add_styler_actions
 *                        or UF_STYLER_create_dialog.                 
 *        callback_data - This structure pointer contains information
 *                        specific to the UIStyler Object type that  
 *                        invoked this callback and the callback type.
 * -----------------------------------------------------------------------*/
int SL_fillets_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data)
{
     /* Make sure User Function is available. */  
     if ( UF_initialize() != 0) 
          return ( UF_UI_CB_CONTINUE_DIALOG );

     /* ---- Enter your callback code here ----- */

     UF_terminate ();

    /* Callback acknowledged, do not terminate dialog */
    return (UF_UI_CB_CONTINUE_DIALOG); 
    
    /* or Callback acknowledged, terminate dialog.    */
    /* return ( UF_UI_CB_EXIT_DIALOG );               */

}


/* -------------------------------------------------------------------------
 * Callback Name: SL_slowdowns_cb
 * This is a callback function associated with an action taken from a
 * UIStyler object. 
 *
 * Input: dialog_id   -   The dialog id indicate which dialog this callback
 *                        is associated with.  The dialog id is a dynamic,
 *                        unique id and should not be stored.  It is
 *                        strictly for the use in the NX Open API:
 *                               UF_STYLER_ask_value(s) 
 *                               UF_STYLER_set_value   
 *        client_data -   Client data is user defined data associated
 *                        with your dialog.  Client data may be bound
 *                        to your dialog with UF_MB_add_styler_actions
 *                        or UF_STYLER_create_dialog.                 
 *        callback_data - This structure pointer contains information
 *                        specific to the UIStyler Object type that  
 *                        invoked this callback and the callback type.
 * -----------------------------------------------------------------------*/
int SL_slowdowns_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data)
{
     /* Make sure User Function is available. */  
     if ( UF_initialize() != 0) 
          return ( UF_UI_CB_CONTINUE_DIALOG );

     /* ---- Enter your callback code here ----- */

     UF_terminate ();

    /* Callback acknowledged, do not terminate dialog */
    return (UF_UI_CB_CONTINUE_DIALOG); 
    
    /* or Callback acknowledged, terminate dialog.  */
    /* return ( UF_UI_CB_EXIT_DIALOG );             */

}


/* -------------------------------------------------------------------------
 * Callback Name: SL_length_cb
 * This is a callback function associated with an action taken from a
 * UIStyler object. 
 *
 * Input: dialog_id   -   The dialog id indicate which dialog this callback
 *                        is associated with.  The dialog id is a dynamic,
 *                        unique id and should not be stored.  It is
 *                        strictly for the use in the NX Open API:
 *                               UF_STYLER_ask_value(s) 
 *                               UF_STYLER_set_value   
 *        client_data -   Client data is user defined data associated
 *                        with your dialog.  Client data may be bound
 *                        to your dialog with UF_MB_add_styler_actions
 *                        or UF_STYLER_create_dialog.                 
 *        callback_data - This structure pointer contains information
 *                        specific to the UIStyler Object type that  
 *                        invoked this callback and the callback type.
 * -----------------------------------------------------------------------*/
int SL_length_cb ( int dialog_id,
             void * client_data,
             UF_STYLER_item_value_type_p_t callback_data)
{
     /* Make sure User Function is available. */  
     if ( UF_initialize() != 0) 
          return ( UF_UI_CB_CONTINUE_DIALOG );

     /* ---- Enter your callback code here ----- */

     UF_terminate ();

    /* Callback acknowledged, do not terminate dialog */
    return (UF_UI_CB_CONTINUE_DIALOG); 
    
    /* or Callback acknowledged, terminate dialog.    */
    /* return ( UF_UI_CB_EXIT_DIALOG );               */

}


