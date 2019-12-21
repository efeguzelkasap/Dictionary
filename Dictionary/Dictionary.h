#pragma once
#include <iostream>

namespace Containers {

	template<class Key, class Item>
	class Dictionary
	{
	public:
		
		//Defualt, Copy and move constructors.
		Dictionary() = default;
		//Copy constructor time complexity is 0(n) because a copy of the list is required.
		//Best case is 0(1) if list is empty.
		Dictionary(const Dictionary&);
		//Time complexity of Move Constructor is 0(1) because we are making a shallow copy of the root node.
		Dictionary(Dictionary&&);

		//Move and Copy Assignment Operators.
		//Copy assignment operators time complexity of the best case is 0(1)
		//because if both objects are the exact same then that object will be returned.
		//Also because deepDelete and deepCopy are called if the list is empty the complexity will be 0(1)
		//Otherwise it will be 0(n) because the size of the list will effect the performance of the operator.
		//Worst and average case is 0(n) because a traverse is required to copy and delete nodes.
		Dictionary<Key, Item>& operator=(const Dictionary&);

		//Move assignment operator time complexity is the same as the copy assignment operator.
		Dictionary<Key, Item>& operator=(Dictionary&&);
		
		//Destructor
		//Destructor time complexity is 0(1) if list is empty or list contains a root node.
		// otherwise complexity is 0(n) because the list will contain more that one node and deepDelete is called in the destrucor. 
		~Dictionary();

		//Time complexity of insert is 0(n) in its worst and average case.
		//Inserts worst and average case are both the same because insert, inserts nodes at the end of the list.
		//The best case is when the list is empty to begin with, the time complexity of this case would be 0(1).
		//This is because when the list is empty creating a new node is independent of the size of the list.
		bool insert(Key, Item);

		//Time complexity of remove is 0(n) in its worst and average case.
		//This is because in order to remove an element, keys must match.
		//A traverse of the list is required and size of list will effect the preformance of this method.
		//Best case is 0(1) if first node is to be removed.
		bool remove(Key);

		//RemoveIF returns true if key is absent in list.
		//RemoveIf time complexity is 0(1) because all it does is call remove when called.
		bool (Dictionary::* removeIf)(std::string) = &Dictionary::remove;
		
		//Display dictionaries time complexity is 0(n) because it only has one case.
		//That case is to print the whole list regardless of how large or small it is.
		//The time it takes to execute this method will solely rely on how large the linked list is.
		void displayDictionary();

		//Time complexity of lookup is 0(n) in the average and worst case
		//In the best case it is 0(1) because the first node will be returned 
		//and would not matter how large the list is because we only access the first one.
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
		//Steal data from temporary object then set to valid state for possible reuse.
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

		Node* temp = nullptr;


		// If node to be removed is the head
		if (root->key == key)
		{

			if (currentNode->ptrNext == nullptr)
			{
				return false;
			}

			// Assign the current nodes key and item values to the next nodes.
			currentNode->key = currentNode->ptrNext->key;
			currentNode->item = currentNode->ptrNext->item;

			temp = currentNode->ptrNext;

			currentNode->ptrNext = currentNode->ptrNext->ptrNext;

			delete temp;

			return true;

		}
		// if key matches nodes key, delete it.
		else if (currentNode->key == key)
		{

			temp = currentNode->ptrNext;
			delete currentNode;
			prev->ptrNext = temp;
			return true;
		}
		else
		{
			if (currentNode->ptrNext == nullptr)
				return false;

			prev = currentNode;
			removeRec(currentNode->ptrNext, key);
		}



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

