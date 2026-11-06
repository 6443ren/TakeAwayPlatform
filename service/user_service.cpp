#include "user_service.hpp"
#include "util.hpp"
#include <sstream>

// 账号密码登录
std::optional<User> UserService::login(const std::string& account, const std::string& password)
{
    std::string sql = "SELECT id,account,password,user_name,phone,sex,avatar,description,user_type,status,create_time,update_time "
                      "FROM user WHERE account='" + account + "' AND password='" + password + "' LIMIT 1";
    auto res = query(sql);
    if (res.empty())
        return std::nullopt;

    User u;
    auto& row = res[0];
    u.id = strToLL(row[0]);
    u.account = row[1];
    u.password = row[2];
    u.user_name = row[3];
    u.phone = row[4];
    u.sex = safeStoi(row[5]);
    u.avatar = row[6];
    u.description = row[7];
    u.user_type = static_cast<UserType>(safeStoi(row[8]));
    u.status = static_cast<UserStatus>(safeStoi(row[9]));
    u.create_time = row[10];
    u.update_time = row[11];
    return u;
}

// 新增用户
int UserService::addUser(const User& user)
{
    std::string now = getNowTime();
    std::stringstream ss;
    ss << "INSERT INTO user(account,password,user_name,phone,sex,avatar,description,user_type,status,create_time,update_time) "
       << "VALUES('" << user.account << "','" << user.password << "','" << user.user_name << "','" << user.phone << "',"
       << user.sex << ",'" << user.avatar << "','" << user.description << "',"
       << static_cast<int>(user.user_type) << "," << static_cast<int>(user.status) << ",'" << now << "','" << now << "')";
    return executeUpdate(ss.str());
}

// ID查用户
std::optional<User> UserService::getUserById(uint64_t uid)
{
    std::string sql = "SELECT id,account,password,user_name,phone,sex,avatar,description,user_type,status,create_time,update_time "
                      "FROM user WHERE id=" + std::to_string(uid) + " LIMIT 1";
    auto res = query(sql);
    if (res.empty())
        return std::nullopt;

    User u;
    auto& row = res[0];
    u.id = strToLL(row[0]);
    u.account = row[1];
    u.password = row[2];
    u.user_name = row[3];
    u.phone = row[4];
    u.sex = safeStoi(row[5]);
    u.avatar = row[6];
    u.description = row[7];
    u.user_type = static_cast<UserType>(safeStoi(row[8]));
    u.status = static_cast<UserStatus>(safeStoi(row[9]));
    u.create_time = row[10];
    u.update_time = row[11];
    return u;
}

// 账号查用户
std::optional<User> UserService::getUserByAccount(const std::string& account)
{
    std::string sql = "SELECT id,account,password,user_name,phone,sex,avatar,description,user_type,status,create_time,update_time "
                      "FROM user WHERE account='" + account + "' LIMIT 1";
    auto res = query(sql);
    if (res.empty())
        return std::nullopt;

    User u;
    auto& row = res[0];
    u.id = strToLL(row[0]);
    u.account = row[1];
    u.password = row[2];
    u.user_name = row[3];
    u.phone = row[4];
    u.sex = safeStoi(row[5]);
    u.avatar = row[6];
    u.description = row[7];
    u.user_type = static_cast<UserType>(safeStoi(row[8]));
    u.status = static_cast<UserStatus>(safeStoi(row[9]));
    u.create_time = row[10];
    u.update_time = row[11];
    return u;
}

// 修改用户信息
int UserService::updateUser(const User& user)
{
    std::string now = getNowTime();
    std::stringstream ss;
    ss << "UPDATE user SET user_name='" << user.user_name << "',phone='" << user.phone
       << "',sex=" << user.sex << ",avatar='" << user.avatar << "',description='" << user.description
       << "',status=" << static_cast<int>(user.status) << ",update_time='" << now
       << "' WHERE id=" << user.id;
    return executeUpdate(ss.str());
}

// 设置用户启用/禁用状态
int UserService::setUserStatus(uint64_t uid, UserStatus status)
{
    std::string now = getNowTime();
    std::string sql = "UPDATE user SET status=" + std::to_string(static_cast<int>(status))
                      + ",update_time='" + now + "' WHERE id=" + std::to_string(uid);
    return executeUpdate(sql);
}

// 分页查询所有用户
std::vector<User> UserService::listAllUser(int limit, int offset)
{
    std::vector<User> ret;
    std::string sql = "SELECT id,account,password,user_name,phone,sex,avatar,description,user_type,status,create_time,update_time "
                      "FROM user LIMIT " + std::to_string(limit) + " OFFSET " + std::to_string(offset);
    auto res = query(sql);
    for (auto& row : res)
    {
        User u;
        u.id = strToLL(row[0]);
        u.account = row[1];
        u.password = row[2];
        u.user_name = row[3];
        u.phone = row[4];
        u.sex = safeStoi(row[5]);
        u.avatar = row[6];
        u.description = row[7];
        u.user_type = static_cast<UserType>(safeStoi(row[8]));
        u.status = static_cast<UserStatus>(safeStoi(row[9]));
        u.create_time = row[10];
        u.update_time = row[11];
        ret.push_back(u);
    }
    return ret;
}