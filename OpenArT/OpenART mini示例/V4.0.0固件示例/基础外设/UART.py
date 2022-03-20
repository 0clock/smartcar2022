from machine import UART
uart = UART(2, baudrate=115200)     # 初始化串口 波特率设置为115200 TX是B12 RX是B13
uart.write("uart test\r\n")         # 发送字符串
uart_num = 0                        # 定义一个变量
uart_array = [48,49,50,51,52,53,54,55,56,57]  # 定义一个列表 保存数字
uart.write(bytearray(uart_array))   # 发送列表
uart.write(bytearray([0x41]))       # 发送一个十六进制数据
uart_num = -10
uart.write(uart_num.to_bytes(1, 'little'))  # 发送一个变量
while(True):

    uart_num = uart.any()       # 获取当前串口数据数量
    if(uart_num):
        uart_str = uart.read(uart_num) # 读取串口数据
        uart.write(uart_str)    # 将读取到的串口数据发回
        # uart.read会自动在数据末尾添加\n的操作，如果想去掉可以用strip()去掉
        # 例如 uart_str = uart.read(uart_num).strip()
        # 这样 uart_str得到的数据就不会被自动添加\n

