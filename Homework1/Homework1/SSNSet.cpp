#include "SSNSet.h"
#include <iostream> //checkeronis
SSNSet::SSNSet()
{
}

bool SSNSet::add(ItemType ssn)
{
	return set.insert(ssn);
}

int SSNSet::size() const
{
	return set.size();
}

void SSNSet::print() const
{
	ItemType value;
	for (int i = 0; i < set.size(); i++)
	{
		set.get(i, value);
			std::cout << value << std::endl;
	}
} 
