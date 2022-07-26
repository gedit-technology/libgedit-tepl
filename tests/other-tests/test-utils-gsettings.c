/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <tepl/tepl.h>
#include <stdlib.h>

#define SETTINGS_DESKTOP_INTERFACE "org.gnome.desktop.interface"
#define SETTINGS_NOT_FOUND "one.this.not.probably"

static void
print_can_use_schema (const gchar *schema_id)
{
	gboolean can_use;

	can_use = tepl_utils_can_use_gsettings_schema (schema_id);

	g_print ("Can use '%s': %s\n",
		 schema_id,
		 can_use ? "yes" : "no");
}

int
main (void)
{
	print_can_use_schema (SETTINGS_DESKTOP_INTERFACE);
	print_can_use_schema (SETTINGS_NOT_FOUND);

	return EXIT_SUCCESS;
}
