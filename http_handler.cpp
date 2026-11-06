#include "http_handler.hpp"
#include "util.hpp"
#include <sstream>

HttpHandler::HttpHandler() {}

void HttpHandler::registerRoutes(httplib::Server& server) {
    server.set_pre_routing_handler([this](const auto& req, auto& res) {
        setCorsHeaders(res);
        if (req.method == "OPTIONS") {
            res.status = 200;
            return httplib::Server::HandlerResponse::Handled;
        }
        return httplib::Server::HandlerResponse::Unhandled;
    });

    server.Get("/api/health", [this](const auto& req, auto& res) { handleHealth(req, res); });

    server.Post("/api/user/login", [this](const auto& req, auto& res) { handleLogin(req, res); });
    server.Post("/api/user/register", [this](const auto& req, auto& res) { handleRegister(req, res); });
    server.Get("/api/user/list", [this](const auto& req, auto& res) { handleListUsers(req, res); });
    server.Get("/api/user", [this](const auto& req, auto& res) { handleGetUser(req, res); });
    server.Post("/api/user/update", [this](const auto& req, auto& res) { handleUpdateUser(req, res); });

    server.Get("/api/merchant/list", [this](const auto& req, auto& res) { handleGetMerchants(req, res); });
    server.Get("/api/merchant", [this](const auto& req, auto& res) { handleGetMerchant(req, res); });
    server.Get("/api/merchant/stats", [this](const auto& req, auto& res) { handleMerchantStats(req, res); });
    server.Post("/api/merchant/update_status", [this](const auto& req, auto& res) { handleUpdateMerchantStatus(req, res); });
    server.Post("/api/merchant/update", [this](const auto& req, auto& res) { handleUpdateMerchant(req, res); });
    server.Get("/api/admin/stats", [this](const auto& req, auto& res) { handlePlatformStats(req, res); });

    server.Get("/api/category/list", [this](const auto& req, auto& res) { handleGetCategories(req, res); });

    server.Get("/api/dish/list", [this](const auto& req, auto& res) { handleGetDishes(req, res); });
    server.Get("/api/dish", [this](const auto& req, auto& res) { handleGetDish(req, res); });
    server.Post("/api/dish/add", [this](const auto& req, auto& res) { handleAddDish(req, res); });
    server.Post("/api/dish/update", [this](const auto& req, auto& res) { handleUpdateDish(req, res); });
    server.Post("/api/dish/delete", [this](const auto& req, auto& res) { handleDeleteDish(req, res); });

    server.Post("/api/order/create", [this](const auto& req, auto& res) { handleCreateOrder(req, res); });
    server.Get("/api/order", [this](const auto& req, auto& res) { handleGetOrder(req, res); });
    server.Get("/api/order/list", [this](const auto& req, auto& res) { handleListOrders(req, res); });
    server.Get("/api/order/items", [this](const auto& req, auto& res) { handleOrderItems(req, res); });
    server.Post("/api/order/pay", [this](const auto& req, auto& res) { handlePayOrder(req, res); });
    server.Post("/api/order/cancel", [this](const auto& req, auto& res) { handleCancelOrder(req, res); });
    server.Post("/api/order/update_status", [this](const auto& req, auto& res) { handleUpdateOrderStatus(req, res); });

    server.set_error_handler([this](const auto& req, auto& res) { handleNotFound(req, res); });
}

void HttpHandler::setCorsHeaders(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    res.set_header("Access-Control-Max-Age", "86400");
}

void HttpHandler::handleHealth(const httplib::Request& req, httplib::Response& res) {
    res.set_content(successJson("status", "ok"), "application/json");
}

void HttpHandler::handleNotFound(const httplib::Request& req, httplib::Response& res) {
    res.status = 404;
    res.set_content(errorJson(404, "Not Found"), "application/json");
}

std::string HttpHandler::userToJson(const User& user) {
    JsonBuilder b;
    b.startObject()
        .key("id").value(user.id)
        .key("account").value(user.account)
        .key("user_name").value(user.user_name)
        .key("phone").value(user.phone)
        .key("sex").value(user.sex)
        .key("avatar").value(user.avatar)
        .key("description").value(user.description)
        .key("user_type").value(static_cast<int>(user.user_type))
        .key("status").value(static_cast<int>(user.status))
        .key("create_time").value(user.create_time)
        .key("update_time").value(user.update_time)
        .endObject();
    return b.build();
}

