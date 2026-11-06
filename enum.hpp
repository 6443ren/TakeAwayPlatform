#pragma once
#include <string>

// 用户状态
enum class UserStatus
{
    NORMAL = 1,    // 正常可用
    DISABLE = 0    // 禁用
};

// 菜品上下架状态
enum class DishStatus
{
    ON_SALE = 1,   // 上架售卖
    OFF_SALE = 0   // 下架
};

// 菜品分类状态
enum class CategoryStatus
{
    NORMAL = 1,
    DISABLE = 0
};

// 订单状态（与数据库保持一致）
enum class OrderStatus
{
    PENDING_PAY = 0,    // 待支付
    PAID = 1,           // 已支付待接单
    ACCEPTED = 2,      // 商家已接单
    MAKING = 3,        // 制作中
    WAITING_PICK = 4,  // 待取餐
    DELIVERING = 5,    // 配送中
    DELIVERED = 6,     // 已送达
    FINISHED = 7,      // 订单完成
    CANCELLED = 8,     // 订单取消
    REFUNDING = 9,     // 退款中
    REFUNDED = 10,     // 已退款
    EXCEPTION = 11     // 异常
};

// 配送模式
enum class DeliveryMode
{
    SELF_PICK = 0,     // 到店自取
    PLATFORM_DELIVER = 1 // 平台配送
};

// 支付方式
enum class PayType
{
    WX_PAY = 1,
    ALIPAY = 2
};

// 角色编码映射函数（替代字符串枚举）
inline std::string getRoleCodeStr(int code)
{
    switch(code)
    {
        case 0: return "USER";
        case 1: return "MERCHANT";
        case 2: return "RIDER";
        case 3: return "ADMIN";
        default: return "USER";
    }
}
enum class RoleCode
{
    USER = 0,
    MERCHANT = 1,
    RIDER = 2,
    ADMIN = 3
};