/* drivers\video\msm\hw_mddi_lcd.c
 * LCD driver for 7x30 platform
 *
 * Copyright (C) 2010 HUAWEI Technology Co., ltd.
 * 
 * Date: 2010/12/07
 * By lijianzhao
 * 
 */
#include "msm_fb.h"
#include "mddihost.h"
#include "mddihosti.h"
#include <linux/mfd/pmic8058.h>
#include <mach/gpio.h>
#include <mach/vreg.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/hardware_self_adapt.h>
#include <asm/mach-types.h>
#include "hw_mddi_lcd.h"
#include "mdp.h"

mddi_type mddi_port_type_probe(void)
{
	mddi_type mddi_port_type = MDDI_MAX_TYPE;
	lcd_panel_type lcd_panel = lcd_panel_probe();
	/* U8800 board is MMDI type1, so config it type1 */	
	if (machine_is_msm7x30_u8800())
	{
		mddi_port_type = MDDI_TYPE1;
	}	
	/* U8820 board version A is MMDI type1, so config it type1 
	 * Version B and other is MDDI type2, so config it according to LCD
	 */
	else if(machine_is_msm7x30_u8820())
	{
		if(HW_VER_SUB_VA == get_hw_sub_board_id())
		{
			mddi_port_type = MDDI_TYPE1;
		}
		else
		{
			switch(lcd_panel)
			{
				case LCD_NT35582_BYD_WVGA:
				case LCD_NT35582_TRULY_WVGA:
					mddi_port_type = MDDI_TYPE1;
					break;
				case LCD_NT35510_ALPHA_SI_WVGA:
/* Modify to MDDI type1 to elude the freezing screen */
					mddi_port_type = MDDI_TYPE1;
					break;
   /* this lcd port type is MDDI TYPE2 interface */
				case LCD_NT35510_ALPHA_SI_WVGA_TYPE2:
					mddi_port_type = MDDI_TYPE2;
					break;
				default:
					mddi_port_type = MDDI_TYPE1;
					break;
			}
		}
	}
	/* U8800-51 board is MMDI type2, so config it according to LCD */
	else if (machine_is_msm7x30_u8800_51() || machine_is_msm8255_u8800_pro())
	{
		switch(lcd_panel)
		{
			case LCD_NT35582_BYD_WVGA:
			case LCD_NT35582_TRULY_WVGA:
				mddi_port_type = MDDI_TYPE1;
				break;
			case LCD_NT35510_ALPHA_SI_WVGA:
/* Modify to MDDI type1 to elude the freezing screen */
				mddi_port_type = MDDI_TYPE1;
				break;
   /* this lcd port type is MDDI TYPE2 interface */
			case LCD_NT35510_ALPHA_SI_WVGA_TYPE2:
				mddi_port_type = MDDI_TYPE2;
				break;
			default:
				mddi_port_type = MDDI_TYPE1;
				break;
		}
	}
    else if (machine_is_msm8255_u8860() 
		|| machine_is_msm8255_c8860() 
		|| machine_is_msm8255_u8860lp()
		|| machine_is_msm8255_u8860_92()
		|| machine_is_msm8255_u8680()
		|| machine_is_msm8255_u8860_51()
		|| machine_is_msm8255_u8730())
	{
		switch(lcd_panel)
		{
            case LCD_NT35560_TOSHIBA_FWVGA:
				mddi_port_type = MDDI_TYPE2;
				break;
			case LCD_RSP61408_CHIMEI_WVGA:
				mddi_port_type = MDDI_TYPE2;
				break;
			default:
				mddi_port_type = MDDI_TYPE2;
				break;
		}
	}
	else
	{
		mddi_port_type = MDDI_TYPE1;
	}
	return mddi_port_type;
}
/*write register with many parameters */
int mddi_multi_register_write(uint32 reg,uint32 value)
{
	static boolean first_time = TRUE;
	static uint32 param_num = 0;
	static uint32 last_reg_addr = 0;
	static uint32 value_list_ptr[MDDI_HOST_MAX_CLIENT_REG_IN_SAME_ADDR];
	int ret = 0;
	if (value & TYPE_COMMAND) 
	{		
		if(!first_time)
	    {
			ret = mddi_host_register_multiwrite(last_reg_addr,value_list_ptr ,param_num,TRUE,NULL,
		                                  MDDI_HOST_PRIM);
	    }
		else
		{
			first_time =FALSE;
		}
		last_reg_addr = reg ;
		param_num = 0;
		if(MDDI_MULTI_WRITE_END == last_reg_addr)
		{
			last_reg_addr = 0;
			first_time = TRUE;
		}
	}
	else if (value & TYPE_PARAMETER) 
	{
		value_list_ptr[param_num] = reg;
		param_num++;
	}	
	return ret;
}
int process_lcd_table(struct sequence *table, size_t count, lcd_panel_type lcd_panel)
{
	unsigned int i;
    uint32 reg = 0;
    uint32 value = 0;
    uint32 time = 0;
	int ret = 0;   
    int clk_on = 0;
   
    for (i = 0; i < count; i++) 
    {
        reg = table[i].reg;
        value = table[i].value;
        time = table[i].time;
		switch(lcd_panel)
		{
			case LCD_NT35582_BYD_WVGA:
			case LCD_NT35582_TRULY_WVGA:
			case LCD_NT35510_ALPHA_SI_WVGA:
			case LCD_NT35560_TOSHIBA_FWVGA:
			case LCD_NT35510_ALPHA_SI_WVGA_TYPE2:
                down(&mdp_pipe_ctrl_mutex);
                clk_on = pmdh_clk_func(2);
                pmdh_clk_func(1);
				/* MDDI port to write the reg and value */
				ret = mddi_queue_register_write(reg,value,TRUE,0);
                if (clk_on == 0)
                {
                    pmdh_clk_func(0);
                }
                up(&mdp_pipe_ctrl_mutex);
				break;
			case LCD_RSP61408_CHIMEI_WVGA:
				down(&mdp_pipe_ctrl_mutex);
                clk_on = pmdh_clk_func(2);
                pmdh_clk_func(1);

				ret = mddi_multi_register_write(reg,value);
                if (clk_on == 0)
                {
                    pmdh_clk_func(0);
                }
                up(&mdp_pipe_ctrl_mutex);
				break;
			default:
				break;
		}		
        if (time != 0)
        {
            LCD_MDELAY(time);
        }
	}
	return ret;
}

