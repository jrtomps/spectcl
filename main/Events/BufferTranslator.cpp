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
static const char* Copyright = "(C) Copyright Michigan State University 2006, All rights reserved";
/******************************************************************************
  Implementation of class BufferTranslator
******************************************************************************/

#include <config.h>
#include "BufferTranslator.h"
#include <stdio.h>

#ifdef HAVE_STD_NAMESPACE
using namespace std;
#endif

/*-----------------------------------------------------------------------------
  Name:  MyEndianess

  Purpose:  Determine the running system endianess
-----------------------------------------------------------------------------*/

BufferFactory::Endian MyEndianess()
{
  union {
    Int_t  asInt;
    Char_t asArray[sizeof(Int_t)];
  } x;
  x.asInt =1;

  if (x.asArray[0] == 1 ) {
    return BufferFactory::little;
  }
  
  else {
    return BufferFactory::big;
  }
}

/*-----------------------------------------------------------------------------
  Name:  BufferFactory::CreateBuffer

  Purpose:  Examine the lsignature of a buffer header and return a pointer
            to the appropriate BufferTranslator.
            The translator is dynamically allocated and it is the
	    caller's resopnsibility to delete it at the appropriate time
	    to prevent memory leaks.
-----------------------------------------------------------------------------*/

BufferTranslator*
BufferFactory::CreateBuffer( Address_t pBuffer, Int_t Signature32 )
{
  if( Signature32 == 0x01020304 ) {
    return new NonSwappingBufferTranslator(pBuffer);
  } 
  else {
    return new SwappingBufferTranslator(pBuffer);
  }
}

/*-----------------------------------------------------------------------------
  Name:  SwappingBufferTranslator::GetBlock

  Purpose:  Copy memory from buffer and return swapped
-----------------------------------------------------------------------------*/

void SwappingBufferTranslator::GetBlock( const Address_t pItem, 
					 Int_t size, Int_t iOffset )
{
  UChar_t *pBuff = (UChar_t *) m_pBuffer;
  UChar_t *Item = (UChar_t *) pItem;

  for( Int_t I = 0; I < size; I++ ) {
    Item[I] = pBuff[iOffset+I];
  }

  UChar_t *a = (UChar_t *) pItem, b;
  for( Int_t I = 0; I < size/2; I++ ) {
    b = a[I];
    a[I] = a[(size-1) - I];
    a[(size-1) - I] = b;
  }
}

/**
 * SwappingBuferTranslator::getQuad
 *
 * Turn a source uint64_t into a local uint64_t
 * when the byte orders are backwards
 *
 * @param value - input quadword.
 * @return uint64_t - the host orderd word.
 */
uint64_t
SwappingBufferTranslator::getQuad(uint64_t value)
{
    // The result is that of swapping the two longs
    // that make up the quad after swapping their byte orders:
    
    uint32_t low = value >> 32;
    uint32_t high = value & 0xffffffff;
    
    low  = TranslateLong(low);
    high = TranslateLong(high);
    
    uint64_t result = high;
    result = (result << 32) | low;
    
    return result;
    
}
/*-----------------------------------------------------------------------------
  Name:  NonSwappingBufferTranslator::GetBlock

  Purpose:  Copy memory from buffer and return
-----------------------------------------------------------------------------*/

void NonSwappingBufferTranslator::GetBlock( const Address_t pItem, 
					    Int_t size, Int_t iOffset )
{
  UChar_t *pBuff = (UChar_t *) m_pBuffer;
  UChar_t *Item = (UChar_t *) pItem;
  
  for( Int_t I = 0; I < size; I++ ) {
    Item[I] = pBuff[iOffset+I];
  }
}

/*-----------------------------------------------------------------------------
  Name:  BufferTranslator::GetByte

  Purpose:  Get the byte at offset iOffset from the buffer
-----------------------------------------------------------------------------*/

UChar_t BufferTranslator::GetByte( UInt_t iOffset )
{
  UChar_t Byte;
  GetBlock(&Byte, sizeof(UChar_t), iOffset);
  return (UChar_t)(Byte);  
}

/*-----------------------------------------------------------------------------
  Name:  BufferTranslator::GetWord

  Purpose:  Get the word at offset iOffset from the buffer and byteswap it
            if necessary
-----------------------------------------------------------------------------*/

Short_t BufferTranslator::GetWord( UInt_t iOffset )
{
  Short_t Word;
  GetBlock(&Word, sizeof(Short_t), iOffset);
  return (Short_t)(Word);
}

/*-----------------------------------------------------------------------------
  Name:  BufferTranslator::GetLongword

  Purpose:  Get the longword at offset iOffset from the buffer and byteswap it
            if necessary
-----------------------------------------------------------------------------*/

Long_t BufferTranslator::GetLongword( UInt_t iOffset )
{
  Long_t Longword;
  GetBlock(&Longword, sizeof(Long_t), iOffset);
  return (Long_t)(Longword);
}

/**
 * NonSwappingBufferTranslator::getQuad
 *
 * @param value 64 bits to reorder
 * @return uint64_t local byte ordering.
 */
uint64_t
NonSwappingBufferTranslator::getQuad(uint64_t value)
{
    return value;
}