
#include "main.h"

void CEC_CAN_IRQHandler (void)
{
	uint8_t tmp;

	if (CAN_GetITStatus(CAN, CAN_IT_FMP0)) {
		CAN_ClearITPendingBit(CAN, CAN_IT_FMP0); //сброс флага

		CanRxMsg msg_buf;
		CAN_Receive(CAN, CAN_FIFO0, &msg_buf);
		can_on = 1;

		switch (msg_buf.FMI) {
		case 0: //0x011

			rpm=msg_buf.Data[1]<<8;		//обороты
			rpm |= msg_buf.Data[0];

			if(buttons & 0x10){			//если активен сброс
				if(rpm < idle + 400) buttons &= ~0x10;		//если обороты хх выключить кнопку
			}

			if (power_up) {
				temper = msg_buf.Data[2]; //перегрев
				if (temper != 0xFF) {
					if ((temper > 149) && (power_up)) set_led_temp;
					else reset_led_temp;
				} else {
					if ((msg_buf.Data[4] & 0x3) == 0x1) set_led_temp; //перегрев
					if ((msg_buf.Data[4] & 0x3) == 0) reset_led_temp;
				}

				oil = msg_buf.Data[3]; //давление
				if (oil != 0xFF) {
					if ((oil < 32) && (power_up)) set_led_oil;
					else reset_led_oil;
				} else {
					if ((msg_buf.Data[4] & 0xC) == 0x4) set_led_oil; //давление
					if ((msg_buf.Data[4] & 0xC) == 0) reset_led_oil;
				}
			} else {
				reset_led_oil;
				reset_led_temp;
			}

			tmp = msg_buf.Data[4];		//зажигание
			if((tmp & 0x30) == 0x10) ignition = 1;
			if((tmp & 0x30) == 0){
				ignition = 0;
				power_up = 0;
				is_idle = 1;
				to_revs = idle;
				reset_out_power;
				buttons &= ~0x13;
				reset_led_oil;
				reset_led_temp;
			}

			if((msg_buf.Data[4] & 0xC0) == 0x40) light_mask |= 0x40;	//габариты
			if((msg_buf.Data[4] & 0xC0) == 0) light_mask &= ~0x40;

			break;
		case 1: //0x028
			tmp = 0;
			if ((msg_buf.Data[2] & 0x3) == 1) tmp |= 1; //освещение слева
			if ((msg_buf.Data[2] & 0xC) == 4) tmp |= 2; //освещение справа
			if ((tmp & 1) ^ (light_buf & 1)) { //если освещение слева переключили с кнопочной
				if (tmp & 0x1) { 	//включение освещения
					buttons |= 0x4;
				} else { 			//выключение освещения
					buttons &= ~0x4;
				}
			}
			if ((tmp & 2) ^ (light_buf & 2)) { //если освещение справа переключили с кнопочной
				if (tmp & 0x2) { 	//включение освещения
					buttons |= 0x8;
				} else { 				//выключение освещения
					buttons &= ~0x8;
				}
			}
			light_buf = tmp;

			if((msg_buf.Data[1] & 0x3) == 0x1) light_mask |= 4;	//маяк
			if((msg_buf.Data[1] & 0x3) == 0) light_mask &= ~4;

			if((msg_buf.Data[1] & 0xC) == 0x4) light_mask |= 8;	//задний прожектор
			if((msg_buf.Data[1] & 0xC) == 0) light_mask &= ~8;

			if((msg_buf.Data[0] & 0x30) == 0x10) light_mask |= 0x20;	//стробы
			if((msg_buf.Data[0] & 0x30) == 0) light_mask &= ~0x20;

			if((msg_buf.Data[2] & 0x30) == 0x10) light_mask |= 0x10;	//освещение отсеков
			if((msg_buf.Data[2] & 0x30) == 0) light_mask &= ~0x10;

			if(buttons & 4) light_mask |= 0x1; else light_mask &= ~0x1;	//освещение слева
			if(buttons & 8) light_mask |= 0x2; else light_mask &= ~0x2;	//освещение справа

			break;
		case 2:		//0x0AC
			button_mask = msg_buf.Data[0];		//конигурация кнопок
			input_config = msg_buf.Data[1];			//конфигурация входов

			tmp = msg_buf.Data[2] & 0xF;			//конфигурация выход1
			if(tmp) out1_config = 1 << tmp;
			tmp = (msg_buf.Data[2] & 0xF0) >> 4;	//конфигурация выход2
			if(tmp) out2_config = 1 << tmp;
			tmp = msg_buf.Data[3] & 0xF;			//конфигурация выход3
			if(tmp) out3_config = 1 << tmp;
			pto_address = msg_buf.Data[4];			//адрес TCM для J1939
			tmp = msg_buf.Data[5] & 0xF;			//конфигурация выход4
			if(tmp) out4_config = 1 << tmp;
			tmp = (msg_buf.Data[5] & 0xF0) >> 4;	//конфигурация выход5
			if(tmp) out5_config = 1 << tmp;

			//запрет прерывания CAN
			CAN_ITConfig(CAN, CAN_IT_FMP0, DISABLE);
			//стирание flash
			flash_erase();
			//запись
			flash_prog_all();
			//разрешение прерывания CAN
			CAN_ITConfig(CAN, CAN_IT_FMP0, ENABLE);

			break;
		}
	}
}

void Init_CAN(){
	/* CAN register init */
	CAN_DeInit(CAN);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = ENABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

	/* CAN Baudrate = 250 kBps (CAN clocked at 48 MHz) */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;	//1+8+7=16
	CAN_InitStructure.CAN_Prescaler = 12;		//48MHz / 16 / 12 == 250k
	CAN_Init(CAN, &CAN_InitStructure);

	CAN_ITConfig(CAN, CAN_IT_FMP0, ENABLE);

	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x011 << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0ff << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 1;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x028 << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0ff << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 2;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0AC << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0ff << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);


	//вектор
	NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);



}
