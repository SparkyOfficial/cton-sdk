// TonApiClient.java - HTTP клієнт для TON Center API
// Author: Андрій Будильников (Sparky)
// HTTP client for TON Center API
// HTTP клиент для TON Center API

package com.cton.api;

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
     * @param address адреса у форматі raw (наприклад, 0:1234...)
     * @return інформація про адресу у форматі JSON
     * @throws IOException якщо сталася помилка мережі
     */
    public JsonObject getAddressInformation(String address) throws IOException {
        // формуємо URL запиту
        // формируем URL запроса
        // build request URL
        String url = baseUrl + "getAddressInformation?address=" + address;
        
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
     * @param address адреса у форматі raw (наприклад, 0:1234...)
     * @return CompletableFuture з інформацією про адресу
     */
    public CompletableFuture<JsonObject> getAddressInformationAsync(String address) {
        return CompletableFuture.supplyAsync(() -> {
            try {
                return getAddressInformation(address);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
    }
    
    /**
     * Виконати get-метод смарт-контракту
     * @param address адреса контракту у форматі raw
     * @param method назва методу
     * @param stack параметри методу
     * @return результат виконання методу
     * @throws IOException якщо сталася помилка мережі
     */
    public JsonObject runGetMethod(String address, String method, JsonObject stack) throws IOException {
        // формуємо URL запиту
        // формируем URL запроса
        // build request URL
        String url = baseUrl + "runGetMethod";
        
        // створюємо JSON тіло запиту
        // создаем JSON тело запроса
        // create JSON request body
        JsonObject requestBody = new JsonObject();
        requestBody.addProperty("address", address);
        requestBody.addProperty("method", method);
        requestBody.add("stack", stack);
        
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
     * Асинхронно виконати get-метод смарт-контракту
     * @param address адреса контракту у форматі raw
     * @param method назва методу
     * @param stack параметри методу
     * @return CompletableFuture з результатом виконання методу
     */
    public CompletableFuture<JsonObject> runGetMethodAsync(String address, String method, JsonObject stack) {
        return CompletableFuture.supplyAsync(() -> {
            try {
                return runGetMethod(address, method, stack);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
    }
    
    /**
     * Отримати інформацію про блок
     * @param workchain робочий ланцюг
     * @param shard шард
     * @param seqno номер послідовності
     * @return інформація про блок
     * @throws IOException якщо сталася помилка мережі
     */
    public JsonObject getBlockHeader(int workchain, long shard, long seqno) throws IOException {
        // формуємо URL запиту
        // формируем URL запроса
        // build request URL
        String url = baseUrl + "getBlockHeader";
        
        // створюємо JSON тіло запиту
        // создаем JSON тело запроса
        // create JSON request body
        JsonObject requestBody = new JsonObject();
        requestBody.addProperty("workchain", workchain);
        requestBody.addProperty("shard", shard);
        requestBody.addProperty("seqno", seqno);
        
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
     * Асинхронно отримати інформацію про блок
     * @param workchain робочий ланцюг
     * @param shard шард
     * @param seqno номер послідовності
     * @return CompletableFuture з інформацією про блок
     */
    public CompletableFuture<JsonObject> getBlockHeaderAsync(int workchain, long shard, long seqno) {
        return CompletableFuture.supplyAsync(() -> {
            try {
                return getBlockHeader(workchain, shard, seqno);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
    }
    
    /**
     * Отримати транзакції з блоку
     * @param workchain робочий ланцюг
     * @param shard шард
     * @param seqno номер послідовності
     * @return транзакції з блоку
     * @throws IOException якщо сталася помилка мережі
     */
    public JsonObject getBlockTransactions(int workchain, long shard, long seqno) throws IOException {
        // формуємо URL запиту
        // формируем URL запроса
        // build request URL
        String url = baseUrl + "getBlockTransactions";
        
        // створюємо JSON тіло запиту
        // создаем JSON тело запроса
        // create JSON request body
        JsonObject requestBody = new JsonObject();
        requestBody.addProperty("workchain", workchain);
        requestBody.addProperty("shard", shard);
        requestBody.addProperty("seqno", seqno);
        
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
     * Асинхронно отримати транзакції з блоку
     * @param workchain робочий ланцюг
     * @param shard шард
     * @param seqno номер послідовності
     * @return CompletableFuture з транзакціями з блоку
     */
    public CompletableFuture<JsonObject> getBlockTransactionsAsync(int workchain, long shard, long seqno) {
        return CompletableFuture.supplyAsync(() -> {
            try {
                return getBlockTransactions(workchain, shard, seqno);
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
     * Асинхронно надіслати BOC в мережу
     * @param bocBytes байти BOC
     * @return CompletableFuture з результатом операції
     */
    public CompletableFuture<JsonObject> sendBocAsync(byte[] bocBytes) {
        return CompletableFuture.supplyAsync(() -> {
            try {
                return sendBoc(bocBytes);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
    }
    
    /**
     * Надіслати BOC в мережу у форматі Base64
     * @param bocBase64 BOC у форматі Base64
     * @return результат операції у форматі JSON
     * @throws IOException якщо сталася помилка мережі
     */
    public JsonObject sendBoc(String bocBase64) throws IOException {
        // формуємо URL запиту
        // формируем URL запроса
        // build request URL
        String url = baseUrl + "sendBoc";
        
        // створюємо JSON тіло запиту
        // создаем JSON тело запроса
        // create JSON request body
        JsonObject requestBody = new JsonObject();
        requestBody.addProperty("boc", bocBase64);
        
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
     * Асинхронно надіслати BOC в мережу у форматі Base64
     * @param bocBase64 BOC у форматі Base64
     * @return CompletableFuture з результатом операції
     */
    public CompletableFuture<JsonObject> sendBocAsync(String bocBase64) {
        return CompletableFuture.supplyAsync(() -> {
            try {
                return sendBoc(bocBase64);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
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