//////////////////////////////////////////////////////////////////////////////
//
//  opu_cam3_dlg.cpp
//
//  Description:
//      Contains Unigraphics entry points for the application.
//
//////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE 1

//  Include files
#include <uf.h>
#include <uf_defs.h>
#include <uf_exit.h>
#include <uf_ui.h>
#include <uf_styler.h>
#include <uf_mb.h>

#include <uf_obj.h>
#include <uf_ui_ont.h>
#include <uf_ncgroup.h>
#include <uf_oper.h>
#include <uf_part.h>
#include <uf_param.h>
#include <uf_param_indices.h>
#include <uf_ugopenint.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

/*
#if ! defined ( __hp9000s800 ) && ! defined ( __sgi ) && ! defined ( __sun )
# include <strstream>
  using std::ostrstream;
  using std::endl;
  using std::ends;
#else
# include <strstream.h>
#endif
#include <iostream.h>
*/

#include "opu_cam3_dlg.h"


#define UF_CALL(X) (report( __FILE__, __LINE__, #X, (X)))

static int report( char *file, int line, char *call, int irc)
{
  if (irc)
  {
     char    messg[133];
     printf("%s, line %d:  %s\n", file, line, call);
     (UF_get_fail_message(irc, messg)) ?
       printf("    returned a %d\n", irc) :
       printf("    returned error %d:  %s\n", irc, messg);
  }
  return(irc);
}


#define COUNT_PRG 500

struct PRG
{
   int     num;
   tag_t   tag;
   char    name[UF_OPER_MAX_NAME_LEN+1];
} ;

static struct PRG prg_list[COUNT_PRG] ;
int prg_list_count=0;

static logical cycleGeneratePrg( tag_t   tag, void   *data );

/* */
/*----------------------------------------------------------------------------*/
static logical cycleGeneratePrg( tag_t   tag, void   *data )
{
   char     name[UF_OPER_MAX_NAME_LEN + 1];
   int      ecode;

   name[0]='\0';
   ecode = UF_OBJ_ask_name(tag, name);// спросим имя обьекта
   //UF_UI_write_listing_window("\n");  UF_UI_write_listing_window(name);

   if (prg_list_count>=COUNT_PRG) {
     uc1601("Число Операций-превышает допустимое (>500)\n Уменьшите количество выбора",1);
     return( FALSE );
   }
   prg_list[prg_list_count].num=prg_list_count;
   prg_list[prg_list_count].tag=tag;
   prg_list[prg_list_count].name[0]='\0';
   sprintf(prg_list[prg_list_count].name,"%s",name);
   prg_list_count++;

   return( TRUE );
}

int do_cam3();
int cam3_ask(tag_t ,double [14]);
int cam3_set(tag_t ,double [14]);

/*****************************************************************************************/

