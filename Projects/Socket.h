#ifndef SOCKET_H
#define SOCKET_H

#include "Dev.h"
#include "Word.h"

namespace osp
{
	class Socket : public Object 
	{
		private:
		Dev* src;
		Word* des;
		size_t pin;
		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;

		public:
		Socket() : src(nullptr), des(nullptr), pin(0) {}
		
		virtual ~Socket() 
		{
			src = nullptr;
			des = nullptr;
		}
		
		bool destination(Word& obj) 
		{
			if(des == nullptr)
			{
				des = &obj;
				return true;
			}
			return false;
		}

		bool source(Dev& obj)
		{
			if(obj.valid() && src == nullptr)
			{
				src = &obj;
				return true;
			}
			return false;
		}

		void set(size_t idx)
		{
			if(des != nullptr && idx < des->size())
			{
				pin = idx;
			}
		}

		void unbind() 
		{
			des = nullptr;
			src = nullptr;
		}
		
		bool configured() const 
		{
			bool sck = src != nullptr && src->valid();
			bool dck = des != nullptr;
			return sck && dck;
		}
		
		bool evaluate()  
		{
			if(configured()) 
			{
				des->set(pin,src->output());
				return true;
			}
			return false;
		}
		
		std::string toString() const final 
		{
			if(des != nullptr)
			{
				return ((des->get(pin))?("T"):("F"));
			}
			return "X";
		}
	};
}

#endif
