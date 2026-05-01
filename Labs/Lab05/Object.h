#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>
#include <sstream>

namespace osl
{
	class Object 
	{
		private:
		virtual std::string toString() const = 0;

		friend std::ostream& operator<<(std::ostream& out,const Object& obj)
		{
			return out << obj.toString();
		}

	};
}

#endif
