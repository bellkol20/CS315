#include "Memory.h"
#include <fstream>
#include <stdexcept>

class IAS 
{
    private:
    osn::Memory regs[7];
    osn::Memory locs[4096];
    static const std::string names[7];

    public:
    IAS() 
    {
        for(int i = 0;i < 7;i += 1)
        {
            regs[i].name(names[i]);

            if(i == 0 || i == 3) 
            {
                regs[i].write(std::string(12,'0'));
            }
            else if(i == 1)
            {
                regs[i].write(std::string(8,'0'));
            }
            else if(i == 2)
            {
                regs[i].write(std::string(20,'0'));
            }
            else 
            {
                regs[i].write(std::string(40,'0'));
            }
        }
        
        int pd;

        for(int i = 0;i < 4096;i += 1)
        {
            locs[i].write(std::string(40,'0'));
            if(i == 0) {pd = 3;}
            else {pd = 3 - (int)(log10(i));}
            locs[i].name(std::string(pd,'0') + std::to_string(i));
        }
    }

    IAS(const IAS& obj)
    {
        for(int i = 0;i < 7;i += 1)
        {
            regs[i] = obj.regs[i];
        }

        for(int i = 0;i < 4096;i += 1)
        {
            locs[i] = obj.locs[i];
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

    ~IAS() {}

    osn::Memory& operator[](int idx)
    {
        if(idx >= 0 && idx < 4096) {return locs[idx];}
        throw std::out_of_range("invalid index");
    }

    osn::Memory& operator()(int idx)
    {
        if(idx >= 0 && idx < 7) {return regs[idx];}
        throw std::out_of_range("invalid index");
    }
};

const std::string IAS::names[7] = {"PC","IR","IBR","MAR","MBR","AC","MQ"};

int main()
{
    IAS cmp;

    for(int i = 0;i < 7;i += 1)
    {
        std::cout << cmp(i) << "\n";
    }
    std::cout << cmp[32] << "\n";

    return 0;
}