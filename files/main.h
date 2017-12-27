
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f0xx.h"                  // Device header
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_can.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_iwdg.h"
#include "stm32f0xx_spi.h"

#include "ADC.h"
#include "CAN.h"
#include "GPIO.h"
#include "rcc.h"
#include "timer.h"

#define sys_clock 48000
#define idle 5600
#define koef_k 9200
#define koef_perc 25600

#define set_strobe 		GPIO_SetBits(GPIOC, GPIO_Pin_6);		//строб записи в регистр
#define reset_strobe 	GPIO_ResetBits(GPIOC, GPIO_Pin_6);

#define water_min_address 0x08007800
#define water_max_address 0x08007804
#define foam_min_address 0x08007808
#define foam_max_address 0x0800780C
#define flash_pto_address 0x08007810
#define input_config_address 0x08007814
#define out1_config_address 0x08007818
#define out2_config_address 0x0800781C
#define out3_config_address 0x08007820
#define button_mask_address 0x08007824
#define out4_config_address 0x08007828
#define out5_config_address 0x0800782C

#define set_low_water_led 	GPIO_SetBits(GPIOC, GPIO_Pin_10)
#define reset_low_water_led GPIO_ResetBits(GPIOC, GPIO_Pin_10)
#define set_low_foam_led 	GPIO_SetBits(GPIOA, GPIO_Pin_15)
#define reset_low_foam_led 	GPIO_ResetBits(GPIOA, GPIO_Pin_15)
//********* shield *******************
#define is_button_power 	1<<1
#define is_button_clutch 	1<<2
#define is_button_left		1<<4
#define is_button_right		1<<5
#define is_button_start		1<<0
#define is_button_stop		1<<9
#define is_button_reset		1<<8
#define is_button_inc		1<<6
#define is_button_dec		1<<7
//***********************************
/************ keyboard ********************
#define is_button_power 	1<<1
#define is_button_clutch 	1<<0
#define is_button_left		1<<2
#define is_button_right		1<<4
#define is_button_start		1<<9
#define is_button_stop		1<<8
#define is_button_reset		1<<7
#define is_button_inc		1<<5
#define is_button_dec		1<<6
*************************************/
#define set_out1		GPIO_SetBits(GPIOB, GPIO_Pin_14)
#define reset_out1		GPIO_ResetBits(GPIOB, GPIO_Pin_14)
#define set_out2		GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define reset_out2		GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define set_out3		GPIO_SetBits(GPIOC, GPIO_Pin_9)
#define reset_out3		GPIO_ResetBits(GPIOC, GPIO_Pin_9)
//#define set_out4		GPIO_SetBits(GPIOB, GPIO_Pin_12)
//#define reset_out4		GPIO_ResetBits(GPIOB, GPIO_Pin_12)
//#define set_out5		GPIO_SetBits(GPIOC, GPIO_Pin_8)
//#define reset_out5		GPIO_ResetBits(GPIOC, GPIO_Pin_8)

#define set_out4		GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define reset_out4		GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define set_out5		GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define reset_out5		GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define set_out_power		GPIO_SetBits(GPIOC, GPIO_Pin_7)
#define reset_out_power		GPIO_ResetBits(GPIOC, GPIO_Pin_7)
#define set_out_clutch		GPIO_SetBits(GPIOC, GPIO_Pin_9)
#define reset_out_clutch	GPIO_ResetBits(GPIOC, GPIO_Pin_9)
#define set_out_left		GPIO_SetBits(GPIOB, GPIO_Pin_14)
#define reset_out_left		GPIO_ResetBits(GPIOB, GPIO_Pin_14)
#define set_out_right		GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define reset_out_right		GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define set_out_start		GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define reset_out_start		GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define set_out_stop		GPIO_SetBits(GPIOC, GPIO_Pin_8)
#define reset_out_stop		GPIO_ResetBits(GPIOC, GPIO_Pin_8)
#define set_out_inc			GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define reset_out_inc		GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define set_out_dec			GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define reset_out_dec		GPIO_ResetBits(GPIOB, GPIO_Pin_11)
#define set_led_oil			GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define reset_led_oil		GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define set_led_temp		GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define reset_led_temp		GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define is_in1				(GPIOA->IDR & 0x4) == 0
#define is_in2				(GPIOA->IDR & 0x8) == 0
#define set_background		GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define reset_background	GPIO_ResetBits(GPIOA, GPIO_Pin_6)