int do_cam3()
{
/*  Variable Declarations */
    char str[133];
    char menu[14][16] ; double  da[14];
    int  ia4[14];
    double ra5[14];
    char ca6[14 ][ 31 ]={"","","","","","","","","","","","","",""};
    int  ip7[14];

    tag_t       prg = NULL_TAG;
    int i , count = 0 ;
    int   obj_count = 0;
    tag_t *tags = NULL ;
    char  prog_name[UF_OPER_MAX_NAME_LEN+1];
    int type, subtype ;
    logical generated;
    int generat;
    int response ;
    char *mes1[]={
      "Программа предназначена для изменения параметров торможения в углах.",
      "  ",
      "  Параметры с буквой (L) - являются списком (0,1,2,3,...)",
      "  Параметры с буквой (F) - список из значений (1,0)=(on,off)",
      "  Параметры без букв () - вещественные или целые значения)",
      "  ",
      "Вы должны :",
      "  1) выбрать необходимые операции и нажать кнопку 'Далее..'",
      "  2) в появившемся окне установить необходимые значения ",
      "     если значения совпадают (до и после)-изменение параметра не происходит",
      NULL
    };
    UF_UI_message_buttons_t buttons1 = { TRUE, FALSE, TRUE, "Далее....", NULL, "Отмена", 1, 0, 2 };
    char *mes2[]={
      "Производить генерацию операции после изменения?",
      NULL
    };
    UF_UI_message_buttons_t buttons2 = { TRUE, FALSE, TRUE, "Генерировать..", NULL, "Нет", 1, 0, 2 };

    response=0;
    UF_UI_message_dialog("Cam.....", UF_UI_MESSAGE_INFORMATION, mes1, 10, TRUE, &buttons1, &response);
    if (response!=1) { return (0) ; }

    int module_id;
    UF_ask_application_module(&module_id);
    if (UF_APP_CAM!=module_id) {
       // UF_APP_GATEWAY UF_APP_CAM UF_APP_MODELING UF_APP_NONE
       uc1601("Запуск DLL - производится из модуля обработки\n(ОГТ-ОПУ,КНААПО) - 2005г.",1);
       return (-1);
    }

    /* Ask displayed part tag */
    if (NULL_TAG==UF_PART_ask_display_part()) {
      uc1601("Cam-часть не активна.....\n программа прервана.",1);
      return (0);
    }

  /********************************************************************************/

    // выбранные обьекты и их кол-во
    UF_CALL( UF_UI_ONT_ask_selected_nodes(&obj_count, &tags) );
    if (obj_count<=0) {
      uc1601("Не выбрано операций!\n Программа прервана..",1);
      return(0);
    }

    UF_UI_toggle_stoplight(1);

    if (obj_count) {

      //UF_UI_open_listing_window();

      // инициализация массива
      for(i=0;i<14;i++) { da[i]=-1.;     ia4[i]=0;    ra5[i]=-1.;    ip7[i]=0;  }

      /********************************************************************************/
      // инициализация массива - первой операцией в списке
      for(i=0;i<1;i++) {
         prg = tags[i]; // идентификатор объекта
         cam3_ask(prg,da);
      }

      /********************************************************************************/
        strcpy(&menu[0][0], "Convex Ang(L)\0");       ia4[0]=(int)da[0];   ra5[0]=da[0];   ip7[0]=100;
        strcpy(&menu[1][0], "CirFeedComp(F)\0");      ia4[1]=(int)da[1];   ra5[1]=da[1];   ip7[1]=101;
        strcpy(&menu[2][0], "min=\0");                ia4[2]=(int)da[2];   ra5[2]=da[2];   ip7[2]=200;
        strcpy(&menu[3][0], "max=\0");                ia4[3]=(int)da[3];   ra5[3]=da[3];   ip7[3]=201;
        strcpy(&menu[4][0], "Fillets(F)\0");          ia4[4]=(int)da[4];   ra5[4]=da[4];   ip7[4]=102;
        strcpy(&menu[5][0], "Fillets R=\0");          ia4[5]=(int)da[5];   ra5[5]=da[5];   ip7[5]=202;
        strcpy(&menu[6][0], "Fillets Rmin=\0");       ia4[6]=(int)da[6];   ra5[6]=da[6];   ip7[6]=203;
        strcpy(&menu[7][0], "Slowdowns(F)\0");        ia4[7]=(int)da[7];   ra5[7]=da[7];   ip7[7]=103;
        strcpy(&menu[8][0], "Length(L)\0");           ia4[8]=(int)da[8];   ra5[8]=da[8];   ip7[8]=104;
        strcpy(&menu[9][0], "Per Prev/Tool=\0");      ia4[9]=(int)da[9];   ra5[9]=da[9];   ip7[9]=204;
        strcpy(&menu[10][0], "Slowdowns %=\0");       ia4[10]=(int)da[10]; ra5[10]=da[10]; ip7[10]=205;
        strcpy(&menu[11][0], "Num of Step=\0");       ia4[11]=(int)da[11]; ra5[11]=da[11]; ip7[11]=105;
        strcpy(&menu[12][0], "Ang:min=\0");           ia4[12]=(int)da[12]; ra5[12]=da[12]; ip7[12]=206;
        strcpy(&menu[13][0], "Ang:max=\0");           ia4[13]=(int)da[13]; ra5[13]=da[13]; ip7[13]=207;

        /*
        response = uc1609("..Параметры подач (мм/мин)..", menu, 14, da, &i);
        if (response != 3 && response != 4) { return (-1); }
        */

       response = uc1613 ("..Параметры подач (мм/мин)..", menu, 14 ,ia4, ra5, ca6, ip7);
       if (response != 3 && response != 4) { return (-1); }

      for(i=0;i<14;i++) {
        if (ip7[i]>=100 && ip7[i]<=199) da[i]=ia4[i];
        if (ip7[i]>=200 && ip7[i]<=299) da[i]=ra5[i];
        if (ip7[i]>=300 && ip7[i]<=399) { ; }
      }

      /********************************************************************************/

        generat=1;
        UF_UI_message_dialog("Cam.....", UF_UI_MESSAGE_QUESTION, mes2, 1, TRUE, &buttons2, &generat);
        if (generat==2) { generat=0; }

      /********************************************************************************/

      for(i=0,count=0;i<obj_count;i++)
      {
         prg = tags[i]; // идентификатор объекта
         prog_name[0]='\0';
         //UF_OBJ_ask_name(prg, prog_name);// спросим имя обьекта
         //UF_OPER_ask_name_from_tag(prg, prog_name);
         //UF_UI_write_listing_window("\n");UF_UI_write_listing_window(prog_name);UF_UI_write_listing_window("\n");

         /* type =               subtype =
         //     программа=121              160
         //     операция =100              110 */
         UF_CALL( UF_OBJ_ask_type_and_subtype (prg, &type, &subtype ) );

         if (type!=100) continue ;

         cam3_set(prg,da);

         if (generat==1) { UF_CALL( UF_PARAM_generate (prg,&generated ) ); }

      }
      UF_free(tags);
    }

    UF_UI_toggle_stoplight(0);

    //UF_DISP_refresh ();

    str[0]='\0'; sprintf(str,"Изменено значений =%d \n Просмотрено операций в цикле =%d \n программа завершена.",count,obj_count);
    uc1601(str,1);

 return (0);
}

