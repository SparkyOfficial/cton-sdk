// Slice.java - Java обгортка курсора читання комірки TON
// Author: Андрій Будильников (Sparky)

package com.cton.sdk;

import java.io.Closeable;
import java.lang.ref.Cleaner;
import java.math.BigInteger;
import java.util.Arrays;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.LongByReference;
import com.sun.jna.ptr.PointerByReference;

/**
 * Курсор по бітах і посиланнях комірки.
 * Нативна пам'ять звільняється в {@link #close()} або збирачем сміття.
 */
public class Slice implements Closeable {
    public interface CtonLibrary extends Library {
        CtonLibrary INSTANCE = (CtonLibrary) Native.load("cton-sdk-core", CtonLibrary.class);

        Pointer slice_from_cell(Pointer cell);
        void slice_destroy(Pointer slice);
        boolean slice_load_uint(Pointer slice, int bits, LongByReference out);
        boolean slice_load_int(Pointer slice, int bits, LongByReference out);
        int slice_load_bytes(Pointer slice, int len, byte[] out, int outLen);
        int slice_load_address(Pointer slice, PointerByReference out);
        int slice_load_coins(Pointer slice, byte[] out, int outLen);
        Pointer slice_load_ref(Pointer slice);
        int slice_remaining_bits(Pointer slice);
        int slice_remaining_refs(Pointer slice);
    }

    private Pointer nativeSlice;
    private boolean closed = false;
    private final Cleaner.Cleanable cleanable;

    private Slice(Pointer nativeSlice) {
        this.nativeSlice = nativeSlice;
        this.cleanable = NativeResources.track(this, nativeSlice, CtonLibrary.INSTANCE::slice_destroy);
    }

    /**
     * Почати читання комірки. Slice отримує власну копію дерева.
     */
    public static Slice from(Cell cell) {
        if (cell == null || cell.nativeCell == null) {
            throw new IllegalArgumentException("Cell is required");
        }
        Pointer slice = CtonLibrary.INSTANCE.slice_from_cell(cell.nativeCell);
        if (slice == null) {
            throw new IllegalArgumentException("Failed to create slice");
        }
        return new Slice(slice);
    }

    public long loadUInt(int bits) {
        ensureOpen();
        LongByReference out = new LongByReference();
        if (!CtonLibrary.INSTANCE.slice_load_uint(nativeSlice, bits, out)) {
            throw new IllegalStateException("Failed to load uint of " + bits + " bits");
        }
        return out.getValue();
    }

    public long loadInt(int bits) {
        ensureOpen();
        LongByReference out = new LongByReference();
        if (!CtonLibrary.INSTANCE.slice_load_int(nativeSlice, bits, out)) {
            throw new IllegalStateException("Failed to load int of " + bits + " bits");
        }
        return out.getValue();
    }

    public byte[] loadBytes(int length) {
        ensureOpen();
        if (length < 0) {
            throw new IllegalArgumentException("length must be non-negative");
        }
        byte[] out = new byte[length];
        int read = CtonLibrary.INSTANCE.slice_load_bytes(nativeSlice, length, out, out.length);
        if (read != length) {
            throw new IllegalStateException("Failed to load " + length + " bytes");
        }
        return out;
    }

    /**
     * @return адреса addr_std або null для addr_none
     */
    public Address loadAddress() {
        ensureOpen();
        PointerByReference out = new PointerByReference();
        int code = CtonLibrary.INSTANCE.slice_load_address(nativeSlice, out);
        if (code == 0) {
            return null;
        }
        if (code < 0 || out.getValue() == null) {
            throw new IllegalStateException("Failed to load address");
        }
        return new Address(out.getValue());
    }

    public BigInteger loadCoins() {
        ensureOpen();
        byte[] magnitude = new byte[16];
        int length = CtonLibrary.INSTANCE.slice_load_coins(nativeSlice, magnitude, magnitude.length);
        if (length < 0) {
            throw new IllegalStateException("Failed to load coins");
        }
        if (length == 0) {
            return BigInteger.ZERO;
        }
        return new BigInteger(1, Arrays.copyOf(magnitude, length));
    }

    public Cell loadRef() {
        ensureOpen();
        Pointer ref = CtonLibrary.INSTANCE.slice_load_ref(nativeSlice);
        if (ref == null) {
            throw new IllegalStateException("Failed to load cell reference");
        }
        return new Cell(ref);
    }

    public int remainingBits() {
        ensureOpen();
        return CtonLibrary.INSTANCE.slice_remaining_bits(nativeSlice);
    }

    public int remainingRefs() {
        ensureOpen();
        return CtonLibrary.INSTANCE.slice_remaining_refs(nativeSlice);
    }

    private void ensureOpen() {
        if (closed) {
            throw new IllegalStateException("Slice has been closed");
        }
    }

    @Override
    public void close() {
        if (closed) {
            return;
        }
        closed = true;
        nativeSlice = null;
        if (cleanable != null) {
            cleanable.clean();
        }
    }
}
