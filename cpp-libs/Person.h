#pragma once

#include <string>
#include <iostream>

using namespace std;

class Person
{
private:
	string name;
	uint8_t age;

	string info() const
	{
		return "Person(" + this->name + ", " + to_string(this->age) + "): ";
	}

	void set_info(string name, uint8_t age)
	{
		this->name = name;
		this->age = age;
	}

public:
	Person(string name, uint8_t age) : name(name), age(age)
	{
		cout << info() << "default constructor" << endl;
	}

	Person(const Person &other): name(other.name), age(other.age)
	{
		cout << info() << "copy constructor" << endl;
	}

	void operator=(Person other)
	{
		set_info(other.name, other.age);
		cout << info() << "assigment operator" << endl;
	}

	operator uint8_t() const
	{
		cout << info() << "cast operator" << endl;
		return this->age;
	}

	string get_name() const
	{
		return this->name;
	}
};