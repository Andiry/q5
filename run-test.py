#!/usr/bin/python
#coding=utf-8

import time
import os
import sys

def do_work(num, graph_type, filename):
	array = range(num)
	array = array[1:]
	for j in array:
		i = 0
		while i < 10:
			os.system('./q5 %s %s %s' %(str(array[j - 1]), graph_type, filename))
			i += 1

def main():
	date = time.strftime("%Y-%m-%d_%X", time.localtime(time.time()))
	filename = "result_" + date + ".csv"
	print filename
	f = open(filename, 'w')
	f.write("Num_nodes, Graph type, time(us)\n")
	file.close(f)

	print "Strating..."
	print "Running random graph with edge probability 50%.."
	do_work(13, '0', filename)
	print "Running random sparse graph.."
	do_work(13, '1', filename)
	print "Running bipartite graph.."
	do_work(13, '2', filename)

main()
