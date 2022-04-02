#文件路径
origin_file = "/sd/1.txt"
new_file ="/sd/poem.txt"

#文件读写时都可能产生IOError，为了保证无论是否出错都能正确地关闭文件采用with open(...) as ...的写法
#参数"w"表示只写，如果该文件已存在则将其覆盖。如果该文件不存在，创建新文件
print("“hello”写到1.txt")
with open(origin_file,"w") as f:
   f.write("hello\r")

#参数"a"表示追加写入，如果该文件不存在，创建新文件进行写入
print("“seekfree”写到1.txt")
with open(origin_file,"a") as f:
    f.write("seekfree\r")

#参数"r"表示只读
print("读取1.txt中的内容：")
with open(origin_file,"r") as f:
    print(f.read())

print("诗句“落霞与孤鹜齐飞，秋水共长天一色”写到poem.txt")
with open(new_file,"w") as f:
    f.write("落霞与孤鹜齐飞，\r秋水共长天一色。\r")

#要检查是否写入成功，执行完运行操作后，给OpenART mini断一次电，再插上查看sd中的txt文档


