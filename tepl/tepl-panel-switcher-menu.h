/* SPDX-FileCopyrightText: 2023-2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PANEL_SWITCHER_MENU_H
#define TEPL_PANEL_SWITCHER_MENU_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <tepl/tepl-panel-simple.h>

G_BEGIN_DECLS

#define TEPL_TYPE_PANEL_SWITCHER_MENU             (tepl_panel_switcher_menu_get_type ())
#define TEPL_PANEL_SWITCHER_MENU(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PANEL_SWITCHER_MENU, TeplPanelSwitcherMenu))
#define TEPL_PANEL_SWITCHER_MENU_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_PANEL_SWITCHER_MENU, TeplPanelSwitcherMenuClass))
#define TEPL_IS_PANEL_SWITCHER_MENU(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PANEL_SWITCHER_MENU))
#define TEPL_IS_PANEL_SWITCHER_MENU_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_PANEL_SWITCHER_MENU))
#define TEPL_PANEL_SWITCHER_MENU_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_PANEL_SWITCHER_MENU, TeplPanelSwitcherMenuClass))

typedef struct _TeplPanelSwitcherMenu         TeplPanelSwitcherMenu;
typedef struct _TeplPanelSwitcherMenuClass    TeplPanelSwitcherMenuClass;
typedef struct _TeplPanelSwitcherMenuPrivate  TeplPanelSwitcherMenuPrivate;

struct _TeplPanelSwitcherMenu
{
	GtkBin parent;

	TeplPanelSwitcherMenuPrivate *priv;
};

struct _TeplPanelSwitcherMenuClass
{
	GtkBinClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GType			tepl_panel_switcher_menu_get_type	(void);

G_MODULE_EXPORT
TeplPanelSwitcherMenu *	tepl_panel_switcher_menu_new		(TeplPanelSimple *panel);

G_END_DECLS

#endif /* TEPL_PANEL_SWITCHER_MENU_H */
