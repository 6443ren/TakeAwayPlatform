USE TakeAwayDatabase;

-- 1.角色基础数据
INSERT IGNORE INTO role (role_code, role_name, description)
VALUES
('USER', '普通点餐用户', '平台顾客'),
('MERCHANT', '商家', '店铺经营者'),
('RIDER', '骑手', '配送人员'),
('ADMIN', '平台管理员', '后台系统管理');

-- 2.平台用户（管理员、5个商家账号、3个普通用户）
INSERT IGNORE INTO user (account, password, user_name, phone, sex, avatar, description, user_type, status)
VALUES
('admin', 'admin123', '平台管理员', '13900000000', 1, '', '系统超级管理员', 0, 1),
('shop_laowang', '123456', '老王老板', '13800001111', 1, '', '家常菜店主', 1, 1),
('shop_tianyuan', '123456', '甜品老板娘', '13800002222', 0, '', '甜园甜品店主', 1, 1),
('shop_malatang', '123456', '麻辣烫店主', '13800003333', 1, '', '鲜香麻辣烫店主', 1, 1),
('shop_kfc', '123456', '炸鸡店老板', '13800004444', 1, '', '快乐炸鸡店主', 1, 1),
('shop_mian', '123456', '面馆老板', '13800005555', 1, '', '老北京面馆店主', 1, 1),
('test_user1', '123456', '小明同学', '13700001111', 1, '', '大学生', 0, 1),
('test_user2', '123456', '小红同学', '13700002222', 0, '', '大学生', 0, 1),
('test_user3', '123456', '小刚同学', '13700003333', 1, '', '研究生', 0, 1);

-- 3.用户绑定角色
INSERT IGNORE INTO user_role (user_id, role_id)
VALUES
(1, 4),
(2, 2),
(3, 2),
(4, 2),
(5, 2),
(6, 2),
(7, 1),
(8, 1),
(9, 1);

-- 4.商家信息
INSERT IGNORE INTO merchant (user_id, merchant_name, logo_url, address, business_hours, contact_phone, delivery_mode, start_price, delivery_fee, status)
VALUES
(2, '老王家常菜', '/img/shop/laowang.jpg', '大学城商业街12号', '10:00-22:00', '13800001111', 1, 15.00, 3.00, 1),
(3, '甜园甜品店', '/img/shop/tianyuan.jpg', '大学城二食堂旁', '09:00-21:30', '13800002222', 0, 20.00, 0.00, 1),
(4, '鲜香麻辣烫', '/img/shop/malatang.jpg', '大学生活区5栋楼下', '11:00-23:00', '13800003333', 1, 12.00, 2.50, 1),
(5, '快乐炸鸡店', '/img/shop/kfc.jpg', '大学城步行街88号', '10:00-23:00', '13800004444', 1, 18.00, 4.00, 1),
(6, '老北京面馆', '/img/shop/mian.jpg', '大学城西门外', '07:00-21:00', '13800005555', 1, 10.00, 2.00, 1);

-- 5.菜品分类
INSERT IGNORE INTO category (merchant_id, category_name, sort_no, status)
VALUES
(1, '招牌硬菜', 1, 1),
(1, '素菜小炒', 2, 1),
(1, '汤品主食', 3, 1),
(2, '奶油蛋糕', 1, 1),
(2, '奶茶果饮', 2, 1),
(2, '特色甜点', 3, 1),
(3, '肉类丸子', 1, 1),
(3, '新鲜蔬菜', 2, 1),
(3, '主食粉条', 3, 1),
(4, '炸鸡套餐', 1, 1),
(4, '汉堡薯条', 2, 1),
(4, '饮料甜品', 3, 1),
(5, '招牌面', 1, 1),
(5, '特色面', 2, 1),
(5, '小吃凉菜', 3, 1);

-- 6.测试菜品 - 老王家常菜
INSERT IGNORE INTO dish (merchant_id, category_id, dish_name, image_url, price, original_price, stock, sold_count, description, status)
VALUES
(1, 1, '经典红烧肉', '/img/food/hongshaorou.jpg', 22.80, 28.00, 100, 156, '肥瘦相间，秘制酱汁慢炖', 1),
(1, 1, '红烧排骨', '/img/food/paigu.jpg', 26.80, 32.00, 80, 89, '精选肋排，炖至软烂入味', 1),
(1, 1, '宫保鸡丁', '/img/food/gongbao.jpg', 18.80, 22.00, 120, 234, '鸡肉嫩滑，花生酥脆', 1),
(1, 1, '鱼香肉丝', '/img/food/yuxiang.jpg', 16.80, 20.00, 100, 178, '酸甜微辣，下饭神器', 1),
(1, 2, '清炒油麦菜', '/img/food/youmaicai.jpg', 12.80, 16.00, 200, 67, '当日新鲜青菜，清淡爽口', 1),
(1, 2, '蒜蓉西兰花', '/img/food/xilanhua.jpg', 14.80, 18.00, 150, 45, '蒜香浓郁，营养健康', 1),
(1, 2, '干煸四季豆', '/img/food/sijidou.jpg', 13.80, 16.00, 180, 98, '香辣过瘾，外脆里嫩', 1),
(1, 3, '番茄蛋汤', '/img/food/fanqie.jpg', 8.80, 10.00, 200, 201, '酸甜可口，老少皆宜', 1),
(1, 3, '紫菜蛋花汤', '/img/food/zicai.jpg', 7.80, 9.00, 200, 156, '鲜美清淡，解腻首选', 1),
(1, 3, '扬州炒饭', '/img/food/chaofan.jpg', 15.80, 18.00, 100, 134, '粒粒分明，配料丰富', 1);

-- 7.测试菜品 - 甜园甜品店
INSERT IGNORE INTO dish (merchant_id, category_id, dish_name, image_url, price, original_price, stock, sold_count, description, status)
VALUES
(2, 1, '草莓奶油蛋糕', '/img/food/caomei.jpg', 38.00, 45.00, 50, 78, '新鲜草莓，动物奶油', 1),
(2, 1, '芒果千层蛋糕', '/img/food/mangguo.jpg', 42.00, 50.00, 40, 56, '千层薄饼，芒果多多', 1),
(2, 1, '巧克力慕斯', '/img/food/qiaokeli.jpg', 35.00, 42.00, 60, 43, '丝滑口感，浓郁可可', 1),
(2, 2, '珍珠奶茶', '/img/food/naicha.jpg', 15.80, 18.00, 200, 345, 'Q弹珍珠，茶香浓郁', 1),
(2, 2, '杨枝甘露', '/img/food/ganlu.jpg', 18.80, 22.00, 100, 123, '芒果西柚，椰香满满', 1),
(2, 2, '芝士奶盖茶', '/img/food/naigai.jpg', 17.80, 20.00, 150, 167, '浓郁芝士，茶香清新', 1),
(2, 3, '提拉米苏', '/img/food/tilami.jpg', 28.00, 32.00, 80, 67, '经典意式，咖啡酒香', 1),
(2, 3, '焦糖布丁', '/img/food/buding.jpg', 12.80, 15.00, 100, 89, '嫩滑爽口，焦糖香甜', 1);

-- 8.测试菜品 - 鲜香麻辣烫
INSERT IGNORE INTO dish (merchant_id, category_id, dish_name, image_url, price, original_price, stock, sold_count, description, status)
VALUES
(3, 1, '精选肥牛卷', '/img/food/feiniu.jpg', 25.80, 30.00, 80, 145, '新鲜肥牛，涮煮鲜嫩', 1),
(3, 1, '手工虾滑', '/img/food/xiahua.jpg', 22.80, 26.00, 60, 98, 'Q弹虾滑，鲜美可口', 1),
(3, 1, '撒尿牛丸', '/img/food/niuwan.jpg', 18.80, 22.00, 100, 178, '一口爆汁，口感丰富', 1),
(3, 1, '午餐肉', '/img/food/wucanrou.jpg', 12.80, 15.00, 150, 134, '经典午餐肉，涮煮入味', 1),
(3, 2, '娃娃菜', '/img/food/wawacai.jpg', 6.80, 8.00, 200, 234, '新鲜娃娃菜，清甜爽口', 1),
(3, 2, '金针菇', '/img/food/jinzhengu.jpg', 5.80, 7.00, 200, 312, '爽脆口感，吸味神器', 1),
(3, 2, '菠菜', '/img/food/bocai.jpg', 5.80, 7.00, 200, 156, '新鲜菠菜，营养健康', 1),
(3, 2, '莲藕', '/img/food/lianou.jpg', 7.80, 9.00, 150, 89, '脆嫩莲藕，清香爽口', 1),
(3, 3, '红薯粉', '/img/food/hongshufen.jpg', 8.80, 10.00, 200, 178, '手工红薯粉，爽滑劲道', 1),
(3, 3, '方便面', '/img/food/fangbianmian.jpg', 6.80, 8.00, 200, 267, '吸汤入味，经典必点', 1);

-- 9.测试菜品 - 快乐炸鸡店
INSERT IGNORE INTO dish (merchant_id, category_id, dish_name, image_url, price, original_price, stock, sold_count, description, status)
VALUES
(4, 1, '炸鸡套餐A', '/img/food/zhajiA.jpg', 35.80, 42.00, 100, 234, '5块炸鸡+薯条+饮料', 1),
(4, 1, '炸鸡套餐B', '/img/food/zhajiB.jpg', 45.80, 55.00, 80, 156, '8块炸鸡+薯条+汉堡+饮料', 1),
(4, 1, '香辣鸡腿堡', '/img/food/jitui.jpg', 18.80, 22.00, 150, 345, '酥脆鸡腿，香辣可口', 1),
(4, 1, '奥尔良烤鸡腿', '/img/food/aoerliang.jpg', 15.80, 18.00, 200, 189, '奥尔良风味，鲜嫩多汁', 1),
(4, 2, '牛肉汉堡', '/img/food/niurouhanbao.jpg', 22.80, 26.00, 100, 123, '纯牛肉饼，芝士浓郁', 1),
(4, 2, '薯条(大)', '/img/food/shutiao.jpg', 12.80, 15.00, 200, 456, '金黄酥脆，外酥里嫩', 1),
(4, 2, '鸡块(6块)', '/img/food/jikuai.jpg', 16.80, 20.00, 150, 278, '外酥里嫩，蘸酱美味', 1),
(4, 3, '可乐(大)', '/img/food/keke.jpg', 8.80, 10.00, 300, 512, '冰爽可乐，畅饮畅快', 1),
(4, 3, '柠檬茶', '/img/food/ningmeng.jpg', 12.80, 15.00, 200, 167, '清新柠檬，酸甜解渴', 1);

-- 10.测试菜品 - 老北京面馆
INSERT IGNORE INTO dish (merchant_id, category_id, dish_name, image_url, price, original_price, stock, sold_count, description, status)
VALUES
(5, 1, '招牌炸酱面', '/img/food/zhajiang.jpg', 18.80, 22.00, 150, 345, '秘制炸酱，酱香浓郁', 1),
(5, 1, '牛肉拉面', '/img/food/niuroumians.jpg', 22.80, 26.00, 120, 234, '红烧牛肉，汤鲜味美', 1),
(5, 1, '番茄鸡蛋面', '/img/food/fanqiemian.jpg', 15.80, 18.00, 150, 189, '番茄浓郁，鸡蛋嫩滑', 1),
(5, 2, '麻辣小面', '/img/food/malaxm.jpg', 16.80, 20.00, 150, 267, '麻辣鲜香，劲道爽滑', 1),
(5, 2, '重庆酸辣粉', '/img/food/suanla.jpg', 17.80, 21.00, 120, 178, '酸辣过瘾，开胃首选', 1),
(5, 2, '油泼面', '/img/food/youpo.jpg', 18.80, 22.00, 100, 145, '热油泼面，香气扑鼻', 1),
(5, 3, '拍黄瓜', '/img/food/huanggua.jpg', 8.80, 10.00, 200, 134, '清爽解腻，酸辣可口', 1),
(5, 3, '凉拌海带丝', '/img/food/haidai.jpg', 7.80, 9.00, 200, 98, '鲜香爽口，开胃小菜', 1),
(5, 3, '卤蛋', '/img/food/ludan.jpg', 3.80, 4.50, 300, 456, '卤香浓郁，蛋香四溢', 1);

-- 11.测试用户收货地址
INSERT IGNORE INTO user_address (user_id, receiver_name, receiver_phone, province, city, district, detail_address, is_default)
VALUES
(1, '测试收件人', '13900139000', '湖北省', '武汉市', '洪山区', '华中师范大学3号宿舍楼', 1),
(7, '小明同学', '13700001111', '湖北省', '武汉市', '洪山区', '华中师范大学5号宿舍楼', 1),
(8, '小红同学', '13700002222', '湖北省', '武汉市', '洪山区', '华中师范大学2号教学楼', 1);
