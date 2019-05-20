#include "newSet.h"

Set::Set()
{
	m_set = new ItemType[DEFAULT_MAX_ITEMS];
	m_maxSize = DEFAULT_MAX_ITEMS;
	m_size = 0;
}

Set::Set(int n)
{
	m_set = new ItemType[n];
	m_maxSize = n;
	m_size = 0;
}

Set::Set(const Set &src)
{
	this->m_size = src.m_size;
	this->m_maxSize = src.m_maxSize;
	this->m_set = new ItemType[m_maxSize];
	for (int i = 0; i < m_size; i++)
	{
		m_set[i] = src.m_set[i];
	}
}

Set& Set::operator=(const Set & src)
{
	if (&src == this)
		return *this;
	delete[] m_set;
	m_size = src.m_size;
	m_maxSize = src.m_maxSize;
	m_set = new ItemType[m_size];
	for (int i = 0; i < m_size; i++)
	{
		m_set[i] = src.m_set[i];
	}
	return *this;
}

Set::~Set()
{
	delete[] m_set;
}
bool Set::empty() const
{
	if (m_size == 0)
		return true;
	return false;
}

int Set::size() const
{
	return m_size;
}

bool Set::insert(const ItemType& value)
{
	if (m_size == m_maxSize)
		return false;
	if (contains(value))
		return false;
	m_set[m_size] = value;
	m_size++;
	return true;
}

bool Set::erase(const ItemType& value)
{
	if (!contains(value))
		return false;
	for (int i = 0; i < m_size; i++)
	{
		if (m_set[i] == value)
		{
			for (int j = i; j < m_size - 1; j++)
			{
				m_set[i] = m_set[i + 1];
			}
			m_size--;
			i = -1; //Repeat!
		}
	}
	return true;
}

bool Set::contains(const ItemType& value) const
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_set[i] == value)
			return true;
	}
	return false;
}

bool Set::get(int i, ItemType& value) const
{
	for (int j = 0; j < m_size; j++)
	{
		int count = 0;
		for (int k = 0; k < m_size; k++)
		{
			if (m_set[j] > m_set[k])
				count++;
		}
		if (count == i)
		{
			value = m_set[j];
			return true;
		}
	}
	return false; //check
}

void Set::swap(Set& other)
{
	Set temp = other;
	other = *this;
	*this = temp;
}

