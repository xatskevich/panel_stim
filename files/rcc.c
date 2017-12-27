
#include "main.h"

//watchdog
void Init_IWDG(){
	IWDG_Enable();
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(1000);				//0,8с
	while(IWDG->SR);
	IWDG_ReloadCounter();
}

void Init_RCC(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_TS, ENABLE);
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM, ENABLE);
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART4, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_CRS, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_CEC, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
}
