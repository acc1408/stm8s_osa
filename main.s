; C Compiler for STM8 (COSMIC Software)
; Parser V4.12.4 - 04 Feb 2021
; Generator (Limited) V4.5.2 - 04 Feb 2021
; Optimizer V4.5.2 - 04 Feb 2021
	include	"macro.sm8"
	.info	"cpstm8 V4.12.4:"
	.info	" -h mods0.h"
	.info	" -pb"
	.info	" -u"
	.info
	.info	"cgstm8 V4.5.2:"
	.info
	.info	"costm8 V4.5.2:"
	.info
.const:	section	.text
_HSIDivFactor:
	dc.b	1
	dc.b	2
	dc.b	4
	dc.b	8
_CLKPrescTable:
	dc.b	1
	dc.b	2
	dc.b	4
	dc.b	8
	dc.b	10
	dc.b	16
	dc.b	20
	dc.b	40
	scross	off
	switch	.text
_CLK_DeInit:
	.dcall	"2,0,_CLK_DeInit"
	mov	20672,#1
	clr	20673
	mov	20676,#225
	clr	20677
	mov	20678,#24
	mov	20679,#255
	mov	20682,#255
	clr	20680
	clr	20681
L7:
	btjt	20681,#0,L7
	clr	20681
	clr	20684
	clr	20685
	ret	
	.scheck	_CLK_DeInit
_CLK_HSECmd:
	push	a
OFST:	set	0
	.dcall	"3,0,_CLK_HSECmd"
	tnz	a
	jreq	L21
	dec	a
	jreq	L21
	ldw	x,#128
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_HSECmd:_assert_failed"
	addw	sp,#4
L21:
	ld	a,(OFST+1,sp)
	jreq	L51
	bset	20673,#0
	jra	L71
L51:
	bres	20673,#0
L71:
	pop	a
	ret	
	.scheck	_CLK_HSECmd
_CLK_HSICmd:
	push	a
OFST:	set	0
	.dcall	"3,0,_CLK_HSICmd"
	tnz	a
	jreq	L42
	dec	a
	jreq	L42
	ldw	x,#152
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_HSICmd:_assert_failed"
	addw	sp,#4
L42:
	ld	a,(OFST+1,sp)
	jreq	L12
	bset	20672,#0
	jra	L32
L12:
	bres	20672,#0
L32:
	pop	a
	ret	
	.scheck	_CLK_HSICmd
_CLK_ClockSwitchCmd:
	push	a
OFST:	set	0
	.dcall	"3,0,_CLK_ClockSwitchCmd"
	tnz	a
	jreq	L63
	dec	a
	jreq	L63
	ldw	x,#228
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_ClockSwitchCmd:_assert_failed"
	addw	sp,#4
L63:
	ld	a,(OFST+1,sp)
	jreq	L52
	bset	20677,#1
	jra	L72
L52:
	bres	20677,#1
L72:
	pop	a
	ret	
	.scheck	_CLK_ClockSwitchCmd
_CLK_PeripheralClockConfig:
	pushw	x
OFST:	set	0
	.dcall	"4,0,_CLK_PeripheralClockConfig"
	ld	a,xl
	tnz	a
	jreq	L05
	ld	a,xl
	dec	a
	jreq	L05
	ldw	x,#282
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_PeripheralClockConfig:_assert_failed"
	addw	sp,#4
L05:
	ld	a,(OFST+1,sp)
	jreq	L06
	cp	a,#1
	jreq	L06
	cp	a,#3
	jreq	L06
	cp	a,#4
	jreq	L06
	cp	a,#5
	jreq	L06
	cp	a,#4
	jreq	L06
	cp	a,#6
	jreq	L06
	cp	a,#7
	jreq	L06
	cp	a,#23
	jreq	L06
	cp	a,#19
	jreq	L06
	cp	a,#18
	jreq	L06
	ldw	x,#283
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_PeripheralClockConfig:_assert_failed"
	addw	sp,#4
	ld	a,(OFST+1,sp)
L06:
	bcp	a,#16
	jrne	L13
	tnz	(OFST+2,sp)
	jreq	L33
	and	a,#15
	clrw	x
	ld	xl,a
	ld	a,#1
	tnzw	x
	jreq	L46
L66:
	sll	a
	decw	x
	jrne	L66
L46:
	or	a,20679
	jp	LC002
L33:
	and	a,#15
	clrw	x
	ld	xl,a
	ld	a,#1
	tnzw	x
	jreq	L07
L27:
	sll	a
	decw	x
	jrne	L27
L07:
	cpl	a
	and	a,20679
LC002:
	ld	20679,a
	jra	L73
L13:
	tnz	(OFST+2,sp)
	jreq	L14
	and	a,#15
	clrw	x
	ld	xl,a
	ld	a,#1
	tnzw	x
	jreq	L47
L67:
	sll	a
	decw	x
	jrne	L67
L47:
	or	a,20682
	jp	LC001
L14:
	and	a,#15
	clrw	x
	ld	xl,a
	ld	a,#1
	tnzw	x
	jreq	L001
L201:
	sll	a
	decw	x
	jrne	L201
L001:
	cpl	a
	and	a,20682
LC001:
	ld	20682,a
L73:
	popw	x
	ret	
	.scheck	_CLK_PeripheralClockConfig
_CLK_ClockSwitchConfig:
	pushw	x
	subw	sp,#4
OFST:	set	4
	.dcall	"8,2,_CLK_ClockSwitchConfig"
	ldw	x,#65535
	ldw	(OFST-1,sp),x
	ld	a,(OFST+2,sp)
	cp	a,#225
	jreq	L211
	cp	a,#210
	jreq	L211
	cp	a,#180
	jreq	L211
	ldw	x,#334
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_ClockSwitchConfig:_assert_failed"
	addw	sp,#4
L211:
	ld	a,(OFST+1,sp)
	jreq	L221
	dec	a
	jreq	L221
	ldw	x,#335
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_ClockSwitchConfig:_assert_failed"
	addw	sp,#4
L221:
	ld	a,(OFST+5,sp)
	jreq	L231
	dec	a
	jreq	L231
	ldw	x,#336
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_ClockSwitchConfig:_assert_failed"
	addw	sp,#4
L231:
	ld	a,(OFST+6,sp)
	jreq	L241
	dec	a
	jreq	L241
	ldw	x,#337
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_ClockSwitchConfig:_assert_failed"
	addw	sp,#4
L241:
	ld	a,20675
	ld	(OFST-3,sp),a
	ld	a,(OFST+1,sp)
	dec	a
	jrne	L54
	bset	20677,#1
	ld	a,(OFST+5,sp)
	jreq	L74
	bset	20677,#2
	jra	L15
L74:
	bres	20677,#2
L15:
	ld	a,(OFST+2,sp)
	ld	20676,a
	jra	L75
L35:
	decw	x
	ldw	(OFST-1,sp),x
L75:
	btjf	20677,#0,L36
	ldw	x,(OFST-1,sp)
	jrne	L35
L36:
	ldw	x,(OFST-1,sp)
	jrne	LC004
L56:
	clr	(OFST-2,sp)
L17:
	jreq	L511
	ld	a,(OFST+6,sp)
	jrne	L711
	ld	a,(OFST-3,sp)
	cp	a,#225
	jrne	L711
	bres	20672,#0
	jra	L511
L54:
	ld	a,(OFST+5,sp)
	jreq	L37
	bset	20677,#2
	jra	L57
L37:
	bres	20677,#2
L57:
	ld	a,(OFST+2,sp)
	ld	20676,a
	jra	L301
L77:
	decw	x
	ldw	(OFST-1,sp),x
L301:
	btjf	20677,#3,L701
	ldw	x,(OFST-1,sp)
	jrne	L77
L701:
	ldw	x,(OFST-1,sp)
	jreq	L56
	bset	20677,#1
LC004:
	ld	a,#1
	ld	(OFST-2,sp),a
	jra	L17
L711:
	ld	a,(OFST+6,sp)
	jrne	L321
	ld	a,(OFST-3,sp)
	cp	a,#210
	jrne	L321
	bres	20672,#3
	jra	L511
L321:
	ld	a,(OFST+6,sp)
	jrne	L511
	ld	a,(OFST-3,sp)
	cp	a,#180
	jrne	L511
	bres	20673,#0
L511:
	ld	a,(OFST-2,sp)
	addw	sp,#6
	ret	
	.scheck	_CLK_ClockSwitchConfig
_CLK_HSIPrescalerConfig:
	push	a
OFST:	set	0
	.dcall	"3,0,_CLK_HSIPrescalerConfig"
	tnz	a
	jreq	L451
	cp	a,#8
	jreq	L451
	cp	a,#16
	jreq	L451
	cp	a,#24
	jreq	L451
	ldw	x,#438
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_HSIPrescalerConfig:_assert_failed"
	addw	sp,#4
L451:
	ld	a,20678
	and	a,#231
	ld	20678,a
	ld	a,20678
	or	a,(OFST+1,sp)
	ld	20678,a
	pop	a
	ret	
	.scheck	_CLK_HSIPrescalerConfig
_CLK_ITConfig:
	pushw	x
OFST:	set	0
	.dcall	"4,0,_CLK_ITConfig"
	ld	a,xl
	tnz	a
	jreq	L661
	ld	a,xl
	dec	a
	jreq	L661
	ldw	x,#486
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_ITConfig:_assert_failed"
	addw	sp,#4
