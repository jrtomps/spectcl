/*
    This software is Copyright by the Board of Trustees of Michigan
    State University (c) Copyright 2005.

    You may use this software under the terms of the GNU public license
    (GPL).  The terms of this license are described at:

     http://www.gnu.org/licenses/gpl.txt

     Author:
             Ron Fox
	     NSCL
	     Michigan State University
	     East Lansing, MI 48824-1321
*/

static const char* Copyright = "(C) Copyright Michigan State University 1994, All rights reserved";
/*
** Facility:
**   Xamine - C++ Support objects for Motif.
** Abstract:
**   XMDialogs.cc  - This file contains support routines for the XM dialog
**                   objects.  That is code which for one or another reason
**                   was not deemed appropriate for in-line definition.
** Author:
**   Ron Fox
**   NSCL
**   Michigan State University
**   East Lansing, MI 48824-1321
** Version:
**   @(#)XMDialogs.cc	8.2 9/10/96 
*/

/*
** Include files:
*/
#include <config.h>

#include "helpmenu.h"
#include "errormsg.h"

#include "XMDialogs.h"
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/FileSB.h>
#include <X11/Shell.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>



/*
** This page contains help texts which are defaults for the self contained
** dialog objects.
*/

static const  char *prompter_help[] = {
  "You are being prompted for some text.  Type the text in the text window.\n",
  " When you have edited the text to look like what you want click:  \n\n",
  "   Ok    - To accept the text and remove the dialog.\n",
  "   Apply - To accept the text and leave the dialog displayed \n",
  "   Cancel- To cancel dismiss the dialog without accepting the text\n",
  "   Help  - To display this message\n",
  NULL
};



/*
** Functional Description:
**   StripNonPrinting:
**     Creates an output string which consists of the input string with all
**     non printing characters stripped.
** Formal Parameters:
**     char* out:
**        output string.
**     char* in:
**        input string.
*/
static void StripNonPrinting(char* out, const char* in)
{
  int len = strlen(in);
  for(int i = 0; i < len; i++) {
    if(isprint(*in)) *out++ = *in;
    in++;
  }
  *out = '\0';
}

/*
** Functional Descriptions:
**   SetupBorders:
**     Sets up dialog box borders in motif to remove the menu.
** Formal Parameters:
**   Widget id:
**     Some widget in the dialog hierachy.
*/
static void SetupBorders(Widget id)
{
  /* Locate the vendor shell where all this stuff is */

  while(!XtIsVendorShell(id)) 
    id = XtParent(id);

  /* Turn off the menu but keep everything else the same: */

  if(XmIsMotifWMRunning(id)) {	/* Only applies to motif wm.  */
    int decorations;

    XtVaGetValues(id, XmNmwmDecorations, &decorations, NULL);
    decorations = decorations & (MWM_DECOR_BORDER | MWM_DECOR_RESIZEH |
				 MWM_DECOR_TITLE  | MWM_DECOR_MAXIMIZE |
				 MWM_DECOR_MINIMIZE);
    XtVaSetValues(id, XmNmwmDecorations, decorations, NULL);
  }
}

/*
** Implementation of functions previously implemented in XMDialogs.h
** in an attempt to get this to build on Tru64
*/

void
XMMessageBox::GetButtons()
{
  Widget wid;
  wid = XmMessageBoxGetChild(id, XmDIALOG_OK_BUTTON);
  okbutton = new XMPushButton(wid);
  
  wid = XmMessageBoxGetChild(id, XmDIALOG_CANCEL_BUTTON);
  cancelbutton = new XMPushButton(wid);
  
  wid = XmMessageBoxGetChild(id, XmDIALOG_HELP_BUTTON);
  helpbutton = new  XMPushButton(wid); 
}

XMMessageBox::~XMMessageBox()
{ 
  delete okbutton;
  delete cancelbutton;
  delete helpbutton;
}

XMMessageBox::XMMessageBox(const char *n) :
  XMManagedWidget(n)
{

}

XMMessageBox::XMMessageBox(Widget w) :
  XMManagedWidget(w)
{
  GetButtons();
}

XMPushButton*
XMMessageBox::GetOkButton()
{
  return okbutton; 
}

XMPushButton*
XMMessageBox::GetCancelButton()
{
  return cancelbutton;
}

XMPushButton*
XMMessageBox::GetHelpButton()
{
  return helpbutton; 
}

void
XMMessageBox::SetText(const char *txt)
{
  XmString s = XmStringCreateLtoR(const_cast<char*>(txt), 
				  const_cast<char*>(XmSTRING_DEFAULT_CHARSET));
  SetAttribute(XmNmessageString, s);
  XmStringFree(s);
}

Callback_data*
XMMessageBox::AddOkCallback(void (*cb)(XMWidget *w,
				       XtPointer, 
				       XtPointer),
			    XtPointer cd)
{
  return AddCallback(XmNokCallback, cb, cd);
}

Callback_data*
XMMessageBox::AddCancelCallback(void (*cb)(XMWidget *w,
					   XtPointer, 
					   XtPointer),
				XtPointer cd)
{
  return AddCallback(XmNcancelCallback, cb, cd);
}

void
XMMessageBox::LabelCancelButton(const char *txt) 
{
  XmString str = XmStringCreateLtoR(const_cast<char*>(txt), 
				    const_cast<char*>(XmSTRING_DEFAULT_CHARSET));
  SetAttribute(XmNcancelLabelString, str);
  XmStringFree(str);
}

void
XMMessageBox::LabelOkButton(const char *txt)
{
  XmString str = XmStringCreateLtoR(const_cast<char*>(txt),
				    const_cast<char*>(XmSTRING_DEFAULT_CHARSET));
  SetAttribute(XmNokLabelString, str);
  XmStringFree(str);
}

void
XMMessageBox::LabelHelpButton(const char *txt)
{
  helpbutton->Label(txt); 
}

void
XMMessageBox::DefaultToOk()
{
  SetAttribute(XmNdefaultButtonType,
	       XmDIALOG_OK_BUTTON);
}

void
XMMessageBox::DefaultToCancel()
{
  SetAttribute(XmNdefaultButtonType,
	       XmDIALOG_CANCEL_BUTTON);
}

void
XMMessageBox::DefaultToHelp()
{
  SetAttribute(XmNdefaultButtonType,
	       XmDIALOG_HELP_BUTTON);
}

void
XMMessageBox::Show()
{
  Manage();
  Widget shell = XtParent(id);
  XtPopup(shell, XtGrabNone);
  Window  w = XtWindow(shell);
  Display* d = XtDisplay(shell);
  XRaiseWindow(d,w);
}

void
XMMessageBox::Hide()
{
  UnManage();
}

void
XMMessageBox::SetModal(unsigned char modality)
{
  SetAttribute(XmNdialogStyle, modality);
}



/*
** Functions for class XMErrorDialog
*/

XMErrorDialog::XMErrorDialog(const char *n, Widget parent, const char *msg,
			     void (*cb)(XMWidget *,
					XtPointer, XtPointer),
			     XtPointer cbd,
			     ArgList list, Cardinal argcount) :
  XMMessageBox(n)
{
  id = XmCreateErrorDialog(parent,
			   name,
			   list,
			   argcount);
  SetText(msg);
  GetButtons();
  LabelOkButton("Dismiss");
  cancelbutton->Disable();
  helpbutton->Disable();
  if(cb)
    AddOkCallback(cb, cbd);
  Manage();
  XtPopup(XtParent(id), XtGrabNone);
}

XMErrorDialog::XMErrorDialog(const char *n, XMWidget &parent, const char *msg,
			     void (*cb)(XMWidget *,
					XtPointer, XtPointer),
			     XtPointer cbd,
			     ArgList list, Cardinal argcount):
  XMMessageBox(n)
{
  id = XmCreateErrorDialog(parent.getid(),
			   name,
			   list,
			   argcount);
  SetText(msg);
  GetButtons();
  LabelOkButton("Dismiss");
  cancelbutton->Disable();
  helpbutton->Disable();
  if(cb) 
    AddOkCallback(cb, cbd);
  Manage();
  XtPopup(XtParent(id), XtGrabNone);
}

XMErrorDialog::XMErrorDialog(Widget w) : 
  XMMessageBox(w)
{}




/*
** Functions for class XMInformationDialog
*/

XMInformationDialog::XMInformationDialog(const char *n,Widget parent, 
					 const char *msg,
					 void (*cb)(XMWidget *,
						    XtPointer, XtPointer) ,
					 XtPointer cbd,
					 ArgList list, 
					 Cardinal argcount) :
  XMMessageBox(n)
{
  id = XmCreateInformationDialog(parent,
				 name,
				 list,
				 argcount);
  SetText(msg);
  GetButtons();
  LabelOkButton("Dismiss");
  cancelbutton->Disable();
  helpbutton->Disable();
  if(cb)
    AddOkCallback(cb, cbd);
  Manage();
  XtPopup(XtParent(id), XtGrabNone);
}

XMInformationDialog::XMInformationDialog(const char *n, XMWidget &parent, 
					 const char *msg,
					 void (*cb)(XMWidget *,
						    XtPointer, XtPointer) ,
					 XtPointer cbd,
					 ArgList list, 
					 Cardinal argcount) :
  XMMessageBox(n)
{
  id = XmCreateInformationDialog(
				 parent.getid(),
				 name,
				 list,
				 argcount);
  SetText(msg);
  GetButtons();
  LabelOkButton("Dismiss");
  cancelbutton->Disable();
  helpbutton->Disable();
  if(cb) 
    AddOkCallback(cb, cbd);
  Manage();
  XtPopup(XtParent(id), XtGrabNone);
}

