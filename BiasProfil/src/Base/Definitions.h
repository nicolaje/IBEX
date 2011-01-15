/************************************************************************
 *
 * General Definitions
 * -------------------
 *
 * Copyright (C) 1999 Olaf Knueppel
 *
 * This file is part of PROFIL/BIAS.
 *
 * PROFIL/BIAS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 * $Id: Definitions.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#include <BIAS/BiasType.h>

#define __PROFILVERSION__ "2.0"

#define FALSE 0
#define TRUE  1

#ifdef __USECONST__
#define CONST const
#else
#define CONST
#endif

#endif /* __DEFINITIONS__ */
