#include <stdlib.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// width of normal column and the row index column
const int DEFAULT_COLUMN_WIDTH = 7;
const int DEFAULT_ROW_IDX_WIDTH = 4;

// forward declaration of derived classes
class NumberSpreadSheetCell;
class StringSpreadSheetCell;

// types of spreadsheet cells
enum CellType { Number, String };

// The base SpreadSheetCell class
class SpreadSheetCell {
  friend std::ostream& operator<<(std::ostream& os,
                                  const SpreadSheetCell& cell);

 public:
  SpreadSheetCell(CellType type);
  virtual ~SpreadSheetCell() = default;
  virtual bool operator<(const SpreadSheetCell& that) const = 0;
  CellType GetType() const;

 protected:
  const CellType m_type;  // stores the type of the cell (either
                          // CellType::Number or CellType::String)
};

// The `NumberSpreadSheetCell` class, which content is a double
class NumberSpreadSheetCell : public SpreadSheetCell {
  friend std::ostream& operator<<(std::ostream& os,
                                  const SpreadSheetCell& cell);

 public:
  NumberSpreadSheetCell();
  NumberSpreadSheetCell(double num);
  ~NumberSpreadSheetCell() = default;
  bool operator<(const SpreadSheetCell& that) const override;

 private:
  double content_;
};

// The `StringSpreadSheetCell` class, which content is a string
class StringSpreadSheetCell : public SpreadSheetCell {
  friend std::ostream& operator<<(std::ostream& os,
                                  const SpreadSheetCell& cell);

 public:
  StringSpreadSheetCell();
  StringSpreadSheetCell(std::string str);
  ~StringSpreadSheetCell() = default;

  bool operator<(const SpreadSheetCell& that) const override;

 private:
  std::string content_;
};

class SpreadSheet {
 public:
  // constructors and destructors
  SpreadSheet(int rows, int cols);
  ~SpreadSheet();

  // "hide" a row or column in this sheet, hidden row or column
  // will not be shown in "ExportSheet" function
  void HideRow(int row);
  void HideColumn(int col);

  // re-show a hidden row or column in this sheet.
  // If the given row is not hidden, this does nothing.
  void ShowRow(int row);
  void ShowColumn(int col);

  // set a the pointer at (row, col) to `cell`,
  // if there is already be a cell, delete it and reset it to `cell`
  void UpdateCell(int row, int col, SpreadSheetCell* cell);

  /* print the sheet to the ostream `os`. You may assume that `os' is std::cout.
   * Each column has width = DEFAULT_COLUMN_WIDTH
   * It's guaranteed that length of all cell contents are less than
   * DEFAULT_COLUMN_WIDTH The width of the column of ROW INDEX has width =
   * DEFAULT_ROW_IDX_WIDTH row index and column index are all ints which starts
   * from 1. If your implementation is correct, the output of given "main"
   * function will be similar to: |      1      2      3      4
   * ---------------------------------
   *    1|  0.658   qufk    vry   3.43
   *    2|     ld   fdkr    omx  0.892
   *    3|    mlg   uqpb   4.91      r
   *    4|   wneo   4.49   atdh   itbf
   */
  void ExportSheet(std::ostream& os);

  // PROBLEM 3: Sort all rows by the value in `col`, in accending order, and
  // number < string < nullptr.
  void SortByColumn(int col);

 private:
  std::vector<std::vector<SpreadSheetCell*>> m_cells;
  std::vector<bool> m_showcol, m_showrow;
  int m_row, m_col;
  // Add your own private variables and functions here
};
class ColumnComparator {
 public:
  ColumnComparator(int col);
  ~ColumnComparator() = default;
  bool operator()(const std::vector<SpreadSheetCell*>& first,
                  const std::vector<SpreadSheetCell*>& second) const;

