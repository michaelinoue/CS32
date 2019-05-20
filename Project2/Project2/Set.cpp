#include "Set.h"

Set::Set()
{
	head = nullptr; //no nodes; head and tail must point to nothing
	tail = nullptr;
}

Set::Set(const Set& src)
{
	head = tail = nullptr; //this will allow us to easily use the insert function developed below
	Node* p = src.head;
	while (p != nullptr)
	{
		this->insert(p->value);  //insert every value into object that is copying src
		p = p->next;
	}
}

Set& Set::operator=(const Set& src)
{
	if (this == &src)
		return *this;
	for (int i = 0; i < this->size(); i++)
	{
		ItemType temp;
		get(i, temp);
		this->erase(temp); //get rid of every single item in this object
		i = -1; //reset i to -1 so that we can hit every value (will become i = 0 after the next iteration)
	}
	head = tail = nullptr; //this will allow us to easily use the insert function developed below
	Node* q = src.head;
	while (q != nullptr)
	{
		this->insert(q->value); //copy every item into this object from src
		q = q->next;
	}
	return *this;
}

Set::~Set()
{
	Node* p = head;
	while (p != nullptr) //loop through, deleting every Node
	{
		Node* n = p->next;
		delete p;
		p = n;
	}
}
bool Set::empty() const
{
	if (head == nullptr) //if head points to nothing, there are no Nodes
		return true;
	return false;
}

int Set::size() const
{
	if (head == nullptr) //there are no elements in the list; don't look; return size (0)
		return 0;
	int count = 0;
	Node* p = head;
	while (p != nullptr)
	{
		count++;
		p = p->next;
	}
	return count;
}

bool Set::insert(const ItemType& value)
{
	if (this->contains(value)) //check if value is already there
		return false;
	if (head == nullptr) //we need to check if there is no elements so we can appropriately allocated the head's elements and avoid checking head->next
	{
		head = new Node;
		head->value = value;
		head->prev = nullptr;
		head->next = nullptr;
		tail = head; //if there is only one element, tail and head point to the same element (i.e. effectively the same pointer)
	}
	else
	{
		Node* p = new Node;
		p->value = value;
		tail->next = p; //First, we link up the last value to the new Node* p
		p->prev = tail; //and vice versa for previous
		p->next = nullptr;
		tail = p; //p is the last element; thus, point tail to Node pointed to by p
	}
	return true;
}

bool Set::erase(const ItemType& value)
{
	if (head == nullptr) //there are no elements; thus, can't erase anything
		return false;
	Node* p = head;
	if (head == tail && head->value == value) //if there is only one element in the linked list and it matches the value, delete it
	{
		delete head; //get rid of the value; set head and tail to nullptr because they now don't point to anything
		head = tail = nullptr;
		return true;
	}
	if (head->value == value) //if we delete the head, simply link up the second node to the head pointer and give it a nullptr value for prev
	{
		Node* kill = head;
		head = head->next;
		head->prev = nullptr;
		delete kill;
		return true;
	}
	while (p != nullptr && p->next != nullptr)
	{
		if (p->next->value == value) //we found the value; break out so we can stop checking
			break;
		p = p->next;
	}
	if (p == nullptr || p->next == nullptr) //we reached the end and didn't find the value; return false 
		return false;
	else
	{
		Node* kill = p->next; //saving Node so we can remember where it was
		if (tail == p->next) //checking if p->next is the last value so we can reassign tail and don't go too far
		{
			tail = p; //reassign tail to Node before
			p->next = nullptr;
			delete kill; //delete desired Node
			return true;
		}
		kill->next->prev = p; //linking up Node after deleted Node with Node before deleted Node
		p->next = kill->next;
		delete kill;
	}
	return true;
}

bool Set::contains(const ItemType& value) const
{
	Node* p = head;
	while (p != nullptr) //loop through every Node, searching for value
	{
		if (p->value == value)
			return true;
		p = p->next;
	}
	return false;
}

bool Set::get(int pos, ItemType& value) const
{
	Node* p = head;
	while (p != nullptr)
	{
		int count = 0;
		Node* q = head;
		while (q != nullptr)
		{
			if (p->value > q->value) //essentially, we are comparing a Node's value with every other Node value
				count++; //the moment it finds a value that is smaller than itself, we increment count so we can track its comparative size
			q = q->next;
		}
		if (count == pos) //after we compare it to every Node, count must be exactly the same as pos (strictly greater than exactly pos values)
		{
			value = p->value;
			return true;
		}
		p = p->next;
	}
	return false;
}

void Set::swap(Set& other) //just need to swap the head and tail pointers, using temp Node pointers to accomplish this task
{
	Node* tempHead = head;
	head = other.head;
	other.head = tempHead;
	Node* tempTail = tail;
	tail = other.tail;
	other.tail = tempTail;
}

void unite(const Set& s1, const Set& s2, Set& result)
{
	if (&result == &s1 && &result == &s2) //if the sets are all the same, we don't need to do anything; they are already united
		return;
	if (&result == &s1) //if result is s1, then we can simply add the elements of s2, so long as they aren't already in result
	{
		for (int i = 0; i < s2.size(); i++)
		{
			ItemType temp;
			s2.get(i, temp);
			result.insert(temp); //now, we insert every item from s2 (repetitions shouldn't be added; insert should check already for that)
		}
		return;
	}

	if (&result == &s2) //likewise, if result is s2, we do the same process and and simply add the elements of s1, so long as they aren't already in result
	{
		for (int i = 0; i < s1.size(); i++)
		{
			ItemType temp;
			s1.get(i, temp);
			result.insert(temp); //now, we insert every item from s1 (repetitions shouldn't be added; insert should check already for that)
		}
		return;
	}
	for (int i = 0; i < result.size(); i++) 
	{
		ItemType temp;
		result.get(i, temp); //finds every value of result and deletes it; this way, we can cleanly add each element from s1 and s2
		result.erase(temp);
		i = -1; //this is to allow us to delete every element using the get function; i needs to be reset back to 0 (will happen after iteration)
	}
	for (int i = 0; i < s1.size(); i++)
	{
		ItemType temp;
		s1.get(i, temp); //we use get to get every single item in s1 and then insert said items into temp
		result.insert(temp);
	}
	 
	for (int i = 0; i < s2.size(); i++)
	{
		ItemType temp;
		s2.get(i, temp);
		result.insert(temp); //now, we insert every item from s2 (repetitions shouldn't be added; insert should check already for that)
	}
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
	Set tempSet = s1;
	ItemType temp; //create a temporary set so that we can remove repeated values without touching s1
	for (int i = 0; i < s2.size(); i++)
	{
		s2.get(i, temp);
		if (s1.contains(temp)) //if we find a repetition, just remove value from tempSet
			tempSet.erase(temp);
	}
	result = tempSet; //finally, our result should be equal to tempSet (s1 - repeated values in s2)
}

#include <iostream>
int main()
{
	Set t; 
	t.insert("hi");
	Set s;
	s.insert("hi");
	Set result;
	result.insert("Wow");

	subtract(t, s, result);
		if (!result.contains("Wow"))
			std::cout << "yay";
	
}





