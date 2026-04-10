#ifndef WORD_H
#define WORD_H

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "Object.h"

namespace osl
{
	class Word : public Object
	{
		private:
		std::string value;
		Word* links[2];

		std::string toString() const override 
		{
			std::stringstream out;
			size_t n = size();

			if(n == 0) {return "nil";}

			for(size_t i = 0;i < n;i += 1)
			{
				out << get(i);
			}
			return out.str();
		}

		protected:
		bool frozen;
		bool stable;

		public:
		Word() : Word(0) {}
		
		Word(size_t sz) : links{nullptr,nullptr} 
		{
			stable = false;
			frozen = false;
			value = std::string(sz,'0');
		}

		Word(const std::string& obj) : links{nullptr,nullptr}
		{
			value = obj;
			stable = false;
			frozen = false;

			for(auto ch : obj) 
			{
				if(ch != '1' && ch != '0')
				{
					value = "";
					break;
				}
			}
		}

		Word(const Word& obj) : stable(false) {*this = obj;}

		Word& operator=(const Word& rhs) 
		{
			if(this != &rhs && !stable) 
			{
				if(rhs.empty()) {value = "";}
				else {value = rhs.toString();}
				stable = rhs.stable;
				frozen = rhs.frozen;
				links[0] = nullptr;
				links[1] = nullptr;
			}
			return *this;
		}

		virtual ~Word() {unbind();}
 
		void fix() {if(!empty()) {stable = true;}}

		bool fixed() const {return stable;}

		bool empty() const {return value.empty();}

		bool extended() const {return links[1] != nullptr;}

		bool appended() const {return links[0] != nullptr;}

		bool bounded() const 
		{
			return links[0] != nullptr || links[1] != nullptr;
		}
		
		void unbind()
		{
			if(links[0] != nullptr)
			{
				links[0]->links[1] = nullptr;
			}
			
			if(links[1] != nullptr)
			{
				links[1]->links[0] = nullptr;
			}
		}
	
		virtual size_t size() const 
		{
			size_t m = value.size();
			
			if(links[1] != nullptr) 
			{
				return m + links[1]->size();
			}	
			return m;
		}

		virtual bool get(size_t idx) const
		{
			size_t m = value.size();

			if(idx < m) 
			{
				return (value[idx] == '1');
			}
			else if(links[1] != nullptr) 
			{
				return links[1]->get(idx-m);
			}
			return false;
		}

		virtual void set(size_t idx,bool bit) 
		{
			size_t m = value.size();
			char b = (bit)?('1'):('0');

			if(idx < m) 
			{
				value[idx] = b;
			}
			else if(links[1] != nullptr) 
			{
				links[1]->set(idx-m,bit);
			}
		}
		
		virtual void set(bool bit)
		{
			size_t m = value.size();
			char b = (bit)?('1'):('0');
			value = std::string(m,b);
			
			if(links[1] != nullptr)
			{
				links[1]->set(bit);
			}
		}

		void join(Word& obj)
		{
			if(!(empty() || obj.empty() || frozen || obj.frozen))
			{
				if(!extended() && !obj.bounded())
				{
					links[1] = &obj;
					obj.links[0] = this;
					fix();
					obj.fix();
				}
			}
		}

		friend Word subword(const Word& obj,size_t i,size_t j)
		{
			size_t m = obj.size();
			std::string fc = obj.toString();

			if(i < m && j < m)
			{
				size_t s = i + j, n;

				if(i < j) {s -= j;}
				else {s -= i;}
				n = i + j - 2 * s + 1;
				return Word(fc.substr(s,n));
			}
			return Word();	
		}

		friend void transfer(Word& lhs,const Word& rhs) 
		{
			if(&lhs != &rhs && lhs.size() == rhs.size())
			{
				size_t m = lhs.size(), i = 0;
 
				while(i < m)
				{
					lhs.set(i,rhs.get(i));
					i += 1;
				}
			}
		}
		
		friend size_t value(const Word& obj) 
		{
			size_t v = 0, m = obj.size(), n = m, i = 0;
			
			if(m != 0)
			{
				m -= 1;	
				while(m > 32) 
				{
					if(obj.get(i)) {return 0;}
					i += 1;
					m -= 1;
				}	

				while(i < n)
				{
					if(obj.get(i)) {v = v ^ (1 << m);}
					m -= 1;
					i += 1;
				}
			}
			return v;
		}

		friend bool operator==(const Word& lhs,const Word& rhs)
		{
			size_t m = lhs.size(), n = rhs.size();

			while(m > 0 && n > 0)
			{
				if(lhs.get(m-1) != rhs.get(n-1)) 
				{
					return false;
				}
				m -= 1;
				n -= 1;
			}

			while(m > 0) 
			{
				if(!lhs.get(m-1)) {return false;}
				m -= 1;
			}

			while(n > 0) 
			{
				if(!rhs.get(n-1)) {return false;}
				n -= 1;
			}
			return true;
		}
		
		friend bool operator!=(const Word& lhs,const Word& rhs)
		{
			return !(lhs == rhs);
		}

		friend std::istream& operator>>(std::istream& i,Word& obj)
		{
			std::string str;
			i >> str;
			obj = Word(str);
			return i; 	
		}
	};
}

#endif
