/*
 * integequ.cpp
 *
 */

#include <iostream>
#include <vector>
#include <valarray>
#include <cmath>
#include <string>
#include <sstream>

#include "integequ.h"

const int Inteq::eqcount = 3;
int Inteq::eqIndex = 0;
int Inteq::formIndex = 0;

double Inteq::a = 0;
double Inteq::b = 0;
double Inteq::l = 0;
double Inteq::h = 0;
int Inteq::n = 0;

std::string Inteq::str("");
std::ostringstream Inteq::strs(" ");

std::vector< std::valarray<double> > Inteq::MK;
std::vector< std::valarray<double> > Inteq::Ma;

std::valarray<double> Inteq::VA;
std::valarray<double> Inteq::Vf;
std::valarray<double> Inteq::Vx;
std::valarray<double> Inteq::Vy;

class Inteq::DetZero {};


void Inteq::calc_eq(int ln)
{
	n = ln;
	h_set();

	MK.resize(n);
	Ma.resize(n);
	for(int i = 0; i<n; ++i)
	{
		MK[i].resize(n);
		Ma[i].resize(n);
	}

	VA.resize(n);
	Vf.resize(n);
	Vx.resize(n);
	Vy.resize(n);

	/* Calculates main variables and koefs */
	for(int i = 0; i<n; ++i)
	{
		Vx[i] = x(i);
		Vf[i] = f(Vx[i]);
		VA[i] = A(i);
	}

	for(int i = 0; i<n; ++i)
	{
		for(int t = 0; t<n; ++t)
		{
			MK[i][t] = K(Vx[i],Vx[t]);
		}
	}

	for(int i = 0; i<n; ++i)
	{
		for(int t = 0; t<n; ++t)
		{
			if (i==t)
				Ma[i][t] = 1-(l*VA[t]*MK[i][t]);
			else
				Ma[i][t] = -l*VA[t]*MK[i][t];
		}
	}

	/* If determinant of koef matrix != 0 then system of equials has 1 res */
	double d_Ma = det_Ma(Ma);
	if (d_Ma == 0)
		throw (DetZero());
	else
	{


/*
 *   Gauss's method (begin)
 */

		std::vector< std::valarray<double> > Mat(Ma);
		std::valarray<int> Vaj(n-1);
		std::valarray<double> Vft(Vf);
		std::valarray<double> Vyt(Vy);

		/* direct iterations */
		for (int i = 0; i<(n-1); ++i)
		{
			/* determination of maximum value in a row */
			double tmp_a = Mat[i][i];
			int tmp_aj = i;
			std::vector<double>::iterator tmp_it;

			for(int j = i; j<(n-1); ++j)
			{
				if (tmp_a < Mat[i][j+1])
				{
					tmp_a = Mat[i][j+1];
					tmp_aj = j+1;
				};
			}
			Vaj[i] = tmp_aj;

			/* transform koefs in current row */
			for(int j = i; j<n; ++j)
			{
				Mat[i][j] /= tmp_a;
			}
			Vft[i] /= tmp_a;

			/* change positions of koefs */
			std::valarray<double> tmp_va(Mat[i]);
			Mat[i][i] = tmp_va[tmp_aj];
			for (int k=i; k<tmp_aj; ++k)
			{
				Mat[i][k+1] = tmp_va[k];
			}

			/* subtraction of koefs */
			for (int k = i+1; k<n; ++k)
			{
				double koef = Mat[k][i];
				for (int j = i; j<n; ++j)
				{
					Mat[k][j] = Mat[k][j] - (koef * Mat[i][j]);
				}
				Vft[k] = Vft[k] - (koef * Vft[i]);
			}
		}

		/* revers iterations */
		Vyt[n-1] = Vft[n-1]/Mat[n-1][n-1];

		for (int i = n-2; i>=0; --i)
		{
			/* calculate yi */
			double sum = 0;
			for (int j = i+1; j < n; j++)
				sum += Mat[i][j]*Vyt[j];
			Vyt[i] = Vft[i] - sum;

			/* reverse Mat */
			std::valarray<double> tmp_va(Mat[i]);

			Mat[i][Vaj[i]] = tmp_va[i];
			for (int k=i; k<Vaj[i]; ++k)
			{
				Mat[i][k] = tmp_va[k+1];
			}

			/* reverse y */
			tmp_va = Vyt;
			Vyt[Vaj[i]] = tmp_va[i];
			for (int k=i; k<Vaj[i]; ++k)
			{
				Vyt[k] = tmp_va[k+1];
			}
		}

		Vy = Vyt;

/*
 *   Gauss's method (end)
 */

	}
}


