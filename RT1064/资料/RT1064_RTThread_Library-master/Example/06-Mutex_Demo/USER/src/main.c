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
static rt_mutex_t dynamic_mutex = RT_NULL;						//信号量控制块指针

static rt_uint32_t num1 = 0;
static rt_uint32_t num2 = 0;
// **************************** 变量定义 ****************************

// **************************** 代码区域 ****************************
void thread1_entry (void *parameter);
void thread2_entry (void *parameter);
int mutex_example(void);

// 使用INIT_APP_EXPORT宏自动初始化，也可以通过在其他线程内调用mutex_example函数进行初始化
INIT_APP_EXPORT(mutex_example);									// 应用初始化

// 使用互斥量之前 请先查看 rtconfig.h 中的 RT_USING_MUTEX 宏定义是否定义
// 使用互斥量之前 请先查看 rtconfig.h 中的 RT_USING_MUTEX 宏定义是否定义
// 使用互斥量之前 请先查看 rtconfig.h 中的 RT_USING_MUTEX 宏定义是否定义

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
void thread1_entry (void *parameter)
{
	while(1)
	{
		rt_mutex_take(dynamic_mutex, RT_WAITING_FOREVER);		// 获取互斥量，一直等待
		num1++;
		rt_thread_mdelay(100);
		num2++;
		rt_mutex_release(dynamic_mutex);						// 释放互斥量
	}
}

//------------------------------------------------------------
// @brief		获取信号量线程入口函数
// @param		parameter	参数
// @return		void
// Sample usage:
//------------------------------------------------------------
void thread2_entry (void *parameter)
{
	rt_thread_t tid;

	while(1)
	{
		// 线程2获取到互斥量后，判断num1和num2 的值是否相同，相同表示互斥量起到了锁的作用。
		rt_mutex_take(dynamic_mutex, RT_WAITING_FOREVER);		// 获取互斥量，一直等待
		if(num1 == num2)
		{
			rt_kprintf("\n num1 = num2 , num1 = %d  num2 = %d\n", num1, num2);
		}
		else
		{
			rt_kprintf("\n num1 != num2, num1 = %d  num2 = %d\n", num1, num2);
		}
		if(10 == num1)											// 发送10次后 删除互斥量与线程
		{
			// 删除互斥量
			rt_mutex_delete(dynamic_mutex);
			rt_kprintf("\n delete dynamic_mutex!\n");

			// 获取线程1的控制块指针
			tid = rt_thread_find("thread1");

			// 删除线程1
			rt_thread_delete(tid);
			rt_kprintf("\n delete thread1!\n");

			// 退出循环 线程2自动删除
			break;
		}
		rt_mutex_release(dynamic_mutex);						// 释放互斥量
	}

	rt_kprintf("\n thread2 exit!\n");
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		线程创建以及启动
// @param		void
// @return		void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int mutex_example(void)
{
	// 线程控制块指针
	rt_thread_t tid;

	// 创建动态互斥量
	dynamic_mutex = rt_mutex_create("dynamic mutex", RT_IPC_FLAG_FIFO);
	if (dynamic_mutex == RT_NULL)								// 线程控制块创建成功
	{
		rt_kprintf("create dynamic mutex failed.\n");
		return -1;
	}
	// 创建线程控制块
	tid = rt_thread_create(
		"thread1",												// 线程名称
		thread1_entry,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 栈空间大小
		3,														// 设置线程优先级，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		10);													// 时间片

	if(tid != RT_NULL)											// 线程创建成功
	{
		// 运行该线程
		rt_thread_startup(tid);
	}

	tid = rt_thread_create(
		"thread2",												// 线程名称
		thread2_entry,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 栈空间大小
		4,														// 设置线程优先级，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		10);													// 时间片

	if(tid != RT_NULL)											// 线程创建成功
	{
		// 运行该线程
		rt_thread_startup(tid);
	}

	return 0;
}
// **************************** 代码区域 ****************************
