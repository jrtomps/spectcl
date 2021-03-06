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


#include <config.h>
#include "XMPushbutton.h"

/*
** XMButton class implementation
*/

XMButton::XMButton(const char *n, WidgetClass c, Widget parent)
  : XMManagedWidget(n, c, parent)
{}

XMButton::XMButton(const char *n, WidgetClass c, XMWidget &parent)
  : XMManagedWidget(n, c, parent)
{}

XMButton::XMButton(Widget w) : XMManagedWidget(w)
{}

void
XMButton::Enable()
{
  SetAttribute(XmNsensitive, (XtArgVal)True); 
}

void
XMButton::Disable()
{
  SetAttribute(XmNsensitive, (XtArgVal)False);
}

void 
XMButton::Label(XmString label)
{
  SetAttribute(XmNlabelString, label); 
}

void
XMButton::Label(const char* label)
{
  XmString s;
  s = XmStringCreateLtoR(const_cast<char*>(label), 
			 const_cast<char*>(XmSTRING_DEFAULT_CHARSET));
  Label(s);
  XmStringFree(s);
}

void
XMButton::SetMnemonic(KeySym k)
{
  SetAttribute(XmNmnemonic, k); 
}

void
XMButton::SetAccelerator(const char *translation, const char *prompt)
{
  XmString acctext = XmStringCreateSimple(const_cast<char*>(prompt));
  XtVaSetValues(id,
		XmNaccelerator, translation,
		XmNacceleratorText, acctext,
		NULL);
  XmStringFree(acctext);
}

/////////////////////////////////////////////////////////////////////////////////

/*
 * XMPushButton implementation
 */

void
XMPushButton::Initialize( void (*cb)(XMWidget *, XtPointer, XtPointer),
			  XtPointer cd )
{
  Enable();
  if(cb)
    XMWidget::AddCallback(XmNactivateCallback, cb, cd);
}

XMPushButton::XMPushButton(const char *n, Widget parent, 
			   void (*cb)(XMWidget *, XtPointer, XtPointer),
			   XtPointer cd ):
  XMButton(n, xmPushButtonWidgetClass, parent)
{
  Initialize(cb,cd);
}

XMPushButton::XMPushButton(const char *n, XMWidget &parent, 
			   void (*cb)(XMWidget *, XtPointer, XtPointer),
			   XtPointer cd):
  XMButton(n, xmPushButtonWidgetClass, parent)
{
  Initialize(cb,cd);

}

XMPushButton::XMPushButton(Widget w) : XMButton(w) {}

Callback_data*
XMPushButton::AddCallback(void (*cb)(XMWidget *, 
				     XtPointer, 
				     XtPointer),
			  XtPointer cd)
{
  return XMWidget::AddCallback(XmNactivateCallback, cb, cd);
}

///////////////////////////////////////////////////////////////////////
/*
** Implementation of cascadee buttons.
*/

void
XMCascadeButton::Initialize(void (*cb)(XMWidget *, XtPointer, XtPointer),
			    XtPointer cd)
{
  Enable();
  if(cb)
    XMWidget::AddCallback(XmNactivateCallback, cb, cd);
}


XMCascadeButton::XMCascadeButton(const char *n, Widget parent, 
				 void (*cb)(XMWidget *, XtPointer, XtPointer),
				 XtPointer cd):
  XMButton(n, xmCascadeButtonWidgetClass, parent)
{
  Initialize(cb, cd);
}

XMCascadeButton::XMCascadeButton(const char *n, XMWidget &parent, 
				 void (*cb)(XMWidget *, XtPointer, XtPointer),
				 XtPointer cd):
  XMButton(n, xmCascadeButtonWidgetClass, parent)
{
  Initialize(cb, cd);
}

XMCascadeButton::XMCascadeButton(Widget w) : XMButton(w) {}

void
XMCascadeButton::SetAssociatedMenu(XMWidget &w)
{
  SetAttribute(XmNsubMenuId, w.getid());
}

void
XMCascadeButton::SetAssociatedMenu(Widget w)
{
  SetAttribute(XmNsubMenuId, w);
} 

Callback_data*
XMCascadeButton::AddCallback(void (*cb)(XMWidget *, 
					XtPointer, 
					XtPointer) ,
			     XtPointer cd)
{ 
  return XMWidget::AddCallback(XmNactivateCallback, cb, cd); 
}

/////////////////////////////////////////////////////////////////////////////////////