 private:
  int m_col;
};
// SpreadSheetCell
SpreadSheetCell::SpreadSheetCell(CellType type) : m_type(type){};
CellType SpreadSheetCell::GetType() const { return this->m_type; }
// NumberSpreadSheetCell
NumberSpreadSheetCell::NumberSpreadSheetCell()
    : SpreadSheetCell(CellType::Number) {
  content_ = 0;
}
NumberSpreadSheetCell::NumberSpreadSheetCell(double num)
    : SpreadSheetCell(CellType::Number) {
  content_ = num;
}
bool NumberSpreadSheetCell::operator<(const SpreadSheetCell& that) const {
  if (that.GetType() == CellType::String) return true;
  const NumberSpreadSheetCell& thatnum =
      dynamic_cast<const NumberSpreadSheetCell&>(that);
  return content_ < thatnum.content_;
}
// StringSpreadSheetCell
StringSpreadSheetCell::StringSpreadSheetCell()
    : SpreadSheetCell(CellType::String) {}
StringSpreadSheetCell::StringSpreadSheetCell(std::string str)
    : SpreadSheetCell(CellType::String) {
  content_ = str;
}
bool StringSpreadSheetCell::operator<(const SpreadSheetCell& that) const {
  if (that.GetType() == CellType::Number) return false;
  const StringSpreadSheetCell& thatstr =
      dynamic_cast<const StringSpreadSheetCell&>(that);
  return content_ < thatstr.content_;
}
// SpreadSheet
SpreadSheet::SpreadSheet(int rows, int cols)
    : m_showcol(cols, true), m_showrow(rows, true), m_row(rows), m_col(cols) {
  for (int i = 0; i < rows; i++) {
    std::vector<SpreadSheetCell*> rowvecotr;
    for (int j = 0; j < cols; j++) rowvecotr.push_back(nullptr);
    m_cells.push_back(rowvecotr);
  }
}
SpreadSheet::~SpreadSheet() {
  for (auto row : m_cells)
    for (auto it = row.begin(); it != row.end(); ++it) {
      if (*it != nullptr) delete *it;
      *it = nullptr;
    }
}
void SpreadSheet::HideRow(int row) {
  if (row >= 1 && row <= m_row) m_showrow[row - 1] = false;
}
void SpreadSheet::HideColumn(int col) {
  if (col >= 1 && col <= m_col) m_showcol[col - 1] = false;
}
void SpreadSheet::ShowRow(int row) {
  if (row >= 1 && row <= m_row) m_showrow[row - 1] = true;
}
void SpreadSheet::ShowColumn(int col) {
  if (col >= 1 && col <= m_col) m_showcol[col - 1] = true;
}
void SpreadSheet::UpdateCell(int row, int col, SpreadSheetCell* cell) {
  if (row < 1 || row > m_row || col < 1 || col > m_col) return;
  if (m_cells[row - 1][col - 1] != nullptr) delete m_cells[row - 1][col - 1];
  m_cells[row - 1][col - 1] = cell;
}
void SpreadSheet::ExportSheet(std::ostream& os) {
  int columncount = 0;
  os << std::setw(DEFAULT_ROW_IDX_WIDTH + 1) << '|';
  for (int i = 0; i < m_col; i++)
    if (m_showcol[i]) {
      os << std::setw(DEFAULT_COLUMN_WIDTH) << i + 1;
      ++columncount;
    }
  os << std::endl;
  os << std::setfill('-')
     << std::setw(columncount * DEFAULT_COLUMN_WIDTH + DEFAULT_ROW_IDX_WIDTH +
                  1)
     << '-' << std::endl
     << std::setfill(' ');
  for (int i = 0; i < m_row; i++) {
    if (!m_showrow[i]) continue;
    os << std::setw(DEFAULT_ROW_IDX_WIDTH) << i + 1 << '|';
    for (int j = 0; j < m_col; j++) {
      if (!m_showcol[j]) continue;
      os << std::setw(DEFAULT_COLUMN_WIDTH);
      if (m_cells[i][j] == nullptr)
        os << ' ';
      else
        os << *m_cells[i][j];
    }
    os << std::endl;
  }
}
void SpreadSheet::SortByColumn(int col) {
  std::sort(m_cells.begin(), m_cells.end(), ColumnComparator(col));
}
// ColumnComparator
ColumnComparator::ColumnComparator(int col) : m_col(col) {}
bool ColumnComparator::operator()(
    const std::vector<SpreadSheetCell*>& first,
    const std::vector<SpreadSheetCell*>& second) const {
  if (first[m_col - 1] == nullptr) return false;
  if (second[m_col - 1] == nullptr) return true;
  return *first[m_col - 1] < *second[m_col - 1];
}

