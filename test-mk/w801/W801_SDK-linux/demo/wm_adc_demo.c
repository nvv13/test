/***************************************************************************** 
* 
* File Name : wm_adc_demo.c 
* 
* Description: adc demo function 
* 
* Copyright (c) 2014 Winner Micro Electronic Design Co., Ltd. 
* All rights reserved. 
* 
* Author : dave
* 
* Date : 2014-8-18
*****************************************************************************/ 
#include "wm_include.h"
#include "wm_adc.h"
#include "wm_gpio_afsel.h"


#if DEMO_ADC
#define TMAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define TMACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
int adc_input_voltage_cal_demo(u8 chan, signed short refvoltage)
{
	int ret =0;
	u8 mac[6];

	tls_get_mac_addr(mac);
	printf("Mac["TMACSTR"],Cch[%d], Refvol[%d] ", TMAC2STR(mac), chan, refvoltage);

    ret = adc_offset_calibration(chan, refvoltage);
	if (ret == 0)
	{
		printf("Calok ");
	}
	else
	{
		printf("Calerr \r\n");
		return 0;
	}

	int voltage =0;
	int i =0 ;
	for (i = 0; i < 4; i++)
	{
		if (i != chan)
		{
	    	wm_adc_config(i);
		    voltage = adc_get_inputVolt(i);
			printf("Mch[%d]-%d(mV) ", i, voltage);
		}
	}
	printf("\r\n");

    return 0;
}

int adc_input_voltage_multipoint_cal_demo(int chanused,int chan0ref, int chan1ref, int chan2ref, int chan3ref)
{
	int ret =0;
	u8 mac[6];
	int refvol[4] = {chan0ref, chan1ref, chan2ref, chan3ref};

	tls_get_mac_addr(mac);
	printf("Mac["TMACSTR"],", TMAC2STR(mac));
	for (int i = 0; i < 4; i++)
	{
		if (chanused&(1<<i))
		{
			printf("ch[%d], refvol[%d] ", i, refvol[i]);
		}
	}
	
	ret = adc_multipoint_calibration(chanused, refvol);
	if (ret == 0)
	{
		printf("Calok \r\n");
	}
	else
	{
		printf("Calerr \r\n");
		return 0;
	}

	int voltage =0;
	int i =0 ;
	for (i = 0; i < 2; i++)
	{
    	wm_adc_config(i);
	    voltage = adc_get_inputVolt(i);
		printf("Mch[%d]-%d(mV) \r\n", i, voltage);
	}

    return 0;
}



int adc_input_voltage_demo(u8 chan)
{
	int voltage =0;

	if (chan < 4)
	{
    	wm_adc_config(chan);
	}
	else if (chan == 8 )
	{
    	wm_adc_config(0);		
    	wm_adc_config(1);				
	}
	else if (chan == 9)
	{
    	wm_adc_config(2);	
    	wm_adc_config(3);			
	}
	else
	{
		return -1;
	}
    voltage = adc_get_inputVolt(chan);
	if (voltage < 0)
	{
		voltage = 0 - voltage;
		printf("chan:%d, -%d(mV) or -%d.%03d(V)\r\n", chan, voltage, voltage/1000, voltage%1000);
	}
	else
	{
		printf("chan:%d, %d(mV) or %d.%03d(V)\r\n", chan, voltage, voltage/1000, voltage%1000);	
	}
    
    return 0;
}


int adc_chip_temperature_demo(void)
{
    char temperature[8] = {0};
    int temp;
    
    temp = adc_temp();
	if (temp < 0)
	{
		temp = 0 - temp;
    	sprintf(temperature, "-%d.%03d", temp/1000, temp%1000);
	}
	else
	{
    	sprintf(temperature, "%d.%03d", temp/1000, temp%1000);	
	}
    printf("tem: %s\r\n", temperature);
    
    return 0;
}


int adc_power_voltage_demo(void)
{
	int voltage =0;

    voltage = adc_get_interVolt();
	printf("Power voltage:%d(mV) or %d.%03d(V)\r\n",voltage, voltage/1000, voltage%1000);	
    
    return 0;
}

#endif





