// author : Nana's Effector.
// date   : 2021/09/29

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

.equ	CPSR_MODE_MASK,	0x0000001F		@ CSPR mask
.equ	CPSR_MODE_USR,	0x00000010		@ User Mode
.equ	CPSR_MODE_FIQ,	0x00000011		@ FIQ Mode
.equ	CPSR_MODE_IRQ,	0x00000012		@ IRQ Mode
.equ	CPSR_MODE_SVC,	0x00000013		@ Supervisor Mode
.equ	CPSR_MODE_ABT,	0x00000017		@ Abort Mode
.equ	CPSR_MODE_UND,	0x0000001B		@ Undefined Mode
.equ	CPSR_MODE_SYS,	0x0000001F		@ System Mode
.equ	CPSR_DIS_IRQ,	0x00000080		@ Disable IRQ
.equ	CPSR_DIS_FIQ,	0x00000040		@ Disable FIQ
.equ	EXCEPTION_VECT,	0x80000000		@ Start of exception vectors
.text
	.arm

	.global _vector
_vector:
	b _start
	
	ldr pc, _undefined_instruction
	ldr pc, _software_interrupt
	ldr pc, _prefetch_abort
	ldr pc, _data_abort
	ldr pc, _not_used
	ldr pc, _irq
	ldr pc, _fiq

_undefined_instruction:
	.word undefined_instruction
_software_interrupt:
	.word SoftwareInterrupt_Handler
_prefetch_abort:
	.word prefetch_abort
_data_abort:
	.word data_abort
_not_used:
	.word not_used
_irq:
	.word IRQ_Handler
_fiq:
	.word fiq

not_used:
	b .

_start:
	/* supervisor mode stack */
	ldr sp, _stack_srv_end					@ supervisor の stack ポインタを設定.

    @  In the reset handler, we need to copy our interrupt vector table to 0x0000, its currently at 0x10000
    LDR r0, =_vector                 @ Store the source pointer
    MOV r1, #EXCEPTION_VECT                @ Store the destination pointer.
    @  Copy the branching instructions from vectors start to registers r2-r9 and then to destination
    LDMIA r0!, {r2, r3, r4, r5, r6, r7, r8, r9}     @ Load multiple values from indexed address; auto-increment R0
    STMIA r1!, {r2, r3, r4, r5, r6, r7, r8, r9}     @ Store multiple values from the indexed address; auto-increment R1
    @  Also copy correct addresses of exception handlers
    LDMIA r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
    STMIA r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
	
	mrs r0, cpsr							@ cpsrレジスタ読み込み.
	bic r0, r0, #CPSR_MODE_MASK				@ modeビットをクリア.
	orr r0, r0, #CPSR_DIS_IRQ|CPSR_DIS_FIQ	@ irq, fiqを無効化.

	/* super visor mode irq disable. */
	orr r1, r0, #CPSR_MODE_SVC
	msr cpsr, r1
	
	/* system mode stack */
	orr r1, r0, #CPSR_MODE_SYS
	msr cpsr, r1
	ldr sp, _stack_sys_end					@ system / user の stack ポインタを設定.
	
	/* undefined mode stack */
	orr r1, r0, #CPSR_MODE_UND
	msr cpsr, r1
	ldr sp, _stack_und_end					@ undefined の stack ポインタを設定.

	/* abort mode stack */
	orr r1, r0, #CPSR_MODE_ABT
	msr cpsr, r1
	ldr sp, _stack_abt_end					@ abort の stack ポインタを設定.

	/* irq mode stack */
	orr r1, r0, #CPSR_MODE_IRQ
	msr cpsr, r1
	ldr sp, _stack_irq_end					@ irq の stack ポインタを設定.

	/* fiq mode stack */
	orr r1, r0, #CPSR_MODE_FIQ
	msr cpsr, r1
	ldr sp, _stack_fiq_end					@ fiq の stack ポインタを設定.

	/* supervisor modeで開始. */
	orr r1, r0, #CPSR_MODE_SVC
	msr cpsr, r1

	/* jump to init */
	ldr r1, =_system_init
	mov pc, r1

_system_init:
    mov r0, #0;
    mov r1, #0;
    bl system_init		@ main起動準備.
    
unhandled:
    b .					@ 無限ループ.

/*
 * Exception handlers
 */
	.align 5
undefined_instruction:
	sub lr, lr, #4
	save_regs
	bl UndefinedInstructions_Handler
	restore_regs
    b .		@ 無限ループ.

/*
	.align 5
software_interrupt:
	sub lr, lr, #4
	save_regs
	bl SoftwareInterrupt_Handler
	restore_regs
*/

	.align 5
prefetch_abort:
	sub lr, lr, #4
	save_regs
	bl PrefetchAbort_Handler
	restore_regs
    b .		@ 無限ループ.

	.align 5
data_abort:
	sub lr, lr, #8
	save_regs
	bl DataAbort_Handler
	restore_regs
    b .		@ 無限ループ.
	
/*
	.align 5
irq:
	sub lr, lr, #4
	save_regs
	bl IRQ_Handler
	restore_regs
*/

	.align 5
fiq:
	sub lr, lr, #4
	save_regs
	bl FIQ_Handler
	restore_regs
/*
 * The location of section
 */
 	.align 4
_stack_und_end:
	.long __stack_und_end
_stack_abt_end:
	.long __stack_abt_end
_stack_irq_end:
	.long __stack_irq_end
_stack_fiq_end:
	.long __stack_fiq_end
_stack_srv_end:
	.long __stack_srv_end
_stack_sys_end:
	.long __stack_sys_end
