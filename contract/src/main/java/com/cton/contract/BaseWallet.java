// BaseWallet.java - базова реалізація кошелька
// Author: Андрій Будильников (Sparky)
// Base wallet implementation
// Базовая реализация кошелька

package com.cton.contract;

import java.io.IOException;
import java.math.BigInteger;
import java.util.Base64;

import com.cton.api.TonApiClient;
import com.cton.sdk.Address;
import com.cton.sdk.Boc;
import com.cton.sdk.Cell;
import com.cton.sdk.CellBuilder;
import com.cton.sdk.Crypto;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

/**
 * Базова реалізація кошелька для TON блокчейну
 * 
 * Цей клас надає основну функціональність для роботи з TON кошельками:
 * - Отримання балансу адреси
 * - Створення транзакцій
 * - Відправка повідомлень
 * - Робота з get-методами смарт-контрактів
 * 
 * Base wallet implementation for TON blockchain
 * 
 * This class provides core functionality for working with TON wallets:
 * - Getting address balance
 * - Creating transactions
 * - Sending messages
 * - Working with smart contract get-methods
 * 
 * Базовая реализация кошелька для TON блокчейна
 * 
 * Этот класс предоставляет основной функционал для работы с TON кошельками:
 * - Получение баланса адреса
 * - Создание транзакций
 * - Отправка сообщений
 * - Работа с get-методами смарт-контрактов
 */
public abstract class BaseWallet implements Wallet {
    protected final Address address;
    protected final TonApiClient apiClient;
    
    /**
     * Конструктор
     * @param address адреса кошелька
     * @param apiClient API клієнт для взаємодії з мережею
     */
    public BaseWallet(Address address, TonApiClient apiClient) {
        this.address = address;
        this.apiClient = apiClient;
    }
    
    @Override
    public Address getAddress() {
        return this.address;
    }
    
    @Override
    public BigInteger getBalance() throws IOException {
        // Отримуємо інформацію про адресу через API
        // Получаем информацию об адресе через API
        // Get address information through API
        JsonObject addressInfo = apiClient.getAddressInformation(address.toRaw());
        
        // Перевіряємо чи запит був успішним
        // Проверяем, был ли запрос успешным
        // Check if request was successful
        if (!addressInfo.has("result") || addressInfo.get("result").isJsonNull()) {
            throw new IOException("Failed to get address information");
        }
        
        JsonObject result = addressInfo.getAsJsonObject("result");
        
        // Отримуємо баланс
        // Получаем баланс
        // Get balance
        if (result.has("balance")) {
            return new BigInteger(result.get("balance").getAsString());
        } else {
            throw new IOException("Balance not found in response");
        }
    }
    
    @Override
    public Cell createTransfer(Address destination, BigInteger amount, String comment) throws IOException {
        // Створюємо комірку з тілом повідомлення
        // Создаем ячейку с телом сообщения
        // Create cell with message body
        CellBuilder bodyBuilder = new CellBuilder();
        
        // Додаємо код операції (0 для простого переказу)
        // Добавляем код операции (0 для простого перевода)
        // Add operation code (0 for simple transfer)
        bodyBuilder.storeUInt(32, 0);
        
        // Додаємо коментар якщо він є
        // Добавляем комментарий если он есть
        // Add comment if it exists
        if (comment != null && !comment.isEmpty()) {
            // Додаємо текст коментаря
            // Добавляем текст комментария
            // Add comment text
            bodyBuilder.storeBytes(comment.getBytes());
        }
        
        Cell body = bodyBuilder.build();
        
        // Створюємо зовнішнє повідомлення
        // Создаем внешнее сообщение
        // Create external message
        CellBuilder messageBuilder = new CellBuilder();
        
        // Додаємо адресу одержувача
        // Добавляем адрес получателя
        // Add recipient address
        messageBuilder.storeAddress(destination);
        
        // Додаємо суму
        // Добавляем сумму
        // Add amount
        messageBuilder.storeCoins(amount);
        
        // Додаємо тіло повідомлення
        // Добавляем тело сообщения
        // Add message body
        messageBuilder.storeRef(body);
        
        return messageBuilder.build();
    }
    
