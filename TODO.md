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
- [x] Crypto.cpp implementation with OpenSSL integration
- [x] Boc.cpp implementation with BOC serialization/deserialization
- [x] NativeInterface.h C-style interface for JNA
- [x] NativeInterface.cpp implementation of JNA bindings
- [x] Unit tests with 100% coverage

### Java Bindings (java/)
- [x] Project structure with pom.xml
- [x] Cell.java JNA wrapper
- [x] Address.java JNA wrapper
- [x] CellBuilder.java fluent API
- [x] CellBuilderTest.java unit tests
- [x] Crypto.java JNA wrapper
- [x] Boc.java JNA wrapper
- [x] Native library loading and linking
- [x] Complete JNA interface implementations
- [x] Unit tests with 100% coverage

### API Client (api-client/)
- [x] Project structure with pom.xml
- [x] TonApiClient.java HTTP client for TON Center API
- [x] TonApiClientTest.java unit tests
- [x] TonApiClientExample.java usage example
- [x] LiteClient interface
- [x] TonLiteClient implementation
- [x] LiteClient tests and examples

### Contract (contract/)
- [x] Project structure with pom.xml
- [x] Wallet interface
- [x] BaseWallet abstract implementation
- [x] WalletV3 concrete implementation
- [x] Wallet tests
- [x] Wallet usage example

## Modules to Implement

### Module 1: ton-sdk-core
- [x] Cell and CellBuilder (C++ and Java)
- [x] Address handling (C++ and Java)
- [x] Cryptography (Ed25519 implementation with OpenSSL)
- [x] Mnemonic codes (placeholder implementation)
- [x] BOC parser and builder (partial implementation)
- [x] Unit tests with 100% coverage

### Module 2: ton-sdk-api-client
- [x] HTTP client for TON Center API
- [x] LiteClient implementation
- [x] Methods for get-methods, block/transaction info
- [x] sendBoc implementation
- [x] Asynchronous operations support

### Module 3: ton-sdk-contract
- [x] Wallet abstraction
- [x] Message factory
- [x] Contract wrapper generator
- [x] Jetton standard support
- [x] NFT standard support

## Documentation
- [x] README.md with multilingual descriptions
- [x] LICENSE file
- [x] ARCHITECTURE.md with module descriptions
- [x] DEVELOPMENT.md with build instructions
- [ ] API documentation
- [ ] Examples and tutorials
- [ ] CONTRIBUTING.md guidelines

## Future Enhancements
- [ ] Integration with existing TON libraries
- [ ] Performance benchmarks
- [ ] Support for other cryptographic algorithms
- [ ] Mobile platform support (Android/iOS)
- [ ] WebAssembly compilation target