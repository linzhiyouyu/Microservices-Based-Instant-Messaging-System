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