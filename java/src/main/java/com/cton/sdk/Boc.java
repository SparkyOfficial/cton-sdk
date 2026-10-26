// Boc.java - Java обгортка для Boc C++ класу
// Author: Андрій Будильников (Sparky)

package com.cton.sdk;

import java.io.Closeable;
import java.lang.ref.Cleaner;
import java.util.Arrays;
import java.util.List;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

/**
 * Java обгортка для Boc C++ класу
 * 
 * BOC (Bag of Cells) - це формат серіалізації дерева комірок (Cell)
 * в бінарне представлення для передачі по мережі або зберігання
 */
public class Boc implements Closeable {
    // Інтерфейс для взаємодії з нативною бібліотекою
    public interface CtonLibrary extends Library {
        CtonLibrary INSTANCE = (CtonLibrary) Native.load("cton-sdk-core", CtonLibrary.class);
        
        // Створення нового BOC
        Pointer boc_create();
        
        // Створення BOC з кореневої комірки
        Pointer boc_create_with_root(Pointer rootCell);
        
        // Звільнення пам'яті BOC
        void boc_destroy(Pointer boc);
        
        // Серіалізувати BOC в бінарне представлення
        NativeBuffer boc_serialize(Pointer boc, boolean hasIdx, boolean hashCRC);
        
        // Десеріалізувати BOC з бінарного представлення
        Pointer boc_deserialize(byte[] data, int length);
        
        // Отримати кореневу комірку
        Pointer boc_get_root(Pointer boc);
        
        // Встановити кореневу комірку
        void boc_set_root(Pointer boc, Pointer rootCell);
        
        // Функція для звільнення пам'яті
        void free_string(Pointer str);

        // Звільнити буфер, виділений malloc у DLL
        void free_buffer(Pointer buffer);
    }

    /**
     * Буфер з DLL: вказівник і розмір однієї серіалізації.
     * data звільняється через free_buffer.
     */
    public static class NativeBuffer extends Structure implements Structure.ByValue {
        public Pointer data;
        public long size;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("data", "size");
        }
    }
    
    private Pointer nativeBoc;
    private boolean closed = false;
    private final Cleaner.Cleanable cleanable;
    
    /**
     * Конструктор за замовчуванням
     */
    public Boc() {
        this.nativeBoc = CtonLibrary.INSTANCE.boc_create();
        if (this.nativeBoc == null) {
            throw new OutOfMemoryError("boc_create failed");
        }
        this.cleanable = NativeResources.track(this, nativeBoc, CtonLibrary.INSTANCE::boc_destroy);
    }
    
    /**
     * Конструктор з кореневої комірки
     * @param root коренева комірка
     */
    public Boc(Cell root) {
        this.nativeBoc = CtonLibrary.INSTANCE.boc_create_with_root(root.nativeCell);
        if (this.nativeBoc == null) {
            throw new IllegalArgumentException("Failed to create BOC from root cell");
        }
        this.cleanable = NativeResources.track(this, nativeBoc, CtonLibrary.INSTANCE::boc_destroy);
    }
    
    /**
     * Приватний конструктор для внутрішнього використання
     */
    Boc(Pointer nativeBoc) {
        this.nativeBoc = nativeBoc;
        this.cleanable = nativeBoc == null
            ? null
            : NativeResources.track(this, nativeBoc, CtonLibrary.INSTANCE::boc_destroy);
    }
    
    /**
     * Серіалізувати BOC в бінарне представлення
     * @param hasIdx чи включати індекс
     * @param hashCRC чи включати CRC хеш
     * @return бінарне представлення BOC
     */
    public byte[] serialize(boolean hasIdx, boolean hashCRC) {
        if (closed) {
            throw new IllegalStateException("Boc has been closed");
        }
        
        NativeBuffer buffer = CtonLibrary.INSTANCE.boc_serialize(nativeBoc, hasIdx, hashCRC);
        if (buffer == null || buffer.data == null || buffer.size <= 0) {
            return new byte[0];
        }
        if (buffer.size > Integer.MAX_VALUE) {
            CtonLibrary.INSTANCE.free_buffer(buffer.data);
            return new byte[0];
        }
        
        try {
            return buffer.data.getByteArray(0, (int) buffer.size);
        } finally {
            CtonLibrary.INSTANCE.free_buffer(buffer.data);
        }
    }
    
    /**
     * Десеріалізувати BOC з бінарного представлення
     * @param data бінарні дані BOC
     * @return об'єкт Boc
     */
    public static Boc deserialize(byte[] data) {
        Pointer bocPtr = CtonLibrary.INSTANCE.boc_deserialize(data, data.length);
        if (bocPtr == null) {
            throw new IllegalArgumentException("Failed to deserialize BOC");
        }
        return new Boc(bocPtr);
    }
    
    /**
     * Отримати кореневу комірку
     * @return коренева комірка
     */
    public Cell getRoot() {
        if (closed) {
            throw new IllegalStateException("Boc has been closed");
        }
        Pointer cellPtr = CtonLibrary.INSTANCE.boc_get_root(nativeBoc);
        return new Cell(cellPtr);
    }
    
    /**
     * Встановити кореневу комірку
     * @param root нова коренева комірка
     */
    public void setRoot(Cell root) {
        if (closed) {
            throw new IllegalStateException("Boc has been closed");
        }
        CtonLibrary.INSTANCE.boc_set_root(nativeBoc, root.nativeCell);
    }
    
    /**
     * Закрити об'єкт і звільнити нативні ресурси
     */
    @Override
    public void close() {
        if (closed) {
            return;
        }
        closed = true;
        nativeBoc = null;
        if (cleanable != null) {
            cleanable.clean();
        }
    }
}