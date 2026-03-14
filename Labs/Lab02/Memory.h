#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cctype>
#include "Object.h"

namespace osl
{
    bool isBinary(std::string input)
    {
        for(char c : input)
        {
            if(c != '0' && c != '1') {return false;}
        }
        return true;
    }

    bool isHex(std::string input)
    {
        char ch;

        for(char c : input)
        {
            ch = tolower(c);
            if(!isdigit(ch) && (ch < 'a' || ch > 'f')) {return false;}
        }
        return true;
    }

    int toDecimal(std::string binStr)
    {
        if(!isBinary(binStr)) return -1;
        int result = 0;

        for(char bit : binStr)
        {
            result *= 2;
            if(bit == '1') {result += 1;}
        }
        return result;
    }

    std::string toBinary(int num)
    {
        if(num < 0) {num *= -1;}
        else if(num == 0) {return "0";}

        std::string result;
        
        while(num > 0)
        {
            result = std::to_string(num % 2) + result;
            num /= 2;
        }
        return result;
    }

    std::string negate(std::string binStr)
    {
        if(!isBinary(binStr)) {return "";}
        std::string inverted;
        bool carry = true;

        //inverts
        for(char c : binStr)
        {
            if(c == '1') {inverted += '0';}
            else {inverted += '1';}
        }

        //adds 1
        for(int i = inverted.length() - 1;carry && i >= 0;i -= 1)
        {
            if(inverted[i] == '1') {inverted[i] = '0';}
            else
            {
                inverted[i] = '1';
                carry = false;
            } 
        }
        return inverted;
    }


    class Memory : public Object 
    {
        private:
        std::string content;
        std::string id;
        bool flag;
        static const std::string digits[10];
        static const std::string letters[6];

        std::string toBinary(std::string hexStr) const
        {
            char ch;
            std::string result;

            for(char c : hexStr)
            {
                ch = tolower(c);
                
                if(isdigit(ch)) {result += digits[ch-'0'];}
                else {result += letters[ch-'a'];}
            }
            return result;
        }

        std::string toHex(std::string binStr) const
        {
            int padLen = (4 - binStr.length() % 4) % 4;
            std::string padded = std::string(padLen,'0') + binStr, nibble, result;
            std::string hexDigits = "0123456789ABCDEF";

            for(int i = 0;i < padded.length();i += 4)
            {
                nibble = padded.substr(i,4);
                result += hexDigits[toDecimal(nibble)]; 
            }
            return result;
        }

        public:
        //Default Constructor
        Memory() 
        {
            content = "0";
            id = "name";
            flag = false;
        }

        //Copy Constructor
        Memory(const Memory& other)
        {
            content = other.content;
            id = other.id;
            flag = other.flag;
        }

        //Assignment Operator
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

        //Destructor
        virtual ~Memory() {}

        std::string read() 
        {
            flag = false;
            return content;
        }

        void write(std::string value)
        {
            if(isBinary(value))
            {
                flag = true;
                content = value;
            }
        }

        std::string name() const {return id;}

        void name(std::string value) {id = value;}

        bool active() const {return flag;}

        void change(bool value) {flag = value;}

        friend std::istream& operator>>(std::istream& in,Memory& mem)
        {
            std::string str;
            in >> str;

            if(isBinary(str))
            {
                if(str.length() < 40) str = std::string(40 - str.length(), '0') + str;
                else if(str.length() > 40) str = str.substr(str.length() - 40);
                mem.content = str;
            }
            else if(isHex(str))
            {
                std::string bin = mem.toBinary(str);
                if(bin.length() < 40) bin = std::string(40 - bin.length(), '0') + bin;
                else if(bin.length() > 40) bin = bin.substr(bin.length() - 40);
                mem.content = bin;
            }
            else {mem.content = std::string(40,'0');}
            return in;
        }

        std::string toString() const override 
        {
            return id + ": " + toHex(content);
        }

    };

    const std::string Memory::digits[10] = {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001"};
    const std::string Memory::letters[6] = {"1010","1011","1100","1101","1110","1111"};
        
}
#endif
