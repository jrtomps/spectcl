lib_LTLIBRARIES=libCompress.la

libCompress_la_SOURCES=deflate.cpp deflate.h inflate.cpp inflate.h \
	compressPackage.cpp

libCompress_la_CXXFLAGS=@LIBTCLPLUS_CFLAGS@ @TCL_CPPFLAGS@
libCompress_la_LIBADD=@LIBTCLPLUS_LDFLAGS@ @TCL_LIBS@ -lz

install-data-hook:
	$(mkinstalldirs) @prefix@/TclLibs/compress
	$(INSTALL_PROGRAM) @libdir@/libCompress.so  @prefix@/TclLibs/compress
	echo pkg_mkIndex @prefix@/TclLibs/compress "*.so" | @TCLSH@