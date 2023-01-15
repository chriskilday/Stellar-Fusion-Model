""" plot.py """

import math
import os
import matplotlib.pyplot as plt
from parse import *

tofr_filename = "tofr.txt"
dldr_filename = "dldr.txt"
lofm_filename = "lofm.txt"

def main():
	plot_tofr_all()
	plot_dldr_all()
	plot_lofm(read_yofx(lofm_filename))

def read_yofx(filename):
	infile = open(filename, 'r'); lines = infile.readlines(); infile.close()
	yofx = {}
	for line in lines:
		(Y,x,y) = parse("{}( {} ) = {}", line.strip())
		yofx[float(x)] = float(y)

	return yofx

def plot_yofx(yofx):
	x,y = [],[]
	for key in yofx.keys():
		x.append(key); y.append(yofx[key])
	return (y,x)

def plot_tofr_all():
	fig, ax = plt.subplots()
	for tofr_file in [file for file in os.listdir() if (file.startswith(header(tofr_filename)) and file.endswith(".txt"))]:
		(t,r) = plot_yofx(read_yofx(tofr_file))
		ax.plot(r, [ti / 1e6 for ti in t])

	ax.set(xlabel="Radius / R_sol", ylabel="Temperature (MK)", \
	       title="Temperature(Radius)")
	plt.legend(["2M_sol", "8M_sol", "5M_sol", "20M_sol", "15M_sol"], loc="upper right")
	plt.show()

def expected_luminosity(m):
	return 1.4 * m**3.5

def plot_lofm(lofm):
	fig, ax = plt.subplots()

	(l,m) = plot_yofx(lofm)
	ax.plot([math.log(mi) for mi in m], [math.log(li) for li in l])
	ax.plot([math.log(mi) for mi in m], [math.log(expected_luminosity(mi)) for mi in m], '--')

	ax.set(xlabel="Log( Mass / M_sol )", ylabel="Log (Luminosity / L_sol)", \
	       title="Luminosity(Mass)")
	plt.legend(["Model", "Expected"], loc="upper right")
	plt.show()

def plot_dldr_all():
	fig, ax = plt.subplots()
	for dldr_file in [file for file in os.listdir() if (file.startswith(header(dldr_filename)) and file.endswith(".txt"))]:
		(dl,r) = plot_yofx(read_yofx(dldr_file))
		ax.plot(r, dl)

	ax.set(xlabel="Radius / R_sol", ylabel="Power")
	plt.legend(["2M_sol", "5M_sol", "8M_sol", "15M_sol", "20M_sol"], loc="upper right")
	plt.show()

def header(filename):
	return filename[:-4]

main()
