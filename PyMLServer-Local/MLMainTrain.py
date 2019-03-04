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
#from os import scandir
from os.path import join
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

RTreadDataFromFile = True
RTtrainData = True

class MlMain:
	initiated = False

	curDateStr = datetime.datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
	DataCollector = MlDataCollector()
	Trainer = MlTrainer()
	Storage = MlNodeStorage()
	arrayLength = 40

	clf = None

	savePath = './saveFile/saveFile_D40'
	dataPath = '/home/joseph/Projects/F2MD/mdmSave/Data-1.0/IRT-DATA-0.5/MDBsmsList_V2_2019-3-3_23:8:15'

	def init(self, version, AIType):

		self.savePath = self.savePath +'_'+ str(version)

		self.DataCollector.setCurDateSrt(self.curDateStr)
		self.DataCollector.setSavePath(self.savePath)
		self.Trainer.setCurDateSrt(self.curDateStr)
		self.Trainer.setSavePath(self.savePath)
		self.Trainer.setAIType(AIType)

		self.trainedModelExists(AIType)
		if RTreadDataFromFile:
			self.ReadDataFromFile(AIType)
		if RTtrainData:
			self.TrainData(AIType)

	def mlMain(self):
		version = "V2"
		AIType = "MLP_L3N25"

		if not self.initiated:
			self.init(version,AIType)
			self.initiated = True
		return False


	def trainedModelExists(self, AIType):
		#filesNames = [f.name  for f in scandir(self.savePath) if isfile(join(self.savePath, f.name))]
		filesNames = [f for f in tqdm(os.listdir(self.savePath)) if os.path.isfile(join(self.savePath, f))]

		print ("trainedModelExists?")
		for s in filesNames:
			if s.startswith('clf_'+AIType) and s.endswith(".pkl"):
				self.curDateStr = s[-23:-4]

				print ("Loading " +AIType + " "+ self.curDateStr+ " ...")
				self.clf = joblib.load(self.savePath+'/'+s)
				self.DataCollector.setCurDateSrt(self.curDateStr)
				self.Trainer.setCurDateSrt(self.curDateStr)
				self.DataCollector.loadData()
				self.Trainer.setValuesCollection(self.DataCollector.getValuesCollection())
				self.Trainer.setTargetCollection(self.DataCollector.getTargetCollection())
				print ("Loading " + str(self.DataCollector.valuesCollection.shape) +  " Finished!")

	def ReadDataFromFile(self, AIType):
		print ("DataLoad " + str(self.dataPath) + " Started ...")

		#filesNames = [f.name for f in tqdm(scandir(self.dataPath)) if f.is_file()]
		filesNames = [f for f in tqdm(os.listdir(self.dataPath)) if os.path.isfile(join(self.dataPath, f))]
		print ("bsmDataExists?")

		ValuesData = []
		TargetData = []
		
		for i in tqdm(range(0,len(filesNames))):
		#for i in tqdm(range(0,5000)):
			s = filesNames[i]
			if s.endswith(".bsm"):
				bsmJsonString = open(self.dataPath+'/' +s, 'r').read()
				bsmJsom = json.loads(bsmJsonString)
				curArray = self.getNodeArray(bsmJsom,AIType)
				self.DataCollector.collectData(curArray)
			if s.endswith(".lbsm"):
				bsmJsonString = open(self.dataPath+'/' +s, 'r').read()
				bsmJsom = json.loads(bsmJsonString)
				for bsmItem in bsmJsom:
					curArray = self.getNodeArray(bsmItem,AIType)
					self.DataCollector.collectData(curArray)
		print ("saveData Started...")
		self.DataCollector.saveData()
		print ("saveData Finished!")
		print ("DataLoad " + str(self.dataPath) + " Finished!")

	def TrainData(self, AIType):
		print ("Training " + str(self.dataPath) + " Started ...")
		self.Trainer.setValuesCollection(self.DataCollector.getValuesCollection())
		self.Trainer.setTargetCollection(self.DataCollector.getTargetCollection())
		self.Trainer.train()
		self.clf = joblib.load(self.savePath+'/clf_'+AIType+'_'+self.curDateStr+'.pkl')
		print ("Training " + str(self.dataPath) + " Finished!")


	def getNodeArray(self,bsmJsom,AIType):
		receiverId = bsmJsom['BsmPrint']['Metadata']['receiverId']
		pseudonym = bsmJsom['BsmPrint']['BSMs'][0]['Pseudonym'] 
		time = bsmJsom['BsmPrint']['Metadata']['generationTime']
		self.Storage.add_bsm(receiverId,pseudonym, time, bsmJsom)
		if(AIType == 'SVM'):
			returnArray = self.Storage.get_array(receiverId,pseudonym, self.arrayLength)
		if(AIType == 'MLP_L1N15'):
			returnArray = self.Storage.get_array_MLP_L1N15(receiverId,pseudonym, self.arrayLength)
		if(AIType == 'MLP_L3N25'):
			returnArray = self.Storage.get_array_MLP_L3N25(receiverId,pseudonym, self.arrayLength)
		if(AIType == 'LSTM'):
			returnArray = self.Storage.get_array_lstm(receiverId,pseudonym, self.arrayLength)

		#print "cur_array: " + str(cur_array)
		#print "returnArray: " + str(returnArray)
		return returnArray



def main():
	globalMlMain = MlMain()
	globalMlMain.mlMain()

if __name__ == "__main__": main()
