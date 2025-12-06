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
#include <cstdio>
//#include<print> if using C++26 module for print support
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

	void say_hello() const noexcept { std::printf("%s\n", greeting_.c_str()); } // noexcept indicates this function does not throw exceptions which can help with optimizations and clarity

private:
	std::string greeting_{}; // private member variable with default initialization which is best practice
};

enum class Weekday { Mon, Tue, Wed, Thu, Fri, Sat, Sun }; // scoped enum preferred over unscoped enum to avoid name collisions

#pragma endregion // Types

// ============================================================================
// Utility functions
// ============================================================================
#pragma region Utilities

inline void printNumbers(int n) {
    for (int i = 1; i <= n; ++i) {
        std::cout << i << ' ';
    }
}

inline void printThreadSafe_lockUnlock(int n) {
    // Demonstration of manual lock/unlock (prefer lock_guard / unique_lock)
    g_mtx.lock();
    std::cout << "Thread " << n << '\n';
    g_mtx.unlock();
}

inline void printThreadSafe(int n) {
    // Preferred RAII style
    std::lock_guard<std::mutex> lock(g_mtx);
    std::cout << "Thread " << n << '\n';
}

void printVariant(const std::variant<int, std::string>& var) { // variant example   
    if (std::holds_alternative<int>(var)) {
        std::printf("\nVariant<int>: %d\n", std::get<int>(var));
    }
    else if (std::holds_alternative<std::string>(var)) {
        std::printf("\nVariant<string>: %s\n", std::get<std::string>(var).c_str());
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
		std::printf("Event received in App instance.\n");
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
    float f = 3.14f;
    int i = static_cast<int>(f);
    std::printf("Integer (from float): %d\n", i);

    const char* text = "Hello, C++20!";
    char* writableText = const_cast<char*>(text);
    std::printf("Writable text: %s\n", writableText);

    Animal* animalPtr = new Dog("Rex", "German Shepherd");
    Dog* dogPtr = dynamic_cast<Dog*>(animalPtr); // safe downcast
   

	uintptr_t addr = reinterpret_cast<uintptr_t>(animalPtr); // not recommended except for low-level code
    std::printf("Animal pointer address: %p as integer: %zu\n", animalPtr, addr);

    delete animalPtr; // clean up dynamic memory
    // delete dogPtr; // NOT NEEDED already deleted via animalPtr
	
    uint32_t bit = std::bit_cast<uint32_t>(f); // C++20 bit_cast
    std::printf("Bit-cast float to uint32_t: %u\n", bit);

    // -----------------------------
    // Compile-time computations
    // -----------------------------

    static_assert(factorial(5) == 120);
    std::printf("Factorial of 5: %d\n", factorial(5));
    std::printf("PI constant: %.5f\n", PI);

    int sum = add(10, 20);
    std::printf("Sum using concept-constrained template: %d\n", sum);

    // -----------------------------
    // Simple class usage
    // -----------------------------
    
    World world("Hello, World!");
    world.say_hello();

    Person alice{ "Alice", 30 };
    alice.name = "hello world";
    std::printf("Name: %s, Age: %d\n", alice.name.c_str(), alice.age);

    // -----------------------------
    // Small loops / increments
    // -----------------------------
    for (int iter = 0; iter < 5; ++iter) {
        std::printf("Iteration %d\n", iter);
    }

    int j = 5;
	++j; // better to use pre-increment for non-primitive types
	j++; // post-increment is never preferred
    std::printf("Countdown %d\n", j);

    // -----------------------------
    // Threads: basic examples
    // -----------------------------
    std::thread t1(printNumbers, 5);
	t1.join(); // wait for thread to finish before continuing
    std::printf("\nThread finished!\n");

	std::thread t2([]() { // lambda thread fire and forget
        for (int k = 1; k <= 5; ++k)
            std::printf("%d ", k * 2);
        std::printf("\n");
        });
	t2.detach(); // detached thread from main thread
    std::printf("Launched detached lambda thread.\n");

    std::thread t3(printThreadSafe_lockUnlock, 3);
    std::thread t4(printThreadSafe_lockUnlock, 4);
    t3.join(); 
    t4.join();

    std::thread t5(printThreadSafe, 5);
    t5.join();

    int factor = 3;
    std::thread t6([factor]() {
        for (int n = 1; n <= 5; ++n)
            std::printf("%d ", n * factor);
        std::printf("\n");
        });
    t6.join();
    std::printf("Lambda thread with capture finished!\n");

    std::jthread t7([factor](std::stop_token st) {
        for (int n = 1; n <= 5; ++n) {
            if (st.stop_requested()) break;
            std::printf("%d ", n * factor);
        }
        std::printf("\n");
        });
    t7.request_stop();
    std::printf("jthread with stop token finished!\n");

    // -----------------------------
    // Pairs, lists
    // -----------------------------
    std::pair<int, std::string> myPair = std::make_pair(1, "one");
    std::printf("Pair: (%d, %s)\n", myPair.first, myPair.second.c_str());

    std::list<std::pair<std::string, int>> people;
    people.emplace_back("Charlie", 35);
    people.push_back(std::make_pair("Bob", 25));
    for (const auto& person : people)
        std::printf("Person: %s, Age: %d\n", person.first.c_str(), person.second);

    // -----------------------------
    // Vectors, algorithms, ranges
    // -----------------------------
    std::vector<int> numbers = { 1, 2, 3, 4, 5 };
    numbers.push_back(6);
    std::sort(numbers.begin(), numbers.end());
    std::printf("Sorted numbers: ");
    for (const auto& num : numbers) std::printf("%d ", num);

    std::reverse(numbers.begin(), numbers.end());
    std::printf("\nReversed numbers: ");
    for (const auto& num : numbers) std::printf("%d ", num);

    std::string s = "Hello, World!";
    s.replace(7, 5, "C++");
    std::printf("\nModified string: %s\n", s.c_str());

	std::stringstream ss; // string stream bit like stringbuilder in other languages
    ss << 42 << ' ' << 3.14;
    std::string combined = ss.str();
    std::printf("Combined string: %s\n", combined.c_str());

    // -----------------------------
    // Async and future
    // -----------------------------
    std::future<int> result = std::async(std::launch::async, compute);
    std::printf("Computed result: %d\n", result.get());

	auto lambdaAsyncResult = std::async(std::launch::async, [] { return 2 * 10; }); // lambda async example
    std::printf("%d\n", lambdaAsyncResult.get());

    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::printf("Elapsed: %lld ms\n", static_cast<long long>(duration.count()));

    // -----------------------------
    // Smart pointers
    // -----------------------------
    auto worldptr = std::make_unique<World>("Unique World Pointer Text");
    worldptr->say_hello();
    std::printf("Unique pointer used.\n");

    auto movedPtr = std::move(worldptr);
    movedPtr->say_hello();
    std::printf("Moved unique pointer used.\n");

    auto sptr = std::make_shared<int>(42);
    std::printf("Shared pointer value: %d\n", *sptr);
    auto sptr2 = sptr;
    std::printf("Shared pointer 2 value: %d\n", *sptr2);

	// Both shared and unique pointers go out of scope here and memory is freed .release called automatically

    // -----------------------------
    // Maps
    // -----------------------------
    std::map<std::string, int> ageMap;
    ageMap["Alice"] = 30;
    ageMap["Bob"] = 25;
    ageMap.try_emplace("Charlie", 35);
    ageMap.insert({ "Diana", 28 });
    std::printf("Alice's age: %d\n", ageMap["Alice"]);

    if (ageMap.contains("Bob"))
        std::printf("Bob is in the ageMap.\n");

    for (const auto& [name, age] : ageMap)
        std::printf("Name: %s, Age: %d\n", name.c_str(), age);

    std::unordered_map<std::string, int> scoreMap;
    scoreMap["Player1"] = 100;
    scoreMap["Player2"] = 150;
    scoreMap.emplace("Player3", 200);
    scoreMap.try_emplace("Player3", 200);
    scoreMap.insert({ "Player4", 250 });
    scoreMap.find("Player2")->second = 175;
    scoreMap.erase("Player1");

    if (scoreMap.contains("Player2"))
        std::printf("Player2's score: %d\n", scoreMap["Player2"]);

    for (const auto& [player, score] : scoreMap)
        std::printf("Player: %s, Score: %d\n", player.c_str(), score);

    // -----------------------------
    // Deque
    // -----------------------------
    std::deque<int> dq;
    dq.push_back(10);
    dq.push_back(20);
    dq.push_front(5);
    dq.push_front(1);

    std::printf("First element: %d\n", dq.front());
    std::printf("Last element: %d\n", dq.back());

    std::printf("All elements: ");
    for (int n : dq) std::printf("%d ", n);
    std::printf("\n");

    dq.pop_front();
    dq.pop_back();
    std::printf("After pop_front/pop_back: ");
    for (int n : dq) std::printf("%d ", n);
    std::printf("\n");

    // -----------------------------
    // Atomic, iota, ranges
    // -----------------------------
    std::vector<int> v(10);
    std::printf("Vector before iota: ");
    for (const auto& num : v) std::printf("%d ", num);
    std::iota(v.begin(), v.end(), 1);
    std::printf("\nVector after iota: ");
    for (const auto& num : v) std::printf("%d ", num);
    std::printf("\n");

    std::array<int, 3> arr = { 1, 2, 3 };
    std::printf("Array elements: ");
    for (const auto& num : arr) std::printf("%d ", num);
    std::printf("\n");

    std::atomic<int> counter = 0;
    counter.fetch_add(1, std::memory_order_relaxed);
    ++counter;
    std::printf("Atomic counter: %d\n", counter.load(std::memory_order_relaxed));

    std::printf("Filtered (even) numbers: ");
    for (int n : std::views::filter(numbers, [](int x) { return x % 2 == 0; }))
        std::printf("%d ", n);
    std::printf("\n");

    std::ranges::sort(numbers);
    std::printf("Sorted numbers using ranges: ");
    for (const auto& num : numbers) std::printf("%d ", num);
    std::printf("\n");

    v2 = 100;
    v2 = std::string("Hello Variant");
    printVariant(v2);

    std::vector<std::variant<int, float, std::string>> values;
    values.push_back(10);
    values.push_back(static_cast<float>(PI));
    values.push_back("hello");

    for (auto& val : values) {
        std::visit([](auto&& x) {
            if constexpr (std::is_same_v<std::decay_t<decltype(x)>, std::string>)
                std::printf("%s\n", x.c_str());
            else
                std::printf("%f\n", static_cast<double>(x));
            }, val);
    }

    std::tuple<int, std::string, double> myTuple(42, "Hello", PI);
    std::printf("%d\n%s\n%f\n", std::get<0>(myTuple), std::get<1>(myTuple).c_str(), std::get<2>(myTuple));

    auto tuplemake = std::make_tuple(1, 2.5, "abc");
    int tuple_i = std::get<0>(tuplemake);
    double tuple_d = std::get<1>(tuplemake);
    std::string tuple_s = std::get<2>(tuplemake);

    // -----------------------------
    // Inheritance / polymorphism
    // -----------------------------
    SmartPerson sps("Smartie", 150);
    sps.introduce();

    Animal a("GenericAnimal");
    a.speak();

    Dog d("Buddy", "Golden Retriever");
    d.speak();

    Animal* ptr = &d;
    ptr->speak(); // polymorphism
    // do not delete ptr; it's stack memory

    // -----------------------------
    // Error handling
    // -----------------------------
    try {
        int random = 1 + 1;
        std::printf("value: %d\n", random);
    }
    catch (const std::exception& ex) {
        throw MyError(std::string("Custom error: ") + ex.what());
    }

    std::printf("\nProgram completed successfully.\n");
    
	//-----------------------------
	// Custom Event System Demo ( minimal) c++ 20 does not have built-in event system like c# or java
	// ----------------------------  

    Event event; 
    App app1, app2;

	event.subscribe([&]() { app1.onEvent(); }); // subscribe app1 to event via lambda
	event.subscribe([&]() { app2.onEvent(); }); // subscribe app2 to event via lambda

    event.notify();  // triggers all subscribed events

	// events are good for games, GUI apps. # decoupled systems =( which allow components to communicate without tight coupling )

	//-----------------------------
	// End of main
	// ----------------------------
    return 0;
}
#pragma endregion // Main