const API_BASE = '/api';
let currentUser = null;
let cart = [];
let merchants = [];
let currentMerchant = null;
let currentFilterCategory = 0;
let currentOrderFilter = 'all';
let currentOrders = [];

function showPage(pageId) {
    document.querySelectorAll('.page').forEach(p => p.classList.add('hidden'));
    document.getElementById(`page-${pageId}`).classList.remove('hidden');
    
    document.querySelectorAll('.nav-item').forEach(n => n.classList.remove('active'));
    document.querySelector(`.nav-item[onclick="showPage('${pageId}')"]`)?.classList.add('active');
    
    if (pageId === 'orders') {
        loadOrders();
    }
}

function showToast(msg) {
    const toast = document.getElementById('toast');
    toast.textContent = msg;
    toast.classList.remove('hidden');
    setTimeout(() => toast.classList.add('hidden'), 2000);
}

function showLogin() {
    document.getElementById('login-modal').classList.remove('hidden');
    document.getElementById('register-modal').classList.add('hidden');
}

function closeLogin() {
    document.getElementById('login-modal').classList.add('hidden');
}

function showRegister() {
    document.getElementById('register-modal').classList.remove('hidden');
    document.getElementById('login-modal').classList.add('hidden');
}

function closeRegister() {
    document.getElementById('register-modal').classList.add('hidden');
}

function doLogin() {
    const account = document.getElementById('login-account').value;
    const password = document.getElementById('login-password').value;
    
    if (!account || !password) {
        showToast('请输入账号和密码');
        return;
    }
    
    fetch(`${API_BASE}/user/login`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: `account=${encodeURIComponent(account)}&password=${encodeURIComponent(password)}`
    })
    .then(res => res.json())
    .then(data => {
        if (data.code === 0) {
            currentUser = data.data;
            localStorage.setItem('takeaway_user', JSON.stringify(currentUser));
            
            if (currentUser.user_type === 1) {
                window.location.href = 'merchant.html';
                return;
            }
            
            updateUserInfo();
            closeLogin();
            showToast('登录成功');
            loadOrders();
        } else {
            showToast(data.message);
        }
    })
    .catch(err => showToast('登录失败'));
}

function doRegister() {
    const account = document.getElementById('reg-account').value;
    const password = document.getElementById('reg-password').value;
    const name = document.getElementById('reg-name').value;
    const phone = document.getElementById('reg-phone').value;
    const userType = document.getElementById('reg-user-type').value;
    
    if (!account || !password) {
        showToast('账号和密码不能为空');
        return;
    }
    
    fetch(`${API_BASE}/user/register`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: `account=${encodeURIComponent(account)}&password=${encodeURIComponent(password)}&user_name=${encodeURIComponent(name)}&phone=${encodeURIComponent(phone)}&user_type=${encodeURIComponent(userType)}`
    })
    .then(res => res.json())
    .then(data => {
        if (data.code === 0) {
            showToast('注册成功，请登录');
            showLogin();
        } else {
            showToast(data.message);
        }
    })
    .catch(err => showToast('注册失败'));
}

function logout() {
    currentUser = null;
    localStorage.removeItem('takeaway_user');
    updateUserInfo();
    showToast('已退出登录');
    cart = [];
    updateCart();
}

function updateUserInfo() {
    if (currentUser) {
        document.getElementById('profile-name').textContent = currentUser.user_name;
        document.querySelectorAll('.nav-item').forEach(n => {
            if (n.textContent.includes('登录')) {
                n.innerHTML = '<span class="nav-icon">👤</span><span>' + currentUser.user_name + '</span>';
                n.onclick = showPage.bind(null, 'profile');
            }
        });
    } else {
        document.getElementById('profile-name').textContent = '未登录';
        document.querySelectorAll('.nav-item').forEach(n => {
            if (!n.textContent.includes('首页') && !n.textContent.includes('订单')) {
                n.innerHTML = '<span class="nav-icon">👤</span><span>登录</span>';
                n.onclick = showLogin;
            }
        });
    }
}

function handleSearch() {
    const keyword = document.getElementById('search-input').value.toLowerCase();
    const merchantList = document.getElementById('merchant-list');
    
    merchantList.innerHTML = '';
    merchants.forEach(m => {
        if (!keyword || m.shop_name.toLowerCase().includes(keyword)) {
            renderMerchantCard(m);
        }
    });
}