double Inteq::det_Ma(std::vector<std::valarray<double> > lMa)
{
   double det = 0;
   int n = lMa.size();

   if (n<1)
   {
	   det = 0;
   }
   else if (n == 1)
   {
	   det = lMa[0][0];
   }
   else if (n == 2)
   {
	   det = lMa[0][0] * lMa[1][1] - lMa[1][0] * lMa[0][1];
   }
   else
   {
	   for (int i=0; i<n; ++i)
	   {
		   std::vector<std::valarray<double> > slMa(n-1);
		   for (int j=0; j<(n-1); ++j) slMa[j].resize(n-1);
		   for (int j=1; j<n; ++j)
		   {
			   int i1 = 0;
			   for(int i2=0; i2<n; ++i2)
			   {
				   if (i2 == i) continue;
				   slMa[j-1][i1] = lMa[j][i2];
				   ++i1;
			   }
		   }
		   det += pow(-1.0, 1.0 + (i+1.0)) * lMa[0][i] * det_Ma(slMa);
	  }
   }
   return det;
}


/*
 *  Quadrature formulas section
 */
void Inteq::set_form(int lfi)
{
	formIndex = lfi;
}


void Inteq::h_set(void)
{
	switch(formIndex)
	{
	case 1:
		h = h_1();
		break;
	case 2:
		h = h_2();
		break;
	default:
		h = 0;
	}
}


double Inteq::x(int &i)
{
	switch(formIndex)
	{
	case 1:
		return	x_1(i);
	case 2:
		return	x_2(i);
	default:
		return 0;
	}
}


double Inteq::A(int &i)
{
	switch(formIndex)
	{
	case 1:
		return	A_1(i);
	case 2:
		return	A_2(i);
	default:
		return 0;
	}
}



/*
 *  Trapezium formula section
 */
double Inteq::h_1(void)
{
	return (b - a)/(n - 1);
}


double Inteq::x_1(int i)
{
	return a + h*((i+1)-1);
}


double Inteq::A_1(int i)
{
	if ( (i==0) || (i==(n-1)))
		return 0.5*h;
	else
		return h;
}


/*
 *  Simpsons formula section
 */
double Inteq::h_2(void)
{
	return (b - a)/(n - 1);
}


double Inteq::x_2(int i)
{
	return a + h*((i+1)-1);
}


double Inteq::A_2(int i)
{
	if ( (i==0) || (i==(n-1)))
		return h/3;
	else if (i%2 != 0)
		return h*4/3;
	else
		return h*2/3;
	return 0;
}




/*
 *  Equtions section
 */

void Inteq::set_eq(int lei)
{
	eqIndex = lei;

	switch(eqIndex)
	{
	case 1:
		set_Eq1();
		break;
	case 2:
		set_Eq2();
		break;
	case 3:
		set_Eq3();
		break;
	default:
		;
	}
}


const char* Inteq::eqstring(int lei)
{
	switch(lei)
	{
	case 1:
		return eqstring_Eq1();
	case 2:
		return eqstring_Eq2();
	case 3:
		return eqstring_Eq3();
	default:
		return "Error: Equation not specified!";
	}
}


