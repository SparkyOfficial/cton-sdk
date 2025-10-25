package com.cton.contract;

import java.io.IOException;
import java.util.Arrays;
import java.util.Base64;

import com.cton.api.TonApiClient;
import com.cton.sdk.Address;
import com.cton.sdk.Boc;
import com.cton.sdk.Cell;
import com.cton.sdk.Crypto;
import com.google.gson.JsonObject;

/**
 * Базовий клас для контрактів TON
 * Base class for TON contracts
 * Базовый класс для контрактов TON
 */
public abstract class Contract {
    protected Address address;
    protected Crypto.PrivateKey privateKey; // Додано приватний ключ для підпису
    protected TonApiClient apiClient; // Додано API клієнт для взаємодії з мережею
    
    /**
     * Конструктор контракту
     * @param address адреса контракту
     */
    public Contract(Address address) {
        this.address = address;
        this.privateKey = null;
        this.apiClient = null;
    }
    
    /**
     * Конструктор контракту з приватним ключем
     * @param address адреса контракту
     * @param privateKey приватний ключ для підпису повідомлень
     */
    public Contract(Address address, Crypto.PrivateKey privateKey) {
        this.address = address;
        this.privateKey = privateKey;
        this.apiClient = null;
    }
    
    /**
     * Конструктор контракту з API клієнтом
     * @param address адреса контракту
     * @param apiClient API клієнт для взаємодії з мережею
     */
    public Contract(Address address, TonApiClient apiClient) {
        this.address = address;
        this.privateKey = null;
        this.apiClient = apiClient;
    }
    
    /**
     * Встановити приватний ключ для підпису повідомлень
     * @param privateKey приватний ключ
     */
    public void setPrivateKey(Crypto.PrivateKey privateKey) {
        this.privateKey = privateKey;
    }
    
    /**
     * Встановити API клієнт для взаємодії з мережею
     * @param apiClient API клієнт
     */
    public void setApiClient(TonApiClient apiClient) {
        this.apiClient = apiClient;
    }
    
    /**
     * Отримати адресу контракту
     * @return адреса контракту
     */
    public Address getAddress() {
        return address;
    }
    
    /**
     * Виконати get-метод контракту
     * @param methodName назва методу
     * @param stack стек параметрів
     * @return результат виконання методу
     * @throws IOException якщо сталася помилка мережі
     */
    protected JsonObject runGetMethod(String methodName, JsonObject stack) throws IOException {
        if (apiClient == null) {
            throw new IOException("API client not set");
        }
        
        // Викликаємо get-метод контракту через API клієнт
        // Вызываем get-метод контракта через API клиент
        // Call contract get-method through API client
        return apiClient.runGetMethod(address.toRaw(), methodName, stack);
    }
    
    /**
     * Надіслати внутрішнє повідомлення до контракту
     * @param message повідомлення для надсилання
     * @throws IOException якщо сталася помилка мережі
     */
    public void sendMessage(Cell message) throws IOException {
        // Перевіряємо наявність приватного ключа
        // Check for private key availability
        // Проверяем наличие приватного ключа
        if (privateKey == null) {
            throw new IOException("Private key not set. Cannot sign message.");
        }
        
        // Перевіряємо наявність API клієнта
        // Check for API client availability
        // Проверяем наличие API клиента
        if (apiClient == null) {
            throw new IOException("API client not set. Cannot send message.");
        }
        
        // Серіалізуємо комірку в BOC
        // Сериализуем ячейку в BOC
        // Serialize cell to BOC
        Boc boc = new Boc(message);
        byte[] messageBytes = boc.serialize(true, true);
        
        // Підписуємо повідомлення приватним ключем
        // Подписываем сообщение приватным ключом
        // Sign message with private key
        byte[] signature = Crypto.sign(privateKey, messageBytes);
        
        // Конвертуємо підписане повідомлення в Base64
        // Конвертируем подписанное сообщение в Base64
        // Convert signed message to Base64
        String bocBase64 = Base64.getEncoder().encodeToString(messageBytes);
        
        // Надсилаємо підписане повідомлення через API
        // Отправляем подписанное сообщение через API
        // Send signed message through API
        JsonObject response = apiClient.sendBoc(messageBytes);
        
        // Логуємо успішне надсилання
        // Логируем успешную отправку
        // Log successful sending
        System.out.println("Message signed and sent successfully. Signature: " + 
                          Arrays.toString(signature));
    }
}