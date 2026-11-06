#pragma once
#include "db.hpp"
#include "entity.hpp"
#include "enum.hpp"
#include <vector>
#include <optional>

class DishService
{
public:
    // 新增菜品
    int addDish(const Dish& dish);

    // 根据菜品id查询
    std::optional<Dish> getDishById(uint64_t dishId);

    // 根据商家id查询该店铺所有菜品（分页）
    std::vector<Dish> listDishByMerchant(uint64_t merchantId, int limit = 20, int offset = 0);

    // 查询所有菜品（管理端用）
    std::vector<Dish> listAllDishes(int limit = 50, int offset = 0);

    // 根据分类id查菜品
    std::vector<Dish> listDishByCategory(uint64_t categoryId);

    // 修改菜品信息
    int updateDish(const Dish& dish);

    // 上下架菜品
    int setDishStatus(uint64_t dishId, DishStatus status);

    // 扣减库存（下单使用）
    int reduceStock(uint64_t dishId, int num);

    // 增加销量
    int addSoldCount(uint64_t dishId, int num);
};