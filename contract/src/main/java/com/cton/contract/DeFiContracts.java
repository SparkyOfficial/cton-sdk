// DeFiContracts.java - DeFi контракти для ліквідності, стейкінгу та кредитування
// Author: Андрій Будильников (Sparky)
// DeFi contracts for liquidity, staking and lending
// DeFi контракты для ликвидности, стейкинга и кредитования

package com.cton.contract;

import java.io.IOException;
import java.math.BigInteger;

import com.cton.api.TonApiClient;
import com.cton.sdk.Address;
import com.cton.sdk.Crypto;

/**
 * DeFi контракти для ліквідності, стейкінгу та кредитування
 * 
 * DeFi contracts for liquidity, staking and lending
 * DeFi контракты для ликвидности, стейкинга и кредитования
 */
public class DeFiContracts extends Contract {
    
    /**
     * Конструктор
     * @param address адреса контракту
     * @param apiClient API клієнт для взаємодії з мережею
     */
    public DeFiContracts(Address address, TonApiClient apiClient) {
        super(address, apiClient);
    }
    
    /**
     * Додати ліквідність до пулу
     * Add liquidity to pool
     * Добавить ликвидность в пул
     * 
     * @param token0Amount кількість токенів першого типу
     * @param token1Amount кількість токенів другого типу
     * @param wallet гаманець V3/V4, який надсилає внутрішнє повідомлення і сплачує газ
     * @param privateKey приватний ключ гаманця
     * @throws IOException якщо сталася помилка мережі
     */
    public void addLiquidity(BigInteger token0Amount, BigInteger token1Amount,
                            Wallet wallet, Crypto.PrivateKey privateKey) throws IOException {
        refusePlaceholderOpcode();
    }
    
    /**
     * Видалити ліквідність з пулу
     * Remove liquidity from pool
     * Удалить ликвидность из пула
     * 
     * @param liquidityAmount кількість ліквідних токенів для видалення
     * @param wallet гаманець V3/V4, який надсилає внутрішнє повідомлення і сплачує газ
     * @param privateKey приватний ключ гаманця
     * @throws IOException якщо сталася помилка мережі
     */
    public void removeLiquidity(BigInteger liquidityAmount, Wallet wallet,
                               Crypto.PrivateKey privateKey) throws IOException {
        refusePlaceholderOpcode();
    }
    
    /**
     * Стейкінг токенів
     * Staking tokens
     * Стейкинг токенов
     * 
     * @param amount кількість токенів для стейкінгу
     * @param wallet гаманець V3/V4, який надсилає внутрішнє повідомлення і сплачує газ
     * @param privateKey приватний ключ гаманця
     * @throws IOException якщо сталася помилка мережі
     */
    public void stakeTokens(BigInteger amount, Wallet wallet,
                           Crypto.PrivateKey privateKey) throws IOException {
        refusePlaceholderOpcode();
    }
    
    /**
     * Вивести стейкінг токенів
     * Withdraw staked tokens
     * Вывести стейкинг токенов
     * 
     * @param amount кількість токенів для виведення
     * @param wallet гаманець V3/V4, який надсилає внутрішнє повідомлення і сплачує газ
     * @param privateKey приватний ключ гаманця
     * @throws IOException якщо сталася помилка мережі
     */
    public void unstakeTokens(BigInteger amount, Wallet wallet,
                             Crypto.PrivateKey privateKey) throws IOException {
        refusePlaceholderOpcode();
    }
    
    /**
     * Взяти кредит
     * Take a loan
     * Взять кредит
     * 
     * @param collateralAmount кількість токенів в якості застави
     * @param loanAmount кількість токенів для позики
     * @param wallet гаманець V3/V4, який надсилає внутрішнє повідомлення і сплачує газ
     * @param privateKey приватний ключ гаманця
     * @throws IOException якщо сталася помилка мережі
     */
    public void takeLoan(BigInteger collateralAmount, BigInteger loanAmount,
                        Wallet wallet, Crypto.PrivateKey privateKey) throws IOException {
        refusePlaceholderOpcode();
    }
    
    /**
     * Погасити кредит
     * Repay a loan
     * Погасить кредит
     * 
     * @param repaymentAmount кількість токенів для погашення
     * @param wallet гаманець V3/V4, який надсилає внутрішнє повідомлення і сплачує газ
     * @param privateKey приватний ключ гаманця
     * @throws IOException якщо сталася помилка мережі
     */
    public void repayLoan(BigInteger repaymentAmount, Wallet wallet,
                         Crypto.PrivateKey privateKey) throws IOException {
        refusePlaceholderOpcode();
    }

    private static void refusePlaceholderOpcode() throws IOException {
        throw new IOException("This call was not sent. The SDK has no STON.fi or DeDust message schema, so it does not broadcast a placeholder opcode.");
    }
}