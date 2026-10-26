// Jetton.java - інтерфейс для роботи з Jetton токенами
// Author: Андрій Будильников (Sparky)
// Interface for working with Jetton tokens
// Интерфейс для работы с Jetton токенами

package com.cton.contract;

import java.io.IOException;
import java.math.BigInteger;

import com.cton.api.TonApiClient;
import com.cton.sdk.Address;
import com.cton.sdk.CellBuilder;
import com.cton.sdk.Crypto;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

/**
 * Інтерфейс для роботи з Jetton токенами відповідно до TEP-74
 * 
 * Interface for working with Jetton tokens according to TEP-74
 * Интерфейс для работы с Jetton токенами согласно TEP-74
 */
public class Jetton extends Contract {
    
    /**
     * Конструктор
     * @param address адреса контракту токена
     * @param apiClient API клієнт для взаємодії з мережею
     */
    public Jetton(Address address, TonApiClient apiClient) {
        super(address, apiClient);
    }
    
    /**
     * Отримати загальну емісію токенів
     * @return загальна емісія
     * @throws IOException якщо сталася помилка мережі
     */
    public BigInteger getTotalSupply() throws IOException {
        JsonArray stack = new JsonArray();
        JsonObject result = runGetMethod("get_jetton_data", stack);
        
        // Перевіряємо чи є результат
        // Проверяем, есть ли результат
        // Check if there is a result
        if (!result.has("result") || result.get("result").isJsonNull()) {
            throw new IOException("Failed to get jetton data");
        }
        
        // Отримуємо стек результатів
        // Получаем стек результатов
        // Get result stack
        JsonArray stackArray = result.getAsJsonArray("result");
        if (stackArray.size() < 4) {
            throw new IOException("Invalid jetton data format");
        }
        
        // Загальна емісія - перший елемент стеку
        // Total supply - first element of stack
        // Общая эмиссия - первый элемент стека
        JsonElement supplyElement = stackArray.get(0);
        if (!supplyElement.isJsonArray()) {
            throw new IOException("Invalid supply data format");
        }
        
        JsonArray supplyArray = supplyElement.getAsJsonArray();
        if (supplyArray.size() < 2) {
            throw new IOException("Invalid supply array format");
        }
        
        // Другий елемент містить значення
        // Second element contains the value
        // Второй элемент содержит значение
        String supplyStr = supplyArray.get(1).getAsString();
        return new BigInteger(supplyStr);
    }
    
    /**
     * Отримати адресу minter контракту
     * @return адреса minter контракту
     * @throws IOException якщо сталася помилка мережі
     */
    public Address getJettonMinter() throws IOException {
        JsonArray stack = new JsonArray();
        JsonObject result = runGetMethod("get_jetton_data", stack);
        
        // Перевіряємо чи є результат
        // Проверяем, есть ли результат
        // Check if there is a result
        if (!result.has("result") || result.get("result").isJsonNull()) {
            throw new IOException("Failed to get jetton data");
        }
        
        // Отримуємо стек результатів
        // Получаем стек результатов
        // Get result stack
        JsonArray stackArray = result.getAsJsonArray("result");
        if (stackArray.size() < 4) {
            throw new IOException("Invalid jetton data format");
        }
        
        // Адреса minter - третій елемент стеку
        // Minter address - third element of stack
        // Адрес minter - третий элемент стека
        JsonElement minterElement = stackArray.get(2);
        if (!minterElement.isJsonArray()) {
            throw new IOException("Invalid minter data format");
        }
        
        JsonArray minterArray = minterElement.getAsJsonArray();
        if (minterArray.size() < 2) {
            throw new IOException("Invalid minter array format");
        }
        
        // Другий елемент містить адресу
        // Second element contains the address
        // Второй элемент содержит адрес
        String addressStr = minterArray.get(1).getAsString();
        return new Address(addressStr);
    }
    
    /**
     * Отримати баланс користувача
     * @param owner адреса власника
     * @return баланс користувача
     * @throws IOException якщо сталася помилка мережі
     */
    public BigInteger getBalance(Address owner) throws IOException {
        JsonArray stack = new JsonArray();
        JsonArray ownerItem = new JsonArray();
        ownerItem.add("tvm.Slice");
        ownerItem.add(owner.toRaw());
        stack.add(ownerItem);
        
        JsonObject result = runGetMethod("get_wallet_data", stack);
        
        // Перевіряємо чи є результат
        // Проверяем, есть ли результат
        // Check if there is a result
        if (!result.has("result") || result.get("result").isJsonNull()) {
            throw new IOException("Failed to get wallet data");
        }
        
        // Отримуємо стек результатів
        // Получаем стек результатов
        // Get result stack
        JsonArray stackArray = result.getAsJsonArray("result");
        if (stackArray.size() < 1) {
            throw new IOException("Invalid wallet data format");
        }
        
        // Баланс - перший елемент стеку
        // Balance - first element of stack
        // Баланс - первый элемент стека
        JsonElement balanceElement = stackArray.get(0);
        if (!balanceElement.isJsonArray()) {
            throw new IOException("Invalid balance data format");
        }
        
        JsonArray balanceArray = balanceElement.getAsJsonArray();
        if (balanceArray.size() < 2) {
            throw new IOException("Invalid balance array format");
        }
        
        // Другий елемент містить значення
        // Second element contains the value
        // Второй элемент содержит значение
        String balanceStr = balanceArray.get(1).getAsString();
        return new BigInteger(balanceStr);
    }
    
