#ifndef HASHMAP_H
#define HASHMAP_H

#include <iostream>
#include <string>
#include "Node.h"
#include "Block.h"

namespace osl
{
	class HashMap
	{
		private:
		mutable Node<Block>* slots[50];

		HashMap(const HashMap&) = delete;
		HashMap& operator=(const HashMap&) = delete;

		// maps key to an index in slots
		static size_t hash(size_t tagKey) { return tagKey % 50; }

		public:
		// initialize all slots to null
		HashMap()
		{
			for(size_t slot = 0; slot < 50; slot++)
				slots[slot] = nullptr;
		}

		// clear all slots on destruction
		~HashMap()
		{
			for(size_t slot = 0; slot < 50; slot++)
				slots[slot] = nullptr;
		}

		// returns true if a block with the given tag exists
		bool contains(size_t tagKey) const
		{
			size_t slotIndex = hash(tagKey);
			return slots[slotIndex] != nullptr && slots[slotIndex]->data().tag() == tagKey;
		}

		Node<Block>*& operator[](size_t tagKey)
		{
			return slots[hash(tagKey)];
		}

		const Node<Block>*& operator[](size_t tagKey) const
		{
			return *reinterpret_cast<const Node<Block>**>(static_cast<void*>(&slots[hash(tagKey)]));
		}

		// update content if tag exists, otherwise insert a new node
		void put(Block& blockObj)
		{
			size_t slotIndex = hash(blockObj.tag());
			if(slots[slotIndex] != nullptr && slots[slotIndex]->data().tag() == blockObj.tag())
			{
				slots[slotIndex]->data().content() = blockObj.content();
			}
			else
			{
				slots[slotIndex] = new Node<Block>(blockObj);
			}
		}
	};
}

#endif