L661:
	ld	a,(OFST+1,sp)
	cp	a,#12
	jreq	L671
	cp	a,#28
	jreq	L671
	ldw	x,#487
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_ITConfig:_assert_failed"
	addw	sp,#4
L671:
	ld	a,(OFST+2,sp)
	jreq	L541
	ld	a,(OFST+1,sp)
	sub	a,#12
	jreq	L331
	sub	a,#16
	jrne	L351
	bset	20677,#2
	jra	L351
L331:
	bset	20680,#2
	jra	L351
L541:
	ld	a,(OFST+1,sp)
	sub	a,#12
	jreq	L141
	sub	a,#16
	jrne	L351
	bres	20677,#2
	jra	L351
L141:
	bres	20680,#2
L351:
	popw	x
	ret	
	.scheck	_CLK_ITConfig
_CLK_SYSCLKConfig:
	push	a
OFST:	set	0
	.dcall	"3,0,_CLK_SYSCLKConfig"
	tnz	a
	jreq	L012
	cp	a,#8
	jreq	L012
	cp	a,#16
	jreq	L012
	cp	a,#24
	jreq	L012
	cp	a,#128
	jreq	L012
	cp	a,#129
	jreq	L012
	cp	a,#130
	jreq	L012
	cp	a,#131
	jreq	L012
	cp	a,#132
	jreq	L012
	cp	a,#133
	jreq	L012
	cp	a,#134
	jreq	L012
	cp	a,#135
	jreq	L012
	ldw	x,#529
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_SYSCLKConfig:_assert_failed"
	addw	sp,#4
L012:
	ld	a,(OFST+1,sp)
	jrmi	L161
	ld	a,20678
	and	a,#231
	ld	20678,a
	ld	a,(OFST+1,sp)
	and	a,#24
	jra	L361
L161:
	ld	a,20678
	and	a,#248
	ld	20678,a
	ld	a,(OFST+1,sp)
	and	a,#7
L361:
	or	a,20678
	ld	20678,a
	pop	a
	ret	
	.scheck	_CLK_SYSCLKConfig
_CLK_ClockSecuritySystemEnable:
	.dcall	"2,0,_CLK_ClockSecuritySystemEnable"
	bset	20680,#0
	ret	
	.scheck	_CLK_ClockSecuritySystemEnable
_CLK_GetSYSCLKSource:
	.dcall	"2,0,_CLK_GetSYSCLKSource"
	ld	a,20675
	ret	
	.scheck	_CLK_GetSYSCLKSource
_CLK_GetClockFreq:
	subw	sp,#9
OFST:	set	9
	.dcall	"11,0,1,_CLK_GetClockFreq"
	ld	a,20675
	ld	(OFST+0,sp),a
	cp	a,#225
	jrne	L561
	ld	a,20678
	and	a,#24
	srl	a
	srl	a
	srl	a
	clrw	x
	ld	xl,a
	ld	a,(_HSIDivFactor,x)
	ld	(OFST+0,sp),a
	ld	c_lreg+3,a
	clr	c_lreg+2
	clr	c_lreg+1
	clr	c_lreg
	ldw	x,sp
	incw	x
	call	c_rtol
	.dcall	"_CLK_GetClockFreq:c_rtol"
	ldw	x,#9216
	ldw	c_lreg+2,x
	ldw	x,#244
	ldw	c_lreg,x
	ldw	x,sp
	incw	x
	call	c_ludv
	.dcall	"_CLK_GetClockFreq:c_ludv"
	ldw	x,sp
	addw	x,#OFST-4
	call	c_rtol
	.dcall	"_CLK_GetClockFreq:c_rtol"
	jra	L761
L561:
	cp	a,#210
	jrne	L171
	ldw	x,#62464
	ldw	(OFST-2,sp),x
	ldw	x,#1
	jp	LC005
L171:
	ldw	x,#4608
	ldw	(OFST-2,sp),x
	ldw	x,#122
LC005:
	ldw	(OFST-4,sp),x
L761:
	ldw	x,sp
	addw	x,#OFST-4
	call	c_ltor
	.dcall	"_CLK_GetClockFreq:c_ltor"
	addw	sp,#9
	ret	
	.scheck	_CLK_GetClockFreq
_CLK_GetCPUClk:
	push	a
OFST:	set	1
	.dcall	"3,0,_CLK_GetCPUClk"
	ld	a,20678
	ld	(OFST+0,sp),a
	call	_CLK_GetClockFreq
	.dcall	"_CLK_GetCPUClk:_CLK_GetClockFreq"
	ld	a,(OFST+0,sp)
	and	a,#7
	call	c_lursh
	.dcall	"_CLK_GetCPUClk:c_lursh"
	pop	a
	ret	
	.scheck	_CLK_GetCPUClk
_CLK_SYSCLKEmergencyClear:
	.dcall	"2,0,_CLK_SYSCLKEmergencyClear"
	bres	20677,#0
	ret	
	.scheck	_CLK_SYSCLKEmergencyClear
_CLK_GetFlagStatus:
	pushw	x
	subw	sp,#3
OFST:	set	3
	.dcall	"7,0,_CLK_GetFlagStatus"
	cpw	x,#272
	jreq	L632
	cpw	x,#258
	jreq	L632
	cpw	x,#514
	jreq	L632
	cpw	x,#776
	jreq	L632
	cpw	x,#769
	jreq	L632
	cpw	x,#1032
	jreq	L632
	cpw	x,#1026
	jreq	L632
	cpw	x,#1284
	jreq	L632
	cpw	x,#1282
	jreq	L632
	ldw	x,#691
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_GetFlagStatus:_assert_failed"
	addw	sp,#4
L632:
	ld	a,(OFST+1,sp)
	ld	xl,a
	clr	a
	rlwa	x,a
	ldw	(OFST-2,sp),x
	cpw	x,#256
	jrne	L571
	ld	a,20672
	jra	L771
L571:
	cpw	x,#512
	jrne	L102
	ld	a,20673
	jra	L771
L102:
	cpw	x,#768
	jrne	L502
	ld	a,20677
	jra	L771
L502:
	cpw	x,#1024
	jrne	L112
	ld	a,20680
	jra	L771
L112:
	ld	a,20681
L771:
	ld	(OFST+0,sp),a
	ld	a,(OFST+2,sp)
	bcp	a,(OFST+0,sp)
	jreq	L512
	ld	a,#1
	jra	L712
L512:
	clr	a
L712:
	addw	sp,#5
	ret	
	.scheck	_CLK_GetFlagStatus
_CLK_GetITStatus:
	push	a
	push	a
OFST:	set	1
	.dcall	"4,0,_CLK_GetITStatus"
	cp	a,#12
	jreq	L052
	cp	a,#28
	jreq	L052
	ldw	x,#744
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_GetITStatus:_assert_failed"
	addw	sp,#4
L052:
	ld	a,(OFST+1,sp)
	cp	a,#28
	jrne	L122
	ld	a,20677
	and	a,(OFST+1,sp)
	cp	a,#12
	jrne	L132
	jp	LC007
L122:
	ld	a,20680
	and	a,(OFST+1,sp)
	cp	a,#12
	jrne	L132
LC007:
	ld	a,#1
	jra	L722
L132:
	clr	a
L722:
	popw	x
	ret	
	.scheck	_CLK_GetITStatus
_CLK_ClearITPendingBit:
	push	a
OFST:	set	0
	.dcall	"3,0,_CLK_ClearITPendingBit"
	cp	a,#12
	jreq	L262
	cp	a,#28
	jreq	L262
	ldw	x,#786
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L31
	call	_assert_failed
	.dcall	"_CLK_ClearITPendingBit:_assert_failed"
	addw	sp,#4
L262:
	ld	a,(OFST+1,sp)
	cp	a,#12
	jrne	L532
	bres	20680,#3
	jra	L732
L532:
	bres	20677,#3
L732:
	pop	a
	ret	
	.scheck	_CLK_ClearITPendingBit
_GPIO_DeInit:
	.dcall	"2,0,_GPIO_DeInit"
	clr	(x)
	clr	(2,x)
	clr	(3,x)
	clr	(4,x)
	ret	
	.scheck	_GPIO_DeInit
_GPIO_Init:
	pushw	x
OFST:	set	0
	.dcall	"4,2,_GPIO_Init"
	ld	a,(OFST+6,sp)
	jreq	L672
	cp	a,#64
	jreq	L672
	cp	a,#32
	jreq	L672
	cp	a,#96
	jreq	L672
	cp	a,#160
	jreq	L672
	cp	a,#224
	jreq	L672
	cp	a,#128
	jreq	L672
	cp	a,#192
	jreq	L672
	cp	a,#176
	jreq	L672
	cp	a,#240
	jreq	L672
	cp	a,#144
	jreq	L672
	cp	a,#208
	jreq	L672
	ldw	x,#77
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L142
	call	_assert_failed
	.dcall	"_GPIO_Init:_assert_failed"
	addw	sp,#4
L672:
	ld	a,(OFST+5,sp)
	jrne	L403
	ldw	x,#78
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L142
	call	_assert_failed
	.dcall	"_GPIO_Init:_assert_failed"
	addw	sp,#4
	ld	a,(OFST+5,sp)
L403:
	ldw	x,(OFST+1,sp)
	cpl	a
	and	a,(4,x)
	ld	(4,x),a
	ld	a,(OFST+6,sp)
	jrpl	L342
	bcp	a,#16
	jreq	L542
	ld	a,(x)
	or	a,(OFST+5,sp)
	jra	L742
