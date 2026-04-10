#ifndef PAIR_H
#define PAIR_H

#include <iostream>
#include <string>
#include <sstream>
#include "Object.h"

namespace osl 
{
	template <class K, class V>
	class Pair : public Object
	{
		private:
		K identity;
		V content;

		public:
		Pair() : Pair(K(),V()) {}

		Pair(const K& key,const V& value)
		{
			identity = key;
			content = value;
		}

		Pair(const Pair<K,V>& obj) 
		{
			identity = obj.identity;
			content = obj.content;
		}

		Pair<K,V>& operator=(const Pair<K,V>& rhs)
		{
			if(this != &rhs)
			{
				identity = rhs.identity;
				content = rhs.content;
			}
			return *this;
		}

		~Pair() {}

		K& key() {return identity;}
		const K& key() const {return identity;}

		V& value() {return content;}
		const V& value() const {return content;}

		std::string toString() const override 
		{
			std::stringstream out;

			out << "(" << identity << "," << content << ")";
			return out.str();
		}
	};
}

#endif
