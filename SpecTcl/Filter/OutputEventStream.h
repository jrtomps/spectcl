#ifndef __OUTPUTEVENTSTREAM_H //Required for current class
#define __OUTPUTEVENTSTREAM_H

#ifndef __STL_STRING
#include <string>
#define __STL_STRING
#endif

#ifndef __STL_VECTOR
#include <vector>
#define __STL_VECTOR
#endif

#ifndef __STDIO_H
#include <stdio.h>
#define __STDIO_H
#endif

#ifndef __RPC_XDR_H
#include <rpc/xdr.h>
#define __RPC_XDR_H
#endif

#ifndef __HISTOTYPES_H
#include <histotypes.h>
#define __HISTOTYPES_H
#endif

#ifndef __PARAMETER_H
#include <Parameter.h>
#define __PARAMETER_H
#endif

#ifndef __EVENT_H
#include <Event.h>
#define __EVENT_H
#endif

#ifndef __EVENTSINK_H
#include <EventSink.h>
#define __EVENTSINK_H
#endif

class COutputEventStream {
  Bool_t m_fActive;
  string m_sFileName;
  vector<string> m_vParameterNames;
  UInt_t m_nValidParameters; // Number of parameters declared in the filter dictionary. (Does not necessarily coincide with the isValid check done later.)
  char *m_pBitMask;
  UInt_t m_nEvents;
  FILE *m_pFile;
  XDR m_xdrs;
  Bool_t m_fXDRError;
  UInt_t m_nOffset; // Number of bytes sent for this block/buffer.
  UInt_t m_nBUFFERSIZE;
  UInt_t m_nMAXEVENTBUFFERSIZE;
  vector<CEvent*> m_vEventBuffer;

 public:
  // Constructors.
  COutputEventStream();
  COutputEventStream(string&, vector<string>&);
  virtual ~COutputEventStream();

  // Operators.
  Bool_t operator()(const string&); // To set file name. DO NOT ABUSE BY SENDING AN EVENT OR SOMETHING ELSE!
  Bool_t operator()(CEvent&); // To receive and subsequently send an event.
  COutputEventStream& operator=(const COutputEventStream&);

  // Additional functions.
  Bool_t isActive();
  string ParseFileName(string&);
  string getFileName();
  Bool_t Open();
  Bool_t Close();
  Bool_t ReceiveEvent(CEvent&);
  Bool_t SendEventBuffer();
  Bool_t XDRstring(string&);
  Bool_t XDRuint(UInt_t&);
  Bool_t XDRarray(char**);
  Bool_t XDRfloat(Float_t&);
  Bool_t XDRfill(UInt_t);
  void incr_offset(UInt_t);
}; // COutputEventStream.

#endif