std::string HttpHandler::dishToJson(const Dish& dish) {
    JsonBuilder b;
    b.startObject()
        .key("id").value(dish.id)
        .key("merchant_id").value(dish.merchant_id)
        .key("category_id").value(dish.category_id)
        .key("dish_name").value(dish.dish_name)
        .key("image_url").value(dish.image_url)
        .key("price").value(dish.price)
        .key("original_price").value(dish.original_price)
        .key("stock").value(dish.stock)
        .key("sold_count").value(dish.sold_count)
        .key("description").value(dish.description)
        .key("status").value(static_cast<int>(dish.status))
        .key("create_time").value(dish.create_time)
        .key("update_time").value(dish.update_time)
        .endObject();
    return b.build();
}

std::string HttpHandler::orderToJson(const Order& order) {
    JsonBuilder b;
    b.startObject()
        .key("id").value(order.id)
        .key("order_no").value(order.order_no)
        .key("user_id").value(order.user_id)
        .key("merchant_id").value(order.merchant_id)
        .key("rider_id").value(order.rider_id)
        .key("receiver_name").value(order.receiver_name)
        .key("receiver_phone").value(order.receiver_phone)
        .key("receiver_addr").value(order.receiver_addr)
        .key("total_price").value(order.total_price)
        .key("pay_price").value(order.pay_price)
        .key("delivery_fee").value(order.delivery_fee)
        .key("pay_type").value(static_cast<int>(order.pay_type))
        .key("order_status").value(static_cast<int>(order.order_status))
        .key("pay_time").value(order.pay_time)
        .key("finish_time").value(order.finish_time)
        .key("create_time").value(order.create_time)
        .key("update_time").value(order.update_time)
        .endObject();
    return b.build();
}

std::string HttpHandler::orderItemToJson(const OrderItem& item) {
    JsonBuilder b;
    b.startObject()
        .key("id").value(item.id)
        .key("order_id").value(item.order_id)
        .key("dish_id").value(item.dish_id)
        .key("dish_name").value(item.dish_name)
        .key("dish_price").value(item.dish_price)
        .key("num").value(item.num)
        .key("create_time").value(item.create_time)
        .endObject();
    return b.build();
}

std::string HttpHandler::merchantToJson(const Merchant& merchant) {
    JsonBuilder b;
    b.startObject()
        .key("id").value(merchant.id)
        .key("user_id").value(merchant.user_id)
        .key("name").value(merchant.merchant_name)
        .key("merchant_name").value(merchant.merchant_name)
        .key("image").value(merchant.logo_url)
        .key("logo_url").value(merchant.logo_url)
        .key("shop_name").value(merchant.merchant_name)
        .key("shop_img").value(merchant.logo_url)
        .key("address").value(merchant.address)
        .key("contact_phone").value(merchant.contact_phone)
        .key("delivery_mode").value(static_cast<int>(merchant.delivery_mode))
        .key("min_order_price").value(merchant.start_price)
        .key("start_price").value(merchant.start_price)
        .key("delivery_fee").value(merchant.delivery_fee)
        .key("status").value(merchant.status)
        .key("rating").value(4.8)
        .key("month_sales").value(2000)
        .key("delivery_time").value(25)
        .key("description").value(merchant.description)
        .key("create_time").value(merchant.create_time)
        .key("update_time").value(merchant.update_time)
        .endObject();
    return b.build();
}

void HttpHandler::handleLogin(const httplib::Request& req, httplib::Response& res) {
    auto params = parseQueryParams(req.body);
    std::string account = getParam(params, "account");
    std::string password = getParam(params, "password");

    if (account.empty() || password.empty()) {
        res.set_content(errorJson(1, "账号或密码不能为空"), "application/json");
        return;
    }

    auto user = userService.login(account, password);
    if (user.has_value()) {
        res.set_content(successJson(userToJson(user.value())), "application/json");
    } else {
        res.set_content(errorJson(2, "账号或密码错误"), "application/json");
    }
}

