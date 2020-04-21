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

import os
import sys
from os import listdir
from os.path import isfile, join
import json
import datetime
import warnings
from MAStorage import MaStorage
from MAStats import MaStats
from WebGUIBuilder import WebGuiBuilder
import time

UPDATEINT = 1

class MaMain:

	curDateStr = datetime.datetime.now().strftime("%Y-%m-%d_%H:%M:%S")

	storage = MaStorage()
	stats = MaStats()

	cur_time = 0

	reports_num = 5

	httpstring = ""
	updatehttp = 0

	attack_names = ["GlobalAttacker","LocalAttacker", "Genuine"]
	reaction_names = ["NoReaction", "Warning", "Ticket", "PassiveRev", "ActiveRev"]

	gui = WebGuiBuilder()

	newreport = False

	def pingma(self):
		self.set_httpstring()
		if self.newreport:
			self.newreport = False
		print('Attack : ' + str(self.storage.get_mean_num_attacks()) + ' ' + str(self.storage.get_sd_num_attacks()))
		print('Genuine : ' +   str(self.storage.get_mean_num_genuine()) + ' ' + str(self.storage.get_sd_num_genuine()))

	def maMain(self,version,reportJsonString):
		self.newreport = True

		reportJsom = json.loads(reportJsonString)

		reportedId = reportJsom['Report']['Metadata']['reportedId']
		generation_time = reportJsom['Report']['Metadata']['generationTime']
		realAttack = reportJsom['Report']['Metadata']['mbType']

		self.cur_time = generation_time
		self.storage.add_report(reportedId,generation_time,realAttack)

		reportsNum = self.storage.get_reports_len(reportedId)

		predicted = ''
		if reportsNum > self.reports_num:
			#print 'hello world'
			predicted = 'LocalAttacker'
		else:
			predicted = 'Genuine'

		self.storage.set_rlevel(reportsNum)

		self.storage.add_mbType_pred(reportedId,predicted)

		predicted_agr = self.storage.get_arrg_pred(reportedId, 20)

		self.stats.update_detection(predicted,realAttack)
		self.stats.update_detection_agr(predicted_agr,realAttack)

		curTime = self.gui.millis()
		#curTime = generation_time * 1000
		'''
		if (curTime - self.updatehttp) > UPDATEINT:
			self.updatehttp = curTime
			self.set_httpstring()
			print('Attack : ' + str(self.storage.get_mean_num_attacks()) + ' ' + str(self.storage.get_sd_num_attacks()))
			print('Genuine : ' +   str(self.storage.get_mean_num_genuine()) + ' ' + str(self.storage.get_sd_num_genuine()))
		'''
		return "False"


	def get_datasets(self):
		datasets = self.gui.get_datasets(self.storage,self.attack_names)
		datasets_str = ""
		for dataset in datasets:
			datasets_str = datasets_str + dataset + ","
		return datasets_str

	def get_reports_data(self):
		return self.gui.get_reports_data(self.storage)

	def get_reports_labels(self):
		return self.gui.get_reports_labels(self.storage)

	def get_time_labels(self):
		return self.gui.get_time_label(self.cur_time)

	def get_time_info(self):
		info_inst = self.stats.get_detectionInfo_inst()
		return self.gui.get_detection_info_data(info_inst[0],-info_inst[1],info_inst[2])

	def get_httpstring(self):
		#self.set_httpstring()
		return self.httpstring


	def set_httpstring(self,):
		string = open("WebGUI/index.html", "r").read()
		string = string.replace("#labelsTime#", str(self.get_time_labels()))
		_info_inst = self.get_time_info()

		if self.newreport:
			string = string.replace("#NewReport#", str("New Report Received!"))
			string = string.replace("#LineColor#", str('rgba(255, 99, 132,0.8)'))
			string = string.replace("#Line1#", str("New Report Received!"))
			string = string.replace("#FFFFFF", str("#ff0000"))
		else:
			string = string.replace("#NewReport#", str("Waiting For Reports..."))
			string = string.replace("#LineColor#", str('rgba(70,70,70,0.8)'))
			string = string.replace("#Line1#", str("Waiting For Reports..."))
			string = string.replace("#FFFFFF", str("#0000ff"))

		string = string.replace("#Line2#", "Cumulative Reports Number: " + str(self.storage.report_num))
		string = string.replace("#RepNum#", str(self.storage.report_num))
		string = string.replace("#data_acc#", str(_info_inst[0]))
		string = string.replace("#data_nacc#", str(_info_inst[1]))
		string = string.replace("#data_rate#", str(_info_inst[2]))
		string = string.replace("#bg_colors_acc#", str(self.gui.get_bgcolor_time(1)))
		string = string.replace("#border_colors_acc#", str(self.gui.get_bordercolor_time(1)))
		string = string.replace("#bg_colors_nacc#", str(self.gui.get_bgcolor_time(0)))
		string = string.replace("#border_colors_nacc#", str(self.gui.get_bordercolor_time(0)))
		string = string.replace("#bg_colors_rate#", str(self.gui.get_bgcolor_time(3)))
		string = string.replace("#border_colors_rate#", str(self.gui.get_bordercolor_time(3)))
		string = string.replace("#labels1#", str(self.get_reports_labels()))
		string = string.replace("#data1#", str(self.get_reports_data()))
		string = string.replace("#bgcolor1#", str(self.gui.get_bgcolor()))
		string = string.replace("#bordercolor1#", str(self.gui.get_bordercolor()))

		string = string.replace("#line3#", "Cumulative Detection Rate: "+str(self.stats.get_detectionRate())+"%")
		#string = string.replace("#test2#", "("+str(self.stats.get_detectionRateAgr())+"%)")
		string = string.replace("#test2#", "")
		string = string.replace("#labels2#", str(self.reaction_names))
		string = string.replace("#datasets2#", self.get_datasets())
		self.httpstring = string
		#print('set_httpstring UPDATED')









