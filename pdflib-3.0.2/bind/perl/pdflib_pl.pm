package pdflib_pl;
require Exporter;
require DynaLoader;
$VERSION=3.02;
@ISA = qw(Exporter DynaLoader);
package pdflibc;
bootstrap pdflib_pl;
var_pdflib_init();
@EXPORT = qw( );

# ---------- BASE METHODS -------------

package pdflib_pl;

sub TIEHASH {
    my ($classname,$obj) = @_;
    return bless $obj, $classname;
}

sub CLEAR { }

sub this {
    my $ptr = shift;
    return tied(%$ptr);
}


# ------- FUNCTION WRAPPERS --------

package pdflib_pl;

*PDF_new = *pdflibc::PDF_new;
*PDF_delete = *pdflibc::PDF_delete;
*PDF_open_file = *pdflibc::PDF_open_file;
*PDF_close = *pdflibc::PDF_close;
*PDF_get_buffer = *pdflibc::PDF_get_buffer;
*PDF_begin_page = *pdflibc::PDF_begin_page;
*PDF_end_page = *pdflibc::PDF_end_page;
*PDF_set_parameter = *pdflibc::PDF_set_parameter;
*PDF_get_parameter = *pdflibc::PDF_get_parameter;
*PDF_set_value = *pdflibc::PDF_set_value;
*PDF_get_value = *pdflibc::PDF_get_value;
*PDF_findfont = *pdflibc::PDF_findfont;
*PDF_setfont = *pdflibc::PDF_setfont;
*PDF_show = *pdflibc::PDF_show;
*PDF_show_xy = *pdflibc::PDF_show_xy;
*PDF_continue_text = *pdflibc::PDF_continue_text;
*PDF_show_boxed = *pdflibc::PDF_show_boxed;
*PDF_set_text_pos = *pdflibc::PDF_set_text_pos;
*PDF_set_text_matrix = *pdflibc::PDF_set_text_matrix;
*PDF_stringwidth = *pdflibc::PDF_stringwidth;
*PDF_setdash = *pdflibc::PDF_setdash;
*PDF_setpolydash = *pdflibc::PDF_setpolydash;
*PDF_setflat = *pdflibc::PDF_setflat;
*PDF_setlinejoin = *pdflibc::PDF_setlinejoin;
*PDF_setlinecap = *pdflibc::PDF_setlinecap;
*PDF_setmiterlimit = *pdflibc::PDF_setmiterlimit;
*PDF_setlinewidth = *pdflibc::PDF_setlinewidth;
*PDF_save = *pdflibc::PDF_save;
*PDF_restore = *pdflibc::PDF_restore;
*PDF_translate = *pdflibc::PDF_translate;
*PDF_scale = *pdflibc::PDF_scale;
*PDF_rotate = *pdflibc::PDF_rotate;
*PDF_skew = *pdflibc::PDF_skew;
*PDF_concat = *pdflibc::PDF_concat;
*PDF_moveto = *pdflibc::PDF_moveto;
*PDF_lineto = *pdflibc::PDF_lineto;
*PDF_curveto = *pdflibc::PDF_curveto;
*PDF_circle = *pdflibc::PDF_circle;
*PDF_arc = *pdflibc::PDF_arc;
*PDF_rect = *pdflibc::PDF_rect;
*PDF_closepath = *pdflibc::PDF_closepath;
*PDF_stroke = *pdflibc::PDF_stroke;
*PDF_closepath_stroke = *pdflibc::PDF_closepath_stroke;
*PDF_fill = *pdflibc::PDF_fill;
*PDF_fill_stroke = *pdflibc::PDF_fill_stroke;
*PDF_closepath_fill_stroke = *pdflibc::PDF_closepath_fill_stroke;
*PDF_endpath = *pdflibc::PDF_endpath;
*PDF_clip = *pdflibc::PDF_clip;
*PDF_setgray_fill = *pdflibc::PDF_setgray_fill;
*PDF_setgray_stroke = *pdflibc::PDF_setgray_stroke;
*PDF_setgray = *pdflibc::PDF_setgray;
*PDF_setrgbcolor_fill = *pdflibc::PDF_setrgbcolor_fill;
*PDF_setrgbcolor_stroke = *pdflibc::PDF_setrgbcolor_stroke;
*PDF_setrgbcolor = *pdflibc::PDF_setrgbcolor;
*PDF_place_image = *pdflibc::PDF_place_image;
*PDF_open_image = *pdflibc::PDF_open_image;
*PDF_open_image_file = *pdflibc::PDF_open_image_file;
*PDF_close_image = *pdflibc::PDF_close_image;
*PDF_open_CCITT = *pdflibc::PDF_open_CCITT;
*PDF_add_bookmark = *pdflibc::PDF_add_bookmark;
*PDF_set_info = *pdflibc::PDF_set_info;
*PDF_attach_file = *pdflibc::PDF_attach_file;
*PDF_add_note = *pdflibc::PDF_add_note;
*PDF_add_pdflink = *pdflibc::PDF_add_pdflink;
*PDF_add_launchlink = *pdflibc::PDF_add_launchlink;
*PDF_add_locallink = *pdflibc::PDF_add_locallink;
*PDF_add_weblink = *pdflibc::PDF_add_weblink;
*PDF_set_border_style = *pdflibc::PDF_set_border_style;
*PDF_set_border_color = *pdflibc::PDF_set_border_color;
*PDF_set_border_dash = *pdflibc::PDF_set_border_dash;

@EXPORT = qw( PDF_add_bookmark PDF_add_launchlink PDF_add_locallink PDF_add_note PDF_add_pdflink PDF_add_weblink PDF_arc PDF_attach_file PDF_begin_page PDF_boot PDF_circle PDF_clip PDF_close PDF_close_image PDF_closepath PDF_closepath_fill_stroke PDF_closepath_stroke PDF_concat PDF_continue_text PDF_curveto PDF_delete PDF_end_page PDF_endpath PDF_fill PDF_fill_stroke PDF_findfont PDF_get_buffer PDF_get_parameter PDF_get_value PDF_lineto PDF_moveto PDF_new PDF_open_CCITT PDF_open_file PDF_open_image PDF_open_image_file PDF_place_image PDF_rect PDF_restore PDF_skew PDF_rotate PDF_save PDF_scale PDF_set_border_color PDF_set_border_dash PDF_set_border_style PDF_set_info PDF_set_parameter PDF_set_text_matrix PDF_set_text_pos PDF_set_value PDF_setdash PDF_setflat PDF_setfont PDF_setgray PDF_setgray_fill PDF_setgray_stroke PDF_setlinecap PDF_setlinejoin PDF_setlinewidth PDF_setmiterlimit PDF_setpolydash PDF_setrgbcolor PDF_setrgbcolor_fill PDF_setrgbcolor_stroke PDF_show PDF_show_boxed PDF_show_xy PDF_shutdown PDF_stringwidth PDF_stroke PDF_translate );

# ------- VARIABLE STUBS --------

package pdflib_pl;

1;
