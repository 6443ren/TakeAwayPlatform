#pragma once
#include "enum.hpp"
#include <string>
#include <vector>
#include <cstdint>

enum class UserType {
    CUSTOMER = 0,
    MERCHANT = 1,
    RIDER = 2
};

// 1. 用户表 user
struct User
{
    uint64_t id;
    std::string account;
    std::string password;
    std::string user_name;
    std::string phone;
    int sex;
    std::string avatar;
    std::string description;
    UserType user_type;
    UserStatus status;
    std::string create_time;
    std::string update_time;
};

// 2. 角色表 role
struct Role
{
    uint64_t id;
    std::string role_code;
    std::string role_name;
    std::string description;
};

// 3. 用户角色中间表 user_role
struct UserRole
{
    uint64_t id;
    uint64_t user_id;
    uint64_t role_id;
    std::string create_time;
};

// 4. 商家表 merchant
struct Merchant
{
    uint64_t id;
    uint64_t user_id;
    std::string merchant_name;
    std::string logo_url;
    std::string shop_name;
    std::string shop_img;
    std::string address;
    std::string contact_phone;
    DeliveryMode delivery_mode;
    double start_price;
    double delivery_fee;
    int status;
    std::string description;
    std::string create_time;
    std::string update_time;
};

// 5. 菜品分类 category
struct Category
{
    uint64_t id;
    uint64_t merchant_id;
    std::string category_name;
    int sort_no;
    CategoryStatus status;
    std::string create_time;
};

// 6. 菜品表 dish
struct Dish
{
    uint64_t id;
    uint64_t merchant_id;
    uint64_t category_id;
    std::string dish_name;
    std::string image_url;
    double price;
    double original_price;
    int stock;
    int sold_count;
    std::string description;
    DishStatus status;
    std::string create_time;
    std::string update_time;
};

// 7. 购物车 cart
struct Cart
{
    uint64_t id;
    uint64_t user_id;
    uint64_t dish_id;
    int num;
    std::string create_time;
    std::string update_time;
};

// 8. 订单主表 orders
struct Order
{
    uint64_t id;
    std::string order_no;
    uint64_t user_id;
    uint64_t merchant_id;
    uint64_t rider_id;
    std::string receiver_name;
    std::string receiver_phone;
    std::string receiver_addr;
    double total_price;
    double pay_price;
    double delivery_fee;
    PayType pay_type;
    OrderStatus order_status;
    std::string pay_time;
    std::string finish_time;
    std::string create_time;
    std::string update_time;
};

// 9. 订单详情 order_item
struct OrderItem
{
    uint64_t id;
    uint64_t order_id;
    uint64_t dish_id;
    std::string dish_name;
    double dish_price;
    int num;
    std::string create_time;
};

// 10. 骑手表 rider
struct Rider
{
    uint64_t id;
    uint64_t user_id;
    std::string real_name;
    std::string id_card;
    std::string phone;
    int status;
    std::string create_time;
    std::string update_time;
};

// 11. 配送记录表 delivery
struct Delivery
{
    uint64_t id;
    uint64_t order_id;
    uint64_t rider_id;
    std::string pick_time;
    std::string deliver_time;
    int status;
    std::string create_time;
};

// 12. 地址簿 user_address
struct UserAddress
{
    uint64_t id;
    uint64_t user_id;
    std::string receiver;
    std::string phone;
    std::string address;
    int is_default;
    int status;
    std::string create_time;
};

// 13. 评价表 comment
struct Comment
{
    uint64_t id;
    uint64_t order_id;
    uint64_t user_id;
    uint64_t merchant_id;
    int score;
    std::string content;
    std::string img_url;
    int status;
    std::string create_time;
};

// 14. 退款记录 refund
struct Refund
{
    uint64_t id;
    std::string order_no;
    double refund_money;
    std::string reason;
    int refund_status;
    std::string create_time;
};