// Subscription.java - реалізація контракту підписки
// Author: Андрій Будильников (Sparky)
// Subscription contract implementation
// Реализация контракта подписки

package com.cton.contract;

import java.io.IOException;
import java.math.BigInteger;
import java.time.Instant;

import com.cton.api.TonApiClient;
import com.cton.sdk.Address;
import com.cton.sdk.Cell;
import com.cton.sdk.CellBuilder;

/**
 * Реалізація контракту підписки TON
 * 
 * Subscription contract implementation
 * Реализация контракта подписки TON
 */
public class Subscription extends Contract {
    
    /**
     * Конструктор
     * @param address адреса контракту
     * @param apiClient API клієнт для взаємодії з мережею
     */
    public Subscription(Address address, TonApiClient apiClient) {
        super(address, apiClient);
    }
    
    /**
     * Створити повідомлення для підписки
     * @param beneficiary адреса бенефіціара
     * @param amount сума в нанотоні
     * @param period період у секундах
     * @param timeout таймаут у секундах
     * @param comment коментар
     * @return комірка з повідомленням підписки
     * @throws IOException якщо сталася помилка
     */
    public Cell createSubscriptionMessage(Address beneficiary, BigInteger amount, 
                                        long period, long timeout, String comment) throws IOException {
        CellBuilder builder = new CellBuilder();
        
        // Додаємо адресу бенефіціара
        builder.storeAddress(beneficiary);
        
        // Додаємо суму
        builder.storeCoins(amount);
        
        // Додаємо період
        builder.storeUInt(32, period);
        
        // Додаємо таймаут
        builder.storeUInt(32, timeout);
        
        // Додаємо час початку (поточний час)
        long startTime = Instant.now().getEpochSecond();
        builder.storeUInt(32, startTime);
        
        // Додаємо коментар якщо він є
        if (comment != null && !comment.isEmpty()) {
            CellBuilder commentBuilder = new CellBuilder();
            commentBuilder.storeBytes(comment.getBytes());
            builder.storeRef(commentBuilder.build());
        }
        
        return builder.build();
    }
    
    /**
     * Створити повідомлення для скасування підписки
     * @param subscriptionId ідентифікатор підписки
     * @return комірка з повідомленням скасування
     * @throws IOException якщо сталася помилка
     */
    public Cell createCancelSubscriptionMessage(long subscriptionId) throws IOException {
        CellBuilder builder = new CellBuilder();
        
        // Додаємо код операції для скасування (1)
        builder.storeUInt(32, 1);
        
        // Додаємо ідентифікатор підписки
        builder.storeUInt(64, subscriptionId);
        
        return builder.build();
    }
    
    /**
     * Отримати інформацію про підписку
     * @param subscriptionId ідентифікатор підписки
     * @return інформація про підписку
     * @throws IOException якщо сталася помилка мережі
     */
    public SubscriptionInfo getSubscriptionInfo(long subscriptionId) throws IOException {
        // Виконуємо get-метод get_subscription
        com.google.gson.JsonObject stack = new com.google.gson.JsonObject();
        stack.addProperty("subscription_id", String.valueOf(subscriptionId));
        
        com.google.gson.JsonObject result = runGetMethod("get_subscription", stack);
        
        if (!result.has("result") || result.get("result").isJsonNull()) {
            throw new IOException("Failed to get subscription info");
        }
        
        // Обробляємо результат (спрощена реалізація)
        // In a real implementation, we would parse the actual stack data
        return new SubscriptionInfo(subscriptionId, BigInteger.ZERO, 0, 0, 0, false);
    }
    
    /**
     * Отримати загальну кількість підписок
     * @return кількість підписок
     * @throws IOException якщо сталася помилка мережі
     */
    public long getTotalSubscriptions() throws IOException {
        // Виконуємо get-метод get_total_subscriptions
        com.google.gson.JsonObject stack = new com.google.gson.JsonObject();
        com.google.gson.JsonObject result = runGetMethod("get_total_subscriptions", stack);
        
        if (!result.has("result") || result.get("result").isJsonNull()) {
            throw new IOException("Failed to get total subscriptions");
        }
        
        // Обробляємо результат (спрощена реалізація)
        return 0;
    }
    
    /**
     * Клас для інформації про підписку
     */
    public static class SubscriptionInfo {
        private final long id;
        private final BigInteger amount;
        private final long period;
        private final long timeout;
        private final long lastPayment;
        private final boolean active;
        
        public SubscriptionInfo(long id, BigInteger amount, long period, 
                              long timeout, long lastPayment, boolean active) {
            this.id = id;
            this.amount = amount;
            this.period = period;
            this.timeout = timeout;
            this.lastPayment = lastPayment;
            this.active = active;
        }
        
        // Геттери
        public long getId() { return id; }
        public BigInteger getAmount() { return amount; }
        public long getPeriod() { return period; }
        public long getTimeout() { return timeout; }
        public long getLastPayment() { return lastPayment; }
        public boolean isActive() { return active; }
    }
}