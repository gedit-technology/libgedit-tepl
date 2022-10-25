/* SPDX-FileCopyrightText: 2005 - Paolo Maggi
 * SPDX-FileCopyrightText: 2020-2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "config.h"
#include "tepl-io-error-info-bars.h"
#include <glib/gi18n-lib.h>
#include "tepl-utils.h"

/**
 * SECTION:io-error-info-bars
 * @Short_description: Verbose error reporting for file I/O operations
 * @Title: IO error info bars
 *
 * Verbose error reporting for file I/O operations.
 */

static gchar *
get_filename_for_display (GFile *location)
{
	gchar *parse_name;
	gchar *filename_for_display;

	parse_name = g_file_get_parse_name (location);
	filename_for_display = tepl_utils_replace_home_dir_with_tilde (parse_name);

	g_free (parse_name);
	return filename_for_display;
}

/**
 * tepl_io_error_info_bar_file_already_open:
 * @location: the #GFile already open in another window.
 *
 * Creates a warning about @location being already open in another window,
 * offering two possible actions:
 * - Edit anyway: %GTK_RESPONSE_YES.
 * - Don't edit: %GTK_RESPONSE_CANCEL.
 *
 * Returns: (transfer floating): the newly created #TeplInfoBar.
 * Since: 5.0
 */
TeplInfoBar *
tepl_io_error_info_bar_file_already_open (GFile *location)
{
	TeplInfoBar *info_bar;
	gchar *filename;
	gchar *filename_in_italic;
	gchar *app_name_escaped;
	gchar *primary_text;
	gchar *primary_text_in_bold;
	GtkLabel *primary_label;
	gchar *secondary_msg;

	g_return_val_if_fail (G_IS_FILE (location), NULL);
	g_return_val_if_fail (g_get_application_name () != NULL, NULL);

	info_bar = tepl_info_bar_new ();
	gtk_info_bar_set_message_type (GTK_INFO_BAR (info_bar), GTK_MESSAGE_WARNING);
	tepl_info_bar_set_icon_from_message_type (info_bar, TRUE);

	/* Note that below the markup plus translatable strings is not handled
	 * perfectly. But "perfect is the enemy of good". This should be:
	 * "foo%sbar" with foo and bar both escaped.
	 */

	filename = get_filename_for_display (location);
	filename_in_italic = g_markup_printf_escaped ("<i>%s</i>", filename);

	app_name_escaped = tepl_utils_markup_escape_text (g_get_application_name ());

	/* Translators: the first %s is a filename and the second %s is an
	 * application name.
	 */
	primary_text = g_strdup_printf (_("This file (%s) is already open in another %s window."),
					filename_in_italic,
					app_name_escaped);
	primary_text_in_bold = g_strdup_printf ("<b>%s</b>", primary_text);

	primary_label = tepl_info_bar_create_label ();
	gtk_label_set_markup (primary_label, primary_text_in_bold);
	gtk_widget_show (GTK_WIDGET (primary_label));

	tepl_info_bar_add_content_widget (info_bar,
					  GTK_WIDGET (primary_label),
					  TEPL_INFO_BAR_LOCATION_ALONGSIDE_ICON);

	secondary_msg = g_strdup_printf (_("%s opened this instance of the file in a non-editable way. "
					   "Do you want to edit it anyway?"),
					 g_get_application_name ());
	tepl_info_bar_add_secondary_message (info_bar, secondary_msg);

	gtk_info_bar_add_button (GTK_INFO_BAR (info_bar),
				 _("_Edit Anyway"),
				 GTK_RESPONSE_YES);

	gtk_info_bar_add_button (GTK_INFO_BAR (info_bar),
				 _("_Keep Read-Only"),
				 GTK_RESPONSE_CANCEL);

	g_free (filename);
	g_free (filename_in_italic);
	g_free (app_name_escaped);
	g_free (primary_text);
	g_free (primary_text_in_bold);
	g_free (secondary_msg);
	return info_bar;
}

/**
 * tepl_io_error_info_bar_cant_create_backup:
 * @location: the #GFile for which the backup failed to be created.
 * @error: must be a %G_IO_ERROR_CANT_CREATE_BACKUP.
 *
 * When a %G_IO_ERROR_CANT_CREATE_BACKUP error occurs while saving @location,
 * offer two possible actions:
 * - Save anyway: %GTK_RESPONSE_YES.
 * - Don't save: %GTK_RESPONSE_CANCEL.
 *
 * Returns: (transfer floating): the newly created #TeplInfoBar.
 * Since: 5.0
 */