    @Override
    public void sendTransaction(Cell transaction) throws IOException {
        // Серіалізуємо транзакцію в BOC
        // Сериализуем транзакцию в BOC
        // Serialize transaction to BOC
        Boc boc = new Boc(transaction);
        byte[] bocBytes = boc.serialize(true, true);
        
        // Конвертуємо в Base64
        // Конвертируем в Base64
        // Convert to Base64
        String bocBase64 = Base64.getEncoder().encodeToString(bocBytes);
        
        // Надсилаємо через API
        // Отправляем через API
        // Send through API
        apiClient.sendBoc(bocBytes);
    }
    
    @Override
    public long getSeqno() throws IOException {
        // Виконуємо get-метод seqno
        // Выполняем get-метод seqno
        // Execute seqno get-method
        JsonObject stack = new JsonObject();
        JsonObject result = runGetMethod("seqno", stack);
        
        // Перевіряємо чи є результат
        // Проверяем, есть ли результат
        // Check if there is a result
        if (!result.has("result") || result.get("result").isJsonNull()) {
            throw new IOException("Failed to get seqno");
        }
        
        // Отримуємо стек результатів
        // Получаем стек результатов
        // Get result stack
        JsonArray stackArray = result.getAsJsonArray("result");
        if (stackArray.size() < 1) {
            throw new IOException("Invalid seqno format");
        }
        
        // Seqno - перший елемент стеку
        // Seqno - first element of stack
        // Seqno - первый элемент стека
        JsonElement seqnoElement = stackArray.get(0);
        if (!seqnoElement.isJsonArray()) {
            throw new IOException("Invalid seqno data format");
        }
        
        JsonArray seqnoArray = seqnoElement.getAsJsonArray();
        if (seqnoArray.size() < 2) {
            throw new IOException("Invalid seqno array format");
        }
        
        // Другий елемент містить значення
        // Second element contains the value
        // Второй элемент содержит значение
        String seqnoStr = seqnoArray.get(1).getAsString();
        return Long.parseLong(seqnoStr);
    }
    
    @Override
    public int getVersion() throws IOException {
        // Виконуємо get-метод get_version
        // Выполняем get-метод get_version
        // Execute get_version get-method
        JsonObject stack = new JsonObject();
        JsonObject result = runGetMethod("get_version", stack);
        
        // Перевіряємо чи є результат
        // Проверяем, есть ли результат
        // Check if there is a result
        if (!result.has("result") || result.get("result").isJsonNull()) {
            throw new IOException("Failed to get version");
        }
        
        // Отримуємо стек результатів
        // Получаем стек результатов
        // Get result stack
        JsonArray stackArray = result.getAsJsonArray("result");
        if (stackArray.size() < 1) {
            throw new IOException("Invalid version format");
        }
        
        // Версія - перший елемент стеку
        // Version - first element of stack
        // Версия - первый элемент стека
        JsonElement versionElement = stackArray.get(0);
        if (!versionElement.isJsonArray()) {
            throw new IOException("Invalid version data format");
        }
        
        JsonArray versionArray = versionElement.getAsJsonArray();
        if (versionArray.size() < 2) {
            throw new IOException("Invalid version array format");
        }
        
        // Другий елемент містить значення
        // Second element contains the value
        // Второй элемент содержит значение
        String versionStr = versionArray.get(1).getAsString();
        return Integer.parseInt(versionStr);
    }
    
    /**
     * Виконати get-метод смарт-контракту
     * @param method назва методу
     * @param stack параметри методу
     * @return результат виконання методу
     * @throws IOException якщо сталася помилка мережі
     */
    protected JsonObject runGetMethod(String method, JsonObject stack) throws IOException {
        return apiClient.runGetMethod(address.toRaw(), method, stack);
    }
    
    /**
     * Transfer funds to another address
     * @param destination адреса одержувача
     * @param amount сума для переказу
     * @param comment коментар до переказу
     * @param privateKey приватний ключ для підпису
     * @throws IOException якщо сталася помилка мережі
     */
    public void transfer(Address destination, BigInteger amount, String comment, Crypto.PrivateKey privateKey) throws IOException {
        // Get seqno
        long seqno = getSeqno();
        
        // Create transfer message
        Cell message = createTransfer(destination, amount, comment);
        
        // In a real implementation, you would:
        // 1. Create a proper wallet message with seqno
        // 2. Sign it with the private key
        // 3. Wrap it in an external message
        // 4. Send it via the API
        
        // For now, just send the basic message
        sendTransaction(message);
    }
}