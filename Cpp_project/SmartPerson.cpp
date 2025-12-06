#include "SmartPerson.hpp"

SmartPerson::SmartPerson(std::string name, int iq)
	: name_(std::move(name)), iq_(iq) {
}

void SmartPerson::introduce() const noexcept { 
	std::printf("Hello, my name is %s and my IQ is %d.\n", name_.c_str(), iq_); // c_str() for C-style string support
}