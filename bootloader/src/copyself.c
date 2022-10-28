/*
 * sys-copyself.c
 *
 * Copyright(c) 2007-2018 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include <stdint.h>
#include <stdio.h>
#include "mmu.h"
#include "spiflash.h"

extern unsigned char __image_start;
extern unsigned char __image_end;
extern unsigned int __flash_load_addr;

void sys_copyself(void)
{
	void * mem;
	uint32_t size;
	uint32_t flashaddr;

	mem = (void *)&__image_start;
	size = &__image_end - &__image_start;
	flashaddr = (uint32_t)&__flash_load_addr;
	sys_mmu_init();
	
	sys_spi_flash_init();
	sys_spi_flash_read(flashaddr, mem, size);
	sys_spi_flash_exit();
}