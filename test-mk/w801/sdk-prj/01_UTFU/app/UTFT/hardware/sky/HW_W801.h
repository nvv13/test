/**
 *
 *
**/


// *** Hardware specific functions ***
void UTFT__hw_special_init()
{
}

void UTFT_LCD_Writ_Bus(char VH,char VL, byte mode)
{
	switch (mode)
	{
	case 1:
		if (display_serial_mode==SERIAL_4PIN)
		{
			if (VH==1)
				sbi(P_SDA, B_SDA);
			else
				cbi(P_SDA, B_SDA);
			pulse_low(P_SCL, B_SCL);
		}
		else
		{
			if (VH==1)
				sbi(P_RS, B_RS);
			else
				cbi(P_RS, B_RS);
		}

		if (VL & 0x80)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x40)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x20)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x10)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x08)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x04)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x02)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x01)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		break;
	case 8:
		tls_gpio_write (PIN0, VH & 0x80 ? 1 : 0);
		tls_gpio_write (PIN1, VH & 0x40 ? 1 : 0);
		tls_gpio_write (PIN2, VH & 0x20 ? 1 : 0);
		tls_gpio_write (PIN3, VH & 0x10 ? 1 : 0);
		tls_gpio_write (PIN4, VH & 0x08 ? 1 : 0);
		tls_gpio_write (PIN5, VH & 0x04 ? 1 : 0);
		tls_gpio_write (PIN6, VH & 0x02 ? 1 : 0);
		tls_gpio_write (PIN7, VH & 0x01 ? 1 : 0);
		pulse_low(P_WR, B_WR);
		tls_gpio_write (PIN0, VL & 0x80 ? 1 : 0);
		tls_gpio_write (PIN1, VL & 0x40 ? 1 : 0);
		tls_gpio_write (PIN2, VL & 0x20 ? 1 : 0);
		tls_gpio_write (PIN3, VL & 0x10 ? 1 : 0);
		tls_gpio_write (PIN4, VL & 0x08 ? 1 : 0);
		tls_gpio_write (PIN5, VL & 0x04 ? 1 : 0);
		tls_gpio_write (PIN6, VL & 0x02 ? 1 : 0);
		tls_gpio_write (PIN7, VL & 0x01 ? 1 : 0);
		pulse_low(P_WR, B_WR);
		break;
	case 16:
		{
/*
		tls_gpio_write (PIN0, VH & 0x80 ? 1 : 0);
		tls_gpio_write (PIN1, VH & 0x40 ? 1 : 0);
		tls_gpio_write (PIN2, VH & 0x20 ? 1 : 0);
		tls_gpio_write (PIN3, VH & 0x10 ? 1 : 0);
		tls_gpio_write (PIN4, VH & 0x08 ? 1 : 0);
		tls_gpio_write (PIN5, VH & 0x04 ? 1 : 0);
		tls_gpio_write (PIN6, VH & 0x02 ? 1 : 0);
		tls_gpio_write (PIN7, VH & 0x01 ? 1 : 0);
		tls_gpio_write (PIN8, VL & 0x80 ? 1 : 0);
		tls_gpio_write (PIN9, VL & 0x40 ? 1 : 0);
		tls_gpio_write (PIN10, VL & 0x20 ? 1 : 0);
		tls_gpio_write (PIN11, VL & 0x10 ? 1 : 0);
		tls_gpio_write (PIN12, VL & 0x08 ? 1 : 0);
		tls_gpio_write (PIN13, VL & 0x04 ? 1 : 0);
		tls_gpio_write (PIN14, VL & 0x02 ? 1 : 0);
		tls_gpio_write (PIN15, VL & 0x01 ? 1 : 0);
*/
		//!ВНИМАНИЕ! PIN0 - PIN15, использовать только PB порт GPIO !ВНИМАНИЕ!
		u32 cpu_sr = 0;
        	u32 reg;
		u32 reg_en;
		cpu_sr = tls_os_set_critical();  // disable Interrupt !!!
		reg_en = tls_reg_read32(HR_GPIO_DATA_EN + TLS_IO_AB_OFFSET);
		tls_reg_write32(HR_GPIO_DATA_EN + TLS_IO_AB_OFFSET, reg_en 
                        | (1 << (PIN0 - WM_IO_PB_00))
                        | (1 << (PIN1 - WM_IO_PB_00))
                        | (1 << (PIN2 - WM_IO_PB_00))
                        | (1 << (PIN3 - WM_IO_PB_00))
                        | (1 << (PIN4 - WM_IO_PB_00))
                        | (1 << (PIN5 - WM_IO_PB_00))
                        | (1 << (PIN6 - WM_IO_PB_00))
                        | (1 << (PIN7 - WM_IO_PB_00))
                        | (1 << (PIN8 - WM_IO_PB_00))
                        | (1 << (PIN9 - WM_IO_PB_00))
                        | (1 << (PIN10 - WM_IO_PB_00))
                        | (1 << (PIN11 - WM_IO_PB_00))
                        | (1 << (PIN12 - WM_IO_PB_00))
                        | (1 << (PIN13 - WM_IO_PB_00))
                        | (1 << (PIN14 - WM_IO_PB_00))
                        | (1 << (PIN15 - WM_IO_PB_00))
                        ); // enabled control reg from need pin
	
		reg = tls_reg_read32(HR_GPIO_DATA + TLS_IO_AB_OFFSET); // load all pins from port
    
                if(VH & 0x80)reg |= (1 << (PIN0 - WM_IO_PB_00)); else reg &= (~(1 << (PIN0 - WM_IO_PB_00)));
                if(VH & 0x40)reg |= (1 << (PIN1 - WM_IO_PB_00)); else reg &= (~(1 << (PIN1 - WM_IO_PB_00)));
                if(VH & 0x20)reg |= (1 << (PIN2 - WM_IO_PB_00)); else reg &= (~(1 << (PIN2 - WM_IO_PB_00)));
                if(VH & 0x10)reg |= (1 << (PIN3 - WM_IO_PB_00)); else reg &= (~(1 << (PIN3 - WM_IO_PB_00)));
                if(VH & 0x08)reg |= (1 << (PIN4 - WM_IO_PB_00)); else reg &= (~(1 << (PIN4 - WM_IO_PB_00)));
                if(VH & 0x04)reg |= (1 << (PIN5 - WM_IO_PB_00)); else reg &= (~(1 << (PIN5 - WM_IO_PB_00)));
                if(VH & 0x02)reg |= (1 << (PIN6 - WM_IO_PB_00)); else reg &= (~(1 << (PIN6 - WM_IO_PB_00)));
                if(VH & 0x01)reg |= (1 << (PIN7 - WM_IO_PB_00)); else reg &= (~(1 << (PIN7 - WM_IO_PB_00)));

                if(VL & 0x80)reg |= (1 << (PIN8 - WM_IO_PB_00)); else reg &= (~(1 << (PIN8 - WM_IO_PB_00)));
                if(VL & 0x40)reg |= (1 << (PIN9 - WM_IO_PB_00)); else reg &= (~(1 << (PIN9 - WM_IO_PB_00)));
                if(VL & 0x20)reg |= (1 << (PIN10- WM_IO_PB_00)); else reg &= (~(1 << (PIN10- WM_IO_PB_00)));
                if(VL & 0x10)reg |= (1 << (PIN11- WM_IO_PB_00)); else reg &= (~(1 << (PIN11- WM_IO_PB_00)));
                if(VL & 0x08)reg |= (1 << (PIN12- WM_IO_PB_00)); else reg &= (~(1 << (PIN12- WM_IO_PB_00)));
                if(VL & 0x04)reg |= (1 << (PIN13- WM_IO_PB_00)); else reg &= (~(1 << (PIN13- WM_IO_PB_00)));
                if(VL & 0x02)reg |= (1 << (PIN14- WM_IO_PB_00)); else reg &= (~(1 << (PIN14- WM_IO_PB_00)));
                if(VL & 0x01)reg |= (1 << (PIN15- WM_IO_PB_00)); else reg &= (~(1 << (PIN15- WM_IO_PB_00)));

	        tls_reg_write32(HR_GPIO_DATA + TLS_IO_AB_OFFSET, reg );/* write reg all enabled pin port B */
	
	        tls_reg_write32(HR_GPIO_DATA_EN + TLS_IO_AB_OFFSET, reg_en); // reg_en return
		tls_os_release_critical(cpu_sr); // enable Interrupt

		pulse_low(P_WR, B_WR);
		}
		break;
	}
}

