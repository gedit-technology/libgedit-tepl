/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PANEL_CONTAINER_H
#define TEPL_PANEL_CONTAINER_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <tepl/tepl-panel-item.h>

G_BEGIN_DECLS

#define TEPL_TYPE_PANEL_CONTAINER             (tepl_panel_container_get_type ())
#define TEPL_PANEL_CONTAINER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PANEL_CONTAINER, TeplPanelContainer))
#define TEPL_PANEL_CONTAINER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_PANEL_CONTAINER, TeplPanelContainerClass))
#define TEPL_IS_PANEL_CONTAINER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PANEL_CONTAINER))
#define TEPL_IS_PANEL_CONTAINER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_PANEL_CONTAINER))
#define TEPL_PANEL_CONTAINER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_PANEL_CONTAINER, TeplPanelContainerClass))

typedef struct _TeplPanelContainer         TeplPanelContainer;
typedef struct _TeplPanelContainerClass    TeplPanelContainerClass;
typedef struct _TeplPanelContainerPrivate  TeplPanelContainerPrivate;

struct _TeplPanelContainer
{
	GtkBin parent;

	TeplPanelContainerPrivate *priv;
};

struct _TeplPanelContainerClass
{
	GtkBinClass parent_class;

	gpointer padding[12];
};

_TEPL_EXTERN
GType			tepl_panel_container_get_type			(void);

_TEPL_EXTERN
TeplPanelContainer *	tepl_panel_container_new			(void);

_TEPL_EXTERN
GList *			tepl_panel_container_get_items			(TeplPanelContainer *container);

_TEPL_EXTERN
TeplPanelItem *		tepl_panel_container_get_active_item		(TeplPanelContainer *container);

_TEPL_EXTERN
const gchar *		tepl_panel_container_get_active_item_name	(TeplPanelContainer *container);

_TEPL_EXTERN
void			tepl_panel_container_set_active_item_name	(TeplPanelContainer *container,
									 const gchar        *name);

_TEPL_EXTERN
gboolean		tepl_panel_container_has_several_items		(TeplPanelContainer *container);

G_END_DECLS

#endif /* TEPL_PANEL_CONTAINER_H */
