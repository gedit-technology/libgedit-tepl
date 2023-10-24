/* SPDX-FileCopyrightText: 2016 - David Rabel <david.rabel@noresoft.com>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_FOLD_REGION_H
#define TEPL_FOLD_REGION_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TEPL_TYPE_FOLD_REGION (tepl_fold_region_get_type ())
G_MODULE_EXPORT
G_DECLARE_DERIVABLE_TYPE (TeplFoldRegion, tepl_fold_region,
			  TEPL, FOLD_REGION,
			  GObject)

struct _TeplFoldRegionClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
TeplFoldRegion *
		tepl_fold_region_new		(GtkTextBuffer     *buffer,
						 const GtkTextIter *start,
						 const GtkTextIter *end);

G_MODULE_EXPORT
GtkTextBuffer * tepl_fold_region_get_buffer	(TeplFoldRegion    *fold_region);

G_MODULE_EXPORT
gboolean	tepl_fold_region_get_folded 	(TeplFoldRegion    *fold_region);

G_MODULE_EXPORT
void		tepl_fold_region_set_folded 	(TeplFoldRegion    *fold_region,
						 gboolean           folded);

G_MODULE_EXPORT
gboolean	tepl_fold_region_get_bounds	(TeplFoldRegion    *fold_region,
						 GtkTextIter       *start,
						 GtkTextIter       *end);

G_MODULE_EXPORT
void		tepl_fold_region_set_bounds	(TeplFoldRegion    *fold_region,
						 const GtkTextIter *start,
						 const GtkTextIter *end);

G_END_DECLS

#endif /* TEPL_FOLD_REGION_H */
