/*
    Gri - A language for scientific graphics programming
    Copyright (C) 2008 Daniel Kelley

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#if !defined(_tags_h_)

#define _tags_hh_
#include "gr.hh"
bool	assertCmd(void),
	cdCmd(void),
	closeCmd(void),
	convert_col_fro_pol_to_recCmd(void),
	convert_col_fro_rec_to_polCmd(void),
	convert_col_to_gridCmd(void),
	convert_col_to_splineCmd(void),
	convert_grid_to_columnsCmd(void),
	convert_grid_to_imageCmd(void),
	convert_image_to_gridCmd(void),
	debugCmd(void),
	deleteCmd(void),
	differentiateCmd(void),
	draw_arcCmd(void),
	draw_arrow_from_toCmd(void),
	draw_arrowsCmd(void),
	draw_axes_if_needed(void),
	draw_axesCmd(void),
	draw_boxCmd(void),
	draw_box_filledCmd(void),
	draw_circleCmd(void),
	draw_contourCmd(void),
	draw_curveCmd(void),
	draw_gri_logoCmd(void),
	draw_gridCmd(void),
	draw_imageCmd(void),
	draw_image_histogramCmd(void),
	draw_image_paletteCmd(void),
	draw_isopycnalCmd(void),
	draw_labelCmd(void),
	draw_line_from_toCmd(void),
	draw_linesCmd(void),
	draw_patchesCmd(void),
	draw_polygonCmd(void),
	draw_symbolCmd(void),
	draw_titleCmd(void),
	draw_valuesCmd(void),
	draw_x_axisCmd(void),
	draw_x_box_plotCmd(void),
	draw_y_axisCmd(void),
	draw_y_box_plotCmd(void),
	draw_zero_lineCmd(void),
	end_groupCmd(void),
	expectingCmd(void),
	filter_columnCmd(void),
	filter_gridCmd(void),
	filter_imageCmd(void),
	flipCmd(void),
	get_envCmd(void),
	get_optionsCmd(void),
	groupCmd(void),
	healCmd(void),
	helpCmd(void),
	ignoreCmd(void),
	inputCmd(void),
	insertCmd(void),
	interpolateCmd(void),
	listCmd(void),
	lsCmd(void),
	maskCmd(void),
	newCmd(void),
	new_pageCmd(void),
	new_postscript_fileCmd(void),
	nullCmd(void),
	openCmd(void),
	postscriptCmd(void),
	pwdCmd(void),
	queryCmd(void),
	quitCmd(void),
	read_colornamesCmd(void),
	read_columnsCmd(void),
	read_from_filenameCmd(void),
	read_gridCmd(void),
	read_imageCmd(void),
	read_image_colorscaleCmd(void),
	read_image_grayscaleCmd(void),
	read_image_maskCmd(void),
	read_image_mask_rasterfileCmd(void),
	read_image_pgmCmd(void),
	read_image_rasterfileCmd(void),
	read_synonym_or_variableCmd(void),
	read_lineCmd(void),
	regressCmd(void),
	reorder_columnsCmd(void),
	rpnfunctionCmd(void),
	rescaleCmd(void),
	rewindCmd(void),
	set_axes_styleCmd(void),
	set_arrow_sizeCmd(void),
	set_arrow_typeCmd(void),
	set_beepCmd(void),
	set_bounding_boxCmd(void),
	set_clipCmd(void),
	set_colorCmd(void),
	set_colornameCmd(void),
	set_contour_formatCmd(void),
	set_contour_labelCmd(void),
	set_contour_labelsCmd(void),
	set_dashCmd(void),
	set_environment(void),
	set_error_actionCmd(void),
	set_flagCmd(void),
	set_font_colorCmd(void),
	set_font_encodingCmd(void),
	set_font_sizeCmd(void),
	set_font_toCmd(void),
	set_graylevelCmd(void),
	set_grid_missingCmd(void),
	set_image_colorscaleCmd(void),
	set_image_grayscale_using_hist(void),
	set_image_grayscaleCmd(void),
	set_image_missingCmd(void),
	set_image_rangeCmd(void),
	set_ignore_initial_newlineCmd(void),
	set_ignoreCmd(void),
	set_input_data_windowCmd(void),
	set_input_data_separatorCmd(void),
	set_line_capCmd(void),
	set_line_joinCmd(void),
	set_line_widthCmd(void),
	set_missing_valueCmd(void),
	set_pageCmd(void),
	set_page_sizeCmd(void),
	set_pathCmd(void),
	set_postscript_filenameCmd(void),
	set_symbol_sizeCmd(void),
	set_ticsCmd(void),
	set_tic_sizeCmd(void),
	set_transparencyCmd(void),
	set_u_scaleCmd(void),
	set_v_scaleCmd(void),
	set_traceCmd(void),
	set_x_axisCmd(void),
	set_x_formatCmd(void),
	set_x_gridCmd(void),
	set_x_marginCmd(void),
	set_x_nameCmd(void),
	set_x_sizeCmd(void),
	set_x_typeCmd(void),
	set_y_axisCmd(void),
	set_y_axis_nameCmd(void),
	set_y_axisCmd(void),
	set_y_formatCmd(void),
	set_y_gridCmd(void),
	set_y_marginCmd(void),
	set_y_nameCmd(void),
	set_y_sizeCmd(void),
	set_y_typeCmd(void),
	set_z_missingCmd(void),
	setCmd(void), 
	show_allCmd(void),
	show_axesCmd(void),
	show_colorCmd(void),
	show_colornamesCmd(void),
	show_columnsCmd(void),
	show_expression_or_stringCmd(void),
	show_flagsCmd(void),
	show_gridCmd(void),
	show_imageCmd(void),
	show_licenseCmd(void),
	show_miscCmd(void),
	show_next_lineCmd(void),
	show_stopwatchCmd(void),
	show_synonymsCmd(void),
	show_timeCmd(void),
	show_tracebackCmd(void),
	show_variablesCmd(void),
	smoothCmd(void),
	sourceCmd(void),
	skipCmd(void),
	sleepCmd(void),
	sprintfCmd(void),
	stateCmd(void),
	systemCmd(void),
	superuserCmd(void),
        unlinkCmd(void),
	whileCmd(void),
	writeCmd(void);


typedef struct {
	bool(*fcn) ();
	const char *name;
	unsigned int name_len;	// speeds finding the command
}               C_FCN;

C_FCN           c_fcn[] = {
	{assertCmd,			"assertCmd",					 9},
	{cdCmd,				"cdCmd",					 5},
	{closeCmd,			"closeCmd",					 8},
	{convert_col_to_gridCmd,	"convert_columns_to_gridCmd",			26},
	{convert_col_to_splineCmd,	"convert_columns_to_splineCmd",			28},
	{convert_grid_to_columnsCmd,	"convert_grid_to_columnsCmd",			26},
	{convert_grid_to_imageCmd,	"convert_grid_to_imageCmd",			24},
	{convert_image_to_gridCmd,	"convert_image_to_gridCmd",			24},
	{debugCmd,			"debugCmd",					 8},
	{deleteCmd,			"deleteCmd",					 9},
	{differentiateCmd,		"differentiateCmd",				16},
	{draw_arcCmd,                   "draw_arcCmd",                                  11},
	{draw_arrow_from_toCmd,		"draw_arrow_from_toCmd",			21},
	{draw_arrowsCmd,		"draw_arrowsCmd",				14},
	{draw_axes_if_needed,		"draw_axes_if_needed",				19},
	{draw_axesCmd,			"draw_axesCmd",					12},
	{draw_boxCmd,			"draw_boxCmd",					11},
	{draw_box_filledCmd,		"draw_box_filledCmd",				18},
	{draw_circleCmd,		"draw_circleCmd",				14},
	{draw_contourCmd,		"draw_contourCmd",				15},
	{draw_curveCmd,			"draw_curveCmd",				13},
	{draw_gri_logoCmd,		"draw_gri_logoCmd",				16},
	{draw_gridCmd,			"draw_gridCmd",					12},
	{draw_imageCmd,			"draw_imageCmd",				13},
	{draw_image_histogramCmd,	"draw_image_histogramCmd",			23},
	{draw_image_paletteCmd,		"draw_image_paletteCmd",			21},
	{draw_isopycnalCmd,		"draw_isopycnalCmd",				17},
	{draw_labelCmd,			"draw_labelCmd",				13},
	{draw_line_from_toCmd,		"draw_line_from_toCmd",				20},
	{draw_linesCmd,			"draw_linesCmd",				13},
	{draw_patchesCmd,		"draw_patchesCmd",				15},
	{draw_polygonCmd,		"draw_polygonCmd",				15},
	{draw_symbolCmd,		"draw_symbolCmd",				14},
	{draw_titleCmd,			"draw_titleCmd",				13},
	{draw_valuesCmd,		"draw_valuesCmd",				14},
	{draw_x_axisCmd,		"draw_x_axisCmd",				14},
	{draw_x_box_plotCmd,		"draw_x_box_plotCmd",				18},
	{draw_y_axisCmd,		"draw_y_axisCmd",				14},
	{draw_y_box_plotCmd,		"draw_y_box_plotCmd",				18},
	{draw_zero_lineCmd,		"draw_zero_lineCmd",				17},
	{end_groupCmd,		        "end_groupCmd",				        12},
	{expectingCmd,			"expectingCmd",					12},
	{filter_columnCmd,		"filter_columnCmd",				16},
	{filter_gridCmd,		"filter_gridCmd",				14},
	{filter_imageCmd,		"filter_imageCmd",				15},
	{flipCmd,			"flipCmd",					 7},
	{get_envCmd,			"get_envCmd",					10},
	{get_optionsCmd,		"get_optionsCmd",				14},
	{groupCmd,		        "groupCmd",				         8},
	{healCmd,			"healCmd",					 7},
	{helpCmd,			"helpCmd",					 7},
	{ignoreCmd,			"ignoreCmd",					 9},
	{inputCmd,			"inputCmd",					 8},
	{insertCmd,			"insertCmd",					 9},
	{interpolateCmd,		"interpolateCmd",				14},
	{listCmd,			"listCmd",					 7},
	{lsCmd,				"lsCmd",					 5},
	{maskCmd,			"maskCmd",					 7},
	{newCmd,			"newCmd",					 6},
	{new_pageCmd,			"new_pageCmd",					11},
	{new_postscript_fileCmd,        "new_postscript_fileCmd",                       22},
	{nullCmd,			"nullCmd",					 7},
	{openCmd,			"openCmd",					 7},
	{postscriptCmd,			"postscriptCmd",				13},
	{pwdCmd,			"pwdCmd",					 6},
	{queryCmd,			"queryCmd",					 8},
	{quitCmd,			"quitCmd",					 7},
	{read_colornamesCmd,		"read_colornamesCmd",				18},
	{read_columnsCmd,		"read_columnsCmd",				15},
	{read_from_filenameCmd,		"read_from_filenameCmd",			21},
	{read_gridCmd,			"read_gridCmd",					12},
	{read_imageCmd,			"read_imageCmd",				13},
	{read_image_colorscaleCmd,	"read_image_colorscaleCmd",			24},
	{read_image_grayscaleCmd,	"read_image_grayscaleCmd",			23},
	{read_image_maskCmd,		"read_image_maskCmd",				18},
	{read_image_mask_rasterfileCmd,	"read_image_mask_rasterfileCmd",		29},
	{read_image_pgmCmd,		"read_image_pgmCmd",				17},
	{read_image_rasterfileCmd,	"read_image_rasterfileCmd",			24},
	{read_synonym_or_variableCmd,	"read_synonym_or_variableCmd",			27},
	{read_lineCmd,			"read_lineCmd",					12},
	{regressCmd,			"regressCmd",					10},
	{reorder_columnsCmd,		"reorder_columnsCmd",				18},
	{rpnfunctionCmd,		"rpnfunctionCmd",				14},
	{rescaleCmd,			"rescaleCmd",					10},
// resizeCmd is written in Gri
	{rewindCmd,			"rewindCmd",					 9},
	{set_axes_styleCmd,		"set_axes_styleCmd",				17},
	{set_arrow_sizeCmd,		"set_arrow_sizeCmd",				17},
	{set_arrow_typeCmd,		"set_arrow_typeCmd",				17},
	{set_beepCmd,			"set_beepCmd",					11},
	{set_bounding_boxCmd,		"set_bounding_boxCmd",				19},
	{set_clipCmd,			"set_clipCmd",					11},
	{set_colorCmd,			"set_colorCmd",					12},
	{set_colornameCmd,		"set_colornameCmd",				16},
	{set_contour_formatCmd,		"set_contour_formatCmd",			21},
	{set_contour_labelCmd,		"set_contour_labelCmd",				20},
	{set_contour_labelsCmd,		"set_contour_labelsCmd",			21},
	{set_dashCmd,			"set_dashCmd",					11},
	{set_environment,		"set_environmentCmd",				18},
	{set_error_actionCmd,		"set_error_actionCmd",				19},
	{set_flagCmd,			"set_flagCmd",					11},
	{set_font_colorCmd,		"set_font_colorCmd",				17},
	{set_font_encodingCmd,		"set_font_encodingCmd",				20},
	{set_font_sizeCmd,		"set_font_sizeCmd",				16},
	{set_font_toCmd,		"set_font_toCmd",				14},
	{set_graylevelCmd,		"set_graylevelCmd",				16},
	{set_grid_missingCmd,		"set_grid_missingCmd",				19},
	{set_image_colorscaleCmd,	"set_image_colorscaleCmd",			23},
	{set_image_grayscale_using_hist,"set_image_grayscale_using_histogram",		35},
	{set_image_grayscaleCmd,	"set_image_grayscaleCmd",			22},
	{set_image_missingCmd,		"set_image_missingCmd",				20},
	{set_image_rangeCmd,		"set_image_rangeCmd",				18},
	{set_ignore_initial_newlineCmd,	"set_ignore_initial_newlineCmd",		29},
	{set_ignoreCmd,			"set_ignoreCmd",				13},
	{set_input_data_windowCmd,	"set_input_data_windowCmd",			24},
	{set_input_data_separatorCmd,	"set_input_data_separatorCmd",			27},
	{set_line_capCmd,		"set_line_capCmd",				15},
	{set_line_joinCmd,		"set_line_joinCmd",				16},
	{set_line_widthCmd,		"set_line_widthCmd",				17},
	{set_missing_valueCmd,		"set_missing_valueCmd",				20},
	{set_pageCmd,			"set_pageCmd",					11},
	{set_page_sizeCmd,		"set_page_sizeCmd",				16},
	{set_pathCmd,		        "set_pathCmd",				        11},
	{set_postscript_filenameCmd,	"set_postscript_filenameCmd",			26},
	{set_symbol_sizeCmd,		"set_symbol_sizeCmd",				18},
	{set_ticsCmd,			"set_ticsCmd",					11},
	{set_tic_sizeCmd,		"set_tic_sizeCmd",				15},
	{set_transparencyCmd,		"set_transparencyCmd",				19},
	{set_u_scaleCmd,		"set_u_scaleCmd",				14},
	{set_v_scaleCmd,		"set_v_scaleCmd",				14},
	{set_traceCmd,			"set_traceCmd",					12},
	{set_x_axisCmd,			"set_x_axisCmd",				13},
	{set_x_formatCmd,		"set_x_formatCmd",				15},
	{set_x_gridCmd,			"set_x_gridCmd",				13},
	{set_x_marginCmd,		"set_x_marginCmd",				15},
	{set_x_nameCmd,			"set_x_nameCmd",				13},
	{set_x_sizeCmd,			"set_x_sizeCmd",				13},
	{set_x_typeCmd,			"set_x_typeCmd",				13},
	{set_y_axisCmd,			"set_y_axisCmd",				13},
	{set_y_axis_nameCmd,		"set_y_axis_nameCmd",				18},
	{set_y_axisCmd,			"set_y_axisCmd",				13},
	{set_y_formatCmd,		"set_y_formatCmd",				15},
	{set_y_gridCmd,			"set_y_gridCmd",				13},
	{set_y_marginCmd,		"set_y_marginCmd",				15},
	{set_y_nameCmd,			"set_y_nameCmd",				13},
	{set_y_sizeCmd,			"set_y_sizeCmd",				13},
	{set_y_typeCmd,			"set_y_typeCmd",				13},
	{set_z_missingCmd,		"set_z_missingCmd",				16},
	{setCmd,			"setCmd",					 6},
	{show_allCmd,			"show_allCmd",					11},
	{show_axesCmd,			"show_axesCmd",					12},
	{show_colorCmd,			"show_colorCmd",				13},
	{show_colornamesCmd,		"show_colornamesCmd",				18},
	{show_columnsCmd,		"show_columnsCmd",				15},
	{show_expression_or_stringCmd,	"show_expression_or_stringCmd",			28},
	{show_flagsCmd,			"show_flagsCmd",				13},
	{show_gridCmd,			"show_gridCmd",					12},
	{show_imageCmd,			"show_imageCmd",				13},
	{show_licenseCmd,		"show_licenseCmd",				15},
	{show_miscCmd,			"show_miscCmd",					12},
	{show_next_lineCmd,		"show_next_lineCmd",				17},
	{show_stopwatchCmd,		"show_stopwatchCmd",				17},
	{show_synonymsCmd,		"show_synonymsCmd",				16},
	{show_timeCmd,			"show_timeCmd",					12},
	{show_tracebackCmd,		"show_tracebackCmd",				17},
	{show_variablesCmd,		"show_variablesCmd",				17},
	{smoothCmd,			"smoothCmd",					 9},
	{sourceCmd,			"sourceCmd",					 9},
	{skipCmd,			"skipCmd",					 7},
	{sleepCmd,			"sleepCmd",					 8},
	{sprintfCmd,			"sprintfCmd",					10},
	{stateCmd,			"stateCmd",					 8},
	{systemCmd,			"systemCmd",					 9},
	{superuserCmd,			"superuserCmd",					12},
        {unlinkCmd,                     "unlinkCmd",                                     9},
	{whileCmd,			"whileCmd",					 8},
	{writeCmd,			"writeCmd",					 8},
	{0,				"", 0}
};
#endif				// _tags_hh
