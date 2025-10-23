// LiteClientExample.java - приклад використання LiteClient
// Author: Андрій Будильников (Sparky)
// Example of using LiteClient
// Пример использования LiteClient

package com.cton.api.example;

import com.cton.api.liteclient.TonLiteClient;
import com.cton.api.liteclient.LiteClient;
import com.google.gson.JsonObject;
import java.io.IOException;

/**
 * Приклад використання LiteClient
 */
public class LiteClientExample {
    
    public static void main(String[] args) {
        // Створюємо клієнт для LiteServer
        // Создаем клиент для LiteServer
        // Create client for LiteServer
        LiteClient client = new TonLiteClient();
        
        try {
            System.out.println("CTON-SDK LiteClient Example");
            System.out.println("==========================");
            
            // Підключаємось до LiteServer
            // Подключаемся к LiteServer
            // Connect to LiteServer
            System.out.println("Connecting to LiteServer...");
            client.connect("127.0.0.1", 8080, "server-public-key");
            
            if (client.isConnected()) {
                System.out.println("Connected successfully!");
                
                // Виконуємо get-метод смарт-контракту
                // Выполняем get-метод смарт-контракта
                // Execute smart contract get-method
                System.out.println("Running get-method...");
                JsonObject stack = new JsonObject();
                JsonObject result = client.runGetMethod(
                    "EQCD39VS5jcptHL8vMjEXrzGaRcCVYto7HUn4bpAOg8xqB2N", 
                    "get_balance", 
                    stack
                );
                
                System.out.println("Method result: " + result.toString());
                
                // Отримуємо інформацію про блок
                // Получаем информацию о блоке
                // Get block information
                System.out.println("Getting block header...");
                JsonObject blockHeader = client.getBlockHeader(0, 123456789L, 1000L);
                System.out.println("Block header: " + blockHeader.toString());
                
            } else {
                System.out.println("Failed to connect to LiteServer");
            }
            
        } catch (IOException e) {
            System.err.println("Error communicating with LiteServer: " + e.getMessage());
            e.printStackTrace();
        } finally {
            // Відключаємось від LiteServer
            // Отключаемся от LiteServer
            // Disconnect from LiteServer
            if (client.isConnected()) {
                client.disconnect();
                System.out.println("Disconnected from LiteServer");
            }
        }
    }
}