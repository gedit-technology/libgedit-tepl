/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PANEL_STACK_H
#define TEPL_PANEL_STACK_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <tepl/tepl-panel-simple.h>

G_BEGIN_DECLS

#define TEPL_TYPE_PANEL_STACK             (tepl_panel_stack_get_type ())
#define TEPL_PANEL_STACK(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PANEL_STACK, TeplPanelStack))
#define TEPL_PANEL_STACK_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_PANEL_STACK, TeplPanelStackClass))
#define TEPL_IS_PANEL_STACK(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PANEL_STACK))
#define TEPL_IS_PANEL_STACK_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_PANEL_STACK))
#define TEPL_PANEL_STACK_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_PANEL_STACK, TeplPanelStackClass))

typedef struct _TeplPanelStack         TeplPanelStack;
typedef struct _TeplPanelStackClass    TeplPanelStackClass;
typedef struct _TeplPanelStackPrivate  TeplPanelStackPrivate;

struct _TeplPanelStack
{
	GObject parent;

	TeplPanelStackPrivate *priv;
};

struct _TeplPanelStackClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GType			tepl_panel_stack_get_type		(void);

G_MODULE_EXPORT
TeplPanelStack *	tepl_panel_stack_new			(TeplPanelSimple *panel_simple,
								 GtkStack        *stack);

G_MODULE_EXPORT
TeplPanelSimple *	tepl_panel_stack_get_panel_simple	(TeplPanelStack *panel_stack);

G_MODULE_EXPORT
GtkStack *		tepl_panel_stack_get_stack		(TeplPanelStack *panel_stack);

G_END_DECLS

#endif /* TEPL_PANEL_STACK_H */
