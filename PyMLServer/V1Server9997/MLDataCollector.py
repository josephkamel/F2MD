import os
import json
import numpy as np
from numpy import array

class MlDataCollector:
	
	initCollection = False

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
		np.save(self.savePath+'/valuesSave_'+self.curDateStr, self.valuesCollection)
		np.save(self.savePath+'/targetSave_'+self.curDateStr, self.targetCollection)

	def loadData(self):
		self.valuesCollection = np.load(self.savePath+'/valuesSave_'+self.curDateStr +'.npy' )
		self.targetCollection = np.load(self.savePath+'/targetSave_'+self.curDateStr +'.npy')
		self.initCollection = True	

	def collectData(self,bsmArray):
		if not self.initCollection:
			self.initCollection = True
			self.valuesCollection = np.array([bsmArray[0]])
			self.targetCollection = bsmArray[1]
		else:
			self.valuesCollection = np.vstack([self.valuesCollection,bsmArray[0]])
			self.targetCollection = np.concatenate([self.targetCollection,bsmArray[1]])


