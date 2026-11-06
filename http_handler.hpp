#pragma once
#include "httplib.h"
#include "service/user_service.hpp"
#include "service/dish_service.hpp"
#include "service/order_service.hpp"
#include "service/merchant_service.hpp"
#include "json_util.hpp"

class HttpHandler {
public:
    HttpHandler();
    void registerRoutes(httplib::Server& server);

private:
    UserService userService;
    DishService dishService;
    OrderService orderService;
    MerchantService merchantService;

    void handleLogin(const httplib::Request& req, httplib::Response& res);
    void handleRegister(const httplib::Request& req, httplib::Response& res);
    void handleGetUser(const httplib::Request& req, httplib::Response& res);
    void handleUpdateUser(const httplib::Request& req, httplib::Response& res);
    void handleListUsers(const httplib::Request& req, httplib::Response& res);

    void handleGetMerchants(const httplib::Request& req, httplib::Response& res);
    void handleGetMerchant(const httplib::Request& req, httplib::Response& res);
    void handleUpdateMerchantStatus(const httplib::Request& req, httplib::Response& res);
    void handleUpdateMerchant(const httplib::Request& req, httplib::Response& res);

    void handleGetCategories(const httplib::Request& req, httplib::Response& res);

    void handleGetDishes(const httplib::Request& req, httplib::Response& res);
    void handleGetDish(const httplib::Request& req, httplib::Response& res);
    void handleAddDish(const httplib::Request& req, httplib::Response& res);
    void handleUpdateDish(const httplib::Request& req, httplib::Response& res);
    void handleDeleteDish(const httplib::Request& req, httplib::Response& res);

    void handleCreateOrder(const httplib::Request& req, httplib::Response& res);
    void handleGetOrder(const httplib::Request& req, httplib::Response& res);
    void handleListOrders(const httplib::Request& req, httplib::Response& res);
    void handlePayOrder(const httplib::Request& req, httplib::Response& res);
    void handleCancelOrder(const httplib::Request& req, httplib::Response& res);
    void handleUpdateOrderStatus(const httplib::Request& req, httplib::Response& res);
    void handleOrderItems(const httplib::Request& req, httplib::Response& res);

    void handleHealth(const httplib::Request& req, httplib::Response& res);
    void handleNotFound(const httplib::Request& req, httplib::Response& res);
    void handleMerchantStats(const httplib::Request& req, httplib::Response& res);
    void handlePlatformStats(const httplib::Request& req, httplib::Response& res);

    void setCorsHeaders(httplib::Response& res);

    std::string userToJson(const User& user);
    std::string dishToJson(const Dish& dish);
    std::string orderToJson(const Order& order);
    std::string orderItemToJson(const OrderItem& item);
    std::string merchantToJson(const Merchant& merchant);
};
