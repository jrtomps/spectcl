/*
    This software is Copyright by the Board of Trustees of Michigan
    State University (c) Copyright 2008

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
**    Xamine -- NSCL display program.
** Abstract:
**    buttonreq.cc:
**      This file processes requests which manage the client button box.
**      A client button box is a Top level shell which contains a form that's
**      used to package a rectangular array of buttons.  Buttons can be
**      programmed by the user to prompt for additional information or just
**      to send a message to the client.
** Author:
**   Ron Fox
**   NSCL
**   Michigan State University
**   East Lansing, MI 48824-1321
*/

/*
** Include files:
*/
#include <config.h>
#include "XMShell.h"
#include "XMPushbutton.h"
#include "XMDialogs.h"

#include "dispshare.h"
#include "errormsg.h"
#include "dispwind.h"
#include "gateio.h"
#include "clientops.h"
#include "buttonreq.h"
#include "messages.h"
#include "buttondlg.h"
#include "panemgr.h"
#include "spcchoice.h"
#include "ptlist.h"
/*
** Local data:
*/

static ButtonBox *buttons = NULL; // This will be instantiated to a button box.
/*
** Local types:
*/

enum SpectrumType  {  StUndefined,
		       StOned,
		       StTwod
		       };

/*
** Forward declarations:
*/
static Logical      ButtonAvailable(ButtonSensitivity sensitivity, 
				    SpectrumType sptype);
static SpectrumType InitializeEvent(msg_XamineEvent &evt, int code);

static Boolean      AcceptSpectrum(XMWidget *wid, XtPointer userd, 
				   int reason, int specno);

/*
** Local class SimpleClientPushButton:
**    This section implements the functionality of a client push button.
**    Form and function are derived from the PushButton class.
**    The simple button just needs to remember it's code, label,
**    sensitivity state and availability.
*/
class SimpleClientPushButton : public PushButton {
  public:
    SimpleClientPushButton(ButtonBox *parent, ButtonDescription *bdef);
    virtual ~SimpleClientPushButton();
    ButtonDescription *GetButtonDef() { return &description; }
  protected:
    virtual void ButtonPresscb(XMWidget *wid, XtPointer userd,
			       XtPointer buttond);

    ButtonDescription description;
  private:
};

/*
** Functional Description:
**   SimpleClientPushButton::
**     Builds a simple client pushbutton.   Most of the work is done
**     by the superclass constructors.  We just have to store some extra
**     information away that's used by the callback to figure out what to
**     tell the user when the button is hit.
** Functional Description:
**   ButtonBox  *parent:
**     The Button box that'll own us.  Needed to get the owner widget.
**   ButtonDescription *bdef:
**     Describes the button being created.  Used to fill in some private fields
*/
SimpleClientPushButton::SimpleClientPushButton(ButtonBox *parent,
					       ButtonDescription *bdef) :
      PushButton(parent->GetButtonParent(),
		 (XtPointer)bdef->button_code)
{
  memcpy(&description, bdef, sizeof(ButtonDescription));
  SetLabel(bdef->label);
  if(bdef->sensitive) Enable();
  else                Disable();


}

/*
** Destructor just has to exist to be sure that the destructor chain fires off.
*/
SimpleClientPushButton::~SimpleClientPushButton()
{}

/*
** This method is the callback which describes to the client the 
**  button which was thwacked.  The button sends a message to the client
**  only all of the following are true:
**  1. The button is not ghosted (safety here)
**  2. The current pane makes the button sensitive.
** Formal Parameters:
**    XMWidget *xwid:
**      This should be the button widget handle.
**    XtPointer userd:
**       This is the button's user data.  It represents the button notification
**       code.  At the time of writing this comment I haven't decided if I'm
**       gonna get it from here or from the code attribute in the 
**       object data store.
**    XtPointer buttond:
**       The button specific structure data which I have decided will be 
**       ignored.
** NOTE:
**    This is a virtual function and hence overrides the do-nothing
**    function in the base class (I hope).
*/
void SimpleClientPushButton::ButtonPresscb(XMWidget *xwid,
					   XtPointer userd,
					   XtPointer buttond)
{
  msg_XamineEvent event;
  int nbytes = sizeof(msg_XamineEvent);

  if(IsEnabled()) {

    SpectrumType sptype = InitializeEvent(event, description.button_code);

    if(ButtonAvailable(description.whenavailable, sptype))
       nbytes = Xamine_DeclareClientEvent(&event);

  }
  if(nbytes <= 0)
    Xamine_error_msg(xwid, 
		     "Could not notify client of button push");
  return;
}

/*
** This page defines the class 'ConfirmClientPushButton' which is derived from
** 'SimpleClientPushbutton'.  The only real difference between this class and
** the SimpleClientPushButton class is that the callback for the button push
** pops up a confirmation dialog prior to returning the function to the caller.
** This gives the user a chance to cancel the function prior to execution.
** Useful for functions that produce irretrievable and destructive results.
**  This class needs a helper class called ClientConfirmation which is a
** derivation of XMQuestioner that actually peforms the confirmation.
*/
class ClientConfirmation : public XMQuestioner
{
  public:
     ClientConfirmation(char *name, XMWidget &parent, char *prompt,
		       SimpleClientPushButton *button) :
                       XMQuestioner(name, parent, prompt, (XtPointer *)button) {}
     ~ClientConfirmation() {}
  protected:
     virtual void Yescb(XMWidget *wid, // This override allows us to send the
			XtPointer ud,  // notification message to the client.
			XtPointer cd);
  private:
  
};
class ConfirmClientPushButton : public SimpleClientPushButton {
   public:
      ConfirmClientPushButton(ButtonBox *parent, ButtonDescription *bdef);
      virtual ~ConfirmClientPushButton();
      
