#include <stdlib.h>
#include <stdio.h>

struct _retire_info {
  int months;
  double contribution;
  double rate_of_return;
};

typedef struct _retire_info retire_info;

double calc_balance (double curr_balance, double contr, double rate) {
  
  curr_balance = curr_balance + (curr_balance * ((rate/100)/12) + contr);
  
  return curr_balance;
}


void retirement (int startAge, double initial, retire_info working, retire_info retired) {
  int currAge = startAge;
  //  printf("Age: %i\n", currAge);
  for (int i=0; i<working.months; i++) {
    currAge = startAge+i;
    printf("Age %3d month %2d you have $%.2lf\n", currAge / 12, currAge % 12, initial);
    initial = calc_balance (initial,working.contribution,working.rate_of_return);
  }
  // printf("Age: %i\n", currAge);
  
  for (int i=1; i<=retired.months; i++) {
    printf("Age %3d month %2d you have $%.2lf\n", (currAge + i) /12, (currAge + i) % 12 , initial);
    initial = calc_balance (initial,retired.contribution,retired.rate_of_return);
  }

  
}


int main (void) {
  retire_info working, retired;

  working.months = 489;
  working.contribution = 1000;
  working.rate_of_return = 4.5;

  retired.months = 384;
  retired.contribution = -4000;
  retired.rate_of_return = 1;

  int startAge = 327;
  double initial = 21345;

  retirement(startAge, initial, working, retired);
  
  return 0;  

}
