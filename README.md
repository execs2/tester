##压测工具

主要用libcurl实现的压测工具，写这个主要测试用于RPC以及其他API接口的服务器性能<br>
多线程、短连接，快速发包，记录丢包的数量<br>
因为测试机都是Windows，所以实现的是Windows版本<br>

##INSTALL:
```Bash
使用visual studio 2015生成，依赖libcurl，需要自行下载libcurl，项目设置libcurl库路径才能正确编译
```

##Usage:
```Bash
tester -t 1 -h http://127.0.0.1 -c 2 -p D:\\1.txt
```

t：压测的线程数，可以看情况多开点 <br>
h：压测目标host API 的Url<br>
c：数据包个数，c参数多少个，p参数就要对应多少个，发送的时候，随机选择一个数据包<br>
p：数据包的文件路径，多个的话空格隔开，可以用postman生成对应的数据包文件<br>

##TODO:
1.修改成读取某个目录下全部文件作为发送的数据包
2.自定义header

##我的邮箱：
  
646452100@qq.com
