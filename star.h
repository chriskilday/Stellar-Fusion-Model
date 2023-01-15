/* star.h */

// Standard units: R_sol M_sol L_sol s
#define G ((long double) 3.9423e-7)
#define M_H ((long double) 8.411e-58)
#define K ((long double) 1.435e-71)

typedef struct Star_Model {
	long double m;
} Star;

long double radius(Star*);
long double mass(double, Star*);
long double density(double, Star*);
long double pressure(double, Star*);
long double temperature(double, Star*);
long double luminosity(double, Star*);
long double normalized_dldr(double, Star*, long double);
