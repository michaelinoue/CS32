#include "provided.h"
#include <string>
#include <vector>
#include <fstream>
#include "Trie.h"
#include <unordered_map>
#include <algorithm>

using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
	~GenomeMatcherImpl();
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	int m_minimumSearchLength;
	struct Pair
	{
		const Genome* genome;
		int pos;
		string name;
	};
	Trie<Pair*>* m_pair;
	vector<Genome> genomes;
	vector<Pair*> m_savedPairs; //we'll use this to delete the pairs we created later
	static bool comp(const GenomeMatch& gm1, const GenomeMatch& gm2);
};

bool GenomeMatcherImpl::comp(const GenomeMatch& gm1, const GenomeMatch& gm2) //Comparison function used to sort to GenomeMatch objects; used by findRelatedGenomes
{
	if (gm1.percentMatch == gm2.percentMatch)
		return gm1.genomeName > gm2.genomeName;
	else
		return gm1.percentMatch > gm2.percentMatch;
}

GenomeMatcherImpl::~GenomeMatcherImpl() //destroy pairs we dynamically allocated
{
	for (int i = 0; i < m_savedPairs.size(); i++)
		delete m_savedPairs[i];
	delete m_pair; //destroy trie
}

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
	m_pair = new Trie<Pair*>(); 
	m_minimumSearchLength = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
	genomes.push_back(genome); //add to library
	for (int i = 0; i <= genome.length(); i++)
	{
		string fragment;
		if (!genome.extract(i, m_minimumSearchLength, fragment)) //we can't go any further; stop looking
			return;
		Pair* pair = new Pair(); //We will store all the information of this subsequence into a struct named Pair; this will be helpful for the function defined directly below
		pair->pos = i;
		pair->genome = &genome; 
		pair->name = fragment; //name = subsequence of genome
		m_pair->insert(fragment, pair);
		m_savedPairs.push_back(pair); //save this to delete later
	} 
}