int cam3_ask(tag_t param_tag,double da[14])
{
  int param_int;
  //double fillet[2],prev_dia[2];

  UF_PARAM_ask_int_value(param_tag,UF_PARAM_CORNER_CONTROL_METHOD,&param_int);
  da[0]=param_int;

  /******************************************************************************/

  UF_PARAM_ask_int_value(param_tag,UF_PARAM_CORNER_CIR_FEED_COMP,&param_int);
  da[1]=param_int;
  UF_PARAM_ask_double_value(param_tag,UF_PARAM_CIR_FEED_COMP_MINIMUM,&da[2]);
  UF_PARAM_ask_double_value(param_tag,UF_PARAM_CIR_FEED_COMP_MAXIMUM,&da[3]);

  /******************************************************************************/

  UF_PARAM_ask_int_value(param_tag,UF_PARAM_CORNER_FILLET_TYPE,&param_int);
  da[4]=param_int;

  UF_PARAM_ask_double_value(param_tag,UF_PARAM_CORNER_FILLET_PATH_RADIUS,&da[5]);
  UF_PARAM_ask_double_value(param_tag,UF_PARAM_MINIMUM_CORNER_FILLET_RADIUS,&da[6]);

/*
  UF_PARAM_ask_2d_value(param_tag,UF_PARAM_CORNER_FILLET_PATH_RADIUS,fillet);
  da[5]=fillet[0];
  UF_PARAM_ask_2d_value(param_tag,UF_PARAM_MINIMUM_CORNER_FILLET_RADIUS,fillet);
  da[6]=fillet[1];
*/

  /******************************************************************************/

  UF_PARAM_ask_int_value(param_tag,UF_PARAM_CORNER_SLOWDOWN_STATUS,&param_int);
  da[7]=param_int;
  UF_PARAM_ask_int_value(param_tag,UF_PARAM_CORNER_SLOWDOWN_LENGTH_METHOD,&param_int);
  da[8]=param_int;
  if (param_int==1)
    UF_PARAM_ask_double_value(param_tag,UF_PARAM_CORNER_TL_DIAM_PERCENT,&da[9]);
   else
    UF_PARAM_ask_double_value(param_tag,UF_PARAM_CORNER_PREV_TL_DIAM,&da[9]);

  /*UF_PARAM_ask_2d_value(param_tag,UF_PARAM_CORNER_PREV_TL_DIAM,prev_dia);*/

  /******************************************************************************/

  UF_PARAM_ask_double_value(param_tag,UF_PARAM_CORNER_FEED_ADJUST_PERCENT,&da[10]);
  UF_PARAM_ask_int_value(param_tag,UF_PARAM_CORNER_NUM_STEPS,&param_int);
  da[11]=param_int;

  UF_PARAM_ask_double_value(param_tag,UF_PARAM_CORNER_MAX_ANGLE,&da[13]);
  UF_PARAM_ask_double_value(param_tag,UF_PARAM_CORNER_MIN_ANGLE,&da[12]);

  return(0);
}

