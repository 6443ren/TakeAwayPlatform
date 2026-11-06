#include "order_service.hpp"
#include "util.hpp"
#include <sstream>
#include <optional>

bool OrderService::createOrder(Order &order, const std::vector<OrderItem> &itemList)
{
    auto &db = DBConn::getInstance();
    std::string now = getNowTime();
    try
    {
        db.begin();
        std::stringstream orderSs;
        orderSs << "INSERT INTO orders(order_sn,user_id,merchant_id,rider_id,"
                << "total_amount,delivery_fee,pay_amount,order_status,"
                << "receiver_name,receiver_phone,receiver_addr,"
                << "create_time,update_time) "
                << "VALUES('" << order.order_no << "',"
                << order.user_id << ","
                << order.merchant_id << ","
                << (order.rider_id > 0 ? std::to_string(order.rider_id) : "NULL") << ","
                << order.total_price << ","
                << order.delivery_fee << ","
                << order.pay_price << ","
                << static_cast<int>(order.order_status) << ","
                << "'" << order.receiver_name << "',"
                << "'" << order.receiver_phone << "',"
                << "'" << order.receiver_addr << "',"
                << "'" << now << "','" << now << "')";
        executeUpdate(orderSs.str());


        for (const auto &item : itemList)
        {
            std::stringstream itemSs;
            itemSs << "INSERT INTO order_item(order_sn,dish_id,dish_name_snapshot,price_snapshot,quantity,item_total,create_time) "
                   << "VALUES('" << order.order_no << "',"
                   << item.dish_id << ",'" << item.dish_name << "',"
                   << item.dish_price << ","
                   << item.num << ","
                   << item.dish_price * item.num << ",'" << now << "')";
            executeUpdate(itemSs.str());

            std::string stockSql = "UPDATE dish SET stock=stock-" + std::to_string(item.num)
                                    + ",sold_count=sold_count+" + std::to_string(item.num)
                                    + ",update_time='" + now + "' WHERE id=" + std::to_string(item.dish_id);
            executeUpdate(stockSql);
        }
        db.commit();
        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "\n=====SQL异常信息=====\n" << e.what() << std::endl;
        db.rollback();
        return false;
    }
    catch (std::exception &e)
    {
        std::cerr << "\n=====异常信息=====\n" << e.what() << std::endl;
        db.rollback();
        return false;
    }
    catch (...)
    {
        std::cerr << "\n=====未知异常=====" << std::endl;
        db.rollback();
        return false;
    }
}

std::optional<Order> OrderService::getOrderByNo(const std::string &orderNo)
{
    std::string sql = "SELECT id,order_sn,user_id,merchant_id,rider_id,"
                      "total_amount,pay_amount,delivery_fee,order_status,"
                      "pay_time,finish_time,create_time,update_time,"
                      "receiver_name,receiver_phone,receiver_addr "
                      "FROM orders WHERE order_sn='" + orderNo + "' LIMIT 1";
    auto res = query(sql);
    if (res.empty())
        return std::nullopt;

    Order o;
    auto &row = res[0];
    o.id = strToLL(row[0]);
    o.order_no = row[1];
    o.user_id = strToLL(row[2]);
    o.merchant_id = strToLL(row[3]);
    o.rider_id = strToLL(row[4]);
    o.total_price = safeStod(row[5]);
    o.pay_price = safeStod(row[6]);
    o.delivery_fee = safeStod(row[7]);
    o.order_status = static_cast<OrderStatus>(safeStoi(row[8]));
    o.pay_time = row[9];
    o.finish_time = row[10];
    o.create_time = row[11];
    o.update_time = row[12];
    o.receiver_name = row[13];
    o.receiver_phone = row[14];
    o.receiver_addr = row[15];
    return o;
}

std::optional<Order> OrderService::getOrderById(uint64_t orderId)
{
    std::string sql = "SELECT id,order_sn,user_id,merchant_id,rider_id,"
                      "total_amount,pay_amount,delivery_fee,order_status,"
                      "pay_time,finish_time,create_time,update_time,"
                      "receiver_name,receiver_phone,receiver_addr "
                      "FROM orders WHERE id=" + std::to_string(orderId) + " LIMIT 1";
    auto res = query(sql);
    if (res.empty())
        return std::nullopt;

    Order o;
    auto &row = res[0];
    o.id = strToLL(row[0]);
    o.order_no = row[1];
    o.user_id = strToLL(row[2]);
    o.merchant_id = strToLL(row[3]);
    o.rider_id = strToLL(row[4]);
    o.total_price = safeStod(row[5]);
    o.pay_price = safeStod(row[6]);
    o.delivery_fee = safeStod(row[7]);
    o.order_status = static_cast<OrderStatus>(safeStoi(row[8]));
    o.pay_time = row[9];
    o.finish_time = row[10];
    o.create_time = row[11];
    o.update_time = row[12];
    o.receiver_name = row[13];
    o.receiver_phone = row[14];
    o.receiver_addr = row[15];
    return o;
}

