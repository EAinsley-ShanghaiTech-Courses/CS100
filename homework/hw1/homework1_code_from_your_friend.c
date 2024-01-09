#include <math.h>
#include <stdio.h>

int main() {
  double numTotalStudents;
  double numSatisfied;
  double satisfiedRatio;
  const double Eps = 1e-6;
  printf("How many students went to the new dining hall?\n");
  scanf("%lf", &numTotalStudents);
  printf("How many of them are satisfied?\n");
  scanf("%lf", &numSatisfied);

  satisfiedRatio = numSatisfied / numTotalStudents;

  // Prints different messages according to the ratio!
  if (fabs(satisfiedRatio - 0.5) < Eps) {
    printf("Exactly half of the students are satisfied!\n");
  } else if (satisfiedRatio > 0.5) {
    printf("More students are satisfied! :)\n");
  } else if (satisfiedRatio < 0.5) {
    printf("More students are unsatisfied! :(\n");
  } else // In case if it divides by zero
  {
    printf("No one went to the dining hall?\n");
  }
  return 0;
}