XMInformationDialog::XMInformationDialog(Widget w) :
  XMMessageBox(w)
{}



/*
** Functions from class XMMessageDialog
*/

XMMessageDialog::XMMessageDialog(const char *n,Widget parent, 
				 const char *msg,
				 void (*cb)(XMWidget *,
					    XtPointer, XtPointer),
				 XtPointer cbd,
				 ArgList list, Cardinal argcount) :
  XMMessageBox(n)
{
  id = XmCreateMessageDialog(parent,
			     name,
			     list,
			     argcount);
  SetText(msg);
  GetButtons();
  LabelOkButton("Dismiss");
  cancelbutton->Disable();
  helpbutton->Disable();
  if(cb)
    AddOkCallback(cb, cbd);
  Manage();
  XtPopup(XtParent(id), XtGrabNone);
}

XMMessageDialog::XMMessageDialog(const char *n, XMWidget &parent, 
				 const char *msg,
				 void (*cb)(XMWidget *,
					    XtPointer, XtPointer),
				 XtPointer cbd,
				 ArgList list, Cardinal argcount) :
  XMMessageBox(n)
{
  id = XmCreateMessageDialog(parent.getid(),
			     name,
			     list,
			     argcount);
  SetText(msg);
  GetButtons();
  LabelOkButton("Dismiss");
  cancelbutton->Disable();
  helpbutton->Disable();
  if(cb) 
    AddOkCallback(cb, cbd);
  Manage();
  XtPopup(XtParent(id), XtGrabNone);
}

XMMessageDialog::XMMessageDialog(Widget w) :
  XMMessageBox(w)
{}



/*
** Functions for class XMQuestionDialog
*/

void
XMQuestionDialog::Create(const char *n,Widget parent, const char *msg,
			 void (*cb)(XMWidget *,
				    XtPointer, XtPointer),
			 XtPointer cbd,
			 ArgList list, Cardinal argcount)
{
  id = XmCreateQuestionDialog(parent,
			      name,
			      list,
			      argcount);
  SetText(msg);
  GetButtons();
  LabelOkButton("Yes");
  LabelCancelButton("No");
  helpbutton->Disable();
  if(cb)
    AddOkCallback(cb, cbd);
  Manage();
  XtPopup(XtParent(id), XtGrabNone);
}

XMQuestionDialog::XMQuestionDialog(const char *n,Widget parent, const char *msg,
				   void (*cb)(XMWidget *,
					      XtPointer, XtPointer),
				   XtPointer cbd,
				   ArgList list, Cardinal argcount) :
  XMMessageBox(n)
{
  Create(n, parent, msg, cb, cbd, list, argcount);
}

XMQuestionDialog::XMQuestionDialog(const char *n, XMWidget &parent, char const *msg,
				   void (*cb)(XMWidget *,
					      XtPointer, XtPointer),
				   XtPointer cbd,
				   ArgList list, Cardinal argcount) :
  XMMessageBox(n)
{
  Create(n, parent.getid(), msg, cb, cbd, list, argcount);
}

XMQuestionDialog::XMQuestionDialog(Widget w) :
  XMMessageBox(w)
{}



/*
** Functional Description:
**   XMQuestioner::XMQuestioner:
**     This method instantiates a dialog which poses a yes/no question
**     The XMQuestioner class includes self contained behavior in the
**     sense that the two buttons (labelled Yes and No by default) are 
**     associated with callback which are methods of the object.
** Formal Parameters:
**   char *n:
**     Name of the question dialog widget.. also shown in the title part of
**     the widget banner.
**   XMWidget parent:
**   Widget   &parent:
**      Widget which is the parent of this widget.
**   char *msg:
**      Contains message text displayed in the dialog.  This should be a 
**      question with a yes or no answer or the buttons should be re-labelled
**      with the appropriate two answers.
**   XtPointer cbd:
**      Callback data which is passed to the Yes and No callback functions.
**   Arglist list:
**      Pointer to list of resources to override default widget resources
**   Cardinal argcount:
**      Number of resource entries in 'list'
*/
XMQuestioner::XMQuestioner(const char *n, Widget parent, const char *msg, XtPointer cbd,
			   ArgList list, Cardinal argcount) :
			   XMQuestionDialog(n, parent, msg, 
					    NULL, cbd, list, 
					    argcount) ,
					    yescallback(this) ,
					    nocallback(this)
{
  yescallback.Register(this, XmNokCallback, &XMQuestioner::Yescb, cbd);
  nocallback.Register(this, XmNcancelCallback, &XMQuestioner::Nocb, cbd);
  
}
XMQuestioner::XMQuestioner(const char *n, XMWidget &parent, const char *msg, XtPointer cbd,
			   ArgList list, Cardinal argcount) :
			   XMQuestionDialog(n, parent, msg, 
					    NULL, cbd, list, 
					    argcount) ,
			   yescallback(this) ,
			   nocallback(this)
{
  yescallback.Register(this, XmNokCallback, &XMQuestioner::Yescb, cbd);
  nocallback.Register(this, XmNcancelCallback, &XMQuestioner::Nocb, cbd);
  
}


/* 
** Functional Description:
**   XMQuestioner::~XMQuestioner:
**     Destroys an instance of a questioner class. 
*/
XMQuestioner::~XMQuestioner()
{
  yescallback.UnRegister();
  nocallback.UnRegister();
}

/*
** Functional Descriptions:
**   XMQuestioner::Yescb:
**      Virtual function called when the yes button is pressed.
**      default action is to unmanage the dialog box.  This function
**      is usually overridden.
**   XMQuestioner::Nocb:
**      Virtual method called when the no button is pressed.
**      default action is to unmanage the dialog box.  This function
**      is sometimes overridden.
** Formal Parameters:
**   XMWidget *wid:
**     The dialog widget itself.
**   XtPointer userd:
**     User data ignored by us.
**   XtPointer calld:
**     Callback data of the form XmAnyCallbackStruct.
*/
void XMQuestioner::Yescb(XMWidget *wid, XtPointer userd, XtPointer calld)
{
  UnManage();
}

void XMQuestioner::Nocb(XMWidget *wid, XtPointer userd, XtPointer calld)
{
  UnManage();
}

/*
** Functions for class XMWarningDialog
*/


void
XMWarningDialog::Create(const char *n,Widget parent, 
	      const char *msg,
	      void (*cb)(XMWidget *,
			 XtPointer, XtPointer),
	      XtPointer cbd,
	      ArgList list, Cardinal argcount)
{
  id = XmCreateWarningDialog(parent,
			     name,
			     list,
			     argcount);
  SetText(msg);
  LabelOkButton("Dismiss");
  GetButtons();
  cancelbutton->Disable();
  helpbutton->Disable();
  if(cb)
    AddOkCallback(cb, cbd);
  Manage();
  XtPopup(XtParent(id), XtGrabNone);
}


XMWarningDialog::XMWarningDialog(const char *n,Widget parent, 
				 const char *msg,
				 void (*cb)(XMWidget *,
					    XtPointer, XtPointer),
				 XtPointer cbd,
				 ArgList list, Cardinal argcount) :
  XMMessageBox(n)
{
  Create(n,parent,msg,cb,cbd,list,argcount);
}

XMWarningDialog::XMWarningDialog(const char *n, XMWidget &parent, 
				 const char *msg,
				 void (*cb)(XMWidget *,
					    XtPointer, XtPointer),
				 XtPointer cbd,
				 ArgList list, Cardinal argcount) :
  XMMessageBox(n)
{
  Create(n,parent.getid(),msg,cb,cbd,list,argcount);

}

XMWarningDialog::XMWarningDialog(Widget w) : 
  XMMessageBox(w)
{}

/*
** Functions for class XMWorkingDialog
*/
void
XMWorkingDialog::Create(const char *n,Widget parent, 
			const char *msg,
			void (*cb)(XMWidget *,
				   XtPointer, XtPointer),
			XtPointer cbd ,
			ArgList list, Cardinal argcount)
{
  id = XmCreateWorkingDialog(parent,
			     name,
			     list,
			     argcount);
  SetText(msg);
  GetButtons();
  LabelOkButton("Dismiss");
  cancelbutton->Disable();
  helpbutton->Disable();
  GetButtons();
  if(cb)
    AddOkCallback(cb, cbd);
  Manage();
  XtPopup(XtParent(id), XtGrabNone);
}
XMWorkingDialog::XMWorkingDialog(const char *n,Widget parent, 
				 const char *msg,
				 void (*cb)(XMWidget *,
					    XtPointer, XtPointer),
				 XtPointer cbd ,
				 ArgList list, Cardinal argcount) :
  XMMessageBox(n)
{
  Create(n,parent,msg,cb,cbd,list,argcount);
}

XMWorkingDialog::XMWorkingDialog(const char *n, XMWidget &parent, 
				 const char *msg,
				 void (*cb)(XMWidget *,
					    XtPointer, XtPointer),
				 XtPointer cbd,
				 ArgList list, Cardinal argcount) :
  XMMessageBox(n)
{
  Create(n,parent.getid(),msg,cb,cbd,list,argcount);

}

XMWorkingDialog::XMWorkingDialog(Widget w) :
  XMMessageBox(w)
{}



