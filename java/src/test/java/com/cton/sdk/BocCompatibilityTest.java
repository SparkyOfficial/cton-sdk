// BocCompatibilityTest.java - хеш реальної транзакції з tonscan/toncenter
// Author: Андрій Будильников (Sparky)

package com.cton.sdk;

import java.util.Base64;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assumptions.assumeTrue;
import org.junit.jupiter.api.Test;

/**
 * BoC тіла повідомлення і транзакції з toncenter.
 * Це та сама транзакція, яку показує tonscan для USDT master
 * EQCxE6mUtQJKFnGfaROTKOt1lZbDiiX1kCixRv7Nw2Id_sDs, lt 111453883000007.
 * Хеші взято з полів body_hash і transaction_id.hash.
 */
public class BocCompatibilityTest {
    private static final String BODY_BOC =
        "te6cckEBAQEAMAAAWyx2uXNLYc1EQdAnb4AVtZJjqkvFI1sc/tkQFvkhzJVuiaJto/FuFqCUVLEj8Wg5wNLH";
    private static final String BODY_HASH =
        "1kqGOvV0byKR5ye/kR+l6MpkILJvVKA8MROQJc+IYOc=";
    private static final String TX_BOC =
        "te6cckECCwEAAlkAA7V7ETqZS1AkoWcZ9pE5Mo63WVlsOKJfWQKLFG/s3DYh3+AABlXeEa5McPZY/mDbr5eSOVqYl46VF1ShvnEl1NQuZCBtfkbpy3owAAZVyOQK2HatjhoQADRg229oAQIDAgHgBAUAgnIdLAj0a/ZA99vGh4MN4Dd1gK6FOP7zQXYvh/NKTjE8RxOXP1tKkI2o7LnVViAP6G1Z4y3rJqLqHe8ANf3ueD7DAhUMYgjwB+aYYMt70QkKAa1IAVtZJjqkvFI1sc/tkQFvkhzJVuiaJto/FuFqCUVLEj8XACxE6mUtQJKFnGfaROTKOt1lZbDiiX1kCixRv7Nw2Id/jwB+aAX8WAAAyrvCNcmM1bHDQsAGAQHfBwBbLHa5c0thzURB0CdvgBW1kmOqS8UjWxz+2RAW+SHMlW6Jom2j8W4WoJRUsSPxaAGtSAFiJ1MpagSULOM+0icmUdbrKy2HFEvrIFFijf2bhsQ7/QArayTHVJeKRrY5/bIgLfJDmSrdE0TbR+LcLUEoqWJH4s7hGewF/FgAAMq7wjXJkNWxw0LACABb0XNUAEthzURB0CdvgBr1HuuUmlC/kIlJu145geBw5FpVj2RNaeCxAzsh9oSCiACeQw1sC4cEAAAAAAAAAACHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABtyYC+oMn8VAAAAAAAAgAAAAAAAk6WEbxbQdLgT54WAnNFK9iYWkRsuDhrwq69sy5Wz0Y0QJAg7Kjikok=";
    private static final String TX_HASH =
        "uzGeqM7xh/lnzd7C6jrC3XGIsKpfwSrbVAndDckFPjs=";

    @Test
    public void messageBodyHashMatchesExplorer() {
        assumeNativeLibrary();
        assertStableHash(BODY_BOC, BODY_HASH);
    }

    @Test
    public void transactionHashMatchesExplorer() {
        assumeNativeLibrary();
        assertStableHash(TX_BOC, TX_HASH);
    }

    private static void assumeNativeLibrary() {
        boolean present;
        try {
            System.loadLibrary("cton-sdk-core");
            present = true;
        } catch (UnsatisfiedLinkError error) {
            present = false;
        }
        assumeTrue(present, "cton-sdk-core is not on java.library.path");
    }

    private static void assertStableHash(String bocBase64, String hashBase64) {
        byte[] raw = Base64.getDecoder().decode(bocBase64);
        byte[] expected = Base64.getDecoder().decode(hashBase64);
        try (Boc boc = Boc.deserialize(raw)) {
            byte[] hash = boc.getRoot().representationHash();
            assertArrayEquals(expected, hash);
            try (Boc again = Boc.deserialize(boc.serialize(true, true))) {
                assertArrayEquals(expected, again.getRoot().representationHash());
            }
        }
    }
}
