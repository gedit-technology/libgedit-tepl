/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PANEL_CHOOSER_MENU_H
#define TEPL_PANEL_CHOOSER_MENU_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <tepl/tepl-macros.h>

G_BEGIN_DECLS

#define TEPL_TYPE_PANEL_CHOOSER_MENU             (tepl_panel_chooser_menu_get_type ())
#define TEPL_PANEL_CHOOSER_MENU(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PANEL_CHOOSER_MENU, TeplPanelChooserMenu))
#define TEPL_PANEL_CHOOSER_MENU_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_PANEL_CHOOSER_MENU, TeplPanelChooserMenuClass))
#define TEPL_IS_PANEL_CHOOSER_MENU(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PANEL_CHOOSER_MENU))
#define TEPL_IS_PANEL_CHOOSER_MENU_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_PANEL_CHOOSER_MENU))
#define TEPL_PANEL_CHOOSER_MENU_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_PANEL_CHOOSER_MENU, TeplPanelChooserMenuClass))

typedef struct _TeplPanelChooserMenu         TeplPanelChooserMenu;
typedef struct _TeplPanelChooserMenuClass    TeplPanelChooserMenuClass;
typedef struct _TeplPanelChooserMenuPrivate  TeplPanelChooserMenuPrivate;

struct _TeplPanelChooserMenu
{
	GtkBin parent;

	TeplPanelChooserMenuPrivate *priv;
};

struct _TeplPanelChooserMenuClass
{
	GtkBinClass parent_class;

	gpointer padding[12];
};

_TEPL_EXTERN
GType			tepl_panel_chooser_menu_get_type	(void);

_TEPL_EXTERN
TeplPanelChooserMenu *	tepl_panel_chooser_menu_new		(void);

G_END_DECLS

#endif /* TEPL_PANEL_CHOOSER_MENU_H */
