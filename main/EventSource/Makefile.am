AM_CPPFLAGS = -I. -I../Utility -I../NSCLException  -I../Tape -I../SpecTcl \
	-I../Events -I../TCL -I../Filter @TCL_CPPFLAGS@ @LIBTCLPLUS_CFLAGS@

LDADD=-L. @LIBTCLPLUS_LDFLAGS@ @TCL_LIBS@ @TK_LIBS@


lib_LTLIBRARIES = libEventSource.la
libEventSource_la_LDFLAGS = -version-info $(SOVERSION):0 \
	-Wl,"-rpath-link=$(libdir)" -L. @LIBTCLPLUS_LDFLAGS@ @TCL_LIBS@ @TK_LIBS@

libEventSource_la_SOURCES = File.cpp GaussianDistribution.cpp \
	TapeException.cpp TapeFile.cpp TestFile.cpp PipeFile.cpp  \
	MultiTestSource.cpp CalibrationDistribution.cpp

include_HEADERS = DiskFile.h Distribution.h File.h GaussianDistribution.h \
	PipeFile.h TapeException.h TapeFile.h TestFile.h  MultiTestSource.h \
	CalibrationDistribution.h

EXTRA_DIST = classbreakdown.WC classbreakdown.omt
