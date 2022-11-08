/* SPDX-FileCopyrightText: 2014 - Steve Frécinaux
 * SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_MENU_STACK_SWITCHER_H
#define TEPL_MENU_STACK_SWITCHER_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TEPL_TYPE_MENU_STACK_SWITCHER             (tepl_menu_stack_switcher_get_type ())
#define TEPL_MENU_STACK_SWITCHER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_MENU_STACK_SWITCHER, TeplMenuStackSwitcher))
#define TEPL_MENU_STACK_SWITCHER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_MENU_STACK_SWITCHER, TeplMenuStackSwitcherClass))
#define TEPL_IS_MENU_STACK_SWITCHER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_MENU_STACK_SWITCHER))
#define TEPL_IS_MENU_STACK_SWITCHER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_MENU_STACK_SWITCHER))
#define TEPL_MENU_STACK_SWITCHER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_MENU_STACK_SWITCHER, TeplMenuStackSwitcherClass))

typedef struct _TeplMenuStackSwitcher         TeplMenuStackSwitcher;
typedef struct _TeplMenuStackSwitcherClass    TeplMenuStackSwitcherClass;
typedef struct _TeplMenuStackSwitcherPrivate  TeplMenuStackSwitcherPrivate;

struct _TeplMenuStackSwitcher
{
	GtkMenuButton parent;

	TeplMenuStackSwitcherPrivate *priv;
};

struct _TeplMenuStackSwitcherClass
{
	GtkMenuButtonClass parent_class;

	gpointer padding[12];
};

GType			tepl_menu_stack_switcher_get_type	(void);

TeplMenuStackSwitcher *	tepl_menu_stack_switcher_new		(void);

void			tepl_menu_stack_switcher_set_stack	(TeplMenuStackSwitcher *switcher,
								 GtkStack              *stack);

GtkStack *		tepl_menu_stack_switcher_get_stack	(TeplMenuStackSwitcher *switcher);

G_END_DECLS

#endif /* TEPL_MENU_STACK_SWITCHER_H */
