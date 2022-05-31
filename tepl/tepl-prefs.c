/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "config.h"
#include "tepl-prefs.h"
#include <glib/gi18n-lib.h>
#include "tepl-settings.h"
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
 * Since: 300.0
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
