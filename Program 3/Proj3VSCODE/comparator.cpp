/** Jacob Sigman
 *  Programming Assignment 2
 *  Professor Sable
 *  comparator.cpp
 *
 *  This code takes a list of Data objects containing a first name, last name, and social security number.
 *  The implementation of the sort uses the built-in sort method with booleans.
 */

// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, SPRING 2021

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds four public fields
class Data {
public:
    string lastName;
    string firstName;
    string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

    ifstream input(filename);
    if (!input) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    // The first line indicates the size
    string line;
    getline(input, line);
    stringstream ss(line);
    int size;
    ss >> size;

    // Load the data
    for (int i = 0; i < size; i++) {
        getline(input, line);
        stringstream ss2(line);
        Data *pData = new Data();
        ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
        l.push_back(pData);
    }

    input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

    ofstream output(filename);
    if (!output) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    // Write the size first
    int size = l.size();
    output << size << "\n";

    // Write the data
    for (auto pData:l) {
        output << pData->lastName << " "
               << pData->firstName << " "
               << pData->ssn << "\n";
    }

    output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
    string filename;
    cout << "Enter name of input file: ";
    cin >> filename;
    list<Data *> theList;
    loadDataList(theList, filename);

    cout << "Data loaded.\n";

    cout << "Executing sort...\n";
    clock_t t1 = clock();
    sortDataList(theList);
    clock_t t2 = clock();
    double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

    cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

    cout << "Enter name of output file: ";
    cin >> filename;
    writeDataList(theList, filename);

    return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class defintions here if you wish.

/** ssn_comparator(): A boolean for sorting by social security numbers.
 *
 * @param {Data*} obj1 First object to compare.
 * @param {Data*} obj2 Second object to compare.
 * @return {bool} Value being returned to the sort method.
 */

inline bool ssn_comparator(const Data* obj1, const Data* obj2) {

    /** Return a boolean corresponding to the comparison between two social security numbers.
     */

    return !(obj1 -> ssn >= obj2 -> ssn);
}

/** comparator(): A boolean for sorting by first name, last name, and social security number.
 *
 * @param {Data*} obj1 First object to compare.
 * @param {Data*} obj2 Second object to compare.
 * @return {bool} Value being returned to the sort method.
 */

inline bool comparator(const Data* obj1, const Data* obj2) {
    if (obj1 -> lastName > obj2 -> lastName)
        return false;
    if (obj1 -> lastName < obj2 -> lastName)
        return true;
    if (obj1 -> firstName > obj2 -> firstName)
        return false;
    if (obj1 -> firstName < obj2 -> firstName)
        return true;

    /** Call the boolean that compares social security numbers.
     */

    return ssn_comparator(obj1, obj2);
}

/** sortDataList(): Sorts list passed to method using built-in sort method.
 *
 * @param {list<Data *>} l The list to sort.
 */

void sortDataList(list<Data *> &l) {

    /** If the last names of the front and back of the list are the same, sort by social security number.
     */

    if (l.front() -> lastName == l.back() -> lastName) {
        l.sort(ssn_comparator);
        return;
    }

    /** Otherwise, sort by first name, last name, and social security number.
     */

    l.sort(comparator);
}