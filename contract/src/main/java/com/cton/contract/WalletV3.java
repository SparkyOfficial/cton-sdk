// WalletV3.java - реалізація кошелька версії 3
// Author: Андрій Будильников (Sparky)
// Wallet version 3 implementation
// Реализация кошелька версии 3

package com.cton.contract;

import com.cton.sdk.Address;
import com.cton.sdk.Cell;
import com.cton.sdk.CellBuilder;
import com.cton.api.TonApiClient;
import java.math.BigInteger;
import java.io.IOException;

/**
 * Реалізація кошелька версії 3
 * 
 * Wallet version 3 implementation
 * Реализация кошелька версии 3
 */
public class WalletV3 extends BaseWallet {
    private final int subwalletId;
    
    /**
     * Конструктор
     * @param address адреса кошелька
     * @param apiClient API клієнт для взаємодії з мережею
     * @param subwalletId ідентифікатор підкошелька
     */
    public WalletV3(Address address, TonApiClient apiClient, int subwalletId) {
        super(address, apiClient);
        this.subwalletId = subwalletId;
    }
    
    /**
     * Конструктор зі значенням subwalletId за замовчуванням
     * @param address адреса кошелька
     * @param apiClient API клієнт для взаємодії з мережею
     */
    public WalletV3(Address address, TonApiClient apiClient) {
        this(address, apiClient, 698983191);
    }
    
    @Override
    public Cell createTransfer(Address destination, BigInteger amount, String comment) throws IOException {
        // Wallet V3R2 виконує простий переказ як внутрішнє повідомлення (int_msg_info)
        // з тілом op=0x0 (+ коментар). Будкуємо через стандартний конструктор BaseWallet.
        CellBuilder bodyBuilder = new CellBuilder();
        bodyBuilder.storeUInt(32, 0); // op code
        if (comment != null && !comment.isEmpty()) {
            bodyBuilder.storeBytes(comment.getBytes(java.nio.charset.StandardCharsets.UTF_8));
        }
        Cell body = bodyBuilder.build();
        return buildInternalMessage(destination, amount, body, true);
    }
    
    @Override
    public int getVersion() throws IOException {
        return 3;
    }
    
    @Override
    public int getSubwalletId() throws IOException {
        return this.subwalletId;
    }
}