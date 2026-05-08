#ifndef DEV_H
#define DEV_H

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <set>
#include "Object.h"

namespace osp
{
	class Dev : public Object
	{
		private:
		static size_t counter;
		size_t id;
		size_t owner;
		Dev(const Dev&) = delete;
		Dev& operator=(const Dev&) = delete;	

		protected:
		static std::set<size_t> keys;

		public:
		Dev() : id(counter), owner(counter) 
		{
			counter += 1;
			keys.insert(id);
		}
		
		virtual ~Dev() 
		{
			keys.erase(owner);
			keys.erase(id);
		}

		virtual bool bind(Dev& obj) 
		{
			if(!obj.bounded() && !bounded() && valid()) 
			{
				owner = obj.id;
				return true;
			}
			return false;
		}

		virtual void unbind() {owner = id;}
		
		virtual void reset() 
		{
			keys.erase(owner);
			unbind();
			if(keys.find(id) == keys.end()) {keys.insert(id);}
		}	

		virtual bool broken() const
		{
			if(keys.find(owner) == keys.end()) {return true;}
			if(keys.find(id) == keys.end()) {return true;}
			return false;
		}

		virtual bool bounded() const {return owner != id;}

		size_t key() const {return id;}
		
		virtual bool valid() const = 0;

		virtual bool output() const = 0;
	};

	size_t Dev::counter = 0;
	std::set<size_t> Dev::keys;
}

#endif
