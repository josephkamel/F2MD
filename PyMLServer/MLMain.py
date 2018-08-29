import os
import json
import numpy as np
from MLDataCollector import MlDataCollector
from MLTrainer import MlTrainer
from numpy import array
import datetime
class MlMain:

	curDateStr = datetime.datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
	DataCollector = MlDataCollector()
	Trainer = MlTrainer()
	collectDur = 0

	clf = None
	trainPath = './saveTrainFile'

	def init(self):
		self.DataCollector.setCurDateSrt(self.curDateStr)
		self.Trainer.setCurDateSrt(self.curDateStr)
		self.Trainer.setTrainPath(self.trainPath)

		self.trainedModelExists()

	def mlMain(self, bsmJsonString):
		bsmJsom = json.loads(bsmJsonString)
		curArray = self.getArray(bsmJsom)

		if self.collectDur < 10000:
			self.collectDur = self.collectDur + 1;
			self.DataCollector.collectData(curArray)
		else :
			print "DataCollection And Training Init"
			self.collectDur = 0
			self.DataCollector.saveData()
			self.Trainer.valuesCollection = self.DataCollector.valuesCollection
			self.Trainer.valuesCollection = self.DataCollector.valuesCollection
			self.Trainer.train()
			self.clf = joblib.load(trainPath+'/clf_'+curDateStr+'.pkl')

		if self.clf is None:
			return False
		else:
			prediction = self.clf.predict(curArray[0])
			print "========================================"
			print predition
			print curArray[1]
			print "========================================"

		return False


	def trainedModelExists(self):
		filesNames = [f for f in listdir(self.trainPath) if isfile(join(self.trainPath, f))]
		for s in filesNames:
			if s.endswith(".pkl"):
				self.clf = joblib.load(trainPath+'/'+s)

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
		
		valuesArray = array([rP,pP,sP,pC,sC,psC,phC,sA,bF,inT,time])
		targetArray = array([numLabel])
		returnArray = array([valuesArray,targetArray])
		#returnArray = returnArray.astype(np.float)
		return returnArray