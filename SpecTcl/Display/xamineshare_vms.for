C
C Facility:
C   Xamine  -- NSCL display program.
C Abstract:
C    XAMINESHARE.FOR:
C      This file is part of the VAX/VMS fortran bindings for Xamine.
C      In particular, this file provdes definitions for the 
C      Xamine shared memory region.
C Author:
C   Ron Fox
C   NSCL
C   Michigan State University
C   East Lansing, MI 48824-1321
C   October 18, 1993
C
C
C -- The includer must provide the value of:
C    XAMINE_SPECBYTES -- number of spectrum bytes.
C
C	THE  COMMON AREA FOR THE XAMINE DISPLAY PROGRAM
C
C
	INTEGER XAMINE_ISMAX,XAMINE_GRANULE, XAMINE_WORDS
	INTEGER XAMINE_LONGS,XAMINE_PAGES
	PARAMETER (XAMINE_ISMAX=999)
	PARAMETER (XAMINE_GRANULE=65536)
C	PARAMETER (XAMINE_SPECBYTES=1048576*8)
	PARAMETER (XAMINE_WORDS=XAMINE_SPECBYTES/2)
	PARAMETER (XAMINE_LONGS=XAMINE_SPECBYTES/4)
	PARAMETER (XAMINE_PAGES=(XAMINE_SPECBYTES+XAMINE_GRANULE-1)
	1			/XAMINE_GRANULE)
C
C	PARAMETERS TO DEFINE THE SPECTRUM TYPES:
C
C
	INTEGER XAMINE_UNDEFINED, XAMINE_ONEDLONG, XAMINE_ONEDWORD
	INTEGER XAMINE_TWODWORD, XAMINE_TWODBYTE
	PARAMETER (XAMINE_UNDEFINED = 0)
	PARAMETER (XAMINE_ONEDLONG  = 4)
	PARAMETER (XAMINE_ONEDWORD  = 2)
	PARAMETER (XAMINE_TWODWORD  = 3)
	PARAMETER (XAMINE_TWODBYTE  = 1)

	DIMENSION XAMINE_DATA1(XAMINE_PAGES*XAMINE_GRANULE)
	1	  ,XAMINE_DATA2(XAMINE_WORDS)
	2	  ,XAMINE_DATA4(XAMINE_LONGS)
	EQUIVALENCE (XAMINE_DATA1,XAMINE_DATA2),(XAMINE_DATA1,XAMINE_DATA4)
	LOGICAL*1 XAMINE_DATA1
	INTEGER*2 XAMINE_DATA2
	INTEGER*4 XAMINE_DATA4
	INTEGER*2 XAMINE_XY(2,XAMINE_ISMAX)
	BYTE      XAMINE_TITLES(72,XAMINE_ISMAX)
	BYTE      XAMINE_PAD(65536)
	INTEGER   XAMINE_OFFSETS(XAMINE_ISMAX)
	INTEGER   XAMINE_TYPES(XAMINE_ISMAX)
	COMMON/XAMINE_SHARED/ XAMINE_XY, XAMINE_TITLES, XAMINE_OFFSETS,
	1		      XAMINE_TYPES, XAMINE_DATA1, XAMINE_PAD
	VOLATILE /XAMINE_SHARED/

C
C       Describe the communications structures for buttons and events.
C

C	    Button structure size parameters.
	
	INTEGER BUTTON_LABELSIZE, BUTTON_PROMPTSIZE, BUTTON_MAXTEXT
	PARAMETER (BUTTON_LABELSIZE = 16,
	1	   BUTTON_PROMPTSIZE = 80,
	1	   BUTTON_MAXTEXT    = 256)

C	    Graphical object size parameters.

	INTEGER GROBJ_MAXPTS, GROBJ_NAMELEN
	PARAMETER (GROBJ_MAXPTS = 50,
	1	   GROBJ_NAMELEN= 80)	

C	    Event types:

	INTEGER XAMINE_ISGATE, XAMINE_ISBUTTON
	PARAMETER (XAMINE_ISGATE = 0,
	1	   XAMINE_ISBUTTON = 1)

C	    Xamine status code definitions.

	INTEGER XAMINE_SUCCESS, XAMINE_NOSUCHSPECTRUM,
	1   XAMINE_INAPPROPRIATEGATE, XAMINE_OBJECTTABLEFULL, 
	2   XAMINE_DUPLICATEID, XAMINE_NOSUCHOBJECT, XAMINE_NOPROCESS,
	3   XAMINE_CHECKERRNO, XAMINE_BADCOORDINATES, XAMINE_BADTYPE,
	4   XAMINE_BADPROMPTER, XAMINE_BUTTONCREATED
	PARAMETER (XAMINE_SUCCESS = 0,
	1	   XAMINE_NOSUCHSPECTRUM = -1,
	1	   XAMINE_INAPPROPRIATEGATE = -2,
	1	   XAMINE_OBJECTTABLEFULL = -3,
	1	   XAMINE_DUPLICATEID = -4,
	1	   XAMINE_NOSUCHOBJECT = -5 ,
	1	   XAMINE_NOPROCESS = -6,
	1	   XAMINE_CHECKERRNO = -7,
	1	   XAMINE_BADCOORDINATES = -8,
	1	   XAMINE_BADTYPE = -9,
	1	   XAMINE_BADPROMPTER = -10,
	1	   XAMINE_BUTTONCREATED = 1)


C	    Button types:


	INTEGER XAMINE_PUSH, XAMINE_TOGGLE
	PARAMETER (XAMINE_PUSH = 0,
	1	   XAMINE_TOGGLE = 1)


