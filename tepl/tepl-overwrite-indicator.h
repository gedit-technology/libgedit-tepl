/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_OVERWRITE_INDICATOR_H
#define TEPL_OVERWRITE_INDICATOR_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <tepl/tepl-macros.h>

G_BEGIN_DECLS

#define TEPL_TYPE_OVERWRITE_INDICATOR             (tepl_overwrite_indicator_get_type ())
#define TEPL_OVERWRITE_INDICATOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_OVERWRITE_INDICATOR, TeplOverwriteIndicator))
#define TEPL_OVERWRITE_INDICATOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_OVERWRITE_INDICATOR, TeplOverwriteIndicatorClass))
#define TEPL_IS_OVERWRITE_INDICATOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_OVERWRITE_INDICATOR))
#define TEPL_IS_OVERWRITE_INDICATOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_OVERWRITE_INDICATOR))
#define TEPL_OVERWRITE_INDICATOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_OVERWRITE_INDICATOR, TeplOverwriteIndicatorClass))

typedef struct _TeplOverwriteIndicator         TeplOverwriteIndicator;
typedef struct _TeplOverwriteIndicatorClass    TeplOverwriteIndicatorClass;
typedef struct _TeplOverwriteIndicatorPrivate  TeplOverwriteIndicatorPrivate;

struct _TeplOverwriteIndicator
{
	GtkBin parent;

	TeplOverwriteIndicatorPrivate *priv;
};

struct _TeplOverwriteIndicatorClass
{
	GtkBinClass parent_class;

	gpointer padding[12];
};

_TEPL_EXTERN
GType				tepl_overwrite_indicator_get_type	(void);

_TEPL_EXTERN
TeplOverwriteIndicator *	tepl_overwrite_indicator_new		(void);

_TEPL_EXTERN
gboolean			tepl_overwrite_indicator_get_overwrite	(TeplOverwriteIndicator *indicator);

_TEPL_EXTERN
void				tepl_overwrite_indicator_set_overwrite	(TeplOverwriteIndicator *indicator,
									 gboolean                overwrite);

_TEPL_EXTERN
void				tepl_overwrite_indicator_set_view	(TeplOverwriteIndicator *indicator,
									 GtkTextView            *view);

G_END_DECLS

#endif /* TEPL_OVERWRITE_INDICATOR_H */
