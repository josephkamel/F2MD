import numpy as np
from numpy import array

import itertools
import operator

class MlArrayStorage:
	id_index = []
	id_time = []
	id_array_x = []
	id_array_y = []

	def add_array(self, id, time, cur_array):
		index = 0
		try:
			index = self.id_index.index(id)
			self.id_time[index] = time
			self.id_array_x[index].append(cur_array[0])
			self.id_array_y[index].append(cur_array[1])

		except ValueError:
			self.id_index.append(id)
			self.id_time.append(time)
			append_array_x = []
			append_array_x.append(cur_array[0])
			self.id_array_x.append(append_array_x)
			append_array_y = []
			append_array_y.append(cur_array[1])
			self.id_array_y.append(append_array_y)

	def get_array(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = self.id_array_x[index][-batch_size:]
		ret_array = [sum(x) for x in zip(*list_X)]

		list_Y = self.id_array_y[index][-batch_size:]
		return array([array(ret_array),list_Y[0]])

	def get_time(self,id): 
		try:
			index = self.id_index.index(id)
			return self.id_time[index]
		except ValueError:
			return 0