int cam3_set(tag_t param_tag,double da[14])
{

  int param_int;
  //int value_int;
  double value_double;
  int cnt = 0 ;
  char str[133];

  UF_PARAM_ask_int_value(param_tag,UF_PARAM_CORNER_CONTROL_METHOD,&param_int);
  if (param_int!=((int)da[0])) {
    da[0]=fabs(da[0]);
    param_int=(int)da[0];
    if (param_int!=0 && param_int!=1 && param_int!=2) {
      param_int=2;
      str[0]='\0';
      sprintf(str,"corner control method:\nFOR 2-2 1/2 AXIS MILL:0=UNDEFINED, 1=ACTIVE (ADD ARCS), 2=INACTIVE (EXTEND TANGENTS)");
      uc1601(str,1);
    } else {
      UF_PARAM_set_int_value(param_tag,UF_PARAM_CORNER_CONTROL_METHOD,param_int);
      cnt++;
    }
    if (param_int==0) return (0); //  *                0 = UNDEFINED
    if (param_int==2) return (0); //  *                2 = INACTIVE (EXTEND TANGENTS)
  }

  /******************************************************************************/

  UF_PARAM_ask_int_value(param_tag,UF_PARAM_CORNER_CIR_FEED_COMP,&param_int);
  if (param_int!=((int)da[1])) {
    da[1]=fabs(da[1]);
    param_int=(int)da[1];
    if (param_int!=0 && param_int!=1) {
      param_int=0;
      str[0]='\0';
      sprintf(str,"Parameter indicates whether Circular\nFeedrate Compensation is on or off.\n {off,on}={0,1}");
      uc1601(str,1);
    } else {
      UF_PARAM_set_int_value(param_tag,UF_PARAM_CORNER_CIR_FEED_COMP,param_int);
      cnt++;
    }
  }

  UF_PARAM_ask_double_value(param_tag,UF_PARAM_CIR_FEED_COMP_MINIMUM,&value_double);
  if (value_double!=da[2]) {
    if (da[2]<=0.||da[2]>1.) {
      da[2]=1.;
      str[0]='\0';
      sprintf(str,"Paramter is the minimum compensation factor\nfor circular feedrate compensation..\n 0.0 < UF_PARAM_type_double <= 1.0");
      uc1601(str,1);
    } else {
     UF_PARAM_set_double_value(param_tag,UF_PARAM_CIR_FEED_COMP_MINIMUM,da[2]);
     cnt++;
    }
  }

  UF_PARAM_ask_double_value(param_tag,UF_PARAM_CIR_FEED_COMP_MAXIMUM,&value_double);
  if (value_double!=da[3]) {
    if (da[3]<1.) {
      da[3]=1.;
      str[0]='\0';
      sprintf(str,"Paramter is the maximum compensation factor\nfor circular feedrate compensation..\n UF_PARAM_type_double >= 1.0");
      uc1601(str,1);
    } else {
     UF_PARAM_set_double_value(param_tag,UF_PARAM_CIR_FEED_COMP_MAXIMUM,da[3]);
     cnt++;
    }
  }

  /******************************************************************************/

  UF_PARAM_ask_int_value(param_tag,UF_PARAM_CORNER_FILLET_TYPE,&param_int);
  if (param_int!=((int)da[4])) {
    da[4]=fabs(da[4]);
    param_int=(int)da[4];
    if (param_int!=0 && param_int!=1 && param_int!=2){
      param_int=0;
      str[0]='\0';
      sprintf(str,"Parameter indicates filleting types at corners of\nthe toolpaths.\n none=0,on_wall=1,all_passes=2");
      uc1601(str,1);
    } else {
      UF_PARAM_set_int_value(param_tag,UF_PARAM_CORNER_FILLET_TYPE,param_int);
      cnt++;
    }
  }

  UF_PARAM_ask_double_value(param_tag,UF_PARAM_CORNER_FILLET_PATH_RADIUS,&value_double);
  if (value_double!=da[5]) {
    da[5]=fabs(da[5]);
    UF_PARAM_set_double_value(param_tag,UF_PARAM_CORNER_FILLET_PATH_RADIUS,da[5]);
    cnt++;
  }

  UF_PARAM_ask_double_value(param_tag,UF_PARAM_MINIMUM_CORNER_FILLET_RADIUS,&value_double);
  if (value_double!=da[6]) {
    da[6]=fabs(da[6]);
    UF_PARAM_set_double_value(param_tag,UF_PARAM_MINIMUM_CORNER_FILLET_RADIUS,da[6]);
    cnt++;
  }

  /******************************************************************************/

  UF_PARAM_ask_int_value(param_tag,UF_PARAM_CORNER_SLOWDOWN_STATUS,&param_int);
  if (param_int!=((int)da[7])) {
    da[7]=fabs(da[7]);
    param_int=(int)da[7];
    if (param_int!=0 && param_int!=1){
      param_int=0;
      str[0]='\0';
      sprintf(str,"Parameter indicates whether corner slowdown\nis on or off.\n off = 0, on = 1");
      uc1601(str,1);
    } else {
      UF_PARAM_set_int_value(param_tag,UF_PARAM_CORNER_SLOWDOWN_STATUS,param_int);
      cnt++;
    }
  }

  UF_PARAM_ask_int_value(param_tag,UF_PARAM_CORNER_SLOWDOWN_LENGTH_METHOD,&param_int);
  if (param_int!=((int)da[8])) {
    da[8]=fabs(da[8]);
    param_int=(int)da[8];
    if (param_int!=0 && param_int!=1){
      param_int=1;
      str[0]='\0';
      sprintf(str,"Parameter determines which slowdown method\nis to be used if slowdowns are on.\n previous_tool=0, percent_tool=1");
      uc1601(str,1);
    } else {
      UF_PARAM_set_int_value(param_tag,UF_PARAM_CORNER_SLOWDOWN_LENGTH_METHOD,param_int);
      cnt++;
    }
  }
  if (param_int==1) {
     UF_PARAM_ask_double_value(param_tag,UF_PARAM_CORNER_TL_DIAM_PERCENT,&value_double);
     if (value_double!=da[9]) {
       da[9]=fabs(da[9]);
       UF_PARAM_set_double_value(param_tag,UF_PARAM_CORNER_TL_DIAM_PERCENT,da[9]);
       cnt++;
     }
  } else {
     UF_PARAM_ask_double_value(param_tag,UF_PARAM_CORNER_PREV_TL_DIAM,&value_double);
     if (value_double!=da[9]) {
       da[9]=fabs(da[9]);
       UF_PARAM_set_double_value(param_tag,UF_PARAM_CORNER_PREV_TL_DIAM,da[9]);
       cnt++;
     }
  }

  UF_PARAM_ask_double_value(param_tag,UF_PARAM_CORNER_FEED_ADJUST_PERCENT,&value_double);
  if (value_double!=da[10]) {
    da[10]=fabs(da[10]);
    UF_PARAM_set_double_value(param_tag,UF_PARAM_CORNER_FEED_ADJUST_PERCENT,da[10]);
    cnt++;
  }

  UF_PARAM_ask_int_value(param_tag,UF_PARAM_CORNER_NUM_STEPS,&param_int);
  if (param_int!=((int)da[11])) {
    da[11]=fabs(da[11]);
    param_int=(int)da[11];
    UF_PARAM_set_int_value(param_tag,UF_PARAM_CORNER_NUM_STEPS,param_int);
    cnt++;
  }

  /******************************************************************************/

  UF_PARAM_ask_double_value(param_tag,UF_PARAM_CORNER_MAX_ANGLE,&value_double);
  if (value_double!=da[13]) {
    da[13]=fabs(da[13]);
    if (da[13]<=0 || da[13]>180.) {
      da[13]=175.;
      str[0]='\0';
      sprintf(str,"Parameter is the angle for corner control\ni.e. for corner filleting of corner slowdowns.\n 0.0 < maximum angle < 180.0");
      uc1601(str,1);
    }
    UF_PARAM_set_double_value(param_tag,UF_PARAM_CORNER_MAX_ANGLE,da[13]);
    cnt++;
  }

  UF_PARAM_ask_double_value(param_tag,UF_PARAM_CORNER_MIN_ANGLE,&value_double);
  if (value_double!=da[12]) {
    da[12]=fabs(da[12]);
    if (da[12]<0 || da[12]>180.) {
      da[12]=0;
      str[0]='\0';
      sprintf(str,"Parameter is the angle for corner control\ni.e. for corner filleting of corner slowdowns.\n 0.0 <= minimum angle < 180.0");
      uc1601(str,1);
    }
    UF_PARAM_set_double_value(param_tag,UF_PARAM_CORNER_MIN_ANGLE,da[12]);
    cnt++;
  }

  /******************************************************************************/

  return(cnt);
}



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


