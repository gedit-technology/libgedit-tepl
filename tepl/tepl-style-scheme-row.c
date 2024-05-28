/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "tepl-style-scheme-row.h"

struct _TeplStyleSchemeRowPrivate
{
	gint something;
};

G_DEFINE_TYPE_WITH_PRIVATE (TeplStyleSchemeRow, _tepl_style_scheme_row, GTK_TYPE_LIST_BOX_ROW)

static void
_tepl_style_scheme_row_dispose (GObject *object)
{

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
_tepl_style_scheme_row_new (void)
{
	return g_object_new (TEPL_TYPE_STYLE_SCHEME_ROW, NULL);
}
