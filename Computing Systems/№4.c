#include <stdio.h>
#include <math.h>
#include <locale.h>
const double eps = 0.000001;	
double dabs(double);
double dichotomy(double f(double), double, double);
double iteration(double f(double), double, double);
double tangent(double f(double), double F(double), double, double);
double chord(double f(double), double, double);
double f1(double);
double F1(double);
double Fp1(double);
double f2(double);
double F2(double);
double Fp2(double);
double f3(double);
double F3(double);
double Fp3(double);

int main() {
printf("Корень функции f1 методом деления пополам = %.5f\n", dichotomy(f1,1.,2.));
printf("Корень функции f1 методом итераций = %.5f\n", iteration(F1, 1, 2));
printf("Корень функции f1 методом касательных = %.5f\n", tangent(f1,Fp1,1.,2.));
printf("Корень функции f1 методом хорд = %.5f\n\n", chord(f1,1.,2.));

printf("Корень функции f2 методом деления пополам = %.5f\n", dichotomy(f2,1.,2.));
printf("Корень функции f2 методом итераций = %.5f\n", iteration(F2,1, 2));
printf("Корень функции f2 методом касательных = %.5f\n", tangent(f2,Fp2,1.,2.));
printf("Корень функции f2 методом хорд = %.5f\n\n", chord(f2,1.,2.));

printf("Корень функции f3 методом деления пополам = %.5f\n", dichotomy(f3,0.,2.));
printf("Корень функции f3 методом итераций = %.5f\n", iteration(F3,0,2));
printf("Корень функции f3 методом касательных = %.5f\n", tangent(f3,Fp3,0.,2.));
printf("Корень функции f3 методом хорд = %.5f\n\n", chord(f3,0.,2.));
return 0;}

double f1(double x) { return tan(x/2) - cos(x/2)/sin(x/2) + x; }
double F1(double x) { return cos(x/2)/sin(x/2) - tan(x/2); }
double Fp1(double x) { return 1 + 1/(2*cos(x/2)*cos(x/2)) + 1/(2*sin(x/2)*sin(x/2)); }
double f2(double x) { return log(x) - x*x + 3; }
double F2(double x) { return sqrt(log(x)+3); }
double Fp2(double x) { return 1/x - 2*x; }
double f3(double x) { return cos(x) + sin(x) - 2*x; }
double F3(double x) { return (cos(x)+sin(x))/2; }
double Fp3(double x) { return cos(x) - sin(x) - 2; }
double dabs(double x) { return (x > 0 ? x : -x); }

double dichotomy(double f(double), double a, double b) {
    double prevX = b, x = (a + b) / 2.;
    while(dabs(prevX - x) > eps) {
        if(f(x)*f(a) > 0) a = x; else b = x;
        prevX = x; x = (a + b) / 2.;}
    return x;}
    
double iteration(double f(double), double a, double b) {
    double prevX = (a+b)/2., x = prevX + 1;
    while(dabs(x-prevX) > eps) {
        prevX = x; x = f(x);}
    return x;}
    
double tangent(double f(double), double F(double), double a, double b) {
    double prevX = (a+b/2.), x = prevX - f(prevX)/F(prevX);
    while(dabs(prevX - x) > eps) {
        prevX = x; x = prevX - f(prevX)/F(prevX);}
    return x;}
    
double chord(double f(double), double a, double b) {
    double prevX = b, ya = f(a), yb = f(b);
    double x = (ya*b-yb*a)/(ya-yb);
    while(dabs(prevX - x) > eps) {
        if(ya*f(x) > 0) a = x; else b = x;
        ya = f(a), yb = f(b); prevX = x; x = (ya*b-yb*a)/(ya-yb);}
    return x;}
