// NftCollection.java - інтерфейс для роботи з NFT колекціями
// Author: Андрій Будильников (Sparky)
// Interface for working with NFT collections
// Интерфейс для работы с NFT коллекциями

package com.cton.contract;

import java.io.IOException;
import java.util.List;

import com.cton.api.TonApiClient;
import com.cton.sdk.Address;
import com.cton.sdk.Cell;
import com.cton.sdk.CellBuilder;
import com.cton.sdk.Crypto;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

/**
 * Інтерфейс для роботи з NFT колекціями відповідно до TEP-62
 * 
 * Interface for working with NFT collections according to TEP-62
 * Интерфейс для работы с NFT коллекциями согласно TEP-62
 */
public class NftCollection extends Contract {
    
    /**
     * Конструктор
     * @param address адреса контракту колекції
     * @param apiClient API клієнт для взаємодії з мережею
     */
    public NftCollection(Address address, TonApiClient apiClient) {
        super(address, apiClient);
    }
    
    /**
     * Отримати інформацію про колекцію
     * @return інформація про колекцію
     * @throws IOException якщо сталася помилка мережі
     */
    public JsonObject getCollectionData() throws IOException {
        JsonArray stack = new JsonArray();
        return runGetMethod("get_collection_data", stack);
    }
    
    /**
     * Отримати загальну кількість NFT в колекції
     * @return загальна кількість NFT
     * @throws IOException якщо сталася помилка мережі
     */
    public long getNextItemIndex() throws IOException {
        JsonObject data = getCollectionData();
        
        // Перевіряємо чи є результат
        // Проверяем, есть ли результат
        // Check if there is a result
        if (!data.has("result") || data.get("result").isJsonNull()) {
            throw new IOException("Failed to get collection data");
        }
        
        // Отримуємо стек результатів
        // Получаем стек результатов
        // Get result stack
        JsonArray stackArray = data.getAsJsonArray("result");
        if (stackArray.size() < 1) {
            throw new IOException("Invalid collection data format");
        }
        
        // Індекс - перший елемент стеку
        // Index - first element of stack
        // Индекс - первый элемент стека
        JsonElement indexElement = stackArray.get(0);
        if (!indexElement.isJsonArray()) {
            throw new IOException("Invalid index data format");
        }
        
        JsonArray indexArray = indexElement.getAsJsonArray();
        if (indexArray.size() < 2) {
            throw new IOException("Invalid index array format");
        }
        
        // Другий елемент містить значення
        // Second element contains the value
        // Второй элемент содержит значение
        String indexStr = indexArray.get(1).getAsString();
        return Long.parseLong(indexStr);
    }
    
    /**
     * Отримати адресу minter контракту
     * @return адреса minter контракту
     * @throws IOException якщо сталася помилка мережі
     */
    public Address getOwner() throws IOException {
        JsonObject data = getCollectionData();
        
        // Перевіряємо чи є результат
        // Проверяем, есть ли результат
        // Check if there is a result
        if (!data.has("result") || data.get("result").isJsonNull()) {
            throw new IOException("Failed to get collection data");
        }
        
        // Отримуємо стек результатів
        // Получаем стек результатов
        // Get result stack
        JsonArray stackArray = data.getAsJsonArray("result");
        if (stackArray.size() < 2) {
            throw new IOException("Invalid collection data format");
        }
        
        // Адреса власника - другий елемент стеку
        // Owner address - second element of stack
        // Адрес владельца - второй элемент стека
        JsonElement ownerElement = stackArray.get(1);
        if (!ownerElement.isJsonArray()) {
            throw new IOException("Invalid owner data format");
        }
        
        JsonArray ownerArray = ownerElement.getAsJsonArray();
        if (ownerArray.size() < 2) {
            throw new IOException("Invalid owner array format");
        }
        
        // Другий елемент містить адресу
        // Second element contains the address
        // Второй элемент содержит адрес
        String addressStr = ownerArray.get(1).getAsString();
        return new Address(addressStr);
    }
    