   protected:
      virtual void ButtonPresscb(XMWidget *wid, XtPointer userd, 
				 XtPointer buttond);
   private:
      ClientConfirmation *prompter;
};
/*
**  Functional Description:
**    ClientConfirmation::Yescb:
**      This function is called when the Yes button is hit on a confirmation
**      dialog.  If the operation is still legal, then a message is sent to
**      the client indicating what was done and where.
** Formal Parameters:
**   XMWidget *wid:
**     The dialog widget object.
**   XtPointer userd:
**     User data, which in this case is the client button object that
**     caused us to come into existence.
**   XtPoniter calld:
**     Callback data which in this case has the form XmAnyCallbackStruct.
*/
void ClientConfirmation::Yescb(XMWidget *wid, XtPointer userd, XtPointer calld)
{
  SimpleClientPushButton *pb = (SimpleClientPushButton *)userd;
  ButtonDescription      *button = pb->GetButtonDef();
  msg_XamineEvent         event;

  if(pb->IsEnabled()) {		// If the button is enabled.
    SpectrumType sptype = InitializeEvent(event, button->button_code);
    if(ButtonAvailable(button->whenavailable, sptype)) { // And allowed to be used
      Xamine_DeclareClientEvent(&event);
    }
    else {
      Xamine_error_msg(wid,
		       "Function is not allowed in this pane");
    }
    
  }
  else {
    Xamine_error_msg(wid,
			 "This function has now been disabled");
  }
  XMQuestioner::Yescb(wid, userd, calld);
}
/*
** Functional description:
**   ConfirmClientPushButton::ConfirmClientPushButton:
**      This method instantiates an instance.  Other than calling the
**      immediate ancestor constructors, we only need to null out the
**      prompter field which indicates that the prompter will need to
**      be instantiated on a button press.
** Formal Parameters:
**   ButtonBox *parent:
**      The parent button box, which supplies the parent widget.
**   ButtonDescription *bdef:
**      Describes the button being defined.
*/
ConfirmClientPushButton::ConfirmClientPushButton(ButtonBox *parent,
						 ButtonDescription *bdef) :
			 SimpleClientPushButton(parent, bdef)
{
  prompter = (ClientConfirmation *)NULL;
}
/*
** The destructor just has to delete the prompter if it exists:
*/
ConfirmClientPushButton::~ConfirmClientPushButton()
{
  if(prompter != (ClientConfirmation*) NULL) delete prompter;
}
/*
** Functional Description:
**   ConfirmClientPushButton::ButtonPresscb:
**     This method is called when the button is pressed. If necessary,
**     the prompter is constructed.  The prompter is displayed and the
**     user data for the prompter is 'this' so that the prompter's callback
**     methods can call our button methods.
** Formal Parameters:
**   XMWidget *wid:
**     Widget which caused the callback (the button widget).
**   XtPointer userd:
**     The button code.
**   XtPointer calld:
**     The pushbutton callback data
*/
void ConfirmClientPushButton::ButtonPresscb(XMWidget *wid, XtPointer userd,
					    XtPointer calld)
{
  msg_XamineEvent event;
  SpectrumType sptype = InitializeEvent(event, // We really just want sptype
					description.button_code);

  if(ButtonAvailable(description.whenavailable, sptype)) {
    if(prompter == (ClientConfirmation *)NULL) {
      prompter = new ClientConfirmation(const_cast<char*>("TextPrompt"), *wid,
					description.promptstr,
					this);
    }
    prompter->Manage();
  }
  else {
    Xamine_error_msg(wid, 
		     "This function is not available in the selected pane");
  }

}

/*
** This page defines the class 'TextClientPushButton' which is derived from
** 'SimpleClientPushButton'.  This class is very close in initial function to the
** SimpleClientPushButton, however the button press callback method is replaced with
** a function that, if necessary, instantiates a text prompter.  The user receives
** a message only when the text prompter Ok or Apply button is clicked. The text
** that was typed in is passed to the user (truncated to BUTTON_MAXTEX-1 charss).
** as a null terminated string in the button message.   All of this necesitates
** The existence of a helper class called ClientTextPrompter which is a specialized
** version of the XMPrompter class with overrides for the Perform function that
** are appropriate to client buttons... that is one which sends a message back
** to the client process.
*/
class ClientTextPrompter : public XMPrompter {
  public: 
    ClientTextPrompter(char *name, XMWidget &parent, char *prompt,
		       SimpleClientPushButton *button) :
                       XMPrompter(name, parent, prompt, (XtPointer *)button) {}
    ~ClientTextPrompter() {}
  protected:
  private:
    virtual Boolean Perform(XMWidget *wid, XtPointer userd, XtPointer calld);
};

class TextClientPushButton : public SimpleClientPushButton {
   public:
     TextClientPushButton(ButtonBox *parent, ButtonDescription *bdef);
     virtual ~TextClientPushButton();

   protected:
     virtual void ButtonPresscb(XMWidget *wid, XtPointer userd, 
				XtPointer buttond);
   private:
     ClientTextPrompter *prompter;
};

/*
** Functional Description:
**    ClientTextPrompter::Perform:
**       This method is called when either the ok or apply button is pressed.
**       If the button is available and sensitive, then we send the client an
**       event containing the string typed in to the dialog.  Otherwise, we 
**       return False and pop up a message indicating why we did not act.
** Formal Parameters:
**   XMWidget *wid:
**     Pointer to the dialog widget class.
**   XtPointer userd:
**     User specific call data, in this case a pointer to the button definition
**     data structure.
**   XtPointer calld:
**     Callback specific data which in this case is a pointer to the 
**     XmSelectionBoxCallbackStruct describing the action.
** Returns:
**   True if the client was sent the accept message and False otherwise.
**   If False, a transient error message is displayed using 
**   Xamine_error_msg()
*/
Boolean ClientTextPrompter::Perform(XMWidget *wid, XtPointer userd, XtPointer calld)
{
  XmSelectionBoxCallbackStruct *cs = (XmSelectionBoxCallbackStruct *)calld;
  SimpleClientPushButton *pb       = (SimpleClientPushButton *)userd;
  ButtonDescription *button        = pb->GetButtonDef();
  msg_XamineEvent event;
  

  if(pb->IsEnabled()) {	// Button is not ghosted (dialog might still be up)
    SpectrumType sptype = InitializeEvent(event, button->button_code);
    
    if(ButtonAvailable(button->whenavailable, sptype)) { // Proper selected pane.
      
      /* Extract the text typed in by the user from the call back specific data
       ** and copy it into the event.
       */
      
      char *text;
      if(XmStringGetLtoR(cs->value, const_cast<char*>(XmSTRING_DEFAULT_CHARSET), &text)) {
	memset(event.data.button.text, 0, sizeof(event.data.button.text));
	strncpy(event.data.button.text, text,
		sizeof(event.data.button.text)-1);
	XtFree(text);
	
	/* Now declare a client event to pass the information back */
	
	Xamine_DeclareClientEvent(&event);
	return True;
      }
      else {			// XmStringGetLtoR failed.
	Xamine_error_msg(this, "Unable to get string from text prompter");
	return False;
      }
    }
    else {			// Button should not be available in selected pane.
      Xamine_error_msg(this, "Client function is not available in the selected pane");
      return False;
    }
  }
  else {			// Button should be ghosted, but dialog is still up.
    Xamine_error_msg(this, "This client button is not enabled");
    return False;
  }
}
/*
** Functional Description:
**   TextClientPushButton::TextClientPushButton:
**     Instantiates a client push button which prompts for text before 
**     creating a client event.
** Formal Parameters:
**   ButtonBox *parent:
**      Pointer to the button box object into which the button will be pasted.
**   ButtonDescription *bdef:
**      Describes the button.
*/
TextClientPushButton::TextClientPushButton(ButtonBox* parent,
					   ButtonDescription *bdef) :
		      SimpleClientPushButton(parent, bdef)
{
  prompter = (ClientTextPrompter *)NULL;
}
/*
** Functional Description:
**  TextClientPushButton::~TextClientPushButton:
**     Destroys an instance of the Client pushbutton.
*/
TextClientPushButton::~TextClientPushButton()
{
  if(prompter != (ClientTextPrompter *)NULL) delete prompter;
}
/*
** Functional Description:
**   TextClientPushButton::ButtonPresscb:
**     This method is called when the button is pressed.  If the prompter
**     does not exist, then we must instantiate it.  If it does exist, then we
**     must manage it.  The prompter's callback methods take care of passing
**     the message on to the client.  Therefore the prompter will need to get a
**     pointer to the button description in order to fill in the important parts of
**     the event message.
** Formal Parameters:
**     XMWidget *wid:
**        Pointer to the widget which fired us (our pushbutton widget).
**     XtPointer userd:
**        The button code ignored.
**     XtPointer calld:
**        Pointer to the user callback data structure.
*/
void TextClientPushButton::ButtonPresscb(XMWidget *wid, XtPointer userd, 
					 XtPointer buttond)
{
  msg_XamineEvent event;
  SpectrumType sptype = InitializeEvent(event, // We really just want sptype
					description.button_code);

  if(ButtonAvailable(description.whenavailable, sptype)) {
    if(prompter == (ClientTextPrompter *)NULL) {
      prompter = new ClientTextPrompter(const_cast<char*>("TextPrompt"), *wid,
					description.promptstr,
					this);
    }
    prompter->Manage();
  }
  else {
    Xamine_error_msg(wid,
		     "This function is not available in the selected pane");
  }
}

