/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_STYLE_SCHEME_ROW_H
#define TEPL_STYLE_SCHEME_ROW_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TEPL_TYPE_STYLE_SCHEME_ROW             (_tepl_style_scheme_row_get_type ())
#define TEPL_STYLE_SCHEME_ROW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_STYLE_SCHEME_ROW, TeplStyleSchemeRow))
#define TEPL_STYLE_SCHEME_ROW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_STYLE_SCHEME_ROW, TeplStyleSchemeRowClass))
#define TEPL_IS_STYLE_SCHEME_ROW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_STYLE_SCHEME_ROW))
#define TEPL_IS_STYLE_SCHEME_ROW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_STYLE_SCHEME_ROW))
#define TEPL_STYLE_SCHEME_ROW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_STYLE_SCHEME_ROW, TeplStyleSchemeRowClass))

typedef struct _TeplStyleSchemeRow         TeplStyleSchemeRow;
typedef struct _TeplStyleSchemeRowClass    TeplStyleSchemeRowClass;
typedef struct _TeplStyleSchemeRowPrivate  TeplStyleSchemeRowPrivate;

struct _TeplStyleSchemeRow
{
	GtkListBoxRow parent;

	TeplStyleSchemeRowPrivate *priv;
};

struct _TeplStyleSchemeRowClass
{
	GtkListBoxRowClass parent_class;
};

G_GNUC_INTERNAL
GType			_tepl_style_scheme_row_get_type	(void);

G_GNUC_INTERNAL
TeplStyleSchemeRow *	_tepl_style_scheme_row_new	(void);

G_END_DECLS

#endif /* TEPL_STYLE_SCHEME_ROW_H */
