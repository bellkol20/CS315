#ifndef IAS_H
#define IAS_H

#include "Memory.h"
#include <stdexcept>

namespace osl
{
	class IAS : public Object 
	{
    		private:
    		Memory regs[7];
    		Memory locs[4096];

    		public:
    		IAS() 
    		{
			std::string regNames[7] = {"PC","IR","IBR","MAR","MBR","AC","MQ"};
			int sizes[7] = {12,8,20,12,40,40,40}, padLen;

			for(int i = 0;i < 7;i += 1)
			{
				regs[i].name(regNames[i]);
				regs[i].write(std::string(sizes[i],'0'));
				regs[i].change(false);
			}

        		for(int i = 0;i < 4096;i += 1)
        		{
            			locs[i].write(std::string(40,'0'));
            			if(i == 0) {padLen = 3;}
            			else {padLen = 3 - (int)(log10(i));}
            			locs[i].name(std::string(padLen,'0') + std::to_string(i));
				locs[i].change(false);
        		}
    		}

    		IAS(const IAS& other)
    		{
        		for(int i = 0;i < 7;i += 1)
        		{
            			regs[i] = other.regs[i];
        		}

        		for(int i = 0;i < 4096;i += 1)
        		{
            			locs[i] = other.locs[i];
        		}
    		}

    		IAS& operator=(const IAS& rhs)
    		{
        		if(this != &rhs)
        		{
            			for(int i = 0;i < 7;i += 1)
            			{
                			regs[i] = rhs.regs[i];
            			}

            			for(int i = 0;i < 4096;i += 1)
            			{
                			locs[i] = rhs.locs[i];
            			}
        		}
        		return *this;
    		}

    		virtual ~IAS() {}

    		Memory& operator[](int idx)
    		{
        		if(idx >= 0 && idx < 4096) {return locs[idx];}
        		throw std::out_of_range("invalid index");
    		}

    		Memory& operator()(int idx)
    		{
        		if(idx >= 0 && idx < 7) {return regs[idx];}
        		throw std::out_of_range("invalid index");
    		}

		std::string toString() const override 
		{
			std::stringstream stream;

			for(int i = 0;i < 7;i += 1)
			{
				if(regs[i].active()) {stream << regs[i] << " ";}
			}
			
			for(int i = 0;i < 4096;i += 1)
			{
				if(locs[i].active()) {stream << locs[i] << " ";}
			}
			return stream.str();
		}

	};
}

#endif
