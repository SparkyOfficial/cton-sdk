// compat_test.cpp - Slice і сумісність BoC з реальними комірками TON
// Транзакція взята з toncenter (той самий BoC, що показує tonscan.org)
// для USDT jetton master EQCxE6mUtQJKFnGfaROTKOt1lZbDiiX1kCixRv7Nw2Id_sDs,
// lt 111453883000007.

#include "../include/Boc.h"
#include "../include/Cell.h"
#include "../include/Slice.h"
#include "../include/Address.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace cton;

static int failures = 0;

static void expect(bool ok, const std::string& message) {
    if (!ok) {
        std::cerr << "FAIL: " << message << std::endl;
        ++failures;
    }
}

static int b64Value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+' || c == '-') return 62;
    if (c == '/' || c == '_') return 63;
    return -1;
}

static std::vector<uint8_t> decodeBase64(const std::string& in) {
    std::vector<uint8_t> out;
    int val = 0;
    int valb = -8;
    for (char c : in) {
        if (c == '=' || c == '\n' || c == '\r' || c == ' ') {
            continue;
        }
        int d = b64Value(c);
        if (d < 0) {
            throw std::runtime_error("invalid base64");
        }
        val = (val << 6) + d;
        valb += 6;
        if (valb >= 0) {
            out.push_back(static_cast<uint8_t>((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

static std::string toHex(const std::vector<uint8_t>& bytes) {
    static const char* hex = "0123456789abcdef";
    std::string out;
    out.resize(bytes.size() * 2);
    for (size_t i = 0; i < bytes.size(); ++i) {
        out[i * 2] = hex[bytes[i] >> 4];
        out[i * 2 + 1] = hex[bytes[i] & 0x0F];
    }
    return out;
}

static void expectHash(const std::vector<uint8_t>& actual, const std::string& expectedB64, const std::string& label) {
    auto expected = decodeBase64(expectedB64);
    if (actual != expected) {
        std::cerr << "FAIL: " << label << "\n  actual   " << toHex(actual)
                  << "\n  expected " << toHex(expected) << std::endl;
        ++failures;
    }
}

static void testSha256Empty() {
    Cell cell;
    // Порожня звичайна комірка: d1=0 d2=0, sha256("") від двох нульових байтів, не від порожнього буфера.
    auto hash = cell.representationHash();
    expect(hash.size() == 32, "empty cell hash length");
    expect(hash != std::vector<uint8_t>(32, 0), "empty cell hash is not the openssl-missing stub");
}

static void testSliceRoundTrip() {
    std::vector<uint8_t> hash(32, 0xAB);
    hash[0] = 0x11;
    hash[31] = 0x22;

    CellBuilder child;
    child.storeUInt(8, 0x42);

    CellBuilder body;
    body.storeUInt(32, 0x0f8a7ea5);
    body.storeInt(16, -100);
    body.storeBytes({0x01, 0x02, 0x03, 0x04});
    body.storeUInt(4, 3);
    body.storeUInt(8, 0x01);
    body.storeUInt(8, 0x5F);
    body.storeUInt(8, 0x90);
    body.storeUInt(2, 0); // addr_none
    body.storeUInt(2, 2); // addr_std
    body.storeUInt(1, 0); // no anycast
    body.storeInt(8, -1);
    body.storeBytes(hash);
    body.storeRef(child.build());

    auto cell = body.build();
    Slice slice(*cell);
    expect(slice.loadUInt(32) == 0x0f8a7ea5ULL, "opcode");
    expect(slice.loadInt(16) == -100, "negative int");
    auto bytes = slice.loadBytes(4);
    expect(bytes.size() == 4 && bytes[0] == 1 && bytes[3] == 4, "bytes");
    auto coins = slice.loadCoins();
    expect(coins.size() == 3 && coins[0] == 0x01 && coins[1] == 0x5F && coins[2] == 0x90, "coins");
    expect(!slice.loadAddress().has_value(), "addr_none");
    auto address = slice.loadAddress();
    expect(address.has_value(), "addr_std present");
    if (address) {
        expect(address->getWorkchain() == -1, "workchain -1");
        expect(address->getHashPart() == hash, "address hash");
    }
    auto ref = slice.loadRef();
    expect(ref && ref->getBitSize() == 8, "ref bit size");
    Slice refSlice(*ref);
    expect(refSlice.loadUInt(8) == 0x42, "ref payload");
    expect(slice.remainingBits() == 0 && slice.remainingRefs() == 0, "slice exhausted");
}

static void testBuiltCellRoundTrip() {
    CellBuilder child;
    child.storeUInt(32, 7);
    CellBuilder root;
    root.storeUInt(9, 0x15A); // не кратно байту
    root.storeRef(child.build());
    auto cell = root.build();
    auto before = cell->representationHash();

    Boc boc(cell);
    auto encoded = boc.serialize(false, true);
    auto decoded = Boc::deserialize(encoded);
    auto after = decoded.getRoot()->representationHash();
    expect(before == after, "built cell hash survives serialize without index");

    auto encodedIdx = boc.serialize(true, true);
    auto decodedIdx = Boc::deserialize(encodedIdx);
    expect(decodedIdx.getRoot()->representationHash() == before, "built cell hash survives serialize with index");

    Slice slice(*decoded.getRoot());
    expect(slice.loadUInt(9) == 0x15A, "unaligned uint survives boc");
    Slice nested(*slice.loadRef());
    expect(nested.loadUInt(32) == 7, "child survives boc");
}

static void testRealBoc(const char* bocB64, const char* hashB64, const char* label) {
    auto bytes = decodeBase64(bocB64);
    Boc decoded = Boc::deserialize(bytes);
    auto root = decoded.getRoot();
    expect(static_cast<bool>(root), std::string(label) + " has a root");
    if (!root) {
        return;
    }
    auto hash = root->representationHash();
    expectHash(hash, hashB64, std::string(label) + " representation hash");

    auto again = Boc::deserialize(decoded.serialize(true, true));
    auto hash2 = again.getRoot()->representationHash();
    expect(hash == hash2, std::string(label) + " hash is stable after deserialize -> serialize -> deserialize");

    auto plain = Boc::deserialize(decoded.serialize(false, false));
    expect(plain.getRoot()->representationHash() == hash,
           std::string(label) + " hash is stable without index and crc");
}

int main() {
    try {
        testSha256Empty();
        testSliceRoundTrip();
        testBuiltCellRoundTrip();

        // Тіло вхідного повідомлення. body_hash з toncenter / tonscan.
        testRealBoc(
            "te6cckEBAQEAMAAAWyx2uXNLYc1EQdAnb4AVtZJjqkvFI1sc/tkQFvkhzJVuiaJto/FuFqCUVLEj8Wg5wNLH",
            "1kqGOvV0byKR5ye/kR+l6MpkILJvVKA8MROQJc+IYOc=",
            "message body");

        // Повний BoC транзакції. transaction_id.hash — хеш кореневої комірки.
        testRealBoc(
            "te6cckECCwEAAlkAA7V7ETqZS1AkoWcZ9pE5Mo63WVlsOKJfWQKLFG/s3DYh3+AABlXeEa5McPZY/mDbr5eSOVqYl46VF1ShvnEl1NQuZCBtfkbpy3owAAZVyOQK2HatjhoQADRg229oAQIDAgHgBAUAgnIdLAj0a/ZA99vGh4MN4Dd1gK6FOP7zQXYvh/NKTjE8RxOXP1tKkI2o7LnVViAP6G1Z4y3rJqLqHe8ANf3ueD7DAhUMYgjwB+aYYMt70QkKAa1IAVtZJjqkvFI1sc/tkQFvkhzJVuiaJto/FuFqCUVLEj8XACxE6mUtQJKFnGfaROTKOt1lZbDiiX1kCixRv7Nw2Id/jwB+aAX8WAAAyrvCNcmM1bHDQsAGAQHfBwBbLHa5c0thzURB0CdvgBW1kmOqS8UjWxz+2RAW+SHMlW6Jom2j8W4WoJRUsSPxaAGtSAFiJ1MpagSULOM+0icmUdbrKy2HFEvrIFFijf2bhsQ7/QArayTHVJeKRrY5/bIgLfJDmSrdE0TbR+LcLUEoqWJH4s7hGewF/FgAAMq7wjXJkNWxw0LACABb0XNUAEthzURB0CdvgBr1HuuUmlC/kIlJu145geBw5FpVj2RNaeCxAzsh9oSCiACeQw1sC4cEAAAAAAAAAACHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABtyYC+oMn8VAAAAAAAAgAAAAAAAk6WEbxbQdLgT54WAnNFK9iYWkRsuDhrwq69sy5Wz0Y0QJAg7Kjikok=",
            "uzGeqM7xh/lnzd7C6jrC3XGIsKpfwSrbVAndDckFPjs=",
            "transaction");
    } catch (const std::exception& ex) {
        std::cerr << "FAIL: exception " << ex.what() << std::endl;
        return 1;
    }

    if (failures != 0) {
        std::cerr << failures << " failure(s)" << std::endl;
        return 1;
    }
    std::cout << "Slice and real BoC compatibility checks passed" << std::endl;
    return 0;
}
