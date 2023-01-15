/* star.c */

#include <math.h>
#include <stdio.h>

#include "star.h"

#define NSTEPS 1000

long double radius(Star* s) {
	double a = 0.6;

	return pow(s->m, a);
}

long double central_density(Star* s) {
	return (3/M_PI) * (s->m / pow(radius(s), 3));
}

long double density(double r, Star* s) {
	return central_density(s) * (1 - r / radius(s));
}

long double mass(double r, Star* s) {
	return M_PI*central_density(s) * pow(r, 3) * (4.0/3.0 - r / radius(s));
}

long double central_pressure(Star* s){
	return (5.0/36.0)*M_PI*G * pow(radius(s), 2)*pow(central_density(s), 2);
}

long double pressure(double r, Star* s) {
	double R = radius(s);
	return central_pressure(s) +
	       (1.0/36.0) * M_PI * G *
	       pow(central_density(s), 2) * pow(R, 2) *
	       (-9*(pow(r,4) / pow(R,4)) + 28*(pow(r,3) / pow(R,3)) - 24*(pow(r,2) / pow(R,2)));
}

double mean_molecular_weight(Star* s) {
	double X = 1;
	double Z = 0;

	return 4.0 / (3 + 5*X - Z);
}

long double temperature(double r, Star* s) {
	return mean_molecular_weight(s) * M_H * pressure(r,s) / (K * density(r,s));
}

long double dldr(double r, Star* s) {
	long double ppchain = (3.07e-17)*density(r, s)*pow(temperature(r, s),4);
	long double eps = ppchain;

	return (4*M_PI) * pow(r,2) * density(r,s) * eps;
}

long double simpsons_dldr(double r, double h, Star* s) {
	return (1.0/6.0) * h * (dldr(r,s) + 4*dldr(r+0.5*h,s) + dldr(r+h,s));
}

long double luminosity(double r, Star* s) {
	long double out = 0.0;
	double h = r / ((double) NSTEPS);

	for (double ri = 0; ri <= r; ri += h)
		out += simpsons_dldr(ri, h, s);

	return out;
}

long double normalized_dldr(double r, Star* s, long double ltot) {
	return dldr(r, s) / ltot;
}
