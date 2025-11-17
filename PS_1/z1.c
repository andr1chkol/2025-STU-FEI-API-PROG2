#include <stdio.h>
#include <math.h>

double functia_1(double x){
 if (x < 0) {
  return 0;
 }
 else {
  return 1;
 }
}

double functia_2(double x) {
 if (x<=0) {
  return 0;
 }
 else {
  return x;
 }
}

double functia_3(double x) {
 const int b = 2;
 return 0.5 * (x +sqrt(x*x + b));
}

double functia_4(double x) {
 return exp(-(x*x));
}

double functia_5(double x) {
 return (exp(x)-exp(-x))/(exp(x)+exp(-x));
}

double functia_6(double x) {
 return x/(1+exp(-0.5*x));
}

int main() {
 double x_start, x_stop, step;
 int precision, width, activation[100], cislo_activacie,  pozicia = 0, rezim = 0;

 if (scanf("%lf %lf %lf %d %d",&x_start, &x_stop, &step, &precision, &width) < 5)
 {
  printf("E1\n");
  return 0;
 }
 if (x_start >= x_stop)
 {
  printf("E2\n");
  return 0;
 }


 while (scanf("%d", &cislo_activacie) == 1) {
  if (cislo_activacie >= 1 && cislo_activacie <= 6) {
   activation[pozicia] = cislo_activacie;
   pozicia++;
   rezim = 1;
  }
 }



 if (rezim == 0) {
  printf("%*c%*d%*d%*d%*d%*d%*d\n",
   width, 'x', width, 1, width, 2, width, 3, width, 4, width, 5, width, 6);

  for (double i = x_start; i <= x_stop; i += step)
  {
   printf("%*.*lf", width, precision, i);
   printf("%*.*lf", width, precision, functia_1(i));
   printf("%*.*lf", width, precision, functia_2(i));
   printf("%*.*lf", width, precision, functia_3(i));
   printf("%*.*lf", width, precision, functia_4(i));
   printf("%*.*lf", width, precision, functia_5(i));
   printf("%*.*lf\n", width, precision, functia_6(i));
  }
  return 0;
 }


 printf("%*c", width, 'x');
 for (double i = x_start; i <= x_stop; i += step) {
  printf("%*.*lf", width, precision, i);
 }
 printf("\n");

 for (int i = 0; i < pozicia; i++) {
  printf("%*d", width, activation[i]);

  for (double x = x_start; x <= x_stop; x += step) {
   if (activation[i] == 1) printf("%*.*lf", width, precision, functia_1(x));
   if (activation[i] == 2) printf("%*.*lf", width, precision, functia_2(x));
   if (activation[i] == 3) printf("%*.*lf", width, precision, functia_3(x));
   if (activation[i] == 4) printf("%*.*lf", width, precision, functia_4(x));
   if (activation[i] == 5) printf("%*.*lf", width, precision, functia_5(x));
   if (activation[i] == 6) printf("%*.*lf", width, precision, functia_6(x));
  }
  printf("\n");
 }
 return 0;
}