double Inteq::f(double &xi)
{
	switch(eqIndex)
	{
	case 1:
		return	f_Eq1(xi);
	case 2:
		return	f_Eq2(xi);
	case 3:
		return	f_Eq3(xi);
	default:
		return 0;
	}
}


double Inteq::K(double &x, double &t)
{
	switch(eqIndex)
	{
	case 1:
		return	K_Eq1(x,t);
	case 2:
		return	K_Eq2(x,t);
	case 3:
		return	K_Eq3(x,t);
	default:
		return 0;
	}
}


const char* Inteq::ystr(void)
{
	switch(eqIndex)
	{
	case 1:
		return ystr_Eq1();
	case 2:
		return ystr_Eq2();
	case 3:
		return ystr_Eq3();
	default:
		return "Error: Equation not specified!";
	}
}


double Inteq::y(double & x)
{
	double y = f(x);
	for (int i=0; i<n; ++i)
	{
		y += l * VA[i] * K(x, Vx[i]) * Vy[i];
	}
	return y;
}


/*
 * Equition 1 parameters
 */
const char* Inteq::eqstring_Eq1()
{
	return "y(x)=(5/6)*x+(1/2)*integral[0,1]x*t*y(t)dt";
}


void Inteq::set_Eq1(void)
{
	a = 0;
	b = 1;
	l = 0.5;
}


double Inteq::f_Eq1(double xi)
{
	return xi*5/6;
}


double Inteq::K_Eq1(double x, double t)
{
	return x*t;
}


const char* Inteq::ystr_Eq1()
{
	str = "y(x) = (5/6)*x";
	for (int i=0; i<n; ++i)
	{
		str += " + (1/2)*(";
		strs.str(" ");
		strs << VA[i];
		str += strs.str();
		str += ")*(";
		strs.str(" ");
		strs << Vy[i];
		str += strs.str();
		str += ")*(";
		strs.str(" ");
		strs << Vx[i];
		str += strs.str();
		str += ")*x";
	}

	return str.c_str();
}


/*
 *  Equition 2 parameters
 */
const char* Inteq::eqstring_Eq2()
{
	return "y(x)=1+integral[0,1]x*t^2*y(t)dt";
}


void Inteq::set_Eq2(void)
{
	a = 0;
	b = 1;
	l = 1;
}


double Inteq::f_Eq2(double)
{
	return 1;
}


double Inteq::K_Eq2(double x, double t)
{
	return x*pow(t,2);
}


const char* Inteq::ystr_Eq2()
{
	str = "y(x) = 1";
	for (int i=0; i<n; ++i)
	{
		str += " + (";
		strs.str(" ");
		strs << VA[i];
		str += strs.str();
		str += ")*(";
		strs.str(" ");
		strs << Vx[i];
		str += strs.str();
		str += ")^2*(";
		strs.str(" ");
		strs << Vy[i];
		str += strs.str();
		str += ")*x";
	}
	return str.c_str();
}


/*
 *  Equition 3 parameters
 */
const char* Inteq::eqstring_Eq3()
{
	return "y(x)=1+(4/3)*x+integral[-1,1]((x*t^2)-x)*y(t)dt";
}


void Inteq::set_Eq3(void)
{
	a = -1;
	b = 1;
	l = 1;
}


double Inteq::f_Eq3(double xi)
{
	return 1 + xi*4/3;
}


double Inteq::K_Eq3(double x, double t)
{
	return x*pow(t,2) - x;
}


const char* Inteq::ystr_Eq3()
{
	str = "y(x) = 1 + (4/3)*x";
	for (int i=0; i<n; ++i)
	{
		str += " + (";
		strs.str(" ");
		strs << VA[i];
		str += strs.str();
		str += ")*(";
		strs.str(" ");
		strs << Vy[i];
		str += strs.str();
		str += ")*(x*(";
		strs.str(" ");
		strs << Vx[i];
		str += strs.str();
		str += ")^2-x)";
	}
	return str.c_str();
}

