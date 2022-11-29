/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_WIDGET_LIST_CATEGORY_H
#define TEPL_WIDGET_LIST_CATEGORY_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <tepl/tepl-widget-list-item.h>

G_BEGIN_DECLS

typedef struct _TeplWidgetListCategory TeplWidgetListCategory;

/**
 * TEPL_WIDGET_LIST_CATEGORY_DEFAULT:
 *
 * A category name, used as a default.
 *
 * Usually used when only a single list is preferred.
 *
 * Since: 6.4
 */
#define TEPL_WIDGET_LIST_CATEGORY_DEFAULT "default"

/**
 * TEPL_WIDGET_LIST_CATEGORY_CORE:
 *
 * A category name for core components.
 *
 * Usually used in combination with %TEPL_WIDGET_LIST_CATEGORY_PLUGINS to
 * distinguish between the two.
 *
 * Since: 6.4
 */
#define TEPL_WIDGET_LIST_CATEGORY_CORE "core"

/**
 * TEPL_WIDGET_LIST_CATEGORY_PLUGINS:
 *
 * A category name for plugins components.
 *
 * Usually used in combination with %TEPL_WIDGET_LIST_CATEGORY_CORE to
 * distinguish between the two.
 *
 * Since: 6.4
 */
#define TEPL_WIDGET_LIST_CATEGORY_PLUGINS "plugins"

_TEPL_EXTERN
TeplWidgetListCategory *tepl_widget_list_category_new		(const gchar *name);

_TEPL_EXTERN
void			tepl_widget_list_category_free		(TeplWidgetListCategory *category);

_TEPL_EXTERN
const gchar *		tepl_widget_list_category_get_name	(const TeplWidgetListCategory *category);

_TEPL_EXTERN
void			tepl_widget_list_category_add_item	(TeplWidgetListCategory *category,
								 TeplWidgetListItem     *item);

_TEPL_EXTERN
GList *			tepl_widget_list_category_get_all	(TeplWidgetListCategory *category);

G_END_DECLS

#endif /* TEPL_WIDGET_LIST_CATEGORY_H */
