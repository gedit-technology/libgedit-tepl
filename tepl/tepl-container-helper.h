/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_CONTAINER_HELPER_H
#define TEPL_CONTAINER_HELPER_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TEPL_TYPE_CONTAINER_HELPER             (_tepl_container_helper_get_type ())
#define TEPL_CONTAINER_HELPER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_CONTAINER_HELPER, TeplContainerHelper))
#define TEPL_CONTAINER_HELPER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_CONTAINER_HELPER, TeplContainerHelperClass))
#define TEPL_IS_CONTAINER_HELPER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_CONTAINER_HELPER))
#define TEPL_IS_CONTAINER_HELPER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_CONTAINER_HELPER))
#define TEPL_CONTAINER_HELPER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_CONTAINER_HELPER, TeplContainerHelperClass))

typedef struct _TeplContainerHelper         TeplContainerHelper;
typedef struct _TeplContainerHelperClass    TeplContainerHelperClass;
typedef struct _TeplContainerHelperPrivate  TeplContainerHelperPrivate;

struct _TeplContainerHelper
{
	GObject parent;

	TeplContainerHelperPrivate *priv;
};

struct _TeplContainerHelperClass
{
	GObjectClass parent_class;
};

GType			_tepl_container_helper_get_type		(void);

TeplContainerHelper *	_tepl_container_helper_new		(GtkContainer *container);

G_END_DECLS

#endif /* TEPL_CONTAINER_HELPER_H */
