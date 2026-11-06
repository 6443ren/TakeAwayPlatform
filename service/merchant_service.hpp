#pragma once
#include "db.hpp"
#include "entity.hpp"
#include <vector>
#include <optional>

class MerchantService {
public:
    std::vector<Merchant> listAllMerchant();
    std::optional<Merchant> getMerchantById(uint64_t id);
    std::optional<Merchant> getMerchantByUserId(uint64_t userId);
    int addMerchant(const Merchant& merchant);
    int updateMerchant(const Merchant& merchant);
};
