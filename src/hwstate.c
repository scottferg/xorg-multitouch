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

#include "hwstate.h"

void init_hwstate(struct HWState *s, const struct Capabilities *caps)
{
	int i;
	memset(s, 0, sizeof(struct HWState));
	for (i = 0; i < DIM_FINGER; i++)
		s->data[i].tracking_id = caps->nullid;
}

static void finish_packet(struct HWState *s, const struct Capabilities *caps,
			  const struct input_event *syn)
{
	static const mstime_t ms = 1000;
	int i;
	foreach_bit(i, s->used) {
		if (!caps->has_abs[BIT_TOUCH_MINOR])
			s->data[i].touch_minor = s->data[i].touch_major;
		if (!caps->has_abs[BIT_WIDTH_MINOR])
			s->data[i].width_minor = s->data[i].width_major;
	}
	s->evtime = syn->time.tv_usec / ms + syn->time.tv_sec * ms;
}

int hwstate_read(struct HWState *s, const struct Capabilities *caps,
		 const struct input_event *ev)
{
	switch (ev->type) {
	case EV_SYN:
		switch (ev->code) {
		case SYN_REPORT:
			finish_packet(s, caps, ev);
			return 1;
		}
		break;
	case EV_KEY:
		switch (ev->code) {
		case BTN_LEFT:
			MODBIT(s->button, MT_BUTTON_LEFT, ev->value);
			break;
		case BTN_MIDDLE:
			MODBIT(s->button, MT_BUTTON_MIDDLE, ev->value);
			break;
		case BTN_RIGHT:
			MODBIT(s->button, MT_BUTTON_RIGHT, ev->value);
			break;
		}
		break;
	case EV_ABS:
		switch (ev->code) {
		case ABS_MT_SLOT:
			if (ev->value >= 0 && ev->value < DIM_FINGER)
				s->slot = ev->value;
			break;
		case ABS_MT_TOUCH_MAJOR:
			s->data[s->slot].touch_major = ev->value;
			break;
		case ABS_MT_TOUCH_MINOR:
			s->data[s->slot].touch_minor = ev->value;
			break;
		case ABS_MT_WIDTH_MAJOR:
			s->data[s->slot].width_major = ev->value;
			break;
		case ABS_MT_WIDTH_MINOR:
			s->data[s->slot].width_minor = ev->value;
			break;
		case ABS_MT_ORIENTATION:
			s->data[s->slot].orientation = ev->value;
			break;
		case ABS_MT_PRESSURE:
			s->data[s->slot].pressure = ev->value;
			break;
		case ABS_MT_POSITION_X:
			s->data[s->slot].position_x = ev->value;
			break;
		case ABS_MT_POSITION_Y:
			s->data[s->slot].position_y = ev->value;
			break;
		case ABS_MT_TRACKING_ID:
			s->data[s->slot].tracking_id = ev->value;
			MODBIT(s->used, s->slot,
			       ev->value != caps->nullid);
			break;
		}
		break;
	}
	return 0;
}
