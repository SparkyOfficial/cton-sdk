// NativeInterface.h - C-стильовий інтерфейс для JNA
// Author: Андрій Будильников (Sparky)
// C-style interface for JNA bindings
// C-стильовой интерфейс для JNA биндингов

#ifndef CTON_SDK_CORE_NATIVE_INTERFACE_H
#define CTON_SDK_CORE_NATIVE_INTERFACE_H

#include "Cell.h"
#include "Address.h"
#include "Crypto.h"
#include "Boc.h"

// Cell functions
extern "C" {
    void* cell_create();
    void cell_destroy(void* cell);
    bool cell_store_uint(void* cell, int bits, uint64_t value);
    bool cell_store_int(void* cell, int bits, int64_t value);
    bool cell_store_bytes(void* cell, const uint8_t* data, int length);
    bool cell_store_ref(void* cell, void* refCell);
    int cell_get_data(void* cell, uint8_t* buffer, int bufferSize);
    int cell_get_bit_size(void* cell);
    int cell_get_refs_count(void* cell);
    void* cell_get_ref(void* cell, int index);

    // Address functions
    void* address_create();
    void* address_create_from_string(const char* addressStr);
    void address_destroy(void* address);
    int8_t address_get_workchain(void* address);
    void address_get_hash_part(void* address, uint8_t* hashPart);
    char* address_to_raw(void* address);
    char* address_to_user_friendly(void* address, bool isBounceable, bool isTestOnly);
    bool address_is_valid(void* address);
    void address_set_workchain(void* address, int8_t workchain);
    void address_set_hash_part(void* address, const uint8_t* hashPart);

    // Private key functions
    void* private_key_create();
    void* private_key_create_from_data(const uint8_t* keyData, int length);
    void private_key_destroy(void* privateKey);
    void* private_key_generate();
    int private_key_get_data(void* privateKey, uint8_t* buffer, int bufferSize);
    void* private_key_get_public_key(void* privateKey);

    // Public key functions
    void* public_key_create();
    void* public_key_create_from_data(const uint8_t* keyData, int length);
    void public_key_destroy(void* publicKey);
    int public_key_get_data(void* publicKey, uint8_t* buffer, int bufferSize);
    bool public_key_verify_signature(void* publicKey, const uint8_t* message, int messageLen, 
                                    const uint8_t* signature, int signatureLen);

    // Crypto functions
    void* crypto_sign(void* privateKey, const uint8_t* message, int messageLen);
    bool crypto_verify(void* publicKey, const uint8_t* message, int messageLen, 
                      const uint8_t* signature, int signatureLen);
    char** crypto_generate_mnemonic();
    void* crypto_mnemonic_to_private_key(char** mnemonic);

    // BOC functions
    void* boc_create();
    void* boc_create_with_root(void* rootCell);
    void boc_destroy(void* boc);
    void* boc_serialize(void* boc, bool hasIdx, bool hashCRC);
    void* boc_deserialize(const uint8_t* data, int length);
    void* boc_get_root(void* boc);
    void boc_set_root(void* boc, void* rootCell);
    
    // Memory management functions
    void free_string(char* str);
    void free_string_array(char** strArray);
}

#endif // CTON_SDK_CORE_NATIVE_INTERFACE_H