std::vector<Order> OrderService::listOrderByUser(uint64_t userId, int limit, int offset)
{
    std::vector<Order> list;
    std::string sql = "SELECT id,order_sn,user_id,merchant_id,rider_id,"
                      "total_amount,pay_amount,delivery_fee,order_status,"
                      "pay_time,finish_time,create_time,update_time,"
                      "receiver_name,receiver_phone,receiver_addr "
                      "FROM orders WHERE user_id=" + std::to_string(userId)
                      + " ORDER BY create_time DESC LIMIT " + std::to_string(limit) + " OFFSET " + std::to_string(offset);
    auto res = query(sql);
    for (auto &row : res)
    {
        Order o;
        o.id = strToLL(row[0]);
        o.order_no = row[1];
        o.user_id = strToLL(row[2]);
        o.merchant_id = strToLL(row[3]);
        o.rider_id = strToLL(row[4]);
        o.total_price = safeStod(row[5]);
        o.pay_price = safeStod(row[6]);
        o.delivery_fee = safeStod(row[7]);
        o.order_status = static_cast<OrderStatus>(safeStoi(row[8]));
        o.pay_time = row[9];
        o.finish_time = row[10];
        o.create_time = row[11];
        o.update_time = row[12];
        o.receiver_name = row[13];
        o.receiver_phone = row[14];
        o.receiver_addr = row[15];
        list.push_back(o);
    }
    return list;
}

std::vector<Order> OrderService::listOrderByMerchant(uint64_t merchantId)
{
    std::vector<Order> list;
    std::string sql = "SELECT id,order_sn,user_id,merchant_id,rider_id,"
                      "total_amount,pay_amount,delivery_fee,order_status,"
                      "pay_time,finish_time,create_time,update_time,"
                      "receiver_name,receiver_phone,receiver_addr "
                      "FROM orders WHERE merchant_id=" + std::to_string(merchantId) + " ORDER BY create_time DESC";
    auto res = query(sql);
    for (auto &row : res)
    {
        Order o;
        o.id = strToLL(row[0]);
        o.order_no = row[1];
        o.user_id = strToLL(row[2]);
        o.merchant_id = strToLL(row[3]);
        o.rider_id = strToLL(row[4]);
        o.total_price = safeStod(row[5]);
        o.pay_price = safeStod(row[6]);
        o.delivery_fee = safeStod(row[7]);
        o.order_status = static_cast<OrderStatus>(safeStoi(row[8]));
        o.pay_time = row[9];
        o.finish_time = row[10];
        o.create_time = row[11];
        o.update_time = row[12];
        o.receiver_name = row[13];
        o.receiver_phone = row[14];
        o.receiver_addr = row[15];
        list.push_back(o);
    }
    return list;
}

std::vector<Order> OrderService::listAllOrders(int limit, int offset)
{
    std::vector<Order> list;
    std::string sql = "SELECT id,order_sn,user_id,merchant_id,rider_id,"
                      "total_amount,pay_amount,delivery_fee,order_status,"
                      "pay_time,finish_time,create_time,update_time,"
                      "receiver_name,receiver_phone,receiver_addr "
                      "FROM orders ORDER BY create_time DESC LIMIT " + std::to_string(limit) + " OFFSET " + std::to_string(offset);
    auto res = query(sql);
    for (auto &row : res)
    {
        Order o;
        o.id = strToLL(row[0]);
        o.order_no = row[1];
        o.user_id = strToLL(row[2]);
        o.merchant_id = strToLL(row[3]);
        o.rider_id = strToLL(row[4]);
        o.total_price = safeStod(row[5]);
        o.pay_price = safeStod(row[6]);
        o.delivery_fee = safeStod(row[7]);
        o.order_status = static_cast<OrderStatus>(safeStoi(row[8]));
        o.pay_time = row[9];
        o.finish_time = row[10];
        o.create_time = row[11];
        o.update_time = row[12];
        o.receiver_name = row[13];
        o.receiver_phone = row[14];
        o.receiver_addr = row[15];
        list.push_back(o);
    }
    return list;
}