double dp[14];

int ufusr_main( );
int _SET ( tag_t  prg ) ;
int _READ ( int dialog_id ) ;
int _SENSITIVITY ( int group , int dialog_id ) ;
int _apply_cb ( int dialog_id ) ;
int _convex_corner_cb ( int dialog_id ) ;
int _circular_feed_rate_comp_cb ( int dialog_id ) ;
int _fillets_cb ( int dialog_id ) ;
int _slowdowns_cb ( int dialog_id ) ;
int _length_cb ( int dialog_id ) ;

//----------------------------------------------------------------------------
//  Activation Methods
//----------------------------------------------------------------------------

//  Explicit Activation
//      This entry point is used to activate the application explicitly, as in
//      "File->Execute UG/Open->User Function..."
extern "C" DllExport void ufusr( char *parm, int *returnCode, int rlen )
{
    /* Initialize the API environment */
    int errorCode = UF_initialize();

    if ( 0 == errorCode )
    {
        /* TODO: Add your application code here */
        ufusr_main( );

        /* Terminate the API environment */
        errorCode = UF_terminate();
    }

    /* Print out any error messages */
    *returnCode=0;
}

//----------------------------------------------------------------------------
//  Utilities
//----------------------------------------------------------------------------

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



/*********************************************



*********************************************/
int ufusr_main( )
{
  int errorCode;
  int response ;

  /****************************************************************************/

    if ( ( errorCode = UF_STYLER_create_dialog (
            actions->styler_file,
           SL_cbs,      /* Callbacks from dialog */
           SL_CB_COUNT, /* number of callbacks*/
            NULL,        /* This is your client data */
            &response ) ) != 0 )
    {
       //uc1601("Ошибка загрузки шаблона диалога\n -Ошибка памяти- \n, 2006.",1);
       do_cam3();
    }

  /****************************************************************************/

  return(0);
}


