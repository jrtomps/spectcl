DEFS = @WCHAR_T_DEF@
AM_CPPFLAGS =  -I. -I@top_srcdir@  -I@top_srcdir@/Events \
	-I@top_srcdir@/Utility -I@top_srcdir@/Sorter  \
	-I@top_srcdir@/EventSource -I@top_srcdir@/Gates \
	-I@top_srcdir@/SpectrumIO -I@top_srcdir@/Xamine -I@top_srcdir@/SpecTcl \
	-I@top_srcdir@/Analysis \
	-I@top_srcdir@/Display -I@top_srcdir@/Tape -I@top_srcdir@/Filter \
	@TCL_CPPFLAGS@ @TK_CPPFLAGS@ -DINSTALLED_IN='"$(prefix)"' @XINCLUDES@ \
	@LIBTCLPLUS_CFLAGS@

bin_PROGRAMS=ScriptedSpecTcl
lib_LTLIBRARIES=libScripted.la

ScriptedSpecTcl_SOURCES = MySpecTclApp.cpp

libScripted_la_LDFLAGS = -version-info $(SOVERSION):0

MainSources=CConfigurableObject.cpp \
	CSegmentUnpacker.cpp \
	CConfigurationParameter.cpp \
	CIntConfigParam.cpp \
	CBoolConfigParam.cpp \
	CStringConfigParam.cpp \
	CPacket.cpp \
	CModuleDictionary.cpp \
	CModule.cpp \
	CModuleCommand.cpp \
	CPacketCreator.cpp \
	CModuleCreator.cpp \
	CScriptableUnpacker.cpp \
	CCAENDigitizerCreator.cpp \
	CCAENV830Creator.cpp \
	CCAENDigitizerModule.cpp \
	CCAENV830Module.cpp CIntArrayParam.cpp \
	CStringArrayparam.cpp 

Main_Objects = CConfigurableObject.o   \
	CSegmentUnpacker.o   \
	CConfigurationParameter.o   \
	CIntConfigParam.o   \
	CBoolConfigParam.o   \
	CStringConfigParam.o   \
	CPacket.o   \
	CModuleDictionary.o   \
	CModule.o   \
	CModuleCommand.o   \
	CPacketCreator.o   \
	CModuleCreator.o   \
	CScriptableUnpacker.o   \
	CCAENDigitizerCreator.o   \
	CCAENV830Creator.o   \
	CCAENDigitizerModule.o   \
	CCAENV830Module.o   CIntArrayParam.o   \
	CStringArrayparam.o 

libScripted_la_SOURCES =  $(MainSources)





noinst_HEADERS =  CBoolConfigParam.h \
	CCAENDigitizerCreator.h \
	CCAENDigitizerModule.h \
	CCAENV830Creator.h \
	CCAENV830Module.h \
	CConfigurableObject.h \
	CConfigurationParameter.h \
	CIntArrayParam.h \
	CIntConfigParam.h \
	CModule.h \
	CModuleCommand.h \
	CModuleCreator.h \
	CModuleDictionary.h \
	CPacket.h CPacketCreator.h \
	CScriptableUnpacker.h \
	CSegmentUnpacker.h \
	CStringArrayparam.h \
	CStringConfigParam.h \
	MySpecTclApp.h \
	CConfigurableObject.h \
	CCAENV1x90Data.h


ScriptedSpecTcl_DEPENDENCIES = libScripted.la

