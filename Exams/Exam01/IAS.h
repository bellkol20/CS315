#ifndef IAS_H
#define IAS_H

#include "Word.h"
#include <stdexcept>

namespace ose
{
	class IAS 
	{
    		private:
		Word mem[4103];

    		public:
    		IAS() 
    		{
			int sz[4] = {12,8,20,12};

			for(int i = 0;i < 4103;i += 1)
			{
				if(i < 4) {mem[i] = Word(sz[i]);}
				else {mem[i] = Word(40);}
				mem[i].fix();
			}
    		}

    		IAS(const IAS& obj)
    		{
        		for(int i = 0;i < 4103;i += 1)
        		{
            			transfer(mem[i],obj.mem[i]);
        		}
    		}

    		IAS& operator=(const IAS& rhs)
    		{
        		if(this != &rhs)
        		{
            			for(int i = 0;i < 4103;i += 1)
            			{
                			transfer(mem[i],rhs.mem[i]);
				}
        		}
        		return *this;
    		}

    		~IAS() {}

    		Word& operator[](int idx)
    		{
        		if(idx >= 0 && idx < 4096) {return mem[7+idx];}
        		throw std::out_of_range("invalid index");
    		}

    		Word& operator()(int idx)
    		{
        		if(idx >= 0 && idx < 7) {return mem[idx];}
        		throw std::out_of_range("invalid index");
    		}
	};
}

#endif