void HttpHandler::handleRegister(const httplib::Request& req, httplib::Response& res) {
    auto params = parseQueryParams(req.body);
    std::string account = getParam(params, "account");
    std::string password = getParam(params, "password");
    std::string user_name = getParam(params, "user_name");
    std::string phone = getParam(params, "phone");
    int user_type = getParamInt(params, "user_type", 0);

    if (account.empty() || password.empty()) {
        res.set_content(errorJson(1, "账号和密码不能为空"), "application/json");
        return;
    }

    User user;
    user.account = account;
    user.password = password;
    user.user_name = user_name.empty() ? account : user_name;
    user.phone = phone;
    user.sex = 0;
    user.avatar = "";
    user.description = "";
    user.user_type = static_cast<UserType>(user_type);
    user.status = UserStatus::NORMAL;

    try {
        int ret = userService.addUser(user);
        if (ret > 0) {
            res.set_content(successJson("注册成功"), "application/json");
        } else {
            res.set_content(errorJson(3, "注册失败"), "application/json");
        }
    } catch (...) {
        res.set_content(errorJson(4, "账号已存在"), "application/json");
    }
}

void HttpHandler::handleGetUser(const httplib::Request& req, httplib::Response& res) {
    std::string id_str = req.get_param_value("id");
    long long uid = safeStoll(id_str);

    if (uid <= 0) {
        res.set_content(errorJson(1, "用户ID不能为空"), "application/json");
        return;
    }

    auto user = userService.getUserById(uid);
    if (user.has_value()) {
        res.set_content(successJson(userToJson(user.value())), "application/json");
    } else {
        res.set_content(errorJson(2, "用户不存在"), "application/json");
    }
}

void HttpHandler::handleUpdateUser(const httplib::Request& req, httplib::Response& res) {
    auto params = parseQueryParams(req.body);
    long long uid = getParamLong(params, "id");
    std::string user_name = urlDecode(getParam(params, "user_name"));
    std::string phone = getParam(params, "phone");
    int status = getParamInt(params, "status", -1);

    if (uid <= 0) {
        res.set_content(errorJson(1, "用户ID不能为空"), "application/json");
        return;
    }

    auto user = userService.getUserById(uid);
    if (!user.has_value()) {
        res.set_content(errorJson(2, "用户不存在"), "application/json");
        return;
    }

    if (!user_name.empty()) user.value().user_name = user_name;
    if (!phone.empty()) user.value().phone = phone;
    if (status >= 0) user.value().status = static_cast<UserStatus>(status);

    int ret = userService.updateUser(user.value());
    if (ret > 0) {
        res.set_content(successJson(), "application/json");
    } else {
        res.set_content(errorJson(3, "更新失败"), "application/json");
    }
}

void HttpHandler::handleListUsers(const httplib::Request& req, httplib::Response& res) {
    std::string limit_str = req.get_param_value("limit");
    std::string offset_str = req.get_param_value("offset");
    int limit = safeStoi(limit_str);
    int offset = safeStoi(offset_str);
    if (limit <= 0) limit = 20;

    auto users = userService.listAllUser(limit, offset);

    JsonBuilder b;
    b.startObject().key("data").startArray();
    for (const auto& user : users) {
        b.startObject()
            .key("id").value(user.id)
            .key("account").value(user.account)
            .key("user_name").value(user.user_name)
            .key("phone").value(user.phone)
            .key("user_type").value(static_cast<int>(user.user_type))
            .key("status").value(static_cast<int>(user.status))
            .endObject();
    }
    b.endArray().key("total").value((int)users.size()).endObject();

    res.set_content(successJson(b.build()), "application/json");
}

void HttpHandler::handleGetMerchants(const httplib::Request& req, httplib::Response& res) {
    std::string status_str = req.get_param_value("status");
    int status = -1;
    if (!status_str.empty()) {
        status = safeStoi(status_str);
    }

    std::string sql = "SELECT id,user_id,merchant_name,logo_url,address,business_hours,contact_phone,delivery_mode,start_price,delivery_fee,status,create_time,update_time FROM merchant";
    if (status >= 0) {
        sql += " WHERE status=" + std::to_string(status);
    }
    
    auto results = query(sql);
    std::vector<Merchant> merchants;
    for (auto& row : results) {
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
        merchants.push_back(m);
    }

    JsonBuilder b;
    b.startObject().key("data").startArray();
    for (const auto& m : merchants) {
        std::string img_url = m.logo_url.empty() 
            ? "/images/default_shop.jpg" 
            : m.logo_url;
        b.startObject()
            .key("id").value(m.id)
            .key("name").value(m.merchant_name)
            .key("image").value(img_url)
            .key("shop_name").value(m.merchant_name)
            .key("shop_img").value(img_url)
            .key("address").value(m.address)
            .key("delivery_fee").value(m.delivery_fee)
            .key("min_order_price").value(m.start_price)
            .key("start_price").value(m.start_price)
            .key("status").value(m.status)
            .key("contact_phone").value(m.contact_phone)
            .key("rating").value(4.8)
            .key("month_sales").value(2000)
            .key("delivery_time").value(25)
            .endObject();
    }
    b.endArray().endObject();

    res.set_content(successJson(b.build()), "application/json");
}