L542:
	ld	a,(OFST+5,sp)
	cpl	a
	and	a,(x)
L742:
	ld	(x),a
	ld	a,(2,x)
	or	a,(OFST+5,sp)
	jra	L152
L342:
	ld	a,(OFST+5,sp)
	cpl	a
	and	a,(2,x)
L152:
	ld	(2,x),a
	ld	a,(OFST+6,sp)
	bcp	a,#64
	jreq	L352
	ld	a,(3,x)
	or	a,(OFST+5,sp)
	jra	L552
L352:
	ld	a,(OFST+5,sp)
	cpl	a
	and	a,(3,x)
L552:
	ld	(3,x),a
	ld	a,(OFST+6,sp)
	bcp	a,#32
	jreq	L752
	ldw	x,(OFST+1,sp)
	ld	a,(4,x)
	or	a,(OFST+5,sp)
	jra	L162
L752:
	ldw	x,(OFST+1,sp)
	ld	a,(OFST+5,sp)
	cpl	a
	and	a,(4,x)
L162:
	ld	(4,x),a
	popw	x
	ret	
	.scheck	_GPIO_Init
_GPIO_Write:
OFST: set -2
	.dcall	"4,1,_GPIO_Write"
	ld	a,(OFST+5,sp)
	ld	(x),a
	ret	
	.scheck	_GPIO_Write
_GPIO_WriteHigh:
OFST: set -2
	.dcall	"4,1,_GPIO_WriteHigh"
	ld	a,(x)
	or	a,(OFST+5,sp)
	ld	(x),a
	ret	
	.scheck	_GPIO_WriteHigh
_GPIO_WriteLow:
OFST: set -2
	.dcall	"4,1,_GPIO_WriteLow"
	ld	a,(OFST+5,sp)
	cpl	a
	and	a,(x)
	ld	(x),a
	ret	
	.scheck	_GPIO_WriteLow
_GPIO_WriteReverse:
OFST: set -2
	.dcall	"4,1,_GPIO_WriteReverse"
	ld	a,(x)
	xor	a,(OFST+5,sp)
	ld	(x),a
	ret	
	.scheck	_GPIO_WriteReverse
_GPIO_ReadOutputData:
	.dcall	"2,0,_GPIO_ReadOutputData"
	ld	a,(x)
	ret	
	.scheck	_GPIO_ReadOutputData
_GPIO_ReadInputData:
	.dcall	"2,0,_GPIO_ReadInputData"
	ld	a,(1,x)
	ret	
	.scheck	_GPIO_ReadInputData
_GPIO_ReadInputPin:
OFST: set -2
	.dcall	"4,1,_GPIO_ReadInputPin"
	ld	a,(1,x)
	and	a,(OFST+5,sp)
	clrw	x
	ld	xl,a
	ld	a,(OFST+5,sp)
	div	x,a
	ld	a,xl
	ret	
	.scheck	_GPIO_ReadInputPin
_GPIO_ExternalPullUpConfig:
	pushw	x
OFST:	set	0
	.dcall	"4,2,_GPIO_ExternalPullUpConfig"
	ld	a,(OFST+5,sp)
	jrne	L233
	ldw	x,#228
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L142
	call	_assert_failed
	.dcall	"_GPIO_ExternalPullUpConfig:_assert_failed"
	addw	sp,#4
L233:
	ld	a,(OFST+6,sp)
	jreq	L243
	dec	a
	jreq	L243
	ldw	x,#229
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L142
	call	_assert_failed
	.dcall	"_GPIO_ExternalPullUpConfig:_assert_failed"
	addw	sp,#4
L243:
	ld	a,(OFST+6,sp)
	jreq	L362
	ldw	x,(OFST+1,sp)
	ld	a,(3,x)
	or	a,(OFST+5,sp)
	jra	L562
L362:
	ldw	x,(OFST+1,sp)
	ld	a,(OFST+5,sp)
	cpl	a
	and	a,(3,x)
L562:
	ld	(3,x),a
	popw	x
	ret	
	.scheck	_GPIO_ExternalPullUpConfig
_TIM4_TimeBaseInit:
	pushw	x
OFST:	set	0
	.dcall	"4,0,_TIM4_TimeBaseInit"
	ld	a,xh
	tnz	a
	jreq	L453
	ld	a,xh
	dec	a
	jreq	L453
	ld	a,xh
	cp	a,#2
	jreq	L453
	ld	a,xh
	cp	a,#3
	jreq	L453
	ld	a,xh
	cp	a,#4
	jreq	L453
	ld	a,xh
	cp	a,#5
	jreq	L453
	ld	a,xh
	cp	a,#6
	jreq	L453
	ld	a,xh
	cp	a,#7
	jreq	L453
	ldw	x,#70
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L762
	call	_assert_failed
	.dcall	"_TIM4_TimeBaseInit:_assert_failed"
	addw	sp,#4
L453:
	ld	a,(OFST+1,sp)
	ld	21317,a
	ld	a,(OFST+2,sp)
	ld	21318,a
	popw	x
	ret	
	.scheck	_TIM4_TimeBaseInit
_TIM4_Cmd:
	push	a
OFST:	set	0
	.dcall	"3,0,_TIM4_Cmd"
	tnz	a
	jreq	L663
	dec	a
	jreq	L663
	ldw	x,#88
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L762
	call	_assert_failed
	.dcall	"_TIM4_Cmd:_assert_failed"
	addw	sp,#4
L663:
	ld	a,(OFST+1,sp)
	jreq	L172
	bset	21312,#0
	jra	L372
L172:
	bres	21312,#0
L372:
	pop	a
	ret	
	.scheck	_TIM4_Cmd
_TIM4_ITConfig:
	pushw	x
OFST:	set	0
	.dcall	"4,0,_TIM4_ITConfig"
	ld	a,xh
	dec	a
	jreq	L673
	ldw	x,#116
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L762
	call	_assert_failed
	.dcall	"_TIM4_ITConfig:_assert_failed"
	addw	sp,#4
L673:
	ld	a,(OFST+2,sp)
	jreq	L604
	dec	a
	jreq	L604
	ldw	x,#117
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L762
	call	_assert_failed
	.dcall	"_TIM4_ITConfig:_assert_failed"
	addw	sp,#4
L604:
	ld	a,(OFST+2,sp)
	jreq	L572
	ld	a,21313
	or	a,(OFST+1,sp)
	jra	L772
L572:
	ld	a,(OFST+1,sp)
	cpl	a
	and	a,21313
L772:
	ld	21313,a
	popw	x
	ret	
	.scheck	_TIM4_ITConfig
_TIM4_UpdateDisableConfig:
	push	a
OFST:	set	0
	.dcall	"3,0,_TIM4_UpdateDisableConfig"
	tnz	a
	jreq	L024
	dec	a
	jreq	L024
	ldw	x,#142
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L762
	call	_assert_failed
	.dcall	"_TIM4_UpdateDisableConfig:_assert_failed"
	addw	sp,#4
L024:
	ld	a,(OFST+1,sp)
	jreq	L103
	bset	21312,#1
	jra	L303
L103:
	bres	21312,#1
L303:
	pop	a
	ret	
	.scheck	_TIM4_UpdateDisableConfig
_TIM4_UpdateRequestConfig:
	push	a
OFST:	set	0
	.dcall	"3,0,_TIM4_UpdateRequestConfig"
	tnz	a
	jreq	L234
	dec	a
	jreq	L234
	ldw	x,#168
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L762
	call	_assert_failed
	.dcall	"_TIM4_UpdateRequestConfig:_assert_failed"
	addw	sp,#4
L234:
	ld	a,(OFST+1,sp)
	jreq	L503
	bset	21312,#2
	jra	L703
L503:
	bres	21312,#2
L703:
	pop	a
	ret	
	.scheck	_TIM4_UpdateRequestConfig
_TIM4_ARRPreloadConfig:
	push	a
OFST:	set	0
	.dcall	"3,0,_TIM4_ARRPreloadConfig"
	tnz	a
	jreq	L444
	dec	a
	jreq	L444
	ldw	x,#253
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L762
	call	_assert_failed
	.dcall	"_TIM4_ARRPreloadConfig:_assert_failed"
	addw	sp,#4
L444:
	ld	a,(OFST+1,sp)
	jreq	L113
	bset	21312,#7
	jra	L313
L113:
	bres	21312,#7
L313:
	pop	a
	ret	
	.scheck	_TIM4_ARRPreloadConfig
_TIM4_SetCounter:
	.dcall	"2,0,_TIM4_SetCounter"
	ld	21316,a
	ret	
	.scheck	_TIM4_SetCounter
_TIM4_SetAutoreload:
	.dcall	"2,0,_TIM4_SetAutoreload"
	ld	21318,a
	ret	
	.scheck	_TIM4_SetAutoreload
_TIM4_ClearFlag:
	push	a
OFST:	set	0
	.dcall	"3,0,_TIM4_ClearFlag"
	dec	a
	jreq	L064
	ldw	x,#377
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L762
	call	_assert_failed
	.dcall	"_TIM4_ClearFlag:_assert_failed"
	addw	sp,#4
L064:
	ld	a,(OFST+1,sp)
	cpl	a
	ld	21314,a
	pop	a
	ret	
	.scheck	_TIM4_ClearFlag
