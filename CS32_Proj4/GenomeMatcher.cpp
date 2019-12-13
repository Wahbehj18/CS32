#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Trie.h"
#include <set>
#include <map>
#include <list>
#include <algorithm>
using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	int m_minSL;

	struct GenPos
	{
		const Genome* gen;
		int pos;
	};

	Trie<GenPos> m_trie;
	list<Genome> m_vec;

};

struct DNAmatchImpl
{
	std::string genomeName;
	int length;
	int position;
	bool operator<(const DNAmatchImpl rhs) const
	{
		if (length < rhs.length)
			return true;
		else if (length > rhs.length)
			return false;
		else
			return position > rhs.position;
	}
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
	m_minSL = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
	m_vec.push_back(genome);
	list<Genome>::const_reverse_iterator it = m_vec.crbegin();
	
	for (int i = 0; i < genome.length(); i++)
	{
		GenPos g;
		g.gen = &(*it);
		g.pos = i;

		string fragment;
		if (genome.extract(i, m_minSL, fragment))
			m_trie.insert(fragment, g);
	}

}

int GenomeMatcherImpl::minimumSearchLength() const
{
	return m_minSL;
}

bool sameName(string& name, vector<DNAMatch> matches)
{
	for (int i = 0; i != matches.size(); i++)
	{
		if (name == matches[i].genomeName)
			return true;
	}
	return false;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	if (fragment.size() < minimumLength)
		return false;
	if (minimumLength < m_minSL)
		return false;

	matches.clear();

	set<DNAmatchImpl> test;

	vector<GenPos> prefix = m_trie.find(fragment.substr(0, m_minSL), exactMatchOnly);

	if (prefix.size() == 0)
		return false;

	if (exactMatchOnly)
	{
		for (int i = 0; i != prefix.size(); i++)
		{
			string suffix;
			int length = m_minSL;
			if (prefix[i].gen->extract(prefix[i].pos + m_minSL, fragment.size() - m_minSL, suffix))
			{
				for (int k = 0; k != fragment.size() - m_minSL && suffix[k] == fragment[k + m_minSL]; k++)
				{
					length++;
				}
			}

			if (length >= minimumLength)
			{
				DNAmatchImpl d;
				d.genomeName = prefix[i].gen->name();
				d.position = prefix[i].pos;
				d.length = length;
				test.insert(d);
			}
		}
	}

	else
	{
		for (int i = 0; i != prefix.size(); i++)
		{
			string suffix;
			string root;
			int deviations = 0;
				
			prefix[i].gen->extract(prefix[i].pos, m_minSL, root);

			int length = m_minSL;

			if (prefix[i].gen->extract(prefix[i].pos + m_minSL, fragment.size() - m_minSL, suffix))
			{

				if (fragment.substr(0, m_minSL) == root)  //Snip deviation not in prefix
				{
					for (int k = 0; k != fragment.size() - m_minSL; k++)
					{
						if (suffix[k] != fragment[k + m_minSL])
							deviations++;

						if (deviations <= 1)
							length++;
					}

				}
				else
				{
					for (int k = 0; k != fragment.size() - m_minSL && suffix[k] == fragment[k + m_minSL]; k++)
					{
						length++;
					}
				}
			}

			if (length >= minimumLength)
			{
				DNAmatchImpl d;
				d.genomeName = prefix[i].gen->name();
				d.position = prefix[i].pos;
				d.length = length;
				test.insert(d);
			}
		}
	}

	if (test.empty())
		return false;

	set<DNAmatchImpl>::iterator it = test.end();
	it--;
	for (int i = 0; i != test.size(); i++)
	{
		DNAMatch d;
		d.genomeName = (*it).genomeName;
		d.length = (*it).length;
		d.position = (*it).position;
		if(!sameName(d.genomeName, matches))
			matches.push_back(d);
		it--;
	}

	if (matches.empty())
		return false;

	return true;



	
}


bool compare(const GenomeMatch& lhs, const GenomeMatch& rhs)
{
	if (lhs.percentMatch > rhs.percentMatch)
		return true;
	else if (lhs.percentMatch < rhs.percentMatch)
		return false;
	else return lhs.genomeName < rhs.genomeName;
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	if (fragmentMatchLength < m_minSL)
		return false;

	results.clear();
	vector<DNAMatch> matches;
	map<string, double> numMatches;
	list<Genome>::const_iterator it;
	string subseq;
	int count = 0;
	int S = query.length() / fragmentMatchLength;
	for (it = m_vec.cbegin(); it != m_vec.cend(); it++)
	{
		numMatches[(*it).name()] = 0;
	}

	for (int i = 0; i < S; i++)
	{
		query.extract(i*fragmentMatchLength, fragmentMatchLength, subseq);
		findGenomesWithThisDNA(subseq, fragmentMatchLength, exactMatchOnly, matches);
		for (int i = 0; i != matches.size(); i++)
		{
			numMatches[matches[i].genomeName]++;
		}
	}


	for (it = m_vec.cbegin(); it != m_vec.cend(); it++)
	{
		if (numMatches[(*it).name()] / S >= matchPercentThreshold/100)
		{
			GenomeMatch m;
			m.genomeName = (*it).name();
			m.percentMatch = 100 * (numMatches[(*it).name()] / S);
			results.push_back(m);
		}

	}

	if (results.empty())
		return false;

	sort(results.begin(), results.end(), compare);
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

