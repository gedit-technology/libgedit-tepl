/* SPDX-FileCopyrightText: 2008 - Jesse van den Kieboom
 * SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-status-menu-button.h"
#include "tepl-utils.h"

/**
 * SECTION:status-menu-button
 * @Title: TeplStatusMenuButton
 * @Short_description: A #GtkMenuButton suitable for #GtkStatusbar
 *
 * #TeplStatusMenuButton is a #GtkMenuButton that is suitable to be put in a
 * #GtkStatusbar.
 *
 * It contains:
 * - Some text.
 * - An arrow.
 * - When clicked, a menu or popover can be shown (see the #GtkMenuButton API).
 */

struct _TeplStatusMenuButtonPrivate
{
	GtkLabel *label;
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplStatusMenuButton, tepl_status_menu_button, GTK_TYPE_MENU_BUTTON)

static void
tepl_status_menu_button_dispose (GObject *object)
{
	TeplStatusMenuButton *button = TEPL_STATUS_MENU_BUTTON (object);

	button->priv->label = NULL;

	G_OBJECT_CLASS (tepl_status_menu_button_parent_class)->dispose (object);
}

static void
tepl_status_menu_button_class_init (TeplStatusMenuButtonClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = tepl_status_menu_button_dispose;
}

static void
create_label (TeplStatusMenuButton *button)
{
	g_return_if_fail (button->priv->label == NULL);

	button->priv->label = GTK_LABEL (gtk_label_new (NULL));
	gtk_widget_set_valign (GTK_WIDGET (button->priv->label), GTK_ALIGN_BASELINE);
	gtk_label_set_single_line_mode (button->priv->label, TRUE);
}

static void
apply_css (TeplStatusMenuButton *button)
{
	GtkCssProvider *css_provider;
	GtkStyleContext *style_context;
	GError *error = NULL;
	const gchar *css_style =
		"button {\n"
		"  padding: 0px 3px;\n"
		"  border: none;\n"
		"  outline-style: none;\n"
		"}\n";

	css_provider = gtk_css_provider_new ();
	gtk_css_provider_load_from_data (css_provider, css_style, -1, &error);
	if (error != NULL)
	{
		g_warning ("TeplStatusMenuButton CSS error: %s", error->message);
		g_clear_error (&error);
	}

	style_context = gtk_widget_get_style_context (GTK_WIDGET (button));
	gtk_style_context_add_provider (style_context,
					GTK_STYLE_PROVIDER (css_provider),
					TEPL_UTILS_STYLE_PROVIDER_PRIORITY_LIBRARY);
	g_object_unref (css_provider);
}

static void
tepl_status_menu_button_init (TeplStatusMenuButton *button)
{
	GtkWidget *arrow;
	GtkGrid *hgrid;

	button->priv = tepl_status_menu_button_get_instance_private (button);

	create_label (button);

	arrow = gtk_image_new_from_icon_name ("pan-down-symbolic", GTK_ICON_SIZE_BUTTON);
	gtk_widget_set_valign (arrow, GTK_ALIGN_BASELINE);

	hgrid = GTK_GRID (gtk_grid_new ());
	gtk_grid_set_column_spacing (hgrid, 3);
	gtk_widget_set_valign (GTK_WIDGET (hgrid), GTK_ALIGN_CENTER);
	gtk_container_add (GTK_CONTAINER (hgrid), GTK_WIDGET (button->priv->label));
	gtk_container_add (GTK_CONTAINER (hgrid), arrow);

	gtk_widget_show_all (GTK_WIDGET (hgrid));
	gtk_container_add (GTK_CONTAINER (button),
			   GTK_WIDGET (hgrid));

	gtk_menu_button_set_direction (GTK_MENU_BUTTON (button), GTK_ARROW_UP);
	gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
	apply_css (button);
}

/**
 * tepl_status_menu_button_new:
 *
 * Returns: (transfer floating): a new #TeplStatusMenuButton widget.
 * Since: 6.8
 */
TeplStatusMenuButton *
tepl_status_menu_button_new (void)
{
	return g_object_new (TEPL_TYPE_STATUS_MENU_BUTTON, NULL);
}

/**
 * tepl_status_menu_button_set_label_text:
 * @button: a #TeplStatusMenuButton.
 * @str: the text to set.
 *
 * To change the text displayed in @button.
 *
 * Since: 6.8
 */
void
tepl_status_menu_button_set_label_text (TeplStatusMenuButton *button,
					const gchar          *str)
{
	g_return_if_fail (TEPL_IS_STATUS_MENU_BUTTON (button));
	g_return_if_fail (str != NULL);

	gtk_label_set_label (button->priv->label, str);
}
