
#include "main.h"

uint8_t is_idle, left, right, is_stop, power_up, morg, pto_address, can_on, can_cnt;
uint8_t w_morg, f_morg;		//8 бит - рост шкалы
							//значение без 8 бита - спад шкалы
uint16_t to_revs, rpm, rpm_buf;
uint8_t oil, temper, light_buf, ignition,
inputs;	//0 - роллета
		//1 - ступень
uint8_t blink_cnt, blink;
uint16_t butt;	//0 - сцепление
				//1 - питание
				//2 - осв слева
				//3 -
				//4 - осв справа
				//5 - обор+
				//6 - обор-
				//7 - сброс
				//8 - стоп
				//9 - пуск

uint8_t pwm_count;		//счетчик до 8 для ШИМ

uint8_t buttons;	//0 - питание
					//1 - сцепление
					//2 - осв лев
					//3 - осв прав
					//4 - сброс
					//5 - сигнал на проверку калибровки
					//6 - моргалка
					//7 -
uint8_t buttons_out;	//0 - ПУСК
						//1 - СТОП
						//2 - обор+
						//3 - обор-
uint8_t button_mask;
uint8_t light_mask;		//0 - осв слева
						//1 - осв справа
						//2 - маяк
						//3 - задний прожектор
						//4 - осв отсеков
						//5 - строб
						//6 - габариты
						//7 -

uint8_t input_config,	//0 - вх1 роллета
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

uint8_t butt_b[5],		//буфер ШИМ кнопок
		analog_b[2],	//буфер ШИМ нижних сегментов
		ind_b[16];		//буфер ШИМ индикаторов
uint8_t rev_p, rev_m;


levels water, foam;

CanTxMsg TxMessage;
GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
CAN_InitTypeDef CAN_InitStructure;
CAN_FilterInitTypeDef CAN_FilterInitStructure;
SPI_InitTypeDef SPI_InitStructure;


int main(void)
{
	uint32_t temp;

	//Init_IWDG();			//

	//начальные установки
	water.min = 40;
	foam.min = 40;
	water.max = 200;
	foam.max = 150;	//*/
	pto_address = 0x7;
	input_config = 0;
	out1_config = 0;
	out2_config = 0;
	out3_config = 0;

	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;

	//загрузка данных из flash
	temp = (*(__IO uint32_t*) water_min_address);
	if(temp != 0xffffffff) water.min = temp;
	temp = (*(__IO uint32_t*) water_max_address);
	if(temp != 0xffffffff) water.max = temp;
	temp = (*(__IO uint32_t*) foam_min_address);
	if(temp != 0xffffffff) foam.min = temp;
	temp = (*(__IO uint32_t*) foam_max_address);
	if(temp != 0xffffffff) foam.max = temp;
	temp = (*(__IO uint32_t*) flash_pto_address);
	if(temp != 0xffffffff) pto_address = temp;
	temp = (*(__IO uint32_t*) input_config_address);
	if(temp != 0xffffffff) input_config = temp;
	temp = (*(__IO uint32_t*) out1_config_address);
	if(temp != 0xffffffff) out1_config = temp;
	temp = (*(__IO uint32_t*) out2_config_address);
	if(temp != 0xffffffff) out2_config = temp;
	temp = (*(__IO uint32_t*) out3_config_address);
	if(temp != 0xffffffff) out3_config = temp;
	temp = (*(__IO uint32_t*) button_mask_address);
	if(temp != 0xffffffff) button_mask = temp;
	temp = (*(__IO uint32_t*) out4_config_address);
	if(temp != 0xffffffff) out4_config = temp;
	temp = (*(__IO uint32_t*) out5_config_address);
	if(temp != 0xffffffff) out5_config = temp;

	//вычисление коэффициента
	if(water.max-water.min) water.koeff=koef_k/(water.max-water.min);
	else water.koeff=koef_k;
	if(foam.max-foam.min) foam.koeff=koef_k/(foam.max-foam.min);
	else foam.koeff=koef_k;

	if(water.max-water.min) water.koeff_perc=koef_perc/(water.max-water.min);
	else water.koeff_perc=koef_perc;
	if(foam.max-foam.min) foam.koeff_perc=koef_perc/(foam.max-foam.min);
	else foam.koeff_perc=koef_perc;

//настройка для АЦ10
	input_config |= 9;
	button_mask = 0xFF;
	out1_config |= bit_beam;
	out2_config |= bit_rear;
	out3_config |= bit_inside;
	out4_config |= bit_strobe;
	out5_config |= bit_tail;
	//out5_config |= bit_beam;

	Init_RCC();			//тактирование блоков
	Init_GPIO();		//инициализация портов
	Init_SPI();			//SPI для индикаторов
	Init_ADC();			//инициализация АЦП
	Init_CAN();			//инициализация CAN
	Init_Timer();		//инициализация таймеров

	to_revs=idle;
	is_idle=1;

	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.StdId = 0x001; //
	TxMessage.DLC = 1;
	TxMessage.Data[0] = pto_address;
	CAN_Transmit(CAN, &TxMessage);
	if (CAN_GetLastErrorCode(CAN)) { //ошибка отправки
		//GPIO_SetBits(GPIOA, GPIO_Pin_9);
		//GPIOB->ODR ^= 0x80;
	}


    while(1)
    {

    }
}

//стирание flash
void flash_erase(void){

	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = water_min_address;
    FLASH->CR |= FLASH_CR_STRT;
    while (FLASH->SR & FLASH_SR_BSY);
    FLASH->SR = FLASH_SR_EOP;
    FLASH->CR &= ~FLASH_CR_PER;
}

//запись данных во flash
void flash_prog_all(void){

	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}
	FLASH->CR |= FLASH_CR_PG;
	*(__IO uint16_t*)water_min_address = water.min;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	*(__IO uint16_t*)water_max_address = water.max;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	*(__IO uint16_t*)foam_min_address = foam.min;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	*(__IO uint16_t*)foam_max_address = foam.max;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	*(__IO uint16_t*)flash_pto_address = pto_address;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	*(__IO uint16_t*)input_config_address = input_config;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	*(__IO uint16_t*)out1_config_address = out1_config;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	*(__IO uint16_t*)out2_config_address = out2_config;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	*(__IO uint16_t*)out3_config_address = out3_config;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	*(__IO uint16_t*)button_mask_address = button_mask;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	*(__IO uint16_t*)out4_config_address = out4_config;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	*(__IO uint16_t*)out5_config_address = out5_config;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;

	FLASH->CR &= ~(FLASH_CR_PG);
}


