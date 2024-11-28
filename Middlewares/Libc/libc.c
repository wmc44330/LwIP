#include <stdio.h>
#include "stm32f4xx.h"

#if defined(__CC_ARM)

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)

#pragma import(__use_no_semihosting)

struct __FILE
{
	int handle;
};

FILE __stdout;

void _sys_exit(int x)
{
	x = x;
}

#elif defined(__GNUC__)

int __io_putchar(int ch);
int __io_getchar(void);

__attribute__((weak)) int _write(int file, char *ptr, int len)
{
	int DataIdx;
	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		__io_putchar(*ptr++);
	}
	return len;
}
__attribute__((weak)) int _read(int file, char *ptr, int len)
{
	int DataIdx;
	for (DataIdx = 0; DataIdx < len; DataIdx++)
		*ptr++ = __io_getchar();
	return len;
}

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)

#endif

PUTCHAR_PROTOTYPE
{
	while((USART1->SR & 0X40) == RESET);
	USART1->DR = (uint8_t)ch;
	return ch;
}

GETCHAR_PROTOTYPE
{
	uint8_t ch = USART_ReceiveData(USART1);
	return ch;
}