int _READ ( int dialog_id )
{
  UF_STYLER_item_value_type_t data  ;
  int i , j;

  char *ID[]={
    SL_CORNER_CONTROL_METHOD,        "1",

    SL_CORNER_CIR_FEED_COMP,         "1",
    SL_CIR_FEED_COMP_MAXIMUM,        "0",
    SL_CIR_FEED_COMP_MINIMUM,        "0",

    SL_CORNER_FILLET_TYPE,           "1",
    SL_CORNER_FILLET_PATH_RADIUS,    "0",
    SL_MINIMUM_CORNER_FILLET_RADIUS, "0",

    SL_CORNER_SLOWDOWN_STATUS,       "1",
    SL_CORNER_SLOWDOWN_LENGTH_METHOD,"1",
    SL_CORNER_PERCENT_TOOL,          "0",
    SL_CORNER_TL_DIAM_PERCENT,       "0",
    SL_CORNER_NUM_STEPS,             "1",
    SL_CORNER_MIN_ANGLE,             "0",
    SL_CORNER_MAX_ANGLE,             "0",
    NULL
  } ;

  data.item_attr=UF_STYLER_VALUE;

  for (i=0,j=0;i<14;i++,j+=2)
  {
    data.item_id=ID[j];
    UF_STYLER_ask_value(dialog_id,&data);
    if (0==strcmp(ID[j+1],"1"))
      dp[i]=data.value.integer ;
    else
      dp[i]=data.value.real ;
  }

  UF_STYLER_free_value (&data) ;

  return (0);
}


int _SET ( tag_t  prg )
{
  int i ;
  int param_int;

 int IDPARAM[14][2]={
   { UF_PARAM_CORNER_CONTROL_METHOD,        1 } ,

   { UF_PARAM_CORNER_CIR_FEED_COMP,         2 } ,
   { UF_PARAM_CIR_FEED_COMP_MAXIMUM,        0 } ,
   { UF_PARAM_CIR_FEED_COMP_MINIMUM,        0 } ,

   { UF_PARAM_CORNER_FILLET_TYPE,           3 } ,
   { UF_PARAM_CORNER_FILLET_PATH_RADIUS,    0 } ,
   { UF_PARAM_MINIMUM_CORNER_FILLET_RADIUS, 0 } ,

   { UF_PARAM_CORNER_SLOWDOWN_STATUS,       4 } ,
   { UF_PARAM_CORNER_SLOWDOWN_LENGTH_METHOD,5 } ,
   { UF_PARAM_CORNER_PREV_TL_DIAM,          UF_PARAM_CORNER_TL_DIAM_PERCENT } ,
   { UF_PARAM_CORNER_FEED_ADJUST_PERCENT,   0 } ,
   { UF_PARAM_CORNER_NUM_STEPS,             10 } ,
   { UF_PARAM_CORNER_MIN_ANGLE,             0 } ,
   { UF_PARAM_CORNER_MAX_ANGLE,             0 }
  } ;

 /******************************************************************************/

// for (i=0;i<14;i++)
//  printf("\n %d = %.5lf  ( %d , %d ) " ,i, dp[i] , IDPARAM[i][0] , IDPARAM[i][1] );

 i=0 ;
 param_int=(int)dp[i];
 UF_CALL(UF_PARAM_set_int_value(prg,IDPARAM[i][0],param_int));

 i=1 ;
 param_int=(int)dp[i];
 UF_CALL(UF_PARAM_set_int_value(prg,IDPARAM[i][0],param_int));
 if (param_int!=0) {
  i++; UF_CALL( UF_PARAM_set_double_value(prg,IDPARAM[i][0],dp[i]) );
  i++; UF_CALL( UF_PARAM_set_double_value(prg,IDPARAM[i][0],dp[i]) );
 }

 i=4 ;
 param_int=(int)dp[i];
 UF_CALL(UF_PARAM_set_int_value(prg,IDPARAM[i][0],param_int));
 if (param_int!=0) {
  i++; UF_CALL( UF_PARAM_set_double_value(prg,IDPARAM[i][0],dp[i]) );
  i++; UF_CALL( UF_PARAM_set_double_value(prg,IDPARAM[i][0],dp[i]) );
 }

 i=7 ;
 param_int=(int)dp[i];
 UF_CALL(UF_PARAM_set_int_value(prg,IDPARAM[i][0],param_int));
 if (param_int!=0) {

  i++;
  param_int=(int)dp[i];
  UF_CALL(UF_PARAM_set_int_value(prg,IDPARAM[i][0],param_int));
  switch (param_int) {
    case 0:
    i++; UF_CALL( UF_PARAM_set_double_value(prg,IDPARAM[i][0],dp[i]) );
    break;
    default:
    i++; UF_CALL( UF_PARAM_set_double_value(prg,IDPARAM[i][1],dp[i]) );
    break;
  }

  i++; UF_CALL( UF_PARAM_set_double_value(prg,IDPARAM[i][0],dp[i]) );

  i++;
  param_int=(int)dp[i];
  UF_CALL(UF_PARAM_set_int_value(prg,IDPARAM[i][0],param_int));

  i++; UF_CALL( UF_PARAM_set_double_value(prg,IDPARAM[i][0],dp[i]) );
  i++; UF_CALL( UF_PARAM_set_double_value(prg,IDPARAM[i][0],dp[i]) );
 }

 /******************************************************************************/

 return (0);
}