    /**
     * Отримати контент колекції
     * @return контент колекції у вигляді комірки
     * @throws IOException якщо сталася помилка мережі
     */
    public Cell getContent() throws IOException {
        JsonObject data = getCollectionData();
        
        // Перевіряємо чи є результат
        // Проверяем, есть ли результат
        // Check if there is a result
        if (!data.has("result") || data.get("result").isJsonNull()) {
            throw new IOException("Failed to get collection data");
        }
        
        // Отримуємо стек результатів
        // Получаем стек результатов
        // Get result stack
        JsonArray stackArray = data.getAsJsonArray("result");
        if (stackArray.size() < 3) {
            throw new IOException("Invalid collection data format");
        }
        
        // Контент - третій елемент стеку
        // Content - third element of stack
        // Контент - третий элемент стека
        JsonElement contentElement = stackArray.get(2);
        if (!contentElement.isJsonArray()) {
            throw new IOException("Invalid content data format");
        }
        
        JsonArray contentArray = contentElement.getAsJsonArray();
        if (contentArray.size() < 2) {
            throw new IOException("Invalid content array format");
        }
        
        // Другий елемент містить дані контенту
        // Second element contains content data
        // Второй элемент содержит данные контента
        String contentStr = contentArray.get(1).getAsString();
        
        // Для простоти, повертаємо порожню комірку
        // Для простоты, возвращаем пустую ячейку
        // For simplicity, return empty cell
        return new com.cton.sdk.CellBuilder().build();
    }
    
    /**
     * Отримати адресу NFT за індексом
     * @param index індекс NFT
     * @return адреса NFT
     * @throws IOException якщо сталася помилка мережі
     */
    public Address getNftAddressByIndex(long index) throws IOException {
        JsonArray stack = new JsonArray();
        JsonArray indexItem = new JsonArray();
        indexItem.add("num");
        indexItem.add(String.valueOf(index));
        stack.add(indexItem);
        
        JsonObject result = runGetMethod("get_nft_address_by_index", stack);
        
        // Перевіряємо чи є результат
        // Проверяем, есть ли результат
        // Check if there is a result
        if (!result.has("result") || result.get("result").isJsonNull()) {
            throw new IOException("Failed to get NFT address");
        }
        
        // Отримуємо стек результатів
        // Получаем стек результатов
        // Get result stack
        JsonArray stackArray = result.getAsJsonArray("result");
        if (stackArray.size() < 1) {
            throw new IOException("Invalid NFT address format");
        }
        
        // Адреса NFT - перший елемент стеку
        // NFT address - first element of stack
        // Адрес NFT - первый элемент стека
        JsonElement nftElement = stackArray.get(0);
        if (!nftElement.isJsonArray()) {
            throw new IOException("Invalid NFT address data format");
        }
        
        JsonArray nftArray = nftElement.getAsJsonArray();
        if (nftArray.size() < 2) {
            throw new IOException("Invalid NFT address array format");
        }
        
        // Другий елемент містить адресу
        // Second element contains the address
        // Второй элемент содержит адрес
        String addressStr = nftArray.get(1).getAsString();
        return new Address(addressStr);
    }
    
    /**
     * Масове створення NFT. Кожен мінт — окреме внутрішнє повідомлення з гаманця мінтера.
     *
     * @param minterWallet гаманець V3/V4 власника колекції, який сплачує газ
     * @param owner адреса власника нових NFT
     * @param contents список контенту для кожного NFT
     * @param privateKey ключ гаманця мінтера
     */
    public void batchMint(Wallet minterWallet, Address owner, List<Cell> contents, Crypto.PrivateKey privateKey) throws IOException {
        long startIndex = getNextItemIndex();
        for (int i = 0; i < contents.size(); i++) {
            mintNft(minterWallet, owner, contents.get(i), startIndex + i, privateKey);
        }
    }
    
    /**
     * Мінт одного NFT. Колекція не приймає external-in: опкод 0x178d4519
     * надсилається внутрішнім повідомленням через гаманець мінтера.
     *
     * @param minterWallet гаманець V3/V4 власника колекції, який сплачує газ
     * @param owner адреса власника NFT
     * @param content контент NFT
     * @param index індекс NFT
     * @param privateKey ключ гаманця мінтера
     */
    public void mintNft(Wallet minterWallet, Address owner, Cell content, long index, Crypto.PrivateKey privateKey) throws IOException {
        CellBuilder bodyBuilder = new CellBuilder();
        bodyBuilder.storeUInt(32, 0x178d4519);
        bodyBuilder.storeUInt(64, 0);
        bodyBuilder.storeUInt(64, index);
        bodyBuilder.storeAddress(owner);
        bodyBuilder.storeRef(content);

        sendThroughWallet(minterWallet, this.address, bodyBuilder.build(), DEFAULT_ATTACHED_TON, privateKey);
    }
}