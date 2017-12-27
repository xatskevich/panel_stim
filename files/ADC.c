
#include "main.h"


void ADC1_IRQHandler(void)
{
	int16_t analog;
	int32_t temp;

	if(ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET){
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);		//сброс флага
	}
	
		switch (ADC1->CHSELR) {
		case 0x2: //датчик цистерны

				water.analog += ADC1->DR; //чтение данных воды
				if (++water.count == 4) { //среднее 8-ми значений
					water.count = 0;
					analog = water.analog / 4;
					water.analog = 0;
					water.input = (water.input * 4 - water.input + analog) / 4; //новое значение
					temp = (water.input - water.min) * water.koeff / 1024;
					if (temp < 0) temp = 0;
					if (temp > 8) temp = 8;
					if (ADC1->DR > 16) { //если датчик подключен, считаем
						water.out = temp;
						water.percent=(water.input-water.min)*water.koeff_perc/256;
						if(water.percent < 0) water.percent = 0;
						if(water.percent > 100) water.percent = 100;
					} else { //без датчиков моргаем шкалами
						if(blink) water.out = 8;
						else water.out = 0;
					}
				}
			break;
		case 0x1: //датчик пенобака

				foam.analog += ADC1->DR; //чтение данных пены
				if (++foam.count == 4) { //среднее 8-ми значений
					foam.count = 0;
					analog = foam.analog / 4;
					foam.analog = 0;
					foam.input = (foam.input * 4 - foam.input + analog) / 4; //новое значение
					temp = (foam.input - foam.min) * foam.koeff / 1024; //вычисление значения ШИМ
					if (temp < 0) temp = 0;
					if (temp > 8) temp = 8;
					if (ADC1->DR > 16) { //если датчик подключен, считаем
						foam.out = temp;
						foam.percent=(foam.input-foam.min)*foam.koeff_perc/256;
						if(foam.percent < 0) foam.percent = 0;
						if(foam.percent > 100) foam.percent = 100;
					} else { //без датчиков моргаем шкалами
						if(blink) foam.out = 8;
						else foam.out = 0;
					}
				}
			break;
		}
}


void Init_ADC(){
	//параметры АЦП по умолчанию
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
	ADC_Init(ADC1, &ADC_InitStructure);

	//каналы воды пены оборотов
	ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_239_5Cycles);	//для тока 0 канал
	ADC_GetCalibrationFactor(ADC1);
	//прерывания
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	ADC_Cmd(ADC1, ENABLE);
	// ADRDY flag
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));
	
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_COMP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02; //!!!!!!!!!!!!!!!
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	ADC_StartOfConversion(ADC1);
}
