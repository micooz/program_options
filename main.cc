#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, const char* argv[]) {
  ifstream fs("README.md");
  string buffer;

  while (std::getline(fs, buffer)) {
    cout << buffer << endl;
  }

  return 0;
}