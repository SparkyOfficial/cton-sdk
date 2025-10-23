// TonApiClient.java - HTTP клієнт для TON Center API
// Author: Андрій Будильников (Sparky)
// HTTP client for TON Center API
// HTTP клиент для TON Center API

package com.cton.api;

import com.cton.sdk.Address;
import com.cton.sdk.Cell;
import okhttp3.*;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.google.gson.JsonElement;
import java.io.IOException;
import java.util.Base64;
import java.util.concurrent.CompletableFuture;

/**
 * Клієнт для взаємодії з TON Center API
 * 
 * Client for interacting with TON Center API
 * Клиент для взаимодействия с TON Center API
 */
public class TonApiClient {
    private final OkHttpClient httpClient;
    private final String baseUrl;
    private final String apiKey;
    
    /**
     * Конструктор
     * @param baseUrl базова URL адреса API
     */
    public TonApiClient(String baseUrl) {
        this(baseUrl, null);
    }
    
    /**
     * Конструктор з API ключем
     * @param baseUrl базова URL адреса API
     * @param apiKey API ключ (якщо потрібен)
     */
    public TonApiClient(String baseUrl, String apiKey) {
        this.httpClient = new OkHttpClient();
        this.baseUrl = baseUrl.endsWith("/") ? baseUrl : baseUrl + "/";
        this.apiKey = apiKey;
    }
    
    /**
     * Отримати інформацію про адресу
     * @param address адреса
     * @return інформація про адресу у форматі JSON
     * @throws IOException якщо сталася помилка мережі
     */
    public JsonObject getAddressInformation(Address address) throws IOException {
        // формуємо URL запиту
        // формируем URL запроса
        // build request URL
        String url = baseUrl + "getAddressInformation?address=" + address.toRaw();
        
        // створюємо запит
        // создаем запрос
        // create request
        Request.Builder requestBuilder = new Request.Builder()
            .url(url)
            .get();
            
        // додаємо API ключ якщо потрібно
        // добавляем API ключ если нужно
        // add API key if needed
        if (apiKey != null && !apiKey.isEmpty()) {
            requestBuilder.addHeader("X-API-Key", apiKey);
        }
        
        Request request = requestBuilder.build();
        
        // виконуємо запит
        // выполняем запрос
        // execute request
        try (Response response = httpClient.newCall(request).execute()) {
            if (!response.isSuccessful()) {
                throw new IOException("Unexpected response code: " + response);
            }
            
            ResponseBody body = response.body();
            if (body == null) {
                throw new IOException("Empty response body");
            }
            
            // парсимо JSON відповідь
            // парсим JSON ответ
            // parse JSON response
            String responseBody = body.string();
            return JsonParser.parseString(responseBody).getAsJsonObject();
        }
    }
    
    /**
     * Асинхронне отримання інформації про адресу
     * @param address адреса
     * @return CompletableFuture з інформацією про адресу
     */
    public CompletableFuture<JsonObject> getAddressInformationAsync(Address address) {
        return CompletableFuture.supplyAsync(() -> {
            try {
                return getAddressInformation(address);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
    }
    
    /**
     * Надіслати BOC в мережу
     * @param bocBytes байти BOC
     * @return результат операції у форматі JSON
     * @throws IOException якщо сталася помилка мережі
     */
    public JsonObject sendBoc(byte[] bocBytes) throws IOException {
        // формуємо URL запиту
        // формируем URL запроса
        // build request URL
        String url = baseUrl + "sendBoc";
        
        // створюємо JSON тіло запиту
        // создаем JSON тело запроса
        // create JSON request body
        JsonObject requestBody = new JsonObject();
        requestBody.addProperty("boc", bytesToBase64(bocBytes));
        
        // створюємо запит
        // создаем запрос
        // create request
        Request.Builder requestBuilder = new Request.Builder()
            .url(url)
            .post(RequestBody.create(requestBody.toString(), MediaType.get("application/json")));
            
        // додаємо API ключ якщо потрібно
        // добавляем API ключ если нужно
        // add API key if needed
        if (apiKey != null && !apiKey.isEmpty()) {
            requestBuilder.addHeader("X-API-Key", apiKey);
        }
        
        Request request = requestBuilder.build();
        
        // виконуємо запит
        // выполняем запрос
        // execute request
        try (Response response = httpClient.newCall(request).execute()) {
            if (!response.isSuccessful()) {
                throw new IOException("Unexpected response code: " + response);
            }
            
            ResponseBody body = response.body();
            if (body == null) {
                throw new IOException("Empty response body");
            }
            
            // парсимо JSON відповідь
            // парсим JSON ответ
            // parse JSON response
            String responseBody = body.string();
            return JsonParser.parseString(responseBody).getAsJsonObject();
        }
    }
    
    /**
     * Конвертує байти в Base64 строку
     * @param bytes байти для конвертації
     * @return Base64 строка
     */
    private String bytesToBase64(byte[] bytes) {
        return Base64.getEncoder().encodeToString(bytes);
    }
    
    /**
     * Закрити HTTP клієнт
     */
    public void close() {
        httpClient.dispatcher().executorService().shutdown();
        httpClient.connectionPool().evictAll();
    }
}