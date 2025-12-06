#pragma once
#include <iostream>

class Dog : public Animal { // public inheritance from Animal Base class    
public:
    explicit Dog(std::string name, std::string breed)
        : Animal(std::move(name)), breed_(std::move(breed)) {
    }

    void speak() const override { // override base class method
        std::cout << name_ << " barks. Breed: " << breed_ << "\n";
    }

private:
    std::string breed_{};
};