/*
**  These pages define the class SpectrumClientPushButton.
**  The class pops up a spectrum prompter dialog when the associated button
**  is preseed.  The spectrum chosen is returned to the caller.  The spectrum
**  prompt popped up depends on the spectype field of the Button definition.
**    Some helper classes are required.  The helper classes are the
**  specializations of the appropriate spectrum prompters.
*/

/* Prompter for any spectrum in database */

class  ClientSpectrumChooser : public SpectrumChooser
{
  public:
    ClientSpectrumChooser(char *name, XMWidget &parent, char *prompt,
			  XtPointer cbd) : SpectrumChooser(name, parent,
							   prompt, cbd)
			    {}
    virtual ~ClientSpectrumChooser() {}

  protected:
    virtual Boolean AppPerform(XMWidget *wid,
			       XtPointer userd,
			       int reason,
			       int specno) 
      { return AcceptSpectrum(wid,  userd, reason,  specno);
      }
  private:
};

/* Prompter for any spectrum in a class. */

class  ClientLimSpectrumChooser : public LimitedSpectrumChooser
{
  public:
    ClientLimSpectrumChooser(char *name, XMWidget &parent, char *prompt,
			     SpectrumClass limit,
			     XtPointer cbd) : LimitedSpectrumChooser(name, 
								     parent,
								     prompt, 
								     limit,
								     cbd)
			    {}
    virtual ~ClientLimSpectrumChooser() {}

  protected:
    virtual Boolean AppPerform(XMWidget *wid,
			       XtPointer userd,
			       int reason,
			       int specno) 
      { return AcceptSpectrum(wid,  userd, reason, specno);
      }
  private:
};

/* Prompter for any spectrum compatible with selected pane. */

class  ClientCompatSpectrumChooser : public CompatibleSpectrumChooser
{
  public:
    ClientCompatSpectrumChooser(char *name, XMWidget &parent, char *prompt,
				XtPointer cbd) : 
    CompatibleSpectrumChooser(name, parent, prompt, cbd)
                                           {}
    virtual ~ClientCompatSpectrumChooser() {}

  protected:
    virtual Boolean AppPerform(XMWidget *wid,
			       XtPointer userd,
			       int reason,
			       int specno) 
      { return AcceptSpectrum(wid, userd, reason, specno);
      }
  private:
};

/*  The button class, derived from SimpleClientPushButton: */

class SpectrumClientPushButton : public SimpleClientPushButton {
  public:
    SpectrumClientPushButton(ButtonBox *parent, ButtonDescription *bdef);
    virtual ~SpectrumClientPushButton();
  protected:
    virtual void ButtonPresscb(XMWidget *wid, XtPointer userd, 
			       XtPointer calld);
  private:
    SpectrumChooser *prompter;
};

/*
** Functional Description:
**   SpectrumClientPushButton::SpectrumClientPushButton:
**      This function instantiates a push button which when pressed pops up
**      a spectrum prompter.  The type of the prompter depends on the 
**      spectype field of the button descpriptor argument.
** Formal Parameters:
**   ButtonBox *parent:
**    Points to the button box which will parent the push button widget.
**   ButtonDescription *bdef:
**    Describes the button to be created (saved away for future use).
*/
SpectrumClientPushButton::SpectrumClientPushButton(ButtonBox *parent,
						   ButtonDescription *bdef) :
			  SimpleClientPushButton(parent, bdef)
{
  prompter = (SpectrumChooser *)NULL;
}
/*
** The destructor just deletes the prompter if it exists.
*/

