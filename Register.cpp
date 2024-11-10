
// Created by Salma Emara on 2023-06-02.
#include "Register.h"

#include <iostream>

Register::Register(int id, double timePerItem, double overhead,
                   double entryTime) {
  ID = id;
  secPerItem = timePerItem;
  overheadPerCustomer = overhead;
  availableTime = entryTime;
  next = nullptr;           // no other registers yet
  queue = new QueueList();  // no customers in line yet, but has to initialize a
                            // queue
}

Register::~Register() { delete queue; }  // is this correct??

QueueList* Register::get_queue_list() { return queue; }

Register* Register::get_next() { return next; }

int Register::get_ID() { return ID; }

double Register::get_secPerItem() { return secPerItem; }

double Register::get_overheadPerCustomer() { return overheadPerCustomer; }

double Register::get_availableTime() { return availableTime; }

void Register::set_next(Register* nextRegister) { next = nextRegister; }

void Register::set_availableTime(double availableSince) {
  availableTime = availableSince;
}

double Register::calculateDepartTime() {
  // Get the departure time of the first customer in the queue
  // returns -1 if no customer is in the queue

  Customer* customer = queue->get_head();
  if (customer == nullptr) {
    return -1;
  }
  double processing_time = customer->get_numOfItems() * secPerItem;
  double departTime = 0;

  // If the customer at the head of the queue arrived after the register became
  // available, then the departure time is the processing time of the customer
  // plus the arrival time of the customer
  if (customer->get_arrivalTime() >= availableTime) {
    departTime =
        processing_time + overheadPerCustomer + customer->get_arrivalTime();
  }
  // Otherwise, the departure time is the processing time of the customer plus
  // the time the register became available
  else {
    departTime = processing_time + overheadPerCustomer + availableTime;
  }
  return departTime;
}

void Register::departCustomer(QueueList* doneList) {
  // dequeue the head, set last dequeue time, add to doneList,
  // update availableTime of the register

  Customer* customer = queue->dequeue();
  doneList->enqueue(customer);
  std::cout << "Departed a customer at register ID " << ID << " at " << customer->get_departureTime() << std::endl;
  availableTime = customer->get_departureTime();
}

void Register::print() {
  std::cout << "Register ID: " << ID << std::endl;
  std::cout << "Time per item: " << secPerItem << std::endl;
  std::cout << "Overhead per customer: " << overheadPerCustomer << std::endl;
  if (queue->get_head() != nullptr) {
    std::cout << "Queue has customers: \n";
    queue->print();
  }
}
