/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,Öğ·É¿Æ¼¼
 * All rights reserved.
 * ¼¼ÊõÌÖÂÛQQÈº£ºÒ»Èº£º179029047(ÒÑÂú)  ¶şÈº£º244861897
 *
 * ÒÔÏÂËùÓĞÄÚÈİ°æÈ¨¾ùÊôÖğ·É¿Æ¼¼ËùÓĞ£¬Î´¾­ÔÊĞí²»µÃÓÃÓÚÉÌÒµÓÃÍ¾£¬
 * »¶Ó­¸÷Î»Ê¹ÓÃ²¢´«²¥±¾³ÌĞò£¬ĞŞ¸ÄÄÚÈİÊ±±ØĞë±£ÁôÖğ·É¿Æ¼¼µÄ°æÈ¨ÉùÃ÷¡£
 *
 * @file       		headfile
 * @company	   		³É¶¼Öğ·É¿Æ¼¼ÓĞÏŞ¹«Ë¾
 * @author     		Öğ·É¿Æ¼¼(QQ3184284598)
 * @version    		²é¿´docÄÚversionÎÄ¼ş °æ±¾ËµÃ÷
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 
#ifndef _headfile_h
#define _headfile_h


#include <stdint.h>
#include <stdio.h>
#include "fsl_common.h"

#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_pit.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_cache.h"
#include "common.h"
#include "zf_vector.h"

//------ÎÄ¼şÏµÍ³Ïà¹ØÍ·ÎÄ¼ş
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"

#include "SEEKFREE_PRINTF.h"


//------Öğ·É¿Æ¼¼µ¥Æ¬»úÍâÉèÇı¶¯Í·ÎÄ¼ş
#include "zf_gpio.h"
#include "zf_iomuxc.h"
#include "zf_pit.h"
#include "zf_pwm.h"
#include "zf_uart.h"
#include "zf_spi.h"
#include "zf_systick.h"
#include "zf_qtimer.h"
#include "zf_adc.h"
#include "zf_iic.h"
#include "zf_flash.h"
#include "zf_camera.h"
#include "zf_csi.h"
#include "zf_rom_api.h"
#include "zf_usb_cdc.h"
#include "zf_sdcard.h"

//------Öğ·É¿Æ¼¼²úÆ·Çı¶¯Í·ÎÄ¼ş
#include "SEEKFREE_FONT.h"
#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_IPS114_SPI.h"
#include "SEEKFREE_IPS200_PARALLEL8.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_VIRSCO.h"
#include "SEEKFREE_FUN.h"
#include "SEEKFREE_MPU6050.h"
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_NRF24L01.h"
#include "SEEKFREE_MMA8451.h"
#include "SEEKFREE_L3G4200D.h"
#include "SEEKFREE_ICM20602.h"
#include "SEEKFREE_MT9V03X.h"
#include "SEEKFREE_WIRELESS.h"
#include "SEEKFREE_MT9V03X_CSI.h"
#include "SEEKFREE_W25QXXJV.h"
#include "SEEKFREE_SCC8660_CSI.h"
#include "SEEKFREE_SCC8660.h"

//------my head file
#include "motor.h"
#include "GUI.h"
#include "vofa.h"
#include "key.h"
#include "filter.h"
#include "icm.h"
#include "location.h"
#include "ahrs.h"
#include "fast_math.h"


#endif ·

