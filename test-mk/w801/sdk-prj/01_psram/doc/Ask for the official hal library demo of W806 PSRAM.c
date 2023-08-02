http://ask.winnermicro.com/question/86.html

Ask for the official hal library demo of W806 PSRAM

Posted on 2022-04-22 09:58:49
I thought about transplanting the psram driver of tls to hal, but the dma part is a bit complicated, I don’t know what to do, has anyone transplanted it?

*******************************************


This is written by myself. You can recalculate the clock frequency division register according to the specification. I don’t use a standard 40M crystal oscillator.

void LL_QPI_Enable4bit(void)
{

LL_QPI_RegWrite(LL_QPI_REG_CTRL,(LL_QPI_RegRead(LL_QPI_REG_CTRL)|LL_QPI_QUAD_ENABLE)); //reset qpi

while((LL_QPI_RegRead(LL_QPI_REG_CTRL)&0x02u)==0); //wait 
}

void LL_QspiInit(void)
{

unsigned long regVal;
LL_GpioInitStructDef   GpioInit;
LL_QpiState = LL_Qpi_Busy;

LL_PeripheralClockEnable(CLK_QPI);
LL_GpioAfInit(Psram_Ck_Remap,LL_GPIOB,LL_GPIO_P00);
LL_GpioAfInit(Psram_Cs_Remap,LL_GPIOB,LL_GPIO_P27);
LL_GpioAfInit(Psram_D0_Remap,LL_GPIOB,LL_GPIO_P02);
LL_GpioAfInit(Psram_D1_Remap,LL_GPIOB,LL_GPIO_P03);
LL_GpioAfInit(Psram_D2_Remap,LL_GPIOB,LL_GPIO_P04);    
LL_GpioAfInit(Psram_D3_Remap,LL_GPIOB,LL_GPIO_P05);
regVal = LL_QPI_RegRead(LL_QPI_REG_CTRL);
LL_QPI_RegWrite(LL_QPI_REG_CTRL,(regVal|LL_QPI_RESET_ENABLE)); //reset qpi
while(1)
{
    regVal = LL_QPI_RegRead(LL_QPI_REG_CTRL);
    if(~(regVal&0x00000001u))
    {
        break;
    }
}
regVal = LL_QPI_RegRead(LL_QPI_REG_CTRL)&0xFFFFF000u;
LL_QPI_RegWrite(LL_QPI_REG_CTRL,regVal); // reset reg value
regVal = LL_QPI_RegRead(LL_QPI_REG_CTRL)&0xFFFFFF0Fu;
LL_QPI_RegWrite(LL_QPI_REG_CTRL,(regVal|LL_QPI_CLK_DIV)); //set clk div
regVal = LL_QPI_RegRead(LL_QPI_REG_CTRL)&0xFFFFF8FFu;
LL_QPI_RegWrite(LL_QPI_REG_CTRL,(regVal|LL_QPI_tCPH));   //set cCPH time

regVal = LL_QPI_RegRead(LL_QPI_REG_CTRL);
LL_QPI_RegWrite(LL_QPI_REG_CTRL,(regVal|LL_QPI_BURST_ENABLE));   //set burst enable    

regVal = LL_QPI_RegRead(LL_QPI_REG_OVTR)&0xFFFFF000u;
LL_QPI_RegWrite(LL_QPI_REG_OVTR,(regVal|LL_QPI_tCEM));   //set cCEM time ,<= 4uS

LL_QPI_Enable4bit();
LL_QpiState = LL_Qpi_Ready;
// The following is used to test whether the initialization code is correct, there is no other meaning, the initialization is OK
// (unsigned long )(LL_QPI_BUS) = 0xaaaaaaaa;
// TestData[0]= (unsigned long )(LL_QPI_BUS);
// for(i= 0;i<4;i++)
// {
// (unsigned long )(LL_QPI_BUS+(i*4)) = 0xaaaaaaaa;
// }
// for(i=0;i<4;i++)
// {
// TestData[i] = (unsigned long )(LL_QPI_BUS+(i*4));
// }
}