    /**
     * Отримати адресу кошелька токенів для користувача
     * @param owner адреса власника
     * @return адреса кошелька токенів
     * @throws IOException якщо сталася помилка мережі
     */
    public Address getWalletAddress(Address owner) throws IOException {
        JsonArray stack = new JsonArray();
        JsonArray ownerItem = new JsonArray();
        ownerItem.add("tvm.Slice");
        ownerItem.add(owner.toRaw());
        stack.add(ownerItem);
        
        JsonObject result = runGetMethod("get_wallet_address", stack);
        
        // Перевіряємо чи є результат
        // Проверяем, есть ли результат
        // Check if there is a result
        if (!result.has("result") || result.get("result").isJsonNull()) {
            throw new IOException("Failed to get wallet address");
        }
        
        // Отримуємо стек результатів
        // Получаем стек результатов
        // Get result stack
        JsonArray stackArray = result.getAsJsonArray("result");
        if (stackArray.size() < 1) {
            throw new IOException("Invalid wallet address format");
        }
        
        // Адреса кошелька - перший елемент стеку
        // Wallet address - first element of stack
        // Адрес кошелька - первый элемент стека
        JsonElement walletElement = stackArray.get(0);
        if (!walletElement.isJsonArray()) {
            throw new IOException("Invalid wallet address data format");
        }
        
        JsonArray walletArray = walletElement.getAsJsonArray();
        if (walletArray.size() < 2) {
            throw new IOException("Invalid wallet address array format");
        }
        
        // Другий елемент містить адресу
        // Second element contains the address
        // Второй элемент содержит адрес
        String addressStr = walletArray.get(1).getAsString();
        return new Address(addressStr);
    }
    
    /**
     * Переказ жетонів через гаманець власника.
     * Jetton-wallet шукається у мінтера за адресою гаманця.
     *
     * @param ownerWallet гаманець V3/V4, який підписує зовнішнє повідомлення і сплачує газ
     * @param toAddress новий власник жетонів
     * @param amount кількість жетонів
     * @param forwardAmount TON (нанотони), який jetton-wallet перешле далі
     * @param comment коментар у forward_payload
     * @param privateKey ключ гаманця
     */
    public void transfer(Wallet ownerWallet, Address toAddress, BigInteger amount,
                         BigInteger forwardAmount, String comment, Crypto.PrivateKey privateKey) throws IOException {
        if (ownerWallet == null) {
            throw new IOException("Owner wallet is required to send the jetton transfer");
        }
        Address jettonWalletAddress = getWalletAddress(ownerWallet.getAddress());
        transfer(ownerWallet, jettonWalletAddress, toAddress, amount, forwardAmount, comment, privateKey);
    }

    /**
     * Переказ жетонів. Тіло з опкодом 0x0f8a7ea5 іде внутрішнім повідомленням
     * на jetton-wallet відправника. Підпис Ed25519 ставить гаманець власника,
     * а не контракт жетона: jetton-wallet не приймає external-in.
     *
     * @param ownerWallet гаманець V3/V4, який підписує зовнішнє повідомлення і сплачує газ
     * @param jettonWalletAddress jetton-wallet відправника (отримувач внутрішнього повідомлення)
     * @param toAddress новий власник жетонів
     * @param amount кількість жетонів
     * @param forwardAmount TON (нанотони), який jetton-wallet перешле далі
     * @param comment коментар у forward_payload
     * @param privateKey ключ гаманця
     */
    public void transfer(Wallet ownerWallet, Address jettonWalletAddress, Address toAddress, BigInteger amount,
                         BigInteger forwardAmount, String comment, Crypto.PrivateKey privateKey) throws IOException {
        if (ownerWallet == null) {
            throw new IOException("Owner wallet is required to send the jetton transfer");
        }
        if (jettonWalletAddress == null) {
            throw new IOException("Jetton wallet address is required");
        }
        BigInteger forward = forwardAmount == null ? BigInteger.ZERO : forwardAmount;
        if (forward.signum() < 0) {
            throw new IOException("forwardAmount must be non-negative");
        }

        // Тіло transfer#0f8a7ea5 за TEP-74. Це пейлоад внутрішнього повідомлення, не зовнішнє.
        CellBuilder bodyBuilder = new CellBuilder();
        bodyBuilder.storeUInt(32, 0x0f8a7ea5);
        bodyBuilder.storeUInt(64, 0);
        bodyBuilder.storeCoins(amount);
        bodyBuilder.storeAddress(toAddress);
        bodyBuilder.storeAddress(ownerWallet.getAddress());
        bodyBuilder.storeBit(false); // custom_payload = Nothing
        bodyBuilder.storeCoins(forward);
        if (comment != null && !comment.isEmpty()) {
            bodyBuilder.storeBit(true); // forward_payload = ^Cell
            CellBuilder commentBuilder = new CellBuilder();
            commentBuilder.storeUInt(32, 0);
            commentBuilder.storeBytes(comment.getBytes(java.nio.charset.StandardCharsets.UTF_8));
            bodyBuilder.storeRef(commentBuilder.build());
        } else {
            bodyBuilder.storeBit(false); // порожній inline forward_payload
        }

        // Гаманець прикріплює forward_ton плюс газ, інакше jetton-wallet не зможе переслати повідомлення.
        BigInteger attachedTon = forward.add(DEFAULT_ATTACHED_TON);
        sendThroughWallet(ownerWallet, jettonWalletAddress, bodyBuilder.build(), attachedTon, privateKey);
    }
}