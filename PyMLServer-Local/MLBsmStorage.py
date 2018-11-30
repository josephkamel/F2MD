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

import numpy as np
from numpy import array
import math

import itertools
import operator

class MlBsmStorage:

	def __init__(self, *args, **kwargs):
		self.id_index = []
		self.id_bsms = []

	def add_bsm(self, id, time, cur_bsm):
		index = 0
		try:
			index = self.id_index.index(id)
			self.id_bsms[index].append(cur_bsm)
			self.bubblesort(index)

		except ValueError:
			self.id_index.append(id)
			append_bsms = []
			append_bsms.append(cur_bsm)
			self.id_bsms.append(append_bsms)

		#print  'local: ' + str(index) + ' ' + str(id) + ' ' + str(len(self.id_index))
		#print  'id_index: ' + str(id_index.shape)

	def bubblesort(self, index):
		for iter_num in range(len(self.id_bsms[index])-1,0,-1):
			for idx in range(iter_num):
				if self.id_bsms[index][idx]['BsmPrint']['Metadata']['generationTime']>self.id_bsms[index][idx+1]['BsmPrint']['Metadata']['generationTime']:
					temp = self.id_bsms[index][idx]
					self.id_bsms[index][idx]= self.id_bsms[index][idx+1]
					self.id_bsms[index][idx+1] = temp

	def get_distance(self,p1,p2):
		dist = ((p1[0]-p2[0])**2 + (p1[1]-p2[1])**2)**.5 
		return dist
	def get_speed(self,spd):
		speed = math.sqrt((spd[0]**2) + (spd[1]**2))
		return speed

	def get_angle_delta(self,angl1,angl2):
		deltaX = angl2[0] - angl1[0];
		deltaY = angl2[1] - angl1[1];
		rad = math.atan2(deltaY, deltaX);
		deg = rad * (180 / math.pi)
		return deg

	def get_bsm_array(self,bsmNew,bsmOld):
		rP = bsmNew['BsmPrint']['BsmCheck']['rP']
		pP = bsmNew['BsmPrint']['BsmCheck']['pP']
		sP = bsmNew['BsmPrint']['BsmCheck']['sP']
		pC = bsmNew['BsmPrint']['BsmCheck']['pC']
		sC = bsmNew['BsmPrint']['BsmCheck']['sC']
		psC = bsmNew['BsmPrint']['BsmCheck']['psC']
		phC = bsmNew['BsmPrint']['BsmCheck']['phC']
		sA = bsmNew['BsmPrint']['BsmCheck']['sA']
		#sA = 1
		bF = bsmNew['BsmPrint']['BsmCheck']['bF']
		inT = 1
		for x in bsmNew['BsmPrint']['BsmCheck']['inT']:
			if inT>x['uVal']:
				inT = x['uVal']

		DeltaPos = self.get_distance(bsmNew['BsmPrint']['BSMs'][0]['Pos'], bsmOld['BsmPrint']['BSMs'][0]['Pos'])
		PosConfidence = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['PosConfidence'])
		Speed = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['Speed'])
		DeltaSpeed = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['Speed']) - self.get_speed(bsmOld['BsmPrint']['BSMs'][0]['Speed'])
		SpeedConfidence = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['SpeedConfidence'])
		DeltaHeading = self.get_angle_delta(bsmNew['BsmPrint']['BSMs'][0]['Heading'], bsmOld['BsmPrint']['BSMs'][0]['Heading'])
		HeadingConfidence = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['HeadingConfidence'])
		DeltaTime = bsmNew['BsmPrint']['Metadata']['generationTime'] - bsmOld['BsmPrint']['Metadata']['generationTime']

		label = bsmNew['BsmPrint']['Metadata']['mbType']

		#label = 0
		if(label == 'Genuine'):
			numLabel = 0.0
		else:
			numLabel = 1.0

		valuesArray = array([1-rP,1-pP,1-sP,1-pC,1-sC,1-psC,1-phC,1-sA,1-bF,1-inT, DeltaPos, PosConfidence,Speed, DeltaSpeed, SpeedConfidence, DeltaHeading, HeadingConfidence, DeltaTime])
		targetArray = array([numLabel])
		returnArray = array([valuesArray,targetArray])

		#print valuesArray

		#print "returnArray: " + str(returnArray)
		#returnArray = returnArray.astype(np.float)
		return returnArray


	def get_array(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = []
		list_Y = []

		if batch_size<len(self.id_bsms[index]):
			startindex = len(self.id_bsms[index]) - batch_size
			xy_array = self.get_bsm_array(self.id_bsms[index][startindex],self.id_bsms[index][startindex-1])
			list_X = [xy_array[0]]
			list_Y = [xy_array[1]]
		else:
			startindex = 0
			xy_array = self.get_bsm_array(self.id_bsms[index][0],self.id_bsms[index][0])
			list_X = [xy_array[0]]
			list_Y = [xy_array[1]]

		for i in range(startindex+1,len(self.id_bsms[index])):
			xy_array = self.get_bsm_array(self.id_bsms[index][i],self.id_bsms[index][i-1])
			list_X.append(xy_array[0])
			list_Y.append(xy_array[1])

		app_ary = array([[len(list_X)]])
		for i in range(0,len(list_X)-1):
			app_ary = np.append(app_ary.tolist(),[[len(list_X)]],axis=0)
		list_X = np.append(list_X,app_ary,axis=1)
		
		ret_array = [sum(x) for x in zip(*list_X)]

		#print "------------------------------------------------------------------"
		ret_array_2 = np.append(list_X[:,:10].sum(axis=0),list_X[-1,11:])

		#print ret_array
		#print ret_array_2

		return array([array(ret_array_2),list_Y[-1]])

	def get_array_lstm(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = []
		list_Y = []

		if batch_size<len(self.id_bsms[index]):
			startindex = len(self.id_bsms[index]) - batch_size
			xy_array = self.get_bsm_array(self.id_bsms[index][startindex],self.id_bsms[index][startindex-1])
			list_X = [xy_array[0]]
			list_Y = [xy_array[1]]
		else:
			startindex = 0
			xy_array = self.get_bsm_array(self.id_bsms[index][0],self.id_bsms[index][0])
			list_X = [xy_array[0]]
			list_Y = [xy_array[1]]

		for i in range(startindex+1,len(self.id_bsms[index])):
			xy_array = self.get_bsm_array(self.id_bsms[index][i],self.id_bsms[index][i-1])
			list_X.append(xy_array[0])
			list_Y.append(xy_array[1])

		if len(list_X)<batch_size:
			list_X_Ret = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
			for i in range(0,batch_size-len(list_X) - 1):
				list_X_Ret = np.vstack((list_X_Ret,[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]))
			#print list_X_Ret
			list_X_Ret = np.vstack((list_X_Ret,list_X))
			list_X = list_X_Ret

		#nx , ny = np.array(list_X).shape
		#list_X = np.array(list_X).reshape((nx*ny))

		ret_array = array([list_X,list_Y[-1]])
		#print ret_array
		return ret_array

