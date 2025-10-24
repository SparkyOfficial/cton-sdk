// NativeInterface.cpp - Реалізація C-стильного інтерфейсу для JNA
// Author: Андрій Будильников (Sparky)
// Implementation of C-style interface for JNA bindings
// Реализация C-стильного интерфейса для JNA биндингов

#include "../include/NativeInterface.h"
#include "../include/Cell.h"
#include "../include/Address.h"
#include "../include/Crypto.h"
#include "../include/Boc.h"
#include <cstring>
#include <cstdlib>
#include <stdexcept>

using namespace cton;

// Helper function to convert C++ string to C string
char* string_to_cstr(const std::string& str) {
    if (str.empty()) {
        return nullptr;
    }
    
    char* cstr = (char*)malloc(str.length() + 1);
    if (cstr) {
        std::strcpy(cstr, str.c_str());
    }
    return cstr;
}

// Cell functions
void* cell_create() {
    try {
        return new Cell();
    } catch (...) {
        return nullptr;
    }
}

void cell_destroy(void* cell) {
    if (cell) {
        delete static_cast<Cell*>(cell);
    }
}

bool cell_store_uint(void* cell, int bits, uint64_t value) {
    if (!cell) {
        return false;
    }
    
    try {
        static_cast<Cell*>(cell)->storeUInt(bits, value);
        return true;
    } catch (...) {
        return false;
    }
}

bool cell_store_int(void* cell, int bits, int64_t value) {
    if (!cell) {
        return false;
    }
    
    try {
        static_cast<Cell*>(cell)->storeInt(bits, value);
        return true;
    } catch (...) {
        return false;
    }
}

bool cell_store_bytes(void* cell, const uint8_t* data, int length) {
    if (!cell || !data || length <= 0) {
        return false;
    }
    
    try {
        std::vector<uint8_t> vec(data, data + length);
        static_cast<Cell*>(cell)->storeBytes(vec);
        return true;
    } catch (...) {
        return false;
    }
}

bool cell_store_ref(void* cell, void* refCell) {
    if (!cell || !refCell) {
        return false;
    }
    
    try {
        // Convert void* to shared_ptr<Cell>
        std::shared_ptr<Cell> ref(static_cast<Cell*>(refCell));
        return static_cast<Cell*>(cell)->addReference(ref);
    } catch (...) {
        return false;
    }
}

int cell_get_data(void* cell, uint8_t* buffer, int bufferSize) {
    if (!cell || !buffer || bufferSize <= 0) {
        return -1;
    }
    
    try {
        auto data = static_cast<Cell*>(cell)->getData();
        int copySize = std::min(bufferSize, (int)data.size());
        std::memcpy(buffer, data.data(), copySize);
        return copySize;
    } catch (...) {
        return -1;
    }
}

int cell_get_bit_size(void* cell) {
    if (!cell) {
        return -1;
    }
    
    try {
        return static_cast<Cell*>(cell)->getBitSize();
    } catch (...) {
        return -1;
    }
}

int cell_get_refs_count(void* cell) {
    if (!cell) {
        return -1;
    }
    
    try {
        return static_cast<Cell*>(cell)->getReferencesCount();
    } catch (...) {
        return -1;
    }
}

void* cell_get_ref(void* cell, int index) {
    if (!cell || index < 0) {
        return nullptr;
    }
    
    try {
        std::shared_ptr<Cell> ref = static_cast<Cell*>(cell)->getReference(index);
        if (ref) {
            // Return the raw pointer to the Cell object
            // Note: This is a simplification. In a real implementation, we would need
            // to handle the shared_ptr properly to avoid memory issues.
            return ref.get();
        }
        return nullptr;
    } catch (...) {
        return nullptr;
    }
}

// Address functions
void* address_create() {
    try {
        return new Address();
    } catch (...) {
        return nullptr;
    }
}

void* address_create_from_string(const char* addressStr) {
    if (!addressStr) {
        return nullptr;
    }
    
    try {
        return new Address(std::string(addressStr));
    } catch (...) {
        return nullptr;
    }
}

void address_destroy(void* address) {
    if (address) {
        delete static_cast<Address*>(address);
    }
}

int8_t address_get_workchain(void* address) {
    if (!address) {
        return 0;
    }
    
    try {
        return static_cast<Address*>(address)->getWorkchain();
    } catch (...) {
        return 0;
    }
}

void address_get_hash_part(void* address, uint8_t* hashPart) {
    if (!address || !hashPart) {
        return;
    }
    
    try {
        auto hash = static_cast<Address*>(address)->getHashPart();
        std::memcpy(hashPart, hash.data(), std::min(hash.size(), (size_t)32));
    } catch (...) {
        // Нічого не робимо
        // Do nothing
        // Ничего не делаем
    }
}

