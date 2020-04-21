"""
/*******************************************************************************
 * @author  Joseph Kamel
 * @email   josephekamel@gmail.com
 * @date    28/11/2018
 * @version 2.0
 *
 * SCA (Secure Cooperative Autonomous systems)
 * Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
 * All rights reserved.
 *******************************************************************************/
"""

import numpy as np
import itertools
import operator
import math

class MlStats:
	
	def __init__(self, *args, **kwargs):
		self.TP = 0.0
		self.TN = 0.0
		self.FP = 0.0
		self.FN = 0.0

	def update_stats(self,PredTrue,label_index):
		if PredTrue:
			if label_index == 1:
				self.TP = self.TP + 1.0
			else:
				self.FP = self.FP + 1.0
		else:
			if label_index == 1:
				self.FN = self.FN + 1.0
			else:
				self.TN = self.TN + 1.0

	def print_stats(self):
		print(self.get_stats())

	def get_stats(self):
		return ("TP:" + str(self.TP) + " TN:" + str(self.TN) + " FP:" + str(self.FP) + " FN:" + str(self.FN) + " MCC:" + str(self.get_MCC()) + " K:" + str(self.get_K()))


	def get_MCC(self):
		try:
			MCC = (self.TP*self.TN - self.FP*self.FN)/(math.sqrt((self.TP+self.FP)*(self.TP+self.FN)*(self.TN+self.FP)*(self.TN+self.FN)))
		except ZeroDivisionError as err:
			MCC = 0
		return MCC
	
	def get_K(self):
		try:
			K=(((self.TP+self.TN)/(self.TP+self.FP+self.FN+self.TN))-(((self.TP+self.FP)*(self.TP+self.TN)+(self.TN+self.FP)*(self.TN+self.FN))/((self.TN+self.TP+self.FP+self.FN)**2)))/(1-(((self.TP+self.FP)*(self.TP+self.TN)+(self.TN+self.FP)*(self.TN+self.FN))/((self.TN+self.TP+self.FP+self.FN)**2)))
		except ZeroDivisionError as err:
			K = 0
		return K
	