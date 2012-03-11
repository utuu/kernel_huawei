/* drivers\video\msm\mddi_rsp61408.c
 * rsp61408 LCD driver for 7x27 platform
 *
 * Copyright (C) 2010 HUAWEI Technology Co., ltd.
 * 
 * Date: 2011/09/01
 * By jiaoshuangwei
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
#include <linux/pwm.h>
#include <mach/pmic.h>
#include "hw_backlight.h"
#include "hw_mddi_lcd.h"
#include "lcd_hw_debug.h"
struct sequence* rsp61408_wvga_init_table = NULL;
static lcd_panel_type lcd_panel_wvga = LCD_NONE;
static const struct sequence rsp61408_wvga_init_table_1[] = 
{

/*
   * the 0x00011 must set the first line 
   * otherwise the lcd can not lighten 
   * the delay time is the front sequenc's delay time
   */
	{0x00011,TYPE_COMMAND,0}, //11h
	{0x00000,TYPE_PARAMETER,0},

	{0x000B0,TYPE_COMMAND,125}, //B0h
	{0x00004,TYPE_PARAMETER,0},

	{0x000B3,TYPE_COMMAND,0}, //B3h
	{0x00002,TYPE_PARAMETER,0}, 		
	{0x00000,TYPE_PARAMETER,0},

	{0x000B6,TYPE_COMMAND,0 }, //B6h 
	{0x00052,TYPE_PARAMETER,0},
	{0x00083,TYPE_PARAMETER,0},

	{0x000B7,TYPE_COMMAND,0 }, //B7h
	{0x00000,TYPE_PARAMETER,0},
	{0x00000,TYPE_PARAMETER,0},
	{0x00011,TYPE_PARAMETER,0},
	{0x00025,TYPE_PARAMETER,0},

	{0x000B8,TYPE_COMMAND,0 }, //B8h
	{0x00000,TYPE_PARAMETER,0},
	{0x0000F,TYPE_PARAMETER,0},
	{0x0000F,TYPE_PARAMETER,0},
	{0x000FF,TYPE_PARAMETER,0}, 
	{0x000FF,TYPE_PARAMETER,0},
	{0x000C8,TYPE_PARAMETER,0},
	{0x000C8,TYPE_PARAMETER,0},
	{0x00002,TYPE_PARAMETER,0},
	{0x00018,TYPE_PARAMETER,0},
	{0x00010,TYPE_PARAMETER,0},
	{0x00010,TYPE_PARAMETER,0},
	{0x00037,TYPE_PARAMETER,0},
	{0x0005A,TYPE_PARAMETER,0},
	{0x00087,TYPE_PARAMETER,0},
	{0x000BE,TYPE_PARAMETER,0},
	{0x000FF,TYPE_PARAMETER,0},
	{0x00000,TYPE_PARAMETER,0},
	{0x00000,TYPE_PARAMETER,0},
	{0x00000,TYPE_PARAMETER,0},
	{0x00000,TYPE_PARAMETER,0},

	{0x000B9,TYPE_COMMAND, 0}, //B9h
	{0x00000,TYPE_PARAMETER,0},
	{0x000FF,TYPE_PARAMETER,0},
	{0x00002,TYPE_PARAMETER,0},
	{0x00008,TYPE_PARAMETER,0}, 

	{0x000BD,TYPE_COMMAND,0}, //BDh
	{0x00000,TYPE_PARAMETER,0},

	{0x000C0,TYPE_COMMAND, 0}, //C0h
	{0x00002,TYPE_PARAMETER,0}, 
	{0x00087,TYPE_PARAMETER,0}, 

	{0x000C1,TYPE_COMMAND,0}, //C1h
	{0x00043,TYPE_PARAMETER,0}, 
	{0x00031,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0}, 
	{0x00021,TYPE_PARAMETER,0}, 
	{0x00021,TYPE_PARAMETER,0},
	{0x00032,TYPE_PARAMETER,0}, 
	{0x00012,TYPE_PARAMETER,0}, 
	{0x00028,TYPE_PARAMETER,0}, 
	{0x0004A,TYPE_PARAMETER,0}, 
	{0x0001E,TYPE_PARAMETER,0},
	{0x000A5,TYPE_PARAMETER,0}, 
	{0x0000F,TYPE_PARAMETER,0}, 
	{0x00058,TYPE_PARAMETER,0}, 
	{0x00021,TYPE_PARAMETER,0}, 
	{0x00001,TYPE_PARAMETER,0},

	{0x000C2,TYPE_COMMAND, 0}, //C2h
	{0x00010,TYPE_PARAMETER,0}, 
	{0x00006,TYPE_PARAMETER,0},
	{0x00006,TYPE_PARAMETER,0}, 
	{0x00001,TYPE_PARAMETER,0},
	{0x00003,TYPE_PARAMETER,0}, 
	{0x00004,TYPE_PARAMETER,0},

	{0x000C6,TYPE_COMMAND,0}, //C6h
	{0x00000,TYPE_PARAMETER,0},
	{0x00000,TYPE_PARAMETER,0},

	{0x000C8,TYPE_COMMAND,0}, //C8h
	{0x00000,TYPE_PARAMETER,0}, 
	{0x0000D,TYPE_PARAMETER,0}, 
	{0x0001F,TYPE_PARAMETER,0}, 
	{0x00029,TYPE_PARAMETER,0}, 
	{0x00036,TYPE_PARAMETER,0}, 
	{0x0004D,TYPE_PARAMETER,0}, 
	{0x00037,TYPE_PARAMETER,0}, 
	{0x00025,TYPE_PARAMETER,0}, 
	{0x00019,TYPE_PARAMETER,0}, 
	{0x00011,TYPE_PARAMETER,0}, 
	{0x00008,TYPE_PARAMETER,0}, 
	{0x00001,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0}, 
	{0x0000D,TYPE_PARAMETER,0}, 
	{0x0001F,TYPE_PARAMETER,0}, 
	{0x00029,TYPE_PARAMETER,0}, 
	{0x00036,TYPE_PARAMETER,0}, 
	{0x0004D,TYPE_PARAMETER,0}, 
	{0x00037,TYPE_PARAMETER,0}, 
	{0x00025,TYPE_PARAMETER,0}, 
	{0x00019,TYPE_PARAMETER,0}, 
	{0x00011,TYPE_PARAMETER,0}, 
	{0x00008,TYPE_PARAMETER,0}, 
	{0x00001,TYPE_PARAMETER,0},

	{0x000C9,TYPE_COMMAND,0}, //C9h
	{0x00000,TYPE_PARAMETER,0}, 
	{0x0000D,TYPE_PARAMETER,0},
	{0x0001F,TYPE_PARAMETER,0},  
	{0x00029,TYPE_PARAMETER,0}, 
	{0x00036,TYPE_PARAMETER,0}, 
	{0x0004D,TYPE_PARAMETER,0}, 
	{0x00037,TYPE_PARAMETER,0}, 
	{0x00025,TYPE_PARAMETER,0}, 
	{0x00019,TYPE_PARAMETER,0}, 
	{0x00011,TYPE_PARAMETER,0}, 
	{0x00008,TYPE_PARAMETER,0}, 
	{0x00001,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0}, 
	{0x0000D,TYPE_PARAMETER,0}, 
	{0x0001F,TYPE_PARAMETER,0}, 
	{0x00029,TYPE_PARAMETER,0}, 
	{0x00036,TYPE_PARAMETER,0}, 
	{0x0004D,TYPE_PARAMETER,0}, 
	{0x00037,TYPE_PARAMETER,0}, 
	{0x00025,TYPE_PARAMETER,0}, 
	{0x00019,TYPE_PARAMETER,0}, 
	{0x00011,TYPE_PARAMETER,0}, 
	{0x00008,TYPE_PARAMETER,0}, 
	{0x00001,TYPE_PARAMETER,0},

	{0x000CA,TYPE_COMMAND,0}, //CAh
	{0x00000,TYPE_PARAMETER,0}, 
	{0x0001D,TYPE_PARAMETER,0}, 
	{0x0001F,TYPE_PARAMETER,0}, 
	{0x00029,TYPE_PARAMETER,0}, 
	{0x00036,TYPE_PARAMETER,0}, 
	{0x0004D,TYPE_PARAMETER,0}, 
	{0x00037,TYPE_PARAMETER,0}, 
	{0x00025,TYPE_PARAMETER,0}, 
	{0x00019,TYPE_PARAMETER,0}, 
	{0x00011,TYPE_PARAMETER,0}, 
	{0x00028,TYPE_PARAMETER,0}, 
	{0x00001,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0}, 
	{0x0000D,TYPE_PARAMETER,0}, 
	{0x0001F,TYPE_PARAMETER,0}, 
	{0x00029,TYPE_PARAMETER,0}, 
	{0x00036,TYPE_PARAMETER,0}, 
	{0x0004D,TYPE_PARAMETER,0}, 
	{0x00037,TYPE_PARAMETER,0}, 
	{0x00025,TYPE_PARAMETER,0}, 
	{0x00019,TYPE_PARAMETER,0}, 
	{0x00011,TYPE_PARAMETER,0}, 
	{0x00008,TYPE_PARAMETER,0}, 
	{0x00001,TYPE_PARAMETER,0}, 

	{0x000D0,TYPE_COMMAND,0}, //D0h 
	{0x00029,TYPE_PARAMETER,0}, 
	{0x00003,TYPE_PARAMETER,0}, 
	{0x000CB,TYPE_PARAMETER,0},
	{0x000A3,TYPE_PARAMETER,0},
	{0x0000C,TYPE_PARAMETER,0},
	{0x00071,TYPE_PARAMETER,0}, 
	{0x00020,TYPE_PARAMETER,0}, 
	{0x00010,TYPE_PARAMETER,0}, 
	{0x00001,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0}, 
	{0x00001,TYPE_PARAMETER,0}, 
	{0x00001,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0}, 
	{0x00003,TYPE_PARAMETER,0}, 
	{0x00001,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0},

	{0x000D1,TYPE_COMMAND,0}, //D1h
	{0x00018,TYPE_PARAMETER,0}, 
	{0x0000C,TYPE_PARAMETER,0}, 
	{0x00023,TYPE_PARAMETER,0}, 
	{0x00003,TYPE_PARAMETER,0},
	{0x00075,TYPE_PARAMETER,0}, 
	{0x00002,TYPE_PARAMETER,0}, 
	{0x00050,TYPE_PARAMETER,0},

	{0x000D3,TYPE_COMMAND,0}, //D3h
	{0x00033,TYPE_PARAMETER,0},

	{0x000D5,TYPE_COMMAND,0}, //D5h
	{0x00008,TYPE_PARAMETER,0}, 
	{0x00008,TYPE_PARAMETER,0},

	{0x000D6,TYPE_COMMAND,0}, //D6h
	{0x000A8,TYPE_PARAMETER,0}, 
	 
	{0x000DE,TYPE_COMMAND,0}, //DEh
	{0x00001,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0},

	{0x000E0,TYPE_COMMAND,0}, //E0h
	{0x00000,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0},
	{0x00000,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0},

	{0x000E1,TYPE_COMMAND,0}, //E1h
	{0x00000,TYPE_PARAMETER,0},
	{0x00000,TYPE_PARAMETER,0},
	{0x00000,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0},
	{0x00000,TYPE_PARAMETER,0},
	 
	{0x000E6,TYPE_COMMAND,10}, //E6h
	{0x00000,TYPE_PARAMETER,0},

	{0x000FA,TYPE_COMMAND,10}, //FAh
	{0x00003,TYPE_PARAMETER,0},

	{0x000B0,TYPE_COMMAND,0 }, //B0h
	{0x00003,TYPE_PARAMETER, 0},
		
	//----Exit sleep mode----//
	
	{0x0002A,TYPE_COMMAND,0},// //2Ah
	{0x00000,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0},  
	{0x00001,TYPE_PARAMETER,0},  
	{0x000DF,TYPE_PARAMETER,0},
	 
	{0x0002B,TYPE_COMMAND,10},// 2Bh
	{0x00000,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0},  
	{0x00003,TYPE_PARAMETER,0},  
	{0x0001F,TYPE_PARAMETER,0}, 
