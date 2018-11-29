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
from numpy import array

import itertools
import operator

class MlArrayStorage:

	def __init__(self, *args, **kwargs):
		self.id_index = []
		self.id_time = []
		self.id_array_x = []
		self.id_array_y = []

	def add_array(self, id, time, cur_array):
		index = 0
		try:
			index = self.id_index.index(id)
			self.id_time[index].append(time)
			self.id_array_x[index].append(cur_array[0])
			self.id_array_y[index].append(cur_array[1])
			self.bubblesort(index)

		except ValueError:
			self.id_index.append(id)
			append_time = []
			append_time.append(time)
			self.id_time.append(append_time)
			append_array_x = []
			append_array_x.append(cur_array[0])
			self.id_array_x.append(append_array_x)
			append_array_y = []
			append_array_y.append(cur_array[1])
			self.id_array_y.append(append_array_y)

		#print  'local: ' + str(index) + ' ' + str(id) + ' ' + str(len(self.id_index))
		#print  'id_index: ' + str(id_index.shape)

	def bubblesort(self, index):
		for iter_num in range(len(self.id_time[index])-1,0,-1):
			for idx in range(iter_num):
				if self.id_time[index][idx]>self.id_time[index][idx+1]:
					temp = self.id_time[index][idx]
					self.id_time[index][idx] = self.id_time[index][idx+1]
					self.id_time[index][idx+1] = temp

					temp2 = self.id_array_x[index][idx]
					self.id_array_x[index][idx] = self.id_array_x[index][idx+1]
					self.id_array_x[index][idx+1] = temp2

					temp3 = self.id_array_y[index][idx]
					self.id_array_y[index][idx] = self.id_array_y[index][idx+1]
					self.id_array_y[index][idx+1] = temp3


	def get_array(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = self.id_array_x[index][-batch_size:]

		app_ary = array([[len(list_X)]])
		for i in range(0,len(list_X)-1):
			app_ary = np.append(app_ary.tolist(),[[len(list_X)]],axis=0)
		list_X = np.append(list_X,app_ary,axis=1)
		
		ret_array = [sum(x) for x in zip(*list_X)]
		list_Y = self.id_array_y[index][-batch_size:]
		return array([array(ret_array),list_Y[-1]])

	def get_array_lstm(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = self.id_array_x[index][-batch_size:]

		if len(list_X)<batch_size:
			list_X_Ret = [0,0,0,0,0,0,0,0,0,0]
			for i in range(0,batch_size-len(list_X) - 1):
				list_X_Ret = np.vstack((list_X_Ret,[0,0,0,0,0,0,0,0,0,0]))
			#print list_X_Ret
			list_X_Ret = np.vstack((list_X_Ret,list_X))
			list_X = list_X_Ret

		#nx , ny = np.array(list_X).shape
		#list_X = np.array(list_X).reshape((nx*ny))

		list_Y = self.id_array_y[index][-batch_size:]
		ret_array = array([list_X,list_Y[-1]])
		#print ret_array
		return ret_array

	def get_latest_time(self,id): 
		try:
			index = self.id_index.index(id)
			return self.id_time[index][-1]
		except ValueError:
			return 0