_TIM4_GetITStatus:
	push	a
	pushw	x
OFST:	set	2
	.dcall	"5,0,_TIM4_GetITStatus"
	dec	a
	jreq	L074
	ldw	x,#399
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L762
	call	_assert_failed
	.dcall	"_TIM4_GetITStatus:_assert_failed"
	addw	sp,#4
L074:
	ld	a,21314
	and	a,(OFST+1,sp)
	ld	(OFST-1,sp),a
	ld	a,21313
	and	a,(OFST+1,sp)
	ld	(OFST+0,sp),a
	ld	a,(OFST-1,sp)
	jreq	L513
	ld	a,(OFST+0,sp)
	jreq	L513
	ld	a,#1
	jra	L713
L513:
	clr	a
L713:
	addw	sp,#3
	ret	
	.scheck	_TIM4_GetITStatus
_TIM4_ClearITPendingBit:
	push	a
OFST:	set	0
	.dcall	"3,0,_TIM4_ClearITPendingBit"
	dec	a
	jreq	L005
	ldw	x,#428
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L762
	call	_assert_failed
	.dcall	"_TIM4_ClearITPendingBit:_assert_failed"
	addw	sp,#4
L005:
	ld	a,(OFST+1,sp)
	cpl	a
	ld	21314,a
	pop	a
	ret	
	.scheck	_TIM4_ClearITPendingBit
	bsct
L123_UART2_RxFunc:
	dc.w	0
	switch	.text
_UART2_DeInit:
	.dcall	"2,0,_UART2_DeInit"
	ld	a,21056
	ld	a,21057
	clr	21059
	clr	21058
	clr	21060
	clr	21061
	clr	21062
	clr	21063
	clr	21064
	clr	21065
	ret	
	.scheck	_UART2_DeInit
	switch	.const
L215:
	dc.l	625001
L275:
	dc.l	100
	switch	.text
_UART2_Init:
	subw	sp,#14
OFST:	set	14
	.dcall	"16,9,1,_UART2_Init"
	ldw	x,sp
	addw	x,#OFST+3
	call	c_ltor
	.dcall	"_UART2_Init:c_ltor"
	ldw	x,#L215
	call	c_lcmp
	.dcall	"_UART2_Init:c_lcmp"
	jrult	L415
	ldw	x,#95
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_Init:_assert_failed"
	addw	sp,#4
L415:
	ld	a,(OFST+7,sp)
	jreq	L425
	cp	a,#16
	jreq	L425
	ldw	x,#96
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_Init:_assert_failed"
	addw	sp,#4
L425:
	ld	a,(OFST+8,sp)
	jreq	L435
	cp	a,#16
	jreq	L435
	cp	a,#32
	jreq	L435
	cp	a,#48
	jreq	L435
	ldw	x,#97
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_Init:_assert_failed"
	addw	sp,#4
L435:
	ld	a,(OFST+9,sp)
	jreq	L445
	cp	a,#4
	jreq	L445
	cp	a,#6
	jreq	L445
	ldw	x,#98
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_Init:_assert_failed"
	addw	sp,#4
L445:
	ld	a,(OFST+11,sp)
	cp	a,#8
	jreq	L455
	cp	a,#64
	jreq	L455
	cp	a,#4
	jreq	L455
	cp	a,#128
	jreq	L455
	cp	a,#12
	jreq	L455
	cp	a,#68
	jreq	L455
	cp	a,#192
	jreq	L455
	cp	a,#136
	jreq	L455
	ldw	x,#99
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_Init:_assert_failed"
	addw	sp,#4
L455:
	ld	a,(OFST+10,sp)
	and	a,#136
	cp	a,#136
	jreq	L065
	ld	a,(OFST+10,sp)
	and	a,#68
	cp	a,#68
	jreq	L065
	ld	a,(OFST+10,sp)
	and	a,#34
	cp	a,#34
	jreq	L065
	ld	a,(OFST+10,sp)
	and	a,#17
	cp	a,#17
	jrne	L265
L065:
	ldw	x,#100
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_Init:_assert_failed"
	addw	sp,#4
L265:
	bres	21060,#4
	ld	a,21060
	or	a,(OFST+7,sp)
	ld	21060,a
	ld	a,21062
	and	a,#207
	ld	21062,a
	ld	a,21062
	or	a,(OFST+8,sp)
	ld	21062,a
	ld	a,21060
	and	a,#249
	ld	21060,a
	ld	a,21060
	or	a,(OFST+9,sp)
	ld	21060,a
	clr	21058
	ld	a,21059
	and	a,#15
	ld	21059,a
	ld	a,21059
	and	a,#240
	ld	21059,a
	ldw	x,sp
	addw	x,#OFST+3
	call	c_ltor
	.dcall	"_UART2_Init:c_ltor"
	ld	a,#4
	call	c_llsh
	.dcall	"_UART2_Init:c_llsh"
	ldw	x,sp
	incw	x
	call	c_rtol
	.dcall	"_UART2_Init:c_rtol"
	call	_CLK_GetClockFreq
	.dcall	"_UART2_Init:_CLK_GetClockFreq"
	ldw	x,sp
	incw	x
	call	c_ludv
	.dcall	"_UART2_Init:c_ludv"
	ldw	x,sp
	addw	x,#OFST-3
	call	c_rtol
	.dcall	"_UART2_Init:c_rtol"
	ldw	x,sp
	addw	x,#OFST+3
	call	c_ltor
	.dcall	"_UART2_Init:c_ltor"
	ld	a,#4
	call	c_llsh
	.dcall	"_UART2_Init:c_llsh"
	ldw	x,sp
	incw	x
	call	c_rtol
	.dcall	"_UART2_Init:c_rtol"
	call	_CLK_GetClockFreq
	.dcall	"_UART2_Init:_CLK_GetClockFreq"
	ld	a,#100
	call	c_smul
	.dcall	"_UART2_Init:c_smul"
	ldw	x,sp
	incw	x
	call	c_ludv
	.dcall	"_UART2_Init:c_ludv"
	ldw	x,sp
	addw	x,#OFST-7
	call	c_rtol
	.dcall	"_UART2_Init:c_rtol"
	ldw	x,sp
	addw	x,#OFST-3
	call	c_ltor
	.dcall	"_UART2_Init:c_ltor"
	ld	a,#100
	call	c_smul
	.dcall	"_UART2_Init:c_smul"
	ldw	x,sp
	incw	x
	call	c_rtol
	.dcall	"_UART2_Init:c_rtol"
	ldw	x,sp
	addw	x,#OFST-7
	call	c_ltor
	.dcall	"_UART2_Init:c_ltor"
	ldw	x,sp
	incw	x
	call	c_lsub
	.dcall	"_UART2_Init:c_lsub"
	ld	a,#4
	call	c_llsh
	.dcall	"_UART2_Init:c_llsh"
	ldw	x,#L275
	call	c_ludv
	.dcall	"_UART2_Init:c_ludv"
	ld	a,c_lreg+3
	and	a,#15
	ld	(OFST-9,sp),a
	ldw	x,(OFST-1,sp)
	srlw	x
	srlw	x
	srlw	x
	srlw	x
	rrwa	x,a
	and	a,#240
	ld	(OFST-8,sp),a
	or	a,(OFST-9,sp)
	ld	21059,a
	ld	a,(OFST+0,sp)
	ld	21058,a
	ld	a,21061
	and	a,#243
	ld	21061,a
	ld	a,21062
	and	a,#248
	ld	21062,a
	ld	a,(OFST+10,sp)
	and	a,#7
	or	a,21062
	ld	21062,a
	ld	a,(OFST+11,sp)
	bcp	a,#4
	jreq	L523
	bset	21061,#3
	jra	L723
L523:
	bres	21061,#3
L723:
	bcp	a,#8
	jreq	L133
	bset	21061,#2
	jra	L333
L133:
	bres	21061,#2
L333:
	ld	a,(OFST+10,sp)
	jrpl	L533
	bres	21062,#3
	jra	L733
L533:
	and	a,#8
	or	a,21062
	ld	21062,a
L733:
	addw	sp,#14
	ret	
	.scheck	_UART2_Init
_UART2_Cmd:
	.dcall	"2,0,_UART2_Cmd"
	tnz	a
	jreq	L143
	bres	21060,#5
	ret	
L143:
	bset	21060,#5
	ret	
	.scheck	_UART2_Cmd
_UART2_ITConfig:
	pushw	x
	pushw	x
OFST:	set	2
	.dcall	"6,1,_UART2_ITConfig"
	cpw	x,#256
	jreq	L406
	cpw	x,#631
	jreq	L406
	cpw	x,#614
	jreq	L406
	cpw	x,#517
	jreq	L406
	cpw	x,#580
	jreq	L406
	cpw	x,#1042
	jreq	L406
	cpw	x,#838
	jreq	L406
	ldw	x,#219
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_ITConfig:_assert_failed"
	addw	sp,#4
L406:
	ld	a,(OFST+5,sp)
	jreq	L416
	dec	a
	jreq	L416
	ldw	x,#220
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_ITConfig:_assert_failed"
	addw	sp,#4
L416:
	ld	a,(OFST+1,sp)
	ld	(OFST-1,sp),a
	ld	a,(OFST+2,sp)
	and	a,#15
	clrw	x
	ld	xl,a
	ld	a,#1
	tnzw	x
	jreq	L026
L226:
	sll	a
	decw	x
	jrne	L226