TeplInfoBar *
tepl_io_error_info_bar_cant_create_backup (GFile        *location,
					   const GError *error)
{
	TeplInfoBar *info_bar;
	gchar *filename;
	gchar *primary_msg;
	const gchar *secondary_msg;

	g_return_val_if_fail (G_IS_FILE (location), NULL);
	g_return_val_if_fail (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANT_CREATE_BACKUP), NULL);

	info_bar = tepl_info_bar_new ();
	gtk_info_bar_set_message_type (GTK_INFO_BAR (info_bar), GTK_MESSAGE_WARNING);
	tepl_info_bar_set_icon_from_message_type (info_bar, TRUE);

	filename = get_filename_for_display (location);
	primary_msg = g_strdup_printf (_("Could not create a backup file while saving “%s”."), filename);
	tepl_info_bar_add_primary_message (info_bar, primary_msg);
	g_free (filename);
	g_free (primary_msg);

	secondary_msg = _("Could not back up the old copy of the file before saving the new one. "
			  "You can ignore this warning and save the file anyway, but if an error "
			  "occurs while saving, you could lose the old copy of the file. Save anyway?");
	tepl_info_bar_add_secondary_message (info_bar, secondary_msg);

	if (error->message != NULL)
	{
		gchar *error_msg;

		error_msg = g_strdup_printf (_("Error message: %s"), error->message);
		tepl_info_bar_add_secondary_message (info_bar, error_msg);
		g_free (error_msg);
	}

	gtk_info_bar_add_button (GTK_INFO_BAR (info_bar),
				 _("_Save Anyway"),
				 GTK_RESPONSE_YES);

	gtk_info_bar_add_button (GTK_INFO_BAR (info_bar),
				 _("_Don’t Save"),
				 GTK_RESPONSE_CANCEL);

	return info_bar;
}

/**
 * tepl_io_error_info_bar_externally_modified:
 * @location: the #GFile for which there has been an external modification.
 * @document_modified: whether the document (e.g. the #GtkTextBuffer) has
 *   unsaved modifications.
 *
 * Creates a warning about @location having changed on disk. The possible
 * actions:
 * - Depending on @document_modified, "Reload" or "Drop changes and reload":
 *   %GTK_RESPONSE_OK.
 * - A close button as added with gtk_info_bar_set_show_close_button().
 *
 * Returns: (transfer floating): the newly created #TeplInfoBar.
 * Since: 5.0
 */
TeplInfoBar *
tepl_io_error_info_bar_externally_modified (GFile    *location,
					    gboolean  document_modified)
{
	TeplInfoBar *info_bar;
	gchar *uri;
	gchar *primary_msg;
	const gchar *button_text;

	g_return_val_if_fail (G_IS_FILE (location), NULL);

	info_bar = tepl_info_bar_new ();

	uri = g_file_get_parse_name (location);
	primary_msg = g_strdup_printf (_("The file “%s” changed on disk."), uri);
	tepl_info_bar_add_primary_message (info_bar, primary_msg);
	g_free (uri);
	g_free (primary_msg);

	button_text = document_modified ? _("Drop Changes and _Reload") : _("_Reload");
	gtk_info_bar_add_button (GTK_INFO_BAR (info_bar),
				 button_text,
				 GTK_RESPONSE_OK);

	gtk_info_bar_set_show_close_button (GTK_INFO_BAR (info_bar), TRUE);
	gtk_info_bar_set_message_type (GTK_INFO_BAR (info_bar), GTK_MESSAGE_WARNING);

	return info_bar;
}

/**
 * tepl_io_error_info_bar_invalid_characters:
 * @location: where to save the document.
 *
 * For file saving, creates a warning about invalid characters that can corrupt
 * the file. Possible actions:
 * - Save anyway: %GTK_RESPONSE_YES.
 * - Don't save: %GTK_RESPONSE_CANCEL.
 *
 * Returns: (transfer floating): the newly created #TeplInfoBar.
 * Since: 5.0
 */
TeplInfoBar *
tepl_io_error_info_bar_invalid_characters (GFile *location)
{
	TeplInfoBar *info_bar;
	gchar *uri;
	gchar *primary_msg;
	const gchar *secondary_msg;

	g_return_val_if_fail (G_IS_FILE (location), NULL);

	info_bar = tepl_info_bar_new ();

	gtk_info_bar_add_button (GTK_INFO_BAR (info_bar),
				 _("S_ave Anyway"),
				 GTK_RESPONSE_YES);

	gtk_info_bar_add_button (GTK_INFO_BAR (info_bar),
				 _("_Don’t Save"),
				 GTK_RESPONSE_CANCEL);

	gtk_info_bar_set_message_type (GTK_INFO_BAR (info_bar), GTK_MESSAGE_WARNING);

	uri = g_file_get_parse_name (location);
	primary_msg = g_strdup_printf (_("Some invalid characters have been detected while saving “%s”."), uri);
	tepl_info_bar_add_primary_message (info_bar, primary_msg);
	g_free (uri);
	g_free (primary_msg);

	secondary_msg = _("If you continue saving this file you can corrupt the document. "
			  "Save anyway?");
	tepl_info_bar_add_secondary_message (info_bar, secondary_msg);

	return info_bar;
}