ScriptedSpecTcl_LDFLAGS =   @top_builddir@/contrib/scriptable/libScripted.la \
	 @top_builddir@/SpecTcl/libTclGrammerCommands.la \
	@top_builddir@/Sorter/libSorting.la @top_builddir@/Xamine/libXplus.la \
	@top_builddir@/Gates/libGates.la \
	@top_builddir@/Events/libAnalysis.la \
	@top_builddir@/Filter/libFilter.la @top_builddir@/EventSource/libEventSource.la \
	@top_builddir@/SpectrumIO/libSpecio.la \
	@top_builddir@/Analysis/libBufferAnalysis.la \
	@top_builddir@/Display/libXamine.la  \
	@top_builddir@/TreeParam/libTreeParam.la \
	@top_builddir@/Fits/libFits.la \
	@top_builddir@/Tape/libtape.la \
	-lXt -lX11 $(X11EXTRA_LDFLAGS)  $(LD_EXTRA_FLAGS) \
	-lgsl -lgslcblas					\
	@top_builddir@/DecoderRing/libDecoderRing.la		\
	@LIBTCLPLUS_LDFLAGS@ \
	 @TK_LIBS@ @TCL_LIBS@ @XLIBSW@ \
	 -g -Wl,"-rpath=($prefix)/lib"

#
#  At install time, in order to make this open ended we:
#   Create $(prefix)/contrib/scripted
#          $(prefix)/contrib/scripted/src
#          $(prefix)/contrib/scripted/include
#          $(prefix)/contrib/scripted/skel
#
#    The headers all go in ...include.
#    The .cpp    all go in ...src
#    skel gets:
#                 MySpecTclApp.cpp
#                 MySpecTclApp.h
#        and an extensible  Makefile that's built from Makefile.skel
#        that will, given MySpecTclApp.cpp build the 
#        Scripted SpecTcl.  Extensible means that the user can
#        easily add additional modules to the build.
#

install-exec-local:
	$(mkinstalldirs) $(prefix)/contrib
	$(mkinstalldirs) $(prefix)/contrib/scripted
	$(mkinstalldirs) $(prefix)/contrib/scripted/src
	$(mkinstalldirs) $(prefix)/contrib/scripted/include
	$(mkinstalldirs) $(prefix)/contrib/scripted/skel
	$(INSTALL_DATA) *.cpp $(prefix)/contrib/scripted/src
	$(INSTALL_DATA) *.h   $(prefix)/contrib/scripted/include
	$(INSTALL_DATA) MySpecTclApp.cpp MySpecTclApp.h \
			      $(prefix)/contrib/scripted/skel
	echo "#-------- Begin auto generated stuff " \
	                                  > Makefile.user
	echo INSTDIR=$(prefix)           >> Makefile.user
	cat Makefile.skel                >> Makefile.user
	$(INSTALL_DATA) Makefile.user $(prefix)/contrib/scripted/skel/Makefile
	$(INSTALL_DATA) scriptable_Makefile.include $(sysconfdir)


EXTRA_DIST=Makefile.skel scriptable_Makefile.include 


if CPPUNIT

noinst_PROGRAMS = unittest

unittest_SOURCES = SegUnpackerTest.cpp ModuleTest.cpp PacketTest.cpp CreatorTest.cpp \
	UnpackerTest.cpp TestRunner.cpp

uinttest_CXXFLAGS=@CPPUNIT_CFLAGS@

unittest_DEPENDENCIES = $(Main_Objects)

unittest_LDFLAGS = $(Main_Objects) @CPPUNIT_LIBS@ \
	@top_builddir@/SpecTcl/libTclGrammerCommands.la \
	@top_builddir@/Sorter/libSorting.la \
	@top_builddir@/Xamine/libXplus.la \
	@top_builddir@/Gates/libGates.la \
	@top_builddir@/Events/libAnalysis.la \
	@top_builddir@/EventSource/libEventSource.la \
	@top_builddir@/SpectrumIO/libSpecio.la \
	@top_builddir@/Display/libXamine.la \
	@top_builddir@/Filter/libFilter.la \
	@top_builddir@/TreeParam/libTreeParam.la \
	@top_builddir@/Analysis/libBufferAnalysis.la \
	@top_builddir@/Fits/libFits.la \
	@top_builddir@/DecoderRing/libDecoderRing.la \
	@top_builddir@/Tape/libtape.la \
	@LIBTCLPLUS_LDFLAGS@ \
	$(TCL_LIBS) $(MOTIF_LIBSW) $(XLIBSW)  -lXt -lX11 -lm


#TESTS = ./unittest		#Needs Xamine to fire up :-(
endif