void HttpHandler::handleGetMerchant(const httplib::Request& req, httplib::Response& res) {
    std::string id_str = req.get_param_value("id");
    std::string userId_str = req.get_param_value("user_id");
    long long id = safeStoll(id_str);
    long long userId = safeStoll(userId_str);

    if (id <= 0 && userId <= 0) {
        res.set_content(errorJson(1, "商家ID或用户ID不能为空"), "application/json");
        return;
    }

    std::optional<Merchant> merchant;
    if (id > 0) {
        merchant = merchantService.getMerchantById(id);
    } else {
        merchant = merchantService.getMerchantByUserId(userId);
    }

    if (merchant.has_value()) {
        res.set_content(successJson(merchantToJson(merchant.value())), "application/json");
    } else {
        res.set_content(errorJson(2, "商家不存在"), "application/json");
    }
}

void HttpHandler::handleMerchantStats(const httplib::Request& req, httplib::Response& res) {
    std::string merchantId_str = req.get_param_value("merchant_id");
    long long merchantId = safeStoll(merchantId_str);

    if (merchantId <= 0) {
        res.set_content(errorJson(1, "商家ID不能为空"), "application/json");
        return;
    }

    int todayOrders = orderService.getTodayOrderCount(merchantId);
    double todayRevenue = orderService.getTodayRevenue(merchantId);
    int pendingOrders = orderService.getPendingOrderCount(merchantId);

    JsonBuilder b;
    b.startObject()
        .key("today_orders").value(todayOrders)
        .key("today_revenue").value(todayRevenue)
        .key("pending_orders").value(pendingOrders)
        .key("month_orders").value(0)
        .key("month_revenue").value(0.0)
        .key("total_customers").value(0)
        .key("rating").value(4.8)
        .endObject();

    res.set_content(successJson(b.build()), "application/json");
}

void HttpHandler::handlePlatformStats(const httplib::Request& req, httplib::Response& res) {
    std::string sql;
    auto queryResult = [&](const std::string& s) {
        auto res = query(s);
        return res.empty() ? 0 : safeStoi(res[0][0]);
    };

    auto queryDouble = [&](const std::string& s) {
        auto res = query(s);
        return res.empty() ? 0.0 : safeStod(res[0][0]);
    };

    int userCount = queryResult("SELECT COUNT(*) FROM user");
    int merchantCount = queryResult("SELECT COUNT(*) FROM merchant");
    int activeMerchantCount = queryResult("SELECT COUNT(*) FROM merchant WHERE status=1");
    int todayOrderCount = queryResult("SELECT COUNT(*) FROM orders WHERE DATE(create_time)=CURDATE()");
    double todayRevenue = queryDouble("SELECT COALESCE(SUM(pay_amount), 0) FROM orders WHERE DATE(create_time)=CURDATE() AND order_status>=2");
    int pendingMerchantCount = queryResult("SELECT COUNT(*) FROM merchant WHERE status=0");
    int totalOrderCount = queryResult("SELECT COUNT(*) FROM orders");
    double totalRevenue = queryDouble("SELECT COALESCE(SUM(pay_amount), 0) FROM orders WHERE order_status>=2");
    double avgRating = queryDouble("SELECT COALESCE(AVG(rating), 0) FROM merchant");

    JsonBuilder b;
    b.startObject()
        .key("user_count").value(userCount)
        .key("merchant_count").value(merchantCount)
        .key("active_merchant_count").value(activeMerchantCount)
        .key("today_order_count").value(todayOrderCount)
        .key("today_revenue").value(todayRevenue)
        .key("pending_merchant_count").value(pendingMerchantCount)
        .key("total_order_count").value(totalOrderCount)
        .key("total_revenue").value(totalRevenue)
        .key("avg_rating").value(avgRating)
        .endObject();

    res.set_content(successJson(b.build()), "application/json");
}

