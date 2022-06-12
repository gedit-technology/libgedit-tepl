/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "config.h"
#include "tepl-prefs.h"
#include <glib/gi18n-lib.h>
#include "tepl-settings.h"
#include "tepl-style-scheme-chooser-widget.h"
#include "tepl-utils.h"

/**
 * SECTION:prefs
 * @Short_description: Preferences widgets to configure a text editor
 * @Title: TeplPrefs
 *
 * Functions to create GUI elements that are useful for a preferences dialog or
 * panel. In order to configure a text editor.
 */

#define SETTING_SYSTEM_FONT_KEY "monospace-font-name"

static GtkWidget *
create_checkbutton_simple (GSettings   *settings,
			   const gchar *key,
			   const gchar *mnemonic_text)
{
	GtkWidget *checkbutton;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (key != NULL, NULL);

	checkbutton = gtk_check_button_new_with_mnemonic (mnemonic_text);
	gtk_widget_show (checkbutton);

	g_settings_bind (settings, key,
			 checkbutton, "active",
			 G_SETTINGS_BIND_DEFAULT);

	return checkbutton;
}

static void
update_default_font_checkbutton_label (GtkCheckButton *checkbutton)
{
	TeplSettings *settings;
	GSettings *desktop_interface_settings;
	gchar *monospace_font_name;
	gchar *label;

	settings = tepl_settings_get_singleton ();
	desktop_interface_settings = tepl_settings_peek_desktop_interface_settings (settings);
	monospace_font_name = g_settings_get_string (desktop_interface_settings, SETTING_SYSTEM_FONT_KEY);

	label = g_strdup_printf (_("_Use the system fixed width font (%s)"), monospace_font_name);

	gtk_button_set_label (GTK_BUTTON (checkbutton), label);
	gtk_button_set_use_underline (GTK_BUTTON (checkbutton), TRUE);

	g_free (monospace_font_name);
	g_free (label);
}

static void
system_font_changed_cb (GSettings *desktop_interface_settings,
			gchar     *key,
			gpointer   user_data)
{
	GtkCheckButton *default_font_checkbutton = GTK_CHECK_BUTTON (user_data);
	update_default_font_checkbutton_label (default_font_checkbutton);
}

static GtkWidget *
create_default_font_checkbutton (void)
{
	GtkWidget *default_font_checkbutton;
	TeplSettings *settings;
	GSettings *desktop_interface_settings;

	default_font_checkbutton = gtk_check_button_new ();
	update_default_font_checkbutton_label (GTK_CHECK_BUTTON (default_font_checkbutton));

	settings = tepl_settings_get_singleton ();
	desktop_interface_settings = tepl_settings_peek_desktop_interface_settings (settings);

	g_signal_connect_object (desktop_interface_settings,
				 "changed::" SETTING_SYSTEM_FONT_KEY,
				 G_CALLBACK (system_font_changed_cb),
				 default_font_checkbutton,
				 0);

	return default_font_checkbutton;
}

