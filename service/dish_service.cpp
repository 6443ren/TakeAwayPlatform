#include "dish_service.hpp"
#include "util.hpp"
#include <sstream>

int DishService::addDish(const Dish& dish)
{
    std::string now = getNowTime();
    std::stringstream ss;
    ss << "INSERT INTO dish(merchant_id,category_id,dish_name,image_url,price,original_price,stock,sold_count,description,status,create_time,update_time) "
       << "VALUES("
       << dish.merchant_id << ","
       << dish.category_id << ",'"
       << dish.dish_name << "','"
       << dish.image_url << "',"
       << dish.price << ","
       << dish.original_price << ","
       << dish.stock << ","
       << dish.sold_count << ",'"
       << dish.description << "',"
       << static_cast<int>(dish.status) << ",'"
       << now << "','" << now << "')";
    return executeUpdate(ss.str());
}

std::optional<Dish> DishService::getDishById(uint64_t dishId)
{
    std::string sql = "SELECT id,merchant_id,category_id,dish_name,image_url,price,original_price,stock,sold_count,description,status,create_time,update_time "
                      "FROM dish WHERE id=" + std::to_string(dishId) + " LIMIT 1";
    auto res = query(sql);
    if (res.empty())
        return std::nullopt;

    Dish d;
    auto& row = res[0];
    d.id = strToLL(row[0]);
    d.merchant_id = strToLL(row[1]);
    d.category_id = strToLL(row[2]);
    d.dish_name = row[3];
    d.image_url = row[4];
    d.price = safeStod(row[5]);
    d.original_price = safeStod(row[6]);
    d.stock = safeStoi(row[7]);
    d.sold_count = safeStoi(row[8]);
    d.description = row[9];
    d.status = static_cast<DishStatus>(safeStoi(row[10]));
    d.create_time = row[11];
    d.update_time = row[12];
    return d;
}

std::vector<Dish> DishService::listDishByMerchant(uint64_t merchantId, int limit, int offset)
{
    std::vector<Dish> list;
    std::string sql = "SELECT id,merchant_id,category_id,dish_name,image_url,price,original_price,stock,sold_count,description,status,create_time,update_time "
                      "FROM dish WHERE merchant_id=" + std::to_string(merchantId)
                      + " LIMIT " + std::to_string(limit) + " OFFSET " + std::to_string(offset);
    auto res = query(sql);
    for (auto& row : res)
    {
        Dish d;
        d.id = strToLL(row[0]);
        d.merchant_id = strToLL(row[1]);
        d.category_id = strToLL(row[2]);
        d.dish_name = row[3];
        d.image_url = row[4];
        d.price = safeStod(row[5]);
        d.original_price = safeStod(row[6]);
        d.stock = safeStoi(row[7]);
        d.sold_count = safeStoi(row[8]);
        d.description = row[9];
        d.status = static_cast<DishStatus>(safeStoi(row[10]));
        d.create_time = row[11];
        d.update_time = row[12];
        list.push_back(d);
    }
    return list;
}

std::vector<Dish> DishService::listAllDishes(int limit, int offset)
{
    std::vector<Dish> list;
    std::string sql = "SELECT id,merchant_id,category_id,dish_name,image_url,price,original_price,stock,sold_count,description,status,create_time,update_time "
                      "FROM dish LIMIT " + std::to_string(limit) + " OFFSET " + std::to_string(offset);
    auto res = query(sql);
    for (auto& row : res)
    {
        Dish d;
        d.id = strToLL(row[0]);
        d.merchant_id = strToLL(row[1]);
        d.category_id = strToLL(row[2]);
        d.dish_name = row[3];
        d.image_url = row[4];
        d.price = safeStod(row[5]);
        d.original_price = safeStod(row[6]);
        d.stock = safeStoi(row[7]);
        d.sold_count = safeStoi(row[8]);
        d.description = row[9];
        d.status = static_cast<DishStatus>(safeStoi(row[10]));
        d.create_time = row[11];
        d.update_time = row[12];
        list.push_back(d);
    }
    return list;
}

std::vector<Dish> DishService::listDishByCategory(uint64_t categoryId)
{
    std::vector<Dish> list;
    std::string sql = "SELECT id,merchant_id,category_id,dish_name,image_url,price,original_price,stock,sold_count,description,status,create_time,update_time "
                      "FROM dish WHERE category_id=" + std::to_string(categoryId);
    auto res = query(sql);
    for (auto& row : res)
    {
        Dish d;
        d.id = strToLL(row[0]);
        d.merchant_id = strToLL(row[1]);
        d.category_id = strToLL(row[2]);
        d.dish_name = row[3];
        d.image_url = row[4];
        d.price = safeStod(row[5]);
        d.original_price = safeStod(row[6]);
        d.stock = safeStoi(row[7]);
        d.sold_count = safeStoi(row[8]);
        d.description = row[9];
        d.status = static_cast<DishStatus>(safeStoi(row[10]));
        d.create_time = row[11];
        d.update_time = row[12];
        list.push_back(d);
    }
    return list;
}

int DishService::updateDish(const Dish& dish)
{
    std::string now = getNowTime();
    std::stringstream ss;
    ss << "UPDATE dish SET category_id=" << dish.category_id
       << ",dish_name='" << dish.dish_name
       << "',image_url='" << dish.image_url
       << "',price=" << dish.price
       << ",original_price=" << dish.original_price
       << ",stock=" << dish.stock
       << ",description='" << dish.description
       << "',status=" << static_cast<int>(dish.status)
       << ",update_time='" << now
       << "' WHERE id=" << dish.id;
    return executeUpdate(ss.str());
}

int DishService::setDishStatus(uint64_t dishId, DishStatus status)
{
    std::string now = getNowTime();
    std::string sql = "UPDATE dish SET status=" + std::to_string(static_cast<int>(status))
                      + ",update_time='" + now + "' WHERE id=" + std::to_string(dishId);
    return executeUpdate(sql);
}

int DishService::reduceStock(uint64_t dishId, int num)
{
    std::string now = getNowTime();
    std::string sql = "UPDATE dish SET stock=stock-" + std::to_string(num)
                      + ",update_time='" + now + "' WHERE id=" + std::to_string(dishId);
    return executeUpdate(sql);
}

int DishService::addSoldCount(uint64_t dishId, int num)
{
    std::string now = getNowTime();
    std::string sql = "UPDATE dish SET sold_count=sold_count+" + std::to_string(num)
                      + ",update_time='" + now + "' WHERE id=" + std::to_string(dishId);
    return executeUpdate(sql);
}