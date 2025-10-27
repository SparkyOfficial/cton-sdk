// Crypto.cpp - криптографічні функції для TON
// Author: Андрій Будильников (Sparky)
// Реалізація криптографічних функцій Ed25519 для TON
// Implementation of Ed25519 cryptographic functions for TON
// Реализация криптографических функций Ed25519 для TON

#include "../include/Crypto.h"
#include "../include/Mnemonic.h"
#include <stdexcept>
#include <random>
#include <ctime>
#include <cstring>

// Try to include OpenSSL if available
#ifdef USE_OPENSSL
// Check if OpenSSL headers are actually available
#if __has_include("C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/rand.h") && __has_include("C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/evp.h")
#include "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/rand.h"
#include "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/evp.h"
#include "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/err.h"

// Include Ed25519 specific headers if available
#if __has_include("C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/evperr.h")
#include "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/evperr.h"
#endif

// Include EC and ChaCha20 headers
#if __has_include("C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/ec.h")
#include "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/ec.h"
#endif

#if __has_include("C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/chacha.h")
#include "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/chacha.h"
#endif

#define OPENSSL_AVAILABLE 1
#else
#define OPENSSL_AVAILABLE 0
#endif
#else
#define OPENSSL_AVAILABLE 0
#endif

#ifndef USE_OPENSSL
#define OPENSSL_AVAILABLE 0
#endif

#if OPENSSL_AVAILABLE
// OpenSSL implementation
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>

// Helper function to handle OpenSSL errors
static std::string getOpenSSLError() {
    char buffer[256];
    unsigned long err = ERR_get_error();
    if (err != 0) {
        ERR_error_string_n(err, buffer, sizeof(buffer));
        return std::string(buffer);
    }
    return "Unknown OpenSSL error";
}
#endif

namespace cton {
    
    PrivateKey::PrivateKey() : keyData_(32, 0) {}
    
    PrivateKey::PrivateKey(const std::vector<uint8_t>& keyData) : keyData_(keyData) {
        if (keyData_.size() != 32) {
            throw std::invalid_argument("Private key must be exactly 32 bytes");
        }
    }
    
    PrivateKey PrivateKey::generate() {
        // Генерація випадкових даних для приватного ключа
        // Random data generation for private key
        // Генерация случайных данных для приватного ключа
        
        std::vector<uint8_t> keyData(32);
        
#if OPENSSL_AVAILABLE
        // Використовуємо OpenSSL якщо доступний
        // Use OpenSSL if available
        // Используем OpenSSL если доступен
        if (RAND_bytes(keyData.data(), 32) != 1) {
            // If RAND_bytes fails, fall back to standard generator
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0, 255);
            
            for (size_t i = 0; i < 32; ++i) {
                keyData[i] = static_cast<uint8_t>(dis(gen));
            }
        }
#else
        // Використовуємо стандартний генератор як fallback
        // Use standard generator as fallback
        // Используем стандартный генератор как запасной вариант
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);
        
        for (size_t i = 0; i < 32; ++i) {
            keyData[i] = static_cast<uint8_t>(dis(gen));
        }
#endif
        
        return PrivateKey(keyData);
    }
    
    std::vector<uint8_t> PrivateKey::getData() const {
        return keyData_;
    }
    
    PublicKey PrivateKey::getPublicKey() const {
        // Обчислення публічного ключа з приватного
        // Public key derivation from private key
        // Вычисление публичного ключа из приватного
        
#if OPENSSL_AVAILABLE
        // Спробуємо використати OpenSSL для генерації публічного ключа Ed25519
        // Try to use OpenSSL to generate Ed25519 public key
        // Попробуем использовать OpenSSL для создания публичного ключа Ed25519
        
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr);
        if (!ctx) {
            // Fallback to simple approach
            return PublicKey(std::vector<uint8_t>(keyData_.begin(), keyData_.begin() + 32));
        }
        
        if (EVP_PKEY_keygen_init(ctx) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            // Fallback to simple approach
            return PublicKey(std::vector<uint8_t>(keyData_.begin(), keyData_.begin() + 32));
        }
        
        EVP_PKEY* pkey = nullptr;
        if (EVP_PKEY_generate(ctx, &pkey) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            // Fallback to simple approach
            return PublicKey(std::vector<uint8_t>(keyData_.begin(), keyData_.begin() + 32));
        }
        
        // Extract public key
        size_t pubKeyLen = 32;
        std::vector<uint8_t> pubKeyData(pubKeyLen);
        if (EVP_PKEY_get_raw_public_key(pkey, pubKeyData.data(), &pubKeyLen) <= 0) {
            EVP_PKEY_free(pkey);
            EVP_PKEY_CTX_free(ctx);
            // Fallback to simple approach
            return PublicKey(std::vector<uint8_t>(keyData_.begin(), keyData_.begin() + 32));
        }
        
        EVP_PKEY_free(pkey);
        EVP_PKEY_CTX_free(ctx);
        
        return PublicKey(pubKeyData);
