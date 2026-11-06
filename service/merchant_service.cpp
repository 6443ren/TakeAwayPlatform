#include "merchant_service.hpp"
#include "util.hpp"
#include <sstream>

std::vector<Merchant> MerchantService::listAllMerchant() {
    std::vector<Merchant> list;
    std::string sql = "SELECT id,user_id,merchant_name,logo_url,address,business_hours,contact_phone,delivery_mode,start_price,delivery_fee,status,create_time,update_time FROM merchant WHERE status=1";
    auto res = query(sql);
    for (auto& row : res) {
        Merchant m;
        m.id = strToLL(row[0]);
        m.user_id = strToLL(row[1]);
        m.merchant_name = row[2];
        m.logo_url = row[3];
        m.shop_name = row[2];
        m.shop_img = row[3];
        m.address = row[4];
        m.contact_phone = row[6];
        m.delivery_mode = static_cast<DeliveryMode>(safeStoi(row[7]));
        m.start_price = safeStod(row[8]);
        m.delivery_fee = safeStod(row[9]);
        m.status = safeStoi(row[10]);
        m.create_time = row[11];
        m.update_time = row[12];
        list.push_back(m);
    }
    return list;
}

std::optional<Merchant> MerchantService::getMerchantById(uint64_t id) {
    std::string sql = "SELECT id,user_id,merchant_name,logo_url,address,business_hours,contact_phone,delivery_mode,start_price,delivery_fee,status,create_time,update_time FROM merchant WHERE id=" + std::to_string(id) + " LIMIT 1";
    auto res = query(sql);
    if (res.empty()) return std::nullopt;
    auto& row = res[0];
    Merchant m;
    m.id = strToLL(row[0]);
    m.user_id = strToLL(row[1]);
    m.merchant_name = row[2];
    m.logo_url = row[3];
    m.shop_name = row[2];
    m.shop_img = row[3];
    m.address = row[4];
    m.contact_phone = row[6];
    m.delivery_mode = static_cast<DeliveryMode>(safeStoi(row[7]));
    m.start_price = safeStod(row[8]);
    m.delivery_fee = safeStod(row[9]);
    m.status = safeStoi(row[10]);
    return m;
}

std::optional<Merchant> MerchantService::getMerchantByUserId(uint64_t userId) {
    std::string sql = "SELECT id,user_id,merchant_name,logo_url,address,business_hours,contact_phone,delivery_mode,start_price,delivery_fee,status,create_time,update_time FROM merchant WHERE user_id=" + std::to_string(userId) + " LIMIT 1";
    auto res = query(sql);
    if (res.empty()) return std::nullopt;
    auto& row = res[0];
    Merchant m;
    m.id = strToLL(row[0]);
    m.user_id = strToLL(row[1]);
    m.merchant_name = row[2];
    m.logo_url = row[3];
    m.shop_name = row[2];
    m.shop_img = row[3];
    m.address = row[4];
    m.contact_phone = row[6];
    m.delivery_mode = static_cast<DeliveryMode>(safeStoi(row[7]));
    m.start_price = safeStod(row[8]);
    m.delivery_fee = safeStod(row[9]);
    m.status = safeStoi(row[10]);
    m.create_time = row[11];
    m.update_time = row[12];
    return m;
}

int MerchantService::addMerchant(const Merchant& merchant) {
    std::string now = getNowTime();
    std::stringstream ss;
    ss << "INSERT INTO merchant(user_id,merchant_name,logo_url,address,business_hours,contact_phone,delivery_mode,start_price,delivery_fee,status,create_time,update_time) "
       << "VALUES(" << merchant.user_id << ",'" << merchant.shop_name << "','" << merchant.shop_img << "','" << merchant.address
       << "','','" << merchant.contact_phone << "'," << static_cast<int>(merchant.delivery_mode)
       << "," << merchant.start_price << "," << merchant.delivery_fee << "," << merchant.status
       << ",'" << now << "','" << now << "')";
    return executeUpdate(ss.str());
}

int MerchantService::updateMerchant(const Merchant& merchant) {
    std::string now = getNowTime();
    std::stringstream ss;
    ss << "UPDATE merchant SET merchant_name='" << merchant.shop_name
       << "',logo_url='" << merchant.shop_img << "',address='" << merchant.address
       << "',contact_phone='" << merchant.contact_phone << "',delivery_mode=" << static_cast<int>(merchant.delivery_mode)
       << ",start_price=" << merchant.start_price << ",delivery_fee=" << merchant.delivery_fee
       << ",status=" << merchant.status << ",update_time='" << now << "' WHERE id=" << merchant.id;
    return executeUpdate(ss.str());
}
