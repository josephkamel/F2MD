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
from os import listdir
from os.path import isfile, join
import json
import numpy as np
from MLDataCollector import MlDataCollector
from MLNodeStorage import MlNodeStorage
from MLTrainer import MlTrainer
from numpy import array
import datetime
from tqdm import tqdm
from sklearn import datasets
from sklearn.externals import joblib

import time


RTtrain = True
RTcollectData = True

RTpredict = True

class MlMain:
	initiated = False

	curDateStr = datetime.datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
	DataCollector = MlDataCollector()
	Trainer = MlTrainer()
	Storage = MlNodeStorage()
	arrayLength = 40

	collectDur = 0
	deltaCall = 100000

	clf = None
	savePath = './saveFile/saveFile_D40'
	dataPath = '/home/joseph/Projects/F2MD/mdmSave/Data-1.0/IRT-DATA-0.5/MDBsmsList_V2_2019-3-4_1:1:23'
	RTTrainDataFromFile = False

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
		if self.RTTrainDataFromFile:
			self.ReadDataFromFile(AIType)
			self.TrainData(AIType)
			os._exit(0)

	def mlMain(self, version, bsmJsonString, AIType):
		if not self.initiated:
			self.init(version,AIType)
			self.initiated = True

		start_time = time.time()

		bsmJsom = json.loads(bsmJsonString)
		curArray = self.getNodeArray(bsmJsom,AIType)

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
					self.deltaCall = self.DataCollector.valuesCollection.shape[0]/2
					#self.deltaCall = 10000000
				print "DataSave And Training " + str(self.deltaCall) +" Finished!"
		

		return_value = False

		if self.clf is None:
			return_value = False
		else:
			if RTpredict:
				prediction = self.clf.predict(array([curArray[0]]))
				#print "======================================== " + str(prediction) + str(prediction[0][0]) + str(prediction[0][1])
				if prediction[0][0]>prediction[0][1] :
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

	def ReadDataFromFile(self, AIType):
		print "DataSave And Training " + str(self.dataPath) + " Started ..."
		filesNames = [f for f in tqdm(listdir(self.dataPath)) if isfile(join(self.dataPath, f))]
		print "bsmDataExists?"

		ValuesData = []
		TargetData = []

		for i in tqdm(range(0,len(filesNames))):
		#for i in tqdm(range(0,1000)):
			s = filesNames[i]
			if s.endswith(".bsm"):
				bsmJsonString = open(self.dataPath+'/' +s, 'r').read()
				bsmJsom = json.loads(bsmJsonString)
				curArray = self.getNodeArray(bsmJsom,AIType)
				self.DataCollector.collectData(curArray)
			if s.endswith(".lbsm"):
				bsmJsonString = open(self.dataPath+'/' +s, 'r').read()
				listBsmJsom = json.loads(bsmJsonString)
				for bsmJsom in listBsmJsom:
					curArray = self.getNodeArray(bsmJsom,AIType)
					self.DataCollector.collectData(curArray)

		self.DataCollector.saveData()
		self.Trainer.setValuesCollection(self.DataCollector.getValuesCollection())
		self.Trainer.setTargetCollection(self.DataCollector.getTargetCollection())
		self.Trainer.train()
		self.clf = joblib.load(self.savePath+'/clf_'+AIType+'_'+self.curDateStr+'.pkl')
		#self.deltaCall = self.DataCollector.valuesCollection.shape[0]/5
		print "DataSave And Training " + str(self.dataPath) + " Finished!"

	def TrainData(self, AIType):
		print ("Training " + str(self.dataPath) + " Started ...")
		self.Trainer.setValuesCollection(self.DataCollector.getValuesCollection())
		self.Trainer.setTargetCollection(self.DataCollector.getTargetCollection())
		self.Trainer.train()
		self.clf = joblib.load(self.savePath+'/clf_'+AIType+'_'+self.curDateStr+'.pkl')
		self.deltaCall = self.DataCollector.valuesCollection.shape[0]/5
		print ("Training " + str(self.dataPath) + " Finished!")

	def getNodeArray(self,bsmJsom,AIType):
		receiverId = bsmJsom['BsmPrint']['Metadata']['receiverId']
		pseudonym = bsmJsom['BsmPrint']['BSMs'][0]['Pseudonym'] 
		time = bsmJsom['BsmPrint']['Metadata']['generationTime']
		self.Storage.add_bsm(receiverId,pseudonym, time, bsmJsom)
		if('SVM' in AIType):
			returnArray = self.Storage.get_array(receiverId,pseudonym, self.arrayLength)
		if('MLP' in AIType):
			returnArray = self.Storage.get_array_MLP(receiverId,pseudonym, self.arrayLength)
		if('LSTM' in AIType):
			returnArray = self.Storage.get_array_lstm(receiverId,pseudonym, self.arrayLength)

		#print "cur_array: " + str(cur_array)
		#print "returnArray: " + str(returnArray)
		return returnArray

def main():
	globalMlMain = MlMain()
	version = "V2"
	AIType = "MLP_L4NV25"
	globalMlMain.RTTrainDataFromFile = True
	globalMlMain.mlMain(version,"",AIType)

if __name__ == "__main__": main()