char* address_to_raw(void* address) {
    if (!address) {
        return nullptr;
    }
    
    try {
        std::string raw = static_cast<Address*>(address)->toRaw();
        return string_to_cstr(raw);
    } catch (...) {
        return nullptr;
    }
}

char* address_to_user_friendly(void* address, bool isBounceable, bool isTestOnly) {
    if (!address) {
        return nullptr;
    }
    
    try {
        std::string uf = static_cast<Address*>(address)->toUserFriendly(isBounceable, isTestOnly);
        return string_to_cstr(uf);
    } catch (...) {
        return nullptr;
    }
}

bool address_is_valid(void* address) {
    if (!address) {
        return false;
    }
    
    try {
        return static_cast<Address*>(address)->isValid();
    } catch (...) {
        return false;
    }
}

void address_set_workchain(void* address, int8_t workchain) {
    if (!address) {
        return;
    }
    
    try {
        static_cast<Address*>(address)->setWorkchain(workchain);
    } catch (...) {
        // Нічого не робимо
        // Do nothing
        // Ничего не делаем
    }
}

void address_set_hash_part(void* address, const uint8_t* hashPart) {
    if (!address || !hashPart) {
        return;
    }
    
    try {
        std::vector<uint8_t> hash(hashPart, hashPart + 32);
        static_cast<Address*>(address)->setHashPart(hash);
    } catch (...) {
        // Нічого не робимо
        // Do nothing
        // Ничего не делаем
    }
}

// Private key functions
void* private_key_create() {
    try {
        return new PrivateKey();
    } catch (...) {
        return nullptr;
    }
}

void* private_key_create_from_data(const uint8_t* keyData, int length) {
    if (!keyData || length != 32) {
        return nullptr;
    }
    
    try {
        std::vector<uint8_t> data(keyData, keyData + length);
        return new PrivateKey(data);
    } catch (...) {
        return nullptr;
    }
}

void private_key_destroy(void* privateKey) {
    if (privateKey) {
        delete static_cast<PrivateKey*>(privateKey);
    }
}

void* private_key_generate() {
    try {
        PrivateKey key = PrivateKey::generate();
        // Create a new PrivateKey object and return it
        return new PrivateKey(key);
    } catch (...) {
        return nullptr;
    }
}

int private_key_get_data(void* privateKey, uint8_t* buffer, int bufferSize) {
    if (!privateKey || !buffer || bufferSize < 32) {
        return -1;
    }
    
    try {
        auto data = static_cast<PrivateKey*>(privateKey)->getData();
        std::memcpy(buffer, data.data(), 32);
        return 32;
    } catch (...) {
        return -1;
    }
}

void* private_key_get_public_key(void* privateKey) {
    if (!privateKey) {
        return nullptr;
    }
    
    try {
        PublicKey publicKey = static_cast<PrivateKey*>(privateKey)->getPublicKey();
        // Create a new PublicKey object and return it
        return new PublicKey(publicKey);
    } catch (...) {
        return nullptr;
    }
}

// Public key functions
void* public_key_create() {
    try {
        return new PublicKey();
    } catch (...) {
        return nullptr;
    }
}

void* public_key_create_from_data(const uint8_t* keyData, int length) {
    if (!keyData || length != 32) {
        return nullptr;
    }
    
    try {
        std::vector<uint8_t> data(keyData, keyData + length);
        return new PublicKey(data);
    } catch (...) {
        return nullptr;
    }
}

void public_key_destroy(void* publicKey) {
    if (publicKey) {
        delete static_cast<PublicKey*>(publicKey);
    }
}

int public_key_get_data(void* publicKey, uint8_t* buffer, int bufferSize) {
    if (!publicKey || !buffer || bufferSize < 32) {
        return -1;
    }
    
    try {
        auto data = static_cast<PublicKey*>(publicKey)->getData();
        std::memcpy(buffer, data.data(), 32);
        return 32;
    } catch (...) {
        return -1;
    }
}

bool public_key_verify_signature(void* publicKey, const uint8_t* message, int messageLen, 
                                const uint8_t* signature, int signatureLen) {
    if (!publicKey || !message || messageLen <= 0 || !signature || signatureLen != 64) {
        return false;
    }
    
    try {
        std::vector<uint8_t> msg(message, message + messageLen);
        std::vector<uint8_t> sig(signature, signature + signatureLen);
        return static_cast<PublicKey*>(publicKey)->verifySignature(msg, sig);
    } catch (...) {
        return false;
    }
}

