# SpecTclGui.tcl
# top level of SpecTcl Tcl/Tk GUI interface
# Author: D. Bazin
# Date: Aug 2001 - Sept 2002
# Version 1.2 for SpecTcl 2.1: November 2003

proc CheckVersion {} {
	set version [treeparameter -version]
	if {[string compare $version "1.2"] == 0} {
		return
	}
	InformMessage $version
	exit
}

proc InformMessage {v} {
	text .thetext
	.thetext configure -font "Times -14"
	.thetext insert end "This GUI can only function with version 1.2 of the TreeParameter classes.\n"
	.thetext insert end "Your program was compiled with version $v of the TreeParameter classes.\n"
	.thetext insert end "Please make sure you are using SpecTcl version 2.1 and \
								are including the correct TreeParameter.h and TreeVariable.h files.\n"
	.thetext insert end "The default path for these files is the following:\n"
	.thetext insert end "<../contrib/treeparam/TreeParameter.h>\n"
	.thetext insert end "<../contrib/treeparam/TreeVariable.h>\n\n"
	.thetext insert end "D. Bazin - November 2003"
	pack .thetext -expand 1 -fill both
	tk_messageBox -message "Acknowledge the message"
}

proc CheckFiles {} {
	global TreeParameterHome
	set fileList "\
	mclistbox.tcl \
	notebook.tcl \
	tabnbook.tcl \
	TreeParameter.tcl \
	SpectrumGenerator.tcl \
	ParameterManipulator.tcl \
	TreeVariable.tcl \
	VariableManipulator.tcl \
	GateGenerator.tcl \
	"
	foreach f $fileList {
		if {[file exists $TreeParameterHome/$f] == 0} {
			tk_messageBox -message "Incorrect TreeParameter installation. \
File $f is missing in directory $TreeParameterHome"
			exit
		}
	}
}

if {![info exist TreeParameterHome]} {
	set TreeParameterHome $SpecTclHome/contrib/treeparam
}

source $TreeParameterHome/mclistbox.tcl
source $TreeParameterHome/notebook.tcl
source $TreeParameterHome/tabnbook.tcl

source $TreeParameterHome/TreeParameter.tcl

source $TreeParameterHome/SpectrumGenerator.tcl

source $TreeParameterHome/ParameterManipulator.tcl

source $TreeParameterHome/TreeVariable.tcl

source $TreeParameterHome/VariableManipulator.tcl

source $TreeParameterHome/GateGenerator.tcl

CheckVersion

CheckFiles

GenerateMenuBitmaps
puts "Building SpecTcl GUI ..."
update

toplevel .gui
wm title .gui "SpecTcl 2.1 Graphical User Interface"
tabnotebook_create .gui.main
pack .gui.main -expand 1 -fill both
SetupSpectrumGenerator .gui.main
SetupParameterManipulator .gui.main
SetupVariableManipulator .gui.main
SetupGateGenerator .gui.main
tabnotebook_display .gui.main Gates
tabnotebook_display .gui.main Variables
tabnotebook_display .gui.main Parameters
tabnotebook_display .gui.main Spectra
trace variable spectrumParameterX w SpectrumParameterXCommand
trace variable spectrumParameterY w SpectrumParameterYCommand
for {set i 1} {$i <= 20} {incr i} {
	trace variable parameter(Name$i) w "MenuLoadParameter $i"
	trace variable variable(Name$i) w "MenuLoadVariable $i"
}
foreach v [treevariable -list] {
	set vName [lindex [lindex $v 0] 0]
	trace variable $vName w SetChanged
}
global spectrumMask gateMask
trace variable spectrumMask w DynamicSpectrumList
trace variable gateMask w DynamicGateList
puts "SpecTcl GUI loaded."


