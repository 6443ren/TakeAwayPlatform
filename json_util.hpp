#pragma once
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <sstream>

class JsonBuilder {
public:
    JsonBuilder& startObject();
    JsonBuilder& endObject();
    JsonBuilder& startArray();
    JsonBuilder& endArray();
    JsonBuilder& key(const std::string& k);
    JsonBuilder& value(const std::string& v);
    JsonBuilder& value(const char* v);
    JsonBuilder& value(int v);
    JsonBuilder& value(long long v);
    JsonBuilder& value(uint64_t v);
    JsonBuilder& value(double v);
    JsonBuilder& value(bool v);
    JsonBuilder& null();
    JsonBuilder& comma();
    std::string build();

private:
    std::ostringstream oss;
    bool needComma = false;

    void addComma();
    std::string escape(const std::string& s);
};

std::string toJsonString(const std::string& key, const std::string& value);
std::string toJsonInt(const std::string& key, int value);
std::string toJsonLong(const std::string& key, long long value);
std::string toJsonDouble(const std::string& key, double value);
std::string toJsonBool(const std::string& key, bool value);

std::string successJson();
std::string successJson(const std::string& data);
std::string successJson(const std::string& key, const std::string& value);
std::string errorJson(int code, const std::string& msg);

std::map<std::string, std::string> parseQueryParams(const std::string& query);
std::string getParam(const std::map<std::string, std::string>& params, const std::string& key, const std::string& def = "");
int getParamInt(const std::map<std::string, std::string>& params, const std::string& key, int def = 0);
long long getParamLong(const std::map<std::string, std::string>& params, const std::string& key, long long def = 0);