/*set the address mode to resolve display inversion */
	{0x00036,TYPE_COMMAND,10}, //36h
	{0x000C0,TYPE_PARAMETER,0}, 

/*55:16 BIT/PIXEL, 66:18 BIT/PIXEL, 77:24 BIT/PIXEL*/
	{0x0003A,TYPE_COMMAND,10}, //3
	{0x00077,TYPE_PARAMETER,0},  


	//-----SET DISPLAY ON-----// 
	
	{0x000B0,TYPE_COMMAND,100}, //B0h
	{0x00004,TYPE_PARAMETER,0},
	{0x000C1,TYPE_COMMAND,0}, //C1h
	{0x00043,TYPE_PARAMETER,0}, 
	{0x00031,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0},

	 
	{0x000B0,TYPE_COMMAND,0}, //B0h
	{0x00003,TYPE_PARAMETER,0},
	{0x00029,TYPE_COMMAND,0}, //29h
	{0x00000,TYPE_PARAMETER,0}, //11h
	{0x0002C,TYPE_COMMAND,100},// //2C
	{0x00000,TYPE_PARAMETER,0}, //11h
/*the end flag,it don't sent to driver IC*/
	{MDDI_MULTI_WRITE_END,TYPE_COMMAND,0}, 
};

static const struct sequence rsp61408_wvga_standby_exit_table[]= 
{
	/*set the delay time 100ms*/
	{0x0002A,TYPE_COMMAND,0},// //2Ah
	{0x00000,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0},  
	{0x00001,TYPE_PARAMETER,0},  
	{0x000DF,TYPE_PARAMETER,0},
	 
	{0x0002B,TYPE_COMMAND,10},// 2Bh
	{0x00000,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0},  
	{0x00003,TYPE_PARAMETER,0},  
	{0x0001F,TYPE_PARAMETER,0}, 

	{0x00036,TYPE_COMMAND,10}, //36h
	{0x000C0,TYPE_PARAMETER,0}, 

	{0x0003A,TYPE_COMMAND,10}, //3A
	{0x00077,TYPE_PARAMETER,0},
	
	{0x0011,TYPE_COMMAND,0},
	{0x0000,TYPE_PARAMETER,0},

	{0x000B0,TYPE_COMMAND,100}, //B0h
	{0x00004,TYPE_PARAMETER,0},
	{0x000C1,TYPE_COMMAND,0}, //C1h
	{0x00043,TYPE_PARAMETER,0}, 
	{0x00031,TYPE_PARAMETER,0}, 
	{0x00000,TYPE_PARAMETER,0},

	 
	{0x000B0,TYPE_COMMAND,0}, //B0h
	{0x00003,TYPE_PARAMETER,0},
	{0x00029,TYPE_COMMAND,0}, //29h
	{0x00000,TYPE_PARAMETER,0}, //11h
	{0x0002C,TYPE_COMMAND,100}, // 2C
	{0x00000,TYPE_PARAMETER,0}, //11h
	{MDDI_MULTI_WRITE_END,TYPE_COMMAND,100}, //the end flag,it don't sent to driver IC
};
static const struct sequence rsp61408_wvga_standby_enter_table[]= 
{
	/*set the delay time 100ms*/
	{0x00028,TYPE_COMMAND,0}, //29h
	{0x00000,TYPE_PARAMETER,0},
	{0x0010,TYPE_COMMAND,100},
	{0x0000,TYPE_PARAMETER,0},
	{MDDI_MULTI_WRITE_END,TYPE_COMMAND,100}, //the end flag,it don't sent to driver IC
};