/*
** Implementation of functions from class XMSelection
*/
void
XMSelection::GetButtons()
{
  Widget wid;
  wid = XmSelectionBoxGetChild(id, XmDIALOG_OK_BUTTON);
  okbutton = new XMPushButton(wid);
  
  wid = XmSelectionBoxGetChild(id, XmDIALOG_CANCEL_BUTTON);
  cancelbutton = new XMPushButton(wid);
  
  wid = XmSelectionBoxGetChild(id, XmDIALOG_HELP_BUTTON);
  helpbutton = new  XMPushButton(wid); 
  
  applybutton  = new XMPushButton(XmSelectionBoxGetChild
				  (id, XmDIALOG_APPLY_BUTTON));
}

XMSelection::XMSelection(const char *name) : XMMessageBox(name) {}

XMSelection::XMSelection(Widget w) : 
  XMMessageBox(w) 
{
  GetButtons();
}

XMSelection::~XMSelection()
{
  delete applybutton;
}

void
XMSelection::SetTextWidth(Cardinal n) {
  SetAttribute(XmNtextColumns, (short)n);
}

Callback_data*
XMSelection::AddDoCallback(void (*cb)(XMWidget *, /* Do callbacks are  */
				      XtPointer, /* attached to both the ok */
				      XtPointer), /* and the apply button. */
			   XtPointer client_data ,
			   Callback_data **apply ) 
{
  Callback_data *apcb;
  apcb = AddCallback(XmNapplyCallback, cb, client_data);
  if (apply != NULL) *apply = apcb;
  return AddCallback(XmNokCallback, cb, client_data);
}

Callback_data*
XMSelection::AddApplyCallback(void (*cb)(XMWidget *,
					 XtPointer,
					 XtPointer),
			      XtPointer client_data )
{
  return AddCallback(XmNapplyCallback, cb, client_data);
}

XMPushButton*
XMSelection::GetApplyButton()
{
  return applybutton;
}

void
XMSelection::SetText(const char *txt)
{
  XmString s = XmStringCreateLtoR(const_cast<char*>(txt), const_cast<char*>(XmSTRING_DEFAULT_CHARSET));
  SetAttribute(XmNtextString, s);
  XmStringFree(s);
}

void
XMSelection::SetLabelString(const char *txt)
{
  XmString s = XmStringCreateLtoR(const_cast<char*>(txt), 
				  const_cast<char*>(XmSTRING_DEFAULT_CHARSET));
  SetAttribute(XmNselectionLabelString, s);
  XmStringFree(s);
}

void
XMSelection::LabelApplyButton(const char *txt)
{
  XmString str = XmStringCreateLtoR(const_cast<char*>(txt),
				    const_cast<char*>(XmSTRING_DEFAULT_CHARSET));
  SetAttribute(XmNapplyLabelString, str);
  XmStringFree(str);
}

void
XMSelection::DefaultToOk()
{
  SetAttribute(XmNdefaultButton, okbutton->getid());
}

void
XMSelection::DefaultToCancel()
{
  SetAttribute(XmNdefaultButton, cancelbutton->getid());
}

void
XMSelection::DefaultToHelp()
{
  SetAttribute(XmNdefaultButton, helpbutton->getid());
}

void
XMSelection::DefaultToApply()
{
  SetAttribute(XmNdefaultButton, applybutton->getid());
}


/*
** Implementation of functions from class XMPromptDialog
*/

void
XMPromptDialog::Create(const char *n, Widget parent, 
			       const char *prompt,
			       void (*cb)(XMWidget *,
					  XtPointer, XtPointer),
			       XtPointer cbd,
			       ArgList list, Cardinal argcount)
{
  id = XmCreatePromptDialog(parent, name,
			    list, argcount);
  GetButtons();
  if(prompt) SetLabelString(prompt);
  helpbutton->Disable();
  if(cb) AddDoCallback(cb, cbd);
  Manage();
  XtPopup(XtParent(id), XtGrabNone);  
}

XMPromptDialog::XMPromptDialog(const char *n, Widget parent, 
			       const char *prompt,
			       void (*cb)(XMWidget *,
					  XtPointer, XtPointer),
			       XtPointer cbd,
			       ArgList list, Cardinal argcount) :
  XMSelection(n) 
{
  Create(n, parent, prompt, cb, cbd, list, argcount);
}

XMPromptDialog::XMPromptDialog(const char *n, XMWidget &parent, 
			       const char *prompt,
			       void (*cb)(XMWidget *,
					  XtPointer, XtPointer) ,
			       XtPointer cbd ,
			       ArgList list , Cardinal argcount ) :
  XMSelection(n)
{
  Create(n, parent.getid(), prompt, cb, cbd, list, argcount);
}

XMPromptDialog::XMPromptDialog(Widget w) : 
  XMSelection(w)
{ }

void
XMPromptDialog::GetButtons()
{
  Widget w;
  w = XmSelectionBoxGetChild(id, XmDIALOG_HELP_BUTTON);
  helpbutton = new XMPushButton(w);
  
  w = XmSelectionBoxGetChild(id, XmDIALOG_CANCEL_BUTTON);
  cancelbutton = new XMPushButton(w);
  
  w = XmSelectionBoxGetChild(id, XmDIALOG_APPLY_BUTTON);
  applybutton = new XMPushButton(w);
  
  w = XmSelectionBoxGetChild(id, XmDIALOG_OK_BUTTON);
  okbutton = new XMPushButton(w);
}



/*
** Functions that implement XMPrompter:
*/
void
XMPrompter::Create(const char *name, 
		   const char *prompt, 
		   XtPointer calldata)
{
  /* Register the callbacks: */

  ok.Register(this, XmNokCallback,&XMPrompter::OkCallback, calldata);
  apply.Register(this, XmNapplyCallback, &XMPrompter::ApplyCallback, calldata);
  cancel.Register(this, XmNcancelCallback, 
		  &XMPrompter::CancelCallback, calldata);
  help.Register(helpbutton, XmNactivateCallback, &XMPrompter::HelpCallback, 
		calldata);

  /* Register the default help text: */


  help_info.name = "Prompt_Help";
  help_info.dialog    = NULL;
  help_info.text      = prompter_help;

  // Enale the help button:

  helpbutton->Enable();  
}

/*
** Functional Description:
**   XMPrompter::XMPrompter:
**        This method constructs a new prompter object.  The object is 
**        selfcontained in the sense that callbacks are mapped to method
**        functions.  There are two instantiators that work essentially
**        the same way.   The only difference is whether or not the parent
**        widget is an objectified widget or an Xt Widget handle.
** Formal Parameters:
**     char *name:
**        Name of the widget.
**     XMWidget &parent
** or  Widget    parent:
**        Identifies the parent widget for this object.
**     char *prompt:
**        Points to the string which specifies the prompt, if non NULL.
**     XtPointer calldata:
**        Optional User data which will be passed to callback routines.
*/
XMPrompter::XMPrompter(const char *name, Widget parent, 
		       const char *prompt, 
		       XtPointer calldata) :
		       XMPromptDialog(name, parent, prompt),
		       ok(this),
		       apply(this),
		       cancel(this),
		       help(this)
{

  Create(name, prompt, calldata);
}
XMPrompter::XMPrompter(const char *name, XMWidget &parent, const char *prompt,
		       XtPointer calldata) :
		       XMPromptDialog(name, parent, prompt),
		       ok(this),
		       apply(this),
		       cancel(this),
		       help(this)
{
  Create(name, prompt, calldata);
}

/*
** Functional Description:
**   XMPrompter::~XMPrompter:
**      This method is used to destroy a prompter.  All of the callbacks
**      must be unregistersed, and the help widget, if it exists must
**      also be deleted.
*/
XMPrompter::~XMPrompter()
{
  ok.UnRegister();
  apply.UnRegister();
  cancel.UnRegister();
  help.UnRegister();

  if(help_info.dialog) delete help_info.dialog;

}

/*
** Functional Description:
**   XMPrompter::SetHelpText:
**     This function allows the user to set a dialog specific help text
**     in for the prompter object.
** Formal Parameters:
**   char **newhelp
**    List of null terminated text strings to display in the dialog.
**    The last string pointer should be NULL.
*/
void XMPrompter::SetHelpText(const char **new_help)
{
  help_info.text = new_help;
}
/*
** Functional Description:
**    XMPrompter::RevertHelpText:
**      Revert to standard 'generic' help text.
*/
void XMPrompter::RevertHelpText()
{
  help_info.text = prompter_help;
}





/*
** Method Description:
**    FileListDialog::GetDirectory - Get the current search directory for
**                                   a file selectionbox.
** Returns:
**   Pointer to an XtAlloc'd string which must be freed by the caller.  The
**   string contains the ASCIZ directory path string.
*/

char *XMFileListDialog::GetDirectory() {
  XmString dir_compound;
  char     *directory;

  GetAttribute(XmNdirectory, &dir_compound);

  XmStringGetLtoR(dir_compound, 
		  const_cast<char*>(XmSTRING_DEFAULT_CHARSET), &directory);
  XmStringFree(dir_compound);
  return directory;
}

/*
** Method Description:
**    FileListDialog::GetFileMask - Get the file search mask.
** Returns:
**  pointer to an XtMalloc'd ASCIZ string which must be released by the caller
*/
char *XMFileListDialog::GetFileMask()
{
  XmString mask_compound;
  char    *mask;

  GetAttribute(XmNdirMask, &mask_compound);
  XmStringGetLtoR(mask_compound, 
		  const_cast<char*>(XmSTRING_DEFAULT_CHARSET), &mask);
  XmStringFree(mask_compound);
  return mask;
}