SpectrumClientPushButton::~SpectrumClientPushButton()
{
  if(prompter != (SpectrumChooser *)NULL) delete prompter;
}
/*
** Functional Description:
**   SpectrumClientPushButton::ButtonPresscb:
**      This function is called whenever our button is pressed.  What we
**      do is, if necessary, instantiate a spectrum chooser dialog of the
**      appropriate type and then manage it to make it visible.  The chooser
**      and it's AppPerform callback will take care of all of the actual
**      messaging to the client.
** Formal Parameters:
**    XMWidget *wid:
**       This widget.
**    XtPointer userd:
**       User data, which is the button code and is ignored.
**    XtPointer buttond:
**       Pointer to the motif callback structure describing the callback
**       for a pushbutton (ignored too).
*/
void SpectrumClientPushButton::ButtonPresscb(XMWidget *wid, XtPointer userd,
					     XtPointer buttond)
{
  msg_XamineEvent event;
  SpectrumType sptype = InitializeEvent(event,
					description.button_code);

  if(!ButtonAvailable(description.whenavailable, sptype)) {
    Xamine_error_msg(wid,
		     "This function is not available in the selected pane");
    return;
  }

  if(prompter == (SpectrumChooser *)NULL) {
    switch(description.spectype) {
    default:
      Xamine_error_msg(wid,
		"Invalid spectrum prompt type on button, defaulting to 'Any'");

    case Any:
      prompter = new ClientSpectrumChooser(const_cast<char*>("Spectrum_Choice"),
					   *wid,
					   description.promptstr,
					   (XtPointer)this);
      break;
    case Oned:
      prompter = new ClientLimSpectrumChooser(const_cast<char*>("Spectrum_Choice"),
					      *wid,
					      description.promptstr,
					      oned,
					      (XtPointer)this);
      break;
    case Twod:
      prompter = new ClientLimSpectrumChooser(const_cast<char*>("Spectrum_Choice"),
					      *wid,
					      description.promptstr,
					      twod,
					      (XtPointer)this);
      break;
    case Compatible:
      prompter = new ClientCompatSpectrumChooser(const_cast<char*>("Spectrum_Choice"),
						 *wid,
						 description.promptstr,
						 (XtPointer)this);
      break;
   }
  }
 
  prompter->PopUp();
}
/*
** The page below define the local class: FileClientPushButton.
** This class is derived from SimpleClientPushButton.  When the associated
** button is pressed, the user is prompted for a filename with an
** XMFileSelectorDialog box.  If the user selects a file, then the filename
** is passed to the client process along with the button event.
** In order to function properly we make use of a help class:
**   ClientFileSelector which is subclassed from XMFileSelector with the
**   Perform callback replaced with one that sends the appropriate message
**   to the client process.
*/
class ClientFileSelector : public XMFileSelector
{
  public:
    ClientFileSelector(const char *n, XMWidget &parent,
		       XtPointer ud,
		       const char *directory = XMFILE_DEFAULT_DIRMASK);
    virtual ~ClientFileSelector();

  protected:
    virtual Boolean Perform(XMWidget *wid, XtPointer ud, char *filename,
			    int reason);
};
class FileClientPushButton : public SimpleClientPushButton
{
  public:
    FileClientPushButton(ButtonBox *parent, ButtonDescription *bdef);
    ~FileClientPushButton();

  protected:
    virtual void ButtonPresscb(XMWidget *wid, XtPointer userd,
			       XtPointer buttond);
  private:
    ClientFileSelector *prompter;
};

/*
** Functional Description:
**   ClientFileSelector::ClientFileSelector:
**      This constructs an XMFileSelector which is specialized to
**      serve the needs of a client button prompter.
**      We don't need to do any work. It's all done by the super class
**      constructors.   The only meat in this class is the 
**      replacement for the Perform method.
** Formal Parameters:
**   char *n:
**      Name of prompter widget.
**    XMWidget &parent:
**      Dialog's parent.
*     XtPointer ud:
**      Pointer to callback data... in this case it'll be a pointer to
**      a client button class.
**    char *directory:
**      The directory to search, defaults to cwd.
*/

ClientFileSelector::ClientFileSelector(const char *n, XMWidget &parent, XtPointer ud,
				       const char *directory) :
		    XMFileSelector(n, parent, ud, directory)
{
}
/*
** Functional Description:
**   ClientFileSelector::~ClientFileSelector:
**     Destroys the file selector prompter.. we just let the superclass
**     destructors do their work.
*/
ClientFileSelector::~ClientFileSelector()
{
}
/*
** Functional Description:
**   ClientFileSelector::Perform:
**     This method is called when the user selects a filename from the list
**     or types one in.  The action is to format a button press event and
**     send it to the client so that it can pick up the new filename.
**
** Formal Parameters:
**   XMWidget *wid:
**      The widget which triggered us, probably 'this'.
**   XtPointer ud:
**      User data, assumed to be a SimpleClientPushButton * from which
**      we can extract useful information about the pushbutton.
**   char *filename:
**      Name of the file that the user chose.
**   int reason:
**      The reason we were called... we don't care about that.
** Returns:
**   True if we feel the dialog should be dismissed on return.
*/
Boolean ClientFileSelector::Perform(XMWidget *wid, XtPointer ud, 
				    char *filename,
				    int reason)
{
  SimpleClientPushButton *pb       = (SimpleClientPushButton *)ud;
  ButtonDescription      *button   = pb->GetButtonDef();
  msg_XamineEvent         event;

  /* We'll require the following before notifying the caller:c
  **   1. The button is still enabled.
  **   2. The selected pane still contains the proper type of spectrum.
  */
  if(pb->IsEnabled()) {
    SpectrumType sptype = InitializeEvent(event, button->button_code);
    if(ButtonAvailable(button->whenavailable, sptype)) {
      memset(event.data.button.text, 0, sizeof(ButtonText));
      strncpy(event.data.button.text, filename, sizeof(ButtonText) - 1);
      Xamine_DeclareClientEvent(&event);
      return True;
    }
    else {
      Xamine_error_msg(this,
		       "This function not available in selected pane");
      UnManage();
      return True;
    }
  }
  else {
    Xamine_error_msg(this,
		     "Function is disabled at this time");
    UnManage();
    return True;
  }
}
/*
** Functional Description:
**   FileClientPushButton::FileClientPushButton:
**     This method instantiates a client push button which, when clicked,
**     pops up a dialog prompting for a filename.  When the filename is
**     entered, the client is notified.
** Formal Parameters:
**   ButtonBox *parent:
**     The button box that this button will be pasted into (used to parent
**     the widget).
**   ButtonDescription *bdef:
**     Definition of the button.
**   
*/
FileClientPushButton::FileClientPushButton(ButtonBox *parent, 
					   ButtonDescription *bdef) :
		      SimpleClientPushButton(parent, bdef)
{
  prompter = NULL;		// Prompter is instantiated by ButtonPresscb.
}
/*
** Destructor just deletes the prompter if it exists:
*/
FileClientPushButton::~FileClientPushButton()
{
  if(prompter) delete prompter;
}
/*
** Functional Description:
**   FileClientPushButton::ButtonPresscb:
**     This method does the real work of the class. It is invoked whenever
**     the button is clicked.  If conditions are right, we pop up the
**     prompter which is a ClientFileSelector.  The prompter autonomously
**     receives filenames and makes them available to the client process.
**
** Formal Parameters:
**   XMWidget *wid:
**     Pointer to the push button widget which got us called.
**   XtPointer userd:
**     client button id, which is ignored for now.
**   XtPointer buttond:
**     Button data from Motif, ignored.
*/
void FileClientPushButton::ButtonPresscb(XMWidget *wid, XtPointer userd, 
				    XtPointer buttond)
{
  msg_XamineEvent event;
  SpectrumType sptype = InitializeEvent(event, description.button_code);

  if(!ButtonAvailable(description.whenavailable, sptype)) {
    Xamine_error_msg(wid,
		     "This function is not available in the selected pane");
    return;
  }

  if(prompter == (ClientFileSelector *)NULL) {
    prompter = new ClientFileSelector(const_cast<char*>("Choose_File"),
				      *wid,
				      (XtPointer)this);
  }
  prompter->Manage();
}

