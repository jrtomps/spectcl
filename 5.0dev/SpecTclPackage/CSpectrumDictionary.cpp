/*
    This software is Copyright by the Board of Trustees of Michigan
    State University (c) Copyright 2009.

    You may use this software under the terms of the GNU public license
    (GPL).  The terms of this license are described at:

     http://www.gnu.org/licenses/gpl.txt

     Author:
             Ron Fox
	     NSCL
	     Michigan State University
	     East Lansing, MI 48824-1321
*/
#include "CSpectrumDictionary.h"
#include "CSpectrum.h"

/** 
 * @file CSpectrumDictionary.cpp
 * @brief Implementation code of the spectrum dictionary.
 */

/**
 * Class level attributes:
 */

CSpectrumDictionary* CSpectrumDictionary::m_pInstance(0);

/**
 * Constructor - no-op.
 */
CSpectrumDictionary::CSpectrumDictionary() {}
/**
 * Destructor
 *   To facilitate testing (This method is not
 *   called in productioncode); delete all spectra in the
 *   dictionary.
 */
CSpectrumDictionary::~CSpectrumDictionary()
{
  m_pInstance = 0;		// Reset our instance ptr.
  for (DictionaryIterator p = m_Dictionary.begin(); 
       p != m_Dictionary.end(); p++) {
    delete p->second;
  }
}
/**
 * instance
 *    This method makes this a singleton by creating
 *    the one instance, if needed, and returning a pointer to
 *    it.
 *
 * @return CSpectrumDictionary*
 */
CSpectrumDictionary*
CSpectrumDictionary::instance()
{
  if (!m_pInstance) {
    m_pInstance = new CSpectrumDictionary;
  }
  return m_pInstance;
}
/**
 * add
 *   Add an entry tothe dictionary.
 *
 * @param spectrum - reference to the added spectrum.  Adding the spectrum does not
 *                   transfer storage ownership in production code.
 * 
 * @throw  spectrum_dictionary_exception if spectrum is already in the dictionary.
 */
void
CSpectrumDictionary::add(CSpectrum& spectrum)
{
  if (m_Dictionary.find(spectrum.getName()) == m_Dictionary.end()) {
    m_Dictionary[spectrum.getName()] = &spectrum;
    fireObservers(Add);

  } else {
    std::string msg = "CSpectrumDictionary::add - cannot insert duplicate spectrum named: '";
    msg += spectrum.getName();
    msg += "'";
    throw spectrum_dictionary_exception(msg);
  }
}
/**
 * find
 *   Find a spectrum given its name.
 *
 * @param name - Name of the spectrum to find.
 * @return CSpectrum*
 * @retval  0 if no such spectrum.
 */
CSpectrum*
CSpectrumDictionary::find(const char* pName)
{
  DictionaryIterator p = m_Dictionary.find(std::string(pName));
  if (p != m_Dictionary.end()) {
    return p->second;
  } else {
    return 0;
  }
}
/**
 * remove
 *
 *  Removes the specfied spectrum from the dictionary.
 *
 * @param pName - name of the spectrum.
 *
 * @throw spectrum_dictionary_exception - spectrum is no in the dictionary.
 */
void
CSpectrumDictionary::remove(const char* pName)
{
  DictionaryIterator  p = m_Dictionary.find(std::string(pName));
  if (p == m_Dictionary.end()) {
    std::string msg = "CSpectrumDictionary::remove - cannot remove spectrum '";
    msg += pName;
    msg += "' does not exist";
    throw spectrum_dictionary_exception(msg);
  } else {
    fireObservers(Remove);
    m_Dictionary.erase(p);
  }

}
/**
 * size
 *
 *  @return size_t - the number of spectra in the dictionary.
  */
size_t
CSpectrumDictionary::size()
{
  return m_Dictionary.size();
}


/*
 * getReason
 *
 *   Get the reason for the current observation.
 *
 * @return CSpectrumDictionary::Op
 *
 * @note exception thrown on empty.
 */
CSpectrumDictionary::Op
CSpectrumDictionary::getReason()
{
  if (! m_observationReason.empty()) {
    return m_observationReason.top();
  } else {
    throw spectrum_dictionary_exception
      ("CSpectrumDictionary::getReason - not observing so there's no reason");
  }
}
/*----------------------------------------------------------------------
 *  Private methods.
 */

/**
 * fireObservers
 *
 *   Request the observable interface invoke the observers.
 *
 * @param op - Operation that triggered observation.  The most recent (current
 *             one is available via getReason.
 *
 * @note nested observation is supported via a stack of operations.
 */
void
CSpectrumDictionary::fireObservers(CSpectrumDictionary::Op reason)
{
  m_observationReason.push(reason);
  
  observe();

  m_observationReason.pop();
  
}
