/*!
  This is an abstract base class that is the base
  class for individual readout modules.
  This class provides support for a parameter
  map.  Parameter maps provide a mapping
  between module chnannel numbers and
  SpecTcl parameter ids.

*/

// Author:
//   Ron Fox
//   NSCL
//   Michigan State University
//   East Lansing, MI 48824-1321
//   mailto:fox@nscl.msu.edu
//
// Copyright 

#ifndef __CMODULE_H  //Required for current class
#define __CMODULE_H

//
// Include files:
//

                               //Required for base classes
#ifndef __CSEGMENTUNPACKER_H     //CSegmentUnpacker
#include "CSegmentUnpacker.h"
#endif
 
#ifndef __TCLPROCESSOR_H
#include <TCLProcessor.h>        
#endif

#ifndef __STL_STRING
#include <string> 
#ifndef __STL_STRING       
#define __STL_STRING
#endif
#endif

#ifndef __STL_VECTOR
#include <vector>
#ifndef __STL_VECTOR
#define __STL_VECTOR
#endif
#endif


class CModule  : public CSegmentUnpacker
{
private:
  

    STD(vector)<int> m_ParameterMap;  //!<  Maps channel # -> parameter id.  
   

public:
    //  Constructors and other canonical operations.

    CModule (const STD(string)& rName,CTCLInterpreter& rInterp); //!< Constructor.
    ~CModule ( ); //!< Destructor.
private:
    CModule (const CModule& rSource ); //!< Copy construction.
    CModule& operator= (const CModule& rhs); //!< Assignment.
    int operator== (const CModule& rhs) const; //!< == comparison.
    int operator!= (const CModule& rhs) const; //!< != comparison.
public: 

    
 public:
    
    void CreateMap (int nMapSize)   ;                    //!< Define an empty map.
    void MapElement (int nChannel, const STD(string)& sName)   ; //!< Make a  mapping.
    int Id (int nChannel)   ; 	                         //!< Translate map. 
    

};

#endif
