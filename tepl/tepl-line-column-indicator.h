/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_LINE_COLUMN_INDICATOR_H
#define TEPL_LINE_COLUMN_INDICATOR_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <tepl/tepl-tab-group.h>
#include <tepl/tepl-macros.h>

G_BEGIN_DECLS

#define TEPL_TYPE_LINE_COLUMN_INDICATOR             (tepl_line_column_indicator_get_type ())
#define TEPL_LINE_COLUMN_INDICATOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_LINE_COLUMN_INDICATOR, TeplLineColumnIndicator))
#define TEPL_LINE_COLUMN_INDICATOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_LINE_COLUMN_INDICATOR, TeplLineColumnIndicatorClass))
#define TEPL_IS_LINE_COLUMN_INDICATOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_LINE_COLUMN_INDICATOR))
#define TEPL_IS_LINE_COLUMN_INDICATOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_LINE_COLUMN_INDICATOR))
#define TEPL_LINE_COLUMN_INDICATOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_LINE_COLUMN_INDICATOR, TeplLineColumnIndicatorClass))

typedef struct _TeplLineColumnIndicator         TeplLineColumnIndicator;
typedef struct _TeplLineColumnIndicatorClass    TeplLineColumnIndicatorClass;
typedef struct _TeplLineColumnIndicatorPrivate  TeplLineColumnIndicatorPrivate;

struct _TeplLineColumnIndicator
{
	GtkBin parent;

	TeplLineColumnIndicatorPrivate *priv;
};

struct _TeplLineColumnIndicatorClass
{
	GtkBinClass parent_class;

	gpointer padding[12];
};

_TEPL_EXTERN
GType		tepl_line_column_indicator_get_type		(void);

_TEPL_EXTERN
TeplLineColumnIndicator *
		tepl_line_column_indicator_new			(void);

_TEPL_EXTERN
void		tepl_line_column_indicator_set_tab_group	(TeplLineColumnIndicator *indicator,
								 TeplTabGroup            *tab_group);

G_END_DECLS

#endif /* TEPL_LINE_COLUMN_INDICATOR_H */