/*
** Method Description:
**    FileListDialog::GetFullSearchString() - Returns the full directory/search
**                                            mask string.
** Returns:
**   char * pointer to a null terminated string containing the mask.
**   The string was XtMalloc'd and must therefore be XtFree'd by the caller
**   after use.
**/
char *XMFileListDialog::GetFullSearchString()
{
  XmString search_compound;
  char     *mask;

  GetAttribute(XmNdirSpec, &search_compound);
  XmStringGetLtoR(search_compound, 
		  const_cast<char*>(XmSTRING_DEFAULT_CHARSET), &mask);
  XmStringFree(search_compound);
  return mask;
}

/*
** Method Description:
**   XMCustomDialog::CreateDialog:
**     This method create a skeleton custom dialog.  Since this class
**     is a superclass for all user defined custom dialogs, nothing is
**     actually managed or popped up until Manage() is called.
** Formal Parameters:
**   char *name:
**     Name of the dialog... will be applied to the dialog shell.
**   Widget parent:
**     The dialog's parent.
**   char *title:
**     Contains the title that will be displayed on the banner of the dialog
**     shell.
**   ArgList l:
**     List of resource argument which will be applied to the shell.
**   Cardinal num_args:
**     Count of resource argumnts which will be applied to the shell.
*/
void XMCustomDialog::CreateDialog(const char *name, Widget parent, 
				  const char *title,
				  ArgList l, Cardinal num_args)
{

  /* Create the shell */

  id = XtVaCreatePopupShell(name, xmDialogShellWidgetClass, parent,
			    XmNtitle, title,
			    XmNdeleteResponse, XmDESTROY,
			    NULL);

  /*
  **  For motif custom dialogs we'll turn off the controls on the title bar.
  **  This will prevent closes from accidently destroying our widget 
  **  Heirarchy. 
  */
  SetupBorders(id);

  if( (num_args > 0) && (l != NULL)) XtSetValues(id, l, num_args);

  /*  Create a form which is used to allow the modalization to be controlled */

  shell_child = new XMForm("Dialog_Form", id);

  /* Create the pane manager widget which manages the dialog elements */


  Arg pane_args[2];
  XtSetArg(pane_args[0], XmNsashWidth, 1);
  XtSetArg(pane_args[1], XmNsashHeight,1);

  top_manager= new XMPanedWindow("Dialog_toplevel",
				 *shell_child, pane_args, 2);
  /* Glue the pane to all corners of the form:  */

  shell_child->SetTopAttachment(*top_manager, XmATTACH_FORM);
  shell_child->SetLeftAttachment(*top_manager, XmATTACH_FORM);
  shell_child->SetRightAttachment(*top_manager, XmATTACH_FORM);
  shell_child->SetBottomAttachment(*top_manager, XmATTACH_FORM);

  /* Create the sub region managers: */

  work_area  = new XMForm("Dialog_work_area",
			  *top_manager);
  action_area= new XMRowColumn("Dialog_action_area",
			       *top_manager);


  /* Put the buttons in the action area widget */
 
  action_area->SetOrientation(XmHORIZONTAL);
  action_area->SetPacking(XmPACK_COLUMN);

  Ok    = new XMPushButton("Ok", *action_area);
  Apply = new XMPushButton("Apply", *action_area);
  Cancel= new XMPushButton("Cancel", *action_area);
  Help  = new XMPushButton("Help", *action_area);


}

/*
  Pop the widget down:
*/

void XMCustomDialog::popDown()
{
  XtPopdown(id);
}

/* 
   Pop the widget up:
*/
void 
XMCustomDialog::popUp()
{
  XtPopup(id, XtGrabNone);

}


/*
** Method Description:
**    XMCustomDialog::Manage:
**      This function manages the dialog. This involves managing the
**      action_area, setting it's minimum/maximum pane size to actual
**      pane size, and then managing the form, the pane and popping up the
**      shell.
*/
void XMCustomDialog::Manage()
{
  XtWidgetGeometry size;

  /* Manage the action area and set up the pane height, turning off sashes */

  action_area->Manage();


  
  shell_child->SetTopAttachment(*top_manager, XmATTACH_FORM);
  shell_child->SetLeftAttachment(*top_manager, XmATTACH_FORM);
  shell_child->SetRightAttachment(*top_manager, XmATTACH_FORM);
  shell_child->SetBottomAttachment(*top_manager, XmATTACH_FORM);

  XtQueryGeometry(action_area->getid(), NULL, &size);
  top_manager->MaxPaneSize(*action_area, size.height);
  top_manager->MinPaneSize(*action_area, size.height);

  /*  Manage the work area, and the paned window: */

  work_area->Manage();
  top_manager->Manage();
  shell_child->Manage();

  /* Pop up the shell: */

  XtPopup(id, XtGrabNone);
  
}

/*
** Method Description:
**    XMCustomDialog::UnManage:
**       This method unmanages a custom dialog.
**       It does this by unmanaging the manager widgets and
**       popping down the shell
*/
void XMCustomDialog::UnManage()
{
  shell_child->UnManage();
  XtPopdown(id);
}

/*
** Functional Description:
**   XMPrompter::Perform:
**     This function is meant to be overridden by the person specializing
**     this dialog.  The override should perform the application specific
**     task and then return True if the task was properly performed or
**     False if not.  The boolean is used by the OkCallback to decide if
**     it's necessary to dismiss the dialog.
** Formal Parameters:
**   XMWidget *wid:
**    This pointer actually.
**   XtPointer userd:
**    Application specific callback data.
**   XtPointer calld:
**    Actually points to an XmSelectionBoxCallbackStruct which 
**    describes what's happening.
** Returns:
**   True    - If action was successfully performed.
**   False   - If action was not successfully performed.
*/
Boolean XMPrompter::Perform(XMWidget *wid, XtPointer userd, XtPointer calld)
{
  XmSelectionBoxCallbackStruct *info = (XmSelectionBoxCallbackStruct *)calld;

  assert((info->reason == XmCR_APPLY) || 
	 (info->reason == XmCR_OK));  // This for debugging.

  return True;	   	              // default does nothing but Ok dismisses
}

/*
** Functional Description:
**    XMPrompter::OkCallback:
**      This virtual function is called in response to the 
**      Ok Button.  In most cases, the default action should be sufficient,
**      however, the function is virtual to allow easy replacement without
**      worry about handling generic dialogs.  The Default action is
**      to call Perform to get the appropriate stuff done.  If Perform
**      returns True, then we unmanage ourself.
**    XMPRompter::ApplyCallback:
**      Virtual function called in response to the Apply button. 
**      Normallly the default action,  to call Perform, will be sufficient.
**      The function is declared virtual to allow hassle free overrides.
** Formal Parameters:
**   XMWidget *wid:
**     Pointer to the widget (this)
**   XtPointer userd:
**     User data that we're supposed to pass on to perform.
**   XtPointer calld:
**     Callback specific data from motif which we pass into Perform.
**     It's really pointing to an XmSelectionBoxCallbackStruct.
*/
void XMPrompter::OkCallback(XMWidget *wid, XtPointer userd, XtPointer calld)
{
  if(Perform(wid, userd, calld))
    UnManage();
}
void XMPrompter::ApplyCallback(XMWidget *wid, XtPointer userd, XtPointer calld)
{
  Perform(wid, userd, calld);
}

/*
** Functional description:
**  XMPrompter::CancelCallback:
**    This function is called for a cancel hit.  Normally, the default
**    action, to unmanage the dialog should be sufficient.  However
**    the function is virtual to allow easy override in case there are
**    special requirements.
** Formal Parameters:
**    As for all callbacks.
*/
void XMPrompter::CancelCallback(XMWidget *wind, XtPointer userd, 
				XtPointer calld)
{
  UnManage();
}

/*
** Functional Description:
**   XMPrompter::HelpCallback:
**     This function is called whenever the help button is struck.
**     The default is to display a message/help dialog box whith the text
**     that was specified in the lsat SetHelpText call or the default
**     help text.
*/
void XMPrompter::HelpCallback(XMWidget *wind, XtPointer userd, XtPointer calld)
{
  Xamine_display_help( this, &help_info, NULL);
}


/*
** Implementation of functions from class XMSelectionDialog
*/

void 
XMSelectionDialog::Create(const char *n, Widget parent, const char *prompt,
			  void (*cb)(XMWidget *,
				     XtPointer, XtPointer),
			  XtPointer cbd,
			  ArgList list,
			  Cardinal argcount)
{
  id = XmCreateSelectionDialog(parent, name,
			       list, argcount);
  GetButtons();
  if(prompt) SetLabelString(prompt);
  helpbutton->Disable();
  if(cb) AddDoCallback(cb, cbd);
  Manage();
  XtPopup(XtParent(id), XtGrabNone);
}
XMSelectionDialog::XMSelectionDialog(const char *n, Widget parent, 
				     const char *prompt ,
				     void (*cb)(XMWidget *,
						XtPointer, XtPointer) ,
				     XtPointer cbd ,
				     ArgList list , 
				     Cardinal argcount ) :
  XMSelection(n)
{
  Create(n, parent, prompt, cb, cbd, list);
}

XMSelectionDialog::XMSelectionDialog(const char *n, XMWidget &parent, 
				     const char *prompt ,
				     void (*cb)(XMWidget *,
						XtPointer, XtPointer) ,
				     XtPointer cbd,
				     ArgList list, 
				     Cardinal argcount) :
  XMSelection(n) 
{

  Create(n, parent.getid(), prompt, cb, cbd, list);

}