/**
 * tepl_prefs_create_font_component:
 * @settings: a #GSettings.
 * @use_default_font_key: a key part of @settings to store whether to use the
 *   system font. The type of the key must be a boolean.
 * @editor_font_key: a key part of @settings to store the editor font. The type
 *   of the key must be a string.
 *
 * Returns: (transfer floating): A component for configuring the font.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_font_component (GSettings   *settings,
				  const gchar *use_default_font_key,
				  const gchar *editor_font_key)
{
	GtkWidget *default_font_checkbutton;
	GtkWidget *label;
	GtkWidget *font_button;
	GtkWidget *vgrid;
	GtkWidget *hgrid;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (use_default_font_key != NULL, NULL);
	g_return_val_if_fail (editor_font_key != NULL, NULL);

	/* Widgets */
	default_font_checkbutton = create_default_font_checkbutton ();
	label = gtk_label_new_with_mnemonic (_("Editor _font:"));
	font_button = gtk_font_button_new ();
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), font_button);

	/* GSettings */
	g_settings_bind (settings, use_default_font_key,
			 default_font_checkbutton, "active",
			 G_SETTINGS_BIND_DEFAULT);

	g_settings_bind (settings, editor_font_key,
			 font_button, "font",
			 G_SETTINGS_BIND_DEFAULT);

	/* Packing */
	vgrid = gtk_grid_new ();
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (GTK_GRID (vgrid), 6);
	gtk_container_add (GTK_CONTAINER (vgrid), default_font_checkbutton);

	hgrid = gtk_grid_new ();
	gtk_grid_set_column_spacing (GTK_GRID (hgrid), 12);
	gtk_container_add (GTK_CONTAINER (hgrid), label);
	gtk_container_add (GTK_CONTAINER (hgrid), font_button);
	gtk_container_add (GTK_CONTAINER (vgrid), hgrid);

	/* Sensitivity */
	g_object_bind_property (default_font_checkbutton, "active",
				hgrid, "sensitive",
				G_BINDING_DEFAULT |
				G_BINDING_SYNC_CREATE |
				G_BINDING_INVERT_BOOLEAN);

	return tepl_utils_get_titled_component (_("Font"), vgrid);
}

/**
 * tepl_prefs_create_color_scheme_component:
 * @settings: a #GSettings.
 * @style_scheme_key: a key part of @settings to store the style scheme ID. The
 *   type of the key must be a string.
 *
 * This function creates a component containing a #TeplStyleSchemeChooserWidget.
 *
 * Returns: (transfer floating): A component for choosing the
 *   #GtkSourceStyleScheme to use.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_color_scheme_component (GSettings   *settings,
					  const gchar *style_scheme_key)
{
	TeplStyleSchemeChooserWidget *style_scheme_chooser;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (style_scheme_key != NULL, NULL);

	style_scheme_chooser = tepl_style_scheme_chooser_widget_new ();

	g_settings_bind (settings, style_scheme_key,
			 style_scheme_chooser, "tepl-style-scheme-id",
			 G_SETTINGS_BIND_DEFAULT);

	return tepl_utils_get_titled_component (_("Color Scheme"),
						GTK_WIDGET (style_scheme_chooser));
}

/**
 * tepl_prefs_create_display_line_numbers_checkbutton:
 * @settings: a #GSettings.
 * @display_line_numbers_key: a key part of @settings. The type of the key must
 *   be a boolean.
 *
 * Returns: (transfer floating): A #GtkCheckButton intended for
 *   #GtkSourceView:show-line-numbers.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_display_line_numbers_checkbutton (GSettings   *settings,
						    const gchar *display_line_numbers_key)
{
	return create_checkbutton_simple (settings,
					  display_line_numbers_key,
					  _("_Display line numbers"));
}

/**
 * tepl_prefs_create_tab_width_spinbutton:
 * @settings: a #GSettings.
 * @tab_width_key: a key part of @settings. The type of the key must be an
 *   unsigned integer, with a range.
 *
 * Returns: (transfer floating): A widget containing a #GtkSpinButton intended
 *   for #GtkSourceView:tab-width.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_tab_width_spinbutton (GSettings   *settings,
					const gchar *tab_width_key)
{
	guint32 min = 0;
	guint32 max = 0;
	gboolean success;
	GtkWidget *label;
	GtkWidget *spinbutton;
	GtkWidget *hgrid;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (tab_width_key != NULL, NULL);

	success = tepl_settings_get_range_uint (settings, tab_width_key, &min, &max);
	g_return_val_if_fail (success, NULL);

	label = gtk_label_new_with_mnemonic (_("_Tab width:"));
	spinbutton = gtk_spin_button_new_with_range (min, max, 1.0);
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), spinbutton);

	g_settings_bind (settings, tab_width_key,
			 spinbutton, "value",
			 G_SETTINGS_BIND_DEFAULT);

	/* Packing */
	hgrid = gtk_grid_new ();
	gtk_grid_set_column_spacing (GTK_GRID (hgrid), 6);
	gtk_container_add (GTK_CONTAINER (hgrid), label);
	gtk_container_add (GTK_CONTAINER (hgrid), spinbutton);
	gtk_widget_show_all (hgrid);

	return hgrid;
}

