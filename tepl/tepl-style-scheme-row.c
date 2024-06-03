/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "config.h"
#include "tepl-style-scheme-row.h"
#include <glib/gi18n-lib.h>

/* A GtkListBoxRow to be used in the GtkListBox of TeplStyleSchemeChooserWidget.
 * The row is something static. It does *not* change automatically when the
 * theme variant changes; when that happens, a new row needs to be created with
 * the new configuration.
 */

struct _TeplStyleSchemeRowPrivate
{
	GtkSourceStyleScheme *style_scheme; /* owned */
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplStyleSchemeRow, _tepl_style_scheme_row, GTK_TYPE_LIST_BOX_ROW)

static gboolean
is_supported (TeplStyleSchemeRow *row,
	      gboolean            for_dark_theme_variant)
{
	GtkSourceStyleSchemeKind kind;

	kind = gtk_source_style_scheme_get_kind (row->priv->style_scheme);

	switch (kind)
	{
		case GTK_SOURCE_STYLE_SCHEME_KIND_LIGHT:
		case GTK_SOURCE_STYLE_SCHEME_KIND_DARK:
			return TRUE;

		case GTK_SOURCE_STYLE_SCHEME_KIND_LIGHT_ONLY:
			return !for_dark_theme_variant;

		case GTK_SOURCE_STYLE_SCHEME_KIND_DARK_ONLY:
			return for_dark_theme_variant;

		default:
			break;
	}

	g_return_val_if_reached (FALSE);
}

static const gchar *
get_description (TeplStyleSchemeRow *row,
		 gboolean            for_dark_theme_variant)
{
	if (is_supported (row, for_dark_theme_variant))
	{
		return gtk_source_style_scheme_get_description (row->priv->style_scheme);
	}

	/* Instead of hiding the style schemes that are unsupported, show them
	 * with a message. That way the user understands, to know that there
	 * isn't a bug (no issues to report).
	 * Normally (at least with the default style schemes) the number of
	 * unsupported style schemes is small, so it's not a problem to add them
	 * to the list.
	 */
	if (for_dark_theme_variant)
	{
		return _("Unsupported for the dark theme variant");
	}

	return _("Unsupported for the light theme variant");
}

static void
_tepl_style_scheme_row_dispose (GObject *object)
{
	TeplStyleSchemeRow *row = TEPL_STYLE_SCHEME_ROW (object);

	g_clear_object (&row->priv->style_scheme);

	G_OBJECT_CLASS (_tepl_style_scheme_row_parent_class)->dispose (object);
}

static void
_tepl_style_scheme_row_class_init (TeplStyleSchemeRowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = _tepl_style_scheme_row_dispose;
}

static void
_tepl_style_scheme_row_init (TeplStyleSchemeRow *row)
{
	row->priv = _tepl_style_scheme_row_get_instance_private (row);
}

static void
add_label (TeplStyleSchemeRow *row,
	   gboolean            for_dark_theme_variant)
{
	const gchar *name;
	const gchar *description;
	gchar *markup;
	GtkWidget *label;

	name = gtk_source_style_scheme_get_name (row->priv->style_scheme);
	description = get_description (row, for_dark_theme_variant);

	if (description != NULL)
	{
		markup = g_markup_printf_escaped ("<b>%s</b> - %s", name, description);
	}
	else
	{
		markup = g_markup_printf_escaped ("<b>%s</b>", name);
	}

	label = gtk_label_new (NULL);
	gtk_label_set_markup (GTK_LABEL (label), markup);
	gtk_widget_set_halign (label, GTK_ALIGN_START);

	gtk_container_add (GTK_CONTAINER (row), label);

	g_free (markup);
}

TeplStyleSchemeRow *
_tepl_style_scheme_row_new (GtkSourceStyleScheme *style_scheme,
			    gboolean              for_dark_theme_variant)
{
	TeplStyleSchemeRow *row;
	gboolean selectable;

	g_return_val_if_fail (GTK_SOURCE_IS_STYLE_SCHEME (style_scheme), NULL);

	row = g_object_new (TEPL_TYPE_STYLE_SCHEME_ROW, NULL);
	row->priv->style_scheme = g_object_ref (style_scheme);

	add_label (row, for_dark_theme_variant);

	selectable = is_supported (row, for_dark_theme_variant);
	gtk_list_box_row_set_selectable (GTK_LIST_BOX_ROW (row), selectable);
	gtk_widget_set_sensitive (GTK_WIDGET (row), selectable);

	gtk_widget_show_all (GTK_WIDGET (row));

	return row;
}

GtkSourceStyleScheme *
_tepl_style_scheme_row_get_style_scheme (TeplStyleSchemeRow *row)
{
	g_return_val_if_fail (TEPL_IS_STYLE_SCHEME_ROW (row), NULL);
	return row->priv->style_scheme;
}
