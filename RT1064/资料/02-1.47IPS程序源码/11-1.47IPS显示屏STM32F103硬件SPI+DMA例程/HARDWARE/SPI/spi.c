#include "spi.h"



void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIOA
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;//只发送模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设置SPI工作模式：主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//设置SPI数据大小：8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//串行同步时钟空闲时SCLK位高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//串行同步时钟空第二个时钟沿捕获
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS信号由硬件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//波特率预分频值：波特率预分频值为4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输高位先行
	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC值计算的多项式
	SPI_Init(SPI1,&SPI_InitStructure);//初始化SPI
	SPI_Cmd(SPI1, ENABLE);//使能SPI
}






