#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join

// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);
void updateSingle();

// Global variables
RegisterList *registerList;  // holding the list of registers
QueueList *doneList;         // holding the list of customers served
QueueList *singleQueue;      // holding customers in a single virtual queue
double expTimeElapsed;  // time elapsed since the beginning of the simulation

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      cout << "Invalid operation" << endl;
    }
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }

  // You have to make sure all dynamically allocated memory is freed
  // before return 0
  return 0;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments" << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Depending on the mode of the simulation (single or multiple),
  // add the customer to the single queue or to the register with
  // fewest items
  expTimeElapsed += timeElapsed;
  Customer *customer = new Customer(expTimeElapsed, items);
  if (mode == "single") {
    updateSingle();
    cout << "A customer entered" << endl;
    singleQueue->enqueue(customer);
    if (registerList->get_free_register() == nullptr) {
      cout << "No free registers" << endl;
    } else {
      updateSingle();
    }
  }
  return;
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments" << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Check if the register is already open
  // If it's open, print an error message
  // Otherwise, open the register
  // If we were simulating a single queue,
  // and there were customers in line, then
  // assign a customer to the new register

  if (registerList->foundRegister(ID)) {
    cout << "Error: register " << ID << " is already open" << endl;
    return;
  }
  expTimeElapsed += timeElapsed;
  Register *newRegister =
      new Register(ID, secPerItem, setupTime, expTimeElapsed);

  if (mode == "single") {
    updateSingle();

    registerList->enqueue(newRegister);
    cout << "Opened register " << ID << endl;
    updateSingle();
  }
  return;
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  // If it is open dequeue it and free its memory
  // Otherwise, print an error message

  if (!registerList->foundRegister(ID)) {
    cout << "Error: register " << ID << " is not open" << endl;
    return;
  }

  expTimeElapsed += timeElapsed;
  if (mode == "single") {
    updateSingle();
  }

  Register *closedRegister = registerList->dequeue(ID);
  cout << "Closed register " << closedRegister->get_ID() << endl;
  delete closedRegister;
  closedRegister = nullptr;
  return;
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}

void updateSingle() {
  // cout << "debug: total exp elapsed time: " << expTimeElapsed << endl;
  if (registerList->get_head() == nullptr) {
    return;
  }

  Register *minRegister =
      registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  Register *freeRegister = registerList->get_free_register();

  // if (minRegister == nullptr) {
  //   if (freeRegister == nullptr) {
  //     // all registers are occupied
  //     cout << "debug: all regs occupied, returning" << endl;
  //     return;
  //   }
  //   // handle case when all regs are free
  //   minRegister = freeRegister;
  // }

  while (freeRegister != nullptr && singleQueue->get_head() != nullptr) {
    // cout << "debug: entered freeRegister loop" << endl;
    // cout << "debug: freeRegister is: " << freeRegister->get_ID() << endl;
    Customer *nextCustomer = singleQueue->dequeue();
    if (nextCustomer != nullptr) {
      freeRegister->get_queue_list()->enqueue(nextCustomer);
      cout << "Queued a customer with free register " << freeRegister->get_ID()
           << endl;
      nextCustomer->set_departureTime(freeRegister->calculateDepartTime());
    }
    freeRegister = registerList->get_free_register();
  }

  while (minRegister != nullptr) {
    // cout << "debug: entered minRegister loop" << endl;
    minRegister->departCustomer(doneList);
    Customer *nextCustomer = singleQueue->dequeue();
    if (nextCustomer != nullptr) {
      minRegister->get_queue_list()->enqueue(nextCustomer);
      cout << "Queued a customer with free register " << minRegister->get_ID()
           << endl;
      nextCustomer->set_departureTime(minRegister->calculateDepartTime());
    }

    minRegister = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  }
}