// Crypto functions
void* crypto_sign(void* privateKey, const uint8_t* message, int messageLen) {
    if (!privateKey || !message || messageLen <= 0) {
        return nullptr;
    }
    
    try {
        std::vector<uint8_t> msg(message, message + messageLen);
        std::vector<uint8_t> signature = Crypto::sign(*static_cast<PrivateKey*>(privateKey), msg);
        
        // Allocate memory for the signature and copy it
        uint8_t* result = (uint8_t*)malloc(signature.size());
        if (result) {
            std::memcpy(result, signature.data(), signature.size());
        }
        return result;
    } catch (...) {
        return nullptr;
    }
}

bool crypto_verify(void* publicKey, const uint8_t* message, int messageLen, 
                  const uint8_t* signature, int signatureLen) {
    if (!publicKey || !message || messageLen <= 0 || !signature || signatureLen != 64) {
        return false;
    }
    
    try {
        std::vector<uint8_t> msg(message, message + messageLen);
        std::vector<uint8_t> sig(signature, signature + signatureLen);
        return Crypto::verify(*static_cast<PublicKey*>(publicKey), msg, sig);
    } catch (...) {
        return false;
    }
}

char** crypto_generate_mnemonic() {
    try {
        auto words = Crypto::generateMnemonic();
        
        // Allocate memory for the array of C-strings
        char** result = (char**)malloc((words.size() + 1) * sizeof(char*));
        if (!result) {
            return nullptr;
        }
        
        // Convert each word to a C-string
        for (size_t i = 0; i < words.size(); ++i) {
            result[i] = string_to_cstr(words[i]);
            if (!result[i]) {
                // If allocation fails, clean up and return nullptr
                for (size_t j = 0; j < i; ++j) {
                    free(result[j]);
                }
                free(result);
                return nullptr;
            }
        }
        
        // Null-terminate the array
        result[words.size()] = nullptr;
        return result;
    } catch (...) {
        return nullptr;
    }
}

void* crypto_mnemonic_to_private_key(char** mnemonic) {
    if (!mnemonic) {
        return nullptr;
    }
    
    try {
        // Convert array of C-strings to vector of strings
        std::vector<std::string> words;
        for (int i = 0; mnemonic[i] != nullptr; ++i) {
            words.push_back(std::string(mnemonic[i]));
        }
        
        PrivateKey key = Crypto::mnemonicToPrivateKey(words);
        // Create a new PrivateKey object and return it
        return new PrivateKey(key);
    } catch (...) {
        return nullptr;
    }
}

// BOC functions
void* boc_create() {
    try {
        return new Boc();
    } catch (...) {
        return nullptr;
    }
}

void* boc_create_with_root(void* rootCell) {
    if (!rootCell) {
        return nullptr;
    }
    
    try {
        // Convert void* to shared_ptr<Cell>
        std::shared_ptr<Cell> cell(static_cast<Cell*>(rootCell));
        // Create a new Boc object with the root cell
        return new Boc(cell);
    } catch (...) {
        return nullptr;
    }
}

void boc_destroy(void* boc) {
    if (boc) {
        delete static_cast<Boc*>(boc);
    }
}

void* boc_serialize(void* boc, bool hasIdx, bool hashCRC) {
    if (!boc) {
        return nullptr;
    }
    
    try {
        std::vector<uint8_t> data = static_cast<Boc*>(boc)->serialize(hasIdx, hashCRC);
        
        // Allocate memory for the serialized data and copy it
        uint8_t* result = (uint8_t*)malloc(data.size());
        if (result) {
            std::memcpy(result, data.data(), data.size());
        }
        return result;
    } catch (...) {
        return nullptr;
    }
}

void* boc_deserialize(const uint8_t* data, int length) {
    if (!data || length <= 0) {
        return nullptr;
    }
    
    try {
        std::vector<uint8_t> vec(data, data + length);
        Boc boc = Boc::deserialize(vec);
        // Create a new Boc object and return it
        return new Boc(boc);
    } catch (...) {
        return nullptr;
    }
}

void* boc_get_root(void* boc) {
    if (!boc) {
        return nullptr;
    }
    
    try {
        std::shared_ptr<Cell> root = static_cast<Boc*>(boc)->getRoot();
        // Return the raw pointer to the Cell object
        // Note: This is a simplification. In a real implementation, we would need
        // to handle the shared_ptr properly to avoid memory issues.
        if (root) {
            return root.get();
        }
        return nullptr;
    } catch (...) {
        return nullptr;
    }
}

void boc_set_root(void* boc, void* rootCell) {
    if (!boc || !rootCell) {
        return;
    }
    
    try {
        // Convert void* to shared_ptr<Cell>
        std::shared_ptr<Cell> cell(static_cast<Cell*>(rootCell));
        static_cast<Boc*>(boc)->setRoot(cell);
    } catch (...) {
        // Нічого не робимо
        // Do nothing
        // Ничего не делаем
    }
}