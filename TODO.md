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
- [x] Crypto.cpp implementation (TODO)
- [x] Boc.cpp implementation (TODO)

### Java Bindings (java/)
- [x] Project structure with pom.xml
- [x] Cell.java JNA wrapper
- [x] Address.java JNA wrapper
- [x] CellBuilder.java fluent API
- [x] CellBuilderTest.java unit tests
- [x] Crypto.java JNA wrapper
- [x] Boc.java JNA wrapper
- [x] Native library loading and linking (TODO)
- [x] Complete JNA interface implementations (TODO)

## Modules to Implement

### Module 1: ton-sdk-core
- [x] Cell and CellBuilder (C++ and Java)
- [x] Address handling (C++ and Java)
- [ ] Cryptography (Ed25519 implementation)
- [ ] Mnemonic codes
- [ ] BOC parser and builder
- [ ] Unit tests with 100% coverage

### Module 2: ton-sdk-api-client
- [ ] HTTP client for TON Center API
- [ ] LiteClient implementation
- [ ] Methods for get-methods, block/transaction info
- [ ] sendBoc implementation
- [ ] Asynchronous operations support

### Module 3: ton-sdk-contract
- [ ] Wallet abstraction
- [ ] Message factory
- [ ] Contract wrapper generator
- [ ] Jetton standard support
- [ ] NFT standard support

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