/*
** Locally defined class PointsClientPushButton defines a class to handle
**   pushbuttons which require a points prompter.  A points prompter allows
**   the client process to get a set of points from one of Xamine's spectra
**   displayed in a pane.  In order to handle the prompter, we need to make
**   a specialization of the AcceptPointList class to manage the prompting and
**   the acceptance of the points.  These AcceptPointList derivation instances
**   are also going to be the ones responsible for signalling the client.
*/
class AcceptPointsPrompter : public AcceptPointList // Prompter class.
{
public:
  AcceptPointsPrompter(const char *name, 
		       XMWidget *parent,
		       SimpleClientPushButton *button,
		       int minpts, int maxpts, 
		       const char **help_text= NULL);
  ~AcceptPointsPrompter();

  virtual Boolean DoAccept();	// Must override accept behavior.
  virtual void SelectChanged(int oldc, int oldr, // Need to check the new pane
			     int newc, int newr); // for legitimacy.
  
private:
  SimpleClientPushButton *button; // Pointer to the associated button.
};


class PointsClientPushButton : public SimpleClientPushButton // Pushbutton.
{
public:
  PointsClientPushButton(ButtonBox *parent, ButtonDescription *bdef);
  ~PointsClientPushButton();
protected:
  virtual void ButtonPresscb(XMWidget *wid, XtPointer userd, 
			     XtPointer buttond);
private:
  AcceptPointsPrompter *prompter;
};


/*
** Functional Description:
**   AcceptPointsPrompter::AcceptPointsPrompter:
**     This method instantiates a points prompter.  We save the pointer
**     to our parent pushbutton and call the base class constructor to do
**     the hard work.
** Formal Parameters:
**   char *name:
**      Name of the dialog widget tree which will be instantiatied.
**   XMWidget *parent:
**      Parent to the dialog (probably a widget from inside our pushbutton
**      parent, but who cares.
**   SimpleClientPushButton *button:
**      Pointer to the push button for which we're prompting.
**      Retained to let us fish call some methods from this object in our
**      callbacks.
**   int minpts, maxpts:
**      Range of number of points which must have been designated to allow
**      an Accept to be legal.
**   char **help_text:
**      Possible override for help text.  If null, default help text is used.
*/

AcceptPointsPrompter::AcceptPointsPrompter(const char *name, XMWidget *parent,
					   SimpleClientPushButton *pbutton,
					   int minpts, int maxpts,
					   const char **help_text) :
		      AcceptPointList(name, parent, minpts, maxpts,
				      help_text)
{
  button = pbutton;
  NextLabel->SetLabel(button->GetButtonDef()->promptstr);
}
AcceptPointsPrompter::~AcceptPointsPrompter() // Destructor lets parents work.
{
}


/*
** Functional Description:
**   AcceptPointsPrompter::DoAccept:
**     This method is used to process the accept function of the dialog.
**     The parent is used to check for the right number of points.
**     We are responsible for sending the appropriate message (if any) to the
**     client.
*/

Boolean AcceptPointsPrompter::DoAccept()
{
  if(AcceptPointList::DoAccept()) { // Proper number of points.
    ButtonDescription *bd   = button->GetButtonDef();
    msg_XamineEvent    event;
    if(button->IsEnabled()) {
      SpectrumType sptype = InitializeEvent(event, bd->button_code);
      if(ButtonAvailable(bd->whenavailable, sptype)) {
	/*
	** The remainder of the event will be:
        **    text:   the point list name.
        **    npts:   number of points in the graphical object.
	**    points: point values.
	*/

	/* Set the text information */

	memset(event.data.button.text, // Null out the return text and...
	       0, sizeof(ButtonText));
	char *name = name_prompt->GetText();
	strncpy(event.data.button.text, name, sizeof(ButtonText) -1);
	XtFree(name);

	/* Get the number of points, and fill the point list:  */

	event.data.button.npts = object->pointcount();
	int i;
	grobj_point *pt;
	for(i = 0, pt = object->firstpt();
	    pt != NULL; pt = object->nextpt(), (i++)) {
	  event.data.button.points[i].x = pt->getx();
	  event.data.button.points[i].y = pt->gety();
	}
	Xamine_DeclareClientEvent(&event);

	/*
        ** Clear the points and so on.
	*/

	ClearPoints( Xamine_GetSelectedDrawingArea(), NULL);
	ClearDialog();
	object->clrpts();

	return True;

      }
      else {
	Xamine_error_msg(Points,
			 "Function is disabled at this time");
	UnManage();
	return True;
      }
    }
    else {
      Xamine_error_msg(Points, "This function is disabled at this time");
      UnManage();
      return True;
    }
  }
  else {
    return False;		// Leave the dialog and error messages up.
  }
}

/*
** Functional Description:
**   AcceptPointsPrompter::SelectChanged:
**     This method is called when the user selects a new pane.  If the
**     spectrum in the pane is not a valid type, then we put up a dialog
**     saying so and unmanage ourselves.  Regardless, we invoke the 
**     parent class method since that will get rid of old points and the
**     object as well.  Note that regardless of what the user requests,
**     The function is not legal if there is no spectrum in the underlying
**     pane since it's impossible to select points in that condition.
** 
**     
** Formal Parameters:
**   int oldc, oldr:
**     Old row and column designating the pane wer'e leaving.
**   int newc, newr:
**     New row and column designating the pane we're entering.
*/

void AcceptPointsPrompter::SelectChanged(int oldc, int oldr, 
					 int newc, int newr)
{
  AcceptPointList::SelectChanged(oldc, oldr, newc, newr);

  /* Now worry about whether or not we need to pop down with error: */

  if(object == NULL) {		// This indicates an empty pane:
    Xamine_error_msg(Points,
	     "Buttons which prompt for points are illegal in empty panes");
    UnManage();
    return;
  }

  /* Look at the spectrum type and figure out if we're allowed to operate
  ** here we cheat a bit given the functions we're using:
  */
  msg_XamineEvent event;
  ButtonDescription *bd = button->GetButtonDef();
  SpectrumType sptype = InitializeEvent(event, bd->button_code);
  if(! ButtonAvailable(bd->whenavailable, sptype)) {
    Xamine_error_msg(Points,
		     "This function is not available in this spectrum");
    UnManage();
    return;
  }
  /* I don't think there's any special additional action required here. */

}

