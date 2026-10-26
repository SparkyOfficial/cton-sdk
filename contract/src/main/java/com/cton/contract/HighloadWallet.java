// HighloadWallet.java - реалізація високонавантаженого кошелька
// Author: Андрій Будильников (Sparky)
// Highload wallet implementation
// Реализация высоконагруженного кошелька

package com.cton.contract;

import java.io.IOException;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

import com.cton.api.TonApiClient;
import com.cton.sdk.Address;
import com.cton.sdk.Cell;
import com.cton.sdk.CellBuilder;
import com.cton.sdk.Crypto;

/**
 * Реалізація високонавантаженого кошелька (Highload Wallet v2)
 * 
 * Highload wallet implementation (Highload Wallet v2)
 * Реализация высоконагруженного кошелька (Highload Wallet v2)
 */
public class HighloadWallet extends BaseWallet {
    private final int subwalletId;
    
    /**
     * Конструктор
     * @param address адреса кошелька
     * @param apiClient API клієнт для взаємодії з мережею
     * @param subwalletId ідентифікатор підкошелька
     */
    public HighloadWallet(Address address, TonApiClient apiClient, int subwalletId) {
        super(address, apiClient);
        this.subwalletId = subwalletId;
    }
    
    /**
     * Конструктор зі значенням subwalletId за замовчуванням
     * @param address адреса кошелька
     * @param apiClient API клієнт для взаємодії з мережею
     */
    public HighloadWallet(Address address, TonApiClient apiClient) {
        this(address, apiClient, 698983191);
    }
    
    @Override
    public Cell createTransfer(Address destination, BigInteger amount, String comment) throws IOException {
        // Для високонавантаженого кошелька створюємо один елемент масиву
        BulkTransfer[] transfers = new BulkTransfer[] {
            new BulkTransfer(destination, amount, comment)
        };
        return createBulkTransfer(transfers);
    }
    
    @Override
    public Cell createBulkTransfer(BulkTransfer[] transfers) throws IOException {
        if (transfers == null || transfers.length == 0) {
            throw new IllegalArgumentException("At least one transfer is required");
        }
        if (transfers.length > 254) {
            throw new IllegalArgumentException("Highload Wallet v2 accepts at most 254 transfers");
        }

        // highload-wallet-v2: subwallet_id:uint32 query_id:uint64 messages:(HashmapE 16 Message)
        // Message = mode:uint8 ^internal. query_id має бути не менший за now()<<32.
        CellBuilder builder = new CellBuilder();
        builder.storeUInt(32, this.subwalletId);
        long queryId = (getValidUntil() << 32) | (System.nanoTime() & 0xffffffffL);
        builder.storeUInt(64, queryId);

        List<HashmapE.Entry> messages = new ArrayList<>(transfers.length);
        for (int i = 0; i < transfers.length; i++) {
            BulkTransfer transfer = transfers[i];
            Cell body = commentBody(transfer.getComment());
            Cell message = buildInternalMessage(
                transfer.getDestination(), transfer.getAmount(), body, true);
            int mode = transfer.getMode();
            messages.add(new HashmapE.Entry(i, value -> {
                value.storeUInt(8, mode);
                value.storeRef(message);
            }));
        }
        HashmapE.store(builder, 16, messages);
        return builder.build();
    }

    private static Cell commentBody(String comment) {
        CellBuilder body = new CellBuilder();
        body.storeUInt(32, 0);
        if (comment != null && !comment.isEmpty()) {
            body.storeBytes(comment.getBytes(StandardCharsets.UTF_8));
        }
        return body.build();
    }
    
    @Override
    public int getVersion() throws IOException {
        return 2; // Highload Wallet v2
    }
    
    @Override
    public int getSubwalletId() throws IOException {
        return this.subwalletId;
    }
    
    /**
     * Надіслати багато транзакцій
     * @param transfers масив транзакцій
     * @param privateKey приватний ключ для підпису
     * @throws IOException якщо сталася помилка мережі
     */
    public void sendBulkTransfer(BulkTransfer[] transfers, Crypto.PrivateKey privateKey) throws IOException {
        // Створюємо багато транзакцій
        Cell message = createBulkTransfer(transfers);
        
        // Підписуємо повідомлення приватним ключем
        com.cton.sdk.Boc boc = new com.cton.sdk.Boc(message);
        byte[] messageBytes = boc.serialize(true, true);
        byte[] signature = com.cton.sdk.Crypto.sign(privateKey, messageBytes);
        
        // Створюємо зовнішнє повідомлення з підписом
        CellBuilder externalMessageBuilder = new CellBuilder();
        externalMessageBuilder.storeBytes(signature); // Додаємо підпис
        externalMessageBuilder.storeRef(message); // Додаємо повідомлення
        
        Cell externalMessage = externalMessageBuilder.build();
        
        // Надсилаємо через API
        sendTransaction(externalMessage);
    }
}