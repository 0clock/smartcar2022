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
static rt_uint8_t thread2_stack[1024];						// 静态线程要用到的栈数组
struct rt_thread thread2_thread;							// 静态线程控制块
// **************************** 变量定义 ****************************

// **************************** 代码区域 ****************************
void thread1_entry (void *parameter);
void thread2_entry (void *parameter);
int dynamic_thread_example (void);
int static_thread_example (void);

//使用INIT_APP_EXPORT宏自动初始化，也可以通过在其他线程内调用dynamic_thread_example函数进行初始化
INIT_APP_EXPORT(dynamic_thread_example);					// 应用初始化

//使用INIT_APP_EXPORT宏自动初始化，也可以通过在其他线程内调用static_thread_example函数进行初始化
INIT_APP_EXPORT(static_thread_example);						// 应用初始化

// 静态创建方法
// 当我们没有开启RT_USING_HEAP宏定义我们只能使用静态的方法创建线程、信号量、互斥量等等
// 或者当我们想要指定控制块控制块或者栈的位置的时候，也可以用静态创建方法

// 动态创建方法
// 动态创建务必开启RT_USING_HEAP宏定义
// 动态创建好处在于我们不用自己定义控制块或者栈数组，创建的时候填写的参数更加的少非常的方便
// 如果HEAP大小不够了，可以在board.c中找到RT_HEAP_SIZE宏进行修改

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
// @brief		线程1入口
// @param		parameter	参数
// @return		void
// Sample usage:
//------------------------------------------------------------
void thread1_entry(void *parameter)
{
	while(1)
	{
		rt_kprintf("dynamic thread is running.\n");
		rt_thread_mdelay(1000);
	}
}

//------------------------------------------------------------
// @brief		线程2入口
// @param		parameter	参数
// @return		void
// Sample usage:
//------------------------------------------------------------
void thread2_entry(void *parameter)
{
	while(1)
	{
		rt_kprintf("static thread is running.\n");
		rt_thread_mdelay(500);
	}
}

//------------------------------------------------------------
// @brief		动态线程创建以及启动
// @param		void		空
// @return		void
// Sample usage:
//------------------------------------------------------------
int dynamic_thread_example(void)
{
	// 线程控制块指针
	rt_thread_t tid1;
	// 创建动态线程
	tid1 = rt_thread_create("thread1",						// 线程名称
		thread1_entry,										// 线程入口函数
		RT_NULL,											// 线程参数
		1024,												// 1024 个字节的栈空间
		5,													// 线程优先级为5，数值越小，优先级越高，0为最高优先级。
															// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		5);													// 时间片为5

	rt_kprintf("create dynamic thread.\n");
	if(tid1 != RT_NULL)										// 线程创建成功
	{
		rt_kprintf("thread1 dynamic thread create OK.\n");
		rt_thread_startup(tid1);							// 运行该线程
	}
	else													// 线程创建失败
	{
		rt_kprintf("thread1 dynamic thread create ERROR.\n");
		return 1;
	}

	return 0;
}

//------------------------------------------------------------
// @brief		静态线程创建以及启动
// @param		void		空
// @return		void
// Sample usage:
//------------------------------------------------------------
int static_thread_example(void)
{
	rt_err_t res;
	//创建静态线程
	res = rt_thread_init(
		&thread2_thread,									// 线程控制块
		"thread2",											// 线程名称
		thread2_entry,										// 线程入口函数
		RT_NULL,											// 线程参数
		thread2_stack,										// 栈的起始地址
		sizeof(thread2_stack),								// 栈大小
		3,													// 线程优先级为3，数值越小，优先级越高，0为最高优先级。
															// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		5													// 时间片为5
	);

	rt_kprintf("create static thread.\n");

	if(res == RT_EOK)										// 线程创建成功
	{
		rt_kprintf("thread2 static thread create OK\n");
		rt_thread_startup(&thread2_thread);					// 运行该线程

	}
	else													// 线程创建失败
	{
		rt_kprintf("thread2 static thread create ERROR\n");
		return 1;
	}
	return 0;
}
// **************************** 代码区域 ****************************