std::ostream& operator<<(std::ostream& os, const SpreadSheetCell& cell) {
  if (cell.GetType() == CellType::Number) {
    const NumberSpreadSheetCell& numbercell =
        dynamic_cast<const NumberSpreadSheetCell&>(cell);
    os << std::setprecision(3) << numbercell.content_ << std::setprecision(6);
  } else if (cell.GetType() == CellType::String) {
    const StringSpreadSheetCell& stringcell =
        dynamic_cast<const StringSpreadSheetCell&>(cell);
    os << stringcell.content_;
  }
  return os;
}

int main() {
  // Question 1
  // std::vector<SpreadSheetCell*> cells;
  // cells.push_back(new NumberSpreadSheetCell(1.5333));
  // cells.push_back(new NumberSpreadSheetCell(2.0));
  // cells.push_back(new StringSpreadSheetCell("abc"));
  // cells.push_back(new StringSpreadSheetCell("bbb"));
  // std::cout << std::boolalpha << (*(cells[0]) < *(cells[1])) << std::endl;
  // // true (1.5333 < 2.0)
  // std::cout << std::boolalpha << (*(cells[1]) < *(cells[0])) << std::endl;
  // // false (2.0 > 1.5333)
  // std::cout << std::boolalpha << (*(cells[2]) < *(cells[3])) << std::endl;
  // // true ("abc" < "bbb")
  // std::cout << std::boolalpha << (*(cells[3]) < *(cells[2])) << std::endl;
  // // false ("bbb" > "abc")
  // std::cout << std::boolalpha << (*(cells[0]) < *(cells[3])) << std::endl;
  // // true (numbers < strings)
  // std::cout << std::boolalpha << (*(cells[3]) < *(cells[0])) << std::endl;
  // // false (strings > numbers)
  // std::cout << (*cells[0]) << std::endl;
  // // 1.53 (precision = 3)
  // std::cout << (*cells[2]) << std::endl;
  // // abc (just print it)
  // std::cout << cells[0]->GetType() << std::endl;
  // // 0 (CellType::Number)
  // std::cout << cells[2]->GetType() << std::endl;
  // // 1 (CellType::String)
  // Question 2
  SpreadSheet sheet(5, 5);
  sheet.UpdateCell(1, 1, new NumberSpreadSheetCell(1.1));
  sheet.UpdateCell(1, 3, new NumberSpreadSheetCell(3.4));
  sheet.UpdateCell(2, 4, new StringSpreadSheetCell("foo"));
  sheet.UpdateCell(3, 1, new StringSpreadSheetCell("bar"));
  sheet.UpdateCell(3, 2, new StringSpreadSheetCell("baz"));
  sheet.UpdateCell(4, 1, new NumberSpreadSheetCell(0.5));
  sheet.UpdateCell(4, 2, new NumberSpreadSheetCell(0));
  sheet.UpdateCell(5, 1, new StringSpreadSheetCell("raz"));

  // sheet.HideColumn(3);
  // sheet.HideRow(2);
  sheet.ExportSheet(std::cout);
  sheet.SortByColumn(1);
  sheet.ExportSheet(std::cout);
}
