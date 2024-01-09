#include <math.h>
#include <stdlib.h>

#include <iomanip>
#include <iostream>
#include <list>
#include <string>

const float PI = 3.14159f;

/***********************************************************************
 * Declaration/Definition of the base-class Shape *
 ***********************************************************************/
class Shape {
 public:
  // constructors and destructor
  Shape(int corner);
  virtual ~Shape() = default;

  /*
   * No need to use virtual function. Since the size of the shape will not be
   * altered during runtime, there's no need to record those parameters and thus
   * the calculations can be done during initialization. (Although the name of
   * the fucntions seems to be inappropriate then.) */

  float CalculateArea() const;
  float CalculatePerimeter() const;
  int NumberCorners() const;

 protected:
  const int corner;
  float area, perimeter;

 private:
  // member variables;
};

/***********************************************************************
 * Declaration/Definition of the child-classes *
 ***********************************************************************/
class Rectangle : public Shape {
 public:
  Rectangle(float size1, float size2);
  ~Rectangle();
  int NumberCorners() const;

};  // ...
class Square : public Shape {
 public:
  Square(float size);
  ~Square();
  int NumberCorners() const;
};  // ...
class Triangle : public Shape {
 public:
  Triangle(float size1, float size2);
  ~Triangle();
  int NumberCorners() const;
};  // ...
class Circle : public Shape {
 public:
  Circle(float size);
  ~Circle();
  int NumberCorners() const;
};  // ...

/************************************************************************
 * Main Function which is creating/reporting database (do not change!) *
 ************************************************************************/
int main() {
  // initialize an empty list of shapes
  std::list<Shape *> shapeDatabase;
  // interact with the user: ask the user to enter shapes one by one
  while (1) {
    // print instructions as to how to enter a shape
    std::cout << "Enter a type (Circle, Triangle, Square, or Rectangle) ";
    std::cout << "and one or two size parameters, ";
    std::cout << "separated with blank spaces:\n";
    float size1;
    float size2;
    // check which shape has been requested and store in the database
    std::string shapeType;
    std::cin >> shapeType;
    if (shapeType == std::string("Circle")) {
      std::cin >> size1;
      shapeDatabase.push_back(new Circle(size1));
    } else if (shapeType == std::string("Triangle")) {
      std::cin >> size1 >> size2;
      shapeDatabase.push_back(new Triangle(size1, size2));
    } else if (shapeType == std::string("Square")) {
      std::cin >> size1;
      shapeDatabase.push_back(new Square(size1));
    } else if (shapeType == std::string("Rectangle")) {
      std::cin >> size1 >> size2;
      shapeDatabase.push_back(new Rectangle(size1, size2));
    } else {
      std::cout << "Unrecognized shape!!\n";
    }
    // check if the user wants to add more shapes
    std::cout << "Do you want to add more shapes? (Enter Y or N)\n";
    std::string answer;
    std::cin >> answer;
    if (answer != std::string("Y")) break;
  }
  // iterate through the list and output the area, perimeter,
  // and number of corners of each entered shape
  std::list<Shape *>::iterator it = shapeDatabase.begin();
  int shapeCounter = 0;
  while (it != shapeDatabase.end()) {
    std::cout << "Properties of shape " << shapeCounter++ << ":\n";
    std::cout << "Area: " << std::setprecision(5) << (*it)->CalculateArea()
              << "\n";
    std::cout << "Perimeter: " << std::setprecision(5)
              << (*it)->CalculatePerimeter() << "\n";
    std::cout << "Corners: " << (*it)->NumberCorners() << "\n";
    std::cout << std::endl;
    it++;
  }

  it = shapeDatabase.begin();
  while (it != shapeDatabase.end()) {
    delete (*it);
    it = shapeDatabase.erase(it);
  }

  return 0;
}
Shape::Shape(int c) : corner(c), area(0), perimeter(0){};
float Shape::CalculateArea() const { return area; }
float Shape::CalculatePerimeter() const { return perimeter; }
int Shape::NumberCorners() const { return corner; }

Rectangle::Rectangle(float length, float height) : Shape(4) {
  area = length * height;
  perimeter = length * 2 + height * 2;
}
Rectangle::~Rectangle() { std::cout << "A Rectangle has been destroyed.\n"; }

Circle::Circle(float radius) : Shape(0) {
  area = PI * radius * radius;
  perimeter = 2 * PI * radius;
}
Circle::~Circle() { std::cout << "A Circle has been destroyed.\n"; }

Triangle::Triangle(float length1, float length2) : Shape(3) {
  area = 0.5 * length1 * length2;
  perimeter = length1 + length2 + sqrt(length1 * length1 + length2 * length2);
}
Triangle::~Triangle() { std::cout << "A Triangle has been destroyed.\n"; }

Square::Square(float length) : Shape(4) {
  area = length * length;
  perimeter = 4 * length;
}
Square::~Square() { std::cout << "A Square has been destroyed.\n"; }
