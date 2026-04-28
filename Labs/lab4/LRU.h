#ifndef LRU_H
#define LRU_H

#include <iostream>
#include <string>
#include "Cache.h"

namespace osl
{
	class LRUCache : public Cache
	{
		private:
		size_t itemCount;

		LRUCache(const LRUCache&) = delete;
		LRUCache& operator=(const LRUCache&) = delete;

		public:
		LRUCache() : Cache(5), itemCount(0) {}

		virtual ~LRUCache() {}

		// move accessed node to head (most recently used), return it
		Node<Block>* read(size_t tagKey) override
		{
			if(!values.contains(tagKey)) return nullptr;

			Node<Block>* accessedNode = values[tagKey];

			if(accessedNode != head)
			{
				// detach node from its current position
				if(accessedNode->prev()) accessedNode->prev()->next() = accessedNode->next();
				if(accessedNode->next()) accessedNode->next()->prev() = accessedNode->prev();
				if(accessedNode == tail) tail = accessedNode->prev();

				// insert at head
				accessedNode->prev() = nullptr;
				accessedNode->next() = head;
				if(head) head->prev() = accessedNode;
				head = accessedNode;
			}

			return accessedNode;
		}

		// update content if tag exists, otherwise add to head and evict tail if full
		void write(Block& incomingBlock) override
		{
			if(values.contains(incomingBlock.tag()))
			{
				values[incomingBlock.tag()]->data().content() = incomingBlock.content();
			}
			else
			{
				values.put(incomingBlock);
				Node<Block>* newNode = values[incomingBlock.tag()];

				// insert at head
				newNode->next() = head;
				if(head) head->prev() = newNode;
				head = newNode;
				if(tail == nullptr) tail = newNode;
				itemCount++;

				// evict least recently used (tail) if at capacity
				if(itemCount == capacity())
				{
					Node<Block>* evictedNode = tail;
					values[evictedNode->data().tag()] = nullptr;

					if(tail->prev())
					{
						tail->prev()->next() = nullptr;
						tail = tail->prev();
					}
					else
					{
						head = nullptr;
						tail = nullptr;
					}

					delete evictedNode;
					itemCount--;
				}
			}
		}
	};
}

#endif
