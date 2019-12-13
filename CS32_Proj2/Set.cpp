#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;


Set::Set()
{
	head = nullptr;
	tail = nullptr;
	m_size = 0;
}

Set::~Set()
{
	for (Node* p = head; p != nullptr;)
	{
		Node* temp = p;
		p = p->next;
		delete temp;
	}
}

Set::Set(const Set& other)
{
	head = nullptr;
	tail = nullptr;
	m_size = other.m_size;

	if (other.empty())		//empty sets
		return;

	Node* n = new Node;
	n->item = other.head->item;		//insert the first node
	head = n;						//assign to head
	head->prev = nullptr;
	head->next = nullptr;

	Node* p = other.head;

	for (int i = 1; i < other.m_size; i++)	
	{
		p = p->next;			//loop through other set
		Node* k = new Node;		//create new nodes and copy the values
		k->item = p->item;
		n->next = k;
		k->prev = n;			//link to previous and next nodes
		k->next = nullptr;
		n = n->next;
	}

	tail = n;					//assign tail to last node
}

Set& Set::operator=(const Set& other)
{
	if (this == &other)
		return *this;

	Set temp(other);		//use copy constructor to make a temporary set
	
	this->swap(temp);		//swap this set with temp
							//temp gets automatically deleted
	return *this;
	
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

bool Set::insert(const ItemType& value)
{

	if (contains(value))			//check if value is already in set
		return false;

	Node *p = new Node;			//create new node and assign its value
	p->item = value;

	if (empty())
	{
		p->next = nullptr;		//add to front if empty
		p->prev = nullptr;
		head = p;
		tail = p;
		m_size++;
		return true;
	}

	for (Node* n = head; n != nullptr; n = n->next)
	{
		if (value < head->item)
		{
			p->next = head;
			p->prev = nullptr;				//add to front
			head->prev = p;
			head = p;
			break;
		}						
		else if (value > tail->item)
		{
			p->prev = tail;
			p->next = nullptr;				//add to end
			tail->next = p;
			tail = p;
			break;
		}
		else if (value > n->item && value < n->next->item)
		{
			p->next = n->next;
			p->prev = n;
			n->next->prev = p;			//add to middle
			n->next = p;
			break;
		}
	}
	
	m_size++;
	return true;
}

bool Set::erase(const ItemType& value)
{
	if (!contains(value))
		return false;			//check if value is in set

	if (empty())
		return false;

	if (m_size == 1)
	{
		delete head;
		tail = nullptr;			//delete first node if there is only 
		head = nullptr;
		m_size--;
		return true;
	}

	if (value == head->item)
	{
		Node* killMe = head;
		head = killMe->next;
		head->prev = nullptr;	//delete node at the front 
		delete killMe;
		m_size--;
		return true;
	}
	else if (value == tail->item)
	{
		Node* killMe = tail;
		tail = killMe->prev;		//delete node at the end
		tail->next = nullptr;
		delete killMe;
		m_size--;
		return true;
	}
	else
	{
		for (Node* p = head; p->next != nullptr; p = p->next)
			if (value == p->next->item)
			{
				Node* killMe = p->next;			//delete node in the middle
				p->next = killMe->next;
				killMe->next->prev = p;
				delete killMe;
				m_size--;
				return true;
			}
	}

	return false;
}

bool Set::contains(const ItemType& value) const
{
	for (Node* q = head; q != nullptr; q = q->next)
	{
		if (q->item == value)
			return true;
	}
	return false;
}


bool Set::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= m_size)
		return false;

	Node* p = head;
	for (int i = 0; i < pos;i++)			//loop pos times through set
	{
		p = p->next;
	}

	value = p->item;
	return true;
}

void Set::swap(Set& other)
{
	Node* t1 = head;
	head = other.head;
	other.head = t1;

	Node* t2 = tail;
	tail = other.tail;
	other.tail = t2;

	int t3 = m_size;
	m_size = other.m_size;
	other.m_size = t3;
}

void Set::dump() const
{
	for (Node* p = head; p != nullptr; p = p->next)
		cerr << p->item << endl;

	cerr << "-----\n";
}

void unite(const Set& s1, const Set& s2, Set& result)
{
	Set empty;				//empty set so result is the same no matter which set is passed

	for (int i = 0; i < s1.size(); i++)
	{
		ItemType x;							//insert automatically checks if there are duplicates
		s1.get(i, x);
		empty.insert(x);
	}

	for (int i = 0; i < s2.size(); i++)
	{
		ItemType x;
		s2.get(i, x);
		empty.insert(x);
	}

	result.swap(empty);			//swaps sets and automatically deletes former result set
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
	Set empty;

	for (int i = 0; i < s1.size(); i++)
	{
		ItemType x;
		s1.get(i, x);				//loop through set 1
		if (!s2.contains(x))		//check each node's value to see if 
			empty.insert(x);		//it is contained in set 2 
	}

	result.swap(empty);
}

