/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 14/10/01 5:11p $
 * @brief    Template project for NUC029xEE series MCU
 *
 * @note
 * Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

void UART_TX(UART_T* uart, int c)
{
	while(UART_GET_TX_FULL(uart));
	UART_WRITE(uart, c);	
}

void TMR3_IRQHandler(void)
{
	static uint32_t LOG = 0;
	static uint16_t CNT = 0;
	static uint16_t CNT_UART2 = 0;
//	uint8_t pu8TxBuf[] = {"pu8TxBuf test U2"};
	
    if(TIMER_GetIntFlag(TIMER3) == 1)
    {
        TIMER_ClearIntFlag(TIMER3);

		if (CNT++ >= 250)
		{	
			PB4 ^= 1;
			CNT = 0;
//        	printf("%s : %4d\r\n",__FUNCTION__,LOG++);
			LOG = (LOG >= 15) ? (0) : (LOG+1);
//        	printf("%x\r\n", LOG );

//		    UART_TX(UART2, 0x40);
//		    UART_TX(UART2, '\r');
//		    UART_TX(UART2, '\n');			

		}	

		if (CNT_UART2++ >= 500)
		{		
			CNT_UART2 = 0;

//			UART_Write(UART2, pu8TxBuf, sizeof(pu8TxBuf));
			printf("UARTx\r\n");

		    UART_TX(UART0, 0x42);
		    UART_TX(UART0, '\r');
		    UART_TX(UART0, '\n');

		    UART_TX(UART1, 0x41);
		    UART_TX(UART1, '\r');
		    UART_TX(UART1, '\n');

		    UART_TX(UART2, 0x40);
		    UART_TX(UART2, '\r');
		    UART_TX(UART2, '\n');			
		}			
    }
}

void TIMER3_Init(void)
{
    TIMER_Open(TIMER3, TIMER_PERIODIC_MODE, 1000);
    TIMER_EnableInt(TIMER3);
    NVIC_EnableIRQ(TMR3_IRQn);	
    TIMER_Start(TIMER3);
}

void UART1_IRQHandler(void)
{
	//UART1
	if(UART_GET_INT_FLAG(UART1, UART_ISR_RDA_INT_Msk))    
	{
		while(!UART_GET_RX_EMPTY(UART1))
		{
			printf("UART1 : 0x%2X\r\n" , UART_READ(UART1));
		}		
	}
}

void UART02_IRQHandler(void)
{       	
	//UART0
	if(UART_GET_INT_FLAG(UART0, UART_ISR_RDA_INT_Msk))    
	{
		while(!UART_GET_RX_EMPTY(UART0))
		{
			printf("UART0 : 0x%2X\r\n" , UART_READ(UART0));
		}		
	}

	//UART2
	if(UART_GET_INT_FLAG(UART2, UART_ISR_RDA_INT_Msk))    
	{
		while(!UART_GET_RX_EMPTY(UART2))
		{
			printf("UART2 : 0x%2X\r\n" , UART_READ(UART2));			
		}
	}

}


void UART2_Init(void)
{
    SYS_ResetModule(UART2_RST);
	
    UART_Open(UART2, 115200);

	UART_SetTimeoutCnt(UART2, 20);

	UART2->FCR = UART_FCR_TFR_Msk | UART_FCR_RFR_Msk;

	UART_ENABLE_INT(UART0, (UART_IER_RDA_IEN_Msk));	//| UART_IER_THRE_IEN_Msk | UART_IER_TOUT_IEN_Msk)
	UART_ENABLE_INT(UART1, (UART_IER_RDA_IEN_Msk));	//| UART_IER_THRE_IEN_Msk | UART_IER_TOUT_IEN_Msk)	
	UART_ENABLE_INT(UART2, (UART_IER_RDA_IEN_Msk));	//| UART_IER_THRE_IEN_Msk | UART_IER_TOUT_IEN_Msk)
	
	NVIC_EnableIRQ(UART02_IRQn);	

	UART_WAIT_TX_EMPTY(UART2);	
}

void UART1_Init(void)
{
    SYS_ResetModule(UART1_RST);
	
    UART_Open(UART1, 115200);

	UART_SetTimeoutCnt(UART1, 20);

	UART1->FCR = UART_FCR_TFR_Msk | UART_FCR_RFR_Msk;

	UART_WAIT_TX_EMPTY(UART1);	
}

void UART0_Init(void)
{
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);

	/* Set UART receive time-out */
	UART_SetTimeoutCnt(UART0, 20);

	/* Set UART FIFO RX interrupt trigger level to 4-bytes*/
//    UART0->FCR = ((UART0->FIFO & (~UART_FCR_RFITL_Msk)) | UART_FCR_RFITL_4BYTES);
	UART0->FCR = UART_FCR_TFR_Msk | UART_FCR_RFR_Msk;
	
