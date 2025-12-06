#pragma once // Require once directive so if included multiple times, only included once
#include <string>
class SmartPerson
{
public: // public interface
	explicit SmartPerson(std::string name, int iq);// constructor with member initializer list
	void introduce() const noexcept; // defined in SmartPerson.cpp inline void SmartPerson::introduce() const noexcept;
private: // incapsulate data members
	std::string name_{}; // bracket initialization is safest
	int iq_{};
};