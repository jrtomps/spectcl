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
	INTEGER AED_BYTES, AED_WORDS, AED_LONGS, AED_PAGES
	PARAMETER (AED_BYTES=1048576*8)
	PARAMETER (AED_WORDS=AED_BYTES/2)
	PARAMETER (AED_LONGS=AED_BYTES/4)
	PARAMETER (AED_PAGES=AED_BYTES/512)

	INTEGER XAMINE_SPECBYTES
	PARAMETER (XAMINE_SPECBYTES = AED_BYTES)
	INCLUDE 'XAMINE_DIR:XAMINESHARE.FOR'
C
	INTEGER AED_ISMAX
	PARAMETER (AED_ISMAX = XAMINE_ISMAX)

	DIMENSION AED_DATA1(AED_BYTES)
	1	  ,AED_DATA2(AED_WORDS)
	2	  ,AED_DATA4(AED_LONGS)
	EQUIVALENCE (AED_DATA1,XAMINE_DATA1)
	EQUIVALENCE (AED_DATA2, XAMINE_DATA2)
	EQUIVALENCE (AED_DATA4, XAMINE_DATA4)

	LOGICAL*1 AED_DATA1
	INTEGER*2 AED_DATA2
	INTEGER*4 AED_DATA4

	INTEGER*2 AED_XY(2,AED_ISMAX)
	BYTE      AED_TITLE(72, AED_ISMAX)
	INTEGER   AED_OFFSET(AED_ISMAX)
	INTEGER   AED_TYPE(AED_ISMAX)

	EQUIVALENCE (AED_XY, XAMINE_XY)
	EQUIVALENCE (AED_TITLE, XAMINE_TITLES)
	EQUIVALENCE (AED_OFFSET, XAMINE_OFFSETS)
	EQUIVALENCE (AED_TYPE,   XAMINE_TYPES)