function filterByCategory(categoryId) {
    currentFilterCategory = categoryId;
    document.querySelectorAll('.category-item').forEach(c => c.classList.remove('active'));
    document.querySelector(`.category-item[onclick="filterByCategory(${categoryId})"]`)?.classList.add('active');
    
    loadMerchants();
}

function filterOrders(type) {
    currentOrderFilter = type;
    document.querySelectorAll('.tab-btn').forEach(t => t.classList.remove('active'));
    document.querySelector(`.tab-btn[onclick="filterOrders('${type}')"]`)?.classList.add('active');
    
    renderOrders();
}

function loadMerchants() {
    fetch(`${API_BASE}/merchant/list`)
    .then(res => res.json())
    .then(data => {
        merchants = data.code === 0 ? data.data.list : [];
        
        const merchantList = document.getElementById('merchant-list');
        merchantList.innerHTML = '';
        
        if (merchants.length === 0) {
            merchantList.innerHTML = '<div class="empty-state"><p>暂无商家</p></div>';
            return;
        }
        
        merchants.forEach(m => {
            renderMerchantCard(m);
        });
    });
}

function renderMerchantCard(m) {
    const card = document.createElement('div');
    card.className = 'merchant-card';
    card.onclick = () => openMerchantDetail(m.id);
    
    const shopIcons = ['🏪', '🍜', '🍕', '🍰', '🥘', '🍝', '🍢'];
    const icon = shopIcons[m.id % shopIcons.length];
    
    card.innerHTML = `
        <div class="card-img">${icon}</div>
        <div class="card-content">
            <h3>${m.shop_name}</h3>
            <p>${m.address}</p>
            <div class="merchant-info">
                <span>🚗 ¥${m.delivery_fee}</span>
                <span>💰 起送¥${m.start_price}</span>
                <span>${m.status === 1 ? '营业中' : '休息中'}</span>
            </div>
        </div>
    `;
    
    document.getElementById('merchant-list').appendChild(card);
}

function openMerchantDetail(merchantId) {
    currentMerchant = merchants.find(m => m.id === merchantId);
    if (!currentMerchant) return;
    
    document.getElementById('modal-merchant-name').textContent = currentMerchant.shop_name;
    document.getElementById('modal-merchant-address').textContent = currentMerchant.address;
    document.getElementById('modal-delivery-fee').textContent = currentMerchant.delivery_fee;
    document.getElementById('modal-start-price').textContent = currentMerchant.start_price;
    
    document.getElementById('merchant-detail-modal').classList.remove('hidden');
    
    loadMerchantDishes(merchantId);
}

function closeMerchantModal() {
    document.getElementById('merchant-detail-modal').classList.add('hidden');
    currentMerchant = null;
}

function loadMerchantDishes(merchantId) {
    fetch(`${API_BASE}/dish/list?merchant_id=${merchantId}`)
    .then(res => res.json())
    .then(data => {
        const dishes = data.code === 0 ? data.data.list : [];
        renderMerchantDishes(dishes);
    });
}

function filterMerchantDishes(filterType) {
    document.querySelectorAll('.dish-cat-btn').forEach(b => b.classList.remove('active'));
    document.querySelector(`.dish-cat-btn[onclick="filterMerchantDishes(${filterType})"]`)?.classList.add('active');
    
    fetch(`${API_BASE}/dish/list?merchant_id=${currentMerchant.id}`)
    .then(res => res.json())
    .then(data => {
        let dishes = data.code === 0 ? data.data.list : [];
        
        if (filterType === 1) {
            dishes.sort((a, b) => b.sold_count - a.sold_count);
        } else if (filterType === 2) {
            dishes.sort((a, b) => new Date(b.create_time) - new Date(a.create_time));
        }
        
        renderMerchantDishes(dishes);
    });
}

