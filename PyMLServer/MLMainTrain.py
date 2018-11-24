import os
#from os import scandir
from os.path import join
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

RTreadDataFromFile = False
RTtrainData = True
RTpredict = True

class MlMain:
	initiated = False

	curDateStr = datetime.datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
	DataCollector = MlDataCollector()
	Trainer = MlTrainer()
	Storage = MlArrayStorage()
	arrayLength = 20


	collectDur = 0
	deltaCall = 1000

	clf = None
	savePath = './saveFile/saveFile_Mix_D20_3L15N'
	#dataPath = '/media/sca-team/ef5ca73c-c8ef-4e03-a88c-a54bcbb15b0e/DataF2MD/Test'
<<<<<<< HEAD
	dataPath = '/media/sca-team/DATA/DataF2MD/IRT-Reports-Mix-V2-List/MDBsmsList_2018-11-20_17:35:42'
=======
	dataPath = '/media/joseph/Windows/DataF2MD/IRT-MIX-V2/MDBsms_2018-11-7_6:17:43'
	#dataPath = '/media/sca-team/ef5ca73c-c8ef-4e03-a88c-a54bcbb15b0e/DataF2MD/IRT-BSMS-MIX-V2/MDBsms_2018-11-5_15:22:52'
>>>>>>> 55f2460382f1506909d14701d91df4cbb08f2d0c

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
		if RTtrainData:
			self.TrainData(AIType)

	def mlMain(self):
		version = "V2"
<<<<<<< HEAD
		AIType = "neural_network_3L15N"
=======
		AIType = "neural_network"
>>>>>>> 55f2460382f1506909d14701d91df4cbb08f2d0c
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
 
				self.deltaCall = self.DataCollector.valuesCollection.shape[0]/5
				print ("Loading " + str(self.DataCollector.valuesCollection.shape) +  " Finished!")

	def ReadDataFromFile(self, AIType):
		print ("DataLoad " + str(self.dataPath) + " Started ...")

		#filesNames = [f.name for f in tqdm(scandir(self.dataPath)) if f.is_file()]
		filesNames = [f for f in tqdm(os.listdir(self.dataPath)) if os.path.isfile(join(self.dataPath, f))]
		print ("bsmDataExists?")

		ValuesData = []
		TargetData = []

		for i in tqdm(range(0,len(filesNames))):
		#for i in tqdm(range(0,100)):
			s = filesNames[i]
			if s.endswith(".bsm"):
				bsmJsonString = open(self.dataPath+'/' +s, 'r').read()
				bsmJsom = json.loads(bsmJsonString)
				curArray = self.getNodeArray(bsmJsom)
				self.DataCollector.collectData(curArray)
			if s.endswith(".lbsm"):
				bsmJsonString = open(self.dataPath+'/' +s, 'r').read()
				bsmJsom = json.loads(bsmJsonString)
				for bsmItem in bsmJsom:
					curArray = self.getNodeArray(bsmItem)
					self.DataCollector.collectData(curArray)

		self.DataCollector.saveData()
		print ("DataLoad " + str(self.dataPath) + " Finished!")

	def TrainData(self, AIType):
		print ("Training " + str(self.dataPath) + " Started ...")
		self.Trainer.setValuesCollection(self.DataCollector.getValuesCollection())
		self.Trainer.setTargetCollection(self.DataCollector.getTargetCollection())
		self.Trainer.train()
		self.clf = joblib.load(self.savePath+'/clf_'+AIType+'_'+self.curDateStr+'.pkl')
		self.deltaCall = self.DataCollector.valuesCollection.shape[0]/5
		print ("Training " + str(self.dataPath) + " Finished!")

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
		
		#valuesArray = array([rP,pP,sP,pC,sC,psC,phC,sA,bF,inT])
		valuesArray = array([1-rP,1-pP,1-sP,1-pC,1-sC,1-psC,1-phC,1-sA,1-bF,1-inT,1])
		targetArray = array([numLabel])
		returnArray = array([valuesArray,targetArray])

		#print "returnArray: " + str(returnArray)
		#returnArray = returnArray.astype(np.float)
		return returnArray


def main():
	globalMlMain = MlMain()
	globalMlMain.mlMain()

if __name__ == "__main__": main()