/**
 * tepl_prefs_create_insert_spaces_component:
 * @settings: a #GSettings.
 * @insert_spaces_key: a key part of @settings. The type of the key must be a
 *   boolean.
 * @smart_backspace_key: a key part of @settings. The type of the key must be a
 *   boolean.
 *
 * Returns: (transfer floating): a component intended for
 *   #GtkSourceView:insert-spaces-instead-of-tabs and
 *   #GtkSourceView:smart-backspace.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_insert_spaces_component (GSettings   *settings,
					   const gchar *insert_spaces_key,
					   const gchar *smart_backspace_key)
{
	GtkWidget *insert_spaces_checkbutton;
	GtkWidget *smart_backspace_checkbutton;
	GtkWidget *vgrid;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (insert_spaces_key != NULL, NULL);
	g_return_val_if_fail (smart_backspace_key != NULL, NULL);

	insert_spaces_checkbutton = gtk_check_button_new_with_mnemonic (_("Insert _spaces instead of tabs"));

	smart_backspace_checkbutton = gtk_check_button_new_with_mnemonic (_("_Forget you are not using tabulations"));
	gtk_widget_set_margin_start (smart_backspace_checkbutton, 12);

	g_settings_bind (settings, insert_spaces_key,
			 insert_spaces_checkbutton, "active",
			 G_SETTINGS_BIND_DEFAULT);

	g_settings_bind (settings, smart_backspace_key,
			 smart_backspace_checkbutton, "active",
			 G_SETTINGS_BIND_DEFAULT);

	g_object_bind_property (insert_spaces_checkbutton, "active",
				smart_backspace_checkbutton, "sensitive",
				G_BINDING_DEFAULT | G_BINDING_SYNC_CREATE);

	/* Packing */
	vgrid = gtk_grid_new ();
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (GTK_GRID (vgrid), 7);
	gtk_container_add (GTK_CONTAINER (vgrid), insert_spaces_checkbutton);
	gtk_container_add (GTK_CONTAINER (vgrid), smart_backspace_checkbutton);
	gtk_widget_show_all (vgrid);

	return vgrid;
}

/**
 * tepl_prefs_create_highlighting_component:
 * @settings: a #GSettings.
 * @highlight_current_line_key: a key part of @settings. The type of the key
 *   must be a boolean.
 * @highlight_matching_brackets_key: a key part of @settings. The type of the
 *   key must be a boolean.
 *
 * Returns: (transfer floating): a component intended for
 *   #GtkSourceView:highlight-current-line and
 *   #GtkSourceBuffer:highlight-matching-brackets.
 * Since: 6.2
 */
GtkWidget *
tepl_prefs_create_highlighting_component (GSettings   *settings,
					  const gchar *highlight_current_line_key,
					  const gchar *highlight_matching_brackets_key)
{
	GtkWidget *vgrid;

	g_return_val_if_fail (G_IS_SETTINGS (settings), NULL);
	g_return_val_if_fail (highlight_current_line_key != NULL, NULL);
	g_return_val_if_fail (highlight_matching_brackets_key != NULL, NULL);

	vgrid = gtk_grid_new ();
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (GTK_GRID (vgrid), 7);

	gtk_container_add (GTK_CONTAINER (vgrid),
			   create_checkbutton_simple (settings,
						      highlight_current_line_key,
						      _("Highlight current _line")));
	gtk_container_add (GTK_CONTAINER (vgrid),
			   create_checkbutton_simple (settings,
						      highlight_matching_brackets_key,
						      _("Highlight matching _brackets")));
	gtk_widget_show_all (vgrid);
	return vgrid;
}
