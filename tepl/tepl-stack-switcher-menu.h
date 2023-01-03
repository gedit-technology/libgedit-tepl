/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_STACK_SWITCHER_MENU_H
#define TEPL_STACK_SWITCHER_MENU_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <tepl/tepl-macros.h>

G_BEGIN_DECLS

#define TEPL_TYPE_STACK_SWITCHER_MENU             (tepl_stack_switcher_menu_get_type ())
#define TEPL_STACK_SWITCHER_MENU(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_STACK_SWITCHER_MENU, TeplStackSwitcherMenu))
#define TEPL_STACK_SWITCHER_MENU_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_STACK_SWITCHER_MENU, TeplStackSwitcherMenuClass))
#define TEPL_IS_STACK_SWITCHER_MENU(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_STACK_SWITCHER_MENU))
#define TEPL_IS_STACK_SWITCHER_MENU_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_STACK_SWITCHER_MENU))
#define TEPL_STACK_SWITCHER_MENU_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_STACK_SWITCHER_MENU, TeplStackSwitcherMenuClass))

typedef struct _TeplStackSwitcherMenu         TeplStackSwitcherMenu;
typedef struct _TeplStackSwitcherMenuClass    TeplStackSwitcherMenuClass;
typedef struct _TeplStackSwitcherMenuPrivate  TeplStackSwitcherMenuPrivate;

struct _TeplStackSwitcherMenu
{
	GtkBin parent;

	TeplStackSwitcherMenuPrivate *priv;
};

struct _TeplStackSwitcherMenuClass
{
	GtkBinClass parent_class;

	gpointer padding[12];
};

_TEPL_EXTERN
GType			tepl_stack_switcher_menu_get_type	(void);

_TEPL_EXTERN
TeplStackSwitcherMenu *	tepl_stack_switcher_menu_new		(void);

G_END_DECLS

#endif /* TEPL_STACK_SWITCHER_MENU_H */
