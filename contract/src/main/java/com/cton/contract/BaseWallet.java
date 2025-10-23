// BaseWallet.java - базова реалізація кошелька
// Author: Андрій Будильников (Sparky)
// Base wallet implementation
// Базовая реализация кошелька

package com.cton.contract;

import com.cton.sdk.Address;
import com.cton.sdk.Cell;
import com.cton.sdk.CellBuilder;
import com.cton.sdk.Boc;
import com.cton.api.TonApiClient;
import com.google.gson.JsonObject;
import java.math.BigInteger;
import java.io.IOException;
import java.util.Base64;

/**
 * Базова реалізація кошелька
 * 
 * Base wallet implementation
 * Базовая реализация кошелька
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
            bodyBuilder.storeUInt(32, 0); // Текст коментаря / Текст комментария / Comment text
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
        JsonObject result = apiClient.getAddressInformation(address.toRaw());
        
        // В реальній реалізації тут має бути виклик get-методу контракту
        // В реальной реализации здесь должен быть вызов get-метода контракта
        // In real implementation, contract get-method should be called here
        
        // Повертаємо 0 як значення за замовчуванням
        // Возвращаем 0 как значение по умолчанию
        // Return 0 as default value
        return 0;
    }
    
    @Override
    public int getVersion() throws IOException {
        // В реальній реалізації тут має бути отримання версії кошелька
        // В реальной реализации здесь должно быть получение версии кошелька
        // In real implementation, wallet version should be obtained here
        
        // Повертаємо 0 як значення за замовчуванням
        // Возвращаем 0 как значение по умолчанию
        // Return 0 as default value
        return 0;
    }
}