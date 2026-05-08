#ifndef GATE_H
#define GATE_H

#include "Dev.h"

namespace osp
{	
	class Gate : public Dev 
	{
		private:
		size_t mn;
		size_t mx;
		size_t ct;
		Dev* data[10];
		std::multiset<size_t> inputs;
		Gate(const Gate&) = delete;
		Gate& operator=(const Gate&) = delete;
		using Dev::bind;
		using Dev::unbind;

		bool contains(const Dev& obj) const 
		{
			size_t t = 0;

			while(t < ct && data[t] != &obj) {t += 1;}
			return t != ct;
		}

		protected:
		const Dev& at(size_t idx) const 
		{
			if(idx < ct) {return *(data[idx]);}
			throw std::out_of_range("invalid index");
		}

		public:
		Gate() : Gate(1,1) {} 

		Gate(size_t x) : Gate(1,x) {}

		Gate(size_t x, size_t y) : ct(0)
		{
			if(x == 0 || x > 10 || y == 0 || y > 10)
			{
				x = 1;
				y = 1;
			}
			mx = x + y;
			mn = (x < y)?(x):(y);
			mx -= mn;
		}

		virtual ~Gate() 
		{
			if(!broken())
			{
				for(size_t i = 0;i < ct;i += 1)
				{
					data[i]->unbind();
				}
			}
		}

		bool broken() const final 
		{
			for(const size_t ch : inputs)
			{
				if(keys.find(ch) == keys.end())
				{
					return true;
				}
			}
			return Dev::broken();
		}

		void reset() final 
		{
			for(size_t ch : inputs)
			{
				keys.erase(ch);
			}
			inputs.clear();
			ct = 0;
			Dev::reset();
		}	

		bool input(Dev& obj) 
		{
			if(broken() || full()) {return false;}
			if(contains(obj) || obj.bind(*this)) 
			{
				inputs.insert(obj.key());
				data[ct] = &obj;
				ct += 1;
				return true;
			}
			return false;
		}

		bool valid() const final {return !broken() && ct >= mn;}

		size_t count() const {return ct;}

		bool full() const {return ct == mx;}

		size_t minimum() const {return mn;}

		size_t maximum() const {return mx;}
	};

}

#endif