L026:
	ld	(OFST+0,sp),a
	ld	a,(OFST+5,sp)
	jreq	L543
	ld	a,(OFST-1,sp)
	cp	a,#1
	jrne	L743
	ld	a,21060
	or	a,(OFST+0,sp)
	jp	LC010
L743:
	cp	a,#2
	jrne	L353
	ld	a,21061
	or	a,(OFST+0,sp)
	jp	LC009
L353:
	cp	a,#3
	jrne	L753
	ld	a,21063
	or	a,(OFST+0,sp)
	jp	LC011
L753:
	ld	a,21065
	or	a,(OFST+0,sp)
	jp	LC008
L543:
	ld	a,(OFST-1,sp)
	cp	a,#1
	jrne	L563
	ld	a,(OFST+0,sp)
	cpl	a
	and	a,21060
LC010:
	ld	21060,a
	jra	L363
L563:
	cp	a,#2
	jrne	L173
	ld	a,(OFST+0,sp)
	cpl	a
	and	a,21061
LC009:
	ld	21061,a
	jra	L363
L173:
	cp	a,#3
	jrne	L573
	ld	a,(OFST+0,sp)
	cpl	a
	and	a,21063
LC011:
	ld	21063,a
	jra	L363
L573:
	ld	a,(OFST+0,sp)
	cpl	a
	and	a,21065
LC008:
	ld	21065,a
L363:
	addw	sp,#4
	ret	
	.scheck	_UART2_ITConfig
_UART2_IrDAConfig:
	push	a
OFST:	set	0
	.dcall	"3,0,_UART2_IrDAConfig"
	cp	a,#1
	jreq	L236
	tnz	a
	jreq	L236
	ldw	x,#278
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_IrDAConfig:_assert_failed"
	addw	sp,#4
L236:
	ld	a,(OFST+1,sp)
	jreq	L104
	bset	21064,#2
	jra	L304
L104:
	bres	21064,#2
L304:
	pop	a
	ret	
	.scheck	_UART2_IrDAConfig
_UART2_IrDACmd:
	push	a
OFST:	set	0
	.dcall	"3,0,_UART2_IrDACmd"
	tnz	a
	jreq	L446
	dec	a
	jreq	L446
	ldw	x,#299
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_IrDACmd:_assert_failed"
	addw	sp,#4
L446:
	ld	a,(OFST+1,sp)
	jreq	L504
	bset	21064,#1
	jra	L704
L504:
	bres	21064,#1
L704:
	pop	a
	ret	
	.scheck	_UART2_IrDACmd
_UART2_LINBreakDetectionConfig:
	push	a
OFST:	set	0
	.dcall	"3,0,_UART2_LINBreakDetectionConfig"
	tnz	a
	jreq	L656
	dec	a
	jreq	L656
	ldw	x,#323
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_LINBreakDetectionConfig:_assert_failed"
	addw	sp,#4
L656:
	ld	a,(OFST+1,sp)
	jreq	L114
	bset	21063,#5
	jra	L314
L114:
	bres	21063,#5
L314:
	pop	a
	ret	
	.scheck	_UART2_LINBreakDetectionConfig
_UART2_LINConfig:
	pushw	x
OFST:	set	0
	.dcall	"4,1,_UART2_LINConfig"
	ld	a,xh
	tnz	a
	jreq	L076
	ld	a,xh
	dec	a
	jreq	L076
	ldw	x,#350
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_LINConfig:_assert_failed"
	addw	sp,#4
L076:
	ld	a,(OFST+2,sp)
	dec	a
	jreq	L007
	ld	a,(OFST+2,sp)
	jreq	L007
	ldw	x,#351
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_LINConfig:_assert_failed"
	addw	sp,#4
L007:
	ld	a,(OFST+5,sp)
	jreq	L017
	dec	a
	jreq	L017
	ldw	x,#352
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_LINConfig:_assert_failed"
	addw	sp,#4
L017:
	ld	a,(OFST+1,sp)
	jreq	L514
	bset	21065,#5
	jra	L714
L514:
	bres	21065,#5
L714:
	ld	a,(OFST+2,sp)
	jreq	L124
	bset	21065,#4
	jra	L324
L124:
	bres	21065,#4
L324:
	ld	a,(OFST+5,sp)
	jreq	L524
	bset	21065,#7
	jra	L724
L524:
	bres	21065,#7
L724:
	popw	x
	ret	
	.scheck	_UART2_LINConfig
_UART2_LINCmd:
	push	a
OFST:	set	0
	.dcall	"3,0,_UART2_LINCmd"
	tnz	a
	jreq	L227
	dec	a
	jreq	L227
	ldw	x,#390
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_LINCmd:_assert_failed"
	addw	sp,#4
L227:
	ld	a,(OFST+1,sp)
	jreq	L134
	bset	21062,#6
	jra	L334
L134:
	bres	21062,#6
L334:
	pop	a
	ret	
	.scheck	_UART2_LINCmd
_UART2_SmartCardCmd:
	push	a
OFST:	set	0
	.dcall	"3,0,_UART2_SmartCardCmd"
	tnz	a
	jreq	L437
	dec	a
	jreq	L437
	ldw	x,#413
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_SmartCardCmd:_assert_failed"
	addw	sp,#4
L437:
	ld	a,(OFST+1,sp)
	jreq	L534
	bset	21064,#5
	jra	L734
L534:
	bres	21064,#5
L734:
	pop	a
	ret	
	.scheck	_UART2_SmartCardCmd
_UART2_SmartCardNACKCmd:
	push	a
OFST:	set	0
	.dcall	"3,0,_UART2_SmartCardNACKCmd"
	tnz	a
	jreq	L647
	dec	a
	jreq	L647
	ldw	x,#436
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_SmartCardNACKCmd:_assert_failed"
	addw	sp,#4
L647:
	ld	a,(OFST+1,sp)
	jreq	L144
	bset	21064,#4
	jra	L344
L144:
	bres	21064,#4
L344:
	pop	a
	ret	
	.scheck	_UART2_SmartCardNACKCmd
_UART2_WakeUpConfig:
	push	a
OFST:	set	0
	.dcall	"3,0,_UART2_WakeUpConfig"
	tnz	a
	jreq	L067
	cp	a,#8
	jreq	L067
	ldw	x,#458
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_WakeUpConfig:_assert_failed"
	addw	sp,#4
L067:
	bres	21060,#3
	ld	a,21060
	or	a,(OFST+1,sp)
	ld	21060,a
	pop	a
	ret	
	.scheck	_UART2_WakeUpConfig
_UART2_ReceiverWakeUpCmd:
	push	a
OFST:	set	0
	.dcall	"3,0,_UART2_ReceiverWakeUpCmd"
	tnz	a
	jreq	L277
	dec	a
	jreq	L277
	ldw	x,#472
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_ReceiverWakeUpCmd:_assert_failed"
	addw	sp,#4
L277:
	ld	a,(OFST+1,sp)
	jreq	L544
	bset	21061,#1
	jra	L744
L544:
	bres	21061,#1
L744:
	pop	a
	ret	
	.scheck	_UART2_ReceiverWakeUpCmd
_UART2_ReceiveData8:
	.dcall	"2,0,_UART2_ReceiveData8"
	ld	a,21057
	ret	
	.scheck	_UART2_ReceiveData8
_UART2_ReceiveData9:
	pushw	x
OFST:	set	2
	.dcall	"4,0,_UART2_ReceiveData9"
	ld	a,21060
	and	a,#128
	clrw	x
	rlwa	x,a
	sllw	x
	ldw	(OFST-1,sp),x
	ld	a,21057
	clrw	x
	ld	xl,a
	rrwa	x,a
	or	a,(OFST+0,sp)
	rrwa	x,a
	or	a,(OFST-1,sp)
	and	a,#1
	rrwa	x,a
	addw	sp,#2
	ret	
	.scheck	_UART2_ReceiveData9
_UART2_SendData8:
	.dcall	"2,0,_UART2_SendData8"
	ld	21057,a
	ret	
	.scheck	_UART2_SendData8
_UART2_SendData9:
	pushw	x
OFST:	set	0
	.dcall	"4,0,_UART2_SendData9"
	bres	21060,#6
	srlw	x
	srlw	x
	ld	a,xl
	and	a,#64
	or	a,21060
	ld	21060,a
	ld	a,(OFST+2,sp)
	ld	21057,a
	popw	x
	ret	
	.scheck	_UART2_SendData9
_UART2_SendBreak:
	.dcall	"2,0,_UART2_SendBreak"
	bset	21061,#0
	ret	
	.scheck	_UART2_SendBreak
_UART2_SetAddress:
	push	a
OFST:	set	0
	.dcall	"3,0,_UART2_SetAddress"
	cp	a,#16
	jrult	L4101
	ldw	x,#556
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_SetAddress:_assert_failed"
	addw	sp,#4
L4101:
	ld	a,21063
	and	a,#240
	ld	21063,a
	ld	a,21063
	or	a,(OFST+1,sp)
	ld	21063,a
	pop	a
	ret	
	.scheck	_UART2_SetAddress
_UART2_SetGuardTime:
	.dcall	"2,0,_UART2_SetGuardTime"
	ld	21066,a
	ret	
	.scheck	_UART2_SetGuardTime
_UART2_SetPrescaler:
	.dcall	"2,0,_UART2_SetPrescaler"
	ld	21067,a
	ret	
	.scheck	_UART2_SetPrescaler
