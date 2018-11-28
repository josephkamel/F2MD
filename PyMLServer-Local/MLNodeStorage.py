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
from MLArrayStorage import MlArrayStorage
import itertools
import operator

class MlNodeStorage:
	
	def __init__(self, *args, **kwargs):
		self.id_index = []
		self.id_storage = []
	
	def add_array(self,my_id, test_id, time, cur_array):
		index = 0
		try:
			index = self.id_index.index(my_id)
			self.id_storage[index].add_array(test_id, time, cur_array)

		except ValueError:
			self.id_index.append(my_id)
			Storage = MlArrayStorage()
			Storage.add_array(test_id, time, cur_array)
			self.id_storage.append(Storage)

	def get_array(self,my_id, test_id, batch_size):
		index = self.id_index.index(my_id)
		return self.id_storage[index].get_array(test_id, batch_size)

	def get_latest_time(self,my_id,test_id):
		try:
			index = self.id_index.index(my_id)
			return self.id_storage[index].get_latest_time(test_id)
		except ValueError:
			return 0

