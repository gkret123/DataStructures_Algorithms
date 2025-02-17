// Gabriel Kret
// Programming Assignment #2
// Data Structures and Algorithms I
// Fall 2023

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <list>
#include <string>

// This code implements a data structure known as a "Simple List" that can function (using 2 subclasses) as both a Stack and a Queue. It includes the following features:

//- SimpleList
//- Stack
//- Queue
//- checkNameExist
//- getList
//- processCommands

// These 6 features represent the 6 "most important" parts of the code
// Each of these features have an explanation before the line that they are defined on in the code below
//  The explanation for each is wrapped with astrisks -- i.e. ****Lorem Ipsum****

using namespace std;

// declarations of classes and functions
template <typename T>
class SimpleList;
template <typename T>
class Stack;
template <typename T>
class Queue;
template <typename T>
bool checkNameExist(list<SimpleList<T> *> l, string name);
template <typename T>
SimpleList<T> *getList(list<SimpleList<T> *> l, string name);

// Template class for a Simple List
template <typename T>
//****- SimpleList: A class representing a singly linked list. Allows adding elements to the front and end of the list, as well as for removing elements from the front.****
class SimpleList
{
private:
  // Node struct for singly linked list
  struct Node
  {
    T data;
    Node *advance;

    Node(const T &d, Node *n = nullptr) : data(d), advance(n) {}
  };

  string listName;
  int listSize;
  Node *bottom;
  Node *top;

public:
  SimpleList(string name) : listName(name),
                            listSize(0),
                            bottom(nullptr),
                            top(nullptr) {}

  // Get the size of the list
  int getListSize()
  {
    return this->listSize;
  }

  // Get the name of the list
  string getListName()
  {
    return this->listName;
  }



  // Push an element to the list (to be overridden by child classes)
  virtual int push(T value) = 0;

  // Pop an element from the list (to be overridden by child classes)
  virtual T pop() = 0;

protected:
  // Add an element to the end of the list
  int pushBack(T value);

  // Add an element to the start of the list
  int pushFront(T value);

  // Remove an element from the front of the list
  T popFront();
};

// Add an element to the end of the list
template <typename T>
int SimpleList<T>::pushBack(T value)
{
  Node *newNode = new Node(value);
  newNode->advance = nullptr;

  if (bottom)
  {
    bottom->advance = newNode;
    bottom = newNode;
  }
  else
  {
    top = bottom = newNode;
  }

  listSize = listSize + 1;
  return 0;
}

// Add an element to the start of the list
template <typename T>
int SimpleList<T>::pushFront(T value)
{
  Node *newNode = new Node(value);
  newNode->advance = top;

  if (top)
  {
    top = newNode;
  }
  else
  {
    top = bottom = newNode;
  }

  listSize = listSize + 1;
  return 0;
}

// Remove an element from the front of the list
template <typename T>
T SimpleList<T>::popFront()
{
  Node *Temp_DataStore = top;
  T data = top->data;

  if (top->advance)
  {
    top = top->advance;
  }
  else
  {
    top = bottom = nullptr;
  }

  listSize = listSize - 1;
  delete Temp_DataStore;
  
  return data;
}

template <typename T>
//- Stack: A subclass of SimpleList, that simulates a stack data structure. It overrides the push and pop methods for adding and removing elements to/from the top of the stack (LIFO).****
class Stack : public SimpleList<T>
{
public:
  Stack(string name);

  // Push an element to the stack (overrides the base class method)
  int push(T value) override
  {
    return this->pushFront(value);
  }

  // Pop an element from the stack (overrides the base class method)
  T pop() override
  {
    return this->popFront();
  }
};

// Stack constructor
template <typename T>
Stack<T>::Stack(string name) : SimpleList<T>(name) {}


template <typename T>
//****- Queue: A subclass of SimpleList, that simulates a queue data structure. It overrides the push and pop methods for adding elements to the rear and removing elements from the front of the queue (FIFO).****
class Queue : public SimpleList<T>
{
public:
  Queue(string name); // Queue constructor

  // Push an element to the queue (overrides the base class method)
  int push(T value) override
  {
    return this->pushBack(value);
  }

  // Pop an element from the queue (overrides the base class method)
  T pop() override
  {
    return this->popFront();
  }
};

// Queue constructor
template <typename T>
Queue<T>::Queue(string name) : SimpleList<T>(name) {}

// Check if a given name exists in a list of SimpleLists
template <typename T>
//****- checkNameExist: A function that checks for the existence of a list with a specific name within a container of SimpleLists. It searches through the list container and confirms whether a match exists for the given name.*****
bool checkNameExist(list<SimpleList<T> *> l, string name)
{
  for (auto itr : l)
  {
    if (itr->getListName() == name)
    {
      return true;
    }
  }
  return false;
}

// Get a SimpleList with the given name from a list of SimpleLists
template <typename T>
//****- getList: A function to retrieve a SimpleList with a specified name from a list container of SimpleLists. It scans the list container and returns the first element whose name corresponds to the specified name.****
SimpleList<T> *getList(list<SimpleList<T> *> l, string name)
{
  for (auto itr : l)
  {
    if (itr->getListName() == name)
    {
      return itr;
    }
  }
  return nullptr;
}

