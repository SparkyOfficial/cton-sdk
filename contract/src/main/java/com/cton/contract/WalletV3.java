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
        // Отримуємо послідовний номер
        // Получаем последовательный номер
        // Get sequence number
        long seqno = getSeqno();
        
        // Створюємо комірку з тілом повідомлення
        // Создаем ячейку с телом сообщения
        // Create cell with message body
        CellBuilder bodyBuilder = new CellBuilder();
        
        // Додаємо код операції (0 для простого переказу)
        // Добавляем код операции (0 для простого перевода)
        // Add operation code (0 for simple transfer)
        bodyBuilder.storeUInt(32, 0);
        
        // Додаємо коментар якщо він є
        // Добавляем комментарий если он есть
        // Add comment if it exists
        if (comment != null && !comment.isEmpty()) {
            // Додаємо текст коментаря
            // Добавляем текст комментария
            // Add comment text
            bodyBuilder.storeUInt(32, 0); // Текст коментаря / Текст комментария / Comment text
            bodyBuilder.storeBytes(comment.getBytes());
        }
        
        Cell body = bodyBuilder.build();
        
        // Створюємо зовнішнє повідомлення
        // Создаем внешнее сообщение
        // Create external message
        CellBuilder messageBuilder = new CellBuilder();
        
        // Додаємо ідентифікатор запиту (тільки для Wallet V3+)
        // Добавляем идентификатор запроса (только для Wallet V3+)
        // Add request ID (only for Wallet V3+)
        messageBuilder.storeUInt(8, 0); // expire_at (0 = no expiration)
        
        // Додаємо адресу одержувача
        // Добавляем адрес получателя
        // Add recipient address
        messageBuilder.storeAddress(destination);
        
        // Додаємо суму
        // Добавляем сумму
        // Add amount
        messageBuilder.storeCoins(amount);
        
        // Додаємо тіло повідомлення
        // Добавляем тело сообщения
        // Add message body
        messageBuilder.storeRef(body);
        
        return messageBuilder.build();
    }
    
    @Override
    public int getVersion() throws IOException {
        return 3;
    }
}