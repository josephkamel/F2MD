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

import time

class WebGuiBuilder:
	backgroundColor=['rgba(255, 99, 132, 0.2)','rgba(54, 162, 235, 0.2)','rgba(255, 206, 86, 0.2)','rgba(75, 192, 192, 0.2)','rgba(153, 102, 255, 0.2)','rgba(255, 159, 64, 0.2)']
	borderColor=['rgba(255,99,132,1)','rgba(54, 162, 235, 1)','rgba(255, 206, 86, 1)','rgba(75, 192, 192, 1)','rgba(153, 102, 255, 1)','rgba(255, 159, 64, 1)']

	database = "{label: '#label#',data: #data#,backgroundColor: ['#bgcolor#'],borderColor: ['#bordercolor#'],}"

	cur_time_array = []
	trueDetection_cur = []
	falseDetection_cur = []
	detectionRate_cur = []

	DISPLAY_NODES = 10
	DISPLAY_NODES_DATASETS = 1000
	DISPLAY_STEPS = 40

	SAVE_TIME_STEP = 1000
	dt_save_time = 0
	dt_save_data = 0 

	def get_datasets(self,storage,attack_names):
		attacker_ids = storage.get_attacker_ids(self.DISPLAY_NODES_DATASETS)
		genuine_ids = storage.get_genuine_ids(self.DISPLAY_NODES_DATASETS)
		datasets=[]
		reaction_levels_attacks=[0,0,0,0,0]
		reaction_levels_genuine=[0,0,0,0,0]
		for i in range(0,len(genuine_ids)):
			id = genuine_ids[i]
			for j in range(storage.get_rlevel(id),storage.get_rlevel(id)+1):
				reaction_levels_genuine[j] = reaction_levels_genuine[j] + 1

		for i in range(0,len(attacker_ids)):
			id = attacker_ids[i]
			for j in range(storage.get_rlevel(id),storage.get_rlevel(id)+1):
				reaction_levels_attacks[j] = reaction_levels_attacks[j] + 1

		sum_gen = sum(reaction_levels_genuine)
		sum_att = sum(reaction_levels_attacks)

		if sum_gen > 0:
			reaction_levels_genuine[:] = [100*x / sum_gen for x in reaction_levels_genuine]
		if sum_att > 0:
			reaction_levels_attacks[:] = [100*x / sum_att for x in reaction_levels_attacks]

		str_label =  "Genuines"
		data_temp = self.database.replace("#label#",str_label)
		data_temp = data_temp.replace("#data#", str(reaction_levels_genuine))
		data_temp = data_temp.replace("#bgcolor#", self.backgroundColor[0%len(self.backgroundColor)])
		data_temp = data_temp.replace("#bordercolor#", self.borderColor[0%len(self.backgroundColor)])
		datasets.append(data_temp)


		str_label =  "Attackers"
		data_temp = self.database.replace("#label#",str_label)
		data_temp = data_temp.replace("#data#", str(reaction_levels_attacks))
		data_temp = data_temp.replace("#bgcolor#", self.backgroundColor[1%len(self.backgroundColor)])
		data_temp = data_temp.replace("#bordercolor#", self.borderColor[1%len(self.backgroundColor)])
		datasets.append(data_temp)

		return datasets

	def get_reports_labels(self,storage):
		relevent_ids = storage.get_attacker_ids(self.DISPLAY_NODES)
		labels=[]
		for i in range(0,len(relevent_ids)):
			id = relevent_ids[i]
			attack_str = storage.get_id_attack(id)
			str_label =  str(id) #+ " (" + attack_str + ")"
			labels.append(str_label)
		return labels

	def get_reports_data(self,storage):
		relevent_ids = storage.get_attacker_ids(self.DISPLAY_NODES)
		reports_num=[]
		for i in range(0,len(relevent_ids)):
			id = relevent_ids[i]
			reports_num.append(storage.get_reports_len(id))
		return reports_num

	def get_bgcolor(self):
		return_array = []
		for i in range(0,self.DISPLAY_NODES):
			return_array.append(self.backgroundColor[i%len(self.backgroundColor)])
		return return_array
		
	def get_bordercolor(self):
		return_array = []
		for i in range(0,self.DISPLAY_NODES):
			return_array.append(self.borderColor[i%len(self.borderColor)])
		return return_array

	def get_time_label(self,cur_time):
		if (self.millis()  - self.dt_save_time) > self.SAVE_TIME_STEP:
			self.cur_time_array.append(cur_time)
			self.dt_save_time = self.millis() 

		return_array = []
		if len(self.cur_time_array)<self.DISPLAY_STEPS:
			for tm in self.cur_time_array:
				time_str = str(time.strftime("%M:%S", time.gmtime(tm)))
				return_array.append(time_str)
		else:
			for tm in self.cur_time_array[-self.DISPLAY_STEPS:]:
				time_str = str(time.strftime("%M:%S", time.gmtime(tm)))
				return_array.append(time_str)
		return return_array

	def get_detection_info_data(self,td_cur,fd_cur,dr_cur):
		if (self.millis() - self.dt_save_data) > self.SAVE_TIME_STEP:
			self.trueDetection_cur.append(td_cur)
			self.falseDetection_cur.append(fd_cur)
			self.detectionRate_cur.append(dr_cur)
			self.dt_save_data = self.millis() 

		if len(self.trueDetection_cur)<self.DISPLAY_STEPS:
			return[self.trueDetection_cur,self.falseDetection_cur,self.detectionRate_cur]
		else:
			return[self.trueDetection_cur[-self.DISPLAY_STEPS:],self.falseDetection_cur[-self.DISPLAY_STEPS:],self.detectionRate_cur[-self.DISPLAY_STEPS:]]

	def get_bgcolor_time(self,col_index):
		return_array = []
		for i in range(0,self.DISPLAY_STEPS):
			return_array.append(self.backgroundColor[col_index%len(self.backgroundColor)])
		return return_array

	def get_bordercolor_time(self,col_index):
		return_array = []
		for i in range(0,self.DISPLAY_STEPS):
			return_array.append(self.borderColor[col_index%len(self.backgroundColor)])
		return return_array

	def millis(self):
		return int(round(time.time() * 1000))