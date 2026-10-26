// DeFiContractsExample.java - приклад використання DeFi контрактів
// Author: Андрій Будильников (Sparky)
// Example of using DeFi contracts
// Пример использования DeFi контрактов

package com.cton.contract.example;

import java.io.IOException;
import java.math.BigInteger;

import com.cton.api.TonApiClient;
import com.cton.contract.DeFiContracts;
import com.cton.contract.WalletV4;
import com.cton.sdk.Address;
import com.cton.sdk.Crypto;

/**
 * Приклад використання DeFi контрактів
 * 
 * Example of using DeFi contracts
 * Пример использования DeFi контрактов
 */
public class DeFiContractsExample {
    
    public static void main(String[] args) {
        try {
            // Ініціалізуємо API клієнт
            // Инициализируем API клиент
            // Initialize API client
            TonApiClient apiClient = new TonApiClient("https://toncenter.com/api/v2/jsonRPC");
            
            // Адреса контракту DeFi
            // Адрес контракта DeFi
            // DeFi contract address
            Address defiAddress = new Address("EQCD39VS5jcptHL8vMjEXrzGaRcCVYto7HUn4bpAOg8xqB2N");
            
            // Створюємо екземпляр DeFi контрактів
            // Создаем экземпляр DeFi контрактов
            // Create DeFi contracts instance
            DeFiContracts defi = new DeFiContracts(defiAddress, apiClient);
            
            // Створюємо приватний ключ (для прикладу)
            // Создаем приватный ключ (для примера)
            // Create private key (for example)
            Crypto.PrivateKey privateKey = Crypto.PrivateKey.generate();
            
            // Гаманець користувача підписує зовнішнє повідомлення і платить газ.
            // DeFi-контракт отримує лише внутрішнє повідомлення.
            WalletV4 wallet = new WalletV4(
                new Address("EQCD39VS5jcptHL8vMjEXrzGaRcCVYto7HUn4bpAOg8xqB2N"), apiClient);
            
            defi.addLiquidity(BigInteger.valueOf(1000), BigInteger.valueOf(2000),
                            wallet, privateKey);
            System.out.println("Added liquidity to pool");
            
            defi.removeLiquidity(BigInteger.valueOf(500), wallet, privateKey);
            System.out.println("Removed liquidity from pool");
            
            defi.stakeTokens(BigInteger.valueOf(10000), wallet, privateKey);
            System.out.println("Staked tokens");
            
            defi.unstakeTokens(BigInteger.valueOf(5000), wallet, privateKey);
            System.out.println("Unstaked tokens");
            
            defi.takeLoan(BigInteger.valueOf(5000), BigInteger.valueOf(10000),
                         wallet, privateKey);
            System.out.println("Took a loan");
            
            defi.repayLoan(BigInteger.valueOf(10000), wallet, privateKey);
            System.out.println("Repaid a loan");
            
        } catch (IOException e) {
            System.err.println("Error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}