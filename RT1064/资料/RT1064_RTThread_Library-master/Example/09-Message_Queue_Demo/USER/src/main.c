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
rt_mq_t message_queue;										//消息队列控制块指针
// **************************** 变量定义 ****************************

// **************************** 代码区域 ****************************
void thread1_entry (void *paremeter);
void thread2_entry (void *paremeter);
int message_queue_example (void);

// 使用INIT_APP_EXPORT宏自动初始化，也可以通过在其他线程内调用message_queue_example函数进行初始化
INIT_APP_EXPORT(message_queue_example);

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
// @brief		线程入口
// @param		parameter	参数
// @return		void
// Sample usage:
//------------------------------------------------------------
void thread1_entry (void *paremeter)
{
	uint8 recv_test;
	rt_thread_mdelay(10);									// 等待FINSH初始化完毕
	while(1)
	{
		// 从消息队列获取消息
		rt_mq_recv(message_queue,							// 消息队列控制块
			(void *)&recv_test,								// 接收数据的地址
			1,												// 接收数量
			RT_WAITING_FOREVER);							// 一直等待有数据采退出接收函数

		// 检查收到的数据 与发送数据匹配的话 输出接收到信息
		if(10 == recv_test)
		{
			rt_kprintf("Received data = %d\n", 10);
		}
	}
	rt_kprintf("\n thread1 exit!\n");
}

//------------------------------------------------------------
// @brief		线程入口
// @param		parameter	参数
// @return		void
// Sample usage:
//------------------------------------------------------------
void thread2_entry (void *paremeter)
{
	rt_uint8_t send_test = 10;
	static rt_uint8_t count = 0;
	rt_thread_t tid = rt_thread_find("thread1");			// 线程控制块
	rt_thread_mdelay(10);									// 等待FINSH初始化完毕
	while(1)
	{
		// 发送一个消息到消息队列
		rt_mq_send(message_queue,							// 消息队列控制块
			(void *)&send_test,								// 发送数据的地址
			1);												// 发送字节数

		// 延时100毫秒 释放CPU控制权
		rt_thread_mdelay(100);
		if(count++ > 10)
		{
			rt_thread_delete(tid);							// 删除线程1
			rt_kprintf("\n delete thread1!\n");
			break;
		}
	}
	rt_kprintf("\n thread2 exit!\n");
}

//------------------------------------------------------------
// @brief		线程创建以及启动
// @param		void
// @return		void
// Sample usage:
//------------------------------------------------------------
int message_queue_example (void)
{
	// 线程控制块指针
	rt_thread_t thread1,thread2;

	// 创建队列
	message_queue = rt_mq_create("mq1",						// 消息队列名称
		1,													// 一个消息的大小为一个字节
		10,													// 一共能存放10个消息
		RT_IPC_FLAG_FIFO);									// 如果有多个线程等待，按照先来先得分配


	// 创建动态线程
	thread1 = rt_thread_create("thread1",					// 线程名称
		thread1_entry,										// 线程入口
		RT_NULL,											// 线程参数
		1024,												// 线程栈大小
		3,													// 线程优先级
		10);												// 线程时间片
	// 检查线程控制块不为空则启动线程
	if(thread1 != RT_NULL)
	{
		rt_thread_startup(thread1);
	}

	// 创建动态线程
	thread2 = rt_thread_create("thread2",					// 线程名称
		thread2_entry,										// 线程入口
		RT_NULL,											// 线程参数
		1024,												// 线程栈大小
		4,													// 线程优先级
		10);												// 线程时间片
	// 检查线程控制块不为空则启动线程
	if(thread2 != RT_NULL)
	{
		rt_thread_startup(thread2);
	}
	return 0;
}
// **************************** 代码区域 ****************************
