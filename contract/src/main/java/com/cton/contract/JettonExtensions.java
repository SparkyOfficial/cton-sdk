// JettonExtensions.java - розширені функції для Jetton токенів
// Author: Андрій Будильников (Sparky)
// Extended functions for Jetton tokens
// Расширенные функции для Jetton токенов

package com.cton.contract;

import java.io.IOException;
import java.math.BigInteger;
import java.util.Date;

import com.cton.api.TonApiClient;
import com.cton.sdk.Address;
import com.cton.sdk.Crypto;
import com.google.gson.JsonObject;

/**
 * Розширені функції для Jetton токенів
 * 
 * Extended functions for Jetton tokens
 * Расширенные функции для Jetton токенов
 */
public class JettonExtensions extends Jetton {
    
    /**
     * Конструктор
     * @param address адреса контракту токена
     * @param apiClient API клієнт для взаємодії з мережею
     */
    public JettonExtensions(Address address, TonApiClient apiClient) {
        super(address, apiClient);
    }
    
    /**
     * Переказ жетонів без forward_ton. Повідомлення все одно йде через гаманець власника:
     * jetton-wallet не приймає зовнішній BOC, газ у TON сплачує гаманець.
     *
     * @param ownerWallet гаманець V3/V4 відправника
     * @param toAddress адреса нового власника жетонів
     * @param amount кількість токенів
     * @param privateKey ключ гаманця
     */
    public void feelessTransfer(Wallet ownerWallet, Address toAddress, BigInteger amount,
                               Crypto.PrivateKey privateKey) throws IOException {
        transfer(ownerWallet, toAddress, amount, BigInteger.ZERO, null, privateKey);
    }
    
    /**
     * Створення vesting розкладу для токенів
     * Create vesting schedule for tokens
     * Создание vesting расписания для токенов
     * 
     * @param wallet гаманець V3/V4, який надсилає внутрішнє повідомлення і сплачує газ
     * @param beneficiary адреса бенефіціара
     * @param totalAmount загальна кількість токенів
     * @param startDate дата початку vesting
     * @param endDate дата закінчення vesting
     * @param cliffDate дата кліфу (коли починається розблокування)
     * @param privateKey ключ гаманця
     * @throws IOException якщо сталася помилка мережі
     */
    public void createVestingSchedule(Wallet wallet, Address beneficiary, BigInteger totalAmount,
                                     Date startDate, Date endDate, Date cliffDate,
                                     Crypto.PrivateKey privateKey) throws IOException {
        throw new IOException("This call was not sent. Vesting is not part of the jetton standard, and this SDK does not broadcast a placeholder opcode.");
    }
    
    /**
     * Голосування за пропозицію у governance
     * Vote for a proposal in governance
     * Голосование за предложение в governance
     * 
     * @param wallet гаманець V3/V4 виборця, який надсилає внутрішнє повідомлення і сплачує газ
     * @param proposalId ідентифікатор пропозиції
     * @param vote голос (true - за, false - проти)
     * @param privateKey ключ гаманця
     * @throws IOException якщо сталася помилка мережі
     */
    public void voteOnProposal(Wallet wallet, long proposalId, boolean vote,
                              Crypto.PrivateKey privateKey) throws IOException {
        throw new IOException("This call was not sent. Governance voting is not part of the jetton standard, and this SDK does not broadcast a placeholder opcode.");
    }
    
    /**
     * Отримати інформацію про пропозицію governance
     * Get information about governance proposal
     * Получить информацию о предложении governance
     * 
     * @param proposalId ідентифікатор пропозиції
     * @return інформація про пропозицію
     * @throws IOException якщо сталася помилка мережі
     */
    public JsonObject getProposalInfo(long proposalId) throws IOException {
        throw new IOException("Jetton contracts have no get_proposal_data method.");
    }
}