/* SPDX-FileCopyrightText: 2016-2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_UTILS_H
#define TEPL_UTILS_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

/* Macros */

/**
 * TEPL_UTILS_STYLE_PROVIDER_PRIORITY_LIBRARY:
 *
 * A priority that can be used when adding a #GtkStyleProvider for
 * library-specific style information.
 *
 * Has a higher priority than %GTK_STYLE_PROVIDER_PRIORITY_SETTINGS and a lower
 * priority than %GTK_STYLE_PROVIDER_PRIORITY_APPLICATION.
 *
 * The Tepl library uses this priority for its #GtkStyleProvider's.
 *
 * Since: 6.0
 */
#define TEPL_UTILS_STYLE_PROVIDER_PRIORITY_LIBRARY (GTK_STYLE_PROVIDER_PRIORITY_APPLICATION - 1)

/* String utilities */

G_MODULE_EXPORT
gchar *		tepl_utils_str_middle_truncate			(const gchar *str,
								 guint        truncate_length);

G_MODULE_EXPORT
gchar *		tepl_utils_str_end_truncate			(const gchar *str,
								 guint        truncate_length);

G_MODULE_EXPORT
gchar *		tepl_utils_str_replace				(const gchar *string,
								 const gchar *search,
								 const gchar *replacement);

G_MODULE_EXPORT
gchar *		tepl_utils_markup_escape_text			(const gchar *src);

G_GNUC_INTERNAL
gchar *		_tepl_utils_statusbar_add_padding_to_text	(const gchar *text);

/* File utilities */

G_MODULE_EXPORT
gchar *		tepl_utils_get_file_extension			(const gchar *filename);

G_MODULE_EXPORT
gchar *		tepl_utils_get_file_shortname			(const gchar *filename);

G_GNUC_INTERNAL
gchar *		_tepl_utils_replace_home_dir_with_tilde_with_param
								(const gchar *filename,
								 const gchar *home_dir);

G_MODULE_EXPORT
gchar *		tepl_utils_replace_home_dir_with_tilde		(const gchar *filename);

G_MODULE_EXPORT
gboolean	tepl_utils_decode_uri				(const gchar  *uri,
								 gchar       **scheme,
								 gchar       **user,
								 gchar       **host,
								 gchar       **port,
								 gchar       **path);

G_GNUC_INTERNAL
gchar *		_tepl_utils_get_fallback_basename_for_display	(GFile *location);

G_MODULE_EXPORT
gboolean	tepl_utils_create_parent_directories		(GFile         *file,
								 GCancellable  *cancellable,
								 GError       **error);

G_MODULE_EXPORT
void		tepl_utils_file_query_exists_async		(GFile               *file,
								 GCancellable        *cancellable,
								 GAsyncReadyCallback  callback,
								 gpointer             user_data);

G_MODULE_EXPORT
gboolean	tepl_utils_file_query_exists_finish		(GFile        *file,
								 GAsyncResult *result);

/* Widget utilities */

G_MODULE_EXPORT
GtkWidget *	tepl_utils_create_close_button			(void);

G_GNUC_INTERNAL
void		_tepl_utils_associate_secondary_window		(GtkWindow *secondary_window,
								 GtkWidget *main_window_widget);

G_MODULE_EXPORT
void		tepl_utils_show_warning_dialog			(GtkWindow   *parent,
								 const gchar *format,
								 ...) G_GNUC_PRINTF(2, 3);

G_MODULE_EXPORT
void		tepl_utils_list_box_clear			(GtkListBox *list_box);

G_MODULE_EXPORT
void		tepl_utils_list_box_setup_scrolling		(GtkListBox        *list_box,
								 GtkScrolledWindow *scrolled_window);

G_MODULE_EXPORT
void		tepl_utils_list_box_scroll_to_row		(GtkListBox    *list_box,
								 GtkListBoxRow *row);

G_MODULE_EXPORT
void		tepl_utils_list_box_scroll_to_selected_row	(GtkListBox *list_box);

G_MODULE_EXPORT
GtkListBoxRow *	tepl_utils_list_box_get_row_at_index_with_filter (GtkListBox           *list_box,
								  gint                  index,
								  GtkListBoxFilterFunc  filter_func,
								  gpointer              user_data);

G_MODULE_EXPORT
GtkListBoxRow **tepl_utils_list_box_get_filtered_children	(GtkListBox           *list_box,
								 GtkListBoxFilterFunc  filter_func,
								 gpointer              user_data,
								 gint                 *n_filtered_children);

G_MODULE_EXPORT
void		tepl_utils_override_font_description		(GtkWidget                  *widget,
								 const PangoFontDescription *font_desc);

G_MODULE_EXPORT
void		tepl_utils_override_font_string			(GtkWidget   *widget,
								 const gchar *font_str);

G_MODULE_EXPORT
GtkWidget *	tepl_utils_get_titled_component			(const gchar *title,
								 GtkWidget   *component);

G_MODULE_EXPORT
void		tepl_utils_setup_statusbar			(GtkStatusbar *statusbar);

/* Other */

G_MODULE_EXPORT
gboolean	tepl_utils_binding_transform_func_smart_bool	(GBinding     *binding,
								 const GValue *from_value,
								 GValue       *to_value,
								 gpointer      user_data);

G_MODULE_EXPORT
gboolean	tepl_utils_can_use_gsettings_schema		(const gchar *schema_id);

G_MODULE_EXPORT
gboolean	tepl_utils_can_use_gsettings_key		(GSettings   *settings,
								 const gchar *key);

G_END_DECLS

#endif /* TEPL_UTILS_H */
