// Author:
//   Ron Fox
//   NSCL
//   Michigan State University
//   East Lansing, MI 48824-1321
//   mailto:fox@nscl.msu.edu
//
/*!
  \class CFit

  Abstract base class defining the interface for fits.

*/

//
// Include files:
//

#ifndef __CFIT_H  //Required for current class
#define __CFIT_H

#ifndef __NAMEDITEM_H
#include <NamedItem.h>
#endif

#ifndef __STL_VECTOR
#include <vector>
#ifndef __STL_VECTOR
#define __STL_VECTOR
#endif
#endif

#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif

#ifndef __STL_UTILITY
#include <utility>
#ifndef __STL_UTILITY
#define __STL_UTILITY
#endif
#endif



class CCalibFit : public  CNamedItem     
{
  // Public data types.
public:
  struct Point {		//!< A point for the fit.

    double x;
    double y;

    int operator==(const Point& rhs) const {
      return (x == rhs.x) && (y == rhs.y);
    }
    int operator!=(const Point& rhs) const {
      return !(operator==(rhs));
    }
  };
  typedef STD(vector)<Point> PointArray; //!< Point array is a STD(vector) of points.
  typedef PointArray::iterator PointIterator;

  typedef STD(pair)<STD(string), double> FitParameter;
  typedef STD(vector)<FitParameter> FitParameterList;
  typedef FitParameterList::iterator FitParameterIterator;

  typedef enum {
    Accepting,
    Performed
  } FitState;			//!< State of the fit.
private:
  
  // Private Member data:

  STD(vector)<Point> m_afpPoints;  //!<  //<! Input points.  
  FitState m_eState;  //!<  //!< accumulating | performed  
   


public:
    //  Constructors and other canonical operations.
    //  You may need to adjust the parameters
    //  and the visibility esp. if you cannot
    // implement assignment/copy construction.
    // safely.
  CCalibFit ();		//!< Constructor.
  virtual  ~ CCalibFit ( ); //!< Destructor.
  CCalibFit (const CCalibFit& rSource ); //!< Copy construction.
  CCalibFit& operator= (const CCalibFit& rhs); //!< Assignment.
  int operator== (const CCalibFit& rhs) const; //!< == comparison.
  int operator!= (const CCalibFit& rhs) const { //!< != comparison.
    return !(operator==(rhs));
  }
    

// Insert any mutators you wish to export here.
// visibility should be protected.

protected:
  void SetFitState(FitState state) {	//!< set the state of the fit.
    m_eState = state;
  }

// Class operations:

public:
  
  

  void AddPoint (Point p)   ; 
  PointIterator begin ()   ; 
  PointIterator end ()   ; 
  size_t size ()   ; 
  FitState GetState ()  const ; 

 
  
 
  // Pure virtual functions must be implemented by
  // subclasses.

  virtual CCalibFit* clone() = 0;
  virtual   void Perform ()   = 0 ; 
  virtual  double operator() (double x)  =0 ; 
  virtual  FitParameterList GetParameters () =0  ; 
  virtual  STD(string) Type() const  = 0;
 

private:
  void CopyIn(const CCalibFit& rhs);	//!< Isolate copy in operation.

};

#endif
