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

            if(tail != nullptr)
            {
                while(tail->next() != nullptr) {tail = tail->next();}
            }
        }

        Queue<T>& operator=(const Queue<T>& rhs)
        {
            if(this != &rhs)
            {
                length = rhs.length;
                clear(head);
                head = clone(rhs.head);
                tail = head;

                if(tail != nullptr)
                {
                    while(tail->next() != nullptr) {tail = tail->next();}
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
    };
}

#endif
