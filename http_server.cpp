#include <iostream>
#define CPPHTTPLIB_THREAD_POOL_COUNT 4
#include "httplib.h"
#include "http_handler.hpp"
#include "db.hpp"

#define SERVER_HOST "0.0.0.0"
#define SERVER_PORT 8080

int main() {
    std::cout << "===== 外卖平台 HTTP 服务启动 =====" << std::endl;

    try {
        auto& db = DBConn::getInstance();
        auto conn = db.getConn();
        std::cout << "✅ MySQL数据库连接成功！" << std::endl;
    } catch (sql::SQLException& e) {
        std::cerr << "❌ 数据库连接失败：" << e.what() << std::endl;
        return -1;
    }

    httplib::Server server;
    HttpHandler handler;
    handler.registerRoutes(server);

    server.set_base_dir("/home/riyuetan/TakeAwayPlatform/www");

    std::cout << "🌐 HTTP 服务启动成功！" << std::endl;
    std::cout << "📍 服务地址：http://" << SERVER_HOST << ":" << SERVER_PORT << std::endl;
    std::cout << "📁 静态资源目录：./www" << std::endl;
    std::cout << "\nAPI 接口列表：" << std::endl;
    std::cout << "  GET    /api/health              - 健康检查" << std::endl;
    std::cout << "  POST   /api/user/login          - 用户登录" << std::endl;
    std::cout << "  POST   /api/user/register       - 用户注册" << std::endl;
    std::cout << "  GET    /api/user/list           - 用户列表" << std::endl;
    std::cout << "  GET    /api/user?id=xxx         - 查询用户" << std::endl;
    std::cout << "  POST   /api/user/update         - 更新用户" << std::endl;
    std::cout << "  GET    /api/dish/list?merchant_id=xxx - 菜品列表" << std::endl;
    std::cout << "  GET    /api/dish?id=xxx         - 查询菜品" << std::endl;
    std::cout << "  POST   /api/dish/add            - 添加菜品" << std::endl;
    std::cout << "  POST   /api/order/create        - 创建订单" << std::endl;
    std::cout << "  GET    /api/order?order_no=xxx  - 查询订单" << std::endl;
    std::cout << "  GET    /api/order/list?user_id=xxx - 订单列表" << std::endl;
    std::cout << "  POST   /api/order/pay           - 支付订单" << std::endl;
    std::cout << "  POST   /api/order/cancel        - 取消订单" << std::endl;
    std::cout << "  GET    /api/merchant?user_id=xxx - 查询商家" << std::endl;
    std::cout << "  GET    /api/merchant/stats?merchant_id=xxx - 商家统计" << std::endl;
    std::cout << "  POST   /api/merchant/update     - 更新商家" << std::endl;
    std::cout << "  POST   /api/merchant/update_status - 更新营业状态" << std::endl;
    std::cout << "\n按 Ctrl+C 停止服务" << std::endl;

    server.listen(SERVER_HOST, SERVER_PORT);

    return 0;
}
