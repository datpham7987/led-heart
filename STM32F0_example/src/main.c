

#include "stm32f0xx.h"
#include"stm32f0xx_gpio.h"
#include"stm32f0xx_rcc.h"
#include"stm32f0xx_exti.h"
#include"stm32f0xx_misc.h"
#include"stm32f0xx_syscfg.h"
#include"stm32f0xx_usart.h"
#include"stm32f0xx_spi.h"
#include "stm32f0xx_tim.h"
/*
 *
 */
USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;
EXTI_InitTypeDef  EXTI_InitStructure;
NVIC_InitTypeDef  NVIC_InitStructure;
SPI_InitTypeDef  SPI_InitStruct;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
/*
 *
 */
#define LED GPIO_Pin_4
#define SDI GPIO_Pin_1
#define SCK GPIO_Pin_2
#define LATCH GPIO_Pin_3
/*
 *
 */
uint16_t x = 0x01;
uint16_t y = 0x00;
uint32_t delay = 0;

void Testled(uint32_t timer);
void HC595_Latch(void);
void Shift_Out(uint8_t Data);
void HC595_Pulse(void);
void Led_1(void);
void TIM3_IRQHandler(void);
int main(void)
{
//	/*
//	 * ================================================================SYSTEM_CONFIG_CLOCK_HSI================================================
//	 */
//	SystemInit();
//	RCC_HSEConfig(RCC_HSE_OFF);
//	RCC_HSICmd(ENABLE);
//	RCC_PLLCmd(DISABLE);
//	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	/*
	 * 	==========================================SETUP GPIO================================
	*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED|SDI|SCK|LATCH;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,LED|SDI|SCK|LATCH);
	/*
	 * ==========================================SETUP TIMER3==============================
	 * ===================================================================================
	 */
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 4800;
	TIM_TimeBaseStructure.TIM_Period = 3333;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);  // clear update flag
	TIM_ITConfig( TIM3, TIM_IT_Update, ENABLE); // enable Update Interrupt
	TIM_Cmd(TIM3, ENABLE); //enable timer 4
	 /*Configure Interrupt request for TIM4*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	while(1)
	{
		Shift_Out(x);
		Shift_Out(y);
		HC595_Latch();
		for(int i = 0;i < 5000000; i++)
		{
		}
		x = x << 1;
		if(x > 0x80)
		{
			x = 0x01;
		}
	}
}

/*
 * ==============================================Make Delay by interrupt timer===============
 * ==========================================================================================
 */
void TIM3_IRQHandler(void)
{
	 if(TIM_GetITStatus( TIM3,TIM_IT_Update)!=RESET) //if update flag turns on
	 {
		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //clear update flag
		 GPIO_WriteBit(GPIOA,LED,(BitAction)(1^GPIO_ReadInputDataBit(GPIOA,LED))); //toggle PB9
//		 delay  = 1;

	 }
}

/*
 *===============================================HIEU UNG 1 =================================
 *===========================================================================================
 */
void Led_1(void)
{
	x = 0x01;
	y = 0x00;
	for(int i = 0 ;i < 8 ; i++)
	{
		Shift_Out(x);
		Shift_Out(y);
		HC595_Latch();
		for(int i = 0;i < 5000000; i++)
		{
		}
		x = x << 1;
	}
	x = 0x00;
	y = 0x01;
	for(int i = 0 ;i < 8 ; i++)
	{
		Shift_Out(x);
		Shift_Out(y);
		HC595_Latch();
		for(int i = 0;i < 5000000; i++)
		{
		}
		y = y << 1;
	}
}
/*
 *===============================================HIEU UNG 2 =================================
 *===========================================================================================
 */
void Led_2(void)
{
	x = 0x00;
	y = 0x80;
	for(int i = 0 ;i < 8 ; i++)
	{
		Shift_Out(x);
		Shift_Out(y);
		HC595_Latch();
		for(int i = 0;i < 5000000; i++)
		{
		}
		y = y >> 1;
	}
	x = 0x80;
	y = 0x00;
	for(int i = 0 ;i < 8 ; i++)
	{
		Shift_Out(x);
		Shift_Out(y);
		HC595_Latch();
		for(int i = 0;i < 5000000; i++)
		{
		}
		x = x >> 1;
	}
}
/*
 *===============================================HIEU UNG 3 =================================
 *===========================================================================================
 */
void Led_3(void)
{
	x = 0x01;
	y = 0x80;
	for(int i = 0; i < 8; i++)
	{
		Shift_Out(x);
		Shift_Out(y);
		HC595_Latch();
		for(int i = 0;i < 5000000; i++)
		{
		}
		x = x << 1;
		y = y >> 1;
	}
	x = 0x80;
	y = 0x01;
	for(int i = 0; i < 8; i++)
	{
		Shift_Out(x);
		Shift_Out(y);
		HC595_Latch();
		for(int i = 0;i < 5000000; i++)
		{
		}
		x = x >> 1;
		y = y << 1;
	}
}
/*
 *===============================================HIEU UNG 4 =================================
 *===========================================================================================
 */
void Led_4(void)
{
	x = 0x80;
	uint16_t a = 0x00;
	y = 0x80;
	uint16_t b = 0x00;
	for(int i = 0; i < 8; i++)
	{
		x = 0x80;
		y = 0x80;
		for(int j = 0; j < 8 - i;j++)
		{
			x = x | a;
			y = y | b;
			Shift_Out(x);
			Shift_Out(y);
			HC595_Latch();
			for(int i = 0;i < 5000000; i++)
			{
			}
			x = x >> 1;
			y = y >> 1;
		}
		a = a >> 1;
		a = a |0x01;
		a = b >> 1;
		a = b |0x01;

	}
}
/*
 * ======================================================HC595===============================================
 * ==========================================================================================================
 * ==========================================================================================================
 */
void Shift_Out(uint8_t Data)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		if(Data & 0x80)
		{
			GPIO_SetBits(GPIOA,SDI); // Data High
		}
		else
		{
			GPIO_ResetBits(GPIOA,SDI); // Data Low
		}
		HC595_Pulse();
		Data = Data << 1;
	}
}

void HC595_Pulse(void)
{

	GPIO_SetBits(GPIOA,SCK); // Clock High

	GPIO_ResetBits(GPIOA,SCK); // Clock Low
}
void HC595_Latch(void)
{
	  GPIO_SetBits(GPIOA,LATCH); // Latch High
	  GPIO_ResetBits(GPIOA,LATCH); // Latch Low
}
