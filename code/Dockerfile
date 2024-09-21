# 基于官方 Ubuntu 镜像
FROM ubuntu:latest

# 更新软件包列表并安装 vim、openssh-server 和 net-tools
RUN apt update && apt install -y \
    vim \
    openssh-server \
    net-tools

# 创建运行 sshd 需要的目录
RUN mkdir /var/run/sshd

# 设置 root 用户密码（可根据需要修改密码）
RUN echo 'root:yourpassword' | chpasswd

# 修改 sshd_config 以允许 root 登录
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

# 开放 22 端口用于 SSH 连接
EXPOSE 22

# 启动 SSH 服务并让容器保持运行
CMD ["/usr/sbin/sshd", "-D"]
