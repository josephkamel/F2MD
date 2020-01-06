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

import os
import json
import numpy as np
from tqdm import tqdm
import pickle

class MlDataCollector:
	def __init__(self, *args, **kwargs):
		self.initValuesData = False
		self.ValuesData = []
		self.TargetData = []

		self.curDateStr = ''

		self.savePath = ''

	def setCurDateSrt(self, datastr):
		self.curDateStr = datastr

	def setSavePath(self, datastr):
		self.savePath = datastr
	
	def saveData(self, it_num):
		with open(self.savePath+'/valuesSave_'+self.curDateStr+'.listpkl', 'wb') as fp:
			pickle.dump(self.ValuesData, fp)
		with open(self.savePath+'/targetSave_'+self.curDateStr +'.listpkl', 'wb') as ft:
			pickle.dump(self.TargetData, ft)

	def loadData(self):
		with open (self.savePath+'/valuesSave_'+self.curDateStr+'.listpkl', 'rb') as fp:
			self.ValuesData = pickle.load(fp)
		with open (self.savePath+'/targetSave_'+self.curDateStr +'.listpkl', 'rb') as ft:
			self.TargetData = pickle.load(ft)


	def prepare_arrays(self):
		if isinstance(self.ValuesData[0], list):
			for i in range(0,len(self.ValuesData)):
				self.ValuesData[i] = np.array(self.ValuesData[i])
		else:
			self.ValuesData = np.array(self.ValuesData)
		self.TargetData = np.array(self.TargetData)

        

	def collectData(self,bsmArray):
		if not self.initValuesData:
			self.initValuesData = True
			if isinstance(bsmArray[0], list):
				for i in range(0,len(bsmArray[0])):
					self.ValuesData.append([])

		if isinstance(bsmArray[0], list):
			for i in range(0,len(bsmArray[0])):
				self.ValuesData[i].append(bsmArray[0][i])
		else:
			self.ValuesData.append(bsmArray[0])
		self.TargetData.append(bsmArray[1])