void UTFT__set_direction_registers(byte mode)
{

  tls_gpio_cfg (PIN0, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN1, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN2, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN3, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN4, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN5, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN6, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
  tls_gpio_cfg (PIN7, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?

  if (mode == 16)
    {
    tls_gpio_cfg (PIN8, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
    tls_gpio_cfg (PIN9, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
    tls_gpio_cfg (PIN10, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
    tls_gpio_cfg (PIN11, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
    tls_gpio_cfg (PIN12, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
    tls_gpio_cfg (PIN13, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
    tls_gpio_cfg (PIN14, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
    tls_gpio_cfg (PIN15, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING); // PULLLOW ? PULLHIGH ?
    }

}
void UTFT__fast_fill_16(int ch, int cl, long pix)
{
	long blocks;

		tls_gpio_write (PIN0, ch & 0x80 ? 1 : 0);
		tls_gpio_write (PIN1, ch & 0x40 ? 1 : 0);
		tls_gpio_write (PIN2, ch & 0x20 ? 1 : 0);
		tls_gpio_write (PIN3, ch & 0x10 ? 1 : 0);
		tls_gpio_write (PIN4, ch & 0x08 ? 1 : 0);
		tls_gpio_write (PIN5, ch & 0x04 ? 1 : 0);
		tls_gpio_write (PIN6, ch & 0x02 ? 1 : 0);
		tls_gpio_write (PIN7, ch & 0x01 ? 1 : 0);
		tls_gpio_write (PIN8, cl & 0x80 ? 1 : 0);
		tls_gpio_write (PIN9, cl & 0x40 ? 1 : 0);
		tls_gpio_write (PIN10, cl & 0x20 ? 1 : 0);
		tls_gpio_write (PIN11, cl & 0x10 ? 1 : 0);
		tls_gpio_write (PIN12, cl & 0x08 ? 1 : 0);
		tls_gpio_write (PIN13, cl & 0x04 ? 1 : 0);
		tls_gpio_write (PIN14, cl & 0x02 ? 1 : 0);
		tls_gpio_write (PIN15, cl & 0x01 ? 1 : 0);



	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16); i++)
		{
			pulse_low(P_WR, B_WR);
		}
}

void UTFT__fast_fill_8(int ch, long pix)
{
	long blocks;

		tls_gpio_write (PIN0, ch & 0x80 ? 1 : 0);
		tls_gpio_write (PIN1, ch & 0x40 ? 1 : 0);
		tls_gpio_write (PIN2, ch & 0x20 ? 1 : 0);
		tls_gpio_write (PIN3, ch & 0x10 ? 1 : 0);
		tls_gpio_write (PIN4, ch & 0x08 ? 1 : 0);
		tls_gpio_write (PIN5, ch & 0x04 ? 1 : 0);
		tls_gpio_write (PIN6, ch & 0x02 ? 1 : 0);
		tls_gpio_write (PIN7, ch & 0x01 ? 1 : 0);

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16); i++)
		{
			pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		}
}