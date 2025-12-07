#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <deque>
#include <variant>
#include <tuple>
#include <print>    // for std::println / std::format support
#include <format>
// #include<print> if using C++26 module for print support
// #include <windows.h> // avoid including platform-specific headers in cross-platform code + it increases compile time as it is a large header rather use platform abstraction libraries if needed
// lib header files included above
#include "Animal.hpp"
#include "Dog.hpp"
#include "SmartPerson.hpp"

// My project header files would go here

using namespace std; // avoid in production code, prefer explicit std:: prefix as it makes code clearer
// it makes possible to accidentally introduce name collisions especially in larger projects

// ============================================================================
// Constants / Macros
// ============================================================================
#pragma region Constants
#define DO_NOT_USE_DEFINE 100 // avoid using #define for constants as it has no type safety and will overwrite existing definitions and pollutes global namespace

extern const int CONST_INT = 2; // External linkage constant (avoid in header files)
static constexpr double PI = 3.14159; // compile-time constant with internal linkage
inline constexpr int MAX_SIZE = 100; // inline constexpr for header file constants
inline static constexpr char DEFAULT_CHAR = 'A'; // inline static constexpr for header file constants

#pragma endregion // Constants

// ============================================================================
// Globals (minimal, prefer to avoid globals in production)
// ============================================================================
#pragma region Globals

static std::mutex g_mtx{}; // mutex for simple printing demo
std::variant<int, std::string> v2{};
#pragma endregion // Globals

// ============================================================================
// Types
// ============================================================================
#pragma region Types

struct Person {
    std::string name{}; // default member initialization
    int age{};
};

class World {
public:
    explicit World(std::string greeting) : greeting_(std::move(greeting)) {}

    void say_hello() const noexcept {
        // use std::println for direct output
        std::println("{}", greeting_);
    } // noexcept indicates this function does not throw exceptions which can help with optimizations and clarity

private:
    std::string greeting_{}; // private member variable with default initialization which is best practice
};

enum class Weekday { Mon, Tue, Wed, Thu, Fri, Sat, Sun }; // scoped enum preferred over unscoped enum to avoid name collisions

#pragma endregion // Types

// ============================================================================
// Utility functions
// ============================================================================
#pragma region Utilities

// Note: original printed numbers on one line with spaces. For clarity and teaching,
// we'll print each value on its own line (can be changed easily).
inline void printNumbers(int n) {
    for (int i = 1; i <= n; ++i) {
        std::println("{}", i); // clearer formatting
    }
}

inline void printThreadSafe_lockUnlock(int n) {
    // Demonstration of manual lock/unlock (prefer lock_guard / unique_lock)
    g_mtx.lock();
    std::println("Thread {}", n);
    g_mtx.unlock();
}

inline void printThreadSafe(int n) {
    // Preferred RAII style
    std::lock_guard<std::mutex> lock(g_mtx);
    std::println("Thread {}", n);
}

void printVariant(const std::variant<int, std::string>& var) { // variant example
    if (std::holds_alternative<int>(var)) {
        std::println("\nVariant<int>: {}", std::get<int>(var));
    }
    else if (std::holds_alternative<std::string>(var)) {
        std::println("\nVariant<string>: {}", std::get<std::string>(var));
    }
}

struct MyError : std::runtime_error { // custom error type inheriting from std::runtime_error
    using std::runtime_error::runtime_error;
};

inline int compute() { return 42 * 500; }

constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

template<typename T> // C++20 concept example
concept Integral = std::is_integral_v<T>; // concept to constrain to integral types which are built-in integer types

template<Integral T>
T add(T a, T b) { return a + b; }

class Event {
    std::vector<std::function<void()>> listeners;
public:
    void subscribe(std::function<void()> listener) {
        listeners.push_back(listener);
    }
    void notify() {
        for (auto& listener : listeners) listener();
    }
};

class App {
public:
    void onEvent() {
        std::println("Event received in App instance.");
    }
};

#pragma endregion // Utilities