int OrderService::updateOrderStatus(const std::string &orderNo, OrderStatus status)
{
    std::string now = getNowTime();
    std::string sql = "UPDATE orders SET order_status=" + std::to_string(static_cast<int>(status))
                      + ",update_time='" + now + "' WHERE order_sn='" + orderNo + "'";
    return executeUpdate(sql);
}

std::vector<OrderItem> OrderService::listOrderItemByNo(const std::string &orderNo)
{
    std::vector<OrderItem> itemList;
    std::string sql = "SELECT oi.id,oi.dish_id,oi.dish_name_snapshot,oi.price_snapshot,oi.quantity,oi.create_time "
                      "FROM order_item oi WHERE oi.order_sn='" + orderNo + "'";
    auto res = query(sql);
    for (auto &row : res)
    {
        OrderItem item;
        item.id = strToLL(row[0]);
        item.dish_id = strToLL(row[1]);
        item.dish_name = row[2];
        item.dish_price = safeStod(row[3]);
        item.num = safeStoi(row[4]);
        item.create_time = row[5];
        itemList.push_back(item);
    }
    return itemList;
}

bool OrderService::payOrder(const std::string &orderNo, PayType payType)
{
    auto orderOpt = getOrderByNo(orderNo);
    if (!orderOpt.has_value())
        return false;

    auto &db = DBConn::getInstance();
    std::string now = getNowTime();
    try
    {
        db.begin();
        std::string sql = "UPDATE orders SET order_status=" + std::to_string(static_cast<int>(OrderStatus::PAID))
                          + ",pay_status=1"
                          + ",pay_time='" + now + "',update_time='" + now
                          + "' WHERE order_sn='" + orderNo + "'";
        auto conn = db.getConn();
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        int ret = stmt->executeUpdate(sql);
        db.commit();
        return ret > 0;
    }
    catch (...)
    {
        db.rollback();
        return false;
    }
}

int OrderService::cancelUnpaidOrder(const std::string &orderNo)
{
    auto &db = DBConn::getInstance();
    std::string now = getNowTime();
    try
    {
        db.begin();
        std::string sql = "UPDATE orders SET order_status=" + std::to_string(static_cast<int>(OrderStatus::CANCELLED))
                          + ",update_time='" + now + "' WHERE order_sn='" + orderNo + "' AND order_status IN (0,1,2)";
        int ret = executeUpdate(sql);
        if (ret > 0) {
            auto items = listOrderItemByNo(orderNo);
            for (const auto &item : items) {
                std::string sql2 = "UPDATE dish SET stock=stock+" + std::to_string(item.num)
                                  + ",sold_count=sold_count-" + std::to_string(item.num)
                                  + ",update_time='" + now + "' WHERE id=" + std::to_string(item.dish_id);
                executeUpdate(sql2);
            }
        }
        db.commit();
        return ret;
    }
    catch (...)
    {
        db.rollback();
        return 0;
    }
}

int OrderService::getTodayOrderCount(uint64_t merchantId) {
    std::string sql = "SELECT COUNT(*) FROM orders WHERE merchant_id=" + std::to_string(merchantId) 
                      + " AND DATE(create_time)=CURDATE()";
    auto res = query(sql);
    if (res.empty()) return 0;
    return safeStoi(res[0][0]);
}

double OrderService::getTodayRevenue(uint64_t merchantId) {
    std::string sql = "SELECT COALESCE(SUM(pay_amount), 0) FROM orders WHERE merchant_id=" + std::to_string(merchantId) 
                      + " AND DATE(create_time)=CURDATE() AND order_status>=2 AND order_status NOT IN (8, 9, 10, 11)";
    auto res = query(sql);
    if (res.empty()) return 0.0;
    return safeStod(res[0][0]);
}

int OrderService::getPendingOrderCount(uint64_t merchantId) {
    std::string sql = "SELECT COUNT(*) FROM orders WHERE merchant_id=" + std::to_string(merchantId) 
                      + " AND order_status IN (2, 3)";
    auto res = query(sql);
    if (res.empty()) return 0;
    return safeStoi(res[0][0]);
}