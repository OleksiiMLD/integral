/*
 * integequ.h
 *
 */

#ifndef INTEGEQU_H_
#define INTEGEQU_H_

#include <vector>
#include <valarray>

namespace Inteq
{


/* - Variables -*/
extern const int eqcount;
extern int eqIndex;
/* form of equition */
extern int formIndex;

/* bounds of integral */
extern double a;
extern double b;
/* lambda */
extern double l;
/* number of approximation intervals */
extern int n;
extern double h;

extern std::string str;
extern std::ostringstream strs;

/* Matrixes */
/* for core (K) */
extern std::vector<std::valarray<double> > MK;
extern std::vector<std::valarray<double> > Ma;

/* arrays of functions koefs for approximation intervals */
extern std::valarray<double> VA;
extern std::valarray<double> Vf;
extern std::valarray<double> Vx;
extern std::valarray<double> Vy;

/* - Classes -*/
class DetZero;

/* - Functions -*/
void calc_eq(int n);
/* determinant of matrix*/
double det_Ma(std::vector<std::valarray<double> >);
/* set quadrature folmula */
void set_form(int formulaIndex);

/* Functions that depends on equations */
/* equation string for display to user */
const char* eqstring(int);
const char* eqstring_Eq1(void);
const char* eqstring_Eq2(void);
const char* eqstring_Eq3(void);

/* set equation */
void set_eq(int equationIndex);
void set_Eq1(void);
void set_Eq2(void);
void set_Eq3(void);

/* set equation */
double f(double &);
double f_Eq1(double);
double f_Eq2(double);
double f_Eq3(double);

/* equation core */
double K(double &, double &);
double K_Eq1(double, double);
double K_Eq2(double, double);
double K_Eq3(double, double);

/* textual form of calculated equation*/
const char* ystr(void);
const char* ystr_Eq1(void);
const char* ystr_Eq2(void);
const char* ystr_Eq3(void);

double y(double &);

void h_set(void);
double h_1(void);
double h_2(void);

double x(int &);
double x_1(int);
double x_2(int);

double A(int &);
double A_1(int);
double A_2(int);

};

#endif /* INTEGEQU_H_ */
