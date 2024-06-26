/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "config.h"
#include "tepl-prefs-dialog.h"
#include <glib/gi18n-lib.h>
#include "tepl-abstract-factory.h"

/**
 * SECTION:prefs-dialog
 * @Short_description: Preferences dialog
 * @Title: TeplPrefsDialog
 *
 * #TeplPrefsDialog is a subclass of #GtkDialog for configuring the preferences
 * of an application.
 *
 * tepl_abstract_factory_fill_prefs_dialog() is called by #TeplPrefsDialog
 * during its construction.
 */

struct _TeplPrefsDialogPrivate
{
	gint something;
};

static TeplPrefsDialog *singleton = NULL;

G_DEFINE_TYPE_WITH_PRIVATE (TeplPrefsDialog, tepl_prefs_dialog, GTK_TYPE_DIALOG)

static void
tepl_prefs_dialog_finalize (GObject *object)
{
	if (singleton == TEPL_PREFS_DIALOG (object))
	{
		singleton = NULL;
	}

	G_OBJECT_CLASS (tepl_prefs_dialog_parent_class)->finalize (object);
}

static gboolean
tepl_prefs_dialog_delete_event (GtkWidget   *widget,
				GdkEventAny *event)
{
	gtk_widget_hide (widget);
	return GDK_EVENT_STOP;
}

static void
tepl_prefs_dialog_class_init (TeplPrefsDialogClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	object_class->finalize = tepl_prefs_dialog_finalize;
	widget_class->delete_event = tepl_prefs_dialog_delete_event;
}

static void
tepl_prefs_dialog_init (TeplPrefsDialog *dialog)
{
	GtkWidget *content_area;

	dialog->priv = tepl_prefs_dialog_get_instance_private (dialog);

	gtk_window_set_title (GTK_WINDOW (dialog), _("Preferences"));

	/* Ideally this should be hide-when-parent-destroyed, but it isn't
	 * implemented by GTK. A utils function could be implemented, but it's
	 * more code (so more chances to get it wrong), and it doesn't really
	 * worth it.
	 *
	 * So, keep things simple.
	 *
	 * When the app has several main windows opened, but on different
	 * workspaces, it would be strange to keep the TeplPrefsDialog visible
	 * when its parent is closed. So it needs to be hidden or destroyed.
	 * Hidden is slightly better because a preferences dialog usually has a
	 * GtkNotebook inside (or something similar), so it's better to keep the
	 * same tab when showing again the preferences dialog.
	 */
	gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);

	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

	/* When packing a GtkNotebook inside the content area, we don't want any
	 * margin around the GtkNotebook. More generally, we can also control
	 * the margins that we want with what we pack into the content area.
	 */
	gtk_container_set_border_width (GTK_CONTAINER (content_area), 0);
}

/**
 * tepl_prefs_dialog_get_singleton:
 *
 * Returns: (transfer none): the #TeplPrefsDialog singleton instance.
 * Since: 6.2
 */
TeplPrefsDialog *
tepl_prefs_dialog_get_singleton (void)
{
	if (singleton == NULL)
	{
		TeplAbstractFactory *factory;

		singleton = g_object_new (TEPL_TYPE_PREFS_DIALOG,
					  "use-header-bar", TRUE,
					  NULL);

		factory = tepl_abstract_factory_get_singleton ();
		tepl_abstract_factory_fill_prefs_dialog (factory, singleton);
	}

	return singleton;
}

void
_tepl_prefs_dialog_unref_singleton (void)
{
	if (singleton != NULL)
	{
		gtk_widget_destroy (GTK_WIDGET (singleton));
	}
}

/**
 * tepl_prefs_dialog_show_for_parent:
 * @dialog: a #TeplPrefsDialog.
 * @parent_window: the parent #GtkWindow.
 *
 * This function sets the #GtkWindow:transient-for property and presents
 * @dialog.
 *
 * Since: 6.2
 */
void
tepl_prefs_dialog_show_for_parent (TeplPrefsDialog *dialog,
				   GtkWindow       *parent_window)
{
	g_return_if_fail (TEPL_IS_PREFS_DIALOG (dialog));
	g_return_if_fail (GTK_IS_WINDOW (parent_window));

	gtk_window_set_transient_for (GTK_WINDOW (dialog), parent_window);
	gtk_window_present (GTK_WINDOW (dialog));
}
