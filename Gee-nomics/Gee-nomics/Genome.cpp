#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
	string m_sequence;
	string m_name;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
	m_sequence = sequence;
	m_name = nm;
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
	string name;
	string sequence;
	string s;
	while (getline(genomeSource, s))
	{
		if (s == "")
			return false;
		if (s[0] == '>') //check for double name?
		{
			name = s.substr(1, s.size() - 1);
			if (sequence.size() != 0)
			{
				Genome genome(name, sequence);
				genomes.push_back(genome);
				sequence = "";
			}
		}
		else
		{
			for (int i = 0; i < s.size(); i++)
			{
				char temp = toupper(s[i]);
				if (temp != 'A' && temp != 'C' && temp != 'G' && temp != 'T' && temp != 'N')
					return false;
				sequence += temp;
			}
		}
	}
	if (sequence.size() != 0 && name.size() != 0)
	{
		Genome genome(name, sequence); 
		genomes.push_back(genome);
	}

    return true; 
}

int GenomeImpl::length() const
{
    return m_sequence.size();  
} 

string GenomeImpl::name() const
{
    return m_name; 
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	if (position + length > m_sequence.size())
		return false;
	fragment = m_sequence.substr(position, length);
    return true; 
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
