#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    string line1, line2, f1, f2, which;
    cout << "Input the file you want to check is correct:";
    cin >> f1;
    cout << "Is this sample 1, 2, 3, or 4 (Type '1, 2, 3, 4' OR 'T1, T2, T3, T4')?";
    cin >> which;
  if (which == "1" || which == "T1"){
    f2 = "out_sample1.txt";
  }
  else if (which == "2" || which == "T2"){
    f2 = "out_sample2.txt";
  }
  else if (which == "3" || which == "T3"){
    f2 = "out_sample3.txt";
  }
  else if (which == "4" || which == "T4"){
    f2 = "out_sample4.txt";
  }
  else{cerr << "Error: Choice Is Invalid" << "\n" << "Type 1, 2, 3, 4 OR T1, T2, T3, T4" << "\n";
        exit(1);
      }
  
    ifstream file1(f1);
    ifstream file2(f2);

    if (file1.is_open() && file2.is_open()) {
        int line_number = 1;
        while (getline(file1, line1) && getline(file2, line2)) {
            if (line1 != line2) {
                cout << "Line " << line_number << " is different." << endl;
            }
            line_number++;
        }
        if (getline(file1, line1)) {
            cout << "File 1 has more lines than file 2." << endl;
        }
        else if (getline(file2, line2)) {
            cout << "File 2 has more lines than file 1." << endl;
        }
        else {
            cout << "The files are identical." << endl;
        }
    }
    else {
        cout << "Unable to open one or both files." << endl;
    }

    file1.close();
    file2.close();

    return 0;
}
