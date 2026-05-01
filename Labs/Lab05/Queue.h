#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>
#include "Node.h"
#include "Object.h"

namespace osl 
{
	template <class T>
	class Queue 
	{
		private:
		Node<T>* head;
		Node<T>* tail;

		std::string toString() const override 
		{
			std::stringstream out;

			out << "[";

			for(Node<T>* t = head;t != nullptr;t = t->next())
			{
				out << t->data();

				if(t->next() != nullptr) {out << ",";}
			}
			out << "]";
			return out.str();
		}

		public:
		Queue() : head(nullptr), tail(nullptr) {}

		~Queue() 
		{
			clear(head);
			head = nullptr;
			tail = nullptr;
		}

		Queue(const Queue<T>& obj) 
		{
			head = clone(obj.head);
			tail = head;

			if(tail != nullptr) 
			{
				while(tail->next() != nullptr)
				{
					tail = tail->next();
				}
			}
		}

		Queue<T>& operator=(const Queue<T>& rhs)
		{
			if(this != &rhs)
			{
				clear(head);
				head = clone(rhs.head);
				tail = head;

				if(tail != nullptr)
				{
					while(tail->next() != nullptr)
					{
						tail = tail->next();
					}
				}
			}
			return *this;
		}

		void enqueue(const T& obj) 
		{
			Node<T>* t = new Node<T>(obj);

			t->prev() = tail;

			if(tail == nullptr) {head = t;}
			else {t->prev()->next() = t;}
			tail = t;
		}

		void dequeue() 
		{
			Node<T>* t = head;

			head = head->next();

			if(head == nullptr) {tail = nullptr;}
			else {head->prev() = nullptr;}
			delete t;
			t = nullptr;
		}

		const T& peek() const 
		{
			if(head != nullptr) {return head->data();}
			throw std::out_of_range("queue is empty");
		}
		
		T& peek()
		{
			if(head != nullptr) {return head->data();}
			throw std::out_of_range("queue is empty");
		}

		bool empty() const {return head == nullptr;}
	};
}

#endif
