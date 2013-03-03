#pragma once
#include <string>
#include <iostream>

using namespace std;

class Item
{
public:
	Item(void);
	~Item(void);

	virtual string name() = 0;
	virtual float price() = 0;
};

