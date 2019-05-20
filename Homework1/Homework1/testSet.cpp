#include "Set.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Set a;
	assert(a.insert("roti"));
	assert(a.insert("pita"));
	assert(a.insert("bread"));
	assert(a.size() == 3);
	assert(a.contains("bread"));
	ItemType x = "laobing";
	assert(a.get(0, x) && x == "bread");
	assert(a.get(1, x) && x == "pita");
	assert(a.get(2, x) && x == "roti");
	Set b; 
	b.insert("hello");
	b.insert("hi");
	b.insert("hey");

	Set c;
	c.insert("Ciao");
	c.insert("Bye");

	b.swap(c);
	assert(a.size() == 3);
	assert(b.size() == 2);
	assert(c.get(2, x) && x == "hi");
	assert(b.get(0, x) && x == "Bye");
}

int main()
{
	test();
	cout << "Passed all tests" << endl;
}
