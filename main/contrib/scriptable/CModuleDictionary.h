
#ifndef __CMODULEDICTIONARY_H  //Required for current class
#define __CMODULEDICTIONARY_H

#ifndef __STL_MAP
#include <map>
#ifndef __STL_MAP
#define __STL_MAP
#endif
#endif

#ifndef __STL_STRING
#include <string>
#ifndef __STL_STRING
#define __STL_STRING
#endif
#endif

// Forward class definitions:

class CModule;
class CSegmentUnpacker;
 
/*!
	CModule dictionary is a wrapper class around  a dictionary
	of pointers to CModules.  This class allows the 
	system to keep track of the modules that have been created.
*/
class CModuleDictionary      
{
public:
    typedef STD(map)<STD(string), CSegmentUnpacker*> ModuleMap;
    typedef ModuleMap::iterator       ModuleIterator;
	
private:
    
    ModuleMap m_Modules; 	//!< Set of modules maintained.


public:
    // Constructors and other canonical operations.
    
    CModuleDictionary ();
    ~CModuleDictionary ( );
    CModuleDictionary (const CModuleDictionary& rhs );
    CModuleDictionary& operator= (const CModuleDictionary& rhs);
    int operator== (const CModuleDictionary& rhs) const;
    int operator!=(const CModuleDictionary& rhs) const {
	return !(operator==(rhs));
    }

// Selectors:

public:
    ModuleMap getModules() const
    {
	return m_Modules;
    }

public:

    void Add (CSegmentUnpacker* pModule)   ; 
    ModuleIterator Find (const STD(string)& rName)   ; 
    void Delete (ModuleIterator iModule)   ; 
    ModuleIterator begin ()   ; 
    ModuleIterator end ()   ; 
    int size ()   ;  

};

#endif
