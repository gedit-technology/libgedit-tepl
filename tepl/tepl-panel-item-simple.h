/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PANEL_ITEM_SIMPLE_H
#define TEPL_PANEL_ITEM_SIMPLE_H

#include <gtk/gtk.h>
#include "tepl-panel-item.h"

G_BEGIN_DECLS

#define TEPL_TYPE_PANEL_ITEM_SIMPLE             (_tepl_panel_item_simple_get_type ())
#define TEPL_PANEL_ITEM_SIMPLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PANEL_ITEM_SIMPLE, TeplPanelItemSimple))
#define TEPL_PANEL_ITEM_SIMPLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_PANEL_ITEM_SIMPLE, TeplPanelItemSimpleClass))
#define TEPL_IS_PANEL_ITEM_SIMPLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PANEL_ITEM_SIMPLE))
#define TEPL_IS_PANEL_ITEM_SIMPLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_PANEL_ITEM_SIMPLE))
#define TEPL_PANEL_ITEM_SIMPLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_PANEL_ITEM_SIMPLE, TeplPanelItemSimpleClass))

typedef struct _TeplPanelItemSimple         TeplPanelItemSimple;
typedef struct _TeplPanelItemSimpleClass    TeplPanelItemSimpleClass;
typedef struct _TeplPanelItemSimplePrivate  TeplPanelItemSimplePrivate;

struct _TeplPanelItemSimple
{
	GObject parent;

	TeplPanelItemSimplePrivate *priv;
};

struct _TeplPanelItemSimpleClass
{
	GObjectClass parent_class;
};

G_GNUC_INTERNAL
GType		_tepl_panel_item_simple_get_type	(void);

G_GNUC_INTERNAL
TeplPanelItem *	_tepl_panel_item_simple_new		(GtkWidget   *widget,
							 const gchar *name,
							 const gchar *title,
							 const gchar *icon_name);

G_END_DECLS

#endif /* TEPL_PANEL_ITEM_SIMPLE_H */
