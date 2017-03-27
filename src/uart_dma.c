#include "BlueNRG1.h"
#include "uart_dma.h"
#include "BlueNRG1_conf.h"
#include "SDK_EVAL_Config.h"

#define DMA_CH_UART_TX                  DMA_CH1
#define DMA_FLAG_TC_UART_TX             DMA_FLAG_TC1

#define DMA_CH_UART_RX                  DMA_CH0
#define DMA_FLAG_TC_UART_RX             DMA_FLAG_TC0

#define UART_BUFFER_SIZE        		10

extern struct fifo_t fifo_recv;
extern struct fifo_t fifo_send;
static uint8_t tx_buf[UART_BUFFER_SIZE];
static uint8_t rx_buf[UART_BUFFER_SIZE];

void uart_dma_init(void)
{
    DMA_InitType DMA_InitStructure;
    NVIC_InitType NVIC_InitStructure;
  
    SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_DMA, ENABLE);
  
	/* DMA_CH_UART_TX Initialization */
    DMA_InitStructure.DMA_PeripheralBaseAddr = UART_BASE;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)tx_buf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = (uint32_t)UART_BUFFER_SIZE;  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
    DMA_Init(DMA_CH_UART_TX, &DMA_InitStructure);

    /* Enable DMA_CH_UART_TX Transfer Complete interrupt */
    DMA_FlagConfig(DMA_CH_UART_TX, DMA_FLAG_TC, ENABLE);

    /* DMA_CH_UART_RX Initialization */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)rx_buf;
    DMA_InitStructure.DMA_BufferSize = (uint32_t)UART_BUFFER_SIZE;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_Init(DMA_CH_UART_RX, &DMA_InitStructure);
    
    /* Enable DMA_CH_UART_RX Transfer Complete interrupt */  
    DMA_FlagConfig(DMA_CH_UART_RX, DMA_FLAG_TC, ENABLE);
    
    /* Enable USART_DMAReq_Tx */
	UART_DMACmd(UART_DMAReq_Tx | UART_DMAReq_Rx, ENABLE);

    /* Enable the DMA Interrupt request */
    NVIC_InitStructure.NVIC_IRQChannel = DMA_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = HIGH_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#if 1
    //uint16_t len = sprintf(recv_buf, "Demo start...\r\n");
    //DMA_CH_UART_TX->CNDTR = len;
    //DMA_CH_UART_TX->CCR_b.EN = SET;
    DMA_CH_UART_RX->CNDTR = UART_BUFFER_SIZE;
    DMA_CH_UART_RX->CCR_b.EN = SET;
#endif
}

void uart_dma_irq(void)
{
    if(DMA_GetFlagStatus(DMA_FLAG_TC_UART_TX))
    {
        uint16_t count = 0;
        DMA_ClearFlag(DMA_FLAG_TC_UART_TX);
        DMA_CH_UART_TX->CCR_b.EN = RESET;

        while(!fifo_empty(&fifo_recv))
        {
			rx_buf[count++] = fifo_delete(&fifo_recv);
		}

		if(!count)
		{
			DMA_CH_UART_TX->CNDTR = count;
			DMA_CH_UART_TX->CCR_b.EN = SET;
		}
	}

	if(DMA_GetFlagStatus(DMA_FLAG_TC_UART_RX))
	{
		uint16_t count = 0;
		DMA_ClearFlag(DMA_FLAG_TC_UART_RX);

		DMA_CH_UART_RX->CCR_b.EN = RESET;
		DMA_CH_UART_RX->CNDTR = UART_BUFFER_SIZE;

		do
        {
			if(fifo_full(&fifo_send))
			{
				return;
			}

			fifo_insert(&fifo_send, rx_buf[count]);
		}
		while(count++ < UART_BUFFER_SIZE);

		DMA_CH_UART_RX->CCR_b.EN = SET;
	}
}
