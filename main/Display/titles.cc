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
**   Xamine -- NSCL display program.
** Abstract:
**   titles.cc:
**      This file contains code to manage the display of titles
**      on the spectrum.  There are two types of titles.  The first
**      is the identification titles.  These are displayed along the bottom
**      of the spectrum and are the only ones that are addressed by this
**      module.  The second set of titles are optional titles on or near the
**      graphical objects.  These are addressed by the software that actually
**      draws the graphical objects themselves.
** Author:
**     Ron FOx
**     NSCL
**     Michigan State University
**     East Lansing, MI 48824-1321
*/


/*
** Include files:
*/

#include <config.h>

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>



#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include "dispshare.h"
#include "titles.h"
#include "axes.h"
#include "panemgr.h"
#include "text.h"
#include "refreshctl.h"
#include "chanplot.h"
#include "mapcoord.h"

/*
**  External references:
*/

extern spec_shared *xamine_shared;

/*
** Functional Description:
**   maxpeak1dl:
**     Determines the maximum peak information for a 1-d longword per channel
**     spectrum.  The info that we want is the height and position of the
**     tallest channel.
** Formal Parameters:
**   unsigned integer *c:
**     Points to the first channel of the spectrum.
**   unsigned int xl,xh:
**     Define the limits within which we look for a max channel.
**   unsigned int *maxval:
**     Returned value of the spectrum at the maximum chanel value.
**   unsigned int *maxx:
**     Location of the maximum X channel.
*/
static void maxpeak1dl(volatile unsigned int *c, int xl, int xh, 
		       unsigned int *maxval,
		       unsigned int *maxx)
{
  *maxx  = xl;
  *maxval= 0;
  for(int i = xl; i <= xh; i++)
    if(c[i] > *maxval) {
      *maxval = c[i];
      *maxx   = i;
    }
}
/*
** Functional Description:
**   maxpeak1dw:
**     This function determines the parameters of the maximum channel
**     value for a 1-d word spectrum.  See maxpeak1dl for more information,
**     The only difference is that the channel pointer is a pointer to word.
*/
static void maxpeak1dw(unsigned short *c, int xl, int xh, unsigned int *maxval,
		       unsigned int *maxx)
{
  *maxx  = xl;
  *maxval= 0;
  for(int i = xl; i <= xh; i++)
    if(c[i] > *maxval) {
      *maxval = c[i];
      *maxx   = i;
    }
}



/*  maxpeak2
      Template function that figures out the info for the max peak position
      for 2d spectra.
*/
template<class T>
static void maxpeak2d(T* c, int xl, int xh, int yl, int yh,
			 int xdim, int ydim, unsigned int *maxval,
			 unsigned int* maxx, unsigned int* maxy)
{
  *maxval = 0;
  *maxx   = 0;
  *maxy   = 0;

  /*   Compute the pointer to the first row of the area of interest: */

  c += (yl)*xdim + xl;

  /* Now loop over the rows in the area of interest and look for the max chan */

  for(int iy = yl; iy <= yh; iy++) {
    T *cr  = c;	/* points within the row being scanned */
    for(int ix = xl; ix <= xh; ix++) {
      if( *cr >= *maxval) {
	*maxval = *cr;
	*maxx   = ix;
	*maxy   = iy;
      }
      cr++;			/* Point to the next channel. */
    }
    c += xdim;			/* Point c to the next row.   */
  }
}

// Explicit versions, so we don't have to port:

static void maxpeak2dl(unsigned int* c, int xl, int xh, int yl, int yh,
		       int xdim, int ydim, unsigned int* maxval,
		       unsigned int* maxx, unsigned int* maxy)
{
  maxpeak2d<unsigned int>(c, xl, xh, yl, yh, xdim, ydim, maxval,
			   maxx, maxy);
}


static void maxpeak2dw(unsigned short* c, int xl, int xh, int yl, int yh,
		       int xdim, int ydim, unsigned int* maxval,
		       unsigned int* maxx, unsigned int* maxy)
{
  maxpeak2d<unsigned short>(c, xl, xh, yl, yh, xdim, ydim, maxval,
			   maxx, maxy);
}

