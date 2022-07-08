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

#if 0
static void
tepl_prefs_dialog_dispose (GObject *object)
{
	g_message ("%s()", G_STRFUNC);

	G_OBJECT_CLASS (tepl_prefs_dialog_parent_class)->dispose (object);
}
#endif

static void
tepl_prefs_dialog_finalize (GObject *object)
{
	if (singleton == TEPL_PREFS_DIALOG (object))
	{
		//g_message ("%s() singleton now NULL", G_STRFUNC);
		singleton = NULL;
	}

	G_OBJECT_CLASS (tepl_prefs_dialog_parent_class)->finalize (object);
}

static gboolean
tepl_prefs_dialog_delete_event (GtkWidget   *widget,
				GdkEventAny *event)
{
	//g_message ("%s()", G_STRFUNC);
	gtk_widget_hide (widget);
	return GDK_EVENT_STOP;
}

#if 0
static void
tepl_prefs_dialog_response (GtkDialog *gtk_dialog,
			    gint       response_id)
{
	if (GTK_DIALOG_CLASS (tepl_prefs_dialog_parent_class)->response != NULL)
	{
		GTK_DIALOG_CLASS (tepl_prefs_dialog_parent_class)->response (gtk_dialog,
									     response_id);
	}
}
#endif

static void
tepl_prefs_dialog_class_init (TeplPrefsDialogClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
	//GtkDialogClass *dialog_class = GTK_DIALOG_CLASS (klass);

	//object_class->dispose = tepl_prefs_dialog_dispose;
	object_class->finalize = tepl_prefs_dialog_finalize;
	widget_class->delete_event = tepl_prefs_dialog_delete_event;
	//dialog_class->response = tepl_prefs_dialog_response;
}

static void
tepl_prefs_dialog_init (TeplPrefsDialog *dialog)
{
	GtkWidget *content_area;

	dialog->priv = tepl_prefs_dialog_get_instance_private (dialog);

	gtk_window_set_title (GTK_WINDOW (dialog), _("Preferences"));

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
