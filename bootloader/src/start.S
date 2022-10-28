/*
 * start.S
 *
 * Copyright(c) 2007-2021 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

.macro save_regs
	str lr, [sp, #-4]
	mrs lr, spsr_all
	str lr, [sp, #-8]
	str r1, [sp, #-12]
	str r0, [sp, #-16]
	mov r0, sp
	mrs lr, cpsr_all
	and lr, lr, #~(0x1f)
	orr lr, lr, #0x13
	msr cpsr_c, lr
	ldr r1, [r0, #-4]
	str r1, [sp, #-4]!
	ldr r1, [r0, #-8]
	str r1, [sp, #-(4 * 16)]
	ldr r1, [r0, #-12]
	ldr r0, [r0, #-16]
	stmdb sp, {r0 - r14}^
	sub sp, sp, #(4 * 16)
	ldr r4, [sp]
	and r0, r4, #0x1f
	cmp r0, #0x10
	beq 10f
	cmp r0, #0x13
	beq 11f
	b .
11:	add r1, sp, #(4 * 17)
	str r1, [sp, #(4 * 14)]
	str lr, [sp, #(4 * 15)]
10:	add r1, sp, #(4 * 17)
	str r1, [sp, #-4]!
	mov r0, sp
.endm

.macro restore_regs
	mov r12, sp
	ldr sp, [r12], #4
	ldr r1, [r12], #4
	msr spsr_cxsf, r1
	and r0, r1, #0x1f
	cmp r0, #0x10
	beq 20f
	cmp r0, #0x13
	beq 21f
	b .
20:	ldr lr, [r12, #(4 * 15)]
	ldmia r12, {r0 - r14}^
	movs pc, lr
21:	ldm r12, {r0 - r15}^
	mov r0, r0
.endm

.equ	CPSR_MODE_MASK,	0x0000001F		@ CSPR bits that define operating mode
.equ	CPSR_MODE_SVC,	0x00000013		@ Supervisor Mode
.equ	CPSR_DIS_IRQ,	0x00000080		@ Disable IRQ
.equ	CPSR_DIS_FIQ,	0x00000040		@ Disable FIQ

.text
	.arm

	.global _start
_start:
	/* Boot head information for BROM */
	.long 0xea000016
	.byte 'e', 'G', 'O', 'N', '.', 'B', 'T', '0'
	.long 0, __spl_size
	.byte 'S', 'P', 'L', 2
	.long 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0	/* 0x40 - boot params, 0x58 - fel boot type, 0x5c - dram size */

_vector:
	b reset
	ldr pc, _vect_undefined
	ldr pc, _vect_software
	ldr pc, _vect_prefetch_abort
	ldr pc, _vect_data_abort
	ldr pc, _not_used
	ldr pc, _vect_irq
	ldr pc, _vect_fiq


/*
 * The location of section
 */
_vect_undefined:
	.word __vect_undefined
_vect_software:
	.word __vect_software
_vect_prefetch_abort:
	.word __vect_prefetch_abort
_vect_data_abort:
	.word __vect_data_abort
_not_used:
	.word not_used
_vect_irq:
	.word __vect_irq
_vect_fiq:
	.word __vect_fiq

not_used:
	b .


reset:
	/* Save boot params to 0x00000040 */
	ldr r0, =0x00000040
	str sp, [r0, #0]
	str lr, [r0, #4]
	mrs lr, cpsr
	str lr, [r0, #8]
	mrc p15, 0, lr, c1, c0, 0
	str lr, [r0, #12]
	mrc p15, 0, lr, c1, c0, 0
	str lr, [r0, #16]

	/* Check boot type just for fel */
	mov r0, #0x0
	ldr r1, [r0, #8]
	ldr r2, =0x4c45462e
	cmp r1, r2
	bne 1f
	ldr r1, =0x1
	str r1, [r0, #0x58]
1:	nop

	/* Enter svc mode and mask interrupts */
	mrs r0, cpsr
	bic r0, r0, #CPSR_MODE_MASK
	orr r0, r0, #CPSR_DIS_IRQ|CPSR_DIS_FIQ|CPSR_MODE_SVC
	msr cpsr, r0

	/* Set vector to the low address */
	mrc p15, 0, r0, c1, c0, 0
	bic r0, #(1 << 13)
	mcr p15, 0, r0, c1, c0, 0

	/* Copy vector to the correct address */
	adr r0, _vector
	mrc p15, 0, r2, c1, c0, 0
	ands r2, r2, #(1 << 13)
	ldreq r1, =0x00000000
	ldrne r1, =0xffff0000
	ldmia r0!, {r2-r8, r10}
	stmia r1!, {r2-r8, r10}
	ldmia r0!, {r2-r8, r10}
	stmia r1!, {r2-r8, r10}

	/* call boot0_main. */
	ldr r1, =_main0
	mov pc, r1

_main0:
	mov r0, #1;
	mov r1, #0;
	bl boot0_main
	b _main0


