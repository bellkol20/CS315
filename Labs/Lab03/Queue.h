#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "Node.h"

namespace osl
{
	template <class T>
	class Queue
	{
		private:
		Node<T>* head;
		Node<T>* tail;
		size_t length;

		public:
		Queue() : head(nullptr), tail(nullptr), length(0) {}

		Queue(const Queue<T>& obj)
		{
			length = obj.length;
			head = clone(obj.head);
			tail = head;

			if (tail != nullptr)
			{
				while (tail->next() != nullptr)
				{
					tail = tail->next();
				}
			}
		}

		Queue<T>& operator=(const Queue<T>& rhs)
		{
			if (this != &rhs)
			{
				length = rhs.length;
				clear(head);
				head = clone(rhs.head);
				tail = head;

				if (tail != nullptr)
				{
					while (tail->next() != nullptr)
					{
						tail = tail->next();
					}
				}
			}
			return *this;
		}

		~Queue()
		{
			clear(head);
			head = nullptr;
			tail = nullptr;
		}

		void enqueue(const T& obj)
		{
			Node<T>* node = new Node<T>(obj);
			node->next() = nullptr;
			node->prev() = tail;

			if (tail != nullptr)
			{
				tail->next() = node;
			}
			else
			{
				head = node;
			}
			tail = node;
			length++;
		}

		void dequeue()
		{
			if (empty())
			{
				throw std::runtime_error("dequeue: queue is empty");
			}

			Node<T>* old = head;
			head = head->next();
			if (head != nullptr)
			{
				head->prev() = nullptr;
			}
			else
			{
				tail = nullptr;
			}
			delete old;
			length--;
		}

		const T& peek() const
		{
			if (empty())
			{
				throw std::runtime_error("peek: queue is empty");
			}
			return head->data();
		}

		bool empty() const { return length == 0; }

		size_t size() const { return length; }
	};
}

#endif
