#pragma once
#include "db.hpp"
#include "entity.hpp"
#include "enum.hpp"
#include <vector>
#include <optional>

class UserService
{
public:
    // 根据账号密码登录
    std::optional<User> login(const std::string& account, const std::string& password);

    // 新增用户
    int addUser(const User& user);

    // 根据id查询用户
    std::optional<User> getUserById(uint64_t uid);

    // 根据账号查询用户
    std::optional<User> getUserByAccount(const std::string& account);

    // 修改用户信息
    int updateUser(const User& user);

    // 禁用/启用用户
    int setUserStatus(uint64_t uid, UserStatus status);

    // 查询所有用户（分页简单版）
    std::vector<User> listAllUser(int limit = 20, int offset = 0);
};