/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_OBJECT_COUNTERS_H
#define TEPL_OBJECT_COUNTERS_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gmodule.h>

G_BEGIN_DECLS

G_MODULE_EXPORT
void	tepl_object_counters_init		(void);

G_MODULE_EXPORT
void	tepl_object_counters_increment		(const gchar *type);

G_MODULE_EXPORT
void	tepl_object_counters_decrement		(const gchar *type);

G_MODULE_EXPORT
void	tepl_object_counters_final_check	(void);

G_END_DECLS

#endif /* TEPL_OBJECT_COUNTERS_H */
