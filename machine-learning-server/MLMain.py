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
import datetime
from tqdm import tqdm
from sklearn import datasets
import joblib
from multiprocessing import Pool, Process, Queue, Manager

from MLLabelEncoder import MlLabelEncoder
from MLStats import MlStats
from MLVarThreshold import MlVarThreshold
from MLVarThresholdLite import MlVarThresholdLite

import time

RTDetectAttackTypes = False

RTaddexistingweights= False
RTuseexistingdata= False

RTtrain = False
RTcollectData = False
RTpredict = True

Positive_Threshold = 0.5

RTFilterTime = 100
RTFilterKeepTime = 600

class MlMain:
	initiated = False

	curDateStr = datetime.datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
	DataCollector = MlDataCollector()
	Trainer = MlTrainer()
	Storage = MlNodeStorage()
	arrayLength = 60

	collectDur = 0
	deltaCall = 100000

	clf = None
	savePath = './saveFile/saveFile_D60'
	dataPath = './MDBsmsList_V1_2020-6-16_15:49:0'
	RTTrainDataFromFile = False

	meanRuntime = 0
	meanRuntime_p = 0
	numRuntime = 0
	printRuntime = 10000*10000
	printRuntimeCnt = 0

	filterdelta=0

	labels_legacy = [ "Genuine","LocalAttacker",]

	labels_attacks = [ "Genuine","ConstPos", "ConstPosOffset", "RandomPos", "RandomPosOffset",
		"ConstSpeed", "ConstSpeedOffset", "RandomSpeed", "RandomSpeedOffset",
		"EventualStop", "Disruptive", "DataReplay", "StaleMessages",
		"DoS", "DoSRandom", "DoSDisruptive",
		"GridSybil", "DataReplaySybil", "DoSRandomSybil",
		"DoSDisruptiveSybil",]

	version_added = False

	le = MlLabelEncoder()

	stats = MlStats()
	varthrelite = MlVarThresholdLite()

	RTmultipredict= False
	multi_predict_num = 524288
	multi_predict_count = 0
	multi_predict_array = []
	multi_predict_array_combined = {}
	multi_predict_label = []
	multi_predict_label_combined = {}

	def init(self, version, AIType):
		if RTDetectAttackTypes:
			self.le.fit(self.labels_attacks)
		else:
			self.le.fit(self.labels_legacy)
		if not self.version_added:
			if RTDetectAttackTypes:
				self.savePath = self.savePath + '_Attacks_'+ str(version)
			else:
				self.savePath = self.savePath + '_Legacy_'+ str(version)
			self.version_added = True


		self.DataCollector.setCurDateSrt(self.curDateStr)
		self.DataCollector.setSavePath(self.savePath)
		self.Trainer.setCurDateSrt(self.curDateStr)
		self.Trainer.setSavePath(self.savePath)
		self.Trainer.setAIType(AIType)

		self.trainedModelExists(AIType)
		if self.RTTrainDataFromFile:
			if RTaddexistingweights:
				self.clf = joblib.load(self.savePath+'/clf_'+AIType+'_'+self.curDateStr+'.pkl')
			self.ReadDataFromFile(AIType)
			#self.TrainData(AIType)
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
				print("DataSave And Training " + str(self.deltaCall) + " Started ...")
				self.collectDur = 0
				self.DataCollector.saveData()

				if RTtrain:
					print(len(self.Trainer.dataCollector.ValuesData))
					self.Trainer.train(self.DataCollector,self.le)
					self.clf = joblib.load(self.savePath+'/clf_'+AIType+'_'+self.curDateStr+'.pkl')
					self.deltaCall = len(self.Trainer.dataCollector.ValuesData)/2
					#self.deltaCall = 10000000
				print("DataSave And Training " + str(self.deltaCall) +" Finished!")

		return_value = "False"
		#return return_value

		if self.clf is None:
			return_value = "False"
			start_time_p = 0.0
			end_time_p = 0.0
		else:
			#self.clf.save(self.savePath + "/model.h5")
			#self.clf.save_weights(self.savePath + "/model_weights.h5")
			#os._exit(0)
			if RTpredict:
				if('LSTM' in AIType):
					self.clf.reset_states()
				if self.RTmultipredict:
					start_time_p = 0.0
					end_time_p = 0.0
					if 'COMBINED' in AIType:
						cur_shape_0 = curArray[0][0].shape[0]
						if cur_shape_0 in self.multi_predict_array_combined.keys():
							self.multi_predict_array_combined[cur_shape_0].append([curArray[0][0],curArray[0][1]])
							self.multi_predict_label_combined[cur_shape_0].append(self.le.transform([bsmJsom['BsmPrint']['Metadata']['mbType']])[0])
						else:
							self.multi_predict_array_combined[cur_shape_0] = []
							self.multi_predict_label_combined[cur_shape_0] = []
							self.multi_predict_array_combined[cur_shape_0].append([curArray[0][0],curArray[0][1]])
							self.multi_predict_label_combined[cur_shape_0].append(self.le.transform([bsmJsom['BsmPrint']['Metadata']['mbType']])[0])
					else:
						self.multi_predict_array.append(curArray[0])
						self.multi_predict_label.append(self.le.transform([bsmJsom['BsmPrint']['Metadata']['mbType']])[0])

					if self.multi_predict_count > self.multi_predict_num:
						pred_array_list = []
						if 'COMBINED' in AIType:
							for cur_shape_0 in self.multi_predict_array_combined.keys():
								multi_predict_array = self.multi_predict_array_combined[cur_shape_0]
								lstm_arrays=np.array([xi[0] for xi in multi_predict_array])
								mlp_arrays=np.array([xi[1] for xi in multi_predict_array])
								#lstm_arrays = np.squeeze(lstm_arrays)
								#mlp_arrays = np.squeeze(mlp_arrays)
								pred_array_list.append(self.clf.predict([lstm_arrays,mlp_arrays]))
								self.multi_predict_label.append(self.multi_predict_label_combined[cur_shape_0])
						else:
							pred_array_list.append(self.clf.predict(np.array(self.multi_predict_array)))
							self.multi_predict_label = [self.multi_predict_label]

						for pred_array_index in range(0,len(pred_array_list)):
							pred_array = pred_array_list[pred_array_index]
							for index in range(0,len(pred_array)):
								if 'XGBoost' in AIType or 'SVM' in AIType or 'LogisticRegression' in AIType:
									prediction = pred_array[index]
								else:
									prediction = pred_array[index][1-self.le.transform(['Genuine'])[0]]
								self.varthrelite.update_stats(prediction,self.multi_predict_label[pred_array_index][index])
								if prediction>Positive_Threshold:
									#self.stats.update_stats(True,self.multi_predict_label[index])
									return_value = "True"
								else:
									#self.stats.update_stats(False,self.multi_predict_label[index])
									return_value = "False"
						del self.multi_predict_array[:]
						del self.multi_predict_label[:]
						self.multi_predict_array_combined.clear()
						self.multi_predict_label_combined.clear()
						self.multi_predict_count = 0
					else:
						self.multi_predict_count = self.multi_predict_count + 1

				else:
					if 'COMBINED' in AIType:
						array_npy = [np.array([curArray[0][0]]),np.array([curArray[0][1]])]
					else:
						array_npy = np.array([curArray[0]])
					start_time_p = time.time()
					pred_array = self.clf.predict(array_npy)
					end_time_p = time.time()
					gen_index = self.le.transform(['Genuine'])[0]
					if 'XGBoost' in AIType or 'SVM' in AIType or 'LogisticRegression' in AIType:
						prediction = pred_array[0]
					else:
						prediction = pred_array[0][1-gen_index]

					label_index = self.le.transform([bsmJsom['BsmPrint']['Metadata']['mbType']])[0]
					self.varthrelite.update_stats(prediction,label_index)
					if prediction>Positive_Threshold:
						self.stats.update_stats(True,label_index)
						return_value = "True"
					else:
						self.stats.update_stats(False,label_index)
						return_value = "False"
			#print prediction
			#print curArray[1]
			#print "========================================"

		end_time = time.time()
		self.meanRuntime = (self.numRuntime*self.meanRuntime + (end_time-start_time))/(self.numRuntime+1)
		self.meanRuntime_p = (self.numRuntime*self.meanRuntime_p + (end_time_p-start_time_p))/(self.numRuntime+1)
		if self.printRuntimeCnt >= self.printRuntime:
			self.printRuntimeCnt = 0
			print('meanRuntime: ' + str(self.meanRuntime) + ' ' + str(self.numRuntime) + ' predict:' + str(self.meanRuntime_p))
			self.stats.print_stats()
			self.varthrelite.print_stats()
			self.printRuntimeCnt = self.printRuntimeCnt + 1
		else:
			self.printRuntimeCnt = self.printRuntimeCnt + 1
		self.numRuntime = self.numRuntime + 1

		return return_value


	def trainedModelExists(self, AIType):
		filesNames = [f for f in listdir(self.savePath) if isfile(join(self.savePath, f))]
		print("trainedModelExists?")

		for s in filesNames:
			if s.startswith('clf_'+AIType) and s.endswith(".pkl"):

				print("Loading " + s + " " +AIType + " "+ self.curDateStr+ " ...")
				self.clf = joblib.load(self.savePath+'/'+s)
				if RTcollectData:
					self.curDateStr = s[-23:-4]
					self.DataCollector.setCurDateSrt(self.curDateStr)
					self.Trainer.setCurDateSrt(self.curDateStr)
					self.DataCollector.loadData()
				else:
					self.DataCollector.setCurDateSrt(self.curDateStr)
					self.Trainer.setCurDateSrt(self.curDateStr)
				#self.deltaCall = self.DataCollector.valuesCollection.shape[0]/5
				print("Loading " + str(len(self.DataCollector.ValuesData)) +  " Finished!")

	def ReadDataFromFile(self, AIType):
		print("DataSave And Training " + str(self.dataPath) + " Started ...")
		print("bsmDataExists?")

		filesNames = [f for f in tqdm(listdir(self.dataPath)) if isfile(join(self.dataPath, f))]
		numberOfIters = 1
		numberOfThreads = 8
		multi_processing = True

		if not RTuseexistingdata:
			if multi_processing:
				range_start = 0
				range_end = range_start + int(len(filesNames)/(numberOfThreads*numberOfIters))
				for it_i in range(0,numberOfIters):
					print("Iteration " +str(it_i)+ " Start ...")
					input_data_list = []
					process_list = []
					queue_list = []

					for i in range(0,numberOfThreads):
						local_input_list = []
						localfilesNames = filesNames[range_start:range_end]
						range_start = range_end
						range_end = range_start + int(len(filesNames)/(numberOfThreads*numberOfIters))
						if (i == numberOfThreads-2) and (it_i == numberOfIters-1):
							range_end = len(filesNames)
						local_input_list = [localfilesNames,self.dataPath,AIType]
						q = Queue()
						m = Manager()
						return_dict = m.dict()
						return_dict[1] = []
						p = Process(target=local_process, args=(local_input_list,True,return_dict))
						p.start()
						process_list.append(p)
						queue_list.append(return_dict)
						input_data_list.append(local_input_list)
					#for p in process_list:
					#	p.join()
					#listDataCollectors=[]
					print("Getting Results ....")
					already_parsed = []
					while(len(already_parsed) != len(queue_list)):
						for i_q in range(0,len(queue_list)):
							if (i_q not in already_parsed) and (len(queue_list[i_q][1]) > 0):
								already_parsed.append(i_q)
								tempDataCollector = queue_list[i_q][1][0]
								print("Getting Results .... " + str(i_q) + " ... " + str(len(tempDataCollector.TargetData)))
								for i in range(len(tempDataCollector.TargetData)-1,-1,-1):

									self.DataCollector.collectData([[tempDataCollector.ValuesData[0][i], tempDataCollector.ValuesData[1][i]],tempDataCollector.TargetData[i]])
									del tempDataCollector.TargetData[i]
									del tempDataCollector.ValuesData[0][i]
									del tempDataCollector.ValuesData[1][i]
					print("Getting Results Finished!")

					#self.DataCollector.saveData(it_i)
					print("Iteration " +str(it_i)+ " End!")
			else:
				tempDataCollector = local_process([filesNames,self.dataPath,AIType])
				for i in tqdm(range(len(tempDataCollector.TargetData)-1,-1,-1)):
					self.DataCollector.collectData([tempDataCollector.ValuesData[i],tempDataCollector.TargetData[i]])
					del tempDataCollector.TargetData[i]
					del tempDataCollector.ValuesData[i]
				self.DataCollector.saveData(0)
		if RTaddexistingweights:
			self.Trainer.setSavedModel(self.clf)
		self.Trainer.train(self.DataCollector,self.le)

		self.clf = joblib.load(self.savePath+'/clf_'+AIType+'_'+self.curDateStr+'.pkl')
		#self.deltaCall = self.DataCollector.valuesCollection.shape[0]/5
		print("DataSave And Training " + str(self.dataPath) + " Finished!")

	def extract_time(self,json):
		try:
			return float(json['BsmPrint']['Metadata']['generationTime'])
		except KeyError:
			return 0

	def TrainData(self, AIType):
		print ("Training " + str(self.dataPath) + " Started ...")
		self.Trainer.train(self.DataCollector,self.le)
		self.clf = joblib.load(self.savePath+'/clf_'+AIType+'_'+self.curDateStr+'.pkl')
		self.deltaCall = self.DataCollector.valuesCollection.shape[0]/5
		print ("Training " + str(self.dataPath) + " Finished!")

	def getNodeArray(self,bsmJsom,AIType):
		receiverId = bsmJsom['BsmPrint']['Metadata']['receiverId']
		pseudonym = bsmJsom['BsmPrint']['BSMs'][0]['Pseudonym']
		time = bsmJsom['BsmPrint']['Metadata']['generationTime']

		if RTDetectAttackTypes:
			label = bsmJsom['BsmPrint']['Metadata']['attackType']
		else:
			label = bsmJsom['BsmPrint']['Metadata']['mbType']
			if label == 'GlobalAttacker':
				label = 'Genuine'

		numLabel = np.array(self.le.transform([label])[0], dtype=np.int8)

		self.Storage.add_bsm(receiverId,pseudonym, time, bsmJsom, self.arrayLength, numLabel)

		if time - self.filterdelta > RTFilterTime :
			self.filterdelta = time
			self.Storage.filter_bsms(time ,RTFilterKeepTime)

		if('SINGLE' in AIType):
			returnArray = self.Storage.get_array(receiverId,pseudonym)
		if('FEATURES' in AIType):
			returnArray = self.Storage.get_array_features(receiverId,pseudonym)
		if('AVEFEAT' in AIType):
			returnArray = self.Storage.get_array_MLP_features(receiverId,pseudonym, self.arrayLength)
		if('AVERAGE' in AIType):
			returnArray = self.Storage.get_array_MLP(receiverId,pseudonym, self.arrayLength)
		if('RECURRENT' in AIType):
			returnArray = self.Storage.get_array_lstm(receiverId,pseudonym, self.arrayLength)
		if('RECUFEAT' in AIType):
			returnArray = self.Storage.get_array_lstm_feat(receiverId,pseudonym, self.arrayLength)
		if('RECUSIN' in AIType):
			returnArray = self.Storage.get_array_lstm_sin(receiverId,pseudonym, self.arrayLength)
		if('RECUMIX' in AIType):
			returnArray = self.Storage.get_array_lstm_mix(receiverId,pseudonym, self.arrayLength)
		if('RECUALL' in AIType):
			returnArray = self.Storage.get_array_lstm_all(receiverId,pseudonym, self.arrayLength)
		if('COMBINED' in AIType):
			returnArray = self.Storage.get_array_combined(receiverId,pseudonym, self.arrayLength)


		#print("cur_array: " + str(cur_array))
		#print("returnArray: " + str(returnArray))
		return returnArray