// ============================================================================
// Main
// ============================================================================
#pragma region Main
int main()
{
    // -----------------------------
    // Basic casting / formatted output
    // -----------------------------
    {
        std::println();
        std::println("### Basic casting / formatted output ###");
        std::println();

        float f = 3.14f;
        int i = static_cast<int>(f);
        std::println("Integer (from float): {}", i);

        const char* text = "Hello, C++20!";
        char* writableText = const_cast<char*>(text);
        std::println("Writable text: {}", writableText);

        Animal* animalPtr = new Dog("Rex", "German Shepherd");
        Dog* dogPtr = dynamic_cast<Dog*>(animalPtr); // safe downcast

        uintptr_t addr = reinterpret_cast<uintptr_t>(animalPtr); // not recommended except for low-level code
        // print pointer and integer address; print pointer as void* for consistent formatting
        std::println("Animal pointer address: {} as integer: {}", static_cast<const void*>(animalPtr), addr);

        delete animalPtr; // clean up dynamic memory
        // delete dogPtr; // NOT NEEDED already deleted via animalPtr

        uint32_t bit = std::bit_cast<uint32_t>(f); // C++20 bit_cast
        std::println("Bit-cast float to uint32_t: {}", bit);
    }
    // -----------------------------
    // Compile-time computations
    // -----------------------------
    {
        std::println();
        std::println("### Compile-time computations ###");
        std::println();
        
        static_assert(factorial(5) == 120);
        std::println("Factorial of 5: {}", factorial(5));
        // use format-style specifier for float precision
        std::println("PI constant: {:.5f}", PI);

        int sum = add(10, 20);
        std::println("Sum using concept-constrained template: {}", sum);
    }
    // -----------------------------
    // Simple class usage
    // -----------------------------
    {
        std::println();
        std::println("### Simple class usage ###");
        std::println();

        World world("Hello, World!");
        world.say_hello();

        Person alice{ "Alice", 30 };
        alice.name = "hello world";
        std::println("Name: {}, Age: {}", alice.name, alice.age);
    }
    // -----------------------------
    // Small loops / increments
    // -----------------------------
    {
        std::println();
        std::println("### Small loops / increments ###");
        std::println();

        for (int iter = 0; iter < 5; ++iter) {
            std::println("Iteration {}", iter);
        }

        int j = 5;
        ++j; // better to use pre-increment for non-primitive types
        j++; // post-increment is never preferred
        std::println("Countdown {}", j);
    }
    // -----------------------------
    // Threads: basic examples
    // -----------------------------
    {
        std::println();
        std::println("### Threads basic examples ###"); 
        std::println();

        std::thread t1(printNumbers, 5);
        t1.join(); // wait for thread to finish before continuing
        std::println(); // blank line

        std::thread t2([]() { // lambda thread fire and forget
            // print numbers multiplied by 2 on separate lines for clarity
            for (int k = 1; k <= 5; ++k)
                std::println("{}", k * 2);
            std::println();
            });
        t2.detach(); // detached thread from main thread
        std::println("Launched detached lambda thread.");

        std::thread t3(printThreadSafe_lockUnlock, 3);
        std::thread t4(printThreadSafe_lockUnlock, 4);
        t3.join();
        t4.join();

        std::thread t5(printThreadSafe, 5);
        t5.join();

        int factor = 3;
        std::thread t6([factor]() {
            for (int n = 1; n <= 5; ++n)
                std::println("{}", n * factor);
            std::println();
            });
        t6.join();
        std::println("Lambda thread with capture finished!");

        std::jthread t7([factor](std::stop_token st) {
            for (int n = 1; n <= 5; ++n) {
                if (st.stop_requested()) break;
                std::println("{}", n * factor);
            }
            std::println();
            });
        t7.request_stop();
        std::println("jthread with stop token finished!");
    }
    // -----------------------------
    // Pairs, lists
    // -----------------------------
    {
        std::println();
        std::println("### Pairs / lists ###");
        std::println();

        std::pair<int, std::string> myPair = std::make_pair(1, "one");
        // print pair using format
        std::println("Pair: ({}, {})", myPair.first, myPair.second);

        std::list<std::pair<std::string, int>> people;
        people.emplace_back("Charlie", 35);
        people.push_back(std::make_pair("Bob", 25));
        for (const auto& person : people)
            std::println("Person: {}, Age: {}", person.first, person.second);
    }
    // -----------------------------
    // Vectors, algorithms, ranges
    // -----------------------------
    {
        std::println();
        std::println("### Vectors, algorithms, ranges ###");
        std::println();

        std::vector<int> numbers = { 1, 2, 3, 4, 5 };
        numbers.push_back(6);
        std::sort(numbers.begin(), numbers.end());
        std::println("Sorted numbers: ");
        // print numbers on one line (joined with spaces)
        {
            std::string out;
            out.reserve(numbers.size() * 3);
            for (size_t idx = 0; idx < numbers.size(); ++idx) {
                out += std::format("{}", numbers[idx]);
                if (idx + 1 != numbers.size()) out += ' ';
            }
            std::println("{}", out);
        }

        std::reverse(numbers.begin(), numbers.end());
        std::println("Reversed numbers: ");
        {
            std::string out{};
            out.reserve(numbers.size() * 3);
            for (size_t idx = 0; idx < numbers.size(); ++idx) {
                out += std::format("{}", numbers[idx]);
                if (idx + 1 != numbers.size()) out += ' ';
            }
            std::println("{}", out);
        }

        std::string s = "Hello, World!";
        s.replace(7, 5, "C++");
        std::println("Modified string: {}", s);

        std::stringstream ss{}; // string stream bit like stringbuilder in other languages
        ss << 42 << ' ' << 3.14;
        std::string combined = ss.str();
        // use std::format to construct consistent output
        std::println("Combined string: {}", combined);
    }
    // -----------------------------
    // Async and future
    // -----------------------------
    {
        std::println();
        std::println("### Async and future ###");
        std::println();

        std::future<int> result = std::async(std::launch::async, compute);
        std::println("Computed result: {}", result.get());

        auto lambdaAsyncResult = std::async(std::launch::async, [] { return 2 * 10; }); // lambda async example
        std::println("{}", lambdaAsyncResult.get());
    }
    // -----------------------------
    // Time Tracking
    // -----------------------------
    {
        std::println();
        std::println("### Time Tracking ###");
        std::println();
        
        auto start = std::chrono::high_resolution_clock::now();

        std::this_thread::sleep_for(std::chrono::seconds(1)); // simulate work

        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::println("Elapsed: {} ms", static_cast<long long>(duration.count()));
    }
    // -----------------------------
    // Smart pointers
    // -----------------------------
    {
        std::println();
        std::println("### Smart pointers ###");
        std::println();

        auto worldptr = std::make_unique<World>("Unique World Pointer Text");
        worldptr->say_hello();
        std::println("Unique pointer used.");

        auto movedPtr = std::move(worldptr);
        movedPtr->say_hello();
        std::println("Moved unique pointer used.");

        auto sptr = std::make_shared<int>(42);
        std::println("Shared pointer value: {}", *sptr);
        auto sptr2 = sptr;
        std::println("Shared pointer 2 value: {}", *sptr2);



        // Both shared and unique pointers go out of scope here and memory is freed .release called automatically
    }
    // -----------------------------
    // Maps
    // -----------------------------
    {
        std::println();
        std::println("### Maps ###");
        std::println();

        std::map<std::string, int> ageMap{};
        ageMap["Alice"] = 30;
        ageMap["Bob"] = 25; // somewhat less efficient than emplace
        ageMap.try_emplace("Charlie", 35);
        ageMap.insert({ "Diana", 28 });
        std::println("Alice's age: {}", ageMap["Alice"]);

        if (ageMap.contains("Bob"))
            std::println("Bob is in the ageMap.");

        for (const auto& [name, age] : ageMap)
            std::println("Name: {}, Age: {}", name, age);

        std::unordered_map<std::string, int> scoreMap{};
        scoreMap["Player1"] = 100;
        scoreMap["Player2"] = 150;
        scoreMap.emplace("Player3", 200);
        scoreMap.try_emplace("Player3", 200);
        scoreMap.insert({ "Player4", 250 });
        if (auto it = scoreMap.find("Player2"); it != scoreMap.end()) it->second = 175;
        scoreMap.erase("Player1");

        if (scoreMap.contains("Player2"))
            std::println("Player2's score: {}", scoreMap["Player2"]);

        for (const auto& [player, score] : scoreMap)
            std::println("Player: {}, Score: {}", player, score);
    }
    // -----------------------------
    // Deque
    // -----------------------------
    {
        std::println();
        std::println("### Deque ###");
        std::println();

        std::deque<int> dq{};
        dq.push_back(10);
        dq.push_back(20);
        dq.push_front(5);
        dq.push_front(1);

        std::println("First element: {}", dq.front());
        std::println("Last element: {}", dq.back());

        {
            std::string out{};
            out.reserve(dq.size() * 3);
            for (size_t idx = 0; idx < dq.size(); ++idx) {
                out += std::format("{}", dq[idx]);
                if (idx + 1 != dq.size()) out += ' ';
            }
            std::println("All elements: {}", out);
        }

        dq.pop_front();
        dq.pop_back();
        {
            std::string out{};
            out.reserve(dq.size() * 3);
            for (size_t idx = 0; idx < dq.size(); ++idx) {
                out += std::format("{}", dq[idx]);
                if (idx + 1 != dq.size()) out += ' ';
            }
            std::println("After pop_front/pop_back: {}", out);
        }
    }
    // -----------------------------
	// Atomic, iota, ranges Arrays, Variant, Tuple
    // -----------------------------
    {
        std::println();
        std::println("### Atomic, iota, ranges Arrays, Variant, Tuple ###");
        std::println();

        // Numbers vector used throughout
        std::vector<int> numbers = { 1, 2, 3, 4, 5 };
        numbers.push_back(6);

        // Sort numbers
        std::ranges::sort(numbers);
        std::string out{};
        for (size_t idx = 0; idx < numbers.size(); ++idx) {
            out += std::format("{}", numbers[idx]);
            if (idx + 1 != numbers.size()) out += ' ';
        }
        std::println("Sorted numbers: {}", out);

        // Reverse numbers
        std::reverse(numbers.begin(), numbers.end());
        out.clear();
        for (size_t idx = 0; idx < numbers.size(); ++idx) {
            out += std::format("{}", numbers[idx]);
            if (idx + 1 != numbers.size()) out += ' ';
        }
        std::println("Reversed numbers: {}", out);

        // Filter even numbers using ranges
        out.clear();
        for (int n : numbers | std::views::filter([](int x) { return x % 2 == 0; })) {
            out += std::format("{}", n);
            out += ' ';
        }
        std::println("Filtered (even) numbers: {}", out);

        // Vector after iota
        std::vector<int> v(10);
        std::iota(v.begin(), v.end(), 1);
        out.clear();
        for (int n : v) {
            out += std::format("{}", n);
            out += ' ';
        }
        std::println("Vector after iota: {}", out);

        // Array
        std::array<int, 3> arr = { 1, 2, 3 };
        out.clear();
        for (int n : arr) {
            out += std::format("{}", n);
            out += ' ';
        }
        std::println("Array elements: {}", out);

        // Atomic
        std::atomic<int> counter = 0;
        counter.fetch_add(1, std::memory_order_relaxed);
        ++counter;
        std::println("Atomic counter: {}", counter.load(std::memory_order_relaxed));

        // Variant
        v2 = 100;
        printVariant(v2);
        v2 = std::string("Hello Variant");
        printVariant(v2);

        std::vector<std::variant<int, float, std::string>> values;
        values.push_back(10);
        values.push_back(static_cast<float>(PI));
        values.push_back("hello");

        for (auto& val : values) {
            std::visit([](auto&& x) {
                if constexpr (std::is_same_v<std::decay_t<decltype(x)>, std::string>)
                    std::println("{}", x);
                else
                    std::println("{:.6f}", static_cast<double>(x));
                }, val);
        }

        // Tuple
        std::tuple<int, std::string, double> myTuple(42, "Hello", PI);
        std::println("Tuple elements:");
        std::println("{}", std::get<0>(myTuple));
        std::println("{}", std::get<1>(myTuple));
        std::println("{:.6f}", std::get<2>(myTuple));
    
    }
    // -----------------------------
    // Inheritance / polymorphism
    // -----------------------------
    {
        std::println();
        std::println("### Inheritance / polymorphism ###");
        std::println();

        SmartPerson sps("Smartie", 150);
        sps.introduce();

        // If Animal/Dog/SmartPerson are defined in headers as classes with constructors that accept these args,
        // we assume those definitions don't get changed here (user asked classes not be modified).
        // For demonstration, call appropriate methods that exist on those types.
        // some headers may define constructors that match these calls
        // we rely on existing class APIs present in Animal.hpp etc.
        

        // The following construction assumes Animal and Dog classes have constructors as used originally.
        try {
            Animal a("GenericAnimal");
            a.speak();

            Dog d("Buddy", "Golden Retriever");
            d.speak();

            Animal* ptr = &d;
            ptr->speak(); // polymorphism
            // do not delete ptr; it's stack memory
        }
        catch (...) {
            // In case Animal/Dog don't have these constructors, this block prevents compile-time change here;
            // but normally the original code compiled so these calls should be fine.
        }
    }
    // -----------------------------
    // Error handling
    // -----------------------------
    {
        std::println();
        std::println("### Error handling ###");
        std::println();

        try {
            int random = 1 + 1;
            std::println("value: {}", random);
        }
        catch (const std::exception& ex) {
            throw MyError(std::string("Custom error: ") + ex.what());
        }

        std::println("\nProgram completed successfully.");
    }
    //-----------------------------
    // Custom Event System Demo ( minimal) c++ 20 does not have built-in event system like c# or java
    // ----------------------------
    {
        std::println();
        std::println("### Custom Event System Demo ###");
        std::println();

        Event event{};
        App app1{}, app2{};

        event.subscribe([&] { app1.onEvent(); }); // subscribe app1 to event via lambda
        event.subscribe([&]() { app2.onEvent(); }); // subscribe app2 to event via lambda

        event.notify();  // triggers all subscribed events

        // events are good for games, GUI apps. # decoupled systems =( which allow components to communicate without tight coupling )
    }
    //-----------------------------
    // End of main
    // ----------------------------
    std::println();
    std::println("### End of Program ###");
    std::println();

    return 0;
}
#pragma endregion // Main
