#include <iostream>
#include <string>
#include "Node.h"
#include "Process.h"
#include "Queue.h"

using osl::Process;
using osl::Queue;

int main()
{
	int numProcesses = 0;
	std::cout << "Number of processes: ";
	std::cin >> numProcesses;
	if (numProcesses <= 0)
	{
		std::cerr << "Must have a positive number of processes.\n";
		return 1;
	}

	Process* processes = new Process[static_cast<size_t>(numProcesses)];

	Queue<Process*> newQueue;
	Queue<Process*> readyQueue;
	Queue<Process*> exitQueue;

	Process* running = nullptr;

	unsigned int quantum = 0;
	std::cout << "Quantum (time slice): ";
	std::cin >> quantum;
	if (quantum == 0)
	{
		std::cerr << "Quantum must be positive.\n";
		delete[] processes;
		return 1;
	}

	size_t readyCapacity = 0;
	std::cout << "Maximum Ready queue capacity: ";
	std::cin >> readyCapacity;
	if (readyCapacity == 0)
	{
		std::cerr << "Ready queue capacity must be positive.\n";
		delete[] processes;
		return 1;
	}

	for (int i = 0; i < numProcesses; ++i)
	{
		newQueue.enqueue(&processes[i]);
	}

	unsigned int timeInSlice = 0;

	while (!newQueue.empty() || !readyQueue.empty() || running != nullptr)
	{
		const bool hadCpu = (running != nullptr);

		for (int i = 0; i < numProcesses; ++i)
		{
			processes[i].tick();
		}

		if (hadCpu)
		{
			timeInSlice++;
		}

		while (!newQueue.empty() && readyQueue.size() < readyCapacity)
		{
			Process* p = newQueue.peek();
			newQueue.dequeue();
			p->admit();
			readyQueue.enqueue(p);
		}

		if (running == nullptr && !readyQueue.empty())
		{
			running = readyQueue.peek();
			readyQueue.dequeue();
			running->dispatch();
			timeInSlice = 0;
		}
		else if (running != nullptr && timeInSlice >= quantum && !running->isFinished())
		{
			running->preempt();
			readyQueue.enqueue(running);
			running = nullptr;
			timeInSlice = 0;
		}
		else if (running != nullptr && running->isFinished())
		{
			exitQueue.enqueue(running);
			running = nullptr;
			timeInSlice = 0;
		}

		std::cout << "--- tick snapshot ---\n";
		for (int i = 0; i < numProcesses; ++i)
		{
			std::cout << processes[i].toString() << '\n';
		}
		std::cout << '\n';
	}

	delete[] processes;
	return 0;
}
