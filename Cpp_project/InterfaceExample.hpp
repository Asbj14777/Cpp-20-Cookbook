#pragma once
class InterfaceExample {
	public:
	virtual void doSomething() = 0; // pure virtual function makes this an interface
	virtual ~InterfaceExample() = default;
};