XMSelectionDialog::XMSelectionDialog(Widget w) : 
  XMSelection(w)
{ }

void
XMSelectionDialog::SetVisibleItemCount(Cardinal num_visible)
{
  SetAttribute(XmNvisibleItemCount, num_visible);
}

void
XMSelectionDialog::RestrictChoices()
{
  SetAttribute(XmNmustMatch, True);
}

void
XMSelectionDialog::NoRestrictChoices()
{
  SetAttribute(XmNmustMatch, (XtArgVal)False);
}

Callback_data*
XMSelectionDialog::AddNoMatchCallback(void (*cb)(XMWidget *,
						 XtPointer, XtPointer),
				      XtPointer client_data ) 
{
  return AddCallback(XmNnoMatchCallback, cb, client_data);
}

/*
** Method Description:
**   SetSelectionList   - This function sets the selection list for a 
**                        SelectionDialog object. 
**                        In the special case that list_count == 0,  
**                        We set the selection list pointer to NULL and
**                        zero the count.
** Formal Parameters:
**   Cardinal list_count:
**      The number of list items.
**   char **selections:
**      The set of selections to enter in the box.
*/
void XMSelectionDialog::SetSelectionList(Cardinal list_count, 
					 const char **selections)
{
  XmString *list;		/* The LtoR'd strings. */
  XmString *l;
  const char     **s;

  /* Handle special list_count == 0 case */
  /* Transform the chars to compound strings: */
  
  if(list_count == 0) {
    list = NULL;
  }
  else {

    l = list = (XmString *)XtMalloc(list_count*sizeof(XmString));
    s = selections;
    for(int i = 0; i < list_count; i++) {
      char* stripped = new char[strlen(*s) + 1];
      StripNonPrinting(stripped, *s);
      *l++ = XmStringCreateSimple(stripped);
      s++;
      delete []stripped;
    }
  }
  /* Set the resources: */

  XtVaSetValues(id, XmNlistItems,     list,
		    XmNlistItemCount, list_count,
		NULL);
    
  if(list_count) {
    l = list;
    for(int i = 0; i < list_count; i++)
      XmStringFree(*l++);
    XtFree((char *)list);
  }
}


/*
** Default help text for selection box widgets.:
*/

static const char *SelectionDefaultHelp[] = {
  "  You are being asked to select an item from the list of choices\n",
  "in the list box part of the widget.  You can select an item either by\n",
  "clicking on it with the mouse or by typing it into the text input\n",
  "area below the list box.  Once you have made your selection, you should\n",
  "click a button in the action area at the bottom of the dialog where: \n\n",
  "    Ok          - Will accept your choice and dismiss the dialog\n",
  "    Apply       - Will accept your choice and leave the dialog displayed\n",
  "    Cancel      - Will dismiss the dialog without any action\n",
  "    Help        - Will display this text\n",
  NULL
};


/*
** Functions that implement the XMSelector dialog class.
*/

void
XMSelector::Create(const char *name,  const char *prompt, 
		   XtPointer cbd, ArgList list, Cardinal argcount)
{
  /* Register the callbacks... */


  okbuttonCB.Register(this, XmNokCallback, &XMSelector::OkCb, cbd);
  applybuttonCB.Register(this, XmNapplyCallback, &XMSelector::ApplyCb, cbd);
  cancelbuttonCB.Register(this, XmNcancelCallback, &XMSelector::CancelCb, cbd);
  nomatchCB.Register(this, XmNnoMatchCallback, &XMSelector::NoMatchCb, cbd);
  helpbuttonCB.Register(helpbutton, XmNactivateCallback, &XMSelector::HelpCb, cbd);

  /* Set up the help button with the default help text.: */

  helpbutton->Enable();
  helpinfo.name = "SelectionHelp";
  helpinfo.dialog = (XMInformationDialog *)NULL;
  helpinfo.text   = SelectionDefaultHelp;

  /* Set the initial list values: */

  SetupList();
}

/*
** Functional Description:
**   XMSelector::XMSelector:
**      This method constructs an instance of a self contained selector
**      dialog.  A selector dialog is used to choose one item from
**      a list of choices.  The user's choice can be confined to those in the
**      list or expanded beyond those in the list. The selector is
**      Somewhat incomplete and minimally, the client will usually need
**      to supersede the SetupList and Perform methods.
** Formal Parameters:
**    char *n:
**       Name to be given to the dialog widget.
**    Widget parent:
**    XMWidget &parent:
**       Identifies the parent of this dialog.
**    char *prompt:
**       Prompt string (put near the type-in box).
**    XtPointer cbd:
**       Data to pass to various callbacks.
**    ArgList list:
**       List of resources which override the default resource sets of the
**       widget.
**    Cardinal argcount:
**       Number of override resource value pairs in list.
*/
XMSelector::XMSelector(const char *name, Widget parent, 
		       const char *prompt, 
		       XtPointer cbd, ArgList list, Cardinal argcount) :
		       XMSelectionDialog(name, parent, prompt, NULL,
					 cbd, list, argcount),
		       okbuttonCB(this),
		       applybuttonCB(this),
		       cancelbuttonCB(this),
		       helpbuttonCB(this),
		       nomatchCB(this)
{
  Create(name, prompt, cbd, list, argcount);

}
XMSelector::XMSelector(const char *name, XMWidget &parent, 
		       const char *prompt,
		       XtPointer cbd, ArgList list, Cardinal argcount) :
		       XMSelectionDialog(name, parent, prompt, NULL,
					 cbd, list, argcount),
		       okbuttonCB(this),
		       applybuttonCB(this),
		       cancelbuttonCB(this),
		       helpbuttonCB(this),
		       nomatchCB(this)
{
  Create(name, prompt, cbd, list, argcount);

}
/*
** Functional Description:
**   XMSelector::~XMSelector:
**     This method is responsible for destroying an instance of the
**     selector.  The main thing that we need to do is to release the
**     dynamic storage associated with the callbacks by unregistering them.
*/
XMSelector::~XMSelector()
{
  okbuttonCB.UnRegister();
  applybuttonCB.UnRegister();
  cancelbuttonCB.UnRegister();
  nomatchCB.UnRegister();
  helpbuttonCB.UnRegister();

}

/*
** Functional Description:
**    XMSelector::SetupList:
**      This function sets up the initial display list.  This is the
**      default method which is intended to be overridden by the derived
**      classes.  The default method does nothing.
*/
void XMSelector::SetupList()
{
  
}

/*
** Functional Description:
**    XMSelector::SetHelpText:
**       This function replaces the help text with user defineable help text
**       The help text is a list of string pointers to null terminated strings
**       terminated by a null pointer (see the default help text for an
**       example.
** Formal Parameters:
**    char **newhelp:
**      New help text.
*/
void XMSelector::SetHelpText(const char **newhelp)
{
  helpinfo.text = newhelp;
}
/*
** Functional Description:
**   XMSelector::RevertHelpText:
**     This method restores the default help text.
*/
void XMSelector::RevertHelpText()
{
  helpinfo.text = SelectionDefaultHelp;
}

/*
** Functional Descriptions:
***   XMSelector::OkCb:
**      Callback for the Ok button of the dialog.
**    XMSelector::NoMatchCb:
**      Callback for any click with no match.
**    XMSelector::ApplyCb:
**      Callback for apply button.
**    XMSelector::CancelCb:
**      Callback for cancel button.
**    XMSelector::HelpCb:
**      Callback for help button.
**
**   These all take relatively default actions which can be overridden by
**   the deriver.  Usually, the deriver will override, Perform, however
**   since the other callbacks give a standardized behavior to the dialog
**   widget.
** Formal Parameters:
**   XMWidget *wid:
**     The widget which invoked the callback.
**   XtPointer userd:
**     user specific call data.
**   XtPointer cd:
**     Callback Specific data in this case a pointer to an 
**     XmSelectionBoxCallbackStruct
*/
void XMSelector::OkCb(XMWidget *wid, XtPointer userd, XtPointer cd)
{
  XmSelectionBoxCallbackStruct *calldata = (XmSelectionBoxCallbackStruct *)cd;

  if(Perform(wid, userd, 
	     calldata->reason, calldata->value, calldata->length))
    UnManage();
  
}
void XMSelector::ApplyCb(XMWidget *wid, XtPointer userd, XtPointer cd)
{
  XmSelectionBoxCallbackStruct *calldata = (XmSelectionBoxCallbackStruct *)cd;

  Perform(wid, userd,
	  calldata->reason, calldata->value, calldata->length);
}
void XMSelector::NoMatchCb(XMWidget *wid, XtPointer userd, XtPointer cd)
{
  XmSelectionBoxCallbackStruct *calldata = (XmSelectionBoxCallbackStruct *)cd;

  if(Perform(wid, userd, 
	     calldata->reason, calldata->value, calldata->length))
    UnManage();
  
}

void XMSelector::CancelCb(XMWidget *wid, XtPointer cd, XtPointer ud)
{
  UnManage();
}

void XMSelector::HelpCb(XMWidget *cb, XtPointer cd, XtPointer ud)
{
  Xamine_display_help(cb, (XtPointer)&helpinfo, ud);
}

