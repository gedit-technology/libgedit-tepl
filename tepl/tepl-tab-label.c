/*
 * This file is part of Tepl, a text editor library.
 *
 * Copyright 2017 - Sébastien Wilmet <swilmet@gnome.org>
 *
 * Tepl is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * Tepl is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "tepl-tab-label.h"
#include "tepl-tab.h"
#include "tepl-buffer.h"
#include "tepl-signal-group.h"

/**
 * SECTION:tab-label
 * @Short_description: A #TeplTab label, to use with #GtkNotebook
 * @Title: TeplTabLabel
 *
 * #TeplTabLabel is the label/title of a #TeplTab, suitable for #GtkNotebook.
 */

struct _TeplTabLabelPrivate
{
	TeplTab *tab;
	TeplSignalGroup *buffer_signal_group;

	GtkLabel *label;
};

enum
{
	PROP_0,
	PROP_TAB,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

G_DEFINE_TYPE_WITH_PRIVATE (TeplTabLabel, tepl_tab_label, GTK_TYPE_GRID)

static void
update_label (TeplTabLabel *tab_label)
{
	TeplBuffer *buffer;
	gchar *short_title;

	buffer = tepl_tab_get_buffer (tab_label->priv->tab);
	short_title = tepl_buffer_get_short_title (buffer);

	gtk_label_set_text (tab_label->priv->label, short_title);

	g_free (short_title);
}

static void
buffer_short_title_notify_cb (TeplBuffer   *buffer,
			      GParamSpec   *pspec,
			      TeplTabLabel *tab_label)
{
	update_label (tab_label);
}

static void
buffer_changed (TeplTabLabel *tab_label)
{
	TeplBuffer *buffer;

	_tepl_signal_group_clear (&tab_label->priv->buffer_signal_group);

	buffer = tepl_tab_get_buffer (tab_label->priv->tab);
	tab_label->priv->buffer_signal_group = _tepl_signal_group_new (G_OBJECT (buffer));

	_tepl_signal_group_add (tab_label->priv->buffer_signal_group,
				g_signal_connect (buffer,
						  "notify::tepl-short-title",
						  G_CALLBACK (buffer_short_title_notify_cb),
						  tab_label));

	update_label (tab_label);
}

static void
buffer_notify_cb (GtkTextView  *view,
		  GParamSpec   *pspec,
		  TeplTabLabel *tab_label)
{
	buffer_changed (tab_label);
}

static void
set_tab (TeplTabLabel *tab_label,
	 TeplTab      *tab)
{
	TeplView *view;

	g_return_if_fail (TEPL_IS_TAB (tab));

	g_assert (tab_label->priv->tab == NULL);
	tab_label->priv->tab = g_object_ref (tab);

	view = tepl_tab_get_view (tab);
	g_signal_connect_object (view,
				 "notify::buffer",
				 G_CALLBACK (buffer_notify_cb),
				 tab_label,
				 0);

	buffer_changed (tab_label);
}

static void
tepl_tab_label_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
	TeplTabLabel *tab_label = TEPL_TAB_LABEL (object);

	switch (prop_id)
	{
		case PROP_TAB:
			g_value_set_object (value, tepl_tab_label_get_tab (tab_label));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_tab_label_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
	TeplTabLabel *tab_label = TEPL_TAB_LABEL (object);

	switch (prop_id)
	{
		case PROP_TAB:
			set_tab (tab_label, g_value_get_object (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_tab_label_dispose (GObject *object)
{
	TeplTabLabel *tab_label = TEPL_TAB_LABEL (object);

	g_clear_object (&tab_label->priv->tab);
	_tepl_signal_group_clear (&tab_label->priv->buffer_signal_group);

	G_OBJECT_CLASS (tepl_tab_label_parent_class)->dispose (object);
}

static void
tepl_tab_label_class_init (TeplTabLabelClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = tepl_tab_label_get_property;
	object_class->set_property = tepl_tab_label_set_property;
	object_class->dispose = tepl_tab_label_dispose;

	/**
	 * TeplTabLabel:tab:
	 *
	 * The associated #TeplTab. #TeplTabLabel has a strong reference to the
	 * #TeplTab.
	 *
	 * Since: 3.0
	 */
	properties[PROP_TAB] =
		g_param_spec_object ("tab",
				     "tab",
				     "",
				     TEPL_TYPE_TAB,
				     G_PARAM_READWRITE |
				     G_PARAM_CONSTRUCT_ONLY |
				     G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
tepl_tab_label_init (TeplTabLabel *tab_label)
{
	tab_label->priv = tepl_tab_label_get_instance_private (tab_label);

	tab_label->priv->label = GTK_LABEL (gtk_label_new (NULL));
	gtk_widget_show (GTK_WIDGET (tab_label->priv->label));
	gtk_container_add (GTK_CONTAINER (tab_label),
			   GTK_WIDGET (tab_label->priv->label));
}

/**
 * tepl_tab_label_new:
 * @tab: a #TeplTab.
 *
 * Returns: (transfer floating): a new #TeplTabLabel.
 * Since: 3.0
 */
GtkWidget *
tepl_tab_label_new (TeplTab *tab)
{
	g_return_val_if_fail (TEPL_IS_TAB (tab), NULL);

	return g_object_new (TEPL_TYPE_TAB_LABEL,
			     "tab", tab,
			     NULL);
}

/**
 * tepl_tab_label_get_tab:
 * @tab_label: a #TeplTabLabel.
 *
 * Returns: (transfer none): the #TeplTabLabel:tab.
 * Since: 3.0
 */
TeplTab *
tepl_tab_label_get_tab (TeplTabLabel *tab_label)
{
	g_return_val_if_fail (TEPL_IS_TAB_LABEL (tab_label), NULL);

	return tab_label->priv->tab;
}