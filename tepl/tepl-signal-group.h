/* SPDX-FileCopyrightText: 2017-2020 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_SIGNAL_GROUP_H
#define TEPL_SIGNAL_GROUP_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <glib-object.h>
#include <gmodule.h>

G_BEGIN_DECLS

typedef struct _TeplSignalGroup TeplSignalGroup;

G_MODULE_EXPORT
TeplSignalGroup *	tepl_signal_group_new		(GObject *object);

G_MODULE_EXPORT
void			tepl_signal_group_clear		(TeplSignalGroup **group_pointer);

G_MODULE_EXPORT
void			tepl_signal_group_add		(TeplSignalGroup *group,
							 gulong           signal_handler_id);

G_END_DECLS

#endif /* TEPL_SIGNAL_GROUP_H */
