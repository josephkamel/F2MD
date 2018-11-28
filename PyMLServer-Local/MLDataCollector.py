import os
import json
import numpy as np
from numpy import array

class MlDataCollector:
	
	initCollection = False

	ValuesData = []
	TargetData = []

	valuesCollection = np.array([])
	targetCollection = np.array([])

	curDateStr = ''

	savePath = ''

	def setCurDateSrt(self, datastr):
		self.curDateStr = datastr

	def getValuesCollection(self):
		return self.valuesCollection

	def setSavePath(self, datastr):
		self.savePath = datastr

	def getTargetCollection(self):
		return self.targetCollection

	def saveData(self):
		self.initValuesData(self.ValuesData)
		self.initTargetData(self.TargetData)

		self.ValuesData = []
		self.TargetData = []

		np.save(self.savePath+'/valuesSave_'+self.curDateStr, self.valuesCollection)
		np.save(self.savePath+'/targetSave_'+self.curDateStr, self.targetCollection)

	def loadData(self):
		self.valuesCollection = np.load(self.savePath+'/valuesSave_'+self.curDateStr +'.npy' )
		self.targetCollection = np.load(self.savePath+'/targetSave_'+self.curDateStr +'.npy')
		#print(self.valuesCollection[0:2])
		#print(self.targetCollection[0:2])
		self.initCollection = True	

	def collectData(self,bsmArray):
		self.ValuesData.append(bsmArray[0])
		self.TargetData.append(bsmArray[1])

	def collectDataOld(self,bsmArray):
		if not self.initCollection:
			self.initCollection = True
			self.valuesCollection = np.array([bsmArray[0]])
			self.targetCollection = bsmArray[1]
		else:
			self.valuesCollection = np.vstack([self.valuesCollection,bsmArray[0]])
			self.targetCollection = np.concatenate([self.targetCollection,bsmArray[1]])

	def initValuesData(self,New_Rows):
		if self.valuesCollection.shape[0] == 0:
			self.valuesCollection = np.vstack([row for row in New_Rows])
		else:
			self.valuesCollection = np.vstack([self.valuesCollection, [row for row in New_Rows]])

	def initTargetData(self,New_Rows):
		if self.targetCollection.shape[0] == 0:
			self.targetCollection = np.concatenate([row for row in New_Rows])
		else:
			addTargetCollection = np.concatenate([row for row in New_Rows])
			self.targetCollection  = np.concatenate([self.targetCollection, addTargetCollection])
		
