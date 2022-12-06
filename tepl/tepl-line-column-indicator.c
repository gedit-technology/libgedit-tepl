/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "config.h"
#include "tepl-line-column-indicator.h"
#include <glib/gi18n-lib.h>
#include "tepl-signal-group.h"

/**
 * SECTION:line-column-indicator
 * @Short_description: Indicates the cursor position
 * @Title: TeplLineColumnIndicator
 *
 * #TeplLineColumnIndicator indicates the cursor position by line and column
 * numbers. For example: "Ln 2, Col 3".
 *
 * The usual case is to pack this widget in a #GtkStatusbar.
 */

struct _TeplLineColumnIndicatorPrivate
{
	GtkLabel *label;

	TeplTabGroup *tab_group;

	TeplSignalGroup *view_signal_group;
	TeplSignalGroup *buffer_signal_group;
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplLineColumnIndicator, tepl_line_column_indicator, GTK_TYPE_BIN)

static void
set_values (TeplLineColumnIndicator *indicator,
	    gint                     line,
	    gint                     column)
{
	gchar *text;

	g_return_if_fail (line >= 1);
	g_return_if_fail (column >= 1);

	/* Translators: "Ln" is an abbreviation for "Line", Col is an
	 * abbreviation for "Column". Please, use abbreviations if possible.
	 */
	text = g_strdup_printf (_("Ln %d, Col %d"), line, column);
	gtk_label_set_text (indicator->priv->label, text);
	g_free (text);
}

static void
tepl_line_column_indicator_dispose (GObject *object)
{
	TeplLineColumnIndicator *indicator = TEPL_LINE_COLUMN_INDICATOR (object);

	g_clear_object (&indicator->priv->tab_group);

	tepl_signal_group_clear (&indicator->priv->view_signal_group);
	tepl_signal_group_clear (&indicator->priv->buffer_signal_group);

	/* In case a public function is called after a first dispose. */
	indicator->priv->label = NULL;

	G_OBJECT_CLASS (tepl_line_column_indicator_parent_class)->dispose (object);
}

static void
tepl_line_column_indicator_class_init (TeplLineColumnIndicatorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_line_column_indicator_dispose;
}

static void
tepl_line_column_indicator_init (TeplLineColumnIndicator *indicator)
{
	indicator->priv = tepl_line_column_indicator_get_instance_private (indicator);

	indicator->priv->label = GTK_LABEL (gtk_label_new (NULL));
	gtk_widget_show (GTK_WIDGET (indicator->priv->label));

	gtk_container_add (GTK_CONTAINER (indicator),
			   GTK_WIDGET (indicator->priv->label));
}

/**
 * tepl_line_column_indicator_new:
 *
 * Returns: (transfer floating): a new #TeplLineColumnIndicator widget.
 * Since: 6.4
 */
TeplLineColumnIndicator *
tepl_line_column_indicator_new (void)
{
	return g_object_new (TEPL_TYPE_LINE_COLUMN_INDICATOR, NULL);
}

static void
update_cursor_position (TeplLineColumnIndicator *indicator)
{
	TeplView *active_view;
	GtkTextBuffer *active_buffer;
	GtkTextIter iter;
	gint line;
	gint column;

	active_view = tepl_tab_group_get_active_view (indicator->priv->tab_group);
	if (active_view == NULL)
	{
		gtk_widget_hide (GTK_WIDGET (indicator->priv->label));
		return;
	}

	active_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (active_view));
	gtk_text_buffer_get_iter_at_mark (active_buffer,
					  &iter,
					  gtk_text_buffer_get_insert (active_buffer));

	line = gtk_text_iter_get_line (&iter);
	column = gtk_source_view_get_visual_column (GTK_SOURCE_VIEW (active_view), &iter);

	set_values (indicator, line + 1, column + 1);
	gtk_widget_show (GTK_WIDGET (indicator->priv->label));
}

