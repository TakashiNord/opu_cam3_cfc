//////////////////////////////////////////////////////////////////////////////
//
//  cam3.cpp
//
//  Description:
//      Contains Unigraphics entry points for the application.
//
//////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE 1

//  Include files
#include <uf.h>
#include <uf_exit.h>
#include <uf_ui.h>

#include <uf_obj.h>
#include <uf_ui_ont.h>
#include <uf_oper.h>
#include <uf_part.h>
#include <uf_param.h>
#include <uf_param_indices.h>

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

#include "cam3.h"


//---------------------------------------------------------------

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

int do_cam3();
int cam3_ask(tag_t ,double [14]);
int cam3_set(tag_t ,double [14]);

//---------------------------------------------------------------

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
        do_cam3();

        /* Terminate the API environment */
        errorCode = UF_terminate();
    }

    /* Print out any error messages */
    PrintErrorMessage( errorCode );
}

//----------------------------------------------------------------------------
//  Utilities
//----------------------------------------------------------------------------

// Unload Handler
//     This function specifies when to unload your application from Unigraphics.
//     If your application registers a callback (from a MenuScript item or a
//     User Defined Object for example), this function MUST return
//     "UF_UNLOAD_UG_TERMINATE".
extern "C" int ufusr_ask_unload( void )
{
     /* unload immediately after application exits*/
    return ( UF_UNLOAD_IMMEDIATELY );

     /*via the unload selection dialog... */
     //return ( UF_UNLOAD_SEL_DIALOG );
     /*when UG terminates...              */
     //return ( UF_UNLOAD_UG_TERMINATE );
}

/* PrintErrorMessage
**
**     Prints error messages to standard error and the Unigraphics status
**     line. */
static void PrintErrorMessage( int errorCode )
{
    if ( 0 != errorCode )
    {
        /* Retrieve the associated error message */
        char message[133];
        UF_get_fail_message( errorCode, message );

        /* Print out the message */
        UF_UI_set_status( message );

        fprintf( stderr, "%s\n", message );
    }
}


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
//    int type, subtype ;
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
         UF_OPER_ask_name_from_tag(prg, prog_name);

         //UF_UI_write_listing_window("\n");UF_UI_write_listing_window(prog_name);UF_UI_write_listing_window("\n");

         /* type =               subtype =
         //     программа=121              160
         //     операция =100              110 */
         //UF_CALL( UF_OBJ_ask_type_and_subtype (prg, &type, &subtype ) );

         cam3_set(prg,da);

         if (generat==1) { UF_CALL( UF_PARAM_generate (prg,&generated ) ); }

      }
      UF_free(tags);
    }

    UF_UI_toggle_stoplight(0);

    UF_DISP_refresh ();

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