/*
** Functional Description:
**   PointsClientPushButton::PointsClientPushButton:
**      This constructs an instance of a pushbutton which prompts for
**      a list of points.  When the points are accepted, the user is
**      informed. Most of the work is done by the SimpleClientPushButton 
**      constructor
** Formal Parameters:
**   ButtonBox *parent:
**     The button box into which the pushbutton will be pasted.
**   ButtonDescription *bdef:
**     Button definition.
*/

PointsClientPushButton::PointsClientPushButton(ButtonBox *parent,
					       ButtonDescription *bdef):
			SimpleClientPushButton(parent, bdef)
{
  prompter = NULL;
}
/*
** The destructor is equally simple:
*/
PointsClientPushButton::~PointsClientPushButton()
{
  if(prompter)
    delete prompter;
}

/*
** Functional Description:
**   PointsClientPushButton::ButtonPresscb:
**     This function processes the button press.  If necessary, we pop up
**     a prompter for the point list.  That's all we really have to do,
**     the prompter does the rest.
** Formal Parameters:
**   XMWidget *wid:
**     Pointer to the widget (pushbutton) which was pressed.
**   XtPointer userd:
**     User call data which in this case is ignored.
**   XtPointer calld:
**     Pointer to the call data for pushbuttons which is also ignored for now.
*/

void PointsClientPushButton::ButtonPresscb(XMWidget *wid, XtPointer userd, 
					   XtPointer calld)
{
  /* Make sure the function is legal:  */

  msg_XamineEvent event;
  SpectrumType sptype = InitializeEvent(event, description.button_code);
  if(!ButtonAvailable(description.whenavailable, sptype)) {
    Xamine_error_msg(wid, 
		     "This function is not available in the selected pane");
    return;
  }

  if(prompter == (AcceptPointsPrompter *)NULL) {
    prompter = new AcceptPointsPrompter(const_cast<char*>("User points"),
					wid,
					this,
					description.minpts,
					description.maxpts,
					NULL);
  }
  // Set the correct row and column:

  prompter->setrow(Xamine_PaneSelectedrow());
  prompter->setcol(Xamine_PaneSelectedcol());
  prompter->Manage();
}   
/** Locally defined class SimpleClientToggleButton:
**   This class implements and encapsulates the behavior of 
**   toggle buttons.  Toggle buttons are buttons that have a logical state
**   variable.  The appear as boxes which are either inset if the state
**   variable is 'True' or protruding if the state variable is 'False'.
**   The state variable is flipped (toggled) by pressing the button.
**   Simple toggles cannot have dialogs associated with them.
**   Toggling a button immediately reports an eveny to the client which
**   reports the new state of the toggle.
*/
class SimpleClientToggleButton : public ToggleButton
{ 
  public:
    SimpleClientToggleButton(ButtonBox *parent, ButtonDescription *bdef);
    virtual ~SimpleClientToggleButton();
    ButtonDescription *GetButtonDef() { return &description; }
  private:
    int code;
    ButtonSensitivity sensitivity;
  protected:
    virtual void ButtonPresscb(XMWidget *wid, XtPointer userd,
			       XtPointer buttond);
    ButtonDescription description;
};

/*
** Functional Description:
**   SimpleClientToggleButton::SimpleClientToggleButton:
**     This function instantiates a simple client toggle button.
**     Simple toggle buttons can be thought of as user settable flags
**     inside the client's program.
** Formal Parameters:
**   ButtonBox *parent:
**     Points to the button box object which will contain the desired button.
**   ButtonDescription *bdef:
**     Points to a description of the button which was received from the
**     client program.

*/

SimpleClientToggleButton::SimpleClientToggleButton(ButtonBox *parent,
						   ButtonDescription *bdef) :
			  ToggleButton(parent->GetButtonParent(), 
				       (XtPointer)bdef->button_code)
{
  code        = bdef->button_code;
  sensitivity = bdef->whenavailable;
  state       = bdef->state;
  SetState(state);		// Adjust state to match initial request.
  SetLabel(bdef->label);	// And the button label too.
  if(bdef->sensitive) Enable();
  else                Disable();
  memcpy(&description, bdef, sizeof(ButtonDescription));
}

/*
** Functional Description:
**   SimpleClientToggleButton::~SimpleClientToggleButton:
**     Deletes the button.. this is just a place holder to make sure our
**     anscestors get called.
*/
SimpleClientToggleButton::~SimpleClientToggleButton()
{
}

/*
** Functional Description:
**   SimpleClientToggleButton::ButtonPresscb:
**      This function is called when the toggle button is pressed.
**      We report an event to the user assuming the button is available
**      and enabled.
**
** Formal Parameters:
**   XMWidget *button:
**      Pointer to the widget that is the button.
**   XtPointer userd:
**      In this case the code that the user wants returned with the button.
**   XtPointer calld:
**      Actually a pointer to an XmToggleButtonCallbackStruct which
**      describes the state of the toggle and so on.
*/

void SimpleClientToggleButton::ButtonPresscb(XMWidget *button,
					     XtPointer userd,
					     XtPointer buttond)
{
  XmToggleButtonCallbackStruct *toggle = 
                                       (XmToggleButtonCallbackStruct *)buttond;
  
  int nbytes = sizeof(msg_XamineEvent);

  if(enabled) {

    SpectrumType sptype;
    win_attributed *win = Xamine_GetSelectedDisplayAttributes();
    int             spno;
    if(win == (win_attributed *)NULL) {
      spno = -1;
      sptype= StUndefined;
    } else {
      spno = win->spectrum();
      sptype = win->is1d() ? StOned : StTwod;
    }

    msg_XamineEvent event;	// Fill in the message:
    event.event           = Button_Press;
    event.data.button.buttonid = code;
    event.data.button.selected = spno; 
    event.data.button.spectrum = event.data.button.selected;
    event.data.button.text[0]  = '\0';
    event.data.button.npts     = 0;
    event.data.button.togglestate = toggle->set ? T : F;
    state = event.data.button.togglestate;

    if(ButtonAvailable(sensitivity, sptype))
       nbytes = Xamine_DeclareClientEvent(&event);


  }
  if(nbytes <= 0)
    Xamine_error_msg(button, 
		     "Could not notify client of button push");
  return;
}   

/*
** Functional Description:
**   ButtonAvailable:
**      Function determines if a button is availale.
**   Formal Parameters:
**     ButtonSensitivity sensitivity:
**        Describes when the button should be available.
**     SpectrumType sptype:
**        Describes the currently selected spectrum type.
** Returns:
**   T   - If the button is available.
**   F   - If the button should not be available.
*/
static Logical ButtonAvailable(ButtonSensitivity sensitivity, 
			       SpectrumType sptype)
{
    switch(sensitivity) {
    case Anytime:
      return T;
    case InSpectrum:
      if(sptype != StUndefined) {
	return T;
      } else {
	return F;
      }
      break;
    case In1dSpectrum:
      if(sptype == StOned) {
	return T;
      } else {
	return F;
      }
      break;
    case In2dSpectrum:
      if(sptype == StTwod) {
	return T;
      } else {
	return F;
      }
      break;
    default:
      return F;
      break;			// If sensitivity is screwed up do nothing.
    }
  
}