def local_process(local_input_list, thread, q):
	filesNames = local_input_list[0]
	dataPath= local_input_list[1]
	AIType= local_input_list[2]
	localMaMain = MlMain()
	localDataCollector = MlDataCollector()
	bsm_list = []
	for i in tqdm(range(0,int(len(filesNames)*0.2))):
		s = filesNames[i]
		if s.endswith(".bsm"):
			bsmJsonString = open(dataPath+'/' +s, 'r').read()
			bsmJsom = json.loads(bsmJsonString)
			bsm_list.append(bsmJsom)
		if s.endswith(".lbsm"):
			bsmJsonString = open(dataPath+'/' +s, 'r').read()
			listBsmJsom = json.loads(bsmJsonString)
			for bsmJsom in listBsmJsom:
				bsm_list.append(bsmJsom)

	#print(bsm_list[0]['BsmPrint']['Metadata']['generationTime'])
	#print(bsm_list[-1]['BsmPrint']['Metadata']['generationTime'])
	bsm_list.sort(key=localMaMain.extract_time, reverse=True)
	#print(bsm_list[0]['BsmPrint']['Metadata']['generationTime'])
	#print(bsm_list[-1]['BsmPrint']['Metadata']['generationTime'])

	for i in tqdm(range(len(bsm_list)-1,-1,-1)):
		curArray = localMaMain.getNodeArray(bsm_list[i],AIType)
		localDataCollector.collectData(curArray)
		del bsm_list[i]

	if thread:
		q[1] = [localDataCollector]
	else:
		return localDataCollector

def main():
	globalMlMain = MlMain()
	version = "V2"
	AIType = "COMBINED_LSTM10_DENSE36_DENSE24"
	globalMlMain.RTTrainDataFromFile = True
	globalMlMain.mlMain(version,"",AIType)

if __name__ == "__main__": main()