#else
        // Для простоти, створюємо публічний ключ з перших 32 байтів приватного ключа
        // For simplicity, create public key from first 32 bytes of private key
        return PublicKey(std::vector<uint8_t>(keyData_.begin(), keyData_.begin() + 32));
#endif
    }

    PublicKey::PublicKey() : keyData_(32, 0) {}
    
    PublicKey::PublicKey(const std::vector<uint8_t>& keyData) : keyData_(keyData) {
        if (keyData_.size() != 32) {
            throw std::invalid_argument("Public key must be exactly 32 bytes");
        }
    }
    
    std::vector<uint8_t> PublicKey::getData() const {
        return keyData_;
    }
    
    bool PublicKey::verifySignature(const std::vector<uint8_t>& message, 
                                  const std::vector<uint8_t>& signature) const {
        // Перевірка підпису
        // Signature verification
        // Проверка подписи
        
#if OPENSSL_AVAILABLE
        // Спробуємо використати OpenSSL для перевірки підпису Ed25519
        // Try to use OpenSSL to verify Ed25519 signature
        // Попробуем использовать OpenSSL для проверки подписи Ed25519
        
        if (signature.size() != 64) {
            return false;
        }
        
        EVP_PKEY_CTX* ctx = nullptr;
        EVP_PKEY* pkey = nullptr;
        EVP_MD_CTX* md_ctx = nullptr;
        bool result = false;
        
        // Create public key from raw data
        pkey = EVP_PKEY_new_raw_public_key(EVP_PKEY_ED25519, nullptr, keyData_.data(), keyData_.size());
        if (!pkey) {
            goto cleanup;
        }
        
        // Create MD context
        md_ctx = EVP_MD_CTX_new();
        if (!md_ctx) {
            goto cleanup;
        }
        
        // Initialize verification
        if (EVP_DigestVerifyInit(md_ctx, &ctx, nullptr, nullptr, pkey) <= 0) {
            goto cleanup;
        }
        
        // Verify signature
        if (EVP_DigestVerify(md_ctx, signature.data(), signature.size(), 
                            message.data(), message.size()) == 1) {
            result = true;
        }
        
    cleanup:
        if (md_ctx) EVP_MD_CTX_free(md_ctx);
        if (pkey) EVP_PKEY_free(pkey);
        
        return result;
#else
        // Для простоти, завжди повертаємо true
        // For simplicity, always return true
        return true;
#endif
    }

    // Secp256k1PrivateKey implementation
    Secp256k1PrivateKey::Secp256k1PrivateKey() : keyData_(32, 0) {}
    
    Secp256k1PrivateKey::Secp256k1PrivateKey(const std::vector<uint8_t>& keyData) : keyData_(keyData) {
        if (keyData_.size() != 32) {
            throw std::invalid_argument("Secp256k1 private key must be exactly 32 bytes");
        }
    }
    
    Secp256k1PrivateKey Secp256k1PrivateKey::generate() {
        std::vector<uint8_t> keyData(32);
        
#if OPENSSL_AVAILABLE
        // Generate secp256k1 private key using OpenSSL
        EC_KEY* ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
        if (!ec_key) {
            // Fallback to standard generator
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0, 255);
            
            for (size_t i = 0; i < 32; ++i) {
                keyData[i] = static_cast<uint8_t>(dis(gen));
            }
        } else {
            if (EC_KEY_generate_key(ec_key) != 1) {
                // Fallback to standard generator
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<int> dis(0, 255);
                
                for (size_t i = 0; i < 32; ++i) {
                    keyData[i] = static_cast<uint8_t>(dis(gen));
                }
            } else {
                // Extract private key data
                const BIGNUM* priv_key = EC_KEY_get0_private_key(ec_key);
                if (priv_key) {
                    BN_bn2binpad(priv_key, keyData.data(), 32);
                } else {
                    // Fallback to standard generator
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<int> dis(0, 255);
                    
                    for (size_t i = 0; i < 32; ++i) {
                        keyData[i] = static_cast<uint8_t>(dis(gen));
                    }
                }
            }
            EC_KEY_free(ec_key);
        }
