


#include "stm32f10x.h"
#include "string.h"

void Uart2Config (void)
{
	/******* STEPS FOLLOWED ********
	
	1. Enable the UART CLOCK and GPIO CLOCK
	2. Configure the UART PINs for ALternate Functions
	3. Enable the USART by writing the UE bit in USART_CR1 register to 1.
	4. Program the M bit in USART_CR1 to define the word length.
	5. Enable DMA for Transmit
	6. Select the desired baud rate using the USART_BRR register.
	7. Enable the Transmitter/Receiver by Setting the TE and RE bits in USART_CR1 Register

	********************************/	
	
	
	RCC->APB1ENR |= (1<<17);   // Enable USART2 Clock
	RCC->APB2ENR |= 1<<2;  // Enable GPIOA CLOCK
	
	GPIOA->CRL = 0;
	GPIOA->CRL |= (3<<8);   // output mode 50 MHz for PA2
	GPIOA->CRL |= (2<<10);  // Alternate Func Push Pull For PA2
	
	GPIOA->CRL &= ~(3<<12);   // Intput Mode For PA3
	GPIOA->CRL |= (2<<14);  // Input Pull Up/ Down For PA3	
	
	GPIOA->ODR |= 1<<3;  // Pull Up for PA3
	
	
	USART2->CR1 = 0x00;   // Clear ALL
	USART2->CR1 |= (1<<13);   // UE = 1... Enable USART
	
//	USART2->CR3 |= (1<<7);  // Enable DMA for Transmit
	USART2->CR3 |= (1<<6);  // Enable DMA for Receive
	
	/* 
	
		Tx/ Rx baud = PCLK /(16*USARTDIV) 
		USARTDIV = 19.53125
		MENSTISSA = 19
		FRACTION = 16*0.53125 = 8.5
	
	*/

	USART2->BRR = (8<<0) | (19<<4);   // Baud rate of 115200, PCLK1 at 36MHz
	
	USART2->CR1 |= (1<<2); // RE=1.. Enable the Receiver
	USART2->CR1 |= (1<<3);  // TE=1.. Enable Transmitter
}


void DMA_Init (void)
{
	
	// 1. Enable DMA1 Clock
	RCC->AHBENR |= 1<<0;
	
	// 2. Enable DMA Interrupts
	DMA1_Channel6->CCR |= (1<<1)|(1<<2)|(1<<3);  // TCIE, HTIE, TEIE Enabled
	
	// 3. Set the Data Direction
//	DMA1_Channel7->CCR |= (1<<4);   // Read From Memory
	DMA1_Channel6->CCR &= ~(1<<4);   // Read From Peripheral
	
	// 4. Enable the circular mode (CIRC)
	DMA1_Channel6->CCR |= 1<<5;
	
	// 5. Enable the Memory Increment (MINC)
	DMA1_Channel6->CCR |= 1<<7;
	
	// 6. Set the Peripheral data size (PSIZE)
	DMA1_Channel6->CCR &= ~(3<<8);  // 00 : 8 Bit Data
	
	// 7. Set the Memory data size (MSIZE)
	DMA1_Channel6->CCR &= ~(3<<10);  // 00 : 8 Bit Data
	
	// 8. Set the Priority Level
	DMA1_Channel6->CCR &= ~(3<<12);  // PL = 0
}


void DMA_Config (uint32_t srcAdd, uint32_t destAdd, uint16_t datasize)
{
	
	// 1. Set the data size in CNDTR Register
	DMA1_Channel6->CNDTR = datasize;
	
	// 2. Set the  peripheral address in PAR Register
	DMA1_Channel6->CPAR = srcAdd;
	
	// 3. Set the  Memory address in MAR Register
	DMA1_Channel6->CMAR = destAdd;
	
	// 4. Enable the DMA1
	DMA1_Channel6->CCR |= 1<<0;
	
}


#define RXSIZE 20
uint8_t RxBuf[20];
uint8_t MainBuf [50];

uint8_t indx = 0;

void DMA1_Channel6_IRQHandler (void)
{
	if ((DMA1->ISR)&(1<<22))  // If the Half Transfer Complete Interrupt is set
	{
		memcpy (&MainBuf[indx], &RxBuf[0], RXSIZE/2);
		DMA1->IFCR |= (1<<22);
		indx = indx+(RXSIZE/2);
		if (indx>49) indx=0;
	}
	
	if ((DMA1->ISR)&(1<<21))  // If the Transfer Complete Interrupt is set
	{
		memcpy (&MainBuf[indx], &RxBuf[RXSIZE/2], RXSIZE/2);
		DMA1->IFCR |= (1<<21);
		indx = indx+(RXSIZE/2);
		if (indx>49) indx=0;
	}
	
}


int main ()
{
	SystemInit();
	
	Uart2Config ();
	
	DMA_Init ();
	
	NVIC_SetPriority (DMA1_Channel6_IRQn, 0);
	
	NVIC_EnableIRQ (DMA1_Channel6_IRQn);
	
	DMA_Config ((uint32_t) &USART2->DR, (uint32_t) RxBuf, RXSIZE);
	
	while (1)
	{
		
	}
}
	
	
	
	
	
	


