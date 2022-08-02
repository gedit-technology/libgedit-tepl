/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>
#include <stdlib.h>

#define SETTINGS_DESKTOP_INTERFACE "org.gnome.desktop.interface"
#define SETTINGS_NOT_FOUND "one.this.not.probably"

#define SETTINGS_KEY_MONOSPACE_FONT_NAME "monospace-font-name"
#define SETTINGS_KEY_NOT_FOUND "probably-not-found"

static void
print_can_use_schema (const gchar *schema_id)
{
	gboolean can_use;

	can_use = tepl_utils_can_use_gsettings_schema (schema_id);

	g_print ("Can use '%s': %s\n",
		 schema_id,
		 can_use ? "yes" : "no");
}

static void
print_can_use_key (const gchar *schema_id,
		   const gchar *key)
{
	GSettings *settings;
	gboolean can_use;

	settings = g_settings_new (schema_id);
	can_use = tepl_utils_can_use_gsettings_key (settings, key);

	g_print ("Can use '%s' -> key '%s': %s\n",
		 schema_id,
		 key,
		 can_use ? "yes" : "no");
}

int
main (void)
{
	print_can_use_schema (SETTINGS_DESKTOP_INTERFACE);
	print_can_use_schema (SETTINGS_NOT_FOUND);

	g_print ("\n");
	print_can_use_key (SETTINGS_DESKTOP_INTERFACE, SETTINGS_KEY_MONOSPACE_FONT_NAME);
	print_can_use_key (SETTINGS_DESKTOP_INTERFACE, SETTINGS_KEY_NOT_FOUND);

	// To test that it actually crashes:
	//print_can_use_key (SETTINGS_NOT_FOUND, SETTINGS_KEY_NOT_FOUND);

	return EXIT_SUCCESS;
}