int GenomeMatcherImpl::minimumSearchLength() const
{
    return m_minimumSearchLength;  
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	if (minimumLength > m_minimumSearchLength || fragment.size() < minimumLength) //minimumLength can't be greater (spec requirement; also wouldn't make any sense)
		return false;
	if (exactMatchOnly) //I split exactMatchOnly being true and false into 2 cases; the code is pretty much the same, but the motivation for doing this was so that I wouldn't have to check the prefix if exactMatchOnly is true (I would still have to for it being false, because I don't know if the prefix has a mismatch)
	{
		vector<Pair*> hitPairs = m_pair->find(fragment.substr(0, minimumLength), true); 
		if (hitPairs.size() == 0)
			return false;
		vector<string> hitGenomes;
		unordered_map<string, Pair*> um2;
	
		for (int i = 0; i < hitPairs.size(); i++)
		{
			string temp;
			hitPairs[i]->genome->extract(hitPairs[i]->pos, hitPairs[i]->genome->length() - hitPairs[i]->pos, temp); //Store subsequence of genome, from its position to end of sequence, into string temp
			string subsequence;
			for (int j = minimumLength; j < fragment.size(); j++)
			{
				if (j >= temp.size()) //don't check out of bounds
					break;
				if (temp[j] == fragment[j]) //Here, we keep checking until we find a mismatch, at which point, we stopping adding to subsequence and are left with our final subsequence
					subsequence += temp[j];
				else
					break;
			}
			hitPairs[i]->name += subsequence; //add subsequence to current subsequence (name)
			if (um2.find(hitPairs[i]->genome->name()) == um2.end()) //there are no repeat sequences here; feel free to insert pair
			{
				um2[hitPairs[i]->genome->name()] = hitPairs[i];
				hitGenomes.push_back(hitPairs[i]->genome->name());
			}
			else
			{
				Pair* pair1 = um2[hitPairs[i]->genome->name()];
				Pair* pair2 = hitPairs[i];
				int length1 = pair1->name.size(); //find the already existing length of the subsequence of the genome
				int length2 = pair2->name.size();
				if (length1 == length2) //Swap and place proper pair into the unordered map (depending on which has lower position, or length)
				{
					if (pair1->pos > pair2->pos)
						um2[hitPairs[i]->genome->name()] = pair2;
					continue;
				}
				if (length1 < length2)
				{
					um2[hitPairs[i]->genome->name()] = hitPairs[i];
				}
			}

		}
		for (int i = 0; i < hitGenomes.size(); i++) //Here, we simply look at the genomes that had hits, and we find their corresponding subsequences in the hash map
		{
			Pair* pair = um2[hitGenomes[i]];
			DNAMatch dnaMatch;
			dnaMatch.genomeName = pair->genome->name(); //We then use all of the information from the pair and store it into a dnaMatch object
			dnaMatch.length = pair->name.size();
			dnaMatch.position = pair->pos;
			matches.push_back(dnaMatch);
		}
		if (hitGenomes.size() != 0)
			return true;
		return false;
	}
	else //Like mentioned previously, this code is the same, except for the fact that it must check the prefix (the beginning part of the subsequence), because we don't know if a mismatch occurred, whereas before, that would be impossible. I did this to make exactMatchOnly being true slightly faster.
	{
		vector<Pair*> hitPairs = m_pair->find(fragment.substr(0, minimumLength), false);
		if (hitPairs.size() == 0)
			return false;
		vector<string> hitGenomes;
		unordered_map<string, Pair*> um2;
		for (int i = 0; i < hitPairs.size(); i++)
		{
			int count = 0;
			string temp;
			hitPairs[i]->genome->extract(hitPairs[i]->pos, hitPairs[i]->genome->length() - hitPairs[i]->pos, temp);
			string subsequence;
			for (int j = 0; j < fragment.size(); j++)
			{
				if (count > 1)
					break;
				if (j >= temp.size())
					break;
				if (temp[j] == fragment[j])
				{
					subsequence += fragment[j];
				}
				else
				{
					subsequence += temp[j];
					count++;
				}
			}
			hitPairs[i]->name = subsequence;
			if (um2.find(hitPairs[i]->genome->name()) == um2.end()) //there are no repeat sequences here; feel free to insert pair
			{
				um2[hitPairs[i]->genome->name()] = hitPairs[i];
				hitGenomes.push_back(hitPairs[i]->genome->name());
			}
			else
			{
				Pair* pair1 = um2[hitPairs[i]->genome->name()];
				Pair* pair2 = hitPairs[i];
				int length1 = pair1->name.size(); //find the already existing length of the subsequence of the genome
				int length2 = pair2->name.size();
				if (length1 == length2)
				{
					if (pair1->pos > pair2->pos)
						um2[hitPairs[i]->genome->name()] = pair2;
					continue;
				}
				if (length1 < length2)
				{
					um2[hitPairs[i]->genome->name()] = pair2;
				}
			}

		}
		for (int i = 0; i < hitGenomes.size(); i++)
		{
			Pair* pair = um2[hitGenomes[i]];
			DNAMatch dnaMatch;
			dnaMatch.genomeName = pair->genome->name();
			dnaMatch.length = pair->name.size();
			dnaMatch.position = pair->pos;
			matches.push_back(dnaMatch);
		}
		if (hitGenomes.size() != 0)
			return true;
	}
	return false;
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	string sequence;
	unordered_map<string, double> um;
	vector<string> genomes;
	vector<GenomeMatch> genomeMatch;
	int s = query.length() / fragmentMatchLength;
	for (int i = 0; i < s; i++)
	{
		if (!query.extract(i*fragmentMatchLength, fragmentMatchLength, sequence))
			break;
		vector<DNAMatch> dnaMatch;
		findGenomesWithThisDNA(sequence, fragmentMatchLength, exactMatchOnly, dnaMatch);
		for (int j = 0; j < dnaMatch.size(); j++)
		{
			if (um.find(dnaMatch[j].genomeName) == um.end())
			{
				um[dnaMatch[j].genomeName] = 1;
				genomes.push_back(dnaMatch[j].genomeName);
			}
			else
				um[dnaMatch[j].genomeName] += 1;
		}
	}
	bool foundMatch = false;
	for (int k = 0; k < genomes.size(); k++)
	{
		double numMatch = um[genomes[k]];
		if (numMatch / s * 100 >= matchPercentThreshold)
		{
			foundMatch = true;
			GenomeMatch gm;
			gm.genomeName = genomes[k];
			gm.percentMatch = numMatch / s * 100;
			genomeMatch.push_back(gm);
		}
	}

	if (!foundMatch)
		return false;
	sort(genomeMatch.begin(), genomeMatch.end(), GenomeMatcherImpl::comp);
	results.insert(results.end(), genomeMatch.begin(), genomeMatch.end());
    return true;  
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
