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
from os import listdir
from os.path import isfile, join
import json

globalMlMain = MlMain()
globalMlMain.RTmultipredict = True
version = "V2"
AIType = "COMBINED_LSTM20_MLP36"
dataPath = '/home/joseph/Projects/F2MD/mdmSave/IRT-Paper-Test-Data/MDBsmsList_V2_2019-7-4_11:44:47'

print("Data Predict " + str(dataPath) + " Started ...")
print("bsmDataExists?")
filesNames = [f for f in tqdm(listdir(dataPath)) if isfile(join(dataPath, f))]


totalLenS = int(len(filesNames)*0.0)
totalLen = int(len(filesNames)*1.0)

for i in tqdm(range(totalLenS,totalLen),unit=' '+AIType+' '):
	s = filesNames[i]
	if s.endswith(".bsm"):
		bsmJsonString = open(dataPath+'/' +s, 'r').read()
		globalMlMain.mlMain(version,bsmJsonString,AIType)
	if s.endswith(".lbsm"):
		bsmJsonString = open(dataPath+'/' +s, 'r').read()
		listBsmJsom = json.loads(bsmJsonString)
		totalSubLen = int(len(listBsmJsom))
		for j in range(0,totalSubLen):
			if(i==(totalLen-1) and j==(totalSubLen-1)):
				globalMlMain.multi_predict_count = globalMlMain.multi_predict_num+1
			globalMlMain.mlMain(version,json.dumps(listBsmJsom[j]),AIType)

f = open(globalMlMain.savePath+'/results/res_'+AIType+'_'+globalMlMain.curDateStr+'.txt', "a")
f.write(globalMlMain.varthrelite.get_stats())
f.close()