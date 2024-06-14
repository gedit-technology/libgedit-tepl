/* SPDX-FileCopyrightText: 2023-2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PANEL_H
#define TEPL_PANEL_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <tepl/tepl-panel-item.h>

G_BEGIN_DECLS

#define TEPL_TYPE_PANEL               (tepl_panel_get_type ())
#define TEPL_PANEL(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PANEL, TeplPanel))
#define TEPL_IS_PANEL(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PANEL))
#define TEPL_PANEL_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), TEPL_TYPE_PANEL, TeplPanelInterface))

typedef struct _TeplPanel          TeplPanel;
typedef struct _TeplPanelInterface TeplPanelInterface;

/**
 * TeplPanelInterface:
 * @parent_interface: The parent interface.
 * @add: The virtual function pointer for tepl_panel_add().
 *   By default, does nothing.
 * @remove: The virtual function pointer for tepl_panel_remove().
 *   By default, does nothing.
 * @set_active: The virtual function pointer for tepl_panel_set_active().
 *   By default, does nothing.
 *
 * The virtual function table for #TeplPanel.
 *
 * Since: 6.8
 */
struct _TeplPanelInterface
{
	GTypeInterface parent_interface;

	void	(*add)		(TeplPanel     *panel,
				 TeplPanelItem *item);

	void	(*remove)	(TeplPanel     *panel,
				 TeplPanelItem *item);

	void	(*set_active)	(TeplPanel     *panel,
				 TeplPanelItem *item);
};

G_MODULE_EXPORT
GType	tepl_panel_get_type	(void);

G_MODULE_EXPORT
void	tepl_panel_add		(TeplPanel     *panel,
				 TeplPanelItem *item);

G_MODULE_EXPORT
void	tepl_panel_remove	(TeplPanel     *panel,
				 TeplPanelItem *item);

G_MODULE_EXPORT
void	tepl_panel_set_active	(TeplPanel     *panel,
				 TeplPanelItem *item);

G_END_DECLS

#endif /* TEPL_PANEL_H */
