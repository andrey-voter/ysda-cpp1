#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <set>
#include <typeinfo>

class AbstractTest {
public:
    virtual void SetUp() = 0;
    virtual void TearDown() = 0;
    virtual void Run() = 0;
    virtual ~AbstractTest() = default;
};

class AbstractTestCreator {
public:
    virtual ~AbstractTestCreator() = default;
    virtual std::unique_ptr<AbstractTest> Create() const = 0;
};

template <class T>
class TestCreator : public AbstractTestCreator {
public:
    std::unique_ptr<AbstractTest> Create() const override {
        return std::make_unique<T>();
    }
};

class TestRegistry {
public:
    template <class TestClass>
    void RegisterClass(const std::string& class_name) {
        if (types_.contains(&typeid(TestClass))) {
            throw std::runtime_error("Class already in factory!");
        }
        types_.insert(&typeid(TestClass));
        auto it = factory_.find(class_name);
        if (it == factory_.end()) {
            factory_[class_name] = std::make_unique<TestCreator<TestClass>>();
        } else {
            throw std::runtime_error("Class already in factory!");
        }
    }

    std::unique_ptr<AbstractTest> CreateTest(const std::string& class_name) {
        auto it = factory_.find(class_name);
        if (it != factory_.end()) {
            return it->second->Create();
        } else {
            throw std::out_of_range("Cant create, no such class in factory");
        }
    }

    void RunTest(const std::string& test_name) {
        auto it = factory_.find(test_name);
        auto test = it->second->Create();

        test->SetUp();
        try {
            test->Run();
        } catch (std::exception e) {
            test->TearDown();
            throw;
        }
        test->TearDown();
    }

    template <class Predicate>
    std::vector<std::string> ShowTests(Predicate callback) const {
        std::vector<std::string> test_names;
        for (const auto& [test_name, _] : factory_) {
            if (callback(test_name)) {
                test_names.push_back(test_name);
            }
        }
        return test_names;
    }

    std::vector<std::string> ShowAllTests() const {
        std::vector<std::string> test_names;
        test_names.reserve(factory_.size());

        for (const auto& [test_name, _] : factory_) {
            test_names.push_back(test_name);
        }
        return test_names;
    }

    template <class Predicate>
    void RunTests(Predicate callback) {
        for (const auto& [test_name, _] : factory_) {
            if (callback(test_name)) {
                RunTest(test_name);
            }
        }
    }

    void Clear() {
        factory_.clear();
        types_.clear();
    }

    TestRegistry(const TestRegistry&) = delete;
    TestRegistry& operator=(const TestRegistry&) = delete;
    TestRegistry(TestRegistry&&) = delete;
    TestRegistry& operator=(TestRegistry&&) = delete;

    static TestRegistry& Instance() {
        static TestRegistry instance;
        return instance;
    }

private:
    TestRegistry() = default;
    typedef std::map<std::string, std::unique_ptr<AbstractTestCreator>> FactoryMap;
    FactoryMap factory_;
    std::set<std::type_info const*> types_;
};

struct [[maybe_unused]] FullMatch {
    bool operator()(std::string x) {
        return str == x;
    }

    std::string str;
};

struct [[maybe_unused]] Substr {
    bool operator()(std::string x) {
        return x.find(str) != std::string::npos;
    }

    std::string str;
};
