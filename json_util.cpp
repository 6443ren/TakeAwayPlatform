#include "json_util.hpp"
#include <iomanip>

JsonBuilder& JsonBuilder::startObject() {
    addComma();
    oss << "{";
    needComma = false;
    return *this;
}

JsonBuilder& JsonBuilder::endObject() {
    oss << "}";
    needComma = true;
    return *this;
}

JsonBuilder& JsonBuilder::startArray() {
    addComma();
    oss << "[";
    needComma = false;
    return *this;
}

JsonBuilder& JsonBuilder::endArray() {
    oss << "]";
    needComma = true;
    return *this;
}

JsonBuilder& JsonBuilder::key(const std::string& k) {
    addComma();
    oss << "\"" << escape(k) << "\":";
    needComma = false;
    return *this;
}

JsonBuilder& JsonBuilder::value(const std::string& v) {
    addComma();
    oss << "\"" << escape(v) << "\"";
    needComma = true;
    return *this;
}

JsonBuilder& JsonBuilder::value(const char* v) {
    return value(std::string(v));
}

JsonBuilder& JsonBuilder::value(int v) {
    addComma();
    oss << v;
    needComma = true;
    return *this;
}

JsonBuilder& JsonBuilder::value(long long v) {
    addComma();
    oss << v;
    needComma = true;
    return *this;
}

JsonBuilder& JsonBuilder::value(uint64_t v) {
    addComma();
    oss << v;
    needComma = true;
    return *this;
}

JsonBuilder& JsonBuilder::value(double v) {
    addComma();
    oss << std::fixed << std::setprecision(2) << v;
    needComma = true;
    return *this;
}

JsonBuilder& JsonBuilder::value(bool v) {
    addComma();
    oss << (v ? "true" : "false");
    needComma = true;
    return *this;
}

JsonBuilder& JsonBuilder::null() {
    addComma();
    oss << "null";
    needComma = true;
    return *this;
}

JsonBuilder& JsonBuilder::comma() {
    needComma = true;
    return *this;
}

std::string JsonBuilder::build() {
    return oss.str();
}

void JsonBuilder::addComma() {
    if (needComma) {
        oss << ",";
    }
}

std::string JsonBuilder::escape(const std::string& s) {
    std::string res;
    for (char c : s) {
        switch (c) {
            case '"': res += "\\\""; break;
            case '\\': res += "\\\\"; break;
            case '\n': res += "\\n"; break;
            case '\r': res += "\\r"; break;
            case '\t': res += "\\t"; break;
            default: res += c;
        }
    }
    return res;
}

std::string successJson() {
    return "{\"code\":0,\"message\":\"success\"}";
}

std::string successJson(const std::string& data) {
    return "{\"code\":0,\"message\":\"success\",\"data\":" + data + "}";
}

std::string successJson(const std::string& key, const std::string& value) {
    return "{\"code\":0,\"message\":\"success\",\"data\":{\"" + key + "\":\"" + value + "\"}}";
}

std::string errorJson(int code, const std::string& msg) {
    return "{\"code\":" + std::to_string(code) + ",\"message\":\"" + msg + "\"}";
}

std::map<std::string, std::string> parseQueryParams(const std::string& query) {
    std::map<std::string, std::string> params;
    size_t pos = 0;
    std::string q = query;
    while (pos < q.size()) {
        size_t amp = q.find('&', pos);
        std::string pair = (amp == std::string::npos) ? q.substr(pos) : q.substr(pos, amp - pos);
        size_t eq = pair.find('=');
        if (eq != std::string::npos) {
            std::string key = pair.substr(0, eq);
            std::string value = pair.substr(eq + 1);
            params[key] = value;
        }
        pos = (amp == std::string::npos) ? q.size() : amp + 1;
    }
    return params;
}

std::string getParam(const std::map<std::string, std::string>& params, const std::string& key, const std::string& def) {
    auto it = params.find(key);
    return (it != params.end()) ? it->second : def;
}

int getParamInt(const std::map<std::string, std::string>& params, const std::string& key, int def) {
    auto it = params.find(key);
    if (it == params.end()) return def;
    try {
        return std::stoi(it->second);
    } catch (...) {
        return def;
    }
}

long long getParamLong(const std::map<std::string, std::string>& params, const std::string& key, long long def) {
    auto it = params.find(key);
    if (it == params.end()) return def;
    try {
        return std::stoll(it->second);
    } catch (...) {
        return def;
    }
}
