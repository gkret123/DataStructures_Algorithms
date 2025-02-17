//@Gabriel_Kret Fall 2023
//This code keeps track of foobars, their name, type, place in line, and strengths
#include <iostream>

#include <fstream>

#include <vector>

#include <string>


//Declaration of 'Foobar' base class
//initializes the 'name' member variable with the value passed as an input parameter and sets the 'position' member variable to zero
class Foobar {
  public: Foobar(const std::string & name): name(name),
  position(0) {}
  virtual int getStrength() const {
    return position;
  }
  std::string getName() const {
    return name;
  }
  void setPosition(int pos) {
    position = pos;
  }

  protected: std::string name;
  protected: int position;
};

//Declaration of 'Foo' derived class
class Foo: public Foobar {
  public: Foo(const std::string & name): Foobar(name) {}
  // Override the getStrength() function to calculate strength differently for 'Foo' objects
  int getStrength() const override {
    return position * 3;
  }
};
//Declaration of 'Bar' derived class
class Bar: public Foobar {
  public: Bar(const std::string & name): Foobar(name) {}
  // Override the getStrength() function to calculate strength differently for 'Bar' objects
  int getStrength() const override {
    return position + 15;
  }
};
//Function to read input data from a file and create foobar objects
void InputFile(const std::string & inputFile, std::vector < Foobar * > & foobars) {
  std::ifstream input(inputFile);

  std::string line;
  while (std::getline(input, line)) {
    std::string type, name;

    //Find the position of the space character to split the line into foobar name and foobar type
    size_t spacePos = line.find(' ');
    if (spacePos != std::string::npos) {
      type = line.substr(0, spacePos);
      name = line.substr(spacePos + 1);
      //This line declares a pointer to the base class 'Foobar'
      Foobar * foobar;
      if (type == "foo") {
        foobar = new Foo(name);
      } else if (type == "bar") {
        foobar = new Bar(name);
      } else {
        foobar = new Foobar(name);
      }
      foobars.push_back(foobar);
    }
  }
  input.close();
}
//Function to update the strengths of foobars based on their positions in line
void updateStrengths(std::vector < Foobar * > & foobars) {
  int position = 1;
  for (int i = foobars.size() - 1; i >= 0; --i) {
    foobars[i] -> setPosition(position++);
  }
}
//Function to write output data to a file
void OutputFile(const std::string & outputFile,
  const std::vector < Foobar * > & foobars) {
  std::ofstream output(outputFile);

  for (const Foobar * foobar: foobars) {
    output << foobar -> getName() << " " << foobar -> getStrength() << "\n";
  }
  output.close();
}

int main() {
  std::string inputFile, outputFile;
  std::cout << "Enter the name of the input file: ";
  std::cin >> inputFile;
  std::cout << "Enter the name of the output file: ";
  std::cin >> outputFile;

  //Vector declaration named 'foobars' to store pointers to Foobar objects and will be used to keep track of foobars created from the input file.
  std::vector < Foobar * > foobars;
  InputFile(inputFile, foobars);
  updateStrengths(foobars);
  OutputFile(outputFile, foobars);

  //Clean up allocated objects
  for (Foobar * foobar: foobars) {
    delete foobar;
  }
  return 0;
}