/*
** Functional Description:
**   InitializeEvent:
**      This is a utility function to set up the invariant part of an event.
**      It essentially builds a button event which has no user prompt data.
**      the caller can then format the prompt areas as required by the button.
** Formal Parameters:
**    msg_XamineEvent &evt:
**      Reference to the event to format.
**    int code:
**      The button code to use in the code field.
** Returns:
**    The type of the current spectrum.
*/
static SpectrumType InitializeEvent(msg_XamineEvent &event, int code)
{
  SpectrumType sptype;
  win_attributed *win = Xamine_GetSelectedDisplayAttributes();
  int             spno;

  if(win == (win_attributed *)NULL) {
    spno = -1;
    sptype= StUndefined;
  } else {
    spno = win->spectrum();
    sptype = win->is1d() ? StOned : StTwod;
  }
  
  event.event           = Button_Press;
  event.data.button.buttonid = code;
  event.data.button.selected = spno; 
  event.data.button.spectrum = event.data.button.selected;
  memset(event.data.button.text, 0, sizeof(event.data.button.text));
  event.data.button.npts     = 0;
  
  return sptype;
}

/*
** Functional Description:
**   AcceptSpectrum:
**     This function is called to do all the real work for spectrum
**     prompter's AppPerform methods.  We format the event and send it to 
**     the client.  There are some checks... for example, we make sure that
**     the button is still sensitive and that it's legal to use us at this
**     time.
** Formal Parameters:
**     XMWidget *wid:
**        Pointer to the dialog widget class.
**     XtPointer userd:
**        Pointer to the user data which in this case points to the
**        SpectrumClientPushButton object that instantiated the dialog.
**     int reason:
**        Reason for the callback.. we really don't care.
**     int specno:
**        Number of the spectrum that's been selected.
** Returns:
**    True  - If it's ok to pop down the dialog.
*/
static Boolean AcceptSpectrum(XMWidget *wid, XtPointer userd,
			      int reason, int specno)
{
  SimpleClientPushButton *pb = (SimpleClientPushButton *)userd;	// Be generic.
  ButtonDescription      *bd = pb->GetButtonDef();
  msg_XamineEvent         event;

  if(pb->IsEnabled()) {		// Dialog might be up on ghosted button.
    SpectrumType sptype = InitializeEvent(event, bd->button_code);
    if(ButtonAvailable(bd->whenavailable, sptype)) {	// Check type of pane.

      /* Fill in the rest of the event from the spectrum number and the
      ** button description and so on.
      */
      event.data.button.spectrum = specno; // Put in the spectrum chosen.

      Xamine_DeclareClientEvent(&event);
      return True;
    }
    else {			// Invalid pane type:
      Xamine_error_msg(pb->GetButton(),
		       "Selected pane not valid for this function");
      return True;
      wid->UnManage();
    }
  }
  else {			// Button ghosted.
    Xamine_error_msg(pb->GetButton(),
		     "Button is currently disabled");
    wid->UnManage();
    return True;
  }
}

/*
** Functional Description:
**   CreateButtonBox:
**      Create the user button box shell widget and pop it up.
**  Formal Parameters:
**      XMWidget *parent:
**         Parent widget for the top level shell that we're creating.
**      Cardinal c,r:
**         Number of columns of buttons in the box.
*/
static void CreateButtonBox(XMWidget *parent, 
			    Cardinal c,  Cardinal r)
{
  msg_status stat;
  if(buttons == (ButtonBox *)NULL) {		// Doesn't exist yet so make...
    buttons = new ButtonBox(parent, const_cast<char*>("Client Buttons"), r, c);
    buttons->Manage();		// Pop it up
    stat = Success;
  }
  else {			// Buttons exist so complain.
    stat = DuplicateId;
  }
  Xamine_SetClientStatus(stat);
}


/*
** Functional Description:
**   CreateButton:
**      This function creates a button as requested and pastes it to
**      the button box.  The button box must already be created or an
**      error will be returned to the user.
** Formal Parameters:
**    ButtonDescription:
**       Describes the button to be made.
**
*/
static msg_status CreateAButton(int r, int c, ButtonDescription *bd)
{
  msg_status stat;
  Button *button = NULL;

  /* First thing we do is require that the button box exist... */

  if(buttons == (ButtonBox *)NULL) {
    stat = NoSuchObject;
  }

  /* Next make sure the range of the rows and columns is inside the button
  ** box.  The error BadCoordinates is returned if not.
  */
  else if( !buttons->ValidLocation(r, c)){
    stat = BadCoordinates;
  }
  else {

    switch(bd->type) {
    case Push:
      switch(bd->prompter) {
      case NoPrompt:		// Simple pushbutton with no prompter.
	
	button = new SimpleClientPushButton(buttons,
					    bd);
	stat = Success;
	break;
      case Confirmation:	// Simple push button with confirmation.
	button = new ConfirmClientPushButton(buttons,
					     bd);
	stat  = Success;
	break;
      case Text:		// Simple push button with text prompter.
	button = new TextClientPushButton(buttons,
					  bd);
	stat = Success;
	break;
      case Spectrum:
	button = new SpectrumClientPushButton(buttons,
					      bd);
	stat = Success;
	break;
      case Filename:
	button = new FileClientPushButton(buttons,
					  bd);
	stat = Success;
	break;
      case Points:
	button = new PointsClientPushButton(buttons,
					    bd);
	stat = Success;
	break;
      default:
	stat = BadPrompter;
	break;
      }
      break;
    case Toggle:
      button = (Button *)new SimpleClientToggleButton(buttons,
						      bd);
      stat = Success;
      break;
    default:
      stat = BadType;
    }
    if(stat == Success) {
      buttons->SetButton(button, r, c);
      if(bd->sensitive)
	buttons->EnableButton(r, c);
      else
	buttons->DisableButton(r, c);
    }
  }
  return stat;
}
static void CreateButton(msg_DefineButton *b)
{
  Xamine_SetClientStatus(CreateAButton(b->which.row,
				       b->which.column,
				       &b->button));
  
}


/*
** Functional Description:
**  SetButtonEnable:
**    This function enables and disables buttons.  It can be used by
**    a client to manage 'modes' of operation by ghosting buttons which 
**    should not be accessible.
** Formal Parameters:
**   Boolean enable:
**      Describes into which state to put the button 
**   ButtonCoordinate &where:
**      Defines the coordinates of the buton to modify.
*/

