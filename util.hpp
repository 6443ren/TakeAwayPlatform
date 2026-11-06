#pragma once
#include <string>
#include <chrono>

// 生成UUID字符串（用于订单ID、图片唯一标识）
std::string generateUUID();

// 获取当前时间，格式：yyyy-MM-dd HH:mm:ss
std::string getNowTime();

// 生成16位唯一订单号
std::string createOrderNo();

// 时间戳转格式化日期字符串
std::string timeStampToString(long long ts);

// 字符串转数字
long long strToLL(const std::string& s);

// 安全数值转换（空字符串返回0）
double safeStod(const std::string& s);
int safeStoi(const std::string& s);
long long safeStoll(const std::string& s);

// URL解码
std::string urlDecode(const std::string& encoded);