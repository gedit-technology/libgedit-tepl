/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "config.h"
#include "tepl-overwrite-indicator.h"
#include <glib/gi18n-lib.h>

/**
 * SECTION:overwrite-indicator
 * @Short_description: Indicates Insert or Overwrite mode for text edition
 * @Title: TeplOverwriteIndicator
 *
 * #TeplOverwriteIndicator indicates the mode in which text edition occurs:
 * insert or overwrite.
 *
 * See for example the #GtkTextView:overwrite property.
 *
 * The usual case is to pack this widget in a #GtkStatusbar.
 *
 * # TODO
 *
 * It is planned to extend this widget to be able to click on / activate it to
 * change the mode, with a #GtkPopover for example (but this would be optional,
 * with a "chooser" boolean construct-only property).
 *
 * With a different styling when in overwrite mode.
 *
 * Because not everybody is familiar with the key on the keyboard to switch
 * between the different modes.
 */

struct _TeplOverwriteIndicatorPrivate
{
	GtkStack *stack;
	GtkLabel *label_insert;
	GtkLabel *label_overwrite;

	guint overwrite : 1;
};

enum
{
	PROP_0,
	PROP_OVERWRITE,
	N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

G_DEFINE_TYPE_WITH_PRIVATE (TeplOverwriteIndicator, tepl_overwrite_indicator, GTK_TYPE_BIN)

static const gchar *
get_insert_string (void)
{
	/* Translators: "INS" is the abbreviation for "Insert", for the text
	 * cursor mode. The translation should be short too.
	 */
	return _("INS");
}

static const gchar *
get_overwrite_string (void)
{
	/* Translators: "OVR" is the abbreviation for "Overwrite", for the text
	 * cursor mode. The translation should be short too.
	 */
	return _("OVR");
}

static gchar *
get_string_with_padding (gboolean overwrite)
{
	const gchar *str;

	str = overwrite ? get_overwrite_string () : get_insert_string ();

	/* Use spaces to leave padding proportional to the font size. */
	return g_strdup_printf ("  %s  ", str);
}

static void
tepl_overwrite_indicator_get_property (GObject    *object,
                                       guint       prop_id,
                                       GValue     *value,
                                       GParamSpec *pspec)
{
	TeplOverwriteIndicator *indicator = TEPL_OVERWRITE_INDICATOR (object);

	switch (prop_id)
	{
		case PROP_OVERWRITE:
			g_value_set_boolean (value, tepl_overwrite_indicator_get_overwrite (indicator));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_overwrite_indicator_set_property (GObject      *object,
                                       guint         prop_id,
                                       const GValue *value,
                                       GParamSpec   *pspec)
{
	TeplOverwriteIndicator *indicator = TEPL_OVERWRITE_INDICATOR (object);

	switch (prop_id)
	{
		case PROP_OVERWRITE:
			tepl_overwrite_indicator_set_overwrite (indicator, g_value_get_boolean (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
tepl_overwrite_indicator_class_init (TeplOverwriteIndicatorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = tepl_overwrite_indicator_get_property;
	object_class->set_property = tepl_overwrite_indicator_set_property;

	/**
	 * TeplOverwriteIndicator:overwrite:
	 *
	 * - If %TRUE, indicates the overwrite mode.
	 * - If %FALSE, indicates the insert mode.
	 *
	 * Since: 6.4
	 */
	properties[PROP_OVERWRITE] =
		g_param_spec_boolean ("overwrite",
				      "overwrite",
				      "",
				      FALSE,
				      G_PARAM_READWRITE |
				      G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
tepl_overwrite_indicator_init (TeplOverwriteIndicator *indicator)
{
	gchar *label_insert_str;
	gchar *label_overwrite_str;

	indicator->priv = tepl_overwrite_indicator_get_instance_private (indicator);

	label_insert_str = get_string_with_padding (FALSE);
	label_overwrite_str = get_string_with_padding (TRUE);

	indicator->priv->label_insert = GTK_LABEL (gtk_label_new (label_insert_str));
	indicator->priv->label_overwrite = GTK_LABEL (gtk_label_new (label_overwrite_str));

	g_free (label_insert_str);
	g_free (label_overwrite_str);

	gtk_widget_show (GTK_WIDGET (indicator->priv->label_insert));
	gtk_widget_show (GTK_WIDGET (indicator->priv->label_overwrite));

	indicator->priv->stack = GTK_STACK (gtk_stack_new ());

	gtk_container_add (GTK_CONTAINER (indicator->priv->stack),
			   GTK_WIDGET (indicator->priv->label_insert));
	gtk_container_add (GTK_CONTAINER (indicator->priv->stack),
			   GTK_WIDGET (indicator->priv->label_overwrite));

	gtk_stack_set_visible_child (indicator->priv->stack,
				     GTK_WIDGET (indicator->priv->label_insert));
	gtk_widget_show (GTK_WIDGET (indicator->priv->stack));

	gtk_container_add (GTK_CONTAINER (indicator),
			   GTK_WIDGET (indicator->priv->stack));
}

/**
 * tepl_overwrite_indicator_new:
 *
 * Returns: (transfer floating): a new #TeplOverwriteIndicator.
 * Since: 6.4
 */
TeplOverwriteIndicator *
tepl_overwrite_indicator_new (void)
{
	return g_object_new (TEPL_TYPE_OVERWRITE_INDICATOR, NULL);
}

/**
 * tepl_overwrite_indicator_get_overwrite:
 * @indicator: a #TeplOverwriteIndicator.
 *
 * Returns: the value of the #TeplOverwriteIndicator:overwrite property.
 * Since: 6.4
 */
gboolean
tepl_overwrite_indicator_get_overwrite (TeplOverwriteIndicator *indicator)
{
	g_return_val_if_fail (TEPL_IS_OVERWRITE_INDICATOR (indicator), FALSE);

	return indicator->priv->overwrite;
}

/**
 * tepl_overwrite_indicator_set_overwrite:
 * @indicator: a #TeplOverwriteIndicator.
 * @overwrite: the new value.
 *
 * Sets a new value to the #TeplOverwriteIndicator:overwrite property.
 *
 * Since: 6.4
 */
void
tepl_overwrite_indicator_set_overwrite (TeplOverwriteIndicator *indicator,
					gboolean                overwrite)
{
	g_return_if_fail (TEPL_IS_OVERWRITE_INDICATOR (indicator));

	overwrite = overwrite != FALSE;

	if (indicator->priv->overwrite != overwrite)
	{
		indicator->priv->overwrite = overwrite;

		if (overwrite)
		{
			gtk_stack_set_visible_child (indicator->priv->stack,
						     GTK_WIDGET (indicator->priv->label_overwrite));
		}
		else
		{
			gtk_stack_set_visible_child (indicator->priv->stack,
						     GTK_WIDGET (indicator->priv->label_insert));
		}

		g_object_notify_by_pspec (G_OBJECT (indicator), properties[PROP_OVERWRITE]);
	}
}
