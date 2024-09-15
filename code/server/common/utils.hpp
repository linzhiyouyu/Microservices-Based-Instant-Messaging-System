//实现通用工具

#include <iostream>
#include <string>
#include <atomic>
#include <random>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "logger.hpp"
namespace chat_im {
    std::string uuid() {
        //生成一个由16位随机字符组成的字符串作为唯一ID
        //1. 生成6个0~255之间的随机数字，转化为16进制
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(0, 255);
        std::stringstream ss;
        for(int i = 0; i < 6; ++i) {
            if(i == 2)
                ss << "-";
            ss << std::setw(2) << std::setfill('0') << std::hex << distribution(generator);
        }
        ss << "-";
        static std::atomic<short> idx(0);
        short tmp = idx.fetch_add(1);
        ss << std::setw(4) << std::setfill('0') << std::hex << tmp;
        return ss.str();
    }
    bool readFile(const std::string& filename, std::string& body) {
        //读取文件中所有数据放入body
        std::ifstream ifs(filename, std::ios::binary | std::ios::in);
        if(ifs.is_open() == false) {
            ERROR("打开文件{}失败", filename);
            return false;
        } 
        ifs.seekg(0, std::ios::end);
        size_t flen = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        body.resize(flen);
        ifs.read(&body[0], flen);
        if(ifs.good() == false) {
            ERROR("读取文件{}数据失败", filename);
            ifs.close();
            return false;
        }
        ifs.close();
        return true;
    }
    bool writeFile(const std::string& filename, const std::string& body) {
        //将body中的数据写入到文件中
        std::ofstream ofs(filename, std::ios::binary | std::ios::out | std::ios::trunc);
        if(ofs.is_open() == false) {
            ERROR("打开文件{}失败", filename);
            return false;
        } 
        ofs.write(body.c_str(), body.size());
        if(ofs.good() == false) {
            ERROR("读取文件{}数据失败", filename);
            ofs.close();
            return false;
        }
        ofs.close();
        return true;
    }
}