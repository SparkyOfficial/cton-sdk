// Contract.java - базовий клас для контрактів
// Author: Андрій Будильников (Sparky)
// Base class for contracts
// Базовый класс для контрактов

package com.cton.contract;

import com.cton.sdk.Address;
import com.cton.sdk.Cell;
import com.cton.api.TonApiClient;
import com.google.gson.JsonObject;
import java.io.IOException;

/**
 * Базовий клас для взаємодії зі смарт-контрактами
 * 
 * Base class for interacting with smart contracts
 * Базовый класс для взаимодействия со смарт-контрактами
 */
public abstract class Contract {
    protected final Address address;
    protected final TonApiClient apiClient;
    
    /**
     * Конструктор
     * @param address адреса контракту
     * @param apiClient API клієнт для взаємодії з мережею
     */
    public Contract(Address address, TonApiClient apiClient) {
        this.address = address;
        this.apiClient = apiClient;
    }
    
    /**
     * Отримати адресу контракту
     * @return адреса контракту
     */
    public Address getAddress() {
        return this.address;
    }
    
    /**
     * Виконати get-метод контракту
     * @param method назва методу
     * @param stack параметри методу
     * @return результат виконання методу
     * @throws IOException якщо сталася помилка мережі
     */
    public JsonObject runGetMethod(String method, JsonObject stack) throws IOException {
        return apiClient.runGetMethod(address.toRaw(), method, stack);
    }
    
    /**
     * Виконати get-метод контракту асинхронно
     * @param method назва методу
     * @param stack параметри методу
     * @return CompletableFuture з результатом виконання методу
     */
    public java.util.concurrent.CompletableFuture<JsonObject> runGetMethodAsync(String method, JsonObject stack) {
        return apiClient.runGetMethodAsync(address.toRaw(), method, stack);
    }
    
    /**
     * Надіслати внутрішнє повідомлення до контракту
     * @param message повідомлення для надсилання
     * @throws IOException якщо сталася помилка мережі
     */
    public void sendMessage(Cell message) throws IOException {
        // В реальній реалізації тут має бути підпис повідомлення приватним ключем
        // В реальной реализации здесь должна быть подпись сообщения приватным ключом
        // In real implementation, message should be signed with private key here
        
        // Для демонстрації просто викидаємо виняток
        // Для демонстрации просто выбрасываем исключение
        // For demonstration just throw exception
        throw new IOException("Message signing not implemented. Use wallet to send transactions.");
    }
}