static void SetButtonEnable(Boolean enable, ButtonCoordinate &where)
{
  msg_status stat;
  
  if(buttons == NULL) {
    Xamine_SetClientStatus(NoSuchObject);
    return;
  }
  if(buttons->ValidLocation(where.row, where.column)) {
    if(enable)buttons->EnableButton(where.row, where.column);
    else      buttons->DisableButton(where.row, where.column);
    stat = Success;
  }
  else {
    stat = BadCoordinates;
  }
  Xamine_SetClientStatus(stat);
}


/*
** Functional Description:
**   DeleteButton:
**     This function deletes a button in the client button box.
**     Called in response to a client request to manage the buttons.
** Formal Parameters:
**   ButtonCoordinate &which:
**      Describes which button should be deleted from the button box.
*/

static void DeleteButton(ButtonCoordinate &which)
{
  msg_status stat;

  if(buttons == NULL) {
    Xamine_SetClientStatus(NoSuchObject);
    return;
  }
  if(buttons->ValidLocation(which.row, which.column)) {
    Button *b = buttons->GetButton(which.row, which.column);
    buttons->ClrButton(which.row, which.column);
    delete b;
    stat = Success;
  }
  else {
    stat = BadCoordinates;
  }
  Xamine_SetClientStatus(stat);

} 

/*
** Functional Description:
**   DeleteBox:
**     This function deletes all of the buttons in a box and the button box
**     itself.
* No parameters required.
*/
static void DeleteBox()
{
  msg_status stat;

  if(buttons != NULL) {
    delete buttons;
    buttons = NULL;
    stat    = Success;
  }
  else {
    stat = NoSuchObject;
  }

  Xamine_SetClientStatus(stat);
}  


/*
** Functional Description:
**   InquireButton:
**     This function processes button inquiry messages.  If the button is
**     legal, then the button description is passed back to the client.
** Formal Parameters:
**   ButtonCoordinates &which:
**     Reference to the button coordinates of the button being asked for.
*/

static void InquireButton(ButtonCoordinate &which)
{
  
  msg_status stat;
  msg_InquireButtonAck ackdata;

  if(buttons == NULL) {
    ackdata.size.row = 0;
    ackdata.size.column = 0;
    ackdata.valid = F;
    Xamine_SetClientStatus(NoSuchObject);
    Xamine_SupplyButtonInfo(&ackdata);
    return;
  }
  ackdata.size.row    = buttons->GetRows();

  ackdata.size.column = buttons->GetColumns();
  
  if(buttons->ValidLocation(which.row, which.column)) {
    Button *b = buttons->GetButton(which.row, which.column);
    SimpleClientPushButton *pb;
    SimpleClientToggleButton *tb;
    ButtonDescription *bd;


    if(b->IsToggle()) {
      tb = (SimpleClientToggleButton *)b;
      bd  = tb->GetButtonDef();
    }
    else {
      pb = (SimpleClientPushButton *)b;
      bd = pb->GetButtonDef();
    }
    memcpy(&ackdata.button, bd, sizeof(ButtonDescription));
    ackdata.valid       = T;
    stat                = Success;
  }
  else {
    stat = BadCoordinates;
    ackdata.valid       = F;
  }

  Xamine_SetClientStatus(stat);
  Xamine_SupplyButtonInfo(&ackdata);

}


/*
** Functional Description:
**   ModifyButton:
**     This function modifies an existing button.  If the button exists,
**     it will be deleted, and then created.  If the button does not 
**     exist, then it will be created.
** Formal Parameters:
**   int r,c:
**     The coordinates of the button.
**   ButtonDescrition *bd:
**      The new description of the button.
** Returns:
**   Success       - Everything worked.
**   NoSuchObject  - The button box has not been created.
**   BadCoordinates- The button coordinates are outside the range of
**                   the box.
**   ButtonCreated - There was no prior button, so it was created.
*/

static void ModifyButton(int r, int c, ButtonDescription *bd)
{
  msg_status stat;

  if(buttons == NULL) 
    stat = NoSuchObject;
  else {
    if(!buttons->ValidLocation(r, c)) {
      stat = BadCoordinates;
    }
    else {
      Button *oldb = buttons->ClrButton(r, c); // Clear old button. 
      if(!oldb)
	delete oldb;		// Remove any prior existing button.
      stat = CreateAButton(r,c, bd);
      if(stat == Success) stat = ButtonCreated;
    }
  }
  Xamine_SetClientStatus(stat);
}  


/*
** Functional  Description:
**   ProcessButtonRequest:
**      This function is called to process a button request.  Requests
**      include those which create the button box, add or modify buttons
**      and ask for info about the buttons.
** Formal Parameters:
**    operation op:
**       The op code to execute.  Additional request data is in the
**       communication stream and fetched by us or our dependents.
**
*/
void Xamine_ProcessButtonRequest(operation op)
{
  Cardinal r,c;
  msg_DefineButton bdef;
  msg_EnableButton  which_enable;
  msg_DisableButton which_disable;
  msg_DeleteButton  which_delete;
  msg_InquireButton which_inquire;
  msg_ModifyButton  modifyblock;

  /* BUGBUGBUG -- Note that status of mailbox I/O is not checked */

  switch (op) {
  case Button_CreateBox:
    Xamine_ReadButtonGeometry(&c, &r); // Get the button creation args.
    CreateButtonBox(Xamine_Getpanemgr(),
		    c,r);	       // Create the button shell and pop up.
    break;
  case Button_DefineButton:
    Xamine_ReadButtonDefinition(&bdef);
    CreateButton(&bdef);
    break;
  case Button_EnableButton:
    Xamine_ReadEnableLocation(&which_enable);
    SetButtonEnable(True, which_enable.which);
    break;
  case Button_DisableButton:
    Xamine_ReadDisableLocation(&which_disable);
    SetButtonEnable(False, which_disable.which);
    break;
  case Button_DeleteButton:
    Xamine_ReadDeleteLocation(&which_delete);
    DeleteButton(which_delete.which);
    break;
  case Button_DeleteBox:
    DeleteBox();
    break;
  case Button_InquireButton:
    Xamine_ReadInquireLocation(&which_inquire);
    InquireButton(which_inquire.which);
    break;
  case Button_ModifyButton:
    Xamine_ReadBModifyBlock(&modifyblock);
    ModifyButton(modifyblock.which.row,
		 modifyblock.which.column,
		 &modifyblock.button);
    break;
  default:
    Xamine_error_msg(Xamine_Getpanemgr(),
		    "Xamine_ProcessButtonRequests -- Invalid request ignored");
    break;
  }

}
