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
import itertools
import operator
import math

start_thre = 0.0
step_thre = 0.0001
end_thre = 1.0

class MlVarThresholdLite:
	def __init__(self, *args, **kwargs):
		self.step = []
		self.step_count_A = []
		self.step_count_G = []

		self.step_stats = []
		self.step_MCC = []
		self.step_K = []

		floatList = self.frange(start_thre,end_thre, step_thre)
		for i in floatList:
			self.step.append(float(i))
			self.step_count_A.append(0.0)
			self.step_count_G.append(0.0)
			self.step_stats.append([])
			self.step_MCC.append(0.0)
			self.step_K.append(0.0)

	def update_stats(self,PredVal,label_index):
		step_int = int((start_thre + PredVal)/step_thre)
		if label_index == 1:
			self.step_count_A[step_int] = self.step_count_A[step_int] + 1.0
		else:
			self.step_count_G[step_int] = self.step_count_G[step_int] + 1.0

	def collect(self,tempVerThre):
		for i in range(0,len(self.step)):
			self.step_count_A[i] = self.step_count_A[i] + tempVerThre.step_count_A[i]
			self.step_count_G[i] = self.step_count_G[i] + tempVerThre.step_count_G[i]

	def calculate_stats(self):
		cur_G = sum(self.step_count_G)
		cur_A = sum(self.step_count_A)
		all_G = sum(self.step_count_G)
		all_A = sum(self.step_count_A)

		for i in range(0,len(self.step)):
			if i == 0:
				cur_A_B = all_A - cur_A #FN
				cur_G_B = all_G - cur_G #TN
			else:
				cur_G = cur_G - self.step_count_G[i-1] #FP
				cur_A = cur_A - self.step_count_A[i-1] #TP

				cur_A_B = all_A - cur_A #FN
				cur_G_B = all_G - cur_G #TN

			self.step_stats[i] = [cur_A,cur_G,cur_G_B,cur_A_B]
			self.step_MCC[i] = self.get_MCC(cur_A,cur_G,cur_G_B,cur_A_B)
			self.step_K[i] = self.get_K(cur_A,cur_G,cur_G_B,cur_A_B)

	def get_MCC(self,TP,FP,TN,FN):
		try:
			MCC = (TP*TN - FP*FN)/(math.sqrt((TP+FP)*(TP+FN)*(TN+FP)*(TN+FN)))
		except ZeroDivisionError as err:
			MCC = 0
		return MCC
	
	def get_K(self,TP,FP,TN,FN):
		try:
			K=(((TP+TN)/(TP+FP+FN+TN))-(((TP+FP)*(TP+TN)+(TN+FP)*(TN+FN))/((TN+TP+FP+FN)**2)))/(1-(((TP+FP)*(TP+TN)+(TN+FP)*(TN+FN))/((TN+TP+FP+FN)**2)))
		except ZeroDivisionError as err:
			K = 0
		return K
	
	def print_stats(self):
		print("MlVarThresholdLite:")
		print(self.get_stats())

	def get_stats(self):
		self.calculate_stats()
		res = ''
		for i in range(0, len(self.step)):
			res = res + "step " + str(self.step[i]) + " TP:" + str(self.step_stats[i][0]) + " TN:" + str(self.step_stats[i][2]) + " FP:" + str(self.step_stats[i][1]) + " FN:" + str(self.step_stats[i][3]) + " MCC:" + str(self.step_MCC[i]) + " K:" +  str(self.step_K[i]) +  '\n'
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