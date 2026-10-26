// HardwareWalletExample.java - приклад використання апаратного кошелька
// Author: Андрій Будильников (Sparky)
// Example of using hardware wallet
// Пример использования аппаратного кошелька

package com.cton.contract.example;

import java.io.IOException;
import java.math.BigInteger;
import java.util.List;

import com.cton.api.TonApiClient;
import com.cton.contract.HardwareWallet;
import com.cton.sdk.Address;

/**
 * Приклад використання апаратного кошелька
 */
public class HardwareWalletExample {
    
    public static void main(String[] args) {
        try {
            System.out.println("CTON-SDK Hardware Wallet Example");
            System.out.println("===============================");
            
            // Створюємо API клієнт
            TonApiClient apiClient = new TonApiClient("https://toncenter.com/api/v2/");
            
            // Створюємо адресу кошелька
            Address walletAddress = new Address("EQCD39VS5jcptHL8vMjEXrzGaRcCVYto7HUn4bpAOg8xqB2N");
            
            // Створюємо апаратний кошик (наприклад, Ledger)
            HardwareWallet hardwareWallet = new HardwareWallet(
                walletAddress, 
                apiClient, 
                "Ledger", 
                "ledger_device_id_12345",
                0 // account index
            );
            
            // Перевіряємо підключення пристрою
            boolean isConnected = hardwareWallet.isDeviceConnected();
            System.out.println("Hardware wallet connected: " + isConnected);
            
            // Отримуємо тип пристрою
            String deviceType = hardwareWallet.getDeviceType();
            System.out.println("Device type: " + deviceType);
            
            // Отримуємо ідентифікатор пристрою
            String deviceId = hardwareWallet.getDeviceId();
            System.out.println("Device ID: " + deviceId);
            
            // Отримуємо індекс облікового запису
            int accountIndex = hardwareWallet.getAccountIndex();
            System.out.println("Account index: " + accountIndex);
            
            try {
                hardwareWallet.getAddressFromDevice();
            } catch (IOException e) {
                System.out.println("Address from device: " + e.getMessage());
            }
            
            // Отримуємо баланс
            BigInteger balance = hardwareWallet.getBalance();
            System.out.println("Wallet balance: " + balance.toString() + " nanotons");
            
            // Створюємо транзакцію
            Address recipient = new Address("EQA8cLh74oFKcL523Jz9Hw5ReXY6Yglz8g422w7NwzvzL03V");
            BigInteger amount = BigInteger.valueOf(1000000000L); // 1 TON
            String comment = "Hardware wallet transfer";
            
            com.cton.sdk.Cell transaction = hardwareWallet.createTransfer(recipient, amount, comment);
            System.out.println("Transaction created");
            
            try {
                hardwareWallet.signTransactionOnDevice(transaction);
            } catch (IOException e) {
                System.out.println("Device signing: " + e.getMessage());
            }
            
            // У реальній реалізації тут має бути відправка підписаної транзакції
            // In real implementation, signed transaction should be sent here
            
            System.out.println("\nDevice transport is not implemented, so the transfer stays unsigned.");
            
            // Отримуємо список підтримуваних пристроїв
            List<String> supportedDevices = HardwareWallet.getSupportedDevices();
            System.out.println("\nSupported hardware wallets:");
            for (String device : supportedDevices) {
                System.out.println("  - " + device);
            }
            
        } catch (IOException e) {
            System.err.println("Error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}