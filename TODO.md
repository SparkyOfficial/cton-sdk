# TODO List for CTON-SDK

## Completed Components

### C++ Core (cpp/)
- [x] Project structure with CMakeLists.txt
- [x] Cell.h header with Cell and CellBuilder classes
- [x] Address.h header with Address class
- [x] Crypto.h header with PrivateKey, PublicKey, and Crypto classes
- [x] Boc.h header with Boc, BocParser, and BocBuilder classes
- [x] Cell.cpp implementation
- [x] Address.cpp implementation
- [~] Crypto.cpp implementation with OpenSSL integration (placeholder implementation)
- [~] Boc.cpp implementation with BOC serialization/deserialization (placeholder implementation)
- [x] NativeInterface.h C-style interface for JNA
- [~] NativeInterface.cpp implementation of JNA bindings (missing proper memory management)
- [~] Unit tests (placeholder tests that check compilation only)

### Java Bindings (java/)
- [x] Project structure with pom.xml
- [x] Cell.java JNA wrapper
- [x] Address.java JNA wrapper
- [x] CellBuilder.java fluent API
- [~] CellBuilderTest.java unit tests (placeholder tests)
- [x] Crypto.java JNA wrapper
- [x] Boc.java JNA wrapper
- [x] Native library loading and linking
- [~] Complete JNA interface implementations (missing proper memory management)
- [~] Unit tests (placeholder tests that check compilation only)

### API Client (api-client/)
- [x] Project structure with pom.xml
- [x] TonApiClient.java HTTP client for TON Center API
- [~] TonApiClientTest.java unit tests (mostly disabled)
- [x] TonApiClientExample.java usage example
- [~] LiteClient interface (incomplete implementation)
- [~] TonLiteClient implementation (placeholder using HTTP instead of ADNL)
- [~] LiteClient tests and examples (placeholder tests)

### Contract (contract/)
- [x] Project structure with pom.xml
- [x] Wallet interface
- [~] BaseWallet abstract implementation (placeholder implementation)
- [x] WalletV3 concrete implementation
- [~] Wallet tests (placeholder tests)
- [x] Wallet usage example

## Modules to Implement

### Module 1: ton-sdk-core
- [x] Cell and CellBuilder (C++ and Java)
- [x] Address handling (C++ and Java)
- [~] Cryptography (Ed25519 implementation with OpenSSL) - Placeholder only
- [~] Mnemonic codes (placeholder implementation)
- [~] BOC parser and builder (partial implementation)
- [~] Unit tests (placeholder tests with minimal coverage)

### Module 2: ton-sdk-api-client
- [x] HTTP client for TON Center API
- [~] LiteClient implementation (placeholder using HTTP instead of ADNL)
- [~] Methods for get-methods, block/transaction info (incomplete)
- [x] sendBoc implementation
- [x] Asynchronous operations support

### Module 3: ton-sdk-contract
- [~] Wallet abstraction (partial implementation)
- [~] Message factory (partial implementation)
- [~] Contract wrapper generator (placeholder implementation)
- [~] Jetton standard support (placeholder implementation)
- [~] NFT standard support (placeholder implementation)

## Documentation
- [x] README.md with multilingual descriptions
- [x] LICENSE file
- [~] ARCHITECTURE.md with module descriptions (missing)
- [~] DEVELOPMENT.md with build instructions (missing)
- [x] API documentation
- [x] Examples and tutorials
- [x] CONTRIBUTING.md guidelines

## 🚧 PROJECT STATUS
**The CTON-SDK project is currently in ALPHA state.** The architecture and structure are complete, but most functionality is implemented as placeholders. Core components need proper implementation to work with real TON data and networks.

## Critical Issues to Address
- [ ] Update TODO.md to reflect actual implementation status
- [ ] Implement proper memory management in native interface to prevent leaks
- [ ] Replace placeholder cryptographic implementations with real Ed25519
- [ ] Implement proper BOC serialization/deserialization
- [ ] Fix TON address parsing and formatting
- [ ] Implement real contract functionality (Jetton, NFT, Wallet)
- [ ] Fix CMakeLists.txt to properly integrate OpenSSL
- [ ] Write comprehensive unit tests with real data verification
- [ ] Refactor TonApiClient to eliminate code duplication
- [ ] Implement proper error handling and resource management

## Future Enhancements (Optional Long-term Goals)
- [ ] Integration with existing TON libraries
- [ ] Performance benchmarks
- [ ] Support for other cryptographic algorithms
- [ ] Mobile platform support (Android/iOS)
- [ ] WebAssembly compilation target