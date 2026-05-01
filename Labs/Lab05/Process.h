#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include "Object.h"

namespace osl
{
	class Process : public Object 
	{
		private:
		std::string	process_id;
		size_t arrival_time;         
		size_t burst_time;           
		size_t remaining_time;       
		size_t start_time;           
		size_t completion_time;      
		size_t priority_level;             
		size_t elapsed_time;		
		static size_t counter;
		bool is_active;
		
		static std::string genID() 
		{
			return std::to_string(rand() % 9000 + 1000);
		}
		
		Process(const Process&) = delete;
		Process operator=(const Process&) = delete;
		
		std::string toString() const override 
		{
			std::ostringstream out;
			out << process_id << " >> " << arrival_time << " : " << burst_time;
			return out.str();
		}
		
		public:
		Process() : process_id(genID()), elapsed_time(0), arrival_time(counter)
		{
			counter += 1;
		}
		
		std::string id() const {return process_id;}
		
		size_t arrivalTime() const {return arrival_time;}
		
		const size_t& burstTime() const {return burst_time;}
		
		size_t& burstTime() {return burst_time;}
		
		const size_t& remainingTime() const {return remaining_time;}
		
		const size_t& startTime() const {return start_time;}
		
		size_t& startTime() {return start_time;}
		
		bool active() const {return is_active;}
		
		bool& active() {return is_arrive;}
		
		bool hasCompleted() const {return remaining_time == 0;}
		
		bool hasArrived() const {return elapsed_time >= arrival_time;}
		
		void tick() {elapsed_time += 1;}
		
		void reset() {elapse_time = 0;}
		
		void mark() {start_time = elapse_time;}
	};
	
	int Process::counter = 0;
}

#endif
