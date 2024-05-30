/* SPDX-FileCopyrightText: 2022-2024 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PREFS_H
#define TEPL_PREFS_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

G_MODULE_EXPORT
GtkWidget *	tepl_prefs_create_font_component			(GSettings   *settings,
									 const gchar *use_default_font_key,
									 const gchar *editor_font_key);

G_MODULE_EXPORT
GtkWidget *	tepl_prefs_create_display_line_numbers_checkbutton	(GSettings   *settings,
									 const gchar *display_line_numbers_key);

G_MODULE_EXPORT
GtkWidget *	tepl_prefs_create_tab_width_spinbutton			(GSettings   *settings,
									 const gchar *tab_width_key);

G_MODULE_EXPORT
GtkWidget *	tepl_prefs_create_insert_spaces_component		(GSettings   *settings,
									 const gchar *insert_spaces_key,
									 const gchar *smart_backspace_key);

G_MODULE_EXPORT
GtkWidget *	tepl_prefs_create_highlighting_component		(GSettings   *settings,
									 const gchar *highlight_current_line_key,
									 const gchar *highlight_matching_brackets_key);

G_MODULE_EXPORT
GtkWidget *	tepl_prefs_create_files_component			(GSettings   *settings,
									 const gchar *create_backup_copy_key,
									 const gchar *auto_save_key,
									 const gchar *auto_save_interval);

G_MODULE_EXPORT
GtkWidget *	tepl_prefs_create_theme_variant_combo_box		(GSettings   *settings,
									 const gchar *theme_variant_key);

G_MODULE_EXPORT
GtkWidget *	tepl_prefs_create_right_margin_component		(GSettings   *settings,
									 const gchar *display_right_margin_key,
									 const gchar *right_margin_position_key);

G_MODULE_EXPORT
GtkWidget *	tepl_prefs_create_display_statusbar_checkbutton		(GSettings   *settings,
									 const gchar *display_statusbar_key);

G_MODULE_EXPORT
GtkWidget *	tepl_prefs_create_display_grid_checkbutton		(GSettings   *settings,
									 const gchar *background_pattern_key);

G_END_DECLS

#endif /* TEPL_PREFS_H */