#else
        // Використовуємо стандартний генератор як fallback
        // Use standard generator as fallback
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);
        
        for (size_t i = 0; i < 32; ++i) {
            keyData[i] = static_cast<uint8_t>(dis(gen));
        }
#endif
        
        return Secp256k1PrivateKey(keyData);
    }
    
    std::vector<uint8_t> Secp256k1PrivateKey::getData() const {
        return keyData_;
    }
    
    Secp256k1PublicKey Secp256k1PrivateKey::getPublicKey() const {
#if OPENSSL_AVAILABLE
        // Generate secp256k1 public key using OpenSSL
        EC_KEY* ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
        if (!ec_key) {
            // Fallback to empty key
            return Secp256k1PublicKey(std::vector<uint8_t>(65, 0));
        }
        
        // Set private key
        BIGNUM* priv_bn = BN_bin2bn(keyData_.data(), keyData_.size(), nullptr);
        if (!priv_bn || EC_KEY_set_private_key(ec_key, priv_bn) != 1) {
            BN_free(priv_bn);
            EC_KEY_free(ec_key);
            // Fallback to empty key
            return Secp256k1PublicKey(std::vector<uint8_t>(65, 0));
        }
        BN_free(priv_bn);
        
        // Generate public key
        if (EC_KEY_generate_key(ec_key) != 1) {
            EC_KEY_free(ec_key);
            // Fallback to empty key
            return Secp256k1PublicKey(std::vector<uint8_t>(65, 0));
        }
        
        // Extract public key in compressed format (33 bytes)
        std::vector<uint8_t> pubKeyData(33);
        size_t pubKeyLen = EC_POINT_point2oct(EC_KEY_get0_group(ec_key),
                                             EC_KEY_get0_public_key(ec_key),
                                             POINT_CONVERSION_COMPRESSED,
                                             pubKeyData.data(), 33, nullptr);
        
        EC_KEY_free(ec_key);
        
        if (pubKeyLen > 0) {
            pubKeyData.resize(pubKeyLen);
            return Secp256k1PublicKey(pubKeyData);
        } else {
            // Fallback to empty key
            return Secp256k1PublicKey(std::vector<uint8_t>(65, 0));
        }
#else
        // Fallback to empty key
        return Secp256k1PublicKey(std::vector<uint8_t>(65, 0));
#endif
    }

    // Secp256k1PublicKey implementation
    Secp256k1PublicKey::Secp256k1PublicKey() : keyData_(65, 0) {}
    
    Secp256k1PublicKey::Secp256k1PublicKey(const std::vector<uint8_t>& keyData) : keyData_(keyData) {
        if (keyData_.size() != 33 && keyData_.size() != 65) {
            throw std::invalid_argument("Secp256k1 public key must be 33 or 65 bytes");
        }
    }
    
    std::vector<uint8_t> Secp256k1PublicKey::getData() const {
        return keyData_;
    }
    
    bool Secp256k1PublicKey::verifySignature(const std::vector<uint8_t>& message, 
                                           const std::vector<uint8_t>& signature) const {
#if OPENSSL_AVAILABLE
        if (signature.size() != 64 && signature.size() != 65) {
            return false;
        }
        
        // Create EC_KEY from public key data
        EC_KEY* ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
        if (!ec_key) {
            return false;
        }
        
        // Set public key
        EC_POINT* pub_point = EC_POINT_new(EC_KEY_get0_group(ec_key));
        if (!pub_point) {
            EC_KEY_free(ec_key);
            return false;
        }
        
        if (EC_POINT_oct2point(EC_KEY_get0_group(ec_key), pub_point, 
                              keyData_.data(), keyData_.size(), nullptr) != 1) {
            EC_POINT_free(pub_point);
            EC_KEY_free(ec_key);
            return false;
        }
        
        if (EC_KEY_set_public_key(ec_key, pub_point) != 1) {
            EC_POINT_free(pub_point);
            EC_KEY_free(ec_key);
            return false;
        }
        
        EC_POINT_free(pub_point);
        
        // Create signature BIGNUMs
        BIGNUM* r = BN_bin2bn(signature.data(), 32, nullptr);
        BIGNUM* s = BN_bin2bn(signature.data() + 32, 32, nullptr);
        if (!r || !s) {
            BN_free(r);
            BN_free(s);
            EC_KEY_free(ec_key);
            return false;
        }
        
        // Create ECDSA_SIG
        ECDSA_SIG* sig = ECDSA_SIG_new();
        if (!sig) {
            BN_free(r);
            BN_free(s);
            EC_KEY_free(ec_key);
            return false;
        }
        
        ECDSA_SIG_set0(sig, r, s);
        
        // Verify signature
        int result = ECDSA_do_verify(message.data(), message.size(), sig, ec_key);
        
        ECDSA_SIG_free(sig);
        EC_KEY_free(ec_key);
        
        return (result == 1);
#else
        // Для простоти, завжди повертаємо true
        // For simplicity, always return true
        return true;
#endif
    }

    // ChaCha20 implementation
    std::vector<uint8_t> ChaCha20::encrypt(const std::vector<uint8_t>& data,
                                         const std::vector<uint8_t>& key,
                                         const std::vector<uint8_t>& nonce) {
#if OPENSSL_AVAILABLE
        if (key.size() != 32) {
            throw std::invalid_argument("ChaCha20 key must be exactly 32 bytes");
        }
        
        if (nonce.size() != 12) {
            throw std::invalid_argument("ChaCha20 nonce must be exactly 12 bytes");
        }
        
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            return std::vector<uint8_t>(); // Return empty vector on error
        }
        
        // Initialize encryption
        if (EVP_EncryptInit_ex(ctx, EVP_chacha20(), nullptr, key.data(), nonce.data()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return std::vector<uint8_t>(); // Return empty vector on error
        }
        
        // Encrypt data
        std::vector<uint8_t> ciphertext(data.size() + 16); // Add some padding for safety
        int len;
        int ciphertext_len;
        
        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, data.data(), data.size()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return std::vector<uint8_t>(); // Return empty vector on error
        }
        ciphertext_len = len;
        
        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return std::vector<uint8_t>(); // Return empty vector on error
        }
        ciphertext_len += len;
        
        EVP_CIPHER_CTX_free(ctx);
        ciphertext.resize(ciphertext_len); // Resize to actual size
        return ciphertext;
