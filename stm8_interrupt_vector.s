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
__vectab:
	dc.b	130
	.dcall	"__vectab.T:__stext"
	dc.b	page(__stext)
	dc.w	__stext
	dc.b	130
	.dcall	"__vectab.T:f_TRAP_IRQHandler"
	dc.b	page(f_TRAP_IRQHandler)
	dc.w	f_TRAP_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_TLI_IRQHandler"
	dc.b	page(f_TLI_IRQHandler)
	dc.w	f_TLI_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_AWU_IRQHandler"
	dc.b	page(f_AWU_IRQHandler)
	dc.w	f_AWU_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_CLK_IRQHandler"
	dc.b	page(f_CLK_IRQHandler)
	dc.w	f_CLK_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_EXTI_PORTA_IRQHandler"
	dc.b	page(f_EXTI_PORTA_IRQHandler)
	dc.w	f_EXTI_PORTA_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_EXTI_PORTB_IRQHandler"
	dc.b	page(f_EXTI_PORTB_IRQHandler)
	dc.w	f_EXTI_PORTB_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_EXTI_PORTC_IRQHandler"
	dc.b	page(f_EXTI_PORTC_IRQHandler)
	dc.w	f_EXTI_PORTC_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_EXTI_PORTD_IRQHandler"
	dc.b	page(f_EXTI_PORTD_IRQHandler)
	dc.w	f_EXTI_PORTD_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_EXTI_PORTE_IRQHandler"
	dc.b	page(f_EXTI_PORTE_IRQHandler)
	dc.w	f_EXTI_PORTE_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_NonHandledInterrupt"
	dc.b	page(f_NonHandledInterrupt)
	dc.w	f_NonHandledInterrupt
	dc.b	130
	.dcall	"__vectab.T:f_NonHandledInterrupt"
	dc.b	page(f_NonHandledInterrupt)
	dc.w	f_NonHandledInterrupt
	dc.b	130
	.dcall	"__vectab.T:f_SPI_IRQHandler"
	dc.b	page(f_SPI_IRQHandler)
	dc.w	f_SPI_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_TIM1_UPD_OVF_TRG_BRK_IRQHandler"
	dc.b	page(f_TIM1_UPD_OVF_TRG_BRK_IRQHandler)
	dc.w	f_TIM1_UPD_OVF_TRG_BRK_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_TIM1_CAP_COM_IRQHandler"
	dc.b	page(f_TIM1_CAP_COM_IRQHandler)
	dc.w	f_TIM1_CAP_COM_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_TIM2_UPD_OVF_BRK_IRQHandler"
	dc.b	page(f_TIM2_UPD_OVF_BRK_IRQHandler)
	dc.w	f_TIM2_UPD_OVF_BRK_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_TIM2_CAP_COM_IRQHandler"
	dc.b	page(f_TIM2_CAP_COM_IRQHandler)
	dc.w	f_TIM2_CAP_COM_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_TIM3_UPD_OVF_BRK_IRQHandler"
	dc.b	page(f_TIM3_UPD_OVF_BRK_IRQHandler)
	dc.w	f_TIM3_UPD_OVF_BRK_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_TIM3_CAP_COM_IRQHandler"
	dc.b	page(f_TIM3_CAP_COM_IRQHandler)
	dc.w	f_TIM3_CAP_COM_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_NonHandledInterrupt"
	dc.b	page(f_NonHandledInterrupt)
	dc.w	f_NonHandledInterrupt
	dc.b	130
	.dcall	"__vectab.T:f_NonHandledInterrupt"
	dc.b	page(f_NonHandledInterrupt)
	dc.w	f_NonHandledInterrupt
	dc.b	130
	.dcall	"__vectab.T:f_I2C_IRQHandler"
	dc.b	page(f_I2C_IRQHandler)
	dc.w	f_I2C_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_UART2_TX_IRQHandler"
	dc.b	page(f_UART2_TX_IRQHandler)
	dc.w	f_UART2_TX_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_UART2_RX_IRQHandler"
	dc.b	page(f_UART2_RX_IRQHandler)
	dc.w	f_UART2_RX_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_ADC1_IRQHandler"
	dc.b	page(f_ADC1_IRQHandler)
	dc.w	f_ADC1_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_TIM4_UPD_OVF_IRQHandler"
	dc.b	page(f_TIM4_UPD_OVF_IRQHandler)
	dc.w	f_TIM4_UPD_OVF_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_EEPROM_EEC_IRQHandler"
	dc.b	page(f_EEPROM_EEC_IRQHandler)
	dc.w	f_EEPROM_EEC_IRQHandler
	dc.b	130
	.dcall	"__vectab.T:f_NonHandledInterrupt"
	dc.b	page(f_NonHandledInterrupt)
	dc.w	f_NonHandledInterrupt
	dc.b	130
	.dcall	"__vectab.T:f_NonHandledInterrupt"
	dc.b	page(f_NonHandledInterrupt)
	dc.w	f_NonHandledInterrupt
	dc.b	130
	.dcall	"__vectab.T:f_NonHandledInterrupt"
	dc.b	page(f_NonHandledInterrupt)
	dc.w	f_NonHandledInterrupt
	dc.b	130
	.dcall	"__vectab.T:f_NonHandledInterrupt"
	dc.b	page(f_NonHandledInterrupt)
	dc.w	f_NonHandledInterrupt
	dc.b	130
	.dcall	"__vectab.T:f_NonHandledInterrupt"
	dc.b	page(f_NonHandledInterrupt)
	dc.w	f_NonHandledInterrupt
	xdef	__vectab
	xref	f_EEPROM_EEC_IRQHandler
	xref	f_TIM4_UPD_OVF_IRQHandler
	xref	f_ADC1_IRQHandler
	xref	f_UART2_TX_IRQHandler
	xref	f_UART2_RX_IRQHandler
	xref	f_I2C_IRQHandler
	xref	f_TIM3_CAP_COM_IRQHandler
	xref	f_TIM3_UPD_OVF_BRK_IRQHandler
	xref	f_TIM2_CAP_COM_IRQHandler
	xref	f_TIM2_UPD_OVF_BRK_IRQHandler
	xref	f_TIM1_UPD_OVF_TRG_BRK_IRQHandler
	xref	f_TIM1_CAP_COM_IRQHandler
	xref	f_SPI_IRQHandler
	xref	f_EXTI_PORTE_IRQHandler
	xref	f_EXTI_PORTD_IRQHandler
	xref	f_EXTI_PORTC_IRQHandler
	xref	f_EXTI_PORTB_IRQHandler
	xref	f_EXTI_PORTA_IRQHandler
	xref	f_CLK_IRQHandler
	xref	f_AWU_IRQHandler
	xref	f_TLI_IRQHandler
	xref	f_TRAP_IRQHandler
	xref	f_NonHandledInterrupt
	xref	__stext
	end
