#include "Set.h"

Set::Set()
{
	m_size = 0;
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
	if (m_size == DEFAULT_MAX_ITEMS)
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
	return false;
}

void Set::swap(Set& other)
{
	Set* temp = &other;
	int size;
	if (this->m_size > temp->m_size)
		size = this->m_size;
	else
		size = temp->m_size;
	for (int i = 0; i < size; i++)
	{
		ItemType swap = this->m_set[i];
		this->m_set[i] = temp->m_set[i];
		temp->m_set[i] = swap;
	}
	int tempSize = temp->m_size;
	temp->m_size = this->m_size;
	this->m_size = tempSize;
}

