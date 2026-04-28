#ifndef FIFO_H
#define FIFO_H

#include <iostream>
#include <string>
#include "Cache.h"

namespace osl
{
	class FIFOCache : public Cache
	{
		private:
		size_t itemCount;

		FIFOCache(const FIFOCache&) = delete;
		FIFOCache& operator=(const FIFOCache&) = delete;

		public:
		FIFOCache() : Cache(5), itemCount(0) {}

		virtual ~FIFOCache() {}

		// return the node if found, no reordering
		Node<Block>* read(size_t tagKey) override
		{
			if(!values.contains(tagKey)) return nullptr;
			return values[tagKey];
		}

		// update content if tag exists, otherwise add to tail and evict head if full
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

				// insert at tail
				newNode->prev() = tail;
				if(tail) tail->next() = newNode;
				tail = newNode;
				if(head == nullptr) head = newNode;
				itemCount++;

				// evict oldest (head) if at capacity
				if(itemCount == capacity())
				{
					Node<Block>* evictedNode = head;
					values[evictedNode->data().tag()] = nullptr;

					if(head->next())
					{
						head->next()->prev() = nullptr;
						head = head->next();
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