#define set_led_power		GPIO_SetBits(GPIOC, GPIO_Pin_11)
#define reset_led_power		GPIO_ResetBits(GPIOC, GPIO_Pin_11)
#define set_led_clutch		GPIO_SetBits(GPIOA, GPIO_Pin_10)
#define reset_led_clutch	GPIO_ResetBits(GPIOA, GPIO_Pin_10)
#define set_led_left		GPIO_SetBits(GPIOC, GPIO_Pin_12)
#define reset_led_left		GPIO_ResetBits(GPIOC, GPIO_Pin_12)
#define set_led_right		GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define reset_led_right		GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define set_led_reset		GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define reset_led_reset		GPIO_ResetBits(GPIOA, GPIO_Pin_9)

#define bit_left	1<<0
#define bit_right	1<<1
#define bit_beam	1<<2
#define bit_rear	1<<3
#define bit_inside	1<<4
#define bit_strobe	1<<5
#define bit_tail	1<<6


extern uint8_t is_idle, left, right, is_stop, power_up, morg, pto_address, can_on, can_cnt;
extern uint8_t w_morg, f_morg;		//8 бит - рост шкалы
							//значение без 8 бита - спад шкалы
extern uint16_t to_revs, rpm, rpm_buf;
extern uint8_t oil, temper, light_buf, ignition,
inputs;	//0 - роллета
		//1 - ступень
extern uint8_t blink_cnt, blink;
extern uint16_t butt;	//0 - сцепление
				//1 - питание
				//2 - осв слева
				//3 -
				//4 - осв справа
				//5 - обор+
				//6 - обор-
				//7 - сброс
				//8 - стоп
				//9 - пуск

extern uint8_t pwm_count;		//счетчик до 8 для ШИМ

extern uint8_t buttons;	//0 - питание
					//1 - сцепление
					//2 - осв лев
					//3 - осв прав
					//4 - сброс
					//5 - сигнал на проверку калибровки
					//6 - моргалка
					//7 -
extern uint8_t buttons_out;	//0 - ПУСК
						//1 - СТОП
						//2 - обор+
						//3 - обор-
extern uint8_t button_mask;
extern uint8_t light_mask;		//0 - осв слева
						//1 - осв справа
						//2 - маяк
						//3 - задний прожектор
						//4 - осв отсеков
						//5 - строб
						//6 - габариты
						//7 -

extern uint8_t input_config,	//0 - вх1 роллета
						//1 - вх1 ступень
						//2 - вх1 активный
						//3 - вх1 неактивный
						//4 - вх2 роллета
						//5 - вх2 ступень
						//6 - вх2 активный
						//7 - вх2 неактивный
out1_config, out2_config, out3_config,	//0 - осв слева
out4_config, out5_config;				//1 - осв справа
										//2 - маяк
										//3 - задний прожектор
										//4 - осв отсека
										//5 - строб
										//6 - габарит

extern uint8_t butt_b[5],		//буфер ШИМ кнопок
		analog_b[2],	//буфер ШИМ нижних сегментов
		ind_b[16];		//буфер ШИМ индикаторов
extern uint8_t rev_p, rev_m;

typedef struct {
	int16_t min;
	int16_t max;
	int16_t koeff;
	int16_t koeff_perc;
	int16_t input;
	int8_t out;
	int8_t percent;
	uint8_t count;
	int16_t analog;
} levels;

extern levels water, foam;

void flash_erase(void);
void flash_prog_all(void);
//void morg(uint8_t source);

extern CanTxMsg TxMessage;
extern GPIO_InitTypeDef GPIO_InitStructure;
extern ADC_InitTypeDef ADC_InitStructure;
extern NVIC_InitTypeDef NVIC_InitStructure;
extern CAN_InitTypeDef CAN_InitStructure;
extern CAN_FilterInitTypeDef CAN_FilterInitStructure;
extern SPI_InitTypeDef SPI_InitStructure;


#endif
