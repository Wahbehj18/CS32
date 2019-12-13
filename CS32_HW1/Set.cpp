#include "Set.h"
#include <string>
#include <iostream>
using namespace std;


Set::Set()
	:m_arr()
{
	m_size = 0;
}


bool Set::empty() const
{
	if (m_size == 0)
		return true;
	else
		return false;
}


int Set::size() const
{
	return m_size;
}


bool Set::contains(const ItemType& value) const
{
	for (int i = 0; i < m_size; i++)
		if (m_arr[i] == value)
			return true;

	return false;
}

bool Set::insert(const ItemType& value)
{
	if (m_size == DEFAULT_MAX_ITEMS) //cant add more than max
		return false;

	if (contains(value))  // check for duplicates
		return false;

	m_arr[m_size] = value;
	m_size++;				//increase size with each new item
	return true;
}

bool Set::erase(const ItemType& value)
{
	if (!contains(value))  // if item isnt in Set return false
		return false;

	for (int i = 0; i < m_size; i++)   //find value and swap it with last item
		if (m_arr[i] == value)
		{
			ItemType temp = m_arr[i];
			m_arr[i] = m_arr[m_size];
			m_arr[m_size] = temp;
		}

	m_size--;   //decriminate size to account for deleted item
	return true;
}

bool Set::get(int i, ItemType& value) const
{
	if (i < 0 || i >= m_size)		//return false if i is out of bounds
		return false;

	ItemType cArr[DEFAULT_MAX_ITEMS];	//create a copy of the array

	for (int t = 0; t < m_size; t++)
		cArr[t] = m_arr[t];

	for (int j = 0; j < m_size; j++)
		for (int k = j + 1; k < m_size; k++)		//sorts copy array into 
			if (cArr[k] < cArr[j])				//ascending order
			{
				ItemType temp = cArr[j];
				cArr[j] = cArr[k];
				cArr[k] = temp;
			}

	value = cArr[i];


	return true;
}

void Set::swap(Set& other)
{
	
	int max;
	if (m_size > other.m_size)
		max = m_size;				//chooses largest of the two array sizes
	else
		max = other.m_size;

	for (int i = 0; i < max; i++)
	{								//loops through arrays and swaps each object
		ItemType temp = m_arr[i];
		m_arr[i] = other.m_arr[i];
		other.m_arr[i] = temp;
	}

	int temp = this->m_size;			//swaps the sizes of the arrays
	this->m_size = other.m_size;
	other.m_size = temp;
}

void Set::dump() const
{
	for (int i = 0; i < m_size; i++)
		cerr << m_arr[i] << " ";
	cerr << endl;
}