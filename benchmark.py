#!/usr/bin/env python2.7

import commands
import os

# check for executables, if not there then make them
if not os.path.exists('freq') or not os.path.exists('measure'):
	os.system("make")

# initialize variables and commands to simplify program
items = "1"
loads = ['0.50', '0.75', '0.90', '1.00', '2.00', '4.00', '8.00', '16.0']

# initialize command to simplify program
command = "shuf -i1-100000000 -n {} | ./measure ./freq -l {} > /dev/null"
output = "|{:>11} |{:>11} |{:>11} |{:>11} |"

# automatic testing - create markdown table
print "|   NITEMS   |   ALPHA    |    TIME    |   SPACE    |"
print "|------------|------------|------------|------------|"
for i in xrange(8):
	for l in loads:
		status, outputFreq = commands.getstatusoutput(command.format(items, l))
		time = "{:.2f}".format(round(float(outputFreq.split(' ')[0]), 2))
		space = "{:.2f}".format(round(float(outputFreq.split('\t')[1].split(' ')[0]), 2))
		print output.format(items, l, time, space)
	items += "0"
