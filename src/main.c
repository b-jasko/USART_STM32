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
#include <string.h>

volatile char command[5] = "";

void send_char(char c) {
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, c);
}

void send_string(const char* s) {
	while(*s) {
		send_char(*s++);
	}
}

//printf calls it at the end
int __io_putchar(int c)
{
    if (c=='\n')
        send_char('\r');
    send_char(c);
    return c;
}

void receive_led_command() {
	static int i = 0;
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE)) {
		char c = USART_ReceiveData(USART2);
		command[i] = c;
		i++;
		if (c == '\r') {
			if (strcmp(command, "on\r") == 0) {
				send_string("LED ON\r\n");
				GPIO_SetBits(GPIOC, 1<<0);
			} else if (strcmp(command, "off\r") == 0) {
				send_string("LED OFF\r\n");
				GPIO_ResetBits(GPIOC, 1<<0);
			} else {
				send_string("Nieznany komunikat\r\n");
			}
			i = 0;
			memset(command, 0, 5);
		}
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
		printf("Hello world!\n");
	}
}
