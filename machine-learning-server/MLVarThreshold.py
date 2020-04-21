"""
/*******************************************************************************
 * @author  Joseph Kamel
 * @email   josephekamel@gmail.com
 * @date	28/11/2018
 * @version 2.0
 *
 * SCA (Secure Cooperative Autonomous systems)
 * Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
 * All rights reserved.
 *******************************************************************************/
"""

import numpy as np
from MLStats import MlStats
import itertools
import operator


start_thre = 0.0
step_thre = 0.1
end_thre = 1.0

class MlVarThreshold:
	def __init__(self, *args, **kwargs):
		self.step = []
		self.stats = []
		floatList = self.frange(start_thre,end_thre, step_thre)
		for i in floatList:
			self.step.append(float(i))
			self.stats.append(MlStats())

	def update_stats(self,PredVal,label_index):
		for i in range(0, len(self.step)):
			if(PredVal>=self.step[i]):
				self.stats[i].update_stats(True,label_index)
			else:
				self.stats[i].update_stats(False,label_index)

	def print_stats(self):
		print("MlVarThreshold:")
		print(self.get_stats())

	def get_stats(self):
		res = ''
		for i in range(0, len(self.step)):
			res = res + "step " + str(self.step[i]) + ' '  + self.stats[i].get_stats() +  '\n'
		return res	


	def frange(self,start, stop=None, step=None):
		if stop == None:
			stop = start + 0.0
			start = 0.0
		if step == None:
			step = 1.0
		while True:
			if step > 0 and start >= stop:
				break
			elif step < 0 and start <= stop:
				break
			yield ("%g" % start) # return float number
			start = start + step