static void maxpeak2db(unsigned char* c, int xl, int xh, int yl, int yh,
		       int xdim, int ydim, unsigned int* maxval,
		       unsigned int* maxx, unsigned int* maxy)
{
  maxpeak2d<unsigned char>(c, xl, xh, yl, yh, xdim, ydim, maxval,
			   maxx, maxy);
}



/*
** Functional Description:
**   getpeakinfo:
**     This static function produces the text necessary to describe the
**     current maximum channel location on a spectrum.
** Formal Parameters:
**     char *text:
**        Buffer to hold the resulting string.
**     win_attributed *def:
**        Window definition... used to get spectrum number etc.
*/
static void getpeakinfo(char *text, win_attributed *def)
{
  win_1d *at1 = (win_1d *)def;
  win_2d *at2 = (win_2d *)def;

  /* Get the spectrum number, spectrum base channel, dimensions and 
  ** spectrum type.  We must then branch to the appropriate peak 
  ** channel finding procedure.
  ** NOTE:  Since end channels of the spectrum often contain noisy channels,
  ** we are going to exclude the end channels if they would be part of the
  ** region of interest.  The peak channel that we give is the peak within
  ** the region displayed.
  */
  int spno = def->spectrum();	/* The spectrum number of this spectrum. */
  volatile unsigned int *chans  = 
    xamine_shared->getbase(spno); /* Spectrum channel. */
  unsigned int xdim    = xamine_shared->getxdim(spno); /* X dimension */
  unsigned int ydim    = xamine_shared->getydim(spno); /* Y dimension */
  unsigned int xl = 1, xh = xdim-2;	/* default X range. */
  unsigned int yl = 1, yh = ydim-2;	/* Y range. */
  unsigned int maxval;
  unsigned int maxx, maxy;
  float fxl, fxh, fyl, fyh;
  float xscale, yscale;

  switch(xamine_shared->gettype(spno)) {
  case onedlong:
    if(at1->isexpanded()) {
      xl = at1->lowlimit();
      xh = at1->highlimit();
      if(xl <= 0) xl = 1;
      if(xh >= xdim-1)xh = xdim-2;
    }
    if(at1->ismapped()) {
      fxl = xamine_shared->getxmin_map(spno);
      fxh = xamine_shared->getxmax_map(spno);
    }
    maxpeak1dl(chans, xl, xh, &maxval, &maxx);
    if(at1->ismapped())
      sprintf(text, " Max=%d @ %.1f ", maxval, 
	      Xamine_XChanToMapped(spno, maxx));
    else 
      sprintf(text, " Max=%d @ %u ", maxval, maxx);
    break;
  case onedword:
    if(at1->isexpanded()) {
      xl = at1->lowlimit();
      xh = at1->highlimit();
      if(xl <= 0) xl = 1;
      if(xh >= xdim-1)xh = xdim-2;
    }
    if(at1->ismapped()) {
      fxl = xamine_shared->getxmin_map(spno);
      fxh = xamine_shared->getxmax_map(spno);
    }
    maxpeak1dw((unsigned short *)chans, xl, xh, &maxval, &maxx);
    if(at1->ismapped())
      sprintf(text, " Max=%d @ %.1f ", maxval, 
	      Xamine_XChanToMapped(spno, maxx));
    else 
      sprintf(text, " Max=%d @ %u ", maxval, maxx);
    break;  
  case twodlong:
    if(at2->isexpanded()) {
      xl = at2->xlowlim();
      xh = at2->xhilim();
      if(xl <= 0) xl = 1;
      if(xh >= xdim-1) xh = xdim-2;

      yl = at2->ylowlim();
      yh = at2->yhilim();
      if(yl <= 0) yl = 1;
      if(yh >= ydim-1) yh = ydim - 2;
    }
    if(at2->ismapped()) {
      fxl = xamine_shared->getxmin_map(spno);
      fxh = xamine_shared->getxmax_map(spno);
      fyl = xamine_shared->getymin_map(spno);
      fyh = xamine_shared->getymax_map(spno);
    }
    maxpeak2dl((unsigned int *)chans, xl, xh, yl, yh, xdim, ydim,
	       &maxval, &maxx, &maxy);
    if(at2->ismapped()) 
      sprintf(text, "Max=%d @ %.1f,%.1f ", maxval, 
	      Xamine_XChanToMapped(spno, maxx),
	      Xamine_YChanToMapped(spno, maxy));
    else
      sprintf(text,"Max=%d @ %u,%u ", maxval, maxx, maxy);
    break;  
  case twodword:
    if(at2->isexpanded()) {
      xl = at2->xlowlim();
      xh = at2->xhilim();
      if(xl <= 0) xl = 1;
      if(xh >= xdim-1) xh = xdim-2;

      yl = at2->ylowlim();
      yh = at2->yhilim();
      if(yl <= 0) yl = 1;
      if(yh >= ydim-1) yh = ydim - 2;
    }
    if(at2->ismapped()) {
      fxl = xamine_shared->getxmin_map(spno);
      fxh = xamine_shared->getxmax_map(spno);
      fyl = xamine_shared->getymin_map(spno);
      fyh = xamine_shared->getymax_map(spno);
    }
    maxpeak2dw((unsigned short *)chans, xl, xh, yl, yh, xdim, ydim,
	       &maxval, &maxx, &maxy);
    if(at2->ismapped()) 
      sprintf(text, "Max=%d @ %.1f,%.1f ", maxval, 
	      Xamine_XChanToMapped(spno, maxx),
	      Xamine_YChanToMapped(spno, maxy));
    else
      sprintf(text,"Max=%d @ %u,%u ", maxval, maxx, maxy);
    break;
  case twodbyte:
    if(at2->isexpanded()) {
      xl = at2->xlowlim();
      xh = at2->xhilim();
      if(xl <= 0) xl = 1;
      if(xh >= xdim-1) xh = xdim-2;

      yl = at2->ylowlim();
      yh = at2->yhilim();
      if(yl <= 0) yl = 1;
      if(yh >= ydim-1) yh = ydim - 2;
    }
    if(at2->ismapped()) {
      fxl = xamine_shared->getxmin_map(spno);
      fxh = xamine_shared->getxmax_map(spno);
      fyl = xamine_shared->getymin_map(spno);
      fyh = xamine_shared->getymax_map(spno);
    }
    maxpeak2db((unsigned char *)chans, xl, xh, yl, yh, xdim, ydim,
	       &maxval, &maxx, &maxy);
    if(at2->ismapped()) 
      sprintf(text, "Max=%d @ %.1f,%.1f ", maxval, 
	      Xamine_XChanToMapped(spno, maxx),
	      Xamine_YChanToMapped(spno, maxy));
    else
      sprintf(text,"Max=%d @ %u,%u ", maxval, maxx, maxy);
    break;
  case undefined:
    fprintf(stderr, "BUGBUGBUG - in getpeakinfo undefined spectrum  %d\n",
	    spno);
    exit(-1);
  default:
    fprintf(stderr, 
	    "BUGBUGBUG - in getpeakinfo - illegal spectrum type %d %d\n",
	    xamine_shared->gettype(spno), spno);
    exit(-1);
  }
}

