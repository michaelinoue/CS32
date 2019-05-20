#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
	struct Node
	{
		char m_label;
		std::vector<Node*> children;
		std::vector<ValueType> m_values;
	};
	Trie* root;
	std::vector<Node*> children;
	void helpInsert(const std::string& key, const ValueType& value, Node* node);
	std::vector<ValueType>  helpFind(const std::string& key, bool exactMatchOnly, Node* node) const;
	void clearMemory(Node* child);
};

template<typename ValueType>
void Trie<ValueType>::clearMemory(Node* child) 
{
	if (child->children.size() == 0) //get rid of child if size is 0
	{
		delete child;
		return;
	}
	for (int i = 0; i < child->children.size(); i++)
		clearMemory(child->children[i]); //get rid of child's children
	delete child;
}

template<typename ValueType>
Trie<ValueType>::Trie()
{
	root = this;
}

template<typename ValueType>
Trie<ValueType>::~Trie() 
{
	for (int i = 0; i < children.size(); i++)
	{
		clearMemory(children[i]);
	}
}

template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::helpFind(const std::string& key, bool exactMatchOnly, Node* node) const
{
	if (exactMatchOnly)
	{
		if (key.size() == 1)
		{
			for (int i = 0; i < node->children.size(); i++)
			{
				if (node->children[i]->m_label == key[0]) //We only have one letter; if it's in the Trie, then we should return whatever its values are; else return empty vector
					return node->children[i]->m_values;
			}
			return {};
		}
		for (int i = 0; i < node->children.size(); i++)
		{
			if (node->children[i]->m_label == key[0])
			{
				return helpFind(key.substr(1, key.size() - 1), true, node->children[i]); //Here, we reduce the key down, eliminating the first letter, and we check the child to see what their label is, recursively
			}
		}
		return {};
	}
	else
	{
		std::vector<ValueType> values;
		if (key.size() == 1)
		{
			for (int i = 0; i < node->children.size(); i++)
			{
				values.insert(values.end(), node->children[i]->m_values.begin(), node->children[i]->m_values.end()); //Doesn't matter at this point what the child's label is; we've reached the end of the sequence by this point, and exactMatch is still false; store said values
			}
		}

		for (int i = 0; i < node->children.size(); i++)
		{
			if (node->children[i]->m_label != key[0])
			{
				std::vector<ValueType> temp;
				temp = helpFind(key.substr(1, key.size() - 1), true, node->children[i]); //Here, we iterate through each child and check their children
				values.insert(values.end(), temp.begin(), temp.end());
			}
			else
			{
				std::vector<ValueType> temp = helpFind(key.substr(1, key.size() - 1), false, node->children[i]);
				values.insert(values.end(), temp.begin(), temp.end());
			}
		}
		return values;
	}
}

template<typename ValueType>
void Trie<ValueType>::helpInsert(const std::string& key, const ValueType& value, Node* node)
{
	if (key.size() == 0)
		return;
	if (key.size() == 1)
	{
		for (int i = 0; i < node->children.size(); i++)
		{
			if (node->children[i]->m_label == key[0])
			{
				node->children[i]->m_values.push_back(value);
				return;
			}
		}
		Node* child = new Node();
		child->m_label = key[0];
		child->m_values.push_back(value);
		node->children.push_back(child);
		return;
	}
	else
	{
		for (int i = 0; i < node->children.size(); i++)
		{
			if (node->children[i]->m_label == key[0])
			{
				helpInsert(key.substr(1, key.size() - 1), value, node->children[i]);
				return;
			}
		}
		Node* child = new Node();
		child->m_label = key[0];
		node->children.push_back(child);
		helpInsert(key.substr(1, key.size() - 1), value, child);
	}
}


template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value)
{
	if (key.size() == 0)
		return;
	if (key.size() == 1)
	{
		for (int i = 0; i < children.size(); i++)
		{
			if (children[i]->m_label == key[0])
			{
				children[i]->m_values.push_back(value);
				return;
			}
		}
		Node* child = new Node();
		child->m_label = key[0];
		child->m_values.push_back(value);
		children.push_back(child);
		return;	
	}
	else
	{
		for (int i = 0; i < children.size(); i++)
		{
			if (children[i]->m_label == key[0])
			{
				helpInsert(key.substr(1, key.size() - 1), value, children[i]);
				return;
			}
		}
		Node* child = new Node();
		child->m_label = key[0];
		children.push_back(child);
		helpInsert(key.substr(1, key.size() - 1), value, child);
	}
}


template<typename ValueType> std::vector<ValueType>  Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const
{
	if (exactMatchOnly)
	{
		if (key.size() == 1)
		{
			for (int i = 0; i < children.size(); i++)
			{
				if (children[i]->m_label == key[0])
				{
					return children[i]->m_values;
				}
			}
			return {};
		}
		for (int i = 0; i < children.size(); i++)
		{
			if (children[i]->m_label == key[0])
			{
				return helpFind(key.substr(1, key.size() - 1), true, children[i]);
			}
		}
		return {};
	}
	else
	{
		Node* child = nullptr;
		std::vector<ValueType> values;
		bool firstValid = false;
		for (int i = 0; i < children.size(); i++)
		{
			if (children[i]->m_label == key[0])
			{
				firstValid = true;
				child = children[i];
				if (key.size() == 1)
				{
					return (child->m_values);
				}
				break;
			}
		}
		if (!firstValid)
			return {};
		if (key.size() == 1)
			values.insert(values.end(), child->m_values.begin(), child->m_values.end());
		else
		{
			std::vector<ValueType> temp = helpFind(key.substr(1, key.size() - 1), false, child);
			values.insert(values.end(), temp.begin(), temp.end());
		}
		return values;
	}
}

template<typename ValueType> void Trie<ValueType>::reset()
{
	Trie* t = root;
	root->~Trie();
	t = new Trie();
} 

#endif // TRIE_INCLUDED