_UART2_GetFlagStatus:
	pushw	x
	push	a
OFST:	set	1
	.dcall	"5,0,_UART2_GetFlagStatus"
	cpw	x,#128
	jreq	L2301
	cpw	x,#64
	jreq	L2301
	cpw	x,#32
	jreq	L2301
	cpw	x,#16
	jreq	L2301
	cpw	x,#8
	jreq	L2301
	cpw	x,#4
	jreq	L2301
	cpw	x,#2
	jreq	L2301
	cpw	x,#1
	jreq	L2301
	cpw	x,#257
	jreq	L2301
	cpw	x,#769
	jreq	L2301
	cpw	x,#770
	jreq	L2301
	cpw	x,#528
	jreq	L2301
	ldw	x,#615
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_GetFlagStatus:_assert_failed"
	addw	sp,#4
L2301:
	ldw	x,(OFST+1,sp)
	cpw	x,#528
	jrne	L154
	ld	a,21063
	bcp	a,(OFST+2,sp)
	jreq	L574
	jp	LC013
L154:
	cpw	x,#257
	jrne	L164
	ld	a,21061
	bcp	a,(OFST+2,sp)
	jreq	L574
	jp	LC013
L164:
	cpw	x,#770
	jreq	L374
	cpw	x,#769
	jrne	L174
L374:
	ld	a,21065
	bcp	a,(OFST+2,sp)
	jreq	L574
LC013:
	ld	a,#1
	jra	L754
L574:
	clr	a
L754:
	addw	sp,#3
	ret	
L174:
	ld	a,21056
	bcp	a,(OFST+2,sp)
	jreq	L574
	jp	LC013
	.scheck	_UART2_GetFlagStatus
_UART2_ClearFlag:
	pushw	x
OFST:	set	0
	.dcall	"4,0,_UART2_ClearFlag"
	cpw	x,#32
	jreq	L4401
	cpw	x,#770
	jreq	L4401
	cpw	x,#769
	jreq	L4401
	cpw	x,#528
	jreq	L4401
	ldw	x,#705
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_ClearFlag:_assert_failed"
	addw	sp,#4
L4401:
	ldw	x,(OFST+1,sp)
	cpw	x,#32
	jrne	L705
	mov	21056,#223
	jra	L115
L705:
	cpw	x,#528
	jrne	L315
	bres	21063,#4
	jra	L115
L315:
	cpw	x,#770
	jrne	L715
	bres	21065,#1
	jra	L115
L715:
	bres	21065,#0
L115:
	popw	x
	ret	
	.scheck	_UART2_ClearFlag
_UART2_GetITStatus:
	pushw	x
	pushw	x
OFST:	set	2
	.dcall	"6,0,_UART2_GetITStatus"
	cpw	x,#631
	jreq	L6501
	cpw	x,#614
	jreq	L6501
	cpw	x,#597
	jreq	L6501
	cpw	x,#580
	jreq	L6501
	cpw	x,#565
	jreq	L6501
	cpw	x,#838
	jreq	L6501
	cpw	x,#1042
	jreq	L6501
	cpw	x,#256
	jreq	L6501
	ldw	x,#751
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_GetITStatus:_assert_failed"
	addw	sp,#4
L6501:
	ld	a,(OFST+2,sp)
	and	a,#15
	clrw	x
	ld	xl,a
	ld	a,#1
	tnzw	x
	jreq	L2601
L4601:
	sll	a
	decw	x
	jrne	L4601
L2601:
	ld	(OFST-1,sp),a
	ld	a,(OFST+2,sp)
	swap	a
	and	a,#15
	ld	(OFST+0,sp),a
	clrw	x
	ld	xl,a
	ld	a,#1
	tnzw	x
	jreq	L6601
L0701:
	sll	a
	decw	x
	jrne	L0701
L6601:
	ld	(OFST+0,sp),a
	ldw	x,(OFST+1,sp)
	cpw	x,#256
	jrne	L325
	ld	a,21060
	and	a,(OFST+0,sp)
	ld	(OFST+0,sp),a
	ld	a,21056
	jp	LC016
L325:
	cpw	x,#838
	jrne	L335
	ld	a,21063
	and	a,(OFST+0,sp)
	ld	(OFST+0,sp),a
	ld	a,21063
	jp	LC016
L335:
	cpw	x,#1042
	jrne	L345
	ld	a,21065
	and	a,(OFST+0,sp)
	ld	(OFST+0,sp),a
	ld	a,21065
LC016:
	bcp	a,(OFST-1,sp)
	jreq	L355
	ld	a,(OFST+0,sp)
	jreq	L355
LC015:
	ld	a,#1
	jra	L135
L345:
	ld	a,21061
	and	a,(OFST+0,sp)
	ld	(OFST+0,sp),a
	ld	a,21056
	bcp	a,(OFST-1,sp)
	jreq	L355
	ld	a,(OFST+0,sp)
	jrne	LC015
L355:
	clr	a
L135:
	addw	sp,#4
	ret	
	.scheck	_UART2_GetITStatus
_UART2_ClearITPendingBit:
	pushw	x
OFST:	set	0
	.dcall	"4,0,_UART2_ClearITPendingBit"
	cpw	x,#597
	jreq	L0011
	cpw	x,#1042
	jreq	L0011
	cpw	x,#838
	jreq	L0011
	ldw	x,#858
	pushw	x
	clrw	x
	pushw	x
	ldw	x,#L323
	call	_assert_failed
	.dcall	"_UART2_ClearITPendingBit:_assert_failed"
	addw	sp,#4
L0011:
	ldw	x,(OFST+1,sp)
	cpw	x,#597
	jrne	L755
	mov	21056,#223
	jra	L165
L755:
	cpw	x,#838
	jrne	L365
	bres	21063,#4
	jra	L165
L365:
	bres	21065,#1
L165:
	popw	x
	ret	
	.scheck	_UART2_ClearITPendingBit
_UART2_SetRxHandler:
	.dcall	"2,0,_UART2_SetRxHandler"
	ldw	L123_UART2_RxFunc,x
	ret	
	.scheck	_UART2_SetRxHandler
	bsct
_delay_us_pred:
	dc.b	0
_delay_us_post:
	dc.b	0
	switch	.const
L2111:
	dc.l	4000000
	switch	.text
_Init_Delay:
	subw	sp,#5
OFST:	set	5
	.dcall	"7,0,1,_Init_Delay"
	clr	_delay_us_pred
	clr	_delay_us_post
	call	_CLK_GetCPUClk
	.dcall	"_Init_Delay:_CLK_GetCPUClk"
	ldw	x,sp
	incw	x
	call	c_rtol
	.dcall	"_Init_Delay:c_rtol"
	ldw	x,sp
	incw	x
	call	c_ltor
	.dcall	"_Init_Delay:c_ltor"
	ldw	x,#L2111
	call	c_ludv
	.dcall	"_Init_Delay:c_ludv"
	ld	a,c_lreg+3
	ld	(OFST+0,sp),a
	jra	L375
L765:
	srl	(OFST+0,sp)
	inc	_delay_us_pred
	ld	a,(OFST+0,sp)
L375:
	cp	a,#2
	jruge	L765
	ld	a,(OFST+0,sp)
	jrne	L775
	ldw	x,#2304
	ldw	c_lreg+2,x
	ldw	x,#61
	ldw	c_lreg,x
	ldw	x,sp
	incw	x
	call	c_ludv
	.dcall	"_Init_Delay:c_ludv"
	ld	a,c_lreg+3
	ld	(OFST+0,sp),a
	jra	L506
L106:
	srl	(OFST+0,sp)
	inc	_delay_us_post
	ld	a,(OFST+0,sp)
L506:
	cp	a,#2
	jruge	L106
L775:
	addw	sp,#5
	ret	
	.scheck	_Init_Delay
_delay_us:
	subw	sp,#8
OFST:	set	8
	.dcall	"10,0,1,_delay_us"
	call	c_uitolx
	.dcall	"_delay_us:c_uitolx"
	ldw	x,sp
	addw	x,#OFST-3
	call	c_rtol
	.dcall	"_delay_us:c_rtol"
	ld	a,_delay_us_pred
	ldw	x,sp
	addw	x,#OFST-3
	call	c_lglsh
	.dcall	"_delay_us:c_lglsh"
	ld	a,_delay_us_post
	ldw	x,sp
	addw	x,#OFST-3
	call	c_lgursh
	.dcall	"_delay_us:c_lgursh"
	ldw	x,(OFST-1,sp)
	ldw	(OFST-5,sp),x
	cpw	x,#31
	jrult	L116
	subw	x,#25
	ldw	(OFST-5,sp),x
	.sfile	"src/stm8s_delay.c",54
L6111:
	decw	X
	nop	
	jrne	L6111

	.sfile
L116:
	addw	sp,#8
	ret	
	.scheck	_delay_us
_delay_ms:
	pushw	x
OFST:	set	0
	.dcall	"4,0,_delay_ms"
	jra	L516
L316:
	ldw	x,#1000
	call	_delay_us
	.dcall	"_delay_ms:_delay_us"
L516:
	ldw	x,(OFST+1,sp)
	decw	x
	ldw	(OFST+1,sp),x
	incw	x
	jrne	L316
	popw	x
	ret	
	.scheck	_delay_ms
	scross	on