/*
** Functional Description:
**    getupdatetime:
**      This static function produces the last update time label fragment.
**  Formal Parameters:
**     char *text:
**       Text buffer to hold the results.
**     win_attributed *def:
**       window definition... holds the last update time.
*/
static void getupdatetime(char *text, win_attributed *def)
{
  struct tm *time;
  time_t    clock;

  clock = def->lastupdated();
  time = localtime(&clock);
  strftime(text, 100," %X  ", time);
}

/*
** Functional Description:
**   drawlabels:
**       This static function draws the accumulated text label
**       in the display window.
** Formal Parameters:
**    XMWidget &w:
**       Widget of a drawing area widget to draw in.
**    char *text:
**       The text to draw.
**    Drawable win:
**       The actual drawable in which the drawing will be done.  Passing it
**       in rather than extracting it from the widget allows drawing to be done
**       to a backing pixmap and then later copied to the window when done.
*/
static void drawlabels(XMWidget &w,  char *text, Drawable win)
{
  /* Construct the graphical context for doing the plotting: */

  Display *disp;
  Screen  *screen;
  GC      gc;

  disp   = XtDisplay(w.getid());
  screen = XtScreen(w.getid());

  gc = Xamine_MakeDrawingGc(disp, &w, -1);


  /* Determine the text bounding box.  The lower left corner is the origin 
  ** so that's easy.  The upper right corner is located XAMINE_MARGINSIZE/2
  ** times the window size above and to the right of the origin.
  */
  float xh,yh;			/* These will be the pixel points of the UR */
  Dimension   nx,ny;			/* Sizes of the window... */
  Dimension   ymargin;

  w.GetAttribute(XmNwidth, &nx);
  w.GetAttribute(XmNheight,&ny);
  w.GetAttribute(XmNborderWidth, &ymargin);
  ny -= (ymargin+2);

  xh = (float)nx - ((float)(nx)*(XAMINE_MARGINSIZE/2.0));
  yh = (float)ny - ((float)(ny)*(XAMINE_MARGINSIZE/2.0));


  /* Write the text: */

  Xamine_DrawFittedString(disp, win, gc, 0,ny, (int)xh,(int)yh, text);

  /* Now free the graphical context now that we're done drawing: */


}

