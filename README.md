# CTON-SDK (C++ & Java JNA/JNI)

Advanced TON Blockchain SDK with performance, type safety, and extensibility.

## Проект на русском / Проект українською / Project in English

<!-- English version -->
## CTON-SDK: Advanced TON Blockchain SDK

This SDK is built on three core principles:

1. **Performance**: Using low-level optimizations, async I/O out of the box, and minimal memory allocations
2. **Type Safety & Convenience**: Modern, intuitive API with Builder patterns and Fluent Interface
3. **Extensibility**: Modular architecture for easy addition of new token standards or custom contracts

### Architecture Overview

The SDK is divided into logically independent modules:

1. **ton-sdk-core**: Foundation without network dependencies. Contains all cryptography and data handling.
2. **ton-sdk-api-client**: Client for blockchain interaction.
3. **ton-sdk-contract**: High-level abstractions for developers.

### Current Status

The CTON-SDK has been significantly enhanced with:

- Proper memory management in native interface
- Improved cryptographic implementations using OpenSSL
- Enhanced BOC serialization/deserialization
- Real functionality in contract modules (Jetton, NFT, Wallet)
- Comprehensive unit tests
- Better error handling and resource management

### Building the Project

To build the complete SDK:

```bash
# Windows
build_all.bat

# Or build components separately
build_cpp.bat    # Build C++ core
build_java.bat   # Build Java components
```

### Prerequisites

- CMake 3.10 or higher
- Visual Studio or compatible C++ compiler
- JDK 8 or higher
- Maven 3.6 or higher
- OpenSSL 3.6.0 (included in the project)

<!-- Russian version -->
## CTON-SDK: Продвинутый SDK для блокчейна TON

Этот SDK построен на трех основных принципах:

1. **Производительность**: Использование низкоуровневых оптимизаций, асинхронный ввод/вывод и минимальные аллокации памяти
2. **Типобезопасность и удобство**: Современный, интуитивно понятный API с паттернами Builder и Fluent Interface
3. **Расширяемость**: Модульная архитектура для легкого добавления новых стандартов токенов или пользовательских контрактов

### Текущий статус

CTON-SDK был значительно улучшен с:

- Правильным управлением памятью в нативном интерфейсе
- Улучшенными криптографическими реализациями с использованием OpenSSL
- Улучшенной сериализацией/десериализацией BOC
- Реальной функциональностью в модулях контрактов (Jetton, NFT, Wallet)
- Комплексными модульными тестами
- Лучшей обработкой ошибок и управлением ресурсами

### Сборка проекта

Для сборки полного SDK:

```bash
# Windows
build_all.bat

# Или сборка компонентов по отдельности
build_cpp.bat    # Сборка C++ ядра
build_java.bat   # Сборка Java компонентов
```

### Предварительные требования

- CMake 3.10 или выше
- Visual Studio или совместимый компилятор C++
- JDK 8 или выше
- Maven 3.6 или выше
- OpenSSL 3.6.0 (включен в проект)

<!-- Ukrainian version -->
## CTON-SDK: Передовий SDK для блокчейну TON

Цей SDK побудований на трьох основних принципах:

1. **Продуктивність**: Використання низькорівневих оптимізацій, асинхронний ввід/вивід та мінімальні алокації пам'яті
2. **Типобезпечність та зручність**: Сучасний, інтуїтивно зрозумілий API з патернами Builder та Fluent Interface
3. **Розширюваність**: Модульна архітектура для легкого додавання нових стандартів токенів або користувацьких контрактів

### Поточний статус

CTON-SDK було значно покращено з:

- Правильним управлінням пам'яттю в нативному інтерфейсі
- Покращеними криптографічними реалізаціями з використанням OpenSSL
- Покращеною серіалізацією/десеріалізацією BOC
- Реальною функціональністю в модулях контрактів (Jetton, NFT, Wallet)
- Комплексними модульними тестами
- Кращою обробкою помилок та управлінням ресурсами

### Збірка проекту

Для збірки повного SDK:

```bash
# Windows
build_all.bat

# Або збірка компонентів окремо
build_cpp.bat    # Збірка C++ ядра
build_java.bat   # Збірка Java компонентів
```

### Вимоги

- CMake 3.10 або вище
- Visual Studio або сумісний компілятор C++
- JDK 8 або вище
- Maven 3.6 або вище
- OpenSSL 3.6.0 (включено в проект)