/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "delay.h"
#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

/* delay.hに移動.
unsigned long millis( void );
*/

/* delay.hに移動.
unsigned long micros( void );
*/

void delayMicroseconds(unsigned int us)
{
	uint32_t end = micros() + us;

	while(end > micros());
}

void delay( unsigned long ms )
{
	uint32_t next = micros() + 1000;

	while(ms > 0)
	{
		if(next <= micros()){
			next = micros() + 1000;
			ms--;
		}
	}
}

#ifdef __cplusplus
}
#endif
