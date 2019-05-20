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
	Trie* root;
	std::vector<Trie*> children;
	std::vector<ValueType> m_values;
	char m_label;
};

template<typename ValueType>
Trie<ValueType>::Trie()
{
	root = this;
	children.push_back(nullptr);
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
		Trie* child = new Trie();
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
				children[i]->insert(key.substr(1, key.size() - 1), value);
				return;
			}
		}
		Trie* child = new Trie();
		child->m_label = key[0];
		children.push_back(child);
		child->insert(key.substr(1, key.size() - 1), value);
		return;
	}
}

template<typename ValueType> std::vector<ValueType>  Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const
{
	if (exactMatchOnly)
	{
		if (key.size() == 0)
		{
			for (int i = 0; i < children.size(); i++)
			{
				if (children[i]->m_label == key[0])
					return children[i]->m_values;
			}
			return {};
		}
		for (int i = 0; i < children.size(); i++)
		{
			if (children[i]->m_label == key[0])
			{
				if (children[i]->m_label == key[0])
					return children[i]->find(key.substr(key[1], key.size() - 1), true);
			}
		}
		return {};
	}
}


#endif // TRIE_INCLUDED

