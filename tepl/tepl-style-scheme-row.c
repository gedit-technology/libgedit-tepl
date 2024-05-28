/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-style-scheme-row.h"

struct _TeplStyleSchemeRowPrivate
{
	GtkSourceStyleScheme *style_scheme;
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplStyleSchemeRow, _tepl_style_scheme_row, GTK_TYPE_LIST_BOX_ROW)

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

TeplStyleSchemeRow *
_tepl_style_scheme_row_new (GtkSourceStyleScheme *style_scheme)
{
	TeplStyleSchemeRow *row;

	g_return_val_if_fail (GTK_SOURCE_IS_STYLE_SCHEME (style_scheme), NULL);

	row = g_object_new (TEPL_TYPE_STYLE_SCHEME_ROW, NULL);

	row->priv->style_scheme = g_object_ref (style_scheme);

	return row;
}

GtkSourceStyleScheme *
_tepl_style_scheme_row_get_style_scheme (TeplStyleSchemeRow *row)
{
	g_return_val_if_fail (TEPL_IS_STYLE_SCHEME_ROW (row), NULL);
	return row->priv->style_scheme;
}