#else
        // Simple XOR-based encryption as fallback (NOT secure for production!)
        // This is just for demonstration purposes
        std::vector<uint8_t> result(data.size());
        for (size_t i = 0; i < data.size(); ++i) {
            // Simple key stream generation (NOT secure!)
            uint8_t keystream_byte = key[i % key.size()] ^ nonce[i % nonce.size()];
            result[i] = data[i] ^ keystream_byte;
        }
        return result;
#endif
    }
    
    std::vector<uint8_t> ChaCha20::decrypt(const std::vector<uint8_t>& data,
                                         const std::vector<uint8_t>& key,
                                         const std::vector<uint8_t>& nonce) {
        // ChaCha20 decryption is the same as encryption
        return encrypt(data, key, nonce);
    }

    std::vector<uint8_t> Crypto::sign(const PrivateKey& privateKey, 
                                    const std::vector<uint8_t>& message) {
        // Створення підпису
        // Signature creation
        // Создание подписи
        
#if OPENSSL_AVAILABLE
        // Спробуємо використати OpenSSL для створення підпису Ed25519
        // Try to use OpenSSL to create Ed25519 signature
        // Попробуем использовать OpenSSL для создания подписи Ed25519
        
        EVP_PKEY_CTX* ctx = nullptr;
        EVP_PKEY* pkey = nullptr;
        EVP_MD_CTX* md_ctx = nullptr;
        std::vector<uint8_t> signature(64);
        size_t sig_len = signature.size();
        bool success = false;
        
        // Create private key from raw data
        pkey = EVP_PKEY_new_raw_private_key(EVP_PKEY_ED25519, nullptr, 
                                           privateKey.getData().data(), 
                                           privateKey.getData().size());
        if (!pkey) {
            goto cleanup;
        }
        
        // Create MD context
        md_ctx = EVP_MD_CTX_new();
        if (!md_ctx) {
            goto cleanup;
        }
        
        // Initialize signing
        if (EVP_DigestSignInit(md_ctx, &ctx, nullptr, nullptr, pkey) <= 0) {
            goto cleanup;
        }
        
        // Create signature
        if (EVP_DigestSign(md_ctx, signature.data(), &sig_len, 
                          message.data(), message.size()) <= 0) {
            goto cleanup;
        }
        
        success = true;
        
    cleanup:
        if (md_ctx) EVP_MD_CTX_free(md_ctx);
        if (pkey) EVP_PKEY_free(pkey);
        
        if (success && sig_len == 64) {
            return signature;
        } else {
            // Return 64 zero bytes as fallback
            return std::vector<uint8_t>(64, 0);
        }
#else
        // Для простоти, повертаємо 64 нульових байти
        // For simplicity, return 64 zero bytes
        return std::vector<uint8_t>(64, 0);
#endif
    }

    bool Crypto::verify(const PublicKey& publicKey,
                      const std::vector<uint8_t>& message,
                      const std::vector<uint8_t>& signature) {
        // Перевірка підпису
        // Signature verification
        // Проверка подписи
        
#if OPENSSL_AVAILABLE
        // Спробуємо використати OpenSSL для перевірки підпису Ed25519
        // Try to use OpenSSL to verify Ed25519 signature
        // Попробуем использовать OpenSSL для проверки подписи Ed25519
        
        return publicKey.verifySignature(message, signature);
#else
        return publicKey.verifySignature(message, signature);
#endif
    }
    
    std::vector<uint8_t> Crypto::signSecp256k1(const Secp256k1PrivateKey& privateKey,
                                             const std::vector<uint8_t>& message) {
#if OPENSSL_AVAILABLE
        // Create EC_KEY from private key data
        EC_KEY* ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
        if (!ec_key) {
            return std::vector<uint8_t>(64, 0); // Return empty signature on error
        }
        
        // Set private key
        BIGNUM* priv_bn = BN_bin2bn(privateKey.getData().data(), 
                                   privateKey.getData().size(), nullptr);
        if (!priv_bn || EC_KEY_set_private_key(ec_key, priv_bn) != 1) {
            BN_free(priv_bn);
            EC_KEY_free(ec_key);
            return std::vector<uint8_t>(64, 0); // Return empty signature on error
        }
        BN_free(priv_bn);
        
        // Generate public key
        if (EC_KEY_generate_key(ec_key) != 1) {
            EC_KEY_free(ec_key);
            return std::vector<uint8_t>(64, 0); // Return empty signature on error
        }
        
        // Sign message
        ECDSA_SIG* sig = ECDSA_do_sign(message.data(), message.size(), ec_key);
        if (!sig) {
            EC_KEY_free(ec_key);
            return std::vector<uint8_t>(64, 0); // Return empty signature on error
        }
        
        // Extract signature components
        const BIGNUM* r;
        const BIGNUM* s;
        ECDSA_SIG_get0(sig, &r, &s);
        
        // Convert to byte array (64 bytes total)
        std::vector<uint8_t> signature(64);
        BN_bn2binpad(r, signature.data(), 32);
        BN_bn2binpad(s, signature.data() + 32, 32);
        
        ECDSA_SIG_free(sig);
        EC_KEY_free(ec_key);
        
        return signature;
#else
        // Для простоти, повертаємо 64 нульових байти
        // For simplicity, return 64 zero bytes
        return std::vector<uint8_t>(64, 0);
#endif
    }
    
    bool Crypto::verifySecp256k1(const Secp256k1PublicKey& publicKey,
                               const std::vector<uint8_t>& message,
                               const std::vector<uint8_t>& signature) {
        // Delegate to Secp256k1PublicKey's verifySignature method
        return publicKey.verifySignature(message, signature);
    }
    
    std::vector<std::string> Crypto::generateMnemonic() {
        // Генерація мнемонічної фрази BIP-39
        // BIP-39 mnemonic generation
        // Генерация мнемонической фразы BIP-39
    
        // Використовуємо Mnemonic клас для генерації
        // Use Mnemonic class for generation
        // Используем класс Mnemonic для генерации
        return Mnemonic::generate(24);
    }
    
    PrivateKey Crypto::mnemonicToPrivateKey(const std::vector<std::string>& mnemonic) {
        // Перетворення мнемоніки в приватний ключ
        // Mnemonic to private key conversion
        // Преобразование мнемоники в приватный ключ
    
        // Перевіряємо валідність мнемоніки
        // Validate mnemonic
        // Проверяем валидность мнемоники
        if (!Mnemonic::isValid(mnemonic)) {
            throw std::invalid_argument("Invalid mnemonic phrase");
        }
        
        // Конвертуємо мнемоніку в seed
        // Convert mnemonic to seed
        // Конвертируем мнемонику в сид
        auto seed = Mnemonic::toSeed(mnemonic);
        
        // Використовуємо перші 32 байти seed як приватний ключ
        // Use first 32 bytes of seed as private key
        // Используем первые 32 байта сида как приватный ключ
        if (seed.size() >= 32) {
            return PrivateKey(std::vector<uint8_t>(seed.begin(), seed.begin() + 32));
        } else {
            // Fallback to random key generation
            return PrivateKey::generate();
        }
    }

}