function renderMerchantDishes(dishes) {
    const dishList = document.getElementById('modal-dish-list');
    dishList.innerHTML = '';
    
    if (dishes.length === 0) {
        dishList.innerHTML = '<div class="empty-state"><p>暂无菜品</p></div>';
        return;
    }
    
    dishes.forEach(dish => {
        const item = document.createElement('div');
        item.className = 'dish-item';
        
        const cartItem = cart.find(c => c.dish_id === dish.id && c.merchant_id === currentMerchant.id);
        const qty = cartItem ? cartItem.num : 0;
        
        item.innerHTML = `
            <div class="dish-info">
                <div class="dish-name">${dish.dish_name}</div>
                <div class="dish-desc">${dish.description}</div>
                <div class="dish-price-row">
                    <span class="dish-price">¥${dish.price}</span>
                    ${dish.original_price > dish.price ? `<span class="dish-original-price">¥${dish.original_price}</span>` : ''}
                </div>
                <div class="dish-stats">已售 ${dish.sold_count} | 库存 ${dish.stock}</div>
            </div>
            <div class="dish-actions">
                ${qty > 0 ? `
                    <div class="qty-control">
                        <button class="qty-btn" onclick="event.stopPropagation(); updateCartQty(${dish.id}, ${currentMerchant.id}, -1)">-</button>
                        <span class="qty-display">${qty}</span>
                        <button class="qty-btn" onclick="event.stopPropagation(); updateCartQty(${dish.id}, ${currentMerchant.id}, 1)">+</button>
                    </div>
                ` : `
                    <button class="add-btn" onclick="event.stopPropagation(); addToCart(${dish.id}, '${dish.dish_name}', ${dish.price}, ${currentMerchant.id})">+ 加入购物车</button>
                `}
            </div>
        `;
        
        dishList.appendChild(item);
    });
}

function addToCart(dishId, dishName, price, merchantId) {
    const existing = cart.find(item => item.dish_id === dishId && item.merchant_id === merchantId);
    if (existing) {
        existing.num++;
    } else {
        cart.push({ dish_id: dishId, dish_name: dishName, dish_price: price, num: 1, merchant_id: merchantId });
    }
    updateCart();
    showToast('已添加到购物车');
    
    if (currentMerchant) {
        loadMerchantDishes(currentMerchant.id);
    }
}

function updateCartQty(dishId, merchantId, delta) {
    const item = cart.find(c => c.dish_id === dishId && c.merchant_id === merchantId);
    if (!item) return;
    
    item.num += delta;
    if (item.num <= 0) {
        cart = cart.filter(c => !(c.dish_id === dishId && c.merchant_id === merchantId));
    }
    updateCart();
    loadMerchantDishes(merchantId);
}

function updateCart() {
    const cartBar = document.getElementById('cart-bar');
    const count = cart.reduce((sum, item) => sum + item.num, 0);
    const total = cart.reduce((sum, item) => sum + item.dish_price * item.num, 0);
    
    document.getElementById('cart-count').textContent = count;
    document.getElementById('cart-total').textContent = `¥${total.toFixed(2)}`;
    
    if (currentMerchant) {
        const diff = currentMerchant.start_price - total;
        if (diff > 0) {
            document.getElementById('cart-hint').textContent = `还差¥${diff.toFixed(2)}起送`;
        } else {
            document.getElementById('cart-hint').textContent = '已达到起送价';
        }
    }
    
    if (count > 0) {
        cartBar.classList.remove('hidden');
    } else {
        cartBar.classList.add('hidden');
    }
}

function checkout() {
    if (!currentUser) {
        showToast('请先登录');
        showLogin();
        return;
    }
    
    const modal = document.getElementById('checkout-modal');
    const itemsDiv = document.getElementById('checkout-items');
    itemsDiv.innerHTML = '';
    
    let totalPrice = 0;
    cart.forEach(item => {
        totalPrice += item.dish_price * item.num;
        const div = document.createElement('div');
        div.className = 'checkout-item';
        div.innerHTML = `
            <span>${item.dish_name} x ${item.num}</span>
            <span>¥${(item.dish_price * item.num).toFixed(2)}</span>
        `;
        itemsDiv.appendChild(div);
    });
    
    const deliveryFee = currentMerchant ? currentMerchant.delivery_fee : 3.0;
    const payPrice = totalPrice + deliveryFee;
    
    document.getElementById('checkout-total-price').textContent = `¥${totalPrice.toFixed(2)}`;
    document.getElementById('checkout-delivery-fee').textContent = `¥${deliveryFee.toFixed(2)}`;
    document.getElementById('checkout-pay-price').textContent = `¥${payPrice.toFixed(2)}`;
    
    modal.classList.remove('hidden');
}