/*
** Functional Description:
**   XMSelector::Perform:
**     This method is intended to be overridden by the deriver.
**     In a typical case, the deriver will override SetupLIst and
**     Perform to get everything needed to do application sensitive list
**     processing.
** Formal Parameters:
*   XMWidget *wid:
**    The widget invoking us, most often this is the object's this pointer.
**  XtPointer cd:
**    User supuplied call data.
**  int reason:
**    Why the callback is called, can be any of:
**     XmCR_APPLY:
**       The apply button was pressed.
**     XmCR_OK:
**       The Ok button was clicked.
**     XmCR_NO_MATCH:
**       There was an accept with no match in the field.  Note that
**       in this case we treat the return value the same as for an accept.
**     XMString value:
**       The text representing the user's choice.
**     int size:
**       number of characters in the user's choice.
** Return Value:
**   True:
**     Valid action was taken.  The dialog is dismissed if OK or NO_MATCH.
**   False:
**     No valid action was taken, the dialog remains if OK or NO_MATCH.
*/
Boolean XMSelector::Perform(XMWidget *wid, XtPointer cd,
			   int reason, XmString value, int size)
{
  return True;
}


/*
** Implementation of functions from class XMFileListDialog
*/


void
XMFileListDialog::Create(const char *n, Widget parent, 
			 const char  *directory,
			 void (*cb)(XMWidget *,
				    XtPointer, XtPointer),
			 XtPointer cbd,
			 ArgList list,
			 Cardinal argcount)
{
  id = XmCreateFileSelectionDialog(parent, name,
				   list, argcount);
  GetButtons();
  helpbutton->Disable();
  SetLabelString("Filename: ");
  if(cb) AddDoCallback(cb, cbd);
  
  /* Set the search path directory: */
  
  DoSearch(directory);
  Manage();
  XtPopup(XtParent(id), XtGrabNone);
}
			 
XMFileListDialog::XMFileListDialog(const char *n, Widget parent, 
				   const char* directory,
				   void (*cb)(XMWidget *,
					      XtPointer, XtPointer),
				   XtPointer cbd,
				   ArgList list , 
				   Cardinal argcount ) :
  XMSelection(n)
{
  Create(n, parent, directory, cb, cbd, list, argcount);
}

XMFileListDialog::XMFileListDialog(const char *n, XMWidget &parent, 
				   const char *directory,
				   void (*cb)(XMWidget *,
					      XtPointer, XtPointer),
				   XtPointer cbd,
				   ArgList list , 
				   Cardinal argcount) :
  XMSelection(n)
{
  Create(n, parent.getid(), directory, cb, cbd, list, argcount);

}

XMFileListDialog::XMFileListDialog(Widget w) : XMSelection(w) { }

void
XMFileListDialog::DoSearch(XmString dir)
{
  SetAttribute(XmNdirMask, &dir);
  XmFileSelectionDoSearch(id, dir);
}

void
XMFileListDialog::DoSearch(const char *dir)
{
  char* pDir = const_cast<char*>(dir);
  XmString d;
  d = XmStringCreateLtoR(const_cast<char*>(pDir), const_cast<char*>(XmSTRING_DEFAULT_CHARSET));
  XmFileSelectionDoSearch(id, d);
  XmStringFree(d);
}

void
XMFileListDialog::DoSearch()
{	/* Do search on current mask. */
  XmString dirmask;	/* Will hold the directory search mask. */
  GetAttribute(XmNdirMask, &dirmask);
  
  XmFileSelectionDoSearch(id, dirmask);
  XmStringFree(dirmask); /* Get rid of dirmask. */
}

void
XMFileListDialog::SetLabelString(const char *txt)
{
  XmString s = XmStringCreateLtoR(const_cast<char*>(txt), const_cast<char*>(XmSTRING_DEFAULT_CHARSET));
  XtVaSetValues(XmFileSelectionBoxGetChild(id, 
					   XmDIALOG_SELECTION_LABEL),
		XmNlabelString, s,
		NULL);
  XmStringFree(s);
}

void
XMFileListDialog::SetFilterString(const char *txt)
{
  XmString s = XmStringCreateLtoR(const_cast<char*>(txt), const_cast<char*>(XmSTRING_DEFAULT_CHARSET));
  SetAttribute(XmNfilterLabelString, s);
  XmStringFree(s);
}

void
XMFileListDialog::SetFileTypes(unsigned char fileset)
{
  SetAttribute(XmNfileTypeMask, fileset);
}

void
XMFileListDialog::RestrictChoices()
{
  SetAttribute(XmNmustMatch, True);
}

void
XMFileListDialog::NoRestrictChoices()
{
  SetAttribute(XmNmustMatch, (XtArgVal)False);
}

Callback_data*
XMFileListDialog::AddNoMatchCallback(void (*cb)(XMWidget *,
						XtPointer, XtPointer),
				     XtPointer client_data)
{
  return AddCallback(XmNnoMatchCallback, cb, client_data);
}

void
XMFileListDialog:: GetButtons() 
{
  helpbutton = new 
    XMPushButton(XmFileSelectionBoxGetChild(id,
					    XmDIALOG_HELP_BUTTON));
  cancelbutton = new
    XMPushButton(XmFileSelectionBoxGetChild(id,
					    XmDIALOG_CANCEL_BUTTON));
  okbutton = new
    XMPushButton(XmFileSelectionBoxGetChild(id,
					    XmDIALOG_OK_BUTTON));
  applybutton = new
    XMPushButton(XmFileSelectionBoxGetChild(id,
					    XmDIALOG_APPLY_BUTTON));
}


/*
** The methods in the pages which follow implement the functions of the
** XMFileSelector dialog.
*/

static const char *FileSelectorHelp[] = // Default help text for file selector.
{
  "  You are being prompted a filename.  If you know the complete name of\n",
  "the file, you can type it in at the text box at the bottom of the work\n",
  "area of the dialog.  If you don't know the name of the file then you can\n",
  "use the facilities of the file selection box to browse the directory\n",
  "tree to hunt for a suitable file name.\n",
  "    The left list is the set of directories one level below the current\n",
  "directory.  There's also a ",
  ".. for the next level up.  ",
  "To move into one\n",
  "of these directories double click on the directory name in the left box\n",
  "   The text type-in at the top of the work area allows you to type in\n",
  "an arbitrary filter string.  A filter string consists of a directory\n",
  "specification and a file search string which can contain wild card\n",
  "characters (and usually does to be useful).  Typing in a new filter\n",
  "string and hitting return or clicking on the Filter button will move you\n",
  "to the appropriate directory and display the files matching the search\n",
  "string in the right hand list\n",
  "   The right list contains the set of files in the current directory\n",
  "which match the search string.  To select a file you can click on it\n",
  "Double clicking is identical to a click followed by clicking the Ok \n",
  "button.\n",
  "   The buttons at the bottom of the dialog perform the following actions\n",
  "when clicked:\n\n",
  "     Ok      - Accepts the selection and dismisses the dialog.\n",
  "     Filter  - Re-applies the search string in the current directory\n",
  "     Cancel  - Dismisses the dialog without accepting any filename.\n",
  "     Help    - Displays this help text\n",
  NULL
};

/*
 * Functions that implement the XMFileSelector
 */
void
XMFileSelector::Create(const char *n, Widget parent, 
		       XtPointer calld,
		       const char *directory)
{
  /* Register the callbacks, note that apply is usurped for the filter 
  ** function (Motif's decision, not mine). 
  */

  okcb.Register(this, XmNokCallback, &XMFileSelector::FilterCb, calld);
  nomatchcb.Register(this, XmNnoMatchCallback, // Requires XmNmustMatch to be
		     &XMFileSelector::NomatchCb, calld);	       // True to invoke.
  filtercb.Register(this, XmNapplyCallback, &XMFileSelector::FilterCb, calld);
  cancelcb.Register(this, XmNcancelCallback, &XMFileSelector::CancelCb, calld);
  helpcb.Register(this, XmNhelpCallback, &XMFileSelector::HelpCb, calld);

  /* Set up the help info data structure */

  helpinfo.name = "File_Selection_Help";
  helpinfo.dialog = (XMInformationDialog *)NULL;
  RevertHelpText();

  helpbutton->Enable();
}

/*
** Functional Description:
**   XMFileSelector::XMFileSelector:
**     This method constructs an instance of a file selector dialog.
**     The file selector dialog is a self contained Motif widget which
**     allows the user to choose a filename.   Constructing an instance
**     consists of:
**       1. Calling the constructor for XMFileListDialog to build the widget.
**       2. Calling the constructors for the callback elements to set their
**          object fields.
**       3. Registering the callback functions.  The callback functions are
**          intended to be overridden, if necessary, by the client.
**       4. Setting up the default help text.  The SetHelpText method
**          allows a client to supply application specific help text. 
**
** Formal Parameters:
**   char *n:
**      Specifies the name of the dialog widget.  This will also appear
**      in the banner of the dialog.
**   Widget parent:
**   XMWidget &parent:
**      Specifies which widget or widget object will be the parent of the
**      dialog widget shell.
**   XtPointer ud:
**      User data passed to the callback methods.  This defaults to NULL.
**   char *directory:
**      Specifies the initial search directory.  Note that if the
**      search directory is not specified, then the XMFILE_DEFAULT_DIRMASK
**      is used instead.  This corresponds to the current working directory
**      as of the time Xamine was started.
** Note:
**    There is more than one constructor binding.
*/

