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

#include <TCLApplication.h>
#include <TCLLiveEventLoop.h>
#include <CTCLServer.h>

class evttclsh : public CTCLApplication
{
public:
  virtual int operator()();
};

int
evttclsh::operator()()
{
  CTCLLiveEventLoop* pLoop = CTCLLiveEventLoop::getInstance();
  CTCLServer         server(getInterpreter(), 1234);
  pLoop->start();
  return TCL_ERROR;
}

CTCLApplication* gpTCLApplication = new evttclsh;
