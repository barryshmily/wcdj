

GBK编码中防止串尾乱码的问题

http://blog.csdn.net/delphiwcdj/article/details/7746446


iconv文件编码转换

例如将一个GBK 编码的文件转换成UTF-8编码

iconv -f GBK -t UTF-8 file1 > file2

-f 原编码
-t 目标编码
-c 忽略无法转换的字符