function closeCheckout() {
    document.getElementById('checkout-modal').classList.add('hidden');
}

function submitOrder() {
    const name = document.getElementById('checkout-name').value;
    const phone = document.getElementById('checkout-phone').value;
    const addr = document.getElementById('checkout-addr').value;
    
    if (!name || !phone || !addr) {
        showToast('请填写完整的收货信息');
        return;
    }
    
    const merchantId = currentMerchant.id;
    let totalPrice = 0;
    let body = `user_id=${currentUser.id}&merchant_id=${merchantId}&receiver_name=${encodeURIComponent(name)}&receiver_phone=${encodeURIComponent(phone)}&receiver_addr=${encodeURIComponent(addr)}&item_count=${cart.length}`;
    
    cart.forEach((item, index) => {
        totalPrice += item.dish_price * item.num;
        body += `&dish_id_${index}=${item.dish_id}&dish_name_${index}=${encodeURIComponent(item.dish_name)}&dish_price_${index}=${item.dish_price}&num_${index}=${item.num}`;
    });
    
    const deliveryFee = currentMerchant.delivery_fee;
    const payType = document.querySelector('input[name="pay_type"]:checked')?.value || 1;
    body += `&total_price=${totalPrice}&pay_price=${totalPrice + deliveryFee}&delivery_fee=${deliveryFee}&pay_type=${payType}`;
    
    fetch(`${API_BASE}/order/create`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: body
    })
    .then(res => res.json())
    .then(data => {
        if (data.code === 0) {
            showToast('订单创建成功！');
            cart = [];
            updateCart();
            closeCheckout();
            closeMerchantModal();
            showPage('orders');
        } else {
            showToast(data.message);
        }
    })
    .catch(err => showToast('创建订单失败'));
}

function loadOrders() {
    if (!currentUser) {
        document.getElementById('order-list').innerHTML = `
            <div class="empty-state">
                <p>请先登录查看订单</p>
            </div>
        `;
        return;
    }
    
    fetch(`${API_BASE}/order/list?user_id=${currentUser.id}`)
    .then(res => res.json())
    .then(data => {
        currentOrders = data.code === 0 ? data.data.list : [];
        renderOrders();
    });
}

function renderOrders() {
    const orderList = document.getElementById('order-list');
    orderList.innerHTML = '';
    
    let filteredOrders = currentOrders;
    if (currentOrderFilter === 'pending') {
        filteredOrders = currentOrders.filter(o => o.order_status === 0);
    } else if (currentOrderFilter === 'paid') {
        filteredOrders = currentOrders.filter(o => o.order_status >= 1 && o.order_status <= 3);
    } else if (currentOrderFilter === 'completed') {
        filteredOrders = currentOrders.filter(o => o.order_status === 4);
    }
    
    if (filteredOrders.length === 0) {
        orderList.innerHTML = `
            <div class="empty-state">
                <p>暂无订单</p>
            </div>
        `;
        return;
    }
    
    filteredOrders.forEach(order => {
        const card = document.createElement('div');
        card.className = 'order-card';
        card.onclick = () => showOrderDetail(order.order_no);
        
        const statusMap = {
            0: { text: '待支付', class: 'pending' },
            1: { text: '已支付', class: 'paid' },
            2: { text: '已接单', class: 'paid' },
            3: { text: '配送中', class: 'paid' },
            4: { text: '已完成', class: 'completed' },
            5: { text: '已取消', class: 'cancelled' },
            6: { text: '退款中', class: 'cancelled' }
        };
        
        const status = statusMap[order.order_status] || { text: '未知', class: '' };
        
        card.innerHTML = `
            <div class="order-header">
                <span class="order-no">订单号：${order.order_no}</span>
                <span class="order-status ${status.class}">${status.text}</span>
            </div>
            <div class="order-items">
                <div class="order-item">
                    <span class="order-item-name">订单商品</span>
                    <span class="order-item-price">¥${order.total_price.toFixed(2)}</span>
                </div>
            </div>
            <div class="order-total">实付：¥${order.pay_price.toFixed(2)}</div>
            ${order.order_status === 0 ? `
                <div class="order-actions">
                    <button onclick="event.stopPropagation(); cancelOrder('${order.order_no}')">取消订单</button>
                    <button class="btn-pay" onclick="event.stopPropagation(); payOrder('${order.order_no}')">立即支付</button>
                </div>
            ` : ''}
        `;
        
        orderList.appendChild(card);
    });
}

