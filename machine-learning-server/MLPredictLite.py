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

from MLMain import MlMain
from tqdm import tqdm
from os import listdir, mkdir
from os.path import isfile, join, exists
import json
from MLVarThresholdLite import MlVarThresholdLite
from multiprocessing import Pool, Process, Queue

version = "V2"
AIType = "COMBINED_LSTM10_DENSE36_DENSE24"
dataPath = '/home/joseph/Projects/F2MD/mdmSave/Data-Experi/IRT-05p-BSMs/MDBsmsList_V2_2019-9-23_11:48:32'

globalMlMain = MlMain()
globalMlMain.init(version,"AIType")
globalMlMain.RTmultipredict = True

numberOfIters = 1
numberOfThreads = 16
multi_processing = True

def local_process(filesNames,threadNumber, thread, q):
	localMlMain = MlMain()
	localMlMain.RTmultipredict = True
	localMlMain.multi_predict_count = int(localMlMain.multi_predict_num*threadNumber/numberOfThreads)
	
	totalLenS = int(len(filesNames)*0.0)
	totalLen = int(len(filesNames)*1.0)
	for i in tqdm(range(totalLenS,totalLen),unit=' '+AIType+' '):
		s = filesNames[i]
		if s.endswith(".bsm"):
			bsmJsonString = open(dataPath+'/' +s, 'r').read()
			localMlMain.mlMain(version,bsmJsonString,AIType)
		if s.endswith(".lbsm"):
			bsmJsonString = open(dataPath+'/' +s, 'r').read()
			listBsmJsom = json.loads(bsmJsonString)
			totalSubLen = int(len(listBsmJsom))
			for j in range(0,totalSubLen):
				if(i==(totalLen-1) and j==(totalSubLen-1)):
					localMlMain.multi_predict_count = localMlMain.multi_predict_num+1
				localMlMain.mlMain(version,json.dumps(listBsmJsom[j]),AIType)
	if thread:
		q.put(localMlMain.varthrelite)
	else:  
		return localMlMain.varthrelite


print("Data Predict " + str(dataPath) + " Started ...")
print("bsmDataExists?")
filesNames = [f for f in tqdm(listdir(dataPath)) if isfile(join(dataPath, f))]

range_start = 0
range_end = range_start + int(len(filesNames)/(numberOfThreads*numberOfIters))
for it_i in range(0,numberOfIters):
	print("Iteration " +str(it_i)+ " Start ...")
	process_list = []
	queue_list = []

	for i in range(0,numberOfThreads):
		local_input_list = []
		localfilesNames = filesNames[range_start:range_end]
		range_start = range_end
		range_end = range_start + int(len(filesNames)/(numberOfThreads*numberOfIters))
		if (i == numberOfThreads-2) and (it_i == numberOfIters-1):
			range_end = len(filesNames)
		q = Queue()
		p = Process(target=local_process, args=(localfilesNames,i,True,q))
		p.start()
		process_list.append(p)
		queue_list.append(q)
	already_parsed = []
	while(len(already_parsed) != len(queue_list)):
		for i_q in range(0,len(queue_list)):
			if (i_q not in already_parsed) and (not queue_list[i_q].empty()):
				already_parsed.append(i_q)
				print("Getting Results .... " + str(i_q))
				tempVarThre = queue_list[i_q].get()
				globalMlMain.varthrelite.collect(tempVarThre)
	print("Getting Results Finished!")

	print("Iteration " +str(it_i)+ " End!")

if not exists(globalMlMain.savePath+'/results'):
		mkdir(globalMlMain.savePath+'/results')

f = open(globalMlMain.savePath+'/results/res_'+AIType+'_'+version+'_'+globalMlMain.curDateStr+'.txt', "a")


f.write(globalMlMain.varthrelite.get_stats())
f.close()