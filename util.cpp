#include "util.hpp"
#include <uuid/uuid.h>
#include <sstream>
#include <iomanip>
#include <random>
#include <ctime>
#include <cctype>
#include <algorithm>

std::string generateUUID()
{
    uuid_t uuid;
    char buf[40];
    uuid_generate(uuid);
    uuid_unparse(uuid, buf);
    return std::string(buf);
}

std::string getNowTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string createOrderNo()
{
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1000, 9999);

    std::ostringstream oss;
    oss << ms << dist(gen);
    std::string num = oss.str();
    return num.substr(num.length() - 16, 16);
}

std::string timeStampToString(long long ts)
{
    std::time_t t = ts / 1000;
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

long long strToLL(const std::string& s)
{
    std::istringstream iss(s);
    long long res = 0;
    iss >> res;
    return res;
}

double safeStod(const std::string& s)
{
    if (s.empty()) return 0.0;
    try { return std::stod(s); } catch (...) { return 0.0; }
}

int safeStoi(const std::string& s)
{
    if (s.empty()) return 0;
    try { return std::stoi(s); } catch (...) { return 0; }
}

long long safeStoll(const std::string& s)
{
    if (s.empty()) return 0;
    try { return std::stoll(s); } catch (...) { return 0; }
}

std::string urlDecode(const std::string& encoded)
{
    std::string decoded;
    decoded.reserve(encoded.size());
    
    for (size_t i = 0; i < encoded.size(); ++i) {
        if (encoded[i] == '%' && i + 2 < encoded.size()) {
            int hex1 = encoded[i + 1];
            int hex2 = encoded[i + 2];
            
            if (std::isxdigit(hex1) && std::isxdigit(hex2)) {
                int value = 0;
                if (hex1 >= '0' && hex1 <= '9') value += (hex1 - '0') * 16;
                else if (hex1 >= 'A' && hex1 <= 'F') value += (hex1 - 'A' + 10) * 16;
                else if (hex1 >= 'a' && hex1 <= 'f') value += (hex1 - 'a' + 10) * 16;
                
                if (hex2 >= '0' && hex2 <= '9') value += hex2 - '0';
                else if (hex2 >= 'A' && hex2 <= 'F') value += hex2 - 'A' + 10;
                else if (hex2 >= 'a' && hex2 <= 'f') value += hex2 - 'a' + 10;
                
                decoded += static_cast<char>(value);
                i += 2;
                continue;
            }
        } else if (encoded[i] == '+') {
            decoded += ' ';
            continue;
        }
        decoded += encoded[i];
    }
    
    return decoded;
}