#pragma once
#include "db.hpp"
#include "entity.hpp"
#include "enum.hpp"
#include <vector>
#include <optional>

class OrderService
{
public:
    // 创建订单（主表+多条订单明细，事务保证原子性）
    bool createOrder(Order &order, const std::vector<OrderItem> &itemList);

    // 根据订单号查询订单主表
    std::optional<Order> getOrderByNo(const std::string &orderNo);
    
    // 根据订单ID查询订单主表
    std::optional<Order> getOrderById(uint64_t orderId);

    // 根据用户ID查询所有订单
    std::vector<Order> listOrderByUser(uint64_t userId, int limit = 20, int offset = 0);

    // 根据商家ID查询店铺订单
    std::vector<Order> listOrderByMerchant(uint64_t merchantId);

    // 查询所有订单（管理端用）
    std::vector<Order> listAllOrders(int limit = 50, int offset = 0);

    // 修改订单状态
    int updateOrderStatus(const std::string &orderNo, OrderStatus status);

    // 根据订单号查询订单明细
    std::vector<OrderItem> listOrderItemByNo(const std::string &orderNo);

    // 支付订单
    bool payOrder(const std::string &orderNo, PayType payType);

    // 取消未支付订单
    int cancelUnpaidOrder(const std::string &orderNo);

    // 获取商家今日订单数
    int getTodayOrderCount(uint64_t merchantId);

    // 获取商家今日营业额
    double getTodayRevenue(uint64_t merchantId);

    // 获取商家待处理订单数
    int getPendingOrderCount(uint64_t merchantId);
};