int _apply_cb ( int dialog_id )
{
  /*  Variable Declarations */
  char   str[133];

  tag_t       prg = NULL_TAG;
  int   i , j , count = 0 ;
  int   obj_count = 0;
  tag_t *tags = NULL ;
  int generat; logical generated ;

  char *mes2[]={
      "Производить генерацию операции после изменения?",
      NULL
  };
  UF_UI_message_buttons_t buttons2 = { TRUE, FALSE, TRUE, "Генерировать..", NULL, "Нет", 1, 0, 2 };

  int module_id;
  UF_ask_application_module(&module_id);
  if (UF_APP_CAM!=module_id) {
     // UF_APP_GATEWAY UF_APP_CAM UF_APP_MODELING UF_APP_NONE
     uc1601("Запуск DLL - производится из модуля обработки\n - 2005г.",1);
     return (-1);
  }

  /* Ask displayed part tag */
  if (NULL_TAG==UF_PART_ask_display_part()) {
    uc1601("Cam-часть не активна.....\n программа прервана.",1);
    return (0);
  }

  /********************************************************************************/

  // выбранные обьекты и их кол-во
  UF_CALL( UF_UI_ONT_ask_selected_nodes(&obj_count, &tags) );
  if (obj_count<=0) {
    uc1601("Не выбрано операций!\n Программа прервана..",1);
    return(0);
  }

  _READ ( dialog_id ) ;

  /********************************************************************************/

  generat=1;
  UF_UI_message_dialog("Cam.....", UF_UI_MESSAGE_QUESTION, mes2, 1, TRUE, &buttons2, &generat);
  if (generat==2) { generat=0; }

  /********************************************************************************/

 UF_UI_toggle_stoplight(1);

 for(i=0,count=0;i<obj_count;i++)
 {
   prg = tags[i]; // идентификатор объекта

   prg_list_count=0;// заполняем структуру
   UF_CALL( UF_NCGROUP_cycle_members( prg, cycleGeneratePrg,NULL ) );

   if (prg_list_count==0) {
      _SET( prg );
      if (generat==1) { UF_CALL( UF_PARAM_generate (prg,&generated ) ); }
      count++;
   } else for (j=0;j<prg_list_count;j++,count++) {
             _SET( prg_list[j].tag );
             if (generat==1) { UF_CALL( UF_PARAM_generate (prg_list[j].tag,&generated ) ); }
           }

 }

 UF_free(tags);

 //UF_DISP_refresh ();
 UF_UI_ONT_refresh();

 UF_UI_toggle_stoplight(0);

 str[0]='\0'; sprintf(str,"Изменено значений =%d \n Просмотрено операций в цикле =%d \n программа завершена.",count,obj_count);
 uc1601(str,1);

 return (0);
}

int _convex_corner_cb ( int dialog_id )
{
  _SENSITIVITY ( 0 , dialog_id ) ;
  return (0);
}

int _circular_feed_rate_comp_cb ( int dialog_id )
{
  _SENSITIVITY ( 1 , dialog_id ) ;
  return (0);
}

int _fillets_cb ( int dialog_id )
{
  _SENSITIVITY ( 2 , dialog_id ) ;
  return (0);
}

int _slowdowns_cb ( int dialog_id )
{
  _SENSITIVITY ( 3 , dialog_id ) ;
  return (0);
}


