/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_STACK_ITEM_H
#define TEPL_STACK_ITEM_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <tepl/tepl-macros.h>

G_BEGIN_DECLS

#define TEPL_TYPE_STACK_ITEM             (tepl_stack_item_get_type ())
#define TEPL_STACK_ITEM(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_STACK_ITEM, TeplStackItem))
#define TEPL_STACK_ITEM_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_STACK_ITEM, TeplStackItemClass))
#define TEPL_IS_STACK_ITEM(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_STACK_ITEM))
#define TEPL_IS_STACK_ITEM_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_STACK_ITEM))
#define TEPL_STACK_ITEM_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_STACK_ITEM, TeplStackItemClass))

typedef struct _TeplStackItem         TeplStackItem;
typedef struct _TeplStackItemClass    TeplStackItemClass;
typedef struct _TeplStackItemPrivate  TeplStackItemPrivate;

struct _TeplStackItem
{
	GObject parent;

	TeplStackItemPrivate *priv;
};

struct _TeplStackItemClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

_TEPL_EXTERN
GType		tepl_stack_item_get_type		(void);

_TEPL_EXTERN
TeplStackItem *	tepl_stack_item_new			(GtkWidget   *widget,
							 const gchar *name,
							 const gchar *title,
							 const gchar *icon_name);

_TEPL_EXTERN
GtkWidget *	tepl_stack_item_get_widget		(TeplStackItem *item);

_TEPL_EXTERN
void		tepl_stack_item_get_infos		(TeplStackItem  *item,
							 gchar         **name,
							 gchar         **title,
							 gchar         **icon_name);

_TEPL_EXTERN
gint		tepl_stack_item_compare_by_title	(TeplStackItem *a,
							 TeplStackItem *b);

G_END_DECLS

#endif /* TEPL_STACK_ITEM_H */