static void
cursor_moved_cb (TeplBuffer              *buffer,
		 TeplLineColumnIndicator *indicator)
{
	update_cursor_position (indicator);
}

static void
connect_to_buffer (TeplLineColumnIndicator *indicator,
		   TeplBuffer              *buffer)
{
	tepl_signal_group_clear (&indicator->priv->buffer_signal_group);

	if (buffer == NULL)
	{
		return;
	}

	indicator->priv->buffer_signal_group = tepl_signal_group_new (G_OBJECT (buffer));

	tepl_signal_group_add (indicator->priv->buffer_signal_group,
			       g_signal_connect (buffer,
						 "tepl-cursor-moved",
						 G_CALLBACK (cursor_moved_cb),
						 indicator));
}

static void
buffer_notify_cb (TeplView                *view,
		  GParamSpec              *pspec,
		  TeplLineColumnIndicator *indicator)
{
	TeplBuffer *buffer;

	buffer = TEPL_BUFFER (gtk_text_view_get_buffer (GTK_TEXT_VIEW (view)));
	connect_to_buffer (indicator, buffer);

	update_cursor_position (indicator);
}

static void
tab_width_notify_cb (TeplView                *view,
		     GParamSpec              *pspec,
		     TeplLineColumnIndicator *indicator)
{
	/* gtk_source_view_get_visual_column() depends on tab-width. */
	update_cursor_position (indicator);
}

static void
connect_to_view (TeplLineColumnIndicator *indicator,
		 TeplView                *view)
{
	tepl_signal_group_clear (&indicator->priv->view_signal_group);

	if (view == NULL)
	{
		return;
	}

	indicator->priv->view_signal_group = tepl_signal_group_new (G_OBJECT (view));

	tepl_signal_group_add (indicator->priv->view_signal_group,
			       g_signal_connect (view,
						 "notify::buffer",
						 G_CALLBACK (buffer_notify_cb),
						 indicator));

	tepl_signal_group_add (indicator->priv->view_signal_group,
			       g_signal_connect (view,
						 "notify::tab-width",
						 G_CALLBACK (tab_width_notify_cb),
						 indicator));
}

static void
active_view_changed (TeplLineColumnIndicator *indicator)
{
	TeplView *active_view;
	TeplBuffer *active_buffer;

	active_view = tepl_tab_group_get_active_view (indicator->priv->tab_group);
	active_buffer = tepl_tab_group_get_active_buffer (indicator->priv->tab_group);

	connect_to_view (indicator, active_view);
	connect_to_buffer (indicator, active_buffer);

	update_cursor_position (indicator);
}

static void
active_view_notify_cb (TeplTabGroup            *tab_group,
		       GParamSpec              *pspec,
		       TeplLineColumnIndicator *indicator)
{
	active_view_changed (indicator);
}

/**
 * tepl_line_column_indicator_set_tab_group:
 * @indicator: a #TeplLineColumnIndicator.
 * @tab_group: a #TeplTabGroup.
 *
 * Sets a #TeplTabGroup to update automatically the values for the line and
 * column of the current cursor position, for the current
 * #TeplTabGroup:active-tab.
 *
 * This function can be called only once per @indicator.
 *
 * Since: 6.4
 */
void
tepl_line_column_indicator_set_tab_group (TeplLineColumnIndicator *indicator,
					  TeplTabGroup            *tab_group)
{
	g_return_if_fail (TEPL_IS_LINE_COLUMN_INDICATOR (indicator));
	g_return_if_fail (TEPL_IS_TAB_GROUP (tab_group));
	g_return_if_fail (indicator->priv->tab_group == NULL);

	indicator->priv->tab_group = g_object_ref_sink (tab_group);

	g_signal_connect_object (tab_group,
				 "notify::active-view",
				 G_CALLBACK (active_view_notify_cb),
				 indicator,
				 0);

	active_view_changed (indicator);
}
