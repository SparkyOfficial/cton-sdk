// TonLiteClient.java - клієнт LiteServer
// Author: Андрій Будильников (Sparky)

package com.cton.api.liteclient;

import java.io.IOException;
import java.util.concurrent.CompletableFuture;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

/**
 * Пряме з'єднання з TON LiteServer.
 * LiteServer говорить бінарним ADNL/TL. HTTP і WebSocket на цей порт не надсилаються.
 */
public class TonLiteClient implements LiteClient {
    private static final String NOT_IMPLEMENTED =
        "TON LiteServer speaks ADNL/TL. This client does not open an HTTP or WebSocket connection to that port.";

    @Override
    public void connect(String host, int port, String publicKeyBase64) throws IOException {
        throw new IOException(NOT_IMPLEMENTED);
    }

    @Override
    public void disconnect() {
    }

    @Override
    public boolean isConnected() {
        return false;
    }

    @Override
    public JsonObject runGetMethod(String address, String method, JsonArray stack) throws IOException {
        throw new IOException(NOT_IMPLEMENTED);
    }

    @Override
    public CompletableFuture<JsonObject> runGetMethodAsync(String address, String method, JsonArray stack) {
        return CompletableFuture.failedFuture(new IOException(NOT_IMPLEMENTED));
    }

    @Override
    public JsonObject getBlockHeader(int workchain, long shard, long seqno) throws IOException {
        throw new IOException(NOT_IMPLEMENTED);
    }

    @Override
    public JsonObject getBlockTransactions(int workchain, long shard, long seqno) throws IOException {
        throw new IOException(NOT_IMPLEMENTED);
    }

    @Override
    public JsonObject sendExternalMessage(String messageBoc) throws IOException {
        throw new IOException(NOT_IMPLEMENTED);
    }
}
