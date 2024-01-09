#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

bool CheckPalindromic(const string &str) {
  int len = str.length();
  for (int i = 0; i < len / 2; i++)
    if (str[i] != str[len - i - 1]) return false;
  return true;
}

int main() {
  string instr;
  getline(cin, instr);
  instr += ',';
  size_t start = 0, end = 0;
  while ((end = instr.find(',', start)) != string::npos) {
    string substr = instr.substr(start, end - start);
    if (CheckPalindromic(substr)) cout << substr << endl;
    start = end + 1;
  }
  return 0;
}