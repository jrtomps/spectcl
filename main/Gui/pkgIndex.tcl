# Tcl package index file, version 1.1
# This file is generated by the "pkg_mkIndex" command
# and sourced either when an application starts up or
# by a "package unknown" script.  It invokes the
# "package ifneeded" command to set up package-related
# information so that packages will be loaded automatically
# in response to "package require" commands.  When this
# script is sourced, the variable $dir must contain the
# full path name of this file's directory.

package ifneeded browser      1.0 [list source [file join $dir browser.tcl]]
package ifneeded SpecTclGui   1.0 [list source [file join $dir newGui.tcl]]
package ifneeded pseudo       1.0 [list source [file join $dir pseudo.tcl]]
package ifneeded gate         1.0 [list source [file join $dir gate.tcl]]
package ifneeded compoundgate 1.0 [list source [file join $dir compoundgate.tcl]]
package ifneeded c2band       1.0 [list source [file join $dir c2band.tcl]]
package ifneeded maskeditor   1.0 [list source [file join $dir maskeditor.tcl]]
package ifneeded sliceditor   1.0 [list source [file join $dir slice.tcl]]
package ifneeded multipointeditor 1.0 [list source [file join $dir multipoint.tcl]]
package ifneeded spectrum     1.0 [list source [file join $dir spectrum.tcl]]
package ifneeded edit1d       1.0 [list source [file join $dir edit1d.tcl]]
package ifneeded edit2d       1.0 [list source [file join $dir edit2d.tcl]]
package ifneeded editmulti    1.0 [list source [file join $dir editmulti.tcl]]
package ifneeded editstrip    1.0 [list source [file join $dir editstrip.tcl]]
package ifneeded editvariable 1.0 [list source [file join $dir editvariable.tcl]]
package ifneeded guiutilities 1.0 [list source [file join $dir guiutilities.tcl]]
package ifneeded editparameter 1.0 [list source [file join $dir editparameter.tcl]]
package ifneeded guihelp      1.0 [list source [file join $dir guihelp.tcl]]
package ifneeded guistate     1.0 [list source [file join $dir guistate.tcl]]
package ifneeded applygate    1.0 [list source [file join $dir applygate.tcl]]
package ifneeded datasource   1.0 [list source [file join $dir datasource.tcl]]
package ifneeded filtercontrol 1.0 [list source [file join $dir filtercontrol.tcl]]

