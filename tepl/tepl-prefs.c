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

/**
 * tepl_prefs_create_font_component:
 * @settings: a #GSettings.
 * @editor_font_key: a key part of @settings to store the editor font. The type
 *   of the key must be a string.
 *
 * Returns: (transfer floating): A component for configuring the font.
 * Since: 300.0
 */
GtkWidget *
tepl_prefs_create_font_component (GSettings   *settings,
				  const gchar *editor_font_key)
{
	GtkWidget *label;
	GtkWidget *font_button;
	GtkWidget *hgrid;

	/* Widgets */
	label = gtk_label_new_with_mnemonic (_("Editor _font:"));
	font_button = gtk_font_button_new ();
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), font_button);

	/* GSettings */
	g_settings_bind (settings, editor_font_key,
			 font_button, "font",
			 G_SETTINGS_BIND_DEFAULT);

	/* Packing */
	hgrid = gtk_grid_new ();
	gtk_grid_set_column_spacing (GTK_GRID (hgrid), 12);
	gtk_container_add (GTK_CONTAINER (hgrid), label);
	gtk_container_add (GTK_CONTAINER (hgrid), font_button);
	gtk_widget_show_all (hgrid);

	return hgrid;
}
