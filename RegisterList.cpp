#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  // Delete all registers in the list
}

Register* RegisterList::get_head() { return head; }

int RegisterList::get_size() { return size; }

Register* RegisterList::get_min_items_register() {
  // loop all registers to find the register with least number of items
  int min_items = head->get_queue_list()->get_items();
  Register* temp = head;
  Register* min_register = head;
  while (temp->get_next() != nullptr) {
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

  Register* empty = nullptr;
  Register* temp = head;
  while (temp->get_next() != nullptr) {
    if (temp->get_queue_list() = nullptr) {
      empty = temp;
      break;
    }
  }
  return empty;
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size

  if (head == nullptr) {
    head = newRegister;
  } else {
    ;  // how to do this with no get_next() method in registerlist class??
  }
  newRegister->set_next(nullptr);
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID

  // return the dequeued register
  // return nullptr if register was not found
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
