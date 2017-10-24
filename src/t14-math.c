#include <stdio.h>
#include <math.h>
#include "t14-math.h"

const double k = 0.00025;
const double m = 0.9;
const double Pi = 3.14159265;

void T14math(double *E, double *St, double *Sb, double *HB, double F, double a0, double a1, double h)
{
	//printf (" F: %.3f\n a0: %.3f\n a1: %.3f\n h: %.3f \n", F, a0 , a1, h); 
	
	double a = a1 - k*pow(F, m);

	*E = 8.927/((1.333*(a - h)*sqrt(0.5*(a + h)))/F - 0.0000444);

	double g = 215000/(215000 + *E);

	double ay = a - h;
	
	//printf (" a: %.3f\n E: %.3f\n g: %.3f\n ay: %.3f \n", a, E, g, ay); 

	double w = g*ay;

	double t = h + w;

	*HB = F/(Pi*t);

	double q = F/(0.5*h);

	//printf (" w: %.3f\n t: %.3f\n HB: %.3f\n q: %.3f \n", w, t, HB, q); 

	*St = 0.343*q;

	*Sb = 3.3**HB;

	//printf (" St: %.3f\n Sb: %.3f\n", St, Sb); 	
}
