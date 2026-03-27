#ifndef PROCESS
#define PROCESS

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "Object.h"

namespace osl
{
	enum class State
	{
		NEW,
		READY,
		RUNNING,
		EXIT
	};

	static const char* stateName(State s)
	{
		switch (s)
		{
			case State::NEW: return "NEW";
			case State::READY: return "READY";
			case State::RUNNING: return "RUNNING";
			case State::EXIT: return "EXIT";
			default: return "UNKNOWN";
		}
	}

	class Process : public Object
	{
		private:
		unsigned int processId;
		static unsigned int count;
		State currentState;
		unsigned int burstTime;
		unsigned int remainingTime;
		unsigned int waitingTime;
		unsigned int arrivalTime;

		public:
		Process()
			: processId(++count),
			  currentState(State::NEW),
			  burstTime(12),
			  remainingTime(12),
			  waitingTime(0),
			  arrivalTime(0)
		{
		}

		explicit Process(unsigned int bt)
			: processId(++count),
			  currentState(State::NEW),
			  burstTime(bt > 0 ? bt : 12),
			  remainingTime(bt > 0 ? bt : 12),
			  waitingTime(0),
			  arrivalTime(0)
		{
		}

		Process(const Process& other)
			: processId(other.processId),
			  currentState(other.currentState),
			  burstTime(other.burstTime),
			  remainingTime(other.remainingTime),
			  waitingTime(other.waitingTime),
			  arrivalTime(other.arrivalTime)
		{
		}

		Process& operator=(const Process& rhs)
		{
			if (this != &rhs)
			{
				currentState = rhs.currentState;
				burstTime = rhs.burstTime;
				remainingTime = rhs.remainingTime;
				waitingTime = rhs.waitingTime;
				arrivalTime = rhs.arrivalTime;
			}
			return *this;
		}

		~Process() = default;

		void initialize()
		{
			currentState = State::NEW;
			remainingTime = burstTime;
			waitingTime = 0;
			arrivalTime = 0;
		}

		void admit()
		{
			if (currentState != State::NEW)
			{
				throw std::runtime_error("admit: process must be in NEW state");
			}
			currentState = State::READY;
		}

		void dispatch()
		{
			if (currentState != State::READY)
			{
				throw std::runtime_error("dispatch: process must be in READY state");
			}
			currentState = State::RUNNING;
		}

		void preempt()
		{
			if (currentState != State::RUNNING)
			{
				throw std::runtime_error("preempt: process must be in RUNNING state");
			}
			currentState = State::READY;
		}

		void terminate()
		{
			if (currentState != State::RUNNING)
			{
				throw std::runtime_error("terminate: process must be in RUNNING state");
			}
			currentState = State::EXIT;
		}

		void tick()
		{
			if (currentState == State::RUNNING)
			{
				if (remainingTime > 0)
				{
					remainingTime--;
				}
				if (remainingTime == 0)
				{
					terminate();
				}
			}
			else if (currentState == State::READY)
			{
				waitingTime++;
			}
			else if (currentState == State::NEW)
			{
				arrivalTime++;
			}
		}

		State getState() const { return currentState; }

		bool isFinished() const
		{
			return remainingTime == 0 || currentState == State::EXIT;
		}

		bool isReady() const { return currentState == State::READY; }

		size_t getWaitingTime() const { return static_cast<size_t>(waitingTime); }

		size_t getRemainingTime() const { return static_cast<size_t>(remainingTime); }

		std::string toString() const override
		{
			std::ostringstream os;
			os << "PID=" << processId
			   << " state=" << stateName(currentState)
			   << " burst=" << burstTime
			   << " remaining=" << remainingTime
			   << " waiting=" << waitingTime
			   << " arrival=" << arrivalTime;
			return os.str();
		}
	};

	inline unsigned int Process::count = 0;
}

#endif
