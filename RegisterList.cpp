#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  if (head == nullptr) {
  }
  while (head != nullptr) {
    Register* temp = head;
    head = head->get_next();
    delete temp;
  }
}

Register* RegisterList::get_head() { return head; }

int RegisterList::get_size() { return size; }

Register* RegisterList::get_min_items_register() {
  // loop all registers to find the register with least number of items
  if (head == nullptr) {
    return nullptr;
  }

  int min_items = head->get_queue_list()->get_items();
  Register* temp = head;
  Register* min_register = head;

  while (temp != nullptr) {
    if (temp->get_queue_list()->get_items() < min_items) {
      min_items = temp->get_queue_list()->get_items();
      min_register = temp;
    }
    temp = temp->get_next();
  }
  return min_register;
}

Register* RegisterList::get_free_register() {
  // return the register with no customers
  // if all registers are occupied, return nullptr

  Register* temp = head;
  while (temp != nullptr) {
    if (temp->get_queue_list()->get_head() == nullptr) {
      return temp;
    }
    temp = temp->get_next();
  }
  return nullptr;
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size
  if (head == nullptr) {
    head = newRegister;
  } else {
    Register* tail = head;
    while (tail->get_next() != nullptr) {
      tail = tail->get_next();
    }
    tail->set_next(newRegister);
  }
  newRegister->set_next(nullptr);
  size++;
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise
  Register* temp = head;
  while (temp != nullptr) {
    if (temp->get_ID() == ID) {
      return true;
    }
    temp = temp->get_next();
  }
  return false;
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID
  // return the dequeued register
  // return nullptr if register was not found
  if (head == nullptr) {
    return nullptr;
  }

  if (head->get_ID() == ID) {
    Register* p = head;
    head = head->get_next();
    p->set_next(nullptr);
    size--;
    return p;
  }

  Register* temp = head;
  Register* prev = head;

  while (temp != nullptr && temp->get_ID() != ID) {
    prev = temp;
    temp = temp->get_next();
  }

  if (temp == nullptr) {
    return nullptr;
  }

  Register* del = temp;
  prev->set_next(del->get_next());
  del->set_next(nullptr);
  size--;
  return del;
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr

  if (head == nullptr) {
    return nullptr;
  }

  Register* temp = head;
  Register* min = nullptr;
  bool isEmpty = true;

  // if all registers are free, return nullptr
  while (temp != nullptr && isEmpty) {
    if (temp->calculateDepartTime() != -1) {
      isEmpty = false;
    }
    temp = temp->get_next();
  }
  if (isEmpty) {
    return nullptr;
  }

  temp = head;
  double minDepartTime = 0;
  min = nullptr;

  while (temp != nullptr) {
    double currentDepartTime = temp->calculateDepartTime();
    if ((currentDepartTime != -1) && (currentDepartTime <= expTimeElapsed)) {
      if (min == nullptr ||
          ((min != nullptr) && (minDepartTime > currentDepartTime))) {
        minDepartTime = currentDepartTime;
        min = temp;
      }
    }
    temp = temp->get_next();
  }

  return min;
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
