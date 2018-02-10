#include "log.h"

#define pbclk		5529600
#define desired_rate	9600

/**********************************************************************************************
 * *函数名 字: UART1Config
 * *功能描 述:串口1硬件驱动配置
 * *调用模块: 使用内部8M FRC作为时钟源+  8分频 =  1 M 外设时钟
 *     里的串口驱动部分
 ***********************************************************************************************/
void UART1Config( void )
{
    ANSELA =0;
    TRISBbits.TRISB4 = 0;//TX
    TRISAbits.TRISA4 = 1;//RX
    
    PPSInput(3,U1RX, RPA4);
    PPSOutput(1,RPB4,U1TX);

	/* 配置和使能UART1, 关闭CTS, RTS数据流信号，只启用TX,RX */
	UARTConfigure( UART1, UART_ENABLE_PINS_TX_RX_ONLY );
	/*
	 * 设置FIFO中断方式
	 * TX FIFO缓冲区未填充满时中断-- 缓冲区未填满。
	 * RX缓冲器非空时中断-- 缓冲区有数据。
	 */
	UARTSetFifoMode( UART1,
			 UART_INTERRUPT_ON_TX_NOT_FULL
			 | UART_INTERRUPT_ON_RX_NOT_EMPTY );
	/* 设置为数据位8，无校验位，1位停止位 */
	UARTSetLineControl( UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1 );
	/* 设置波特率，后面两个参数分别是PBCLK和期望的波特率 */
	UARTSetDataRate( UART1, pbclk, desired_rate );



/* 配置UART1接收中断 */
	INTEnable( INT_SOURCE_UART_RX( UART1 ), INT_ENABLED );
	INTSetVectorPriority( INT_VECTOR_UART( UART1 ), INT_PRIORITY_LEVEL_2 );
	INTSetVectorSubPriority( INT_VECTOR_UART( UART1 ), INT_SUB_PRIORITY_LEVEL_0 );

    /* 使能UART模块，使能发送，使能接收 */
    UARTEnable( UART1, UART_ENABLE_FLAGS( UART_PERIPHERAL | UART_RX | UART_TX ) );
        
}

/**********************************************************************************************
 * *函数名 字: WriteString(const char *string)
 * *功能描 述: UART1数据流发送函数
 * *输      入: string -- 需要发送的字符串
 ***********************************************************************************************/
void WriteString( const char *string )
{
	while ( *string != '\0' )
	{
		while ( !UARTTransmitterIsReady( UART1 ) );
		UARTSendDataByte( UART1, *string );
		string++;
		while ( !UARTTransmissionHasCompleted( UART1 ) );
	}
}

/**********************************************************************************************
 * *函数名 字: PutCharacter(const char character)
 * *功能描 述: UART1发送单字节字符函数
 ***********************************************************************************************/

void PutCharacter( const char character )
{
	while ( !UARTTransmitterIsReady( UART1 ) );
	UARTSendDataByte( UART1, character );
	while ( !UARTTransmissionHasCompleted( UART1 ) );
}
#include "led.h"
void testhandler(void)
{
    static u8 flag = 0;
    if(flag)
    {
        flag = 0;
        led_state(ON);
    }
    else
    {
        flag = 1;
        led_state(OFF);
    }
    
}

/* Specify Interrupt Priority Level = 2, Vector 49 */
void __ISR( _UART_1_VECTOR, ipl2 ) _UART1_TX_handle( void )
{
	unsigned char Receivedata;
/* Is this an RX interrupt? */
	if ( INTGetFlag( INT_SOURCE_UART_RX( UART1 ) ) )
	{
/* 发送接受到的数据，用于验证通信是否正确 */
		PutCharacter( UARTGetDataByte( UART1 ) );
/* 清除接收中断标志 */
		INTClearFlag( INT_SOURCE_UART_RX( UART1 ) );
/* 点亮接收数据指示灯，用以表示接收数据成功 */
		testhandler();
	}
/* 我们不关心发送中断 */
	if ( INTGetFlag( INT_SOURCE_UART_TX( UART1 ) ) )
	{
		INTClearFlag( INT_SOURCE_UART_TX( UART1 ) );
	}
}
