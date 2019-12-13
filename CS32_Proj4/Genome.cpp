#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <istream>
#include <ctype.h>
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
	string m_name;
	string m_sequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
	:m_name(nm), m_sequence(sequence)
{
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
	string name;
	string sequence;
	char c;
	bool getName = true;
	bool newline = false;

	genomeSource.get(c);

	if (c != '>')
		return false;

	while (genomeSource.get(c))
	{
		newline = false;

		if (c == '\n')
		{
			newline = true;
			getName = false;
		}
		if (!newline && !getName && c == '>')
		{
			getName = true;
			Genome g(name, sequence);
			genomes.push_back(g);
			name = "";
			sequence = "";
		}

		if(!newline && getName)
			name += c;

		if (!newline && !getName)
		{
			if (toupper(c) != 'A' && toupper(c) != 'C' && toupper(c) != 'G' && toupper(c) != 'T' && toupper(c) != 'N')
				return false;
			else
				sequence += toupper(c);
		}
	}

	Genome g(name, sequence);
	genomes.push_back(g);

	cout << name << endl;
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
	if (position < 0 || length < 0)
		return false;
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