int _length_cb ( int dialog_id )
{
  UF_STYLER_item_value_type_t data  ;
  int ind;
  char **stropt ;

  data.item_id=SL_CORNER_SLOWDOWN_LENGTH_METHOD;

  data.item_attr=UF_STYLER_VALUE;
  UF_STYLER_ask_value(dialog_id,&data);
  ind=data.value.integer ;
  data.item_attr=UF_STYLER_SUBITEM_VALUES;
  UF_STYLER_ask_value(dialog_id,&data);
  stropt=data.value.strings ;

  data.item_attr=UF_STYLER_LABEL;
  data.item_id=SL_CORNER_PERCENT_TOOL;
  data.value.string=stropt[ind];
  UF_STYLER_set_value(dialog_id,&data);

  data.item_attr=UF_STYLER_VALUE;

  switch (ind) {
  case 0:
    data.item_id=SL_CORNER_PERCENT_TOOL;
    data.value.real=30;
    UF_STYLER_set_value(dialog_id,&data);
    break ;
  default :
    data.item_id=SL_CORNER_PERCENT_TOOL;
    data.value.real=110;
    UF_STYLER_set_value(dialog_id,&data);
    break ;
  }

  UF_free(stropt);
  UF_STYLER_free_value (&data) ;
  return (0);
}


int _SENSITIVITY ( int group , int dialog_id )
{
  UF_STYLER_item_value_type_t data  ;
  int  i;
  const int N = 10 ;
  char STR[N][80];
  int  num = 0;
  int  sens = 0;

  for(i=0;i<N;i++) STR[i][0]='\0';

  switch (group) {
  case 0:
    //group = 0
    num = 0;
    i=0; sprintf(STR[i],"%s",SL_CORNER_CONTROL_METHOD);
    break ;
  case 1:
    //group = 1  SL_CORNER_CIR_FEED_COMP
    num = 3;
    i=0; sprintf(STR[i],"%s",SL_CORNER_CIR_FEED_COMP);
    i++; sprintf(STR[i],"%s",SL_CIR_FEED_COMP);
    i++; sprintf(STR[i],"%s",SL_CIR_FEED_COMP_MAXIMUM);
    i++; sprintf(STR[i],"%s",SL_CIR_FEED_COMP_MINIMUM);
    break ;
  case 2:
    //group = 2  SL_CORNER_FILLET_TYPE
    num = 2;
    i=0; sprintf(STR[i],"%s",SL_CORNER_FILLET_TYPE);
    i++; sprintf(STR[i],"%s",SL_CORNER_FILLET_PATH_RADIUS);
    i++; sprintf(STR[i],"%s",SL_MINIMUM_CORNER_FILLET_RADIUS);
    break ;
  case 3:
    //group = 3  SL_CORNER_SLOWDOWN_STATUS
    num = 7;
    i=0; sprintf(STR[i],"%s",SL_CORNER_SLOWDOWN_STATUS);
    i++; sprintf(STR[i],"%s",SL_CORNER_SLOWDOWN_LENGTH_METHOD);
    i++; sprintf(STR[i],"%s",SL_CORNER_PERCENT_TOOL);
    i++; sprintf(STR[i],"%s",SL_CORNER_TL_DIAM_PERCENT);
    i++; sprintf(STR[i],"%s",SL_CORNER_NUM_STEPS);
    i++; sprintf(STR[i],"%s",SL_LAB_CORNER_ANGLE);
    i++; sprintf(STR[i],"%s",SL_CORNER_MIN_ANGLE);
    i++; sprintf(STR[i],"%s",SL_CORNER_MAX_ANGLE);
  break ;
  default : ; break ;
  }

  data.item_attr=UF_STYLER_VALUE;
  data.item_id=STR[0];
  UF_STYLER_ask_value(dialog_id,&data);
  sens=data.value.integer;
  if (sens>0) sens=1;

  data.item_attr=UF_STYLER_SENSITIVITY;

  for(i=1;i<=num;i++) {

    data.item_id=STR[i];
    data.value.integer=sens; // (int)(!sens);
    UF_STYLER_set_value(dialog_id,&data);

  }

  UF_STYLER_free_value (&data) ;

  return (0);
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
     _apply_cb ( dialog_id ) ;

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
     _convex_corner_cb ( dialog_id ) ;

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
     _circular_feed_rate_comp_cb ( dialog_id ) ;

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
     _fillets_cb ( dialog_id ) ;

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
     _slowdowns_cb ( dialog_id ) ;

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
     _length_cb ( dialog_id ) ;

     UF_terminate ();

    /* Callback acknowledged, do not terminate dialog */
    return (UF_UI_CB_CONTINUE_DIALOG);

    /* or Callback acknowledged, terminate dialog.    */
    /* return ( UF_UI_CB_EXIT_DIALOG );               */

}



