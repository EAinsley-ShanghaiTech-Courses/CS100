#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::vector;

class ReportParser {
 public:
  ReportParser(int numStudents, int numProblems);
  ~ReportParser() = default;
  void readReport();
  void writeStructuredReport() const;

 private:
  inline void writeLines() const;
  inline void writeContent(int line) const;
  int num_students_, num_problems_;
  vector<vector<string>> scoretable_;
  vector<int> widthfield_;

  // Your additional declarations here
};

// DO NOT modify the main function
int main() {
  int n, m;
  cin >> n >> m;
  getchar();
  ReportParser a(n, m);
  a.readReport();
  a.writeStructuredReport();
  return 0;
}

ReportParser::ReportParser(int numStudent, int numProblems)
    : num_students_(numStudent), num_problems_(numProblems) {
  widthfield_.resize(num_problems_);
}

void ReportParser::readReport() {
  for (int i = 0; i < num_students_; i++) {
    vector<string> rows(0);
    for (int j = 0; j < num_problems_; j++) {
      string input;
      std::cin >> input;
      rows.push_back(input);
      widthfield_[j] = std::max(widthfield_[j], int(input.length()));
    }
    scoretable_.push_back(rows);
  }
}

void ReportParser::writeStructuredReport() const {
  cout << std::left;
  int borderlength = 3 * num_problems_;
  for (int i = 0; i < num_problems_; i++) borderlength += widthfield_[i];
  cout << std::setfill('-') << std::setw(borderlength) << "/" << std::setw(0)
       << "\\\n";
  for (int i = 0; i < num_students_ - 1; i++) {
    writeContent(i);
    writeLines();
  }
  writeContent(num_students_ - 1);
  cout << std::setfill('-') << std::setw(borderlength) << "\\" << std::setw(0)
       << "/\n";
}

inline void ReportParser::writeContent(int line) const {
  cout << std::setfill(' ');
  for (int i = 0; i < num_problems_; i++)
    cout << "| " << std::setw(widthfield_[i] + 1) << scoretable_[line][i];
  cout << std::setw(0) << "|\n";
}

inline void ReportParser::writeLines() const {
  cout << std::setfill('-');
  for (int i = 0; i < num_problems_; i++) {
    cout << std::setw(widthfield_[i] + 3) << "|";
  }
  cout << std::setw(0) << "|\n";
}