C    	   PROMPTER TYPES:


	INTEGER XAMINE_NOPROMPT, XAMINE_CONFIRMATION, XAMINE_TEXT, 
	1   XAMINE_POINTS, XAMINE_SPECTRUM, XAMINE_FILENAME
	PARAMETER (XAMINE_NOPROMPT = 0,
	1	   XAMINE_CONFIRMATION = 1,
	1	   XAMINE_TEXT = 2,
	1	   XAMINE_POINTS = 3,
	1	   XAMINE_SPECTRUM = 4,
	1	   XAMINE_FILENAME = 4)

C	   BUTTON AVAILABILIITY:

	INTEGER XAMINE_ANYTIME, XAMINE_INSPECTRUM, XAMINE_IN1D, XAMINE_IN2D
	PARAMETER (XAMINE_ANYTIME = 0,
	1	   XAMINE_INSPECTRUM = 1    ,
	1	   XAMINE_IN1D = 2,
	1	   XAMINE_IN2D = 3)

C	   Spectrum classes allowed in chooser:

	INTEGER XAMINE_ANY, XAMINE_1D, XAMINE_2D, XAMINE_COMPATIBLE
	PARAMETER (XAMINE_ANY = 0,
	1	   XAMINE_1D = 1,
	1	   XAMINE_2D = 2,
	1	   XAMINE_COMPATIBLE = 3)

C	    Button coordinates structure.

	STRUCTURE /XAMINE_BUTTONCOORDINATES/
	    INTEGER column,row
	END STRUCTURE    !XAMINE_BUTTONCOORDINATES

C	    Button description:

	STRUCTURE /XAMINE_BUTTONDESCRIPTION/
	    INTEGER button_code
	    CHARACTER*(BUTTON_LABELSIZE) label
	    INTEGER type
	    LOGICAL state
	    LOGICAL sensitive
	    INTEGER prompter
	    INTEGER whenavailable
	    CHARACTER*(BUTTON_PROMPTSIZE) promptstr
	    INTEGER spectype
	    INTEGER minpts, maxpts
	END STRUCTURE    !XAMINE_BUTTONDESCRIPTION

C	    This is a coordinate point e.g. in a spectrum.

	STRUCTURE /XAMINE_POINT/
	    INTEGER x,y
	END STRUCTURE    !XAMINE_POINT

C	    A graphical object sent in part of an event message.

	STRUCTURE /XAMINE_GROBJ/
	    INTEGER spectrum
	    INTEGER id
	    INTEGER object_type
	    LOGICAL hasname
	    CHARACTER*(80) name
	    INTEGER npts
	    RECORD /XAMINE_POINT/ points(GROBJ_MAXPTS)
	END STRUCTURE    !XAMINE_GROBJ

C	    A button press description sent

	STRUCTURE /XAMINE_BUTTONPRESS/
	    INTEGER buttonid
	    INTEGER selected_spectrum
	    LOGICAL togglestate

	    INTEGER spectrum	    ! This and below are from prompters.
	    CHARACTER*(BUTTON_MAXTEXT) text
	    INTEGER NPTS
	    RECORD /XAMINE_POINT/ points(GROBJ_MAXPTS)
	    END STRUCTURE    !XAMINE_BUTTONPRESS

C	    An event that can come from Xamine:

	STRUCTURE /XAMINE_EVENT/
	    INTEGER event
	    UNION 
	    MAP
		RECORD /XAMINE_GROBJ/ object
	    END MAP
	    MAP
		RECORD/XAMINE_BUTTONPRESS/ button
	    END MAP
	    END UNION
	    END STRUCTURE    !XAMINE_EVENT
C
C        Define functions:
C
	INTEGER F77XAMINE_CREATESHAREDMEMORY ! (IBYTES, LOCATION)
	INTEGER F77XAMINE_START  ! ()
	INTEGER F77XAMINE_STOP   ! ()
	LOGICAL F77XAMINE_ALIVE  ! ()
	INTEGER F77XAMINE_INIT
	INTEGER F77XAMINE_MAPMEMORY
	INTEGER F77XAMINE_ENTERGATE
	INTEGER F77XAMINE_REMOVEGATE
	INTEGER F77XAMINE_STARTSEARCH
	LOGICAL F77XAMINE_NEXTGATE
	INTEGER F77XAMINE_READGATE
	INTEGER F77XAMINE_POLLFORGATE
	INTEGER F77XAMINE_READ
	INTEGER F77XAMINE_WAIT

      INTEGER F77XAMINE_ALLOCMEMORY
      INTEGER F77XAMINE_ALLOCATE1D
      INTEGER F77XAMINE_ALLOCATE2D
	INTEGER F77XAMINE_QUEUEGATEIO
	INTEGER F77XAMINE_ALLOCATESPECTRUM
	INTEGER F77XAMINE_RESERVESPECTRUM
	INTEGER F77XAMINE_DEFINEBUTTONBOX
	INTEGER F77XAMINE_DEFINEBUTTON
	INTEGER F77XAMINE_MODIFYBUTTON
	INTEGER F77XAMINE_QUEUEEVENTREAD	!VMS ONLY
	INTEGER F77XAMINE_POLLFOREVENT
	INTEGER F77XAMINE_READEVENT
	INTEGER F77XAMINE_ENABLEBUTTON
	INTEGER F77XAMINE_DISABLEBUTTON
	INTEGER F77XAMINE_DELETEBUTTON
	INTEGER F77XAMINE_DELETEBUTTONBOX
	INTEGER F77XAMINE_INQUIREBUTTON
	INTEGER F77XAMINE_ENTERPEAKMARKER	

