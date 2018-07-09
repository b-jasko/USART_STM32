/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"

void send_char(char c) {
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, c);
}

void send_string(const char* s) {
	while(*s) {
		send_char(*s++);
	}
}

int main(void)
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE); //podlaczenie zagara
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //modul funkcji alternatywnych
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	//wyjscia PC0-PC9
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP; //wyjscie push-pull
	GPIO_Init(GPIOC, &gpio);

	//TX (wyjscie PA2) i RX (wejscie PA3)
	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);

	USART_StructInit(&uart);
	uart.USART_BaudRate = 115200;
	USART_Init(USART2, &uart);

	USART_Cmd(USART2, ENABLE);

	while(1) {
		if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE)) {
			char c = USART_ReceiveData(USART2);
			switch(c) {
			case 'a':
				send_string("Odebrano komunikat A\r\n");
				break;
			case 'b':
				send_string("Odebrano komunikat B\r\n");
				break;
			default:
				send_string("Nieznany komunikat\r\n");
				break;
			}
		}
	}
}
