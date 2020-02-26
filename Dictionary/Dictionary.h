#pragma once
#include <iostream>

namespace Containers {

	template<class Key, class Item>
	class Dictionary
	{
	public:
		
		//Defualt Constructor.
		Dictionary() = default;

		//Copy Constructor.
		Dictionary(const Dictionary&);

		//Move Constructor.
		Dictionary(Dictionary&&);

		//Move and Copy Assignment Operators.
		Dictionary<Key, Item>& operator=(const Dictionary&);
		Dictionary<Key, Item>& operator=(Dictionary&&);
		
		//Destructor
		~Dictionary();

		bool insert(Key, Item);
		bool remove(Key);

		//RemoveIF returns true if key is absent in list.
		bool (Dictionary::* removeIf)(std::string) = &Dictionary::remove;
		void displayDictionary();
		Item* lookup(Key);

	private:
		struct Node
		{

			Key key;
			Item item;
			//ptrNext points to the next node in the list.
			Node* ptrNext;

			//Initialise varibles
			Node(Key key, Item item) :
				key(key),
				item(item),
				ptrNext(nullptr){}

		};

		bool insertRec(Node*&, Key, Item);
		bool removeRec(Node*&, Key);
		Item* lookupRec(Node*, Key);
		Node* root = nullptr;
		Node* tail = nullptr;
		Node* prev = nullptr;
		static Node* deepCopy(Node*);
		static void deepDelete(Node*&);
		void displayListPrivate(Node*);
		
	};
	
	//Copy Constructor.
	template<class Key, class Item>
	Dictionary<Key, Item>::Dictionary(const Dictionary& original)
	{
		this->root = deepCopy(original.root);
	}

	//Copy Assignment Operator.
	template<class Key, class Item>
	Dictionary<Key, Item>& Dictionary<Key, Item>::operator=(const Dictionary& original)
	{
		//Check if objects are the same.
		if (this == &original)
		{
			return *this;
		}
		deepDelete(root);
		root = deepCopy(original.root);
		return *this;
	}

	//Move Constructor.
	template<class Key, class Item>
	Dictionary<Key, Item>::Dictionary(Dictionary&& original)
	{
		//Steal data from temporary object then set to valid state.
		this->root = original.root;
		original.root = nullptr;
		

	}

	// Move Assignment Operator.
	template<class Key, class Item>
	Dictionary<Key, Item>& Dictionary<Key, Item>::operator=(Dictionary&& original)
	{
		//Check if objects are the same.
		if (this == &original)
		{
			return *this;
		}

		//Clean up existing resorces
		deepDelete(root);

		//Steal data from rvalue reference (Temp object copy that we stealing from).
		this->root = deepCopy(original.root);

		//Set to valid state for possible reuse.
		original.root = nullptr;

		return *this;
	}


	template<class Key, class Item>
	inline Dictionary<Key, Item>::~Dictionary()
	{
		deepDelete(root);
	}

	template<class Key, class Item>
	bool Dictionary<Key, Item>::insert(Key key, Item item)
	{

		return insertRec(root, key, item);

	}

	template<class Key, class Item>
	bool Dictionary<Key, Item>::insertRec(Node*& currentNode, Key key, Item item)
	{
		// Creates a temp to hold the last node and set last's data and next
		Node* last = new Node(key, item);
		bool condition = true;
		
		last->ptrNext = nullptr;

		// If the linked list is empty then set head = last
		if (currentNode == nullptr) {
			currentNode = last;
			
		}
		else {
			// Creates a temp node and sets it to head
			Node* temp = currentNode;
			if (temp->key == key)
			{
				// return false because key is already present in list.
				return false;
			}
			// Uses temp to find the last node
			while (temp->ptrNext != nullptr) {
				if (temp->key == key)
				{
					// return false because key is already present in list.
					return false;
				}
				temp = temp->ptrNext;
				
			}

			// Appends the last node with last
			temp->ptrNext = last;
		}

		// Returns true because of no key match 

		return true;

	}

	template<class Key, class Item>
	Item* Dictionary<Key, Item>::lookup(Key key)
	{
		return lookupRec(root, key);
	}

	template<class Key, class Item>
	Item* Dictionary<Key, Item>::lookupRec(Node* currentNode, Key key)
	{

		if (currentNode != nullptr)
		{
			// If key == nodes key then that nodes item will be returned
			if (currentNode->key == key)
			{
				//return pointer to node
				return &currentNode->item;
			}
			else
			{
				return lookupRec(currentNode->ptrNext, key);
				
			}
		}

		return nullptr;

	}

	template<class Key, class Item>
	bool Dictionary<Key, Item>::remove(Key key)
	{
		return removeRec(root, key);
	}

	template<class Key, class Item>
	bool Dictionary<Key, Item>::removeRec(Node*& currentNode, Key key)
	{

		
		if (root == nullptr) return false; // List is empty.
		if (root->key == key) {
			Node* temp = root;
			root = temp->ptrNext;
			delete temp;
			return true;
		}
		Node* current = root;
		while (current->ptrNext) {
			if (current->key == key) {
				Node* temporary = current->ptrNext;
				prev->ptrNext = temporary;
				delete current;
				return true;

			}
			prev = current;
			current = current->ptrNext;
		}
		return false;
	}

	template<class Key, class Item>
	void Dictionary<Key, Item>::displayDictionary()
	{
		displayListPrivate(root);
	}

	template<class Key, class Item>
	typename Dictionary<Key, Item>::Node* Dictionary<Key, Item>::deepCopy(Node* original)
	{
		// if list is empty return nullptr
		if (original == nullptr)
		{
			return nullptr;
		}
		// Copying Node information.
		Node* newNode = new Node(original->key, original->item);
		newNode->ptrNext = deepCopy(original->ptrNext);

		return newNode;

	}

	template<class Key, class Item>
	void Dictionary<Key, Item>::deepDelete(Node*& currentNode)
	{
		//Delete list.
		if (currentNode == nullptr)
			return;
		
		deepDelete(currentNode->ptrNext);
		delete(currentNode);
		

	}

	//Print list
	template<class Key, class Item>
	void Dictionary<Key, Item>::displayListPrivate(Node* currentNode)
	{

		if (currentNode != nullptr) {

			std::cout << currentNode->key << " : " << currentNode->item << std::endl;
			displayListPrivate(currentNode->ptrNext);
		}

	}










}

