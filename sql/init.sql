CREATE DATABASE IF NOT EXISTS TakeAwayDatabase DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE TakeAwayDatabase;

-- 1.用户表 user
CREATE TABLE `user` (
  `id` bigint NOT NULL AUTO_INCREMENT COMMENT '主键',
  `account` varchar(64) NOT NULL COMMENT '登录账号',
  `password` varchar(255) NOT NULL COMMENT '加密密码',
  `user_name` varchar(64) NOT NULL COMMENT '昵称',
  `phone` varchar(20) DEFAULT NULL COMMENT '手机号',
  `sex` tinyint DEFAULT 0 COMMENT '0未知1男2女',
  `avatar` varchar(255) DEFAULT NULL COMMENT '头像',
  `description` varchar(255) DEFAULT NULL COMMENT '简介',
  `user_type` tinyint DEFAULT 0 COMMENT '0普通用户1商家2骑手',
  `status` tinyint DEFAULT 1 COMMENT '1正常0禁用',
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `update_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_account` (`account`),
  UNIQUE KEY `uk_phone` (`phone`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='平台用户表';

-- 2.角色表 role
CREATE TABLE `role` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `role_code` varchar(32) NOT NULL COMMENT 'USER/MERCHANT/RIDER/ADMIN',
  `role_name` varchar(64) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_code` (`role_code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='角色表';

-- 3.用户角色中间表 user_role
CREATE TABLE `user_role` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `user_id` bigint NOT NULL,
  `role_id` bigint NOT NULL,
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `idx_user` (`user_id`),
  KEY `idx_role` (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户角色多对多';

-- 4.商家表 merchant
CREATE TABLE `merchant` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `user_id` bigint NOT NULL COMMENT '店主用户id',
  `merchant_name` varchar(128) NOT NULL,
  `logo_url` varchar(255) DEFAULT NULL,
  `address` varchar(255) NOT NULL,
  `business_hours` varchar(64) DEFAULT NULL,
  `contact_phone` varchar(20) DEFAULT NULL,
  `delivery_mode` tinyint DEFAULT 1,
  `start_price` decimal(10,2) DEFAULT 0.00,
  `delivery_fee` decimal(10,2) DEFAULT 0.00,
  `status` tinyint DEFAULT 1,
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `update_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='商家店铺';

-- 5.菜品分类 category
CREATE TABLE `category` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `merchant_id` bigint NOT NULL,
  `category_name` varchar(64) NOT NULL,
  `sort_no` int DEFAULT 0,
  `status` tinyint DEFAULT 1,
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `idx_merchant` (`merchant_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='菜品分类';

-- 6.菜品表 dish
CREATE TABLE `dish` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `merchant_id` bigint NOT NULL,
  `category_id` bigint NOT NULL,
  `dish_name` varchar(128) NOT NULL,
  `image_url` varchar(255) DEFAULT NULL,
  `price` decimal(10,2) NOT NULL,
  `original_price` decimal(10,2) DEFAULT NULL,
  `stock` int DEFAULT 0,
  `sold_count` int DEFAULT 0,
  `description` text,
  `status` tinyint DEFAULT 1,
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `update_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `idx_merchant` (`merchant_id`),
  KEY `idx_category` (`category_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='菜品';

-- 7.用户地址 user_address
CREATE TABLE `user_address` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `user_id` bigint NOT NULL,
  `receiver_name` varchar(64) NOT NULL,
  `receiver_phone` varchar(20) NOT NULL,
  `province` varchar(64) NOT NULL,
  `city` varchar(64) NOT NULL,
  `district` varchar(64) NOT NULL,
  `detail_address` varchar(255) NOT NULL,
  `is_default` tinyint DEFAULT 0,
  `latitude` decimal(10,6) DEFAULT NULL,
  `longitude` decimal(10,6) DEFAULT NULL,
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `idx_user` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='收货地址';

-- 8.购物车 cart
CREATE TABLE `cart` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `user_id` bigint NOT NULL,
  `merchant_id` bigint NOT NULL,
  `dish_id` bigint NOT NULL,
  `quantity` int NOT NULL,
  `selected` tinyint DEFAULT 1,
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `update_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `idx_user` (`user_id`),
  KEY `idx_merchant` (`merchant_id`),
  KEY `idx_dish` (`dish_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='购物车';

-- 9.订单主表 orders
CREATE TABLE `orders` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `order_sn` varchar(32) NOT NULL COMMENT '业务订单号',
  `user_id` bigint NOT NULL,
  `merchant_id` bigint NOT NULL,
  `rider_id` bigint DEFAULT NULL,
  `address_id` bigint DEFAULT NULL,
  `total_amount` decimal(10,2) NOT NULL,
  `delivery_fee` decimal(10,2) DEFAULT 0.00,
  `discount_amount` decimal(10,2) DEFAULT 0.00,
  `pay_amount` decimal(10,2) NOT NULL,
  `pay_status` tinyint DEFAULT 0 COMMENT '0未支付1支付中2已支付3支付失败4已退款',
  `order_status` tinyint DEFAULT 0 COMMENT '0待支付 1已支付待接单 2已接单 3制作中 4待取餐 5配送中 6已送达 7已完成 8已取消 9退款中 10已退款 11异常',
  `remark` varchar(255) DEFAULT NULL,
  `idempotency_key` varchar(64) DEFAULT NULL COMMENT '幂等防重key',
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `pay_time` datetime DEFAULT NULL,
  `update_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `finish_time` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_order_sn` (`order_sn`),
  UNIQUE KEY `uk_idempotent` (`idempotency_key`),
  KEY `idx_user` (`user_id`),
  KEY `idx_merchant` (`merchant_id`),
  KEY `idx_status` (`order_status`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='订单主表';

-- 10.订单明细 order_item（快照存储）
CREATE TABLE `order_item` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `order_sn` varchar(32) NOT NULL,
  `dish_id` bigint NOT NULL,
  `dish_name_snapshot` varchar(128) NOT NULL,
  `price_snapshot` decimal(10,2) NOT NULL,
  `quantity` int NOT NULL,
  `item_total` decimal(10,2) NOT NULL,
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `idx_order_sn` (`order_sn`),
  KEY `idx_dish` (`dish_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='订单明细';

-- 11.配送任务 delivery_task
CREATE TABLE `delivery_task` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `order_sn` varchar(32) NOT NULL,
  `rider_id` bigint NOT NULL,
  `pickup_time` datetime DEFAULT NULL,
  `deliver_time` datetime DEFAULT NULL,
  `task_status` tinyint DEFAULT 0 COMMENT '0待接单1已接单2已取餐3配送中4已送达5异常',
  `pickup_lat` decimal(10,6) DEFAULT NULL,
  `pickup_lng` decimal(10,6) DEFAULT NULL,
  `deliver_lat` decimal(10,6) DEFAULT NULL,
  `deliver_lng` decimal(10,6) DEFAULT NULL,
  `exception_reason` varchar(255) DEFAULT NULL,
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `update_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_order_sn` (`order_sn`),
  KEY `idx_rider` (`rider_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='骑手配送任务';

-- 12.售后工单 after_sale
CREATE TABLE `after_sale` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `after_sale_sn` varchar(32) NOT NULL,
  `order_sn` varchar(32) NOT NULL,
  `user_id` bigint NOT NULL,
  `merchant_id` bigint NOT NULL,
  `type` tinyint NOT NULL COMMENT '1退款2投诉3补偿',
  `reason` varchar(255) NOT NULL,
  `evidence_url` varchar(255) DEFAULT NULL,
  `status` tinyint DEFAULT 0 COMMENT '0待处理1处理中2同意3拒绝4完成',
  `operator_id` bigint DEFAULT NULL,
  `handle_result` varchar(255) DEFAULT NULL,
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `update_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_after_sn` (`after_sale_sn`),
  KEY `idx_order_sn` (`order_sn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='售后工单';

-- 13.评价表 review
CREATE TABLE `review` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `order_sn` varchar(32) NOT NULL,
  `user_id` bigint NOT NULL,
  `merchant_id` bigint NOT NULL,
  `score` tinyint NOT NULL COMMENT '1-5星',
  `content` varchar(500) DEFAULT NULL,
  `is_anonymous` tinyint DEFAULT 0,
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_order_sn` (`order_sn`),
  KEY `idx_user` (`user_id`),
  KEY `idx_merchant` (`merchant_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='订单评价';

-- 14.审计日志 audit_log
CREATE TABLE `audit_log` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `operator_id` bigint NOT NULL,
  `operator_role` varchar(32) NOT NULL,
  `module` varchar(64) NOT NULL,
  `action` varchar(64) NOT NULL,
  `target_id` varchar(64) DEFAULT NULL,
  `content` text,
  `ip_address` varchar(64) DEFAULT NULL,
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `idx_operator` (`operator_id`),
  KEY `idx_time` (`create_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='操作审计日志';