function showOrderDetail(orderNo) {
    fetch(`${API_BASE}/order?order_no=${orderNo}`)
    .then(res => res.json())
    .then(orderData => {
        if (orderData.code !== 0) {
            showToast('订单不存在');
            return;
        }
        
        const order = orderData.data;
        
        fetch(`${API_BASE}/order/items?order_no=${orderNo}`)
        .then(res => res.json())
        .then(itemsData => {
            const items = itemsData.code === 0 ? itemsData.data.list : [];
            
            const body = document.getElementById('order-detail-body');
            body.innerHTML = `
                <div class="order-detail-info">
                    <div class="order-detail-row"><span>订单号</span><span>${order.order_no}</span></div>
                    <div class="order-detail-row"><span>状态</span><span>${getOrderStatusText(order.order_status)}</span></div>
                    <div class="order-detail-row"><span>商家</span><span>ID: ${order.merchant_id}</span></div>
                    <div class="order-detail-row"><span>收件人</span><span>${order.receiver_name}</span></div>
                    <div class="order-detail-row"><span>手机号</span><span>${order.receiver_phone}</span></div>
                    <div class="order-detail-row"><span>地址</span><span>${order.receiver_addr}</span></div>
                    <div class="order-detail-row"><span>创建时间</span><span>${order.create_time}</span></div>
                    ${order.pay_time ? `<div class="order-detail-row"><span>支付时间</span><span>${order.pay_time}</span></div>` : ''}
                    ${order.finish_time ? `<div class="order-detail-row"><span>完成时间</span><span>${order.finish_time}</span></div>` : ''}
                </div>
                <div class="checkout-section">
                    <h4>商品清单</h4>
                    ${items.map(item => `
                        <div class="checkout-item">
                            <span>${item.dish_name} x ${item.num}</span>
                            <span>¥${(item.dish_price * item.num).toFixed(2)}</span>
                        </div>
                    `).join('')}
                </div>
                <div class="checkout-total">
                    <div class="total-row">
                        <span>商品总价</span>
                        <span>¥${order.total_price.toFixed(2)}</span>
                    </div>
                    <div class="total-row">
                        <span>配送费</span>
                        <span>¥${order.delivery_fee.toFixed(2)}</span>
                    </div>
                    <div class="total-row total-final">
                        <span>实付金额</span>
                        <span>¥${order.pay_price.toFixed(2)}</span>
                    </div>
                </div>
                ${order.order_status === 0 ? `
                    <div style="display: flex; gap: 12px;">
                        <button onclick="payOrder('${orderNo}'); closeOrderDetail();" class="btn-primary">立即支付</button>
                        <button onclick="cancelOrder('${orderNo}'); closeOrderDetail();" class="btn-secondary">取消订单</button>
                    </div>
                ` : ''}
            `;
            
            document.getElementById('order-detail-modal').classList.remove('hidden');
        });
    });
}

function closeOrderDetail() {
    document.getElementById('order-detail-modal').classList.add('hidden');
}

function getOrderStatusText(status) {
    const map = {
        0: '待支付',
        1: '已支付',
        2: '已接单',
        3: '配送中',
        4: '已完成',
        5: '已取消',
        6: '退款中'
    };
    return map[status] || '未知';
}

function payOrder(orderNo) {
    fetch(`${API_BASE}/order/pay`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: `order_no=${encodeURIComponent(orderNo)}&pay_type=1`
    })
    .then(res => res.json())
    .then(data => {
        if (data.code === 0) {
            showToast('支付成功');
            loadOrders();
        } else {
            showToast(data.message);
        }
    });
}

function cancelOrder(orderNo) {
    if (!confirm('确定要取消此订单吗？')) return;
    
    fetch(`${API_BASE}/order/cancel`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: `order_no=${encodeURIComponent(orderNo)}`
    })
    .then(res => res.json())
    .then(data => {
        if (data.code === 0) {
            showToast('订单已取消');
            loadOrders();
        } else {
            showToast(data.message);
        }
    });
}

window.onload = function() {
    loadMerchants();
    updateUserInfo();
};
