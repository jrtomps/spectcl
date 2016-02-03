LDADD = -L.
# AM_LDFLAGS  = -L.

lib_LTLIBRARIES = libFilter.la

libFilter_la_LDFLAGS = -version-info $(SOVERSION):0 \
	-Wl,"-rpath-link=$(libdir)" @LIBTCLPLUS_LDFLAGS@


libFilter_la_SOURCES = EventFilter.cpp \
	FilterDictionary.cpp \
	FilterBufferDecoder.cpp \
	GatedEventFilter.cpp \
	CXdrOutputStream.cpp CXdrInputStream.cpp \
	CXdrMemInputStream.cpp	\
	CFilterOutputStage.cpp CXdrFilterOutputStage.cpp \
	CFilterOutputStageFactory.cpp \
	CXdrFilterOutputStageCreator.cpp

include_HEADERS = EventFilter.h \
		FilterDictionary.h  \
		FilterBufferDecoder.h \
		GatedEventFilter.h    \
		CXdrOutputStream.h CXdrInputStream.h \
		CXdrMemInputStream.h	\
		CFilterOutputStage.h \
		CXdrFilterOutputStage.h \
		CFilterOutputStageFactory.h \
		CFilterOutputStageCreator.h \
		CXdrFilterOutputStageCreator.h

libFilter_la_CPPFLAGS = -g  -I@srcdir@ \
	    -I@top_srcdir@/Events \
	    -I@top_srcdir@/Utility \
	    -I@top_srcdir@/Gates \
	    -I@top_srcdir@/NSCLException \
	    @TCL_CPPFLAGS@ \
	    @LIBTCLPLUS_CFLAGS@

libFilter_la_LIBADD = @top_builddir@/Events/libAnalysis.la \
		    @top_builddir@/Gates/libGates.la \
		    @LIBTCLPLUS_LDFLAGS@


bin_PROGRAMS = unittests

unittests_SOURCES = TestRunner.cpp CFilterBufferDecoderTests.cpp

unittests_CXXFLAGS = -I@srcdir@ \
		    -I@top_srcdir@/Events \
		    -I@top_srcdir@/Utility \
		    @LIBTCLPLUS_CFLAGS@ \
		    @CPPUNIT_CFLAGS@

unittests_LDADD = @builddir@/libFilter.la \
		@top_builddir@/Events/libAnalysis.la \
		@LIBTCLPLUS_LDFLAGS@ \
		@CPPUNIT_LIBS@

check-TESTS:
	./unittests
