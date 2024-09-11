# Microservices-Based-Instant-Messaging-System


**安装需要的库及Docker**

```
chmod +x ./install_packs.sh
./install_packs.sh
```

## 不直接使用原生spdlog，而是进行二次封装
1. 避免单例的锁冲突，因此直接创建全局的线程安全日志器进行使用
2. 因为日志输出没有行号和文件名，使用宏进行二次封装来输出行号和文件名
3. 封装初始化接口，对于调试模式输出到stdout中，否则输出到文件中

## etcd
键值存储系统
`Client`对象（客户端操作句柄）：提供新增、获取数据的`api`；提供获取保活对象的`api`以及租约的`api`。
`KeepAlive`对象：提供获取租约`ID`的接口，可以针对租约进行不断续租（一直维持数据的有效性）。
`Response`对象：针对请求进行的相应
`Value`对象：存放键值对数据的对象
