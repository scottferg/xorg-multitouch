/***************************************************************************
 *
 * Multitouch X driver
 * Copyright (C) 2008 Henrik Rydberg <rydberg@euromail.se>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 **************************************************************************/

#ifndef MTSTATE_H
#define MTSTATE_H

#include "hwstate.h"

struct MTState {
	struct FingerState finger[DIM_FINGER];
	int nfinger;
	unsigned button;
	mstime_t evtime;
};

void init_mtstate(struct MTState *s);
void extract_mtstate(struct MTState *s,
		     const struct HWState *hs,
		     const struct Capabilities *caps);
void output_mtstate(const struct MTState *s);

const struct FingerState *find_finger(const struct MTState *s, int id);

#endif