f_NonHandledInterrupt:
	.dcall	"6,0;f_NonHandledInterrupt"
	iret	
f_TRAP_IRQHandler:
	.dcall	"6,0;f_TRAP_IRQHandler"
	iret	
f_TLI_IRQHandler:
	.dcall	"6,0;f_TLI_IRQHandler"
	iret	
f_AWU_IRQHandler:
	.dcall	"6,0;f_AWU_IRQHandler"
	iret	
f_CLK_IRQHandler:
	.dcall	"6,0;f_CLK_IRQHandler"
	iret	
f_EXTI_PORTA_IRQHandler:
	.dcall	"6,0;f_EXTI_PORTA_IRQHandler"
	iret	
f_EXTI_PORTB_IRQHandler:
	.dcall	"6,0;f_EXTI_PORTB_IRQHandler"
	iret	
f_EXTI_PORTC_IRQHandler:
	.dcall	"6,0;f_EXTI_PORTC_IRQHandler"
	iret	
f_EXTI_PORTD_IRQHandler:
	.dcall	"6,0;f_EXTI_PORTD_IRQHandler"
	iret	
f_EXTI_PORTE_IRQHandler:
	.dcall	"6,0;f_EXTI_PORTE_IRQHandler"
	iret	
f_SPI_IRQHandler:
	.dcall	"6,0;f_SPI_IRQHandler"
	iret	
f_TIM1_UPD_OVF_TRG_BRK_IRQHandler:
	.dcall	"6,0;f_TIM1_UPD_OVF_TRG_BRK_IRQHandler"
	iret	
f_TIM1_CAP_COM_IRQHandler:
	.dcall	"6,0;f_TIM1_CAP_COM_IRQHandler"
	iret	
f_TIM2_UPD_OVF_BRK_IRQHandler:
	.dcall	"6,0;f_TIM2_UPD_OVF_BRK_IRQHandler"
	iret	
f_TIM2_CAP_COM_IRQHandler:
	.dcall	"6,0;f_TIM2_CAP_COM_IRQHandler"
	iret	
f_TIM3_UPD_OVF_BRK_IRQHandler:
	.dcall	"6,0;f_TIM3_UPD_OVF_BRK_IRQHandler"
	iret	
f_TIM3_CAP_COM_IRQHandler:
	.dcall	"6,0;f_TIM3_CAP_COM_IRQHandler"
	iret	
f_I2C_IRQHandler:
	.dcall	"6,0;f_I2C_IRQHandler"
	iret	
f_UART2_TX_IRQHandler:
	.dcall	"6,0;f_UART2_TX_IRQHandler"
	iret	
f_UART2_RX_IRQHandler:
	push	cc
	pop	a
	and	a,#191
	push	a
	pop	cc
	push	c_x+2
	ldw	x,c_x
	pushw	x
	push	c_y+2
	ldw	x,c_y
	pushw	x
	.dcall	"12,0,+1;f_UART2_RX_IRQHandler"
	ldw	x,L123_UART2_RxFunc
	jreq	L126
	ld	a,21057
	call	(x)
	.dcall	"f_UART2_RX_IRQHandler:"
	jra	L326
L126:
	ldw	x,#597
	call	_UART2_ClearITPendingBit
	.dcall	"f_UART2_RX_IRQHandler:_UART2_ClearITPendingBit"
L326:
	popw	x
	ldw	c_y,x
	pop	c_y+2
	popw	x
	ldw	c_x,x
	pop	c_x+2
	iret	
f_ADC1_IRQHandler:
	.dcall	"6,0;f_ADC1_IRQHandler"
	iret	
f_TIM4_UPD_OVF_IRQHandler:
	.dcall	"6,0;f_TIM4_UPD_OVF_IRQHandler"
	iret	
f_EEPROM_EEC_IRQHandler:
	.dcall	"6,0;f_EEPROM_EEC_IRQHandler"
	iret	
	scross	off
_cmdinit:
OFST: set -1
	.dcall	"3,3,_cmdinit"
	ld	_cmd+41,a
	clr	_cmd+40
	ldw	x,(OFST+4,sp)
	ldw	_cmd+42,x
	ld	a,(OFST+6,sp)
	ld	_cmd+44,a
	clr	_cmd+46
	ret	
	.scheck	_cmdinit
_cmdinputchar:
	push	a
OFST:	set	0
	.dcall	"3,0,_cmdinputchar"
	ld	a,_cmd+45
	jreq	L526
	sub	a,#2
	jreq	L726
	.sfile	"main.c",163
	nop	
	.sfile
	jra	L536
L526:
	ld	a,_cmd+40
	cp	a,#40
	jruge	L736
	inc	_cmd+40
	clrw	x
	ld	xl,a
	ld	a,(OFST+1,sp)
	ld	(_cmd,x),a
	jra	L726
L736:
	mov	_cmd+45,#2
L726:
	ld	a,(OFST+1,sp)
	cp	a,_cmd+41
	jrne	L536
	bset	_cmd+45,#0
L536:
	pop	a
	ret	
	.scheck	_cmdinputchar
_cmdparcer:
	push	a
OFST:	set	1
	.dcall	"3,0,_cmdparcer"
	clr	(OFST+0,sp)
	jra	L156
L546:
	clrw	x
	ld	xl,a
	ld	a,(_cmd,x)
	cp	a,#32
	jrne	L556
	ld	a,(OFST+0,sp)
	clrw	x
	ld	xl,a
	clr	(_cmd,x)
L556:
	inc	(OFST+0,sp)
L156:
	ld	a,(OFST+0,sp)
	cp	a,_cmd+40
	jrult	L546
	ld	a,_cmd+40
	clrw	x
	ld	xl,a
	decw	x
	clr	(_cmd,x)
	ld	a,_cmd
	jreq	L756
	ld	a,_cmd+46
	inc	_cmd+46
	clrw	x
	ld	xl,a
	sllw	x
	ldw	y,#_cmd
	ldw	(_cmd+47,x),y
L756:
	ld	a,#1
	ld	(OFST+0,sp),a
	jra	L566
L166:
	clrw	x
	ld	xl,a
	decw	x
	tnz	(_cmd,x)
	jrne	L176
	clrw	x
	ld	xl,a
	ld	a,(_cmd,x)
	jreq	L176
	ld	a,_cmd+46
	cp	a,#3
	jruge	L176
	ld	a,(OFST+0,sp)
	add	a,#_cmd
	clrw	x
	ld	xl,a
	ld	a,_cmd+46
	inc	_cmd+46
	clrw	y
	ld	yl,a
	sllw	y
	ldw	(_cmd+47,y),x
L176:
	inc	(OFST+0,sp)
L566:
	ld	a,_cmd+40
	clrw	x
	ld	xl,a
	decw	x
	ld	a,(OFST+0,sp)
	clrw	y
	ld	yl,a
	ldw	c_y,y
	cpw	x,c_y
	jrsgt	L166
	.sfile	"main.c",185
	nop	
	.sfile
	pop	a
	ret	
	.scheck	_cmdparcer
_cmdreset:
	.dcall	"2,0,_cmdreset"
	clr	_cmd+40
	clr	_cmd+45
	clr	_cmd+46
	ret	
	.scheck	_cmdreset
_cmdexec:
	subw	sp,#5
OFST:	set	5
	.dcall	"7,0,_cmdexec"
	clr	(OFST+0,sp)
	jra	L776
L376:
	ld	xl,a
	ld	a,#22
	mul	x,a
	addw	x,_cmd+42
	ldw	(OFST-4,sp),x
	ld	a,(OFST+0,sp)
	ld	xl,a
	ld	a,#22
	mul	x,a
	addw	x,_cmd+42
	ldw	x,(20,x)
	ldw	(OFST-2,sp),x
	ldw	x,(OFST-4,sp)
	pushw	x
	ldw	x,_cmd+47
	call	_strcmp
	.dcall	"_cmdexec:_strcmp"
	addw	sp,#2
	tnzw	x
	jrne	L307
	.sfile	"main.c",208
	nop	
	.sfile
	ldw	x,#_cmd+47
	pushw	x
	ld	a,_cmd+46
	ldw	x,(OFST+0,sp)
	call	(x)
	.dcall	"_cmdexec:"
	popw	x
L107:
	call	_cmdreset
	.dcall	"_cmdexec:_cmdreset"
	addw	sp,#5
	ret	
L307:
	inc	(OFST+0,sp)
L776:
	ld	a,(OFST+0,sp)
	cp	a,_cmd+44
	jrult	L376
	jra	L107
	.scheck	_cmdexec
_cmdmainloop:
	.dcall	"2,0,_cmdmainloop"
	ld	a,_cmd+45
	dec	a
	jreq	L507
	sub	a,#2
	jreq	L707
	ret	
L507:
	call	_cmdparcer
	.dcall	"_cmdmainloop:_cmdparcer"
	.dcall	"_cmdmainloop:_cmdexec"
	jp	_cmdexec
L707:
	.dcall	"_cmdmainloop:_cmdreset"
	jp	_cmdreset
	.scheck	_cmdmainloop
_led:
	push	a
OFST:	set	0
	.dcall	"3,2,_led"
	ld	a,#2
	ld	(OFST+1,sp),a
	jreq	L517
	ldw	x,#L127
	pushw	x
	ldw	x,(OFST+6,sp)
	ldw	x,(2,x)
	call	_strcmp
	.dcall	"_led:_strcmp"
	addw	sp,#2
	tnzw	x
	jrne	L717
	push	#32
	ldw	x,#20500
	call	_GPIO_WriteLow
	.dcall	"_led:_GPIO_WriteLow"
	jp	LC017
