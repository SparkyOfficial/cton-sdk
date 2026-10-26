// SliceTest.java - читання бітів, адрес, монет і посилань
// Author: Андрій Будильников (Sparky)

package com.cton.sdk;

import java.math.BigInteger;
import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assumptions.assumeTrue;
import org.junit.jupiter.api.Test;

public class SliceTest {
    @Test
    public void readsWhatCellBuilderStored() {
        boolean present;
        try {
            System.loadLibrary("cton-sdk-core");
            present = true;
        } catch (UnsatisfiedLinkError error) {
            present = false;
        }
        assumeTrue(present, "cton-sdk-core is not on java.library.path");

        byte[] hash = new byte[32];
        Arrays.fill(hash, (byte) 0xAB);
        hash[0] = 0x11;
        hash[31] = 0x22;
        Address owner = new Address();
        owner.setWorkchain((byte) -1);
        owner.setHashPart(hash);

        Cell child = new CellBuilder().storeUInt(8, 0x42).build();
        Cell cell = new CellBuilder()
            .storeUInt(32, 0x0f8a7ea5L)
            .storeInt(16, -100)
            .storeBytes(new byte[]{1, 2, 3, 4})
            .storeCoins(new BigInteger("90000"))
            .storeAddress(null)
            .storeAddress(owner)
            .storeRef(child)
            .build();

        try (Slice slice = cell.toSlice()) {
            assertEquals(0x0f8a7ea5L, slice.loadUInt(32));
            assertEquals(-100L, slice.loadInt(16));
            assertArrayEquals(new byte[]{1, 2, 3, 4}, slice.loadBytes(4));
            assertEquals(new BigInteger("90000"), slice.loadCoins());
            assertNull(slice.loadAddress());
            Address loaded = slice.loadAddress();
            assertEquals(-1, loaded.getWorkchain());
            assertArrayEquals(hash, loaded.getHashPart());
            try (Cell ref = slice.loadRef(); Slice refSlice = ref.toSlice()) {
                assertEquals(0x42L, refSlice.loadUInt(8));
            }
            assertEquals(0, slice.remainingBits());
            assertEquals(0, slice.remainingRefs());
        }
    }
}
