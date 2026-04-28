#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <string>
#include <sstream>
#include "HashMap.h"

namespace osl
{
	class Cache : public Object
	{
		protected:
		Node<Block>* head;
		Node<Block>* tail;
		HashMap values;

		private:
		size_t maxCapacity;

		Cache(const Cache&) = delete;
		Cache& operator=(const Cache&) = delete;

		public:
		Cache() : head(nullptr), tail(nullptr), maxCapacity(50) {}

		// sets CAP to param if valid, otherwise defaults to 50
		Cache(size_t capacity) : head(nullptr), tail(nullptr)
		{
			maxCapacity = (capacity >= 2 && capacity <= 50) ? capacity : 50;
		}

		// deallocate the linked list
		virtual ~Cache()
		{
			clear(head);
		}

		size_t capacity() const { return maxCapacity; }

		// print each block in the list on its own line
		std::string toString() const override
		{
			std::stringstream stream;
			Node<Block>* currentNode = head;
			while(currentNode != nullptr)
			{
				stream << currentNode->data().toString() << "\n";
				currentNode = currentNode->next();
			}
			return stream.str();
		}

		virtual Node<Block>* read(size_t key) = 0;
		virtual void write(Block& obj) = 0;
	};
}

#endif