//	/* Enable UART Interrupt - */
//	UART_ENABLE_INT(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_TOUT_IEN_Msk));
//	
//	NVIC_EnableIRQ(UART02_IRQn);	

	printf("\r\nCLK_GetCPUFreq : %8d\r\n",CLK_GetCPUFreq());
	printf("CLK_GetHCLKFreq : %8d\r\n",CLK_GetHCLKFreq());	
	printf("CLK_GetPCLK0Freq : %8d\r\n",CLK_GetHXTFreq());
	printf("CLK_GetPCLK1Freq : %8d\r\n",CLK_GetPCLKFreq());

	UART_WAIT_TX_EMPTY(UART0);
}


void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    //CLK->PWRCON = (CLK->PWRCON & ~(0x0000000Dul)) | 0x0000001Dul;
    //CLK->PLLCON = (CLK->PLLCON & ~(0x000FFFFFul)) | 0x0000C22Eul;
    //CLK->CLKDIV = (CLK->CLKDIV & ~(0x00FF0F0Ful)) | 0x00000000ul;
    //CLK->CLKSEL0 = (CLK->CLKSEL0 & ~(0x0000003Ful)) | 0x0000003Aul;
    //CLK->CLKSEL1 = (CLK->CLKSEL1 & ~(0xF377773Ful)) | 0xCC8FFFFFul;
    //CLK->CLKSEL2 = (CLK->CLKSEL2 & ~(0x000300FCul)) | 0x000200FFul;
    //CLK->AHBCLK = (CLK->AHBCLK & ~(0x0000001Cul)) | 0x00000001ul;
    //CLK->APBCLK = (CLK->APBCLK & ~(0xD0F3337Dul)) | 0x00110020ul;
    //CLK->FRQDIV = (CLK->FRQDIV & ~(0x0000003Ful)) | 0x00000000ul;
    //SysTick->CTRL = (SysTick->CTRL & ~(0x00000005ul)) | 0x00000000ul;

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable clock source */
    CLK_EnableXtalRC(CLK_PWRCON_OSC10K_EN_Msk|CLK_PWRCON_OSC22M_EN_Msk|CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for clock source ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC10K_STB_Msk|CLK_CLKSTATUS_OSC22M_STB_Msk|CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Disable PLL first to avoid unstable when setting PLL */
    CLK_DisablePLL();

    /* Set PLL frequency */
    CLK->PLLCON = (CLK->PLLCON & ~(0x000FFFFFul)) | 0x0000C22Eul;

    /* Waiting for PLL ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_PLL_STB_Msk);

    /* If the defines do not exist in your project, please refer to the related clk.h in the clk_h folder appended to the tool package. */
    /* Set HCLK clock */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_PLL, CLK_CLKDIV_HCLK(1));

    /* Enable IP clock */
    CLK_EnableModuleClock(TMR3_MODULE);
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(UART1_MODULE);	
    CLK_EnableModuleClock(UART2_MODULE);
	
    /* Set IP clock */
    CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3_S_HXT, MODULE_NoMsk);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HIRC, CLK_CLKDIV_UART(1));
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HIRC, CLK_CLKDIV_UART(1));	
    CLK_SetModuleClock(UART2_MODULE, CLK_CLKSEL1_UART_S_HIRC, CLK_CLKDIV_UART(1));

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB0_Msk | SYS_GPB_MFP_PB1_Msk);
    SYS->GPB_MFP |= (SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD);

    SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB5_Msk);
    SYS->GPB_MFP |= (SYS_GPB_MFP_PB5_UART1_TXD);

 	SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB9_Msk | SYS_GPB_MFP_PB10_Msk );
	SYS->GPB_MFP |= (SYS_GPB_MFP_PB9_UART2_TXD | SYS_GPB_MFP_PB10_UART2_RXD);
	SYS->ALT_MFP = SYS_ALT_MFP_PB9_UART2_TXD|SYS_ALT_MFP_PB10_UART2_RXD;
	
    /* Lock protected registers */
    SYS_LockReg();

    return;
}


int main()
{

	SYS_Init();

	GPIO_SetMode(PB, BIT4, GPIO_PMD_OUTPUT);

	UART0_Init();
	UART1_Init();	
	UART2_Init();

	TIMER3_Init();
	
    while(1)
    {
//		    while(UART_GET_TX_FULL(UART2));
//		    UART_WRITE(UART2, 0x40);

//		    while(UART_GET_TX_FULL(UART2));
//		    UART_WRITE(UART2, '\r');
//			
//		    while(UART_GET_TX_FULL(UART2));
//		    UART_WRITE(UART2, '\n');
    }
	
}

/*** (C) COPYRIGHT 2018 Nuvoton Technology Corp. ***/
