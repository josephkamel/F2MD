import os
from os import listdir
from os.path import isfile, join
import json
import numpy as np
from MLDataCollectorSVM import MlDataCollector
from MLTrainerSVM import MlTrainer
from numpy import array
import datetime
from sklearn import svm
from sklearn import datasets
from sklearn.externals import joblib
class MlMain:

	initiated = False

	curDateStr = datetime.datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
	DataCollector = MlDataCollector()
	Trainer = MlTrainer()
	collectDur = 0

	deltaCall = 1000

	clf = None
	savePath = './saveFileSVM'


	def init(self):
		self.DataCollector.setCurDateSrt(self.curDateStr)
		self.DataCollector.setSavePath(self.savePath)
		self.Trainer.setCurDateSrt(self.curDateStr)
		self.Trainer.setSavePath(self.savePath)

		self.trainedModelExists()

	def mlMain(self, bsmJsonString):
		if not self.initiated:
			self.init()
			self.initiated = True

		bsmJsom = json.loads(bsmJsonString)
		curArray = self.getArray(bsmJsom)

		if self.collectDur < self.deltaCall:
			self.collectDur = self.collectDur + 1;
			self.DataCollector.collectData(curArray)
		else :
			print "DataCollection And Training " + str(self.deltaCall) + " Started ..."

			self.collectDur = 0
			self.DataCollector.saveData()
			self.Trainer.setValuesCollection(self.DataCollector.getValuesCollection())
			self.Trainer.setTargetCollection(self.DataCollector.getTargetCollection())

			print self.Trainer.valuesCollection.shape

			self.Trainer.train()
			self.clf = joblib.load(self.savePath+'/clf_'+self.curDateStr+'.pkl')
			self.deltaCall = self.DataCollector.valuesCollection.shape[0]/2
			print "DataCollection And Training " + str(self.deltaCall) +" Finished!"

			

		if self.clf is None:
			return False
		else:
			prediction = self.clf.predict(array([curArray[0]]))
			#print "========================================"
			if prediction[0] == 0.0:
				return False
			else:
				return True
			#print prediction
			#print curArray[1]
			#print "========================================"

		return False


	def trainedModelExists(self):
		filesNames = [f for f in listdir(self.savePath) if isfile(join(self.savePath, f))]
		for s in filesNames:
			if s.startswith("clf_") and s.endswith(".pkl"):
				self.curDateStr = s[4:-4]

				print "Loading " + self.curDateStr+" ..."
				self.clf = joblib.load(self.savePath+'/'+s)
				self.DataCollector.setCurDateSrt(self.curDateStr)
				self.Trainer.setCurDateSrt(self.curDateStr)
				self.DataCollector.loadData()
				self.Trainer.setValuesCollection(self.DataCollector.getValuesCollection())
				self.Trainer.setTargetCollection(self.DataCollector.getTargetCollection())
 
				self.deltaCall = self.DataCollector.valuesCollection.shape[0]/2
				print "Loading " + str(self.DataCollector.valuesCollection.shape) +  " Finished!"

	def getArray(self,bsmJsom):
		rP = bsmJsom['BsmPrint']['BsmCheck']['rP']
		pP = bsmJsom['BsmPrint']['BsmCheck']['pP']
		sP = bsmJsom['BsmPrint']['BsmCheck']['sP']
		pC = bsmJsom['BsmPrint']['BsmCheck']['pC']
		sC = bsmJsom['BsmPrint']['BsmCheck']['sC']
		psC = bsmJsom['BsmPrint']['BsmCheck']['psC']
		phC = bsmJsom['BsmPrint']['BsmCheck']['phC']
		sA = bsmJsom['BsmPrint']['BsmCheck']['sA']
		bF = bsmJsom['BsmPrint']['BsmCheck']['bF']
		inT = 1
		for x in bsmJsom['BsmPrint']['BsmCheck']['inT']:
			if inT>x['uVal']:
				inT = x['uVal']

		time = bsmJsom['BsmPrint']['Metadata']['generationTime']
		label = bsmJsom['BsmPrint']['Metadata']['mbType']
		#label = 0
		if(label == 'Genuine'):
			numLabel = 0.0
		else:
			numLabel = 1.0
		
		valuesArray = array([rP,pP,sP,pC,sC,psC,phC,sA,bF,inT])
		targetArray = array([numLabel])
		returnArray = array([valuesArray,targetArray])
		#returnArray = returnArray.astype(np.float)
		return returnArray