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
from MLArrayStorage import MlArrayStorage
import itertools
import operator

class MlNodeStorage:
	
	def __init__(self, *args, **kwargs):
		self.id_index = []
		self.id_storage = []
	
	def add_bsm(self,my_id, test_id, time, cur_bsm, batch_size, label):
		index = 0
		if my_id in self.id_index:
			index = self.id_index.index(my_id)
			self.id_storage[index].add_bsm(test_id, time, cur_bsm, batch_size,label)
		else:
			self.id_index.append(my_id)
			Storage = MlArrayStorage()
			Storage.add_bsm(test_id, time, cur_bsm, batch_size,label)
			self.id_storage.append(Storage)

	def filter_bsms(self, curtime ,deltatime):
		for i in range(len(self.id_index)-1,-1,-1):
			self.id_storage[i].filter_array(curtime ,deltatime)
			if len(self.id_storage[i].id_index) == 0:
				del self.id_storage[i]
				del self.id_index[i]


	def get_array(self,my_id, test_id):
		index = self.id_index.index(my_id)
		return self.id_storage[index].get_array(test_id)
	def get_array_features(self,my_id, test_id):
		index = self.id_index.index(my_id)
		return self.id_storage[index].get_array_features(test_id)
	def get_array_MLP(self,my_id, test_id, batch_size):
		index = self.id_index.index(my_id)
		return self.id_storage[index].get_array_MLP(test_id, batch_size)
	def get_array_MLP_features(self,my_id, test_id, batch_size):
		index = self.id_index.index(my_id)
		return self.id_storage[index].get_array_MLP_features(test_id, batch_size)
	def get_array_lstm(self,my_id, test_id, batch_size):
		index = self.id_index.index(my_id)
		return self.id_storage[index].get_array_lstm(test_id, batch_size)
	def get_array_lstm_all(self,my_id, test_id, batch_size):
		index = self.id_index.index(my_id)
		return self.id_storage[index].get_array_lstm_all(test_id, batch_size)
	def get_array_lstm_feat(self,my_id, test_id, batch_size):
		index = self.id_index.index(my_id)
		return self.id_storage[index].get_array_lstm_feat(test_id, batch_size)
	def get_array_lstm_sin(self,my_id, test_id, batch_size):
		index = self.id_index.index(my_id)
		return self.id_storage[index].get_array_lstm_sin(test_id, batch_size)
	def get_array_lstm_mix(self,my_id, test_id, batch_size):
		index = self.id_index.index(my_id)
		return self.id_storage[index].get_array_lstm_mix(test_id, batch_size)
	def get_array_combined(self,my_id, test_id, batch_size):
		index = self.id_index.index(my_id)
		return self.id_storage[index].get_array_combined(test_id, batch_size)
    
    
    
    
    
    
