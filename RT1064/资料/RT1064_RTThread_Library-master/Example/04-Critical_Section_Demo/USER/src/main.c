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
static rt_uint32_t count = 0;
// **************************** 变量定义 ****************************

// **************************** 代码区域 ****************************
void thread1_entry (void *parameter);
void thread2_entry (void *parameter);
int critical_section_example(void);

// 使用INIT_APP_EXPORT宏自动初始化，也可以通过在其他线程内调用critical_section_example函数进行初始化
INIT_APP_EXPORT(critical_section_example);					// 应用初始化

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
// @brief		线程1入口函数
// @param		parameter	参数
// @return		void
// Sample usage:
//------------------------------------------------------------
void thread1_entry (void *parameter)
{
	while(1)
	{
		// 调度器上锁，上锁后将不再切换到其他线程，仅响应中断 
		rt_enter_critical();
		// 以下进入临界区 

		count += 10;										// 计数值+10

		// 调度器解锁 
		rt_exit_critical();

		rt_kprintf("thread = %d , count = %d\n", 10, count);
		rt_thread_mdelay(1000);
	}
}

//------------------------------------------------------------
// @brief		线程2入口函数
// @param		parameter	参数
// @return		void
// Sample usage:
//------------------------------------------------------------
void thread2_entry (void *parameter)
{
	while(1)
	{
		// 调度器上锁，上锁后将不再切换到其他线程，仅响应中断
		rt_enter_critical();
		// 以下进入临界区

		count += 20;										// 计数值+20

		// 调度器解锁
		rt_exit_critical();

		rt_kprintf("thread = %d , count = %d\n", 20, count);
		rt_thread_mdelay(2000);
	}
}

//------------------------------------------------------------
// @brief		线程创建以及启动
// @param		void		空
// @return		void
// Sample usage:
//------------------------------------------------------------
int critical_section_example (void)
{
	// 线程控制块指针
	rt_thread_t tid;
	// 创建动态线程
	tid = rt_thread_create("thread_10",						// 线程名称
		thread1_entry,										// 线程入口函数
		RT_NULL,											// 线程参数
		1024,												// 栈空间
		5,													// 线程优先级为5，数值越小，优先级越高，0为最高优先级。
															// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		10);												// 时间片

	if(tid != RT_NULL)										// 线程创建成功
	{
		// 运行该线程
		rt_thread_startup(tid);
	}

	tid = rt_thread_create("thread_20",						// 线程名称
		thread2_entry,										// 线程入口函数
		RT_NULL,											// 线程参数
		1024,												// 栈空间
		3,													// 线程优先级为3，数值越小，优先级越高，0为最高优先级。
															// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		10);												// 时间片

	if(tid != RT_NULL)										// 线程创建成功
	{
		rt_thread_startup(tid);								// 运行该线程
	}

	return 0;
}
// **************************** 代码区域 ****************************
