/* SPDX-FileCopyrightText: 2020 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-test-utils.h"
#include <gtk/gtk.h>

/* Common utility functions for the unit tests. */

void
_tepl_test_utils_set_file_content (GFile       *file,
				   const gchar *content)
{
	GError *error = NULL;

	g_file_replace_contents (file,
				 content,
				 strlen (content),
				 NULL,
				 FALSE,
				 G_FILE_CREATE_REPLACE_DESTINATION,
				 NULL,
				 NULL,
				 &error);
	g_assert_no_error (error);
}

gchar *
_tepl_test_utils_get_file_content (GFile *file)
{
	GError *error = NULL;
	gchar *file_content = NULL;

	g_file_load_contents (file, NULL, &file_content, NULL, NULL, &error);
	g_assert_no_error (error);
	g_assert_true (file_content != NULL);

	return file_content;
}

struct _TeplWaitSignalData
{
	guint signal_received : 1;
	guint nested_main_loop : 1;
};

static void
wait_signal_cb (TeplWaitSignalData *data)
{
	data->signal_received = TRUE;

	if (data->nested_main_loop)
	{
		gtk_main_quit ();
	}
}

TeplWaitSignalData *
_tepl_test_utils_wait_signal_setup (GObject     *object,
				    const gchar *detailed_signal_name)
{
	TeplWaitSignalData *data;

	data = g_new0 (TeplWaitSignalData, 1);
	data->signal_received = FALSE;
	data->nested_main_loop = FALSE;

	g_signal_connect_swapped (object,
				  detailed_signal_name,
				  G_CALLBACK (wait_signal_cb),
				  data);

	return data;
}

void
_tepl_test_utils_wait_signal (TeplWaitSignalData *data)
{
	if (!data->signal_received)
	{
		data->nested_main_loop = TRUE;
		gtk_main ();
	}

	g_free (data);
}
