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

//
//     NSCL
//     Michigan State University
//     East Lansing, MI 48824-1321
//     mailto: fox@nscl.msu.edu
// 
// (c) Copyright NSCL 1999, All rights reserved TCLHistogrammer.h
//

#ifndef __TCLHISTOGRAMMER_H  //Required for current class
#define __TCLHISTOGRAMMER_H

#ifndef __PSEUDOSCRIPT_H
#include <PseudoScript.h>
#endif
                               //Required for base classes
#ifndef __HISTOGRAMMER_H
#include "Histogrammer.h"
#endif
                               
                               //Required for 1:1 association classes
#ifndef __TCLINTERPRETEROBJECT_H
#include "TCLInterpreterObject.h"
#endif
                                                               
                               //Required for 1:M association classes
#ifndef __PSUEDOSCRIPT_H
#include "PseudoScript.h"
#endif

#ifndef __STL_LIST
#include <list>
#ifndef __STL_LIST
#define __STL_LIST
#endif
#endif
  
typedef STD(list)<CPseudoScript>                   PseudoParameters;
typedef STD(list)<CPseudoScript>::iterator         PseudoParameterIterator;
                                                           
class CTCLHistogrammer  : public CHistogrammer        
{                       
	
private:		
  CTCLInterpreter*       m_pIntObject; //1:1 association object data member 
  PseudoParameters       m_Pseudos;


public:

   // Constructors and other cannonical operations:

  CTCLHistogrammer (CTCLInterpreter* m_pInterpreter,
		    UInt_t nSpecBytes = knDefaultSpectrumSize);
  virtual ~ CTCLHistogrammer ( );  
  
   //Copy constructor 

  CTCLHistogrammer (const CTCLHistogrammer& aCTCLHistogrammer )   : 
    CHistogrammer (aCTCLHistogrammer) 
  {     
    m_pIntObject = aCTCLHistogrammer.m_pIntObject;
    m_Pseudos    = aCTCLHistogrammer.m_Pseudos;
  }                                     

   // Operator= Assignment Operator 

  CTCLHistogrammer& operator= (const CTCLHistogrammer& aCTCLHistogrammer);
 
   //Operator== Equality Operator 

  int operator== (const CTCLHistogrammer& aCTCLHistogrammer);
	
// Selectors:

public:
                       
  CTCLInterpreter* getInterpreter() const
  { 
    return m_pIntObject;
  }
  PseudoParameters getPseudoParameters() const
  {
    return m_Pseudos;
  }
                       
// Mutators:

protected:
       
  void setInterpreter(CTCLInterpreter* am_pIntObject)
  { 
    m_pIntObject = am_pIntObject;
  }
  void setPseudoParameters(const PseudoParameters& rPseudos) {
    m_Pseudos = rPseudos;
  }

public:
  virtual   void operator() (const  CEvent& rEvent);

  void AddPseudo (CPseudoScript& rScript)    ;
  void RemovePseudo (const STD(string)& rPseudoName)    ;
  CPseudoScript& FindPseudo (const STD(string)& rPsuedoName)    ;
  PseudoParameterIterator PseudoBegin ()    ;
  PseudoParameterIterator PseudoEnd ()    ;
  UInt_t PseudoSize ()    ;
};

#endif
