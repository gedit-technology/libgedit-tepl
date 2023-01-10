/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_STACK_H
#define TEPL_STACK_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <tepl/tepl-stack-item.h>

G_BEGIN_DECLS

#define TEPL_TYPE_STACK             (tepl_stack_get_type ())
#define TEPL_STACK(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_STACK, TeplStack))
#define TEPL_STACK_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_STACK, TeplStackClass))
#define TEPL_IS_STACK(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_STACK))
#define TEPL_IS_STACK_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_STACK))
#define TEPL_STACK_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_STACK, TeplStackClass))

typedef struct _TeplStack         TeplStack;
typedef struct _TeplStackClass    TeplStackClass;
typedef struct _TeplStackPrivate  TeplStackPrivate;

struct _TeplStack
{
	GtkBin parent;

	TeplStackPrivate *priv;
};

struct _TeplStackClass
{
	GtkBinClass parent_class;

	gpointer padding[12];
};

_TEPL_EXTERN
GType		tepl_stack_get_type		(void);

_TEPL_EXTERN
TeplStack *	tepl_stack_new			(void);

_TEPL_EXTERN
void		tepl_stack_add_item		(TeplStack     *stack,
						 TeplStackItem *item);

_TEPL_EXTERN
void		tepl_stack_remove_item		(TeplStack     *stack,
						 TeplStackItem *item);

_TEPL_EXTERN
GList *		tepl_stack_get_items		(TeplStack *stack);

_TEPL_EXTERN
void		tepl_stack_changed		(TeplStack *stack);

_TEPL_EXTERN
gboolean	tepl_stack_has_several_items	(TeplStack *stack);

_TEPL_EXTERN
guint		tepl_stack_get_n_visible_items	(TeplStack *stack);

_TEPL_EXTERN
TeplStackItem *	tepl_stack_get_visible_item	(TeplStack *stack);

_TEPL_EXTERN
void		tepl_stack_set_visible_item	(TeplStack     *stack,
						 TeplStackItem *item);

G_END_DECLS

#endif /* TEPL_STACK_H */