static int rsp61408_lcd_on(struct platform_device *pdev)
{
	boolean para_debug_flag = FALSE;
    uint32 para_num = 0;
	int ret = 0;
  /*  add reset lcd 20110830 end */
     gpio_tlmm_config(GPIO_CFG(180, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),1);
	 gpio_set_value(180,0);
     LCD_MDELAY(50);
	 gpio_set_value(180,1);
	 LCD_MDELAY(120);
 /* open debug file and read the para */
	switch(lcd_panel_wvga)
	{
		case LCD_RSP61408_CHIMEI_WVGA:
			para_debug_flag = lcd_debug_malloc_get_para( "rsp61408_wvga_init_table", 
	    		(void**)&rsp61408_wvga_init_table,&para_num);
			break;
		default:
			break;
	}
	/* If exist the init file ,then init lcd with it for debug */
    if( (TRUE == para_debug_flag)&&(NULL != rsp61408_wvga_init_table))
    {
		ret = process_lcd_table(rsp61408_wvga_init_table, para_num, lcd_panel_wvga);
    }
    else
    {
		/* Exit Standby Mode */
		ret = process_lcd_table((struct sequence*)&rsp61408_wvga_init_table_1, 
			ARRAY_SIZE(rsp61408_wvga_init_table_1), lcd_panel_wvga);
    }
       
	/* Must malloc before,then you can call free */
	if((TRUE == para_debug_flag)&&(NULL != rsp61408_wvga_init_table))
	{
		lcd_debug_free_para((void *)rsp61408_wvga_init_table);
	}
	
    MDDI_LCD_DEBUG("%s: rsp61408_lcd exit sleep mode ,on_ret=%d\n",__func__,ret);
	
	return ret;
}

