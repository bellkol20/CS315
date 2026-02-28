#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cctype>
#include "object.h"

namespace osn 
{ 
    bool isBinary(std::string str){
        for(char c : str)
        { 
            if(c != '0' && c != '1') {return false;}
        }
        return true;
    }

bool isHex(std::string str)
{
    char i; 
    
    for(char c : str)
    { 
        i = tolower(c);
        if(!isdigit(i) && (i < 'a' || i > 'f')) {return false;}
    }
    return true;
}
    class Memory : public object
    { 
        private: 
        std::string content;
        std::string id;
        bool flag;
        static const std::string digits[10];
        static const std::string letters[6];

        std::string toBinary(std::string obj)
        {
            char i;
            std::string rslt;
            for(char c : obj)
            {
                i = tolower(c);
                if(isdigit(i)) {rslt += digits[i-'0'];}
                else {rslt += letters[i-'a'];}
            }
            return rslt;
        }

        public:
        Memory()
        {
            content = '0';
            id = "name";
            flag = false;
        }
           
            Memory(const Memory& obj)
        { 
            content = obj.content;
            id = obj.id;
            flag = obj.flag;
        } 
        Memory& operator=(const Memory& rhs)
        { 
            if(this != &rhs)
            {
               content = rhs.content;
            id = rhs.id;
            flag = rhs.flag; 
            }
            return *this; 
        }

        ~Memory() {}

        std::string read()
        {
            flag = false;
            return content;
        }

        void write(std::string obj)
        {
            if(isBinary(obj))
            {
                flag = true;
                content = obj;
            }
        }

        std::string name() const {return id;}

        void name (std::string obj) {is = obj;}

        bool changed() const {return flag;}
        void change(bool obj) {flag = obj;}

        friend std::istream& operator>>(std::istream& i, Memory& obj)
        {
            std::string str;
            i >> str;

            if(isBinary(str)) {obj.content = str;}
            else if(isHex(str)) {obj.content = obj.toBinary(str);}
            else {obj.content = std::string(8, '0');}
            return i;
        }

    };

    const std::string Memory::digits[10] = {"0000", "0001", "0010","0011", "0100", "0110", "0111", }
    const std::string Memory::letters[6];
} 
#endif
       
    