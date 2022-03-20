/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本


//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完

//下载代码前请根据自己使用的下载器在工程里设置下载器为自己所使用的

#include "headfile.h"

// **************************** 宏定义 ****************************
// **************************** 宏定义 ****************************

// **************************** 变量定义 ****************************
static rt_sem_t dynamic_sem = RT_NULL;							// 创建指向信号量的指针
// **************************** 变量定义 ****************************

// **************************** 代码区域 ****************************
void release_semaphore_thread_entry (void *parameter);
void get_semaphore_thread_entry (void *parameter);
int semaphore_example (void);

INIT_APP_EXPORT(semaphore_example);      //应用初始化

int main(void)
{
	//此处编写用户代码(例如：外设初始化代码等)

    gpio_init(B9, GPO, 0, GPIO_PIN_CONFIG);


    EnableGlobalIRQ(0);
    while (1)
    {
        //此处编写需要循环执行的代码
        
        gpio_toggle(B9);
        rt_thread_mdelay(100);
    }
}

//------------------------------------------------------------
// @brief		释放信号量线程入口函数
// @param		parameter	参数
// @return		void
// Sample usage:
//------------------------------------------------------------
void release_semaphore_thread_entry(void *parameter)
{
	static rt_uint16_t num = 0;
	while(1)
	{
		if(num++ < 10)
		{
			rt_kprintf("\n release_semaphore_thread_entry release a semaphore. \n");
			rt_sem_release(dynamic_sem);						// 释放信号量
			rt_thread_mdelay(1000);								// 延时1000ms，并让出CPU使用权
		}
		else
		{
			rt_kprintf("\n demo end \n");
			return;
		}
	}
}

//------------------------------------------------------------
// @brief		获取信号量线程入口函数
// @param		parameter	参数
// @return		void
// Sample usage:
//------------------------------------------------------------
void get_semaphore_thread_entry(void *parameter)
{
	static rt_err_t result;
	static rt_uint8_t sum = 0;

	while(1)
	{
		// 通过永久等待的方式获取信号量，获取到了则执行sum++的操作
		result = rt_sem_take(dynamic_sem, RT_WAITING_FOREVER);

		if(result == RT_EOK)									// 获取信号量成功
		{
			sum++;
			rt_kprintf("\n get_semaphore_thread_entry take a semaphore. sum semaphore = %d \n", sum);
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------
// @brief		程创建以及启动
// @param		void
// @return		void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int semaphore_example(void)
{
	// 创建信号量
	dynamic_sem = rt_sem_create("dynamic_semaphore", 0 ,RT_IPC_FLAG_FIFO);
	// 创建线程控制块
	rt_thread_t tid;

	tid = rt_thread_create(
		"release_semaphore",									// 线程名称
		release_semaphore_thread_entry,							// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 栈空间大小
		4,														// 设置线程优先级，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		10);													// 时间片

	if(tid != RT_NULL)											// 线程创建成功
	{
		rt_thread_startup(tid);									// 运行该线程
	}

	tid = rt_thread_create(
		"get_semaphore",										// 线程名称
		get_semaphore_thread_entry,								// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 栈空间大小
		3,														// 设置线程优先级，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		10);													// 时间片

	if(tid != RT_NULL)											// 线程创建成功
	{
		rt_thread_startup(tid);									// 运行该线程
	}

	return 0;
}
// **************************** 代码区域 ****************************
