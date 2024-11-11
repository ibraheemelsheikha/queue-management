#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {
  while (head != nullptr) {
    Customer* temp = head;
    head = head->get_next();
    delete temp;
  }
}

Customer* QueueList::get_head() { return head; }

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head
  if (head == nullptr) {
    head = customer;
  } else {
    Customer* tail = head;
    while (tail->get_next() != nullptr) {
      tail = tail->get_next();
    }
    tail->set_next(customer);
  }
  customer->set_next(nullptr);
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue
  // and return a pointer to it
  if (head == nullptr) {
    return nullptr;
  }
  Customer* temp = head;
  head = head->get_next();
  temp->set_next(nullptr);
  return temp;
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has
  Customer* temp = head;
  int total = 0;
  while (temp != nullptr) {
    total += temp->get_numOfItems();
    temp = temp->get_next();
  }
  return total;
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
