/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-prefs-dialog.h"

/**
 * SECTION:prefs-dialog
 * @Short_description: Preferences dialog
 * @Title: TeplPrefsDialog
 *
 * #TeplPrefsDialog is a subclass of #GtkDialog for configuring the preferences
 * of an application.
 */

struct _TeplPrefsDialogPrivate
{
	gint something;
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplPrefsDialog, tepl_prefs_dialog, GTK_TYPE_DIALOG)

static void
tepl_prefs_dialog_finalize (GObject *object)
{

	G_OBJECT_CLASS (tepl_prefs_dialog_parent_class)->finalize (object);
}

static void
tepl_prefs_dialog_class_init (TeplPrefsDialogClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = tepl_prefs_dialog_finalize;
}

static void
tepl_prefs_dialog_init (TeplPrefsDialog *dialog)
{
	dialog->priv = tepl_prefs_dialog_get_instance_private (dialog);
}
