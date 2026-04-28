#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Word.h"
#include "Node.h"
#include "Block.h"
#include "Cache.h"
#include "LRU.h"
#include "FIFO.h"

int main()
{
	srand(time(nullptr));

	// 10 blocks each with a unique tag and binary content
	osl::Block memoryBlocks[10];
	std::string bitPatterns[] = {
		"0001", "0010", "0011", "0100", "0101",
		"0110", "0111", "1000", "1001", "1010"
	};

	for(size_t blockIndex = 0; blockIndex < 10; blockIndex++)
	{
		memoryBlocks[blockIndex].tag() = blockIndex;
		memoryBlocks[blockIndex].content() = osl::Word(bitPatterns[blockIndex]);
	}

	osl::LRUCache lru;
	osl::FIFOCache fifo;

	// simulate 20 random block accesses
	for(int iteration = 0; iteration < 20; iteration++)
	{
		osl::Block& requestedBlock = memoryBlocks[rand() % 10];

		lru.write(requestedBlock);
		fifo.write(requestedBlock);

		std::cout << "Iteration " << (iteration + 1) << "\n";
		std::cout << "Block:      " << requestedBlock << "\n";
		std::cout << "LRU Cache:\n" << lru;
		std::cout << "FIFO Cache:\n" << fifo;
		std::cout << "\n";
	}

	return 0;
}
