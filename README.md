# Microservices-Based-Instant-Messaging-System
项目中的设计与困难摘要

## 使用etcd作为服务注册中心
所有的请求通过网关，由网关代理进行服务发现后找到对应服务节点进行`rpc`调用
【`rpc`采用默认配置: 延迟超过`500ms`并且`retry`次数超过`3`次则判定为服务下线】
考虑有可能在某些极端场景下服务并未下线由于网络问题导致服务发现中心误判，为了保证网关的请求不丢失可以采用消息队列来存储请求（未来迭代的时候考虑实现）。

## 消息存储子服务
用户的聊天消息由`mysql`节点存储，由于又要支持用户使用关键字查询历史消息，所以涉及到`mysql`与`es`的同步问题。常见的主流方案: `canal + binlog + kafka`，但是由于都缺乏对于`c++`客户端的支持，所以退而求其次采用流水表的方式来保证最终一致性。更具体地，`mysql`创建流水表存储需要同步给`es`的消息，当消息成功被消费的时候再从流水表中删除数据。采用异步线程轮询流水表的方式来尽可能保证消息被成功消费，极端情况下考虑`3`次消费失败进行人工干预（未来迭代的时候考虑现）。

## 消息转发子服务
客户端与网关通信采用`http`，网关向客户端主动推送消息采用`websocket`。本来使用`websocket`通信即可，但是当数据量大的时候（例如上传文件）出现了乱码，重装后也未能解决，所以另启用一个线程采用`http`进行通信从而解决问题。
消息转发的逻辑：服务发现后向好友子服务进行请求，从而获取好友列表分别推送消息。考虑到可能有些用户并不在线，并不能获取到对应`conn`(`websocket`连接)进行主动推送，所以还需要在消息队列中存储一份，待客户端上线后进行`pull`（客户端的逻辑）。

## Redis在项目中的用途
基本`redis`配置：`maxmemory`的值设置为物理内存的`3/4`；启用惰性删除；兜底的内存淘汰策略采用`allkeys-lru`；全部采用后台异步`unlink`删除以防止`bigkey`阻塞。
没有用作缓存，均作为内存级数据库使用：
1. 短信验证码
2. 用户登录临时会话管理
3. 晚上`7:00`的时候针对`top5`登录的用户采用奖励积分的措施
特别地，针对第`3`点，虽然我们实际场景确实由于使用人数不多并不涉及高并发场景，但是为了考虑可扩展性以及健壮性，我们引入分布式锁来处理`top5`的用户，以防止出现"超卖"现象。最先了解到现成的分布式锁就是`redis`中基于`redlock`的`redisson`，但是由于并不对`c++`客户端支持，所以我们自己实现了分布式锁，参考`redisson`的源码后从几个方面考虑锁的设计：
1) 可重入性，类似`java`中的`synchronized`与`c++`中的`std::unique_lock<std::recursive_mutex>`均能保证锁的可重入性，所以`redis`使用`hset`来实现可重入的功能。
2) 不可抢占，使用`lua`来将判断是否是自己线程加的锁与`del`操作绑定来防止误删操作。
3) 高可用，`hset`设置`30s`过期时间的方式来解决线程因为某些极端原因宕掉后仍能保证锁的正确释放。同时可能异常的线程会恢复导致这个时间周期超过`30s`，采用异步定时器来每隔`20s`定时"续租"锁。

## MySQL
隔离级别调整为`RC`
表的设计都遵循第三范式，每个表不产生冗余，查询不避讳`join`。由于数据量少，查询性能不受影响，如果未来数据量大考虑反范式设计增加冗余度以提升查询效率。



