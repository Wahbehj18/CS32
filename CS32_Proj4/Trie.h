#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
using namespace std;

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

	struct Child;

	struct Node
	{
		vector<ValueType> value;
		vector<Child> children;
	};

	struct Child
	{
		char letter;
		Node* nextNode;
	};


	Node* root;

	void deleteAll(Node* ptr);
	std::vector<ValueType> findHelper(Node* ptr, const std::string& key, bool exactMatchOnly) const;

	void newNode(Node* &ptr)
	{
		ptr = new Node;
	}

	void addChild(Node* &ptr, char ltr)
	{
		Child c;
		c.letter = ltr;
		c.nextNode = nullptr;
		ptr->children.push_back(c);
	}
};

template<typename ValueType>
Trie<ValueType>::Trie()
{
	root = nullptr;
}

template<typename ValueType>
void Trie<ValueType>::deleteAll(typename Trie<ValueType>::Node* ptr)
{
	if (ptr != nullptr)
	{
		for (int i = 0; i != ptr->children.size(); i++)
			deleteAll(ptr->children[i].nextNode);

		delete ptr;
	}
}

template<typename ValueType>
Trie<ValueType>::~Trie()
{
	deleteAll(root);
}

template<typename ValueType>
void Trie<ValueType>::reset()
{
	deleteAll(root);
	root = nullptr;
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value)
{
	if (root == nullptr)
		newNode(root);
	Node* current = root;

	for (int i = 0; i != key.size(); i++)
	{
		if (current->children.size() == 0)
			addChild(current, key[i]);


		bool addNewNode = true;

		int j = 0;
		for (; j != current->children.size() && addNewNode; j++)
		{
			if (current->children[j].letter == key[i])
			{
				if(current->children[j].nextNode == nullptr)
					current->children[j].nextNode = new Node;
				current = current->children[j].nextNode;
				addNewNode = false;
			}
		}

		if (addNewNode)
		{
			addChild(current, key[i]);
			current->children[j].nextNode = new Node;
			current = current->children[j].nextNode;
		}
	}

	current->value.push_back(value);
}

template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::findHelper(Node* ptr, const std::string& key, bool exactMatchOnly) const
{
	vector<ValueType> t;

	if (key.size() == 0)
		return ptr->value;
	for (int i = 0; i != ptr->children.size(); i++)
	{
		if (ptr->children[i].letter == key[0] && exactMatchOnly)
		{
			vector<ValueType> v = findHelper(ptr->children[i].nextNode, key.substr(1), true);
			t.insert(t.end(), v.begin(), v.end());
		}
		else if (ptr->children[i].letter == key[0] && !exactMatchOnly)
		{
			vector<ValueType> v = findHelper(ptr->children[i].nextNode, key.substr(1), false);
			t.insert(t.end(), v.begin(), v.end());
		}
		else if (ptr->children[i].letter != key[0] && !exactMatchOnly)
		{
			vector<ValueType> v = findHelper(ptr->children[i].nextNode, key.substr(1), true);
			t.insert(t.end(), v.begin(), v.end());
		}

	}

	return t;
}

template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const
{
	vector<ValueType> t;
	Node* current = root;

	for (int i = 0; i != current->children.size() && current != nullptr; i++)
	{
		if (current->children[i].letter == key[0])
		{
			vector<ValueType> v = findHelper(current->children[i].nextNode, key.substr(1), exactMatchOnly);
			t.insert(t.end(), v.begin(), v.end());
		}
	}

	return t;
}


#endif // TRIE_INCLUDED