void HttpHandler::handleGetCategories(const httplib::Request& req, httplib::Response& res) {
    std::string merchantId_str = req.get_param_value("merchant_id");
    long long merchantId = safeStoll(merchantId_str);

    std::vector<std::map<std::string, std::string>> categories;
    
    if (merchantId > 0) {
        categories = {
            {{"id", "1"}, {"name", "热销推荐"}},
            {{"id", "2"}, {"name", "招牌菜"}},
            {{"id", "3"}, {"name", "新品上市"}}
        };
    } else {
        categories = {
            {{"id", "1"}, {"name", "热销推荐"}},
            {{"id", "2"}, {"name", "家常菜"}},
            {{"id", "3"}, {"name", "麻辣烫"}},
            {{"id", "4"}, {"name", "甜品饮品"}},
            {{"id", "5"}, {"name", "面食"}},
            {{"id", "6"}, {"name", "烧烤"}},
            {{"id", "7"}, {"name", "快餐便当"}}
        };
    }

    JsonBuilder b;
    b.startObject().key("data").startArray();
    for (const auto& c : categories) {
        b.startObject()
            .key("id").value(c.at("id"))
            .key("name").value(c.at("name"))
            .endObject();
    }
    b.endArray().endObject();

    res.set_content(successJson(b.build()), "application/json");
}

void HttpHandler::handleGetDishes(const httplib::Request& req, httplib::Response& res) {
    std::string merchantId_str = req.get_param_value("merchant_id");
    std::string categoryId_str = req.get_param_value("category_id");
    std::string keyword_str = req.get_param_value("keyword");
    std::string limit_str = req.get_param_value("limit");
    std::string offset_str = req.get_param_value("offset");

    long long merchantId = safeStoll(merchantId_str);
    long long categoryId = safeStoll(categoryId_str);
    int limit = safeStoi(limit_str);
    int offset = safeStoi(offset_str);
    if (limit <= 0) limit = 20;

    std::vector<Dish> dishes;
    if (merchantId > 0) {
        dishes = dishService.listDishByMerchant(merchantId, limit, offset);
    } else {
        dishes = dishService.listAllDishes(limit, offset);
    }

    if (categoryId > 0) {
        std::vector<Dish> filtered;
        for (const auto& d : dishes) {
            if (d.category_id == categoryId) {
                filtered.push_back(d);
            }
        }
        dishes = filtered;
    }

    if (!keyword_str.empty()) {
        std::vector<Dish> filtered;
        for (const auto& d : dishes) {
            if (d.dish_name.find(keyword_str) != std::string::npos) {
                filtered.push_back(d);
            }
        }
        dishes = filtered;
    }

    JsonBuilder b;
    b.startObject().key("data").startArray();
    for (const auto& dish : dishes) {
        std::string dish_img = dish.image_url.empty() 
            ? "/images/default_dish.jpg" 
            : dish.image_url;
        b.startObject()
            .key("id").value(dish.id)
            .key("merchant_id").value(dish.merchant_id)
            .key("name").value(dish.dish_name)
            .key("image").value(dish_img)
            .key("dish_name").value(dish.dish_name)
            .key("image_url").value(dish_img)
            .key("category_id").value(dish.category_id)
            .key("price").value(dish.price)
            .key("original_price").value(dish.original_price)
            .key("stock").value(dish.stock)
            .key("sales").value(dish.sold_count)
            .key("sold_count").value(dish.sold_count)
            .key("description").value(dish.description)
            .key("status").value(static_cast<int>(dish.status))
            .endObject();
    }
    b.endArray().endObject();

    res.set_content(successJson(b.build()), "application/json");
}

void HttpHandler::handleGetDish(const httplib::Request& req, httplib::Response& res) {
    std::string dishId_str = req.get_param_value("id");
    long long dishId = safeStoll(dishId_str);

    if (dishId <= 0) {
        res.set_content(errorJson(1, "菜品ID不能为空"), "application/json");
        return;
    }

    auto dish = dishService.getDishById(dishId);
    if (dish.has_value()) {
        res.set_content(successJson(dishToJson(dish.value())), "application/json");
    } else {
        res.set_content(errorJson(2, "菜品不存在"), "application/json");
    }
}

void HttpHandler::handleAddDish(const httplib::Request& req, httplib::Response& res) {
    auto params = parseQueryParams(req.body);
    long long merchantId = getParamLong(params, "merchant_id");
    long long categoryId = getParamLong(params, "category_id");
    std::string dish_name = urlDecode(getParam(params, "dish_name"));
    std::string image_url = getParam(params, "image_url");
    double price = safeStod(getParam(params, "price", "0"));
    double original_price = safeStod(getParam(params, "original_price", "0"));
    int stock = getParamInt(params, "stock", 0);
    std::string description = urlDecode(getParam(params, "description"));

    if (merchantId <= 0 || dish_name.empty()) {
        res.set_content(errorJson(1, "商家ID和菜品名称不能为空"), "application/json");
        return;
    }

    if (categoryId <= 0) {
        std::string sql = "SELECT id FROM category WHERE merchant_id=" + std::to_string(merchantId) + " LIMIT 1";
        auto res_cat = query(sql);
        if (!res_cat.empty()) {
            categoryId = strToLL(res_cat[0][0]);
        } else {
            categoryId = 1;
        }
    }

    Dish dish;
    dish.merchant_id = merchantId;
    dish.category_id = categoryId;
    dish.dish_name = dish_name;
    dish.image_url = image_url;
    dish.price = price;
    dish.original_price = original_price;
    dish.stock = stock;
    dish.sold_count = 0;
    dish.description = description;
    dish.status = DishStatus::ON_SALE;

    int ret = dishService.addDish(dish);
    if (ret > 0) {
        res.set_content(successJson(), "application/json");
    } else {
        res.set_content(errorJson(2, "添加失败"), "application/json");
    }
}

void HttpHandler::handleCreateOrder(const httplib::Request& req, httplib::Response& res) {
    auto params = parseQueryParams(req.body);
    long long userId = getParamLong(params, "user_id");
    long long merchantId = getParamLong(params, "merchant_id");
    std::string receiver_name = getParam(params, "receiver_name");
    std::string receiver_phone = getParam(params, "receiver_phone");
    std::string receiver_addr = getParam(params, "receiver_addr");
    std::string total_str = getParam(params, "total_price", "0");
    std::string pay_str = getParam(params, "pay_price", "0");
    std::string fee_str = getParam(params, "delivery_fee", "0");
    double total_price = 0, pay_price = 0, delivery_fee = 0;
    total_price = safeStod(total_str);
    pay_price = safeStod(pay_str);
    delivery_fee = safeStod(fee_str);
    int pay_type = getParamInt(params, "pay_type", 1);

    if (userId <= 0 || merchantId <= 0 || receiver_name.empty() || receiver_phone.empty()) {
        res.set_content(errorJson(1, "缺少必要参数"), "application/json");
        return;
    }

    Order order;
    order.order_no = createOrderNo();
    order.user_id = userId;
    order.merchant_id = merchantId;
    order.rider_id = 0;
    order.receiver_name = receiver_name;
    order.receiver_phone = receiver_phone;
    order.receiver_addr = receiver_addr;
    order.total_price = total_price;
    order.pay_price = pay_price;
    order.delivery_fee = delivery_fee;
    order.pay_type = static_cast<PayType>(pay_type);
    order.order_status = OrderStatus::PENDING_PAY;

    std::vector<OrderItem> items;
    int itemCount = getParamInt(params, "item_count", 0);
    for (int i = 0; i < itemCount; i++) {
        OrderItem item;
        item.dish_id = getParamLong(params, "dish_id_" + std::to_string(i));
        item.dish_name = urlDecode(getParam(params, "dish_name_" + std::to_string(i)));
        std::string priceStr = getParam(params, "dish_price_" + std::to_string(i), "0");
        item.dish_price = safeStod(priceStr);
        item.num = getParamInt(params, "num_" + std::to_string(i), 1);
        if (item.dish_id > 0 && !item.dish_name.empty()) {
            items.push_back(item);
        }
    }

    if (items.empty()) {
        res.set_content(errorJson(2, "订单商品不能为空"), "application/json");
        return;
    }

    bool success = orderService.createOrder(order, items);
    if (success) {
        JsonBuilder b;
        b.startObject().key("order_no").value(order.order_no).endObject();
        res.set_content(successJson(b.build()), "application/json");
    } else {
        res.set_content(errorJson(3, "创建订单失败"), "application/json");
    }
}

void HttpHandler::handleGetOrder(const httplib::Request& req, httplib::Response& res) {
    std::string orderNo = req.get_param_value("order_no");

    if (orderNo.empty()) {
        res.set_content(errorJson(1, "订单号不能为空"), "application/json");
        return;
    }

    auto order = orderService.getOrderByNo(orderNo);
    if (order.has_value()) {
        res.set_content(successJson(orderToJson(order.value())), "application/json");
    } else {
        res.set_content(errorJson(2, "订单不存在"), "application/json");
    }
}

