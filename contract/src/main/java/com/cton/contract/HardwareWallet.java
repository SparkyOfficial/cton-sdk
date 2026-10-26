// HardwareWallet.java - підтримка апаратних кошельків (Ledger тощо)
// Author: Андрій Будильников (Sparky)
// Hardware wallet support (Ledger, etc.)
// Поддержка аппаратных кошельков (Ledger и т.д.)

package com.cton.contract;

import java.io.IOException;
import java.math.BigInteger;
import java.util.List;

import com.cton.api.TonApiClient;
import com.cton.sdk.Address;
import com.cton.sdk.Cell;

/**
 * Підтримка апаратних кошельків для TON блокчейну
 * 
 * Hardware wallet support for TON blockchain
 * Поддержка аппаратных кошельков для TON блокчейна
 */
public class HardwareWallet extends BaseWallet {
    private final String deviceType;
    private final String deviceId;
    private final int accountIndex;
    
    /**
     * Конструктор апаратного кошелька
     * @param address адреса кошелька
     * @param apiClient API клієнт для взаємодії з мережею
     * @param deviceType тип пристрою (наприклад, "Ledger")
     * @param deviceId ідентифікатор пристрою
     * @param accountIndex індекс облікового запису
     */
    public HardwareWallet(Address address, TonApiClient apiClient, String deviceType, 
                         String deviceId, int accountIndex) {
        super(address, apiClient);
        this.deviceType = deviceType;
        this.deviceId = deviceId;
        this.accountIndex = accountIndex;
    }
    
    /**
     * Конструктор апаратного кошелька зі значенням accountIndex за замовчуванням
     * @param address адреса кошелька
     * @param apiClient API клієнт для взаємодії з мережею
     * @param deviceType тип пристрою (наприклад, "Ledger")
     * @param deviceId ідентифікатор пристрою
     */
    public HardwareWallet(Address address, TonApiClient apiClient, String deviceType, 
                         String deviceId) {
        this(address, apiClient, deviceType, deviceId, 0);
    }
    
    @Override
    public Cell createTransfer(Address destination, BigInteger amount, String comment) throws IOException {
        return super.createTransfer(destination, amount, comment);
    }
    
    /**
     * Підписати транзакцію на апаратному пристрої
     * @param transaction транзакція для підпису
     * @return підписана транзакція
     * @throws IOException якщо сталася помилка
     */
    public Cell signTransactionOnDevice(Cell transaction) throws IOException {
        throw new IOException("Hardware wallet transport is not implemented. The transaction was not signed.");
    }
    
    /**
     * Отримати адресу з апаратного пристрою
     * @return адреса кошелька
     * @throws IOException якщо сталася помилка
     */
    public Address getAddressFromDevice() throws IOException {
        throw new IOException("Hardware wallet transport is not implemented. The address was not read from a device.");
    }
    
    /**
     * Отримати баланс з апаратного пристрою
     * @return баланс у нанотоні
     * @throws IOException якщо сталася помилка мережі
     */
    @Override
    public BigInteger getBalance() throws IOException {
        return super.getBalance();
    }
    
    /**
     * Отримати тип пристрою
     * @return тип пристрою
     */
    public String getDeviceType() {
        return deviceType;
    }
    
    /**
     * Отримати ідентифікатор пристрою
     * @return ідентифікатор пристрою
     */
    public String getDeviceId() {
        return deviceId;
    }
    
    /**
     * Отримати індекс облікового запису
     * @return індекс облікового запису
     */
    public int getAccountIndex() {
        return accountIndex;
    }
    
    @Override
    public int getVersion() throws IOException {
        return 200; // Версія для апаратного кошелька
    }
    
    /**
     * Перевірити підключення пристрою
     * @return true якщо пристрій підключено, false в іншому випадку
     */
    public boolean isDeviceConnected() {
        return false;
    }
    
    /**
     * Отримати список підтримуваних пристроїв
     * @return список підтримуваних пристроїв
     */
    public static List<String> getSupportedDevices() {
        return List.of();
    }
}