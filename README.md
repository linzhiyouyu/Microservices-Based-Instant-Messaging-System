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


## 服务注册中心

### 使用`etcd`键值存储系统
1. 服务注册：服务启动时，向`etcd`注册自己的地址和端口。
2. 服务发现：客户端通过`etcd`获取服务的地址和端口，用于远程调用。
3. 健康检查：服务定期向`etcd`发送心跳，以维持其注册信息的有效性。

对`etcd-client-api`进行二次封装，实现两种类型的客户端
1. 服务注册客户端：向`etcd`注册并保活
2. 服务发现客户端：从服务端查找数据，并进行改变事件监控


### 使用`brpc`远程调用框架
使用`brpc`远程调用框架，将本地数据处理的过程交给服务器来处理，利用服务端更强的算力来解决问题。

### `etcd`与`brpc`整合
`brpc`针对特定的请求来向服务端进行`rpc`调用从而构建响应，但是具体向哪台服务器请求需要由`etcd`实现的注册中心来进行配合。
更具体地，通过`etcd`注册中心就够获知，"谁"能提供"什么"服务，进行能够连接它，从而发起服务调用。由于不同的服务有不同的调用接口，功能也不一样，所以无法对具体的调用进行封装，所以这里的思想就是管理`channel`通信信道（将不同主机节点的通信信道管理起来，如果有多个节点能够提供相同的服务，采用`RR`轮转来进行负载均衡）。

初步架构：
服务注册端
1. 构造`Echo`服务
2. 搭建`RPC`服务器
3. 运行`RPC`服务
4. 向`etcd`注册中心注册本`RPC`节点提供的服务
服务发现端
1. 构造`RCP`信道管理对象
2. 构造服务发现对象
3. 通过`RPC`信道管理对象，获取提供`Echo`服务信道
4. 发起`RPC`调用

## 关键词搜索子服务
使用`elasticsearch`来进行搜索
使用场景：
1. 添加好友时关键字搜索
2. 查询历史聊天消息时关键字搜索


关于`httplib`与`websocketpp`的选择，一开始是想只用后者，因为既支持`http`又支持`websocket`比较方便。
但是后来在实际并发测试的时候不知道为什么`websocketpp`总是会漏掉一些请求或者一些意料之外的异常。
处于稳定性考虑，`http`协议通信采用`httplib`。

`redis`
使用场景:
1. 短信验证码过期设置
2. 用户再次登录身份认证
3. 群聊

`odb`
使用`odb`来构建对象关系映射，将数据结构与数据库表进行映射，通过对数据结构的操作即可完成对表的`curd`



## 微服务架构：
![alt text](/image/image.png)
1. 网关子服务
与客户端直接交互，用于接收客户端不同类型的请求。经过用户鉴权之后将不同的请求分发到各个子服务进行处理，接收到响应后返回给客户端。
用户鉴权：客户端在登录成功后，后台为客户端创建登录会话并向客户端返回一个登录会话ID。往后客户端发起的请求需要携带ID，否则视为未登录。非登录状态下不允许提供除注册/登录/验证码获取之外的所有服务。


2. 消息转发子服务
![alt text](/image/image1.png)
1) 对消息进行组织发送给网关。
2) 网关通过消息转发子服务来获取到该用户当前会话中有哪些成员，获取到成员之后组织好基本信息再告知网关，由网关来进行回复。
3) 离线用户收不到消息，所以将消息交给RabbitMQ进行"持久化"。


<!-- 2. 好友管理子服务
管理好友相关信息以及聊天会话信息
聊天会话：描述一个会话中有哪些成员，描述某条消息属于哪个会话
1) 两个用户组成好友关系，此后会进行聊天，为这两个用户创建一个聊天会话
2) 多个用户组成聊天群组，多者之间此后会进行聊天，为多个用户创建一个聊天会话

负责业务：
1) 用户(关键字: 昵称/ID/手机号)搜索，以便于添加好友。
2) 申请好友：告诉服务端自己要申请xx为自己的好友，服务器(websocketpp)进行事件推送。
3) 获取最新好友申请事件：某个客户端此前可能处于离线，当上线时获取一下之前的好友申请，进行处理（同意/拒绝），服务端再进行推送。
4) 获取好友列表：用户上线后获取好友列表进行展示。
5)  -->