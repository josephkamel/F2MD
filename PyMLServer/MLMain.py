import os
from os import listdir
from os.path import isfile, join
import json
import numpy as np
from MLDataCollector import MlDataCollector
from MLArrayStorage import MlArrayStorage
from MLTrainer import MlTrainer
from numpy import array
import datetime
from tqdm import tqdm
from sklearn import datasets
from sklearn.externals import joblib

import time

RTtrain = False
RTcollectData = False
RTreadDataFromFile = False
RTpredict = True

class MlMain:
	initiated = False

	curDateStr = datetime.datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
	DataCollector = MlDataCollector()
	Trainer = MlTrainer()
	Storage = MlArrayStorage()
	arrayLength = 20

	collectDur = 0
	deltaCall = 500000

	clf = None
	savePath = './saveFile/saveFile_Mix_D20_3L15N'
	dataPath = './MDBsms_Mix'

	meanRuntime = 0
	numRuntime = 0
	printRuntime = 10000
	printRuntimeCnt = 0

	def init(self, version, AIType):
		self.savePath = self.savePath +'_'+ str(version)

		self.DataCollector.setCurDateSrt(self.curDateStr)
		self.DataCollector.setSavePath(self.savePath)
		self.Trainer.setCurDateSrt(self.curDateStr)
		self.Trainer.setSavePath(self.savePath)
		self.Trainer.setAIType(AIType)

		self.trainedModelExists(AIType)
		if RTreadDataFromFile:
			self.ReadDataFromFile(version, AIType)

	def mlMain(self, version, bsmJsonString, AIType):
		if not self.initiated:
			self.init(version,AIType)
			self.initiated = True


		start_time = time.time()

		bsmJsom = json.loads(bsmJsonString)
		curArray = self.getNodeArray(bsmJsom)

		if RTcollectData:
			if self.collectDur < self.deltaCall:
				self.collectDur = self.collectDur + 1;
				self.DataCollector.collectData(curArray)
			else :
				print "DataSave And Training " + str(self.deltaCall) + " Started ..."
				self.collectDur = 0
				self.DataCollector.saveData()

				if RTtrain:
					self.Trainer.setValuesCollection(self.DataCollector.getValuesCollection())
					self.Trainer.setTargetCollection(self.DataCollector.getTargetCollection())
					print self.Trainer.valuesCollection.shape
					self.Trainer.train()
					self.clf = joblib.load(self.savePath+'/clf_'+AIType+'_'+self.curDateStr+'.pkl')
					self.deltaCall = self.DataCollector.valuesCollection.shape[0]/5
				print "DataSave And Training " + str(self.deltaCall) +" Finished!"
		

		return_value = False

		if self.clf is None:
			return_value = False
		else:
			if RTpredict:
				prediction = self.clf.predict(array([curArray[0]]))
				#print "========================================"
				if prediction[0] == 0.0:
					return_value = False
				else:
					return_value = True
			#print prediction
			#print curArray[1]
			#print "========================================"

		end_time = time.time()
		self.meanRuntime = (self.numRuntime*self.meanRuntime + (end_time-start_time))/(self.numRuntime+1)
		self.numRuntime = self.numRuntime + 1
		if self.printRuntimeCnt > self.printRuntime:
			self.printRuntimeCnt = 0
			print 'meanRuntime: ' + str(self.meanRuntime) + ' ' + str(self.numRuntime)
		else:
			self.printRuntimeCnt = self.printRuntimeCnt + 1



		return return_value


	def trainedModelExists(self, AIType):
		filesNames = [f for f in listdir(self.savePath) if isfile(join(self.savePath, f))]
		print "trainedModelExists?"

		for s in filesNames:
			if s.startswith('clf_'+AIType) and s.endswith(".pkl"):
				self.curDateStr = s[-23:-4]

				print "Loading " +AIType + " "+ self.curDateStr+ " ..."
				self.clf = joblib.load(self.savePath+'/'+s)
				self.DataCollector.setCurDateSrt(self.curDateStr)
				self.Trainer.setCurDateSrt(self.curDateStr)
				self.DataCollector.loadData()
				self.Trainer.setValuesCollection(self.DataCollector.getValuesCollection())
				self.Trainer.setTargetCollection(self.DataCollector.getTargetCollection())
 
				#self.deltaCall = self.DataCollector.valuesCollection.shape[0]/5
				print "Loading " + str(self.DataCollector.valuesCollection.shape) +  " Finished!"

	def ReadDataFromFile(self, version, AIType):
		print "DataSave And Training " + str(self.dataPath+'_'+version) + " Started ..."
		filesNames = [f for f in tqdm(listdir(self.dataPath+'_'+version)) if isfile(join(self.dataPath+'_'+version, f))]
		print "bsmDataExists?"

		ValuesData = []
		TargetData = []

		for i in tqdm(range(0,len(filesNames))):
			s = filesNames[i]
			if s.endswith(".bsm"):
				bsmJsonString = open(self.dataPath+'_'+version+'/' +s, 'r').read()
				bsmJsom = json.loads(bsmJsonString)
				curArray = self.getNodeArray(bsmJsom)
				self.DataCollector.collectData(curArray)

		self.DataCollector.saveData()
		self.Trainer.setValuesCollection(self.DataCollector.getValuesCollection())
		self.Trainer.setTargetCollection(self.DataCollector.getTargetCollection())
		self.Trainer.train()
		self.clf = joblib.load(self.savePath+'/clf_'+AIType+'_'+self.curDateStr+'.pkl')
		#self.deltaCall = self.DataCollector.valuesCollection.shape[0]/5
		print "DataSave And Training " + str(self.dataPath+'_'+version) + " Finished!"

	def getNodeArray(self,bsmJsom):
		cur_array = self.getArray(bsmJsom)
		pseudonym = bsmJsom['BsmPrint']['BSMs'][0]['pseudonym'] 
		time = bsmJsom['BsmPrint']['Metadata']['generationTime']
		self.Storage.add_array(pseudonym, time, cur_array)
		returnArray = self.Storage.get_array(pseudonym, self.arrayLength)

		#print "cur_array: " + str(cur_array)
		#print "returnArray: " + str(returnArray)
		return returnArray

	def getArray(self,bsmJsom):
		rP = bsmJsom['BsmPrint']['BsmCheck']['rP']
		pP = bsmJsom['BsmPrint']['BsmCheck']['pP']
		sP = bsmJsom['BsmPrint']['BsmCheck']['sP']
		pC = bsmJsom['BsmPrint']['BsmCheck']['pC']
		sC = bsmJsom['BsmPrint']['BsmCheck']['sC']
		psC = bsmJsom['BsmPrint']['BsmCheck']['psC']
		phC = bsmJsom['BsmPrint']['BsmCheck']['phC']
		sA = bsmJsom['BsmPrint']['BsmCheck']['sA']
		#sA = 1
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
		
<<<<<<< HEAD
		#hack
		if pP<1.0:
			pP = pP - 0
			if pP < 0:
				pP = 0
		valuesArray = array([rP,pP,sP,pC,sC,psC,phC,sA,bF,inT])
=======
		#valuesArray = array([rP,pP,sP,pC,sC,psC,phC,sA,bF,inT])
		valuesArray = array([1-rP,1-pP,1-sP,1-pC,1-sC,1-psC,1-phC,1-sA,1-bF,1-inT,1])
>>>>>>> 8dde4dbbb7e676bd61cd562c0fc51b3a2a3366f5
		targetArray = array([numLabel])
		returnArray = array([valuesArray,targetArray])

		#print "returnArray: " + str(returnArray)
		#returnArray = returnArray.astype(np.float)
		return returnArray
