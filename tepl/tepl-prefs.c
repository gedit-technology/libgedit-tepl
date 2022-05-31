/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "config.h"
#include "tepl-prefs.h"
#include <glib/gi18n-lib.h>

/**
 * SECTION:prefs
 * @Short_description: Preferences widgets to configure a text editor
 * @Title: TeplPrefs
 *
 * Functions to create GUI elements that are useful for a preferences dialog or
 * panel. In order to configure a text editor.
 */

static void
update_default_font_checkbutton_label (GtkCheckButton *checkbutton)
{
	gchar *label;

	label = g_strdup_printf (_("_Use the system fixed width font"));

	gtk_button_set_label (GTK_BUTTON (checkbutton), label);
	gtk_button_set_use_underline (GTK_BUTTON (checkbutton), TRUE);

	g_free (label);
}

static GtkWidget *
create_default_font_checkbutton (void)
{
	GtkWidget *default_font_checkbutton;

	default_font_checkbutton = gtk_check_button_new ();
	update_default_font_checkbutton_label (GTK_CHECK_BUTTON (default_font_checkbutton));

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

	gtk_widget_show_all (vgrid);
	return vgrid;
}
