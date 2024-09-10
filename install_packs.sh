#!/bin/bash

# 更新系统软件包
sudo apt update

# 安装必要的依赖包，自动确认 '-y'
sudo apt install -y \
    apt-transport-https \
    ca-certificates \
    curl \
    software-properties-common

# 添加 Docker GPG 密钥
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg

# 添加 Docker 的 APT 存储库
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

# 再次更新包索引
sudo apt update

# 安装 Docker 包，自动确认 '-y'
sudo apt install -y docker-ce docker-ce-cli containerd.io
