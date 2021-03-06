C
C Facility:
C   Xamine  - NSCL display program.
C Abstract:
C   AEDSHARE.FOR:
C     This file provides compatibility between XAMINE and the old AEDTSK by
C     providing equivalences between the AEDTSK shared variables and the
C     Xamine shared variables.
C Author:
C     Ron Fox
C     NSCL
C     Michigan State University
C     East Lansing, MI 48824-1321
C     October 18, 1993
C
	INTEGER   AED_ISMAX, AED_BYTES, AED_WORDS, AED_LONGS
C
	INCLUDE 'xamineshare.for'
C
	PARAMETER (AED_ISMAX = XAMINE_ISMAX)
	PARAMETER (AED_BYTES = XAMINE_SPECBYTES)
	PARAMETER (AED_WORDS = XAMINE_WORDS)
	PARAMETER (AED_LONGS = XAMINE_LONGS)

	DIMENSION AED_DATA1 (AED_BYTES),
	1	  AED_DATA2 (AED_WORDS),
	2	  AED_DATA4 (AED_LONGS)

	BYTE      AED_DATA1
	INTEGER*2 AED_DATA2
	INTEGER*4 AED_DATA4

	INTEGER*2 AED_XY (2, AED_ISMAX)
	BYTE      AED_TITLE (72, AED_ISMAX)
	INTEGER   AED_OFFSET (AED_ISMAX)
	INTEGER   AED_TYPE (AED_ISMAX)

	POINTER  (AED_XYPTR,     AED_XY)
	POINTER  (AED_TITLEPTR,  AED_TITLE)
	POINTER  (AED_OFFSETPTR, AED_OFFSET)
	POINTER  (AED_TYPEPTR,   AED_TYPE)
	POINTER  (AED_DATA1PTR,  AED_DATA1)
	POINTER  (AED_DATA2PTR,  AED_DATA2)
	POINTER  (AED_DATA4PTR,  AED_DATA4)

	COMMON / AED_SHARED / AED_XYPTR, AED_TITLEPTR, AED_OFFSETPTR,
	1     	          AED_TYPEPTR, AED_DATA1PTR, AED_DATA2PTR,
	1		  AED_DATA4PTR

C
C     Cannot declare the functions since ULTRIX bitches inside the functions.
C

