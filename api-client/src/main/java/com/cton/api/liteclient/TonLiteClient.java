// TonLiteClient.java - реалізація клієнта для LiteServer
// Author: Андрій Будильников (Sparky)
// Implementation of client for LiteServer
// Реализация клиента для LiteServer

package com.cton.api.liteclient;

import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import okhttp3.*;
import java.io.IOException;
import java.util.concurrent.CompletableFuture;
import java.util.Base64;

/**
 * Реалізація клієнта для прямого з'єднання з TON LiteServer
 * 
 * Implementation of client for direct connection to TON LiteServer
 * Реализация клиента для прямого соединения с TON LiteServer
 */
public class TonLiteClient implements LiteClient {
    private final OkHttpClient httpClient;
    private String baseUrl;
    private boolean connected;
    
    /**
     * Конструктор
     */
    public TonLiteClient() {
        this.httpClient = new OkHttpClient();
        this.connected = false;
    }
    
    @Override
    public void connect(String host, int port, String publicKeyBase64) throws IOException {
        // Формуємо URL для підключення
        // Формируем URL для подключения
        // Build URL for connection
        this.baseUrl = "http://" + host + ":" + port;
        this.connected = true;
        
        // В реальній реалізації тут має бути встановлення WebSocket з'єднання
        // В реальной реализации здесь должно быть установление WebSocket соединения
        // In real implementation, WebSocket connection should be established here
    }
    
    @Override
    public void disconnect() {
        this.connected = false;
        // В реальній реалізації тут має бути закриття WebSocket з'єднання
        // В реальной реализации здесь должно быть закрытие WebSocket соединения
        // In real implementation, WebSocket connection should be closed here
    }
    
    @Override
    public boolean isConnected() {
        return this.connected;
    }
    
    @Override
    public JsonObject runGetMethod(String address, String method, JsonObject stack) throws IOException {
        if (!connected) {
            throw new IOException("Not connected to LiteServer");
        }
        
        // Формуємо URL запиту
        // Формируем URL запроса
        // Build request URL
        String url = baseUrl + "/runGetMethod";
        
        // Створюємо JSON тіло запиту
        // Создаем JSON тело запроса
        // Create JSON request body
        JsonObject requestBody = new JsonObject();
        requestBody.addProperty("address", address);
        requestBody.addProperty("method", method);
        requestBody.add("stack", stack);
        
        // Створюємо запит
        // Создаем запрос
        // Create request
        Request request = new Request.Builder()
            .url(url)
            .post(RequestBody.create(requestBody.toString(), MediaType.get("application/json")))
            .build();
        
        // Виконуємо запит
        // Выполняем запрос
        // Execute request
        try (Response response = httpClient.newCall(request).execute()) {
            if (!response.isSuccessful()) {
                throw new IOException("Unexpected response code: " + response);
            }
            
            ResponseBody body = response.body();
            if (body == null) {
                throw new IOException("Empty response body");
            }
            
            // Парсимо JSON відповідь
            // Парсим JSON ответ
            // Parse JSON response
            String responseBody = body.string();
            return JsonParser.parseString(responseBody).getAsJsonObject();
        }
    }
    
    @Override
    public CompletableFuture<JsonObject> runGetMethodAsync(String address, String method, JsonObject stack) {
        return CompletableFuture.supplyAsync(() -> {
            try {
                return runGetMethod(address, method, stack);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
    }
    
    @Override
    public JsonObject getBlockHeader(int workchain, long shard, long seqno) throws IOException {
        if (!connected) {
            throw new IOException("Not connected to LiteServer");
        }
        
        // Формуємо URL запиту
        // Формируем URL запроса
        // Build request URL
        String url = baseUrl + "/getBlockHeader";
        
        // Створюємо JSON тіло запиту
        // Создаем JSON тело запроса
        // Create JSON request body
        JsonObject requestBody = new JsonObject();
        requestBody.addProperty("workchain", workchain);
        requestBody.addProperty("shard", shard);
        requestBody.addProperty("seqno", seqno);
        
        // Створюємо запит
        // Создаем запрос
        // Create request
        Request request = new Request.Builder()
            .url(url)
            .post(RequestBody.create(requestBody.toString(), MediaType.get("application/json")))
            .build();
        
        // Виконуємо запит
        // Выполняем запрос
        // Execute request
        try (Response response = httpClient.newCall(request).execute()) {
            if (!response.isSuccessful()) {
                throw new IOException("Unexpected response code: " + response);
            }
            
            ResponseBody body = response.body();
            if (body == null) {
                throw new IOException("Empty response body");
            }
            
            // Парсимо JSON відповідь
            // Парсим JSON ответ
            // Parse JSON response
            String responseBody = body.string();
            return JsonParser.parseString(responseBody).getAsJsonObject();
        }
    }
    
    @Override
    public JsonObject getBlockTransactions(int workchain, long shard, long seqno) throws IOException {
        if (!connected) {
            throw new IOException("Not connected to LiteServer");
        }
        
        // Формуємо URL запиту
        // Формируем URL запроса
        // Build request URL
        String url = baseUrl + "/getBlockTransactions";
        
        // Створюємо JSON тіло запиту
        // Создаем JSON тело запроса
        // Create JSON request body
        JsonObject requestBody = new JsonObject();
        requestBody.addProperty("workchain", workchain);
        requestBody.addProperty("shard", shard);
        requestBody.addProperty("seqno", seqno);
        
        // Створюємо запит
        // Создаем запрос
        // Create request
        Request request = new Request.Builder()
            .url(url)
            .post(RequestBody.create(requestBody.toString(), MediaType.get("application/json")))
            .build();
        
        // Виконуємо запит
        // Выполняем запрос
        // Execute request
        try (Response response = httpClient.newCall(request).execute()) {
            if (!response.isSuccessful()) {
                throw new IOException("Unexpected response code: " + response);
            }
            
            ResponseBody body = response.body();
            if (body == null) {
                throw new IOException("Empty response body");
            }
            
            // Парсимо JSON відповідь
            // Парсим JSON ответ
            // Parse JSON response
            String responseBody = body.string();
            return JsonParser.parseString(responseBody).getAsJsonObject();
        }
    }
    
    @Override
    public JsonObject sendExternalMessage(String messageBoc) throws IOException {
        if (!connected) {
            throw new IOException("Not connected to LiteServer");
        }
        
        // Формуємо URL запиту
        // Формируем URL запроса
        // Build request URL
        String url = baseUrl + "/sendExternalMessage";
        
        // Створюємо JSON тіло запиту
        // Создаем JSON тело запроса
        // Create JSON request body
        JsonObject requestBody = new JsonObject();
        requestBody.addProperty("boc", messageBoc);
        
        // Створюємо запит
        // Создаем запрос
        // Create request
        Request request = new Request.Builder()
            .url(url)
            .post(RequestBody.create(requestBody.toString(), MediaType.get("application/json")))
            .build();
        
        // Виконуємо запит
        // Выполняем запрос
        // Execute request
        try (Response response = httpClient.newCall(request).execute()) {
            if (!response.isSuccessful()) {
                throw new IOException("Unexpected response code: " + response);
            }
            
            ResponseBody body = response.body();
            if (body == null) {
                throw new IOException("Empty response body");
            }
            
            // Парсимо JSON відповідь
            // Парсим JSON ответ
            // Parse JSON response
            String responseBody = body.string();
            return JsonParser.parseString(responseBody).getAsJsonObject();
        }
    }
}