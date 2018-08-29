import os
import json
import numpy as np
from numpy import array

class MlDataCollector:
	
	initCollection = False

	valuesCollection = np.array([])
	targetCollection = np.array([])

	curDateStr = ''
	def setCurDateSrt(self, datastr):
		self.curDateStr = datastr

	def saveData(self):
		np.save('saveNumpyFile/valuesSave_'+self.curDateStr, self.valuesCollection)
		np.save('saveNumpyFile/targerSave_'+self.curDateStr, self.targetCollection)

	def collectData(self,bsmArray):
		if not self.initCollection:
			self.initCollection = True
			self.valuesCollection = np.array([bsmArray[0]])
			self.targetCollection = bsmArray[1]
		else:
			self.valuesCollection = np.vstack([self.valuesCollection,bsmArray[0]])
			self.targetCollection = np.concatenate([self.targetCollection,bsmArray[1]])