/*
 * Implementation of the XMToggleButton class: 
 */

void
XMToggleButton::Initialize( void (*cb)(XMWidget *, XtPointer, XtPointer) ,
			    XtPointer cd)
{
 Enable();
  if(cb)
    XMWidget::AddCallback(XmNvalueChangedCallback, 
			  cb, cd);
}

XMToggleButton::XMToggleButton(const char *n, Widget parent,
			       void (*cb)(XMWidget *, XtPointer, XtPointer) ,
			       XtPointer cd) :
  XMButton(n, xmToggleButtonWidgetClass, parent)
{
  Initialize(cb, cd);
}

XMToggleButton::XMToggleButton(const char *n, XMWidget &parent,
			       void (*cb)(XMWidget *, XtPointer, XtPointer) ,
			       XtPointer cd) :
  XMButton(n, xmToggleButtonWidgetClass, parent)
{
  Initialize(cb, cd);
}

XMToggleButton::XMToggleButton(Widget w) : XMButton(w) {}

void
XMToggleButton::ShowIndicator() /* Display the toggle */
{ 
  SetAttribute(XmNindicatorOn, (XtArgVal)True);
} 

void
XMToggleButton::HideIndicator()
{
  SetAttribute(XmNindicatorOn, (XtArgVal)False);
}

void
XMToggleButton::Diamond()
{ 
  SetAttribute(XmNindicatorType, 
	       (XtArgVal)XmONE_OF_MANY);
}

void
XMToggleButton::Box()
{ 
  SetAttribute(XmNindicatorType,
	       (XtArgVal)XmN_OF_MANY);
}

void
XMToggleButton::SetState(Boolean state)
{ 
  SetAttribute(XmNset, state); 
}

void
XMToggleButton::Set()
{
  SetAttribute(XmNset, (XtArgVal)True);
}

void
XMToggleButton::UnSet()
{ 
  SetAttribute(XmNset, (XtArgVal)False);
}

Boolean
XMToggleButton::GetState()
{ 
  Boolean value;
  GetAttribute(XmNset, &value);
  return value;
}

Callback_data*
XMToggleButton::AddCallback(void (*cb)(XMWidget *, 
				       XtPointer, 
				       XtPointer) ,
			    XtPointer cd )
{ 
  return XMWidget::AddCallback(XmNvalueChangedCallback, cb, cd);
}
/////////////////////////////////////////////////////////////////////////

/*
 *  Arrow button implementation
 */
void
XMArrowButton::Initialize(void (*cb)(XMWidget *, XtPointer, XtPointer) ,
			  XtPointer cd )
{
  Enable();
  if(cb) 
    XMWidget::AddCallback(XmNactivateCallback, cb, cd);
}

XMArrowButton::XMArrowButton(const char *n, Widget parent, 
			     void (*cb)(XMWidget *, XtPointer, XtPointer) ,
			     XtPointer cd ) :
  XMButton(n, xmArrowButtonWidgetClass, parent)
{
  Initialize(cb, cd);
}

XMArrowButton::XMArrowButton(const char *n, XMWidget &parent,
			     void (*cb)(XMWidget *, XtPointer, XtPointer) ,
			     XtPointer cd ) :
  XMButton(n, xmArrowButtonWidgetClass, parent)
{
  Initialize(cb, cd);
}

Callback_data*
XMArrowButton::AddCallback(void (*cb)(XMWidget *, XtPointer, XtPointer),
			   XtPointer cd)
{
  return XMWidget::AddCallback(XmNactivateCallback, cb, cd);
}

void
XMArrowButton::PointLeft()
{
  SetAttribute(XmNarrowDirection, (XtArgVal)XmARROW_LEFT);
}

void
XMArrowButton::PointRight()
{
  SetAttribute(XmNarrowDirection, (XtArgVal)XmARROW_RIGHT);
}

void
XMArrowButton::PointUp()
{
  SetAttribute(XmNarrowDirection, (XtArgVal)XmARROW_UP);
}

void
XMArrowButton::PointDown()
{
  SetAttribute(XmNarrowDirection, (XtArgVal)XmARROW_DOWN);
}

unsigned char
XMArrowButton::Direction()
{
  unsigned char dir;
  GetAttribute(XmNarrowDirection, &dir);
  return dir;
}

void
XMArrowButton::Label(XmString label)
{}

void
XMArrowButton::Label(const char*  label)
{}

void
XMArrowButton::SetMnemonic(KeySym k)
{}
