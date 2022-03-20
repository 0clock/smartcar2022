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
// 创建一个字符串
static char mb_str1[] = "i am a mail!";

// 邮箱控制块指针
static rt_mailbox_t mb;
// **************************** 变量定义 ****************************

// **************************** 代码区域 ****************************
void thread1_entry (void *parameter);
void thread2_entry (void *parameter);
int mailbox_example (void);

// 使用INIT_APP_EXPORT宏自动初始化，也可以通过在其他线程内调用mailbox_example函数进行初始化
INIT_APP_EXPORT(mailbox_example);								// 应用初始化

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
void thread1_entry (void *parameter)
{
	char *str;

	rt_kprintf("thread1:try to recv a mail.\n");

	if(rt_mb_recv(mb,											// 邮箱控制块
		(rt_ubase_t *)&str,										// 接收邮箱的字符串 接收 32bit 大小的邮件
		RT_WAITING_FOREVER)										// 一直等待
		== RT_EOK)
	{
		rt_kprintf("thread1:get a mail from mailbox.\nthe content :%s\n", str);	// 输出接收信息
	}
	rt_mb_delete(mb);											// 删除邮箱
}

//------------------------------------------------------------
// @brief		线程入口
// @param		parameter	参数
// @return		void
// Sample usage:
//------------------------------------------------------------
void thread2_entry (void *parameter)
{
	rt_kprintf("thread2:try to send a mail.\n");
	// 这里是使用的方式是将字符串的地址取值 得到 32bit 的地址值发送
	rt_mb_send(mb, (rt_uint32_t)&mb_str1);						// 发送邮件
}

//------------------------------------------------------------
// @brief		邮箱创建以及启动
// @param		void
// @return		void
// Sample usage:
//------------------------------------------------------------
int mailbox_example (void)
{
	rt_thread_t t1,t2;

	// 创建邮箱
	mb = rt_mb_create("mb",
		4,														// 设置 缓冲区为 4 封邮件
		RT_IPC_FLAG_FIFO										// 先进先出
		);

	t1 = rt_thread_create(
		"thread1",												// 线程名称
		thread1_entry,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 栈空间大小
		4,														// 设置线程优先级
		5);														// 时间片

	if(t1 != RT_NULL)											// 线程创建成功
	{
		rt_thread_startup(t1);									// 运行该线程
	}

	t2 = rt_thread_create(
		"thread2",												// 线程名称
		thread2_entry,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 栈空间大小
		3,														// 设置线程优先级
		5);														// 时间片

	if(t2 != RT_NULL)											// 线程创建成功
	{
		rt_thread_startup(t2);									// 运行该线程
	}

	return 0;
}
// **************************** 代码区域 ****************************
