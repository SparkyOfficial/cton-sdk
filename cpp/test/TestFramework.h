// TestFramework.h - простий фреймворк для unit тестів
// Author: Андрій Будильников (Sparky)
// Simple unit test framework
// Простой фреймворк для unit тестов

#ifndef CTON_TEST_FRAMEWORK_H
#define CTON_TEST_FRAMEWORK_H

#include <iostream>
#include <string>
#include <vector>

// Макроси для тестів
// Test macros
// Макросы для тестов

#define TEST(name) \
    void test_##name(); \
    static cton::test::TestRegistrar test_registrar_##name(#name, test_##name); \
    void test_##name()

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << " - " << #condition << std::endl; \
            return; \
        } \
    } while(0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << " - " << #condition << std::endl; \
            return; \
        } \
    } while(0)

#define ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << " - Expected: " << (expected) << ", Actual: " << (actual) << std::endl; \
            return; \
        } \
    } while(0)

#define ASSERT_NOT_EQUAL(expected, actual) \
    do { \
        if ((expected) == (actual)) { \
            std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << " - Expected not equal to: " << (expected) << ", Actual: " << (actual) << std::endl; \
            return; \
        } \
    } while(0)

namespace cton {
namespace test {

class TestRegistrar {
public:
    typedef void (*TestFunction)();
    
    TestRegistrar(const std::string& name, TestFunction func) {
        getInstance().addTest(name, func);
    }
    
    static TestRegistrar& getInstance() {
        static TestRegistrar instance;
        return instance;
    }
    
    void addTest(const std::string& name, TestFunction func) {
        tests_.push_back(std::make_pair(name, func));
    }
    
    int runAllTests() {
        int passed = 0;
        int failed = 0;
        
        std::cout << "Running " << tests_.size() << " tests...\n\n";
        
        for (const auto& test : tests_) {
            std::cout << "Running test: " << test.first << std::endl;
            
            try {
                test.second();
                std::cout << "  PASSED\n";
                passed++;
            } catch (const std::exception& e) {
                std::cerr << "  FAILED: " << e.what() << std::endl;
                failed++;
            } catch (...) {
                std::cerr << "  FAILED: Unknown exception\n";
                failed++;
            }
            
            std::cout << std::endl;
        }
        
        std::cout << "Test Results: " << passed << " passed, " << failed << " failed\n";
        return failed;
    }

private:
    std::vector<std::pair<std::string, TestFunction>> tests_;
    
    TestRegistrar() = default;
};

} // namespace test
} // namespace cton

#define RUN_ALL_TESTS() cton::test::TestRegistrar::getInstance().runAllTests()

#endif // CTON_TEST_FRAMEWORK_H