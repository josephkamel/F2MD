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

import math
import itertools
import operator

class MaStorage:
	id_index = []
	id_time = []
	id_mbType = []
	id_mbType_pred = []

	reaction_level = []

	report_num = 0

	def set_rlevel(self,reports_num):	
		reaction_level = 0
		if reports_num > 20:
			reaction_level = 1
		if reports_num > 50:
			reaction_level = 2
		if reports_num > 250:
			reaction_level = 3
		if reports_num > 1250:
			reaction_level = 4
		return reaction_level

	def get_rlevel(self, id):
		try:
			index = self.id_index.index(id)
			return self.reaction_level[index]
		except ValueError:
			return 0

	def add_report(self, id, time, mbType):
		index = 0
		self.report_num = self.report_num + 1
		try:
			index = self.id_index.index(id)
			self.id_time[index] = time
			self.id_mbType[index].append(mbType)
			self.reaction_level[index] = self.set_rlevel(len(self.id_mbType_pred[index]))

		except ValueError:
			self.id_index.append(id)
			self.id_time.append(time)
			append_mbType = []
			append_mbType.append(mbType)
			self.id_mbType.append(append_mbType)
			append_mbType_pred = []
			self.id_mbType_pred.append(append_mbType_pred)
			self.reaction_level.append(self.set_rlevel(1))

	def add_mbType_pred(self,id,mbType_pred): 
		try:
			index = self.id_index.index(id)
			self.id_mbType_pred[index].append(mbType_pred)
		except ValueError:
			return 0

	def get_time(self,id): 
		try:
			index = self.id_index.index(id)
			return self.id_time[index]
		except ValueError:
			return 0

	def get_arrg_pred(self, id, depth):
		try:
			index = self.id_index.index(id)
			if depth>len(self.id_mbType_pred[index]):
				return self.most_common(self.id_mbType_pred[index])
			else:
				return self.most_common(self.id_mbType_pred[index][-depth:])
		except ValueError:
			return 'Genuine'

	def most_common(self, L):
		# get an iterable of (item, iterable) pairs
		SL = sorted((x, i) for i, x in enumerate(L))
		# print('SL:', SL)
		groups = itertools.groupby(SL, key=operator.itemgetter(0))
		# auxiliary function to get "quality" for an item
		def _auxfun(g):
			item, iterable = g
			count = 0
			min_index = len(L)
			for _, where in iterable:
				count += 1
				min_index = min(min_index, where)
			# print('item %r, count %r, minind %r' % (item, count, min_index))
			return count, -min_index
		# pick the highest-count/earliest item
		return max(groups, key=_auxfun)[0]

	def get_relevent_ids(self,reports_num,ids_num):
		relevent_ids = []
		for i in range(0,len(self.id_index)):
			if len(self.id_mbType[i])>reports_num:
				relevent_ids.append(self.id_index[i])
		if len(relevent_ids)<ids_num:
			return relevent_ids
		else:
			return relevent_ids[-ids_num:]

	def get_attacker_ids(self,ids_num):
		attacker_ids = []
		for i in range(0,len(self.id_index)):
			if self.id_mbType[i][-1]!='Genuine':
				attacker_ids.append(self.id_index[i])
		if len(attacker_ids)<ids_num:
			return attacker_ids
		else:
			return attacker_ids[-ids_num:]

	def get_genuine_ids(self,ids_num):
		gen_ids = []
		for i in range(0,len(self.id_index)):
			if self.id_mbType[i][-1]=='Genuine':
				gen_ids.append(self.id_index[i])
		if len(gen_ids)<ids_num:
			return gen_ids
		else:
			return gen_ids[-ids_num:]

	def get_mean_num_attacks(self):
		attacker_ids = []
		mean = 0
		for i in range(0,len(self.id_index)):
			if self.id_mbType[i][-1]!='Genuine':
				attacker_ids.append(i)

		total_number = 0
		for id_att in attacker_ids:
			total_number = total_number + len(self.id_mbType[id_att])

		if total_number > 0:
			mean = total_number / float(len(attacker_ids))
		return mean

	def get_mean_num_genuine(self):
		gen_ids = []
		mean = 0
		for i in range(0,len(self.id_index)):
			if self.id_mbType[i][-1]=='Genuine':
				gen_ids.append(i)
		total_number = 0
		for id_gen in gen_ids:
			total_number = total_number + len(self.id_mbType[id_gen])

		if total_number>len(gen_ids):
			mean = total_number / float(len(gen_ids))
		return mean

	def get_sd_num_attacks(self):
		attacker_ids = []
		sd = 0
		for i in range(0,len(self.id_index)):
			if self.id_mbType[i][-1]!='Genuine':
				attacker_ids.append(i)
		mean_att = self.get_mean_num_attacks()
		sd_sum = 0
		for id_att in attacker_ids:
			sd_sum = sd_sum + ((len(self.id_mbType[id_att]) - mean_att)**2)
		if len(attacker_ids)>0:
			sd = math.sqrt(sd_sum/float(len(attacker_ids)))
		return sd

	def get_sd_num_genuine(self):
		gen_ids = []
		sd = 0
		for i in range(0,len(self.id_index)):
			if self.id_mbType[i][-1]=='Genuine':
				gen_ids.append(i)
		mean_gen = self.get_mean_num_genuine()
		sd_sum = 0
		for id_gen in gen_ids:
			sd_sum = sd_sum + ((len(self.id_mbType[id_gen]) - mean_gen)**2)
		if len(gen_ids)>0:
			sd = math.sqrt(sd_sum/len(gen_ids))
		return sd

	def get_all_ids(self):
		return self.id_index

	def get_id_attack(self,id):
		index = self.id_index.index(id)
		return self.id_mbType[index][-1]

	def get_pred_array(self,id,attack_names):
		pred_array = []
		index = self.id_index.index(id)
		pred_sum = 0
		for i in range(0,len(attack_names)):
			cnt_pred = self.id_mbType_pred[index].count(attack_names[i])
			pred_sum = pred_sum + cnt_pred
			pred_array.append(cnt_pred)
		new_pred_array = [100* x / pred_sum for x in pred_array]
		return new_pred_array

	def get_reports_len(self,id):
		try:
			index = self.id_index.index(id)
			return len(self.id_mbType[index])
		except ValueError:
			return 0


