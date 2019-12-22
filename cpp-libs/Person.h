#pragma once

#include <string>
#include <iostream>

using namespace std;

class Person
{
private:
	string id;
	string name;
	uint8_t age;

	string info() const
	{
		return "[" + this->id + "]: (" + this->name + ", " + to_string(this->age) + "): ";
	}

	void set_info(string name, uint8_t age)
	{
		this->name = name;
		this->age = age;
	}

public:
	Person(string id, string name, uint8_t age) : id(id), name(name), age(age)
	{
		cout << info() << "default constructor" << endl;
	}

	Person(const Person &other): id(other.id + " copy"), name(other.name), age(other.age)
	{
		cout << info() << "copy constructor" << endl;
	}

	~Person()
	{
		cout << info() << "destructor" << endl;
	}

	void operator=(Person &other)
	{
		cout << info() << "assigment operator" << endl;
		name = other.name;
		age = other.age;
	}

	operator uint8_t() const
	{
		cout << info() << "uint8_t() cast operator" << endl;
		return this->age;
	}

	operator string() const
	{
		cout << info() << "string() cast operator" << endl;
		return info();
	}

	inline uint8_t get_age() const
	{
		return this->age;
	}

	inline string get_name() const
	{
		return this->name;
	}
};