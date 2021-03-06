# Tcl package index file, version 1.1

# This file is generated by the MakeIndex script in the tclhttpd 
# lib directory.  The goal is to not pollute the package namespace
# until you package require httpd

package ifneeded httpd 1.7 "
	source \[file join [list $dir] httpd.tcl\]
	package ifneeded httpd::admin 1.0 \[list source \[file join [list $dir] admin.tcl\]\]
	package ifneeded httpd::auth 2.0 \[list source \[file join [list $dir] auth.tcl\]\]
	package ifneeded httpd::cgi 1.1 \[list source \[file join [list $dir] cgi.tcl\]\]
	package ifneeded httpd::compat 3.3 \[list source \[file join [list $dir] compat.tcl\]\]
	package ifneeded httpd::config 1.0 \[list source \[file join [list $dir] config.tcl\]\]
	package ifneeded httpd::cookie 1.0 \[list source \[file join [list $dir] cookie.tcl\]\]
	package ifneeded httpd::counter 2.0 \[list source \[file join [list $dir] counter.tcl\]\]
	package ifneeded httpd::debug 1.0 \[list source \[file join [list $dir] debug.tcl\]\]
	package ifneeded httpd::digest 1.0 \[list source \[file join [list $dir] digest.tcl\]\]
	package ifneeded httpd::direct 1.1 \[list source \[file join [list $dir] direct.tcl\]\]
	package ifneeded httpd::dirlist 1.1 \[list source \[file join [list $dir] dirlist.tcl\]\]
	package ifneeded httpd::doc 1.1 \[list source \[file join [list $dir] doc.tcl\]\]
	package ifneeded httpd::doc_error 1.0 \[list source \[file join [list $dir] doc_error.tcl\]\]
	package ifneeded httpd::doctools 1.0 \[list source \[file join [list $dir] doctools.tcl\]\]
	package ifneeded httpd::fallback 1.0 \[list source \[file join [list $dir] fallback.tcl\]\]
	package ifneeded httpd::imagemap 1.0 \[list source \[file join [list $dir] imagemap.tcl\]\]
	package ifneeded httpd::include 1.0 \[list source \[file join [list $dir] include.tcl\]\]
	package ifneeded httpd::ismaptcl 1.0 \[list source \[file join [list $dir] maptcl.tcl\]\]
	package ifneeded httpd::ismaptk 1.0 \[list source \[file join [list $dir] maptk.tcl\]\]
	package ifneeded httpd::log 1.1 \[list source \[file join [list $dir] log.tcl\]\]
	package ifneeded httpd::logstd 1.0 \[list source \[file join [list $dir] logstd.tcl\]\]
	package ifneeded httpd::mail 1.0 \[list source \[file join [list $dir] mail.tcl\]\]
	package ifneeded httpd::mtype 1.1 \[list source \[file join [list $dir] mtype.tcl\]\]
	package ifneeded httpd::opentrace 1.0 \[list source \[file join [list $dir] open.tcl\]\]
	package ifneeded httpd::passcheck 1.0 \[list source \[file join [list $dir] passcheck.tcl\]\]
	package ifneeded httpd::passgen 1.0 \[list source \[file join [list $dir] passgen.tcl\]\]
	package ifneeded httpd::redirect 1.0 \[list source \[file join [list $dir] redirect.tcl\]\]
	package ifneeded httpd::safecgio 1.0 \[list source \[file join [list $dir] safecgio.tcl\]\]
	package ifneeded httpd::session 1.0 \[list source \[file join [list $dir] session.tcl\]\]
	package ifneeded httpd::srvui 1.0 \[list source \[file join [list $dir] srvui.tcl\]\]
	package ifneeded httpd::status 1.0 \[list source \[file join [list $dir] status.tcl\]\]
	package ifneeded httpd::stdin 1.1 \[list source \[file join [list $dir] stdin.tcl\]\]
	package ifneeded httpd::subst 1.0 \[list source \[file join [list $dir] subst.tcl\]\]
	package ifneeded httpd::template 1.0 \[list source \[file join [list $dir] template.tcl\]\]
	package ifneeded httpd::threadmgr 1.0 \[list source \[file join [list $dir] thread.tcl\]\]
	package ifneeded httpd::upload 1.0 \[list source \[file join [list $dir] upload.tcl\]\]
	package ifneeded httpd::url 1.2 \[list source \[file join [list $dir] url.tcl\]\]
	package ifneeded httpd::utils 1.0 \[list source \[file join [list $dir] utils.tcl\]\]
	package ifneeded httpd::version 3.5 \[list source \[file join [list $dir] version.tcl\]\]
	package ifneeded tclcrypt 1.0 \[list source \[file join [list $dir] tclcrypt.tcl\]\]
"
