/* stellar_fusion.c */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "star.h"

#define TOFR_FILENAME "tofr"
#define DLDR_FILENAME "dldr"
#define LOFM_FILENAME "lofm.txt"
#define NOUT 100

void out_tofr_all(char*, int, ...);
void out_lofm_plot(double, double, char*);
void out_dldr_all(char*, int, ...);

int main(int argc, char* argv[]) {
	Star s1 = {2.0};
	Star s2 = {5.0};
	Star s3 = {8.0};
	Star s4 = {15.0};
	Star s5 = {20.0};

	//out_tofr_all(TOFR_FILENAME, 5, &s1, &s2, &s3, &s4, &s5);
	//out_dldr_all(DLDR_FILENAME, 5, &s1, &s2, &s3, &s4, &s5);
	//out_lofm_plot(1.0, 50.0, LOFM_FILENAME);

	return 0;
}

char* outstr_temperature(double r, Star* s, char* buff) {
	sprintf(buff, "T( %.2e ) = %Le\n", r, temperature(r,s));
	return buff;
}

char* outstr_lofm(double m, char* buff) {
	Star s = {0}; s.m = m;
	sprintf(buff, "L( %.2e ) = %Le\n", m, luminosity(radius(&s),&s));
	return buff;
}

char* outstr_dldr(double r, Star* s, char* buff, long double ltot) {
	sprintf(buff, "dldr( %.2e ) = %Le\n", r, normalized_dldr(r, s, ltot));
	return buff;
}

void write_tofr(Star* s, int nout, FILE* fout) {
	char strout[512];
	for (double r = 0; r <= radius(s); r += radius(s) / ((double) nout))
		fprintf(fout, "%s", outstr_temperature(r,s,strout));	
}

void write_lofm(double mmin, double mmax, int nout, FILE* fout) {
	char strout[512];
	for (double m = mmin; m <= mmax; m += (mmax-mmin) / ((double) nout))
		fprintf(fout, "%s", outstr_lofm(m,strout));
}

void write_dldr(Star* s, int nout, FILE* fout, long double ltot) {
	char strout[512];
	for (double r = 0; r <= radius(s); r += radius(s) / ((double) nout))
		fprintf(fout, "%s", outstr_dldr(r,s,strout, ltot));
}

void out_tofr_plot(Star* s, char* filename, int n) {
	char fullname[512];
	if (n > 0) sprintf(fullname, "%s%d.txt", filename, n);
	else sprintf(fullname, "%s.txt", filename);

	FILE* of = fopen(fullname, "a");
	write_tofr(s, NOUT, of);
	fclose(of);
}

void out_tofr_all(char* filename, int n, ...) {
	char rmbuff[128]; sprintf(rmbuff, "rm -f %s*.txt", filename);
	system(rmbuff);

	va_list ptr;
	va_start(ptr, n);
	for (int i = 1; i <= n; i++)
		out_tofr_plot(va_arg(ptr, Star*), filename, i);
	va_end(ptr);
}

void out_lofm_plot(double mmin, double mmax, char* filename) {
	char rmbuff[128]; sprintf(rmbuff, "rm -f %s", filename);
	system(rmbuff);
	FILE* of = fopen(filename, "a");
	write_lofm(mmin, mmax, NOUT, of);
	fclose(of);
}

void out_normalized_dldr_plot(Star* s, char* filename, int n) {
	char fullname[512];
	if (n > 0) sprintf(fullname, "%s%d.txt", filename, n);
	else sprintf(fullname, "%s.txt", filename);

	FILE* of = fopen(fullname, "a");
	write_dldr(s, NOUT, of, luminosity(radius(s), s));
	fclose(of);
}

void out_dldr_all(char* filename, int n, ...) {
	char rmbuff[128]; sprintf(rmbuff, "rm -f %s*.txt", filename);
	system(rmbuff);

	va_list ptr;
	va_start(ptr, n);
	for (int i = 1; i <= n; i++)
		out_normalized_dldr_plot(va_arg(ptr, Star*), filename, i);
}