XMFileSelector::XMFileSelector(const char *n, Widget parent, XtPointer calld,
			       const char *directory) :
		 XMFileListDialog(n, parent, directory),
		 okcb(this),
		 nomatchcb(this),
		 filtercb(this),
		 cancelcb(this),
		 helpcb(this)
{
  Create(n, parent, calld, directory);
}   
XMFileSelector::XMFileSelector(const char *n, XMWidget &parent,
			       XtPointer calld, 
			       const char *directory) :
		XMFileListDialog(n, parent, directory),
		okcb(this),
		nomatchcb(this),
		filtercb(this),
		cancelcb(this),
		helpcb(this)
{
  Create(n, parent.getid(), calld, directory);

}

/*
** Functional Description:
**   XMFileSelector::~XMFileSelector:
**      This method destroys the file selector widget.  The only dynamic
**      storage that exists is associated with the registerd callbacks.
**      Therefore we unregister all callbacks.
*/

XMFileSelector::~XMFileSelector()
{
  okcb.UnRegister();
  nomatchcb.UnRegister();
  filtercb.UnRegister();
  cancelcb.UnRegister();
  helpcb.UnRegister();
}   

/*
** Functional Description:
**   XMFileSelector::SetHelpText:
**     This function sets the help text associated with the widget.
** Formal Parameters:
**   char **text:
**     This is a list of character string pointers terminated by a null
**     pointer.  Each string itself is Null terminated.
*/
void XMFileSelector::SetHelpText(const char **text)
{
  helpinfo.text = text;
}   

/*
** FunctionalDescription:
**   XMFileSelector::RevertHelpText:
**     Reverts the help text to the default text.
*/

void XMFileSelector::RevertHelpText()
{
  helpinfo.text = FileSelectorHelp;
}   


/*
** The functions which follow are hooked in to the callbacks.
** While they can be overridden, they provide default behavior which is
** in keeping with the Motif style guide usage of these widgets.  It's best
** to only override the Perform method which is defined as a NOP so that
** the base class is testable.
*/

/*
** Functional Description:
**   Boolean Perform:
**     This function takes application dependent action on accepted filenames.
**     This function is normally overridden in classes derived from the
**     XMFileSelector with an application specific method.
**     
** Parameters:
**   XMWidget *wid:
**     Pointer to the widget object which invokes us...usually this dialog.
**   XtPointer ud:
**     User supplied data supplied at constructor time.
**   char *filename:
**     The name of the file which was received from the user.
**   int reason:
**     The callback reason which can be either of:
**        XmCR_OK       - Ok button was pressed.
**        XmCR_NO_MATCH - Chooser in must match mode and file entered not on
**                        list.
**        XmCR_APPLY    - Is only possible if the FilterCb method is
**                        overridden with a method which calls Perform or
**                        OkCb.
**     
** Returns:
**   True   - If the required function was performed, and the dialog
**            can be dismissed if appropriate.
**   False  - If the client would like the dialog to remain.
*/
Boolean XMFileSelector::Perform(XMWidget *wid, XtPointer ud, 
				char *filename, int reason)
{
  return True;			// Default to No-Op all done with dialog.
}

/*
** Functional Description:
**   XMFileSelector::OkCb:
**     Method called when the Ok button is clicked.  We repackage the
**     filename string as an ASCIZ C string and call the Perform method.
**     If Perform returns true, then we unmanage the dialog.  The user must
**     explicitly destroy the dialog since:
**       a) We don't know if the dialog is dynamically allocated.
**       b) Even if it is dynamic, the user may want to cache it for later use.
** Formal Parameters:
**   XMWidget *wid:
**     This pointer usually.
**   XtPointer ud:
**     User data passed to perform without interpretation.
**   XtPointer cd:
**     Call data which is of the form of a pointer to an 
**     XmFileSelectionBoxCallbackStruct (whew).  Where value and length
**     describe the filename.
*/
void XMFileSelector::OkCb(XMWidget *wid, XtPointer ud, XtPointer cd)
{
  /* Pull the string out of the callback structure:  */

  XmFileSelectionBoxCallbackStruct *calldata = 
      (XmFileSelectionBoxCallbackStruct *)cd;

  char *filename;

  if(!XmStringGetLtoR(calldata->value, const_cast<char*>(XmSTRING_DEFAULT_CHARSET),
		      &filename)) {
    Xamine_error_msg(this,
	     "XMFileSelector::OkCb Unable to retreive file name string");
  }
  else {
    if(Perform(wid, ud, filename, calldata->reason)) {
      UnManage();
    }
    XtFree(filename);
  }
}

/*
** Functional Description:
**   XMFileSelector::NomatchCb:
**      Called when the user types in a filename not in the list.
**      In this case we really could just call *our* OkCb since
**      that routine passes the reason from the callback data rather
**      than assuming it's XmCR_OK
** Formal Parameters:
**   XMWidget *wid:
**     This pointer usually.
**   XtPointer ud:
**     User data passed to perform without interpretation.
**   XtPointer cd:
**     Call data which is of the form of a pointer to an 
**     XmFileSelectionBoxCallbackStruct (whew).  Where value and length
**     describe the filename.
*/

void XMFileSelector::NomatchCb(XMWidget *wid, XtPointer ud, XtPointer cd)
{
  XMFileSelector::OkCb(wid, ud, cd); // Make sure it's *our* OkCb that's called
}

/*
** Functional Description:
**   XMFileSelector::FilterCb:
**     This function is called when the filter button is pressed.
**     This function is a No-op because I believe that the filter button
**     has a callback attached by motif to do the actual filtering.
**     The callback is intended for any application specific processing
**     that might be required.
** Formal Parameters:
**   as for any callback
*/

void XMFileSelector::FilterCb(XMWidget *wid, XtPointer ud, XtPointer cd)
{
}   

/*
** Functional Description:
**   XMFileSelector::CancelCb:
**     This method is called when the cancel button is pressed.
**     The default action is to unmanage the dialog.
** Formal Parameters:
**   As for all callbacks, ignored in this case.
*/

void XMFileSelector::CancelCb(XMWidget *wid, XtPointer ud, XtPointer cd)
{
  UnManage();
}   

/*
** Functional Description:
**   XMFileSelector::HelpCb:
**     This function is called when the help button is pressed.  The
**     default action is to display a help dialog described by the
**     helpinfo attribute.  The text in this dialog can be altered
**     by clients using the SetHelpText method.  Further fancier stuff
**     can be done by superseding this method.
** Formal Parameters:
**   XMWidget *wid:
**    Usually the pointer to the help button.
**   XtPointer ud:
**    Userdata ignored.
**   XtPointer cd:
**    Call data ignored.
*/

void XMFileSelector::HelpCb(XMWidget *wid, XtPointer ud, XtPointer cd)
{
  Xamine_display_help(this, &helpinfo);
}   
   

/*
** The following pages define the self contained custom dialog box class
** method functions.  A self contained custom dialog box (XMCustomDialogBox)
** is derived from the XMCustomDialog in a manner which allows it to have
** callback methods rather than external functions as the button methods.
*/

/*
** We provide default help text for the custom dialog box, however because
** it is really difficult to generically describe a custom dialog box,
** the user will typically have to replace this text using SetHelpText.
*/
static const  char *custom_help[] = {
  "  This is a custom dialog box which was created by a programmer that was\n",
  "too lazy to supply detailed help text about what the dialog does.  Since\n",
  "the work area of the dialog box could be almost anything, I cannot be\n",
  "much of a help about what to do there.  What I can say, however is that\n",
  "after you've done what you need to do in the work area, you should select\n",
  "one of the following buttons from the action area:\n\n",
  "    Ok     - Generally accepts what ever you've done and removes the\n",
  "             dialog box assuming everything was done properly\n",
  "    Apply  - Generally the same as Ok, but leaves the dialog box up.\n",
  "    Cancel - Generally takes no action, but dismisses the dialog box\n",
  "    Help   - Causes this helpful message to appear.\n\n",
  "NOTE:  If you know who wrote this program, then bother the hell out of \n",
  "       that person to replace this help text with something more specific\n",
  "       to the application\n",
  NULL
};


/*
** Implementation of functions from class XMCustomDialog
*/

XMCustomDialog::XMCustomDialog(const char *name, XMWidget &parent, const char *title,
			       ArgList l, Cardinal num_args) : 
  XMWidget(name)
{ 
  CreateDialog(name, parent.getid(), title, l, num_args);
}

XMCustomDialog::XMCustomDialog(const char *name, Widget parent, 
			       const char *title,
			       ArgList l, Cardinal num_args) :
  XMWidget(name)
{
  CreateDialog(name, parent, title, l, num_args); 
}

XMCustomDialog::~XMCustomDialog() 
{
  delete Ok;
  delete Apply;
  delete Cancel;
  delete Help;
  delete action_area;
  delete work_area;
  delete top_manager;
  delete shell_child;
}

XMPanedWindow*
XMCustomDialog::TopManager() { return top_manager; }

XMRowColumn*
XMCustomDialog::ActionArea() { return action_area; }

XMForm*
XMCustomDialog::WorkArea()   { return work_area;   }

XMPushButton*
XMCustomDialog::ok()         { return Ok; }

XMPushButton*
XMCustomDialog::apply()      { return Apply;       }

XMPushButton*
XMCustomDialog::cancel()     { return Cancel;      }

XMPushButton*
XMCustomDialog::help()       { return Help;        }

Callback_data*
XMCustomDialog::AddDoCallback(void (*callback)(XMWidget *, XtPointer, 
					       XtPointer),
			      XtPointer user_data ,
			      Callback_data **okcb ) 
{ 
  Callback_data *okc;
  okc = Ok->AddCallback(callback, user_data);
  if(okcb != NULL) *okcb = okc;
  return Apply->AddCallback(callback, user_data);
} 