L717:
	ldw	x,#L727
	pushw	x
	ldw	x,(OFST+6,sp)
	ldw	x,(2,x)
	call	_strcmp
	.dcall	"_led:_strcmp"
	addw	sp,#2
	tnzw	x
	jrne	L327
	push	#32
	ldw	x,#20500
	call	_GPIO_WriteHigh
	.dcall	"_led:_GPIO_WriteHigh"
LC017:
	pop	a
L327:
	clrw	x
	pop	a
	ret	
L517:
	ldw	x,#1
	pop	a
	ret	
	.scheck	_led
	bsct
_bl:
	dc.b	1
	switch	.text
_blink:
	push	a
OFST:	set	0
	.dcall	"3,2,_blink"
	ld	a,#2
	ld	(OFST+1,sp),a
	jreq	L337
	ldw	x,#L127
	pushw	x
	ldw	x,(OFST+6,sp)
	ldw	x,(2,x)
	call	_strcmp
	.dcall	"_blink:_strcmp"
	addw	sp,#2
	tnzw	x
	jrne	L537
	mov	_bl,#1
	jra	L737
L537:
	ldw	x,#L727
	pushw	x
	ldw	x,(OFST+6,sp)
	ldw	x,(2,x)
	call	_strcmp
	.dcall	"_blink:_strcmp"
	addw	sp,#2
	tnzw	x
	jrne	L737
	clr	_bl
L737:
	clrw	x
	pop	a
	ret	
L337:
	ldw	x,#1
	pop	a
	ret	
	.scheck	_blink
	switch	.const
_testf:
	dc.b	"led",0
	ds.b	16
	.dcall	"_testf.T:_led"
	dc.w	_led
	dc.b	"blink",0
	ds.b	14
	.dcall	"_testf.T:_blink"
	dc.w	_blink
	.dcall	":_cmdinputchar"
	switch	.text
_main:
	.dcall	"2,0,_main"
	call	_Init_Delay
	.dcall	"_main:_Init_Delay"
	push	#2
	ldw	x,#_testf
	pushw	x
	ld	a,#13
	call	_cmdinit
	.dcall	"_main:_cmdinit"
	addw	sp,#3
	push	#12
	push	#128
	push	#0
	push	#0
	push	#0
	ldw	x,#9600
	pushw	x
	clrw	x
	pushw	x
	call	_UART2_Init
	.dcall	"_main:_UART2_Init"
	addw	sp,#9
	ldw	x,#_cmdinputchar
	call	_UART2_SetRxHandler
	.dcall	"_main:_UART2_SetRxHandler"
	push	#1
	ldw	x,#517
	call	_UART2_ITConfig
	.dcall	"_main:_UART2_ITConfig"
	pop	a
	ld	a,#1
	call	_UART2_Cmd
	.dcall	"_main:_UART2_Cmd"
	.sfile	"main.c",360
	rim	
	.sfile
	push	#160
	push	#32
	ldw	x,#20500
	call	_GPIO_Init
	.dcall	"_main:_GPIO_Init"
	popw	x
L347:
	call	_cmdmainloop
	.dcall	"_main:_cmdmainloop"
	ld	a,_bl
	jreq	L747
	push	#32
	ldw	x,#20500
	call	_GPIO_WriteReverse
	.dcall	"_main:_GPIO_WriteReverse"
	pop	a
L747:
	ldw	x,#1000
	call	_delay_ms
	.dcall	"_main:_delay_ms"
	jra	L347
	.scheck	_main
_assert_failed:
	.dcall	"2,4,_assert_failed"
L157:
	jra	L157
	.scheck	_assert_failed
	xdef	_main
	xdef	_testf
	xdef	_blink
	xdef	_bl
	xdef	_led
	xdef	_cmdmainloop
	xdef	_cmdexec
	xdef	_cmdreset
	xdef	_cmdparcer
	xdef	_cmdinputchar
	xdef	_cmdinit
	switch	.ubsct
_cmd:
	ds.b	53
	xdef	_cmd
_mass:
	ds.b	100
	xdef	_mass
	xref	_strcmp
	xdef	_delay_us_post
	xdef	_delay_us_pred
	xdef	_CLKPrescTable
	xdef	_HSIDivFactor
	xdef	f_EEPROM_EEC_IRQHandler
	xdef	f_TIM4_UPD_OVF_IRQHandler
	xdef	f_ADC1_IRQHandler
	xdef	f_UART2_TX_IRQHandler
	xdef	f_UART2_RX_IRQHandler
	xdef	f_I2C_IRQHandler
	xdef	f_TIM3_CAP_COM_IRQHandler
	xdef	f_TIM3_UPD_OVF_BRK_IRQHandler
	xdef	f_TIM2_CAP_COM_IRQHandler
	xdef	f_TIM2_UPD_OVF_BRK_IRQHandler
	xdef	f_TIM1_UPD_OVF_TRG_BRK_IRQHandler
	xdef	f_TIM1_CAP_COM_IRQHandler
	xdef	f_SPI_IRQHandler
	xdef	f_EXTI_PORTE_IRQHandler
	xdef	f_EXTI_PORTD_IRQHandler
	xdef	f_EXTI_PORTC_IRQHandler
	xdef	f_EXTI_PORTB_IRQHandler
	xdef	f_EXTI_PORTA_IRQHandler
	xdef	f_CLK_IRQHandler
	xdef	f_AWU_IRQHandler
	xdef	f_TLI_IRQHandler
	xdef	f_TRAP_IRQHandler
	xdef	f_NonHandledInterrupt
	xdef	_assert_failed
	xdef	_UART2_SetRxHandler
	xdef	_UART2_ClearITPendingBit
	xdef	_UART2_GetITStatus
	xdef	_UART2_ClearFlag
	xdef	_UART2_GetFlagStatus
	xdef	_UART2_SetPrescaler
	xdef	_UART2_SetGuardTime
	xdef	_UART2_SetAddress
	xdef	_UART2_SendBreak
	xdef	_UART2_SendData9
	xdef	_UART2_SendData8
	xdef	_UART2_ReceiveData9
	xdef	_UART2_ReceiveData8
	xdef	_UART2_ReceiverWakeUpCmd
	xdef	_UART2_WakeUpConfig
	xdef	_UART2_SmartCardNACKCmd
	xdef	_UART2_SmartCardCmd
	xdef	_UART2_LINCmd
	xdef	_UART2_LINConfig
	xdef	_UART2_LINBreakDetectionConfig
	xdef	_UART2_IrDACmd
	xdef	_UART2_IrDAConfig
	xdef	_UART2_ITConfig
	xdef	_UART2_Cmd
	xdef	_UART2_Init
	xdef	_UART2_DeInit
	xdef	_TIM4_ClearITPendingBit
	xdef	_TIM4_GetITStatus
	xdef	_TIM4_ClearFlag
	xdef	_TIM4_SetAutoreload
	xdef	_TIM4_SetCounter
	xdef	_TIM4_ARRPreloadConfig
	xdef	_TIM4_UpdateRequestConfig
	xdef	_TIM4_UpdateDisableConfig
	xdef	_TIM4_ITConfig
	xdef	_TIM4_Cmd
	xdef	_TIM4_TimeBaseInit
	xdef	_GPIO_ExternalPullUpConfig
	xdef	_GPIO_ReadInputPin
	xdef	_GPIO_ReadOutputData
	xdef	_GPIO_ReadInputData
	xdef	_GPIO_WriteReverse
	xdef	_GPIO_WriteLow
	xdef	_GPIO_WriteHigh
	xdef	_GPIO_Write
	xdef	_GPIO_Init
	xdef	_GPIO_DeInit
	xdef	_CLK_ClearITPendingBit
	xdef	_CLK_GetITStatus
	xdef	_CLK_GetFlagStatus
	xdef	_CLK_GetSYSCLKSource
	xdef	_CLK_GetCPUClk
	xdef	_CLK_GetClockFreq
	xdef	_CLK_SYSCLKEmergencyClear
	xdef	_CLK_ClockSecuritySystemEnable
	xdef	_CLK_SYSCLKConfig
	xdef	_CLK_ITConfig
	xdef	_CLK_HSIPrescalerConfig
	xdef	_CLK_ClockSwitchConfig
	xdef	_CLK_PeripheralClockConfig
	xdef	_CLK_ClockSwitchCmd
	xdef	_CLK_HSICmd
	xdef	_CLK_HSECmd
	xdef	_CLK_DeInit
	xdef	_delay_ms
	xdef	_delay_us
	xdef	_Init_Delay
	switch	.const
L727:
	dc.b	"off",0
L127:
	dc.b	"on",0
L323:
	dc.b	"src/stm8s_uart2.c",0
L762:
	dc.b	"src/stm8s_tim4.c",0
L142:
	dc.b	"src/stm8s_gpio.c",0
L31:
	dc.b	"src/stm8s_clk.c",0
	xref.b	c_lreg
	xref.b	c_x
	xref.b	c_y
	xref	c_lgursh
	xref	c_lglsh
	xref	c_uitolx
	xref	c_lsub
	xref	c_smul
	xref	c_llsh
	xref	c_lcmp
	xref	c_lursh
	xref	c_ltor
	xref	c_ludv
	xref	c_rtol
	end