// Process the commands from the input file and write the results to the output file
void processCommands(string inputFileName, string outputFileName)
{
  ofstream outputFile(outputFileName);
  ifstream inputFile(inputFileName);
  string line = "";

  // Lists of SimpleLists
  list<SimpleList<int> *> intLists;
  list<SimpleList<double> *> doubleLists;
  list<SimpleList<string> *> stringLists;

  if (inputFile.is_open())
  {
    // Read each line from the input file
    while (getline(inputFile, line))
    {
      if (line.empty())
      {
        continue;
      }

      // Output the command to the output file
      outputFile << "PROCESSING COMMAND: " << line << "\n";
      istringstream commandStream(line);
      vector<string> tokens;

      string token;
      while (commandStream >> token)
      {
        tokens.push_back(token);
      }

      // Get the operation (create, push, pop)
      string operation = tokens[0];
      // Get the name of the SimpleList
      string listName = tokens[1];
      // Get the type of the SimpleList (i, d, s)
      string valueType = listName.substr(0, 1);
      // Get the type of the SimpleList (stack, queue)
      string listType = tokens.size() == 3 ? tokens[2] : "";

      if (operation == "create")
      {
        if (valueType == "i")
        {
          if (checkNameExist(intLists, listName))
          {
            outputFile << "ERROR: This name already exists!"
                       << "\n";
          }
          else
          {
            if (listType == "stack")
            {
              intLists.push_back(new Stack<int>(listName));
            }
            else if (listType == "queue")
            {
              intLists.push_back(new Queue<int>(listName));
            }
          }
        }
        else if (valueType == "d")
        {
          if (checkNameExist(doubleLists, listName))
          {
            outputFile << "ERROR: This name already exists!"
                       << "\n";
          }
          else
          {
            if (listType == "stack")
            {
              doubleLists.push_back(new Stack<double>(listName));
            }
            else if (listType == "queue")
            {
              doubleLists.push_back(new Queue<double>(listName));
            }
          }
        }
        else if (valueType == "s")
        {
          if (checkNameExist(stringLists, listName))
          {
            outputFile << "ERROR: This name already exists!"
                       << "\n";
          }
          else
          {
            if (listType == "stack")
            {
              stringLists.push_back(new Stack<string>(listName));
            }
            else if (listType == "queue")
            {
              stringLists.push_back(new Queue<string>(listName));
            }
          }
        }
      }
      else if (operation == "pop")
      {
        if (valueType == "i")
        {
          SimpleList<int> *list = getList(intLists, listName);
          if (list == nullptr)
          {
            outputFile << "ERROR: This name does not exist!"
                       << "\n";
          }
          else
          {
            if (list->getListSize() == 0)
            {
              outputFile << "ERROR: This list is empty!"
                         << "\n";
            }
            else
            {
              // Pop the top value from the SimpleList
              int poppedValue = list->pop();
              outputFile << "Value popped: " << poppedValue << "\n";
            }
          }
        }
        else if (valueType == "d")
        {
          SimpleList<double> *list = getList(doubleLists, listName);
          if (list == nullptr)
          {
            outputFile << "ERROR: This name does not exist!"
                       << "\n";
          }
          else
          {
            if (list->getListSize() == 0)
            {
              outputFile << "ERROR: This list is empty!"
                         << "\n";
            }
            else
            {
              // Pop the top value from the SimpleList
              double poppedValue = list->pop();
              outputFile << "Value popped: " << poppedValue << "\n";
            }
          }
        }
        else if (valueType == "s")
        {
          SimpleList<string> *list = getList(stringLists, listName);
          if (list == nullptr)
          {
            outputFile << "ERROR: This name does not exist!"
                       << "\n";
          }
          else
          {
            if (list->getListSize() == 0)
            {
              outputFile << "ERROR: This list is empty!"
                         << "\n";
            }
            else
            {
              // Pop the top value from the SimpleList
              string poppedValue = list->pop();
              outputFile << "Value popped: " << poppedValue << "\n";
            }
          }
        }
      }
      else if (operation == "push")
      {
        if (valueType == "i")
        {
          SimpleList<int> *list = getList(intLists, listName);
          if (list == nullptr)
          {
            outputFile << "ERROR: This name does not exist!"
                       << "\n";
          }
          else
          {
            // Declare value as an integer
            int value = stoi(tokens[2]);
            // Push the value to the SimpleList
            list->push(value);
          }
        }
        else if (valueType == "d")
        {
          SimpleList<double> *list = getList(doubleLists, listName);
          if (list == nullptr)
          {
            outputFile << "ERROR: This name does not exist!"
                       << "\n";
          }
          else
          {
            // Declare value as a double -- STOD converts a string to a double-precision floating-point number
            double value = stod(tokens[2]);
            // Push the value to the SimpleList
            list->push(value);
          }
        }
        else if (valueType == "s")
        {
          SimpleList<string> *list = getList(stringLists, listName);
          if (list == nullptr)
          {
            outputFile << "ERROR: This name does not exist!"
                       << "\n";
          }
          else
          {
            // Push the value to the SimpleList
            list->push(tokens[2]);
          }
        }
      }
    }

    // Close input and output files
    inputFile.close();
    outputFile.close();
  }
  else
  {
    cout << "Unable to open input file."
         << "\n";
  }
}

int main()
{
  string inputFileName, outputFileName;

  cout << "Enter input file name: ";
  cin >> inputFileName;
  cout << "Enter output file name: ";
  cin >> outputFileName;
  // Process the commands from the input file
  //****- processCommands: The central function in the code, it takes input and output file names as parameters. Processes commands from the input file and generate results in the output file.****
  processCommands(inputFileName, outputFileName);

  return 0;
}