Callback_data*
XMCustomDialog::AddOkCallback(void (*callback)(XMWidget *, XtPointer, 
					       XtPointer),
			      XtPointer user_data)
{ return Ok->AddCallback(callback, user_data); }

Callback_data*
XMCustomDialog::AddApplyCallback(void (*callback)(XMWidget *, XtPointer,
						  XtPointer),
				 XtPointer user_data)
{ return Apply->AddCallback(callback, user_data); }

Callback_data*
XMCustomDialog::AddCancelCallback(void (*callback)(XMWidget *, XtPointer, 
						   XtPointer),
				  XtPointer user_data )
{ return Cancel->AddCallback(callback, user_data); }

Callback_data*
XMCustomDialog::AddHelpCallback(void (*callback)(XMWidget *, XtPointer, 
						 XtPointer),
				XtPointer user_data)
{ return Help->AddCallback(callback, user_data); }

void
XMCustomDialog::SetModal(unsigned char modality)
{ 
  shell_child->SetAttribute(XmNdialogStyle, modality); 
}




void
XMCustomDialogBox::Initialize()
{
  SetCallbacks();		// Register the callbacks
  InitializeHelp();		// And set the default help text.
}
/*
** Functional Description:
**   XMCustomDialogBox:
**      These functions instantiate a custom dialog box.  A custom dialog
**      box is a self contained dialog box with a work area filled in by the
**      box creator.  Therefore this class just provides some common behavior
**      and the client is expected to subclass an appropriate version for
**      The desired needs. 
** Formal Parameters:
**   char *name:
**     Name of the custom dialog box (actually top level shell name).
**   XMWidget &parent:
**     References the widget which will parent the widget tree that we're
**     creating.
**   char *title:
**     The text which will be placed in the banner of the dialog.
**   ArgList l:
**     List of resources which will be applied at widget creation time.
**   Cardinal num_args:
**     Number of arguments in l.
*/

XMCustomDialogBox::XMCustomDialogBox(const char *name,XMWidget &parent, const char *title,
				     ArgList l, Cardinal num_args) :
		   XMCustomDialog(name, parent, title, l, num_args),
		   OkCb(this),
		   ApplyCb(this),
		   CancelCb(this),
		   HelpCb(this)
{
  Initialize();
}   

XMCustomDialogBox::XMCustomDialogBox(const char* name, Widget parent, const char* title,
				     ArgList l, Cardinal num_args) :
		   XMCustomDialog(name, parent, title, l, num_args),
		   OkCb(this),
		   ApplyCb(this),
		   CancelCb(this),
		   HelpCb(this)
{
  Initialize();
}   


/*
** Functional Description:
**   XMCustomDialogBox::~XMCustomDialogBox
**     This method deletes a custom dialog box.  The only thing required
**     is to unregister all of the callbacks.
*/

XMCustomDialogBox::~XMCustomDialogBox()
{
  OkCb.UnRegister();
  ApplyCb.UnRegister();
  CancelCb.UnRegister();
  HelpCb.UnRegister();

}   

/*
** This page contains functions to manage and setup the help text in case
** the user (as they should) decides to override the default help text
*/

/*
** Functional Description:
**   XMCustomDialogBox::SetHelpText:
**     Overrides the default help text.
** Formal Parameters:
**   char **help:
**    Points to the help text string list.
*/

void XMCustomDialogBox::SetHelpText(const char **help)
{
  help_info.text = help;	// Set new help text.
}   

/*
** Functional Description:
**   XMCustomDialogBox::RevertHelpText:
**      Reverts to default help text.
*/

void XMCustomDialogBox::RevertHelpText()
{
  help_info.text = custom_help;
}   

/*
** This page contains callback functions. As for all of the 
** self contained dialogs, the Ok and Apply callbacks will call a Perform
** function which does the application specific stuff, while the 
** Ok and Apply callback functions take care of widget management.
** We also supply a help callback and a cancel callback which should be 
** sufficient for most of our needs.
*/

/*
** Functional Description:
**   XMCustomDialogBox::perform:
**     This virtual function must be overridden by a subclasser to make the
**     dialog do anything useful.  Default action is to dismiss the dialog
**     without taking any action (like Cancel).
** Formal Parameters:
**   XMWidget *wid:
**     Points to the widget that called us.  This will typically be the
**     appropriate button widget.
**   XtPointer cli:
**     This is the client data which was established on the registration of the
**     callback.  Currently this is one of XmCR_APPLY, if the apply button was
**     hit or XmCR_OK if the OK button was the one that was hit.
**   XtPointer call:
**     This is the call data which in the case of this class is a pointer to
**      XmPushButtonCallbackStruct.
** Returns:
**   True:
**      If the user's function was successfully accomplished.
**   False:
**      If the user's function was not accomplished.
*/
Boolean XMCustomDialogBox::Perform(XMWidget *wid, XtPointer cli, 
				   XtPointer call)
{
  return True;
}   

/*
** Functional Description:
**   XMCustomDialogBox::OkPressed:
**     This method is called when the Ok button is pressed.  It calls
**     Perform() and unmanages the dialog box if Perform returns True.
** Formal Parameters:
**   XMWidget *wid:
**     Points to the widget that called us.  This will typically be the
**     appropriate button widget.
**   XtPointer cli:
**     This is the client data which was established on the registration of the
**     callback.  Currently this is one of XmCR_APPLY, if the apply button was
**     hit or XmCR_OK if the OK button was the one that was hit.
**   XtPointer call:
**     This is the call data which in the case of this class is a pointer to
**      XmPushButtonCallbackStruct.
*/
void XMCustomDialogBox::OkPressed(XMWidget *wid, XtPointer cli, XtPointer call)
{
  if(Perform(wid, cli, call))
    UnManage();
}   

/*
** Functional Description:
**   XMCustomDialogBox::ApplyPressed:
**    Processes the Apply button.  We keep the dialog box displayed
**    no matter what happens.
** Formal Parameters:
**   XMWidget *wid:
**     Points to the widget that called us.  This will typically be the
**     appropriate button widget.
**   XtPointer cli:
**     This is the client data which was established on the registration of the
**     callback.  Currently this is one of XmCR_APPLY, if the apply button was
**     hit or XmCR_OK if the OK button was the one that was hit.
**   XtPointer call:
**     This is the call data which in the case of this class is a pointer to
**      XmPushButtonCallbackStruct.
*/
void XMCustomDialogBox::ApplyPressed(XMWidget *wid, XtPointer cli, 
				     XtPointer call)
{
  Perform(wid, cli, call);
}   

/*
** Functional Description:
**   XMCustomDialogBox::CancelPressed:
**     This method is called when the cancel button was pressed.  We just
**     UnManage the dialog.
** Formal Parameters:
**   XMWidget *wid:
**     Points to the widget that called us.  This will typically be the
**     appropriate button widget.
**   XtPointer cli:
**     This is the client data which was established on the registration of the
**     callback.  Currently this is one of XmCR_APPLY, if the apply button was
**     hit or XmCR_OK if the OK button was the one that was hit.
**   XtPointer call:
**     This is the call data which in the case of this class is a pointer to
**      XmPushButtonCallbackStruct.
*/

void XMCustomDialogBox::CancelPressed(XMWidget *wid, XtPointer cli, 
				      XtPointer call)
{
  UnManage();
}   

/*
** Functional Description:
**   XMCustomDialogBox::HelpPressed:
**     Called in response to the help button.  We display the help text
**     in a popup message dialog box.
** Formal Parameters:
**   XMWidget *wid:
**     Points to the widget that called us.  This will typically be the
**     appropriate button widget.
**   XtPointer cli:
**     This is the client data which was established on the registration of the
**     callback.  Currently this is one of XmCR_APPLY, if the apply button was
**     hit or XmCR_OK if the OK button was the one that was hit.
**   XtPointer call:
**     This is the call data which in the case of this class is a pointer to
**      XmPushButtonCallbackStruct.
*/
void XMCustomDialogBox::HelpPressed(XMWidget *wid, XtPointer cli, 
				    XtPointer call)
{
  Xamine_display_help(this, &help_info);
}

/*
** The functions on this page implement the private functions of the 
** XMCustomDialogBox class.   These functions are used by the
** constructors to provide common code paths for the typical constructor
** duties.
*/


/*
** Functional Description:
**   XMCustomDialogBox::InitializeHelp:
**     This function sets up the help client data for the first time.
*/

void XMCustomDialogBox::InitializeHelp()
{
  help_info.name   = "Custom Dialog Box";
  help_info.dialog = (XMInformationDialog *)NULL;
  help_info.text   = custom_help;
}

/*
** Functional Description:
**   XMCustomDialogBox::SetCallbacks:
**     This function registers the callback methods.
*/

void XMCustomDialogBox::SetCallbacks()
{
  OkCb.Register(Ok, XmNactivateCallback, &XMCustomDialogBox::OkPressed, 
		(XtPointer)XmCR_OK);
  ApplyCb.Register(Apply, XmNactivateCallback, &XMCustomDialogBox::ApplyPressed, 
		   (XtPointer)XmCR_APPLY);
  CancelCb.Register(Cancel, XmNactivateCallback, &XMCustomDialogBox::CancelPressed, NULL);
  HelpCb.Register(Help, XmNactivateCallback, &XMCustomDialogBox::HelpPressed, NULL);
}   