#ifdef CONFIG_FB_DYNAMIC_GAMMA
/***************************************************************
Function: is_panel_support_dynamic_gamma
Description: Check whether the panel supports dynamic gamma function
Parameters:
    void
Return:
    1: Panel support dynamic gamma
    0: Panel doesn't support dynamic gamma
***************************************************************/
int is_panel_support_dynamic_gamma(void)
{
    int ret = FALSE;
    static lcd_panel_type lcd_panel = LCD_NONE;

    if (lcd_panel == LCD_NONE)
    {
        lcd_panel = lcd_panel_probe();
    }

    switch (lcd_panel)
    {
        case LCD_NT35560_TOSHIBA_FWVGA:
            ret = TRUE;
            break;
        default:
            ret = FALSE;
            break;
    }

    return ret;
}
#endif

#ifdef CONFIG_FB_AUTO_CABC
/***************************************************************
Function: is_panel_support_auto_cabc
Description: Check whether the panel supports auto cabc function
Parameters:
    void
Return:
    1: Panel support cabc
    0: Panel doesn't support cabc
***************************************************************/
int is_panel_support_auto_cabc(void)
{
    int ret = FALSE;
    static lcd_panel_type lcd_panel = LCD_NONE;

    lcd_panel = lcd_panel_probe();
    if (machine_is_msm8255_u8860lp()
	  ||machine_is_msm8255_u8860_51())
    {
        switch (lcd_panel)
        {
            case LCD_NT35560_TOSHIBA_FWVGA:
                ret = TRUE;
                break;
            default:
                ret = FALSE;
                break;
        }
    }

    return ret;
}
#endif

