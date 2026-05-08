#ifndef OBJECT_H
#define OBJECT_H

namespace osp 
{
	class Object
	{
		public:
		virtual std::string toString() const = 0;
		virtual ~Object() {}

		friend std::ostream& operator<<(std::ostream& out,const Object& obj)
		{
			out << obj.toString();
			return out;
		}
	};
}

#endif
