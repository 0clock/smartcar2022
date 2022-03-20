
import seekfree
from seekfree import Timer


def tick(s):            # 定义一个需要周期运行的函数
    print('This is a callback function.')
    print(s.count())    # 打印进入了多少次回调函数


seekfree.init()         # 在创建定时器前务必调用此函数进行初始化
tim = Timer(1, 1000)    # 创建一个定时器 1：定时器通道（可设置1-128） 1000：定时器每1000ms触发并调用一次回调函数
tim.callback(tick)      # 设置定时器的回调函数

#tim.help()             # 可以通过调用help查看 有哪些函数接口以及参数介绍
#tim.disable()          # 关闭定时器
#tim.enable()           # 开启定时器
#tim.count()            # 可以通过count函数获取定时器触发了多少次

while(True):
    pass

