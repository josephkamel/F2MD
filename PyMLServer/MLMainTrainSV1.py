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

RTreadDataFromFile = True
RTpredict = True

class MlMain:
	initiated = False

	curDateStr = datetime.datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
	DataCollector = MlDataCollector()
	Trainer = MlTrainer()
	Storage = MlArrayStorage()
	arrayLength = 1

	collectDur = 0
	deltaCall = 1000

	clf = None
	savePath = './saveFile_Sybil'
	dataPath = '/media/sca-team/Data/LinuxData/F2MD-Data/mdmSave/IRT-BSMS/IRT-Sybil-3-0.5-V1/MDBsms_2018-10-31_18:6:51'

	def init(self, version, AIType):

		self.savePath = self.savePath +'_'+ str(version)

		self.DataCollector.setCurDateSrt(self.curDateStr)
		self.DataCollector.setSavePath(self.savePath)
		self.Trainer.setCurDateSrt(self.curDateStr)
		self.Trainer.setSavePath(self.savePath)
		self.Trainer.setAIType(AIType)

		self.trainedModelExists(AIType)
		if RTreadDataFromFile:
			self.ReadDataFromFile( AIType)

	def mlMain(self):
		version = "V1"
		AIType = "neural_network"
		if not self.initiated:
			self.init(version,AIType)
			self.initiated = True
		return False


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
 
				self.deltaCall = self.DataCollector.valuesCollection.shape[0]/5
				print "Loading " + str(self.DataCollector.valuesCollection.shape) +  " Finished!"

	def ReadDataFromFile(self, AIType):
		print "DataSave And Training " + str(self.dataPath) + " Started ..."
		filesNames = [f for f in tqdm(listdir(self.dataPath)) if isfile(join(self.dataPath, f))]
		print "bsmDataExists?"

		ValuesData = []
		TargetData = []

		for i in tqdm(range(0,len(filesNames))):
			s = filesNames[i]
			if s.endswith(".bsm"):
				bsmJsonString = open(self.dataPath+'/' +s, 'r').read()
				bsmJsom = json.loads(bsmJsonString)
				curArray = self.getNodeArray(bsmJsom)
				ValuesData.append(curArray[0])
				TargetData.append(curArray[1])
		
		self.DataCollector.initValuesData(ValuesData)
		self.DataCollector.initTargetData(TargetData)

		self.DataCollector.saveData()
		self.Trainer.setValuesCollection(self.DataCollector.getValuesCollection())
		self.Trainer.setTargetCollection(self.DataCollector.getTargetCollection())
		self.Trainer.train()
		self.clf = joblib.load(self.savePath+'/clf_'+AIType+'_'+self.curDateStr+'.pkl')
		self.deltaCall = self.DataCollector.valuesCollection.shape[0]/5
		print "DataSave And Training " + str(self.dataPath) + " Finished!"

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
		
		valuesArray = array([rP,pP,sP,pC,sC,psC,phC,sA,bF,inT])
		targetArray = array([numLabel])
		returnArray = array([valuesArray,targetArray])

		#print "returnArray: " + str(returnArray)
		#returnArray = returnArray.astype(np.float)
		return returnArray


def main():
	globalMlMain = MlMain()
	globalMlMain.mlMain()

if __name__ == "__main__": main()
