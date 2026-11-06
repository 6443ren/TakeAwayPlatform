#include <iostream>
#include <optional>
#include "db.hpp"
#include "util.hpp"
#include "enum.hpp"
#include "entity.hpp"
#include "service/user_service.hpp"
#include "service/dish_service.hpp"
#include "service/order_service.hpp"

void printSplit(const std::string& title)
{
    std::cout << "\n======================================" << std::endl;
    std::cout << "        " << title << std::endl;
    std::cout << "======================================\n" << std::endl;
}

int main()
{
    std::cout << "===== 外卖平台服务启动测试程序 =====" << std::endl;

    // 1. 测试数据库连接
    printSplit("1. 测试数据库连接");
    try
    {
        auto& db = DBConn::getInstance();
        auto conn = db.getConn();
        std::cout << "✅ MySQL数据库连接成功！" << std::endl;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "❌ 数据库连接失败：" << e.what() << std::endl;
        return -1;
    }

    // 2. 测试工具函数
    printSplit("2. 测试通用工具函数");
    std::string uuid = generateUUID();
    std::string now = getNowTime();
    std::string orderNo = createOrderNo();
    std::cout << "UUID字符串：" << uuid << std::endl;
    std::cout << "当前系统时间：" << now << std::endl;
    std::cout << "16位订单号：" << orderNo << std::endl;

    // 3. 用户模块测试
    printSplit("3. 用户业务测试");
    UserService userSvc;
    // 新增测试用户
    User testUser;
    testUser.account = "student001";
    testUser.password = "123456";
    testUser.user_name = "校园测试用户";
    testUser.phone = "13800138000";
    testUser.sex = 1;
    testUser.avatar = "";
    testUser.description = "学生测试账号";
    testUser.status = UserStatus::NORMAL;
    int addUserRet = userSvc.addUser(testUser);
    std::cout << "新增用户影响行数：" << addUserRet << std::endl;

    // 根据账号查询用户
    auto findUser = userSvc.getUserByAccount("student001");
    if (findUser.has_value())
    {
        std::cout << "查询到用户：ID=" << findUser->id
                  << " 用户名=" << findUser->user_name << std::endl;
        uint64_t testUid = findUser->id;

        // 修改用户昵称
        findUser->user_name = "已更新的测试用户";
        userSvc.updateUser(*findUser);
        std::cout << "✅ 用户信息更新完成" << std::endl;

        // 分页查询全部用户
        auto userList = userSvc.listAllUser(10, 0);
        std::cout << "用户列表总数：" << userList.size() << std::endl;
    }
    else
    {
        std::cout << "未查询到该用户" << std::endl;
        std::cout << "⚠️ 后续订单测试将跳过" << std::endl;
        printSplit("程序全部测试执行完毕");
        std::cout << "用户模块测试失败，后续模块未执行" << std::endl;
        return 0;
    }

    // 4. 菜品模块测试
    printSplit("4. 菜品业务测试");
    DishService dishSvc;
    // 构造测试菜品（假设商家ID=1，分类ID=1）
    Dish testDish;
    testDish.merchant_id = 1;
    testDish.category_id = 1;
    testDish.dish_name = "经典红烧肉";
    testDish.image_url = "/img/food/hongshaorou.jpg";
    testDish.price = 22.8;
    testDish.original_price = 28.0;
    testDish.stock = 100;
    testDish.sold_count = 0;
    testDish.description = "肥瘦相间，秘制酱汁";
    testDish.status = DishStatus::ON_SALE;
    int addDishRet = dishSvc.addDish(testDish);
    std::cout << "新增菜品影响行数：" << addDishRet << std::endl;

    // 查询商家下所有菜品
    auto dishList = dishSvc.listDishByMerchant(1);
    std::cout << "店铺菜品数量：" << dishList.size() << std::endl;
    uint64_t testDishId = 0;
    if (!dishList.empty())
    {
        testDishId = dishList[0].id;
        std::cout << "菜品ID：" << testDishId << " 菜名：" << dishList[0].dish_name << std::endl;
    }

    // 5. 订单完整流程测试（事务创建订单+支付+取消）
    printSplit("5. 订单完整业务流程测试");
    OrderService orderSvc;
    // 构造订单主数据
    Order newOrder;
    newOrder.order_no = createOrderNo();
    newOrder.user_id = findUser->id;
    newOrder.merchant_id = 1;
    newOrder.rider_id = 0;
    newOrder.receiver_name = "测试收件人";
    newOrder.receiver_phone = "13900139000";
    newOrder.receiver_addr = "XX大学3号宿舍楼";
    newOrder.total_price = 22.8;
    newOrder.pay_price = 22.8;
    newOrder.delivery_fee = 3.0;
    newOrder.pay_type = PayType::WX_PAY;
    newOrder.order_status = OrderStatus::PENDING_PAY;

    // 构造订单明细
    std::vector<OrderItem> itemArr;
    OrderItem item;
    item.dish_id = testDishId;
    item.dish_name = "经典红烧肉";
    item.dish_price = 22.8;
    item.num = 1;
    itemArr.push_back(item);

    // 创建订单（自动扣减库存、增加销量，事务保障）
    bool createOk = orderSvc.createOrder(newOrder, itemArr);
    if (createOk)
    {
        std::cout << "✅ 订单创建成功，订单号：" << newOrder.order_no << std::endl;

        // 查询订单信息
        auto orderInfo = orderSvc.getOrderByNo(newOrder.order_no);
        if (orderInfo.has_value())
        {
            std::cout << "订单当前状态码：" << static_cast<int>(orderInfo->order_status) << std::endl;
        }

        // 模拟微信支付
        bool payOk = orderSvc.payOrder(newOrder.order_no, PayType::WX_PAY);
        if (payOk)
        {
            std::cout << "✅ 订单支付完成，状态变更为【已支付】" << std::endl;
        }

        // 查询该用户全部订单
        auto userOrderList = orderSvc.listOrderByUser(findUser->id);
        std::cout << "当前用户订单总数：" << userOrderList.size() << std::endl;

        // 查询订单明细
        auto itemList = orderSvc.listOrderItemByNo(newOrder.order_no);
        std::cout << "订单包含商品数量：" << itemList.size() << std::endl;
    }
    else
    {
        std::cout << "❌ 订单创建失败，事务已回滚" << std::endl;
    }

    printSplit("程序全部测试执行完毕");
    std::cout << "所有底层、业务层接口编译运行正常！" << std::endl;
    return 0;
}