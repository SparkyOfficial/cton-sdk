// Nft.java - інтерфейс для роботи з NFT токенами
// Author: Андрій Будильников (Sparky)
// Interface for working with NFT tokens
// Интерфейс для работы с NFT токенами

package com.cton.contract;

import com.cton.sdk.Address;
import com.cton.sdk.Cell;
import com.cton.api.TonApiClient;
import com.google.gson.JsonObject;
import java.io.IOException;

/**
 * Інтерфейс для роботи з NFT токенами відповідно до TEP-62
 * 
 * Interface for working with NFT tokens according to TEP-62
 * Интерфейс для работы с NFT токенами согласно TEP-62
 */
public class Nft extends Contract {
    
    /**
     * Конструктор
     * @param address адреса NFT контракту
     * @param apiClient API клієнт для взаємодії з мережею
     */
    public Nft(Address address, TonApiClient apiClient) {
        super(address, apiClient);
    }
    
    /**
     * Отримати інформацію про NFT
     * @return інформація про NFT
     * @throws IOException якщо сталася помилка мережі
     */
    public JsonObject getNftData() throws IOException {
        JsonObject stack = new JsonObject();
        return runGetMethod("get_nft_data", stack);
    }
    
    /**
     * Отримати індекс NFT
     * @return індекс NFT
     * @throws IOException якщо сталася помилка мережі
     */
    public long getIndex() throws IOException {
        JsonObject data = getNftData();
        
        // В реальній реалізації тут має бути парсинг результату
        // В реальной реализации здесь должен быть парсинг результата
        // In real implementation, result should be parsed here
        
        // Для демонстрації просто повертаємо 0
        // Для демонстрации просто возвращаем 0
        // For demonstration just return 0
        return 0;
    }
    
    /**
     * Отримати адресу колекції
     * @return адреса колекції
     * @throws IOException якщо сталася помилка мережі
     */
    public Address getCollection() throws IOException {
        JsonObject data = getNftData();
        
        // В реальній реалізації тут має бути парсинг результату
        // В реальной реализации здесь должен быть парсинг результата
        // In real implementation, result should be parsed here
        
        // Для демонстрації просто повертаємо адресу контракту
        // Для демонстрации просто возвращаем адрес контракта
        // For demonstration just return contract address
        return getAddress();
    }
    
    /**
     * Отримати власника NFT
     * @return адреса власника
     * @throws IOException якщо сталася помилка мережі
     */
    public Address getOwner() throws IOException {
        JsonObject data = getNftData();
        
        // В реальній реалізації тут має бути парсинг результату
        // В реальной реализации здесь должен быть парсинг результата
        // In real implementation, result should be parsed here
        
        // Для демонстрації просто повертаємо адресу контракту
        // Для демонстрации просто возвращаем адрес контракта
        // For demonstration just return contract address
        return getAddress();
    }
    
    /**
     * Отримати контент NFT
     * @return контент NFT у вигляді комірки
     * @throws IOException якщо сталася помилка мережі
     */
    public Cell getContent() throws IOException {
        JsonObject data = getNftData();
        
        // В реальній реалізації тут має бути парсинг результату
        // В реальной реализации здесь должен быть парсинг результата
        // In real implementation, result should be parsed here
        
        // Для демонстрації просто повертаємо порожню комірку
        // Для демонстрации просто возвращаем пустую ячейку
        // For demonstration just return empty cell
        return new com.cton.sdk.CellBuilder().build();
    }
}