void HttpHandler::handleListOrders(const httplib::Request& req, httplib::Response& res) {
    std::string userId_str = req.get_param_value("user_id");
    std::string merchantId_str = req.get_param_value("merchant_id");
    std::string limit_str = req.get_param_value("limit");
    std::string offset_str = req.get_param_value("offset");
    std::string status_str = req.get_param_value("status");

    long long userId = safeStoll(userId_str);
    long long merchantId = safeStoll(merchantId_str);
    int limit = safeStoi(limit_str);
    int offset = safeStoi(offset_str);
    int status = -1;
    if (!status_str.empty() && status_str != "all") {
        status = safeStoi(status_str);
    }
    if (limit <= 0) limit = 20;

    std::vector<Order> orders;
    if (userId > 0) {
        orders = orderService.listOrderByUser(userId, limit, offset);
    } else if (merchantId > 0) {
        orders = orderService.listOrderByMerchant(merchantId);
    } else {
        orders = orderService.listAllOrders(limit, offset);
    }

    if (status >= 0) {
        orders.erase(std::remove_if(orders.begin(), orders.end(), 
            [status](const Order& o) { return static_cast<int>(o.order_status) != status; }), 
            orders.end());
    }

    JsonBuilder b;
    b.startObject().key("data").startArray();
    for (const auto& order : orders) {
        b.startObject()
            .key("id").value(order.id)
            .key("order_no").value(order.order_no)
            .key("user_id").value(order.user_id)
            .key("merchant_id").value(order.merchant_id)
            .key("total_price").value(order.total_price)
            .key("amount").value(order.total_price)
            .key("pay_price").value(order.pay_price)
            .key("status").value(static_cast<int>(order.order_status))
            .key("order_status").value(static_cast<int>(order.order_status))
            .key("create_time").value(order.create_time)
            .key("receiver_name").value(order.receiver_name)
            .key("receiver_phone").value(order.receiver_phone)
            .key("receiver_addr").value(order.receiver_addr)
            .endObject();
    }
    b.endArray().endObject();

    res.set_content(successJson(b.build()), "application/json");
}

void HttpHandler::handleOrderItems(const httplib::Request& req, httplib::Response& res) {
    std::string orderNo = req.get_param_value("order_no");

    if (orderNo.empty()) {
        res.set_content(errorJson(1, "订单号不能为空"), "application/json");
        return;
    }

    auto items = orderService.listOrderItemByNo(orderNo);

    JsonBuilder b;
    b.startObject().key("data").startArray();
    for (const auto& item : items) {
        b.startObject()
            .key("id").value(item.id)
            .key("dish_id").value(item.dish_id)
            .key("dish_name").value(item.dish_name)
            .key("dish_price").value(item.dish_price)
            .key("num").value(item.num)
            .endObject();
    }
    b.endArray().key("total").value((int)items.size()).endObject();

    res.set_content(successJson(b.build()), "application/json");
}

void HttpHandler::handlePayOrder(const httplib::Request& req, httplib::Response& res) {
    auto params = parseQueryParams(req.body);
    std::string orderNo = getParam(params, "order_no");
    int pay_type = getParamInt(params, "pay_type", 1);

    if (orderNo.empty()) {
        res.set_content(errorJson(1, "订单号不能为空"), "application/json");
        return;
    }

    orderService.payOrder(orderNo, static_cast<PayType>(pay_type));
    res.set_content(successJson(), "application/json");
}

void HttpHandler::handleCancelOrder(const httplib::Request& req, httplib::Response& res) {
    std::string orderNo;
    
    if (req.has_param("order_no")) {
        orderNo = req.get_param_value("order_no");
    } else {
        auto params = parseQueryParams(req.body);
        orderNo = getParam(params, "order_no");
    }

    if (orderNo.empty()) {
        res.set_content(errorJson(1, "订单号不能为空"), "application/json");
        return;
    }

    orderService.cancelUnpaidOrder(orderNo);
    res.set_content(successJson(), "application/json");
}

void HttpHandler::handleUpdateOrderStatus(const httplib::Request& req, httplib::Response& res) {
    auto params = parseQueryParams(req.body);
    long long orderId = getParamLong(params, "order_id");
    int status = getParamInt(params, "status");

    if (orderId <= 0) {
        res.set_content(errorJson(1, "订单ID不能为空"), "application/json");
        return;
    }

    auto order = orderService.getOrderById(orderId);
    if (!order.has_value()) {
        res.set_content(errorJson(2, "订单不存在"), "application/json");
        return;
    }

    int ret = orderService.updateOrderStatus(order.value().order_no, static_cast<OrderStatus>(status));
    if (ret > 0) {
        res.set_content(successJson(), "application/json");
    } else {
        res.set_content(errorJson(3, "更新失败"), "application/json");
    }
}

void HttpHandler::handleUpdateDish(const httplib::Request& req, httplib::Response& res) {
    auto params = parseQueryParams(req.body);
    long long dishId = getParamLong(params, "id");
    std::string dish_name = urlDecode(getParam(params, "dish_name"));
    double price = safeStod(getParam(params, "price", "0"));
    int stock = getParamInt(params, "stock", 0);
    std::string description = urlDecode(getParam(params, "description"));
    int status = getParamInt(params, "status", 1);

    if (dishId <= 0) {
        res.set_content(errorJson(1, "菜品ID不能为空"), "application/json");
        return;
    }

    auto dishOpt = dishService.getDishById(dishId);
    if (!dishOpt.has_value()) {
        res.set_content(errorJson(2, "菜品不存在"), "application/json");
        return;
    }

    Dish dish = dishOpt.value();
    if (!dish_name.empty()) dish.dish_name = dish_name;
    if (price > 0) dish.price = price;
    if (stock >= 0) dish.stock = stock;
    if (!description.empty()) dish.description = description;
    dish.status = static_cast<DishStatus>(status);

    int ret = dishService.updateDish(dish);
    if (ret > 0) {
        res.set_content(successJson(), "application/json");
    } else {
        res.set_content(errorJson(3, "更新失败"), "application/json");
    }
}

void HttpHandler::handleDeleteDish(const httplib::Request& req, httplib::Response& res) {
    auto params = parseQueryParams(req.body);
    long long dishId = getParamLong(params, "id");

    if (dishId <= 0) {
        res.set_content(errorJson(1, "菜品ID不能为空"), "application/json");
        return;
    }

    std::string sql = "DELETE FROM dish WHERE id=" + std::to_string(dishId);
    int ret = executeUpdate(sql);
    if (ret > 0) {
        res.set_content(successJson(), "application/json");
    } else {
        res.set_content(errorJson(2, "删除失败"), "application/json");
    }
}

void HttpHandler::handleUpdateMerchantStatus(const httplib::Request& req, httplib::Response& res) {
    auto params = parseQueryParams(req.body);
    long long merchantId = getParamLong(params, "id");
    int status = getParamInt(params, "status");

    if (merchantId <= 0) {
        res.set_content(errorJson(1, "商家ID不能为空"), "application/json");
        return;
    }

    std::string now = getNowTime();
    std::string sql = "UPDATE merchant SET status=" + std::to_string(status)
                      + ",update_time='" + now + "' WHERE id=" + std::to_string(merchantId);
    int ret = executeUpdate(sql);
    if (ret > 0) {
        res.set_content(successJson(), "application/json");
    } else {
        res.set_content(errorJson(2, "更新失败"), "application/json");
    }
}

void HttpHandler::handleUpdateMerchant(const httplib::Request& req, httplib::Response& res) {
    auto params = parseQueryParams(req.body);
    long long merchantId = getParamLong(params, "id");
    std::string merchant_name = urlDecode(getParam(params, "merchant_name"));
    double start_price = safeStod(getParam(params, "start_price", "0"));
    double delivery_fee = safeStod(getParam(params, "delivery_fee", "0"));

    if (merchantId <= 0) {
        res.set_content(errorJson(1, "商家ID不能为空"), "application/json");
        return;
    }

    std::string now = getNowTime();
    std::stringstream ss;
    ss << "UPDATE merchant SET update_time='" << now << "'";
    if (!merchant_name.empty()) {
        ss << ",merchant_name='" << merchant_name << "'";
    }
    if (start_price >= 0) {
        ss << ",start_price=" << start_price;
    }
    if (delivery_fee >= 0) {
        ss << ",delivery_fee=" << delivery_fee;
    }
    ss << " WHERE id=" << merchantId;

    int ret = executeUpdate(ss.str());
    if (ret > 0) {
        res.set_content(successJson(), "application/json");
    } else {
        res.set_content(errorJson(2, "更新失败"), "application/json");
    }
}
