	.arch armv8-a
	.file	"LED_BlinkC.c"
	.text
	.global	PWMPIN
	.data
	.align	2
	.type	PWMPIN, %object
	.size	PWMPIN, 4
PWMPIN:
	.word	1
	.text
	.align	2
	.global	setup
	.type	setup, %function
setup:
.LFB0:
	.cfi_startproc
	stp	x29, x30, [sp, -16]!
	.cfi_def_cfa_offset 16
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	mov	x29, sp
	adrp	x0, PWMPIN
	add	x0, x0, :lo12:PWMPIN
	ldr	w0, [x0]
	mov	w1, 2
	bl	pinMode
	mov	w0, 0
	bl	pwmSetMode
	mov	w0, 1024
	bl	pwmSetRange
	mov	w0, 32
	bl	pwmSetClock
	nop
	ldp	x29, x30, [sp], 16
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE0:
	.size	setup, .-setup
	.align	2
	.global	loop
	.type	loop, %function
loop:
.LFB1:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x29, sp
	str	wzr, [sp, 28]
	b	.L3
.L4:
	adrp	x0, PWMPIN
	add	x0, x0, :lo12:PWMPIN
	ldr	w0, [x0]
	ldr	w1, [sp, 28]
	bl	pwmWrite
	mov	w0, 2
	bl	delay
	ldr	w0, [sp, 28]
	add	w0, w0, 1
	str	w0, [sp, 28]
.L3:
	ldr	w0, [sp, 28]
	cmp	w0, 1023
	ble	.L4
	mov	w0, 1023
	str	w0, [sp, 24]
	b	.L5
.L6:
	adrp	x0, PWMPIN
	add	x0, x0, :lo12:PWMPIN
	ldr	w0, [x0]
	ldr	w1, [sp, 24]
	bl	pwmWrite
	mov	w0, 2
	bl	delay
	ldr	w0, [sp, 24]
	sub	w0, w0, #1
	str	w0, [sp, 24]
.L5:
	ldr	w0, [sp, 24]
	cmp	w0, 0
	bge	.L6
	nop
	nop
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE1:
	.size	loop, .-loop
	.section	.rodata
	.align	3
.LC0:
	.string	"Setup failed"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB2:
	.cfi_startproc
	stp	x29, x30, [sp, -16]!
	.cfi_def_cfa_offset 16
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	mov	x29, sp
	bl	wiringPiSetup
	cmn	w0, #1
	bne	.L8
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	puts
	mov	w0, 1
	b	.L11
.L8:
	bl	setup
.L10:
	bl	loop
	b	.L10
.L11:
	ldp	x29, x30, [sp], 16
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