static int rsp61408_lcd_off(struct platform_device *pdev)
{
	int ret = 0;
	ret = process_lcd_table((struct sequence*)&rsp61408_wvga_standby_enter_table, 
    	      		ARRAY_SIZE(rsp61408_wvga_standby_enter_table), lcd_panel_wvga);
    MDDI_LCD_DEBUG("%s: rsp61408_lcd enter sleep mode ,off_ret=%d\n",__func__,ret);
	return ret;
}

static int __devinit rsp61408_probe(struct platform_device *pdev)
{
	msm_fb_add_device(pdev);
 	return 0;
}

static struct platform_driver this_driver = {
	.probe  = rsp61408_probe,
	.driver = {
		.name   = "mddi_rsp61408_wvga",
	},
};

static struct msm_fb_panel_data rsp61408_panel_data = {
	.on = rsp61408_lcd_on,
	.off = rsp61408_lcd_off,
	.set_backlight = pwm_set_backlight,
};

static struct platform_device this_device = {
	.name   = "mddi_rsp61408_wvga",
	.id	= 0,
	.dev	= {
		.platform_data = &rsp61408_panel_data,
	}
};
static int __init rsp61408_init(void)
{
	int ret = 0;
	struct msm_panel_info *pinfo = NULL;
	bpp_type bpp = MDDI_OUT_24BPP;		
	mddi_type mddi_port_type = mddi_port_type_probe();

	lcd_panel_wvga=lcd_panel_probe();
	
	if(LCD_RSP61408_CHIMEI_WVGA != lcd_panel_wvga)
	{
		return 0;
	}

	MDDI_LCD_DEBUG("%s:start init %s\n",__func__,this_device.name);
	/* Select which bpp accroding MDDI port type */
	if(MDDI_TYPE1 == mddi_port_type)
	{
		bpp = MDDI_OUT_16BPP;
	}
	else if(MDDI_TYPE2 == mddi_port_type)
	{
		bpp = MDDI_OUT_24BPP;
	}
	else
	{
		bpp = MDDI_OUT_16BPP;
	}
	
	ret = platform_driver_register(&this_driver);
	if (!ret) 
	{
		pinfo = &rsp61408_panel_data.panel_info;
		pinfo->xres = 480;
		pinfo->yres = 800;
		pinfo->type = MDDI_PANEL;
		pinfo->pdest = DISPLAY_1;
		pinfo->mddi.vdopkt = MDDI_DEFAULT_PRIM_PIX_ATTR;
		pinfo->wait_cycle = 0;
		pinfo->bpp = (uint32)bpp;
		pinfo->fb_num = 2;
        pinfo->clk_rate = 192000000;
	    pinfo->clk_min = 192000000;
	    pinfo->clk_max = 192000000;
        pinfo->lcd.vsync_enable = TRUE;
        pinfo->lcd.refx100 = 5500;
		pinfo->lcd.v_back_porch = 0;
		pinfo->lcd.v_front_porch = 0;
		pinfo->lcd.v_pulse_width = 22;
		pinfo->lcd.hw_vsync_mode = TRUE;
		pinfo->lcd.vsync_notifier_period = 0;
		pinfo->bl_max = 255;

		ret = platform_device_register(&this_device);
		if (ret)
		{
			platform_driver_unregister(&this_driver);
		}
	}

	return ret;
}
module_init(rsp61408_init);