/*
** Functional Description:
**    Xamine_DrawTitles:
**      This procedure draws the textual titles underneath the spectrum in
**      the label section of the spectrum.
** Formal Parameters:
**     XMWidget *pane:
**        The drawing area widget which is to be drawn in.
**     win_attributed *def:
**        The window definition attributes used to determine what to draw
**        and so on and so forth.
** NOTE:
**     The function Xamine_DrawFittedString is used to do the drawing so that
**     the titles will just vanish if the space for them gets too small.
*/
void Xamine_DrawTitles(Xamine_RefreshContext *ctx, win_attributed *def)
{
  /*  The first order of business is to build up the buffer label 
  **  to contain the label string that we would like to put at the bottom
  **  of the display.
  */
  XMWidget *pane = ctx->pane;
  char label[400];		/* String to hold the label text. */
  char label_element[200];	/* The element being built up. */

  memset(label, 0, sizeof(label));
  if(def->shownum()) {
    sprintf(label_element, "[%d] ", def->spectrum());
    strncat(label, label_element, sizeof(label) - strlen(label) -1);
  }

  if(def->showname()) {
    xamine_shared->getname(label_element, def->spectrum());
    if(strlen(label_element) == 0) strcpy(label_element, "<Untitled>");
    strncat(label, label_element, sizeof(label) - strlen(label) - 1);
    xamine_shared->getinfo(label_element, def->spectrum()); // The info string..
    strncat(label, " : ", sizeof(label) - strlen(label) - 1);
    strncat(label, label_element, sizeof(label) - strlen(label) - 1);
    strncat(label, " ", sizeof(label) - strlen(label) -1);
  }
  if(def->showdescrip()) {
    if(def->is1d()) {
      sprintf(label_element, " %d bins",
	      xamine_shared->getxdim(def->spectrum()));
    }
    else {
      sprintf(label_element, " %d x %d bins ", 
	      xamine_shared->getxdim(def->spectrum()), 
	      xamine_shared->getydim(def->spectrum()));
    }
    strncat(label, label_element, sizeof(label) - strlen(label) - 1);
  }
  if(def->showpeak()) {
    getpeakinfo(label_element, def);
    strncat(label, label_element, sizeof(label) - strlen(label) - 1);
  }
  if(def->showupdt()) {
    getupdatetime(label_element, def);
    strncat(label, label_element, sizeof(label) - strlen(label) - 1);
  }
  /* Note that the grobj labels are drawn by the grobj updater */

  /* If the label length is greater than zero, then we attempt to draw it */

  if(strlen(label) > 0) {
    drawlabels(*pane, label, ctx->pixmap);
  }

}







