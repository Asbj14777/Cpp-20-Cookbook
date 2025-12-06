#pragma once
#include <iostream>
#include <string>

class Animal { // base class
public:
    
    explicit Animal(std::string name) : name_(std::move(name)) {}

    virtual void speak() const { // virtual allows overriding
        std::cout << name_ << " makes a sound.\n";
    }

    virtual ~Animal() = default; // virtual destructor for safe polymorphism

protected:
    std::string name_{};
};
