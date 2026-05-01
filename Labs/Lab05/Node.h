#ifndef NODE_H
#define NODE_H

namespace osl 
{
	template <class T>
	class Node 
	{
		private:
		T value;
		Node<T>* links[2];

		public:
		Node() : value(), links{nullptr,nullptr} {}
		
		Node(const T& data) : value(data), links{nullptr,nullptr} {}
		
		Node(const Node<T>& obj) 
		{
			value = obj.value;
			links[0] = nullptr;
			links[1] = nullptr;
		}

		Node<T>& operator=(const Node<T>& rhs)
		{
			if(this != &rhs)
			{
				value = rhs.value;
			}
			return *this;
		}

		~Node() 
		{
			links[0] = nullptr;
			links[1] = nullptr;
		}

		const T& data() const {return value;}
		T& data() {return value;}
		
		const Node<T>* prev() const {return links[0];}
		Node<T>*& prev() {return links[0];}
		
		const Node<T>* next() const {return links[1];}
		Node<T>*& next() {return links[1];}
	};

	template <class T>
	Node<T>* clone(Node<T>* obj) 
	{
		if(obj == nullptr) {return nullptr;}
		Node<T> *cpy, *tr, *tc;

		cpy = new Node<T>(obj->data());
		tr = obj;
		tc = cpy;

		while(tr->next() != nullptr) 
		{
			tc->next() = new Node<T>(tr->next()->data());
			tc->next()->prev() = tc;
			tr = tr->next();
			tc = tc->next();
		}
		return cpy;
	}

	template <class T>
	void clear(Node<T>*& obj)
	{
		Node<T>* t;

		while(obj != nullptr)
		{
			t = obj;
			obj = obj->next();
			delete t;
		}
	}
}

#endif
