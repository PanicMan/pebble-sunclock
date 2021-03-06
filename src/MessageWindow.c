/**
 *  @file
 *  
 */


#include  <pebble.h>

#include  "MessageWindow.h"
#include  "sunclock.h"        // for shared font resources


static Window * pMsgWindow = 0;
static TextLayer * pMsgText = 0;
static TextLayer * pCaption = 0;

static char achTextBuf[128];

#define FULL_WIDTH 144
#define FULL_HEIGHT 168

#define TEXT_Y_ORIGIN 45

void  message_window_init()
{

   //  do all allocations here, since we don't know when our window might be loaded.

   pMsgWindow = window_create();
   pMsgText = text_layer_create((GRect) {.origin = { 0, TEXT_Y_ORIGIN },
                                           .size = { FULL_WIDTH, FULL_HEIGHT-TEXT_Y_ORIGIN } });
   pCaption = text_layer_create((GRect) {.origin = { 0, 0 },
                                           .size = { FULL_WIDTH, TEXT_Y_ORIGIN } });

   if ((pMsgWindow == NULL) || (pMsgText == NULL) || (pCaption == NULL))
   {
      return;
   }

   text_layer_set_font(pCaption, pFontMediumText);
   text_layer_set_font(pMsgText, pFontSmallText);
   
   text_layer_set_text_alignment(pCaption, GTextAlignmentCenter);
   layer_add_child (window_get_root_layer(pMsgWindow), (Layer *) pMsgText);
   layer_add_child (window_get_root_layer(pMsgWindow), (Layer *) pCaption);

}  /* end of message_window_init */


void  message_window_deinit()
{

   message_window_hide();

   layer_remove_child_layers(window_get_root_layer(pMsgWindow));

   text_layer_destroy(pMsgText);
   text_layer_destroy(pCaption);
   window_destroy(pMsgWindow);

}  /* end of message_window_deinit */


static void  show_message_window()
{

   if (pMsgWindow != window_stack_get_top_window())
   {
      window_stack_push(pMsgWindow, false /* animated */);
   }

}  /* end of show_message_window */


void  message_window_show_status (const char *pszCaption, const char *pszText)
{

   text_layer_set_text(pCaption, pszCaption);
   text_layer_set_text(pMsgText, pszText);

   show_message_window();

}  /* end of message_window_show_status */


void  message_window_show_error (FailureSource eErrSrc,
                                 int32_t errCode, const char *pszErrMsg)
{

   const char * pszCaption;

   if (eErrSrc == FAIL_SRC_APP_MSG)
      pszCaption = "Watch/Phone Comms Error";
   else
      pszCaption = "Location Query Error";

   snprintf(achTextBuf, sizeof(achTextBuf), 
            "Code (%d) : %s", (int) errCode, pszErrMsg);

   text_layer_set_text(pCaption, pszCaption);
   text_layer_set_text(pMsgText, achTextBuf);

   show_message_window();

}  /* end of message_window_show_error */


void  message_window_hide()
{

   //  don't destroy it, just undisplay it.
   if (pMsgWindow == window_stack_get_top_window())
   {
      window_stack_pop (false /* animated	*/);
   }

}  /* end of message_window_hide */

