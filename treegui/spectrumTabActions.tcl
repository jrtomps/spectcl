#    This software is Copyright by the Board of Trustees of Michigan
#    State University (c) Copyright 2009.
#
#    You may use this software under the terms of the GNU public license
#    (GPL).  The terms of this license are described at:
#
#     http://www.gnu.org/licenses/gpl.txt
#
#    Author:
#             Ron Fox
#	     NSCL
#	     Michigan State University
#	     East Lansing, MI 48824-1321

package require Tk
package require Itcl

package require spectrumContainer
package require guistate

package provide spectrumTabActions 1.0


##
#  Class to supply the actions for the spectrum tab.
#

itcl::class spectrumTabActions {
    public variable widget;	# spectrumContainer widget option.

    private variable gateAddChain
    private variable gateDeleteChain
    private variable gateChangeChain

    #-------------------------------------------------------------------------
    # Private utility methods
    
    ## Given a parameter name get the units:
    # @param name - parameter name
    # @return string 
    # @retval possibly empty string containing parameter units.
    #
    private method getUnits name {
	set def [parameter -list $name]
	if {[llength $def] > 0} {
	    set def [lindex $def 0]
	    set paramInfo [lindex $def 3]
	    return [lindex $paramInfo 2]

	}
	return {}
    }

    ##
    # Load the parameter names into the parameter menus.
    # 
    private method LoadParameters {} {
	set parameters [list]
	foreach parameter [parameter -list] {
	    lappend parameters [lindex $parameter 0]
	}
	$widget configure -parameters $parameters
    }

    ##
    # Get either selected or all spectra depending on the state of the all button.
    # @return list
    # @retval if all is checked a list of all spectrum names.
    # @retval if all is not checked a list of the names of spectr that are selected
    #         in the spectrum table.
    #
    private method getSelectedSpectra {} {
	if {[$widget cget -all]} {
	    set result [list]
	    foreach spectrum [spectrum -list] {
		lappend result [lindex $spectrum 1]
	    }
	    return $result
	} else {
	    return [$widget getSelection]
	}
    }
    ##
    # Determine if any of a list of items is null.
    # @param list - list of items
    # @return boolean
    # @return true - if at least one item is null.
    # @return false - if no items are null.
    #
    private method anyNulls list {
	foreach item $list {
	    if {$item eq ""} {
		return 1
	    }
	}
	return 0
    }
    ##
    # Generate a new spectrum name based on an existing one.
    # There is an assumption we will make... That already duplicated spectra
    # will be of the form name_integer.  There fore if a spectrum breask up into
    # a list of _ separated components with size > 1, and the last element is an integer
    # we should generate the spectra by incrementing the digits until we come up with one that
    # results in a spectrum name that is not yet used.  Otherwise just append _1 to the 
    # initial name...incrementing that until we get uniqueness.
    #
    #
    # @param baseName - The initial name of the spectrum.
    # @return string
    # @retval - a spectrum name  that is not yet in use.
    #
    private method generateUniqueSpectrumName baseName {
	
	# Figure out what the base part of the name is and the 
	# trailing integer should start with.
	#
	set baseList [split $baseName _]
	if {([llength $baseList] > 1) && ([string is integer [lindex $baseList end]])} {
	    set counter [lindex $baseList end]
	    set baseList [lrange $baseList 0 end-1]; #  lop off the last element.
	    incr counter;			     # and start hunting with the next integer.
	} else {
	    set counter 1
	}
	# Now hunt for a unique name by successively incrementing counter

	while 1 {
	    set candidateName [join [concat $baseList $counter] _]
	    set info [spectrum -list $candidateName]
	    if {[llength $info] == 0} {
		return $candidateName
	    } else {
		incr counter
	    }
	}
	
	
    }
    ##
    # True if the user says its ok to replace a spectrum:
    # @param name - the spectrum name.
    # @return bool
    # @retval true if the user accepts.
    #
    private method okToReplaceSpectrum name {
	if {[llength [spectrum -list $name]] > 0} {
	    if {[tk_messageBox -default cancel -icon warning -parent $widget \
		     -title Overwite -type okcancel \
		     -message "$name already exists replace?"] eq "ok"} {
		return 1
	    } else {
		return 0
	    }
	} else {
	    # It's always ok to replace a nonexistent spectrum:

	    return 1
	}
    }

    #--------------------------------------------------------------------------
    # Call back methods.  These are, by necesity public thought not really part of
    # the public interface.
    #

    ##
    # Save the configuration
    # @param file   - name of the file to save it to.
    #
    public method SaveConfiguration {file} {
	set fd [open $file w]

	# MC Gui does not emit spectrum deletes....
	
	set prior $::guistate::writeDeletes
	set ::guistate::writeDeletes 0

	# write the definition file.

	writeAll $fd
	close $fd

	# Restore the write deletes flag back to what it was.

	set ::guistate::writeDeletes $prior


    }
    ##
    # Read the configuration the 'cumulate' sic checkbutton is used to determine
    # if the current defintions are wiped out first.
    # @param name   - filename from which the configuration is loaded.
    # @param widget - the defintionFile widget (has the -accumulate option we can query)
    #                 that triggered this.
    #
    public method ReadConfiguration {name widget} {
	set noclear [$widget cget -accumulate]

	# If noclear is not set we need to destroy the spectra as redefinition
	# is an error:

	if {!$noclear} {
	    set spectra [spectrum -list]
	    foreach spectrum $spectra {
		set sname [lindex $spectrum 1]
		spectrum -delete $sname
	    }
	}
	# Now we can read the file:
	#
	uplevel #0 source $name
	
	
    }
    ##
    # Load the list of spectra from the current mask 
    # @param mask - glob pattern that determines the set of spectra to load.
    #
    public method LoadSpectra mask {
	set spectra [spectrum -list -showgate $mask]
	set spectrumList [list]; # Build up the data here:

	# Pull each definition apart and add it to spectrumList.
	
	foreach spectrum $spectra {
	    set name [lindex $spectrum 1]
	    set type [lindex $spectrum 2]
	    set parameters [lindex $spectrum 3]
	    set axes       [lindex $spectrum 4]
	    set gate       [lindex $spectrum 6]
	    
	    # Ungated true gate -> ""
	    if {$gate eq "-TRUE-" || $gate eq "-Ungated-"} {
		set gate ""
	    }

	    set xparam [lindex [lindex $parameters 0] 0]
	    set yparam [lindex [lindex $parameters 1] 0]

	    set xaxis [lindex $axes 0]
	    set xlow [lindex $xaxis 0]
	    set xhi  [lindex $xaxis 1]
	    set xbins [lindex $xaxis 2]

	    set yaxis [lindex $axes 1]
	    if {[llength $yaxis] > 0} {
		set ylow [lindex $yaxis 0]
		set yhi  [lindex $yaxis 1]
		set ybins [lindex $yaxis 2]
		
	    } else {
		set ylow ""
		set yhi  ""
		set ybins ""
	    }

	    lappend spectrumList [list $name $type              \
				      $xparam $xlow $xhi $xbins \
				      $yparam $ylow $yhi $ybins \
				      $gate]

	}
	$widget configure -spectra $spectrumList
    }
    ##
    # Called in response to the button to clear spectra.
    # If the -all option is true we clear all of the spectra otherwise
    # only the spectra selected in the spectrum table are cleared.
    #
    public method ClearSpectra {} {
	if {[$widget cget -all]} {
	    clear -all
	} else {
	    ::treeutility::for_each clear [$widget getSelection]
	}
    }
    ##
    # Called in response to the button to delete spectra.
    #
    public method DeleteSpectra {} {
	if {[$widget cget -all]} {
	    spectrum -delete -all
	} else {
	    ::treeutility::for_each [list spectrum -delete] [$widget getSelection]
	}
	LoadSpectra [$widget cget -mask]
    }
    # Duplicate a spectrum:
    # - Assign a  unique name that starts like the existing spectrum.
    # - Get the spectrum defintion.
    # - Create the new spectrum
    # - bind it to the display.
    # @param name - Name of the existing spectrum to duplicate.
    #
    public method duplicateSpectrum name {
	set newName [generateUniqueSpectrumName $name]
	set def     [spectrum -list $name]

	# bypass everything if there are no matching spectra.  This can happen if the
	# spectrum was deleted bu tthe display not updated.
	#
	if {[llength $def] > 0} {
	    set def [lindex $def 0]; # The actual definition.

	    set type     [lindex $def 2]
	    set param    [lindex $def 3]
	    set axes     [lindex $def 4]
	    set dataType [lindex $def 5]

	    spectrum $newName $type $param $axes $dataType
	}
    }
    ##
    #  Called in response to the button to duplicate spectra.
    #  We're going to use the following private methods:
    #  getSelectedSpectra - Gets the list of spectra to operate on.
    #  duplicateSpectrum - Duplicates a single spetrum.
    #
    public method DupSpectra {} {

	::treeutility::for_each [list $this duplicateSpectrum] [getSelectedSpectra]
	LoadSpectra [$widget cget -mask]
    }

    ##
    # Called in response to the ungate button.  Ungates either the selected
    # or all spectra depending on the state of the all checkbutton.
    #
    public method UngateSpectra {} {
	set spectra [getSelectedSpectra]
	if {[llength $spectra] != 0} {
	    ungate {*}$spectra
	    LoadSpectra [$widget cget -mask]

	}

    }
    ## 
    #  Gate traces...these are separated to support
    #  chaining.
    #  @param name - the name of the gate affected.

    public method gateAdded name {
	LoadGateMenu
	if {$gateAddChain ne ""} {
	    uplevel #0 $gateAddChain $name
	}
    }
    public method gateDeleted name {
	LoadGateMenu
	if {$gateDeleteChain ne ""} {
	    uplevel #0 $gateDeleteChain $name
	}
    }
    public method gateChanged name {
	LoadGateMenu
	if {$gateChangeChain ne ""} {
	    uplevel #0 $gateChangeChain $name
	}
    }

    ##
    # Called to refresh the contents of the gate menu.
    #
    public method LoadGateMenu {} {
	set gates [list]
	foreach gate [gate -list] {
	    lappend gates [lindex $gate 0]
	}
	$widget configure -gates $gates
    }
    ## 
    # Whenever a gate is selected its full path is put in the entry below the menu:
    # @param name - full name of the gate.
    #
    public method Selectgate name {
	$widget configure -gate $name
    }
    
    ##
    #  Apply gates to spectra.
    #
    public method ApplyGates {} {
	set spectra [getSelectedSpectra]
	set gate    [$widget cget -gate]
	if {[llength $spectra] != 0} {
	    apply $gate {*}$spectra
	}
	LoadSpectra [$widget cget -mask]
    }

    ##
    # Select a spectrum and load it into the spectrum definition fields.
    #
    # @param name - The name of the spectrum.
    #
    public method SelectSpectrum name {
	set def [spectrum -list $name]

	# If the spectrum became undefined do nothing:

	if {[llength $def] > 0} {
	    set def      [lindex $def 0]
	    set name     [lindex $def 1]
	    set type     [lindex $def 2]
	    set params   [lindex $def 3]
	    set axes     [lindex $def 4]
	    set datatype [lindex $def 5]

	    set xParam [lindex $params 0]
	    set xAxis  [lindex $axes 0]
	    set xlow [lindex $xAxis 0]
	    set xhi  [lindex $xAxis 1]
	    set xbins [lindex $xAxis 2]


	    $widget configure -spectrumtype $type \
		-datatype $datatype               \
		-spectrumname $name               \
		-xparameter $xParam               \
		-xlow       $xlow                 \
		-xhi        $xhi                  \
		-xbins      $xbins

	    # Figure out units

	    $widget configure -xunits [getUnits $xParam]

	    if {[llength $params] > 1} {
		set yparam [lindex $params 1]
		set yaxis  [lindex $axes 1]
		set ylow   [lindex $yaxis 0]
		set yhi    [lindex $yaxis 1]
		set ybins  [lindex $yaxis 2]

		$widget configure -yparameter $yparam \
		    -ylow $ylow -yhi $yhi -ybins $ybins \
		    -yunits [getUnits $yparam] -ystate normal
	    } else {
		$widget configure -ystate disabled
	    }

	}
    }
    ##
    # Load a parameter into one of the axis widgets along with the suggested
    # values (if this is a tree parameter).
    # @param which - x or y - selects which of the  parameter widgets to load.
    # @param name           - Name of the parameter.
    #
    public method LoadParameter {which name} {
	#
	# If there's a tree parameter by that name use it:

        set tdef [treeparameter -list $name]
	if {[llength $tdef] > 0} {
	    set tdef [lindex $tdef 0]
	    set bins [lindex $tdef 1]
	    set lo   [lindex $tdef 2]
	    set hi   [lindex $tdef 3]
	    set units [lindex $tdef 5]
	    

	} else {
	    # Otherwise if there's a parameter by that name use what we can from it:
	    
	    set pdef [parameter -list $name]
	    if {[llength $pdef] > 0} {
		set pdef [lindex $pdef 0]
		set info [lindex $pdef 3]
		set bins [list]
		set lo   [lindex $info 0]
		set hi   [lindex $info 1]
		set units [lindex $info 2]

	    } else {
		# no such parameter so exit out:
		return
	    }
	}
	# If we got here the name, lo, hi, bins and units are set:

	$widget configure \
	    -${which}parameter $name \
	    -${which}low       $lo   \
	    -${which}hi        $hi   \
	    -${which}bins      $bins \
	    -${which}units     $units 


    }
    ##
    #  The spectrum type changed..figure out what the state of the y axis should be.
    #  only if it's 2, or S (stripchart) should we enable it:
    #
    public method ChangeSpectype {} {
	set type [$widget cget -spectrumtype]
	if {($type eq 2) || ($type eq "S")} {
	    $widget configure -ystate normal
	} else {
	    $widget configure -ystate disabled
	}
    }
    ## Invoked to create a spectrum.
    #
    public method CreateSpectrum {} {
	# If the spectrum exists prompt for redef:

	#
	#  We must have at least the following:
	# - spectrum name
	# - spectrum type
	# - Xaxis.   The rest depends on the type of spectrum.

	set type     [$widget cget -spectrumtype]
	set datatype [$widget cget -datatype]
	set name     [$widget cget -spectrumname]

	set xname    [$widget cget -xparameter]
	set xlow     [$widget cget -xlow]
	set xhi      [$widget cget -xhi]
	set xbins    [$widget cget -xbins]

	# Do nothing if any of the above are empty:

	if {[anyNulls [list $type $datatype $name $xname $xlow $xhi $xbins]]} {
	    return
	}


	# What happens next depends entirely on the spectrum type
	# Bitmask and 1d define essentially the same.
	# 2d and Stripchart need different defs.
	#

	switch -exact -- $type {
	    1 - b {
		if {[okToReplaceSpectrum $name]} {
		    catch {spectrum -delete $name}; # get rid of any prior spectrum.
		    spectrum $name $type $xname [list [list $xlow $xhi $xbins]] $datatype
		}
	    }
	    S {
		# Need a y parameter too:
		
		set yname [$widget cget -yparameter]

		if {($yname ne "") && [okToReplaceSpectrum $name]} {
		    catch {spectrum -delete $name}
		    spectrum $name $type [list $xname $yname]  [list [list $xlow $xhi $xbins]] $datatype
		    sbind $name
		}
	    }
	    2 {
		# Need y parameter and axis definitions.

		set yname [$widget cget -yparameter]
		set ylow  [$widget cget -ylow]
		set yhi   [$widget cget -yhi]
		set ybins [$widget cget -ybins]

		if {![anyNulls [list $yname $ylow $yhi $ybins]] && [okToReplaceSpectrum $name]} {
		    catch {spectrum -delete $name}
		    spectrum $name $type [list $xname $yname] \
			[list [list $xlow $xhi $xbins] [list $ylow $yhi $ybins]] $datatype
		    sbind $name
		}
	    }
	    default {
		tk_messageBox -type ok -icon error -title {Can't make this spectrum} \
		    -parent $widget \
		    -message "The tree gui does not know how to create spectra of type: $type"
		    sbind $name
	    }
	}
		      
	LoadSpectra [$widget cget -mask]	  
    }

    #---------------------------------------------------------------------------
    # True public interface.  There are other public methods but they
    # require that exposure to be used as callbacks.
    #
 

    ##
    # Construct the object and view:
    # @paramargs - option/value pairs.. -widget is required.
    #
    constructor args {
	configure {*}$args

	if {$widget eq ""} {
	    error "The -widget option is mandatory"
	}

	spectrumContainer $widget                           \
	    -ystate disabled                                \
	    -savecmd   [list $this SaveConfiguration %N]    \
	    -loadcmd   [list $this ReadConfiguration %N %W] \
	    -updatecmd [list $this LoadSpectra %M]          \
	    -clearcmd  [list $this ClearSpectra]            \
	    -deletecmd [list $this DeleteSpectra]           \
	    -dupcmd    [list $this DupSpectra]              \
	    -ungatecmd [list $this UngateSpectra]           \
	    -gateselectcmd [list $this Selectgate %N]       \
	    -applycmd      [list $this ApplyGates]          \
	    -selectcmd     [list $this SelectSpectrum %N]   \
	    -typechanged   [list $this ChangeSpectype]      \
	    -xparamselected [list $this LoadParameter x %N]    \
	    -yparamselected [list $this LoadParameter y %N] \
	    -createcmd      [list $this CreateSpectrum]


	LoadParameters 

	LoadSpectra [$widget cget -mask]

	# Load the gate menu and set it up to reload each time gates change in any way:

	LoadGateMenu

	set gateAddChain    [gate -trace add    [list $this gateAdded]]
	set gateDeleteChain [gate -trace delete [list $this gateDeleted]]
	set gateChangeChain [gate -trace change [list $this getChanged]]
	

	
    }
}