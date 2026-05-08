#ifndef SPLIT_H
#define SPLIT_H

#include "Dev.h"

namespace osp
{	
	class Split : public Dev 
	{
		private:
		size_t ct;
		size_t owners[20];
		Dev* data;
		size_t idx;
		Split(const Split&) = delete;
		Split& operator=(const Split&) = delete;
		
		bool bind(Dev& obj) final 
		{
			if(!obj.bounded() && !bounded() && valid())
			{
				size_t i = 0;

				while(i < ct && owners[i] != obj.key())
				{
					i += 1;
				}
				if(i == ct) 
				{
					owners[ct] = obj.key();
					ct += 1;
					return true;
				}
				else {return false;}
			}
			return false;
		}
		
		void unbind() final 
		{
			ct = 0;
			Dev::unbind();
		}

		public:
		Split() : ct(0), data(nullptr), idx(key()) {} 

		virtual ~Split() 
		{
			if(data != nullptr) {data->unbind();}

			for(size_t i = 0;i < ct;i += 1) 
			{
				keys.erase(owners[i]);
			}
		}

		bool broken() const final 
		{
			for(size_t i = 0;i < ct;i += 1)
			{
				if(keys.find(owners[i]) == keys.end())
				{
					return true;
				}
			}
			if(keys.find(idx) == keys.end()) {return true;}
			return Dev::broken();
		}

		void reset() final 
		{
			for(size_t i = 0;i < ct;i += 1)
			{
				keys.erase(owners[i]);
			}
			data = nullptr;
			idx = key();
			Dev::reset();
		}	

		bool input(Dev& obj) 
		{
			if(broken()) {return false;}
			else if(obj.bind(*this)) 
			{
				idx = obj.key();
				data = &obj;
				return true;
			}
			return false;
		}

		bool valid() const final 
		{
			return !broken() && data != nullptr;
		}

		bool bounded() const final {return ct == 20;}

		size_t count() const {return ct;}

		bool output() const final
		{
			if(valid()) {return data->output();}
			return false;
		}

		std::string toString() const final 
		{
			if(valid()) return data->toString();
			return "X";
		}
	};

}

#endif
