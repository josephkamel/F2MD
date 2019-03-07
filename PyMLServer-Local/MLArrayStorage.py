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
import numpy as np
from numpy import array
import math
import itertools
import operator

class MlArrayStorage:

	def __init__(self, *args, **kwargs):
		self.id_index = []
		self.id_time = []
		self.id_bsm = []
		self.id_array_x = []
		self.id_array_y = []

	def add_bsm(self, id, time, cur_bsm, batch_size):
		try:
			index = self.id_index.index(id)
			cur_array = self.get_bsm_array(cur_bsm,self.id_bsm[index])
		except ValueError:
			cur_array = self.get_bsm_array(cur_bsm,cur_bsm)

		self.add_array(id,time,cur_array,cur_bsm,batch_size)

	def add_array(self, id, time, cur_array,cur_bsm, batch_size):
		index = 0
		try:
			index = self.id_index.index(id)
			self.id_time[index].append(time)
			self.id_bsm[index] = cur_bsm
			self.id_array_x[index].append(cur_array[0])
			self.id_array_y[index].append(cur_array[1])

			if len(self.id_time[index]) > batch_size:
				self.id_time[index] = self.id_time[index][-batch_size:]
				self.id_array_x[index] = self.id_array_x[index][-batch_size:]
				self.id_array_y[index] = self.id_array_y[index][-batch_size:]
			self.bubblesort(index)

		except ValueError:
			self.id_index.append(id)
			append_time = []
			append_time.append(time)
			self.id_time.append(append_time)
			self.id_bsm.append(cur_bsm)
			append_array_x = []
			append_array_x.append(cur_array[0])
			self.id_array_x.append(append_array_x)
			append_array_y = []
			append_array_y.append(cur_array[1])
			self.id_array_y.append(append_array_y)

		#print  'local: ' + str(index) + ' ' + str(id) + ' ' + str(len(self.id_index))
		#print  'id_index: ' + str(id_index.shape)

	def bubblesort(self, index):
		for iter_num in range(len(self.id_time[index])-2,-1,-1):
			for idx in range(iter_num):
				if self.id_time[index][idx]>self.id_time[index][idx+1]:
					temp = self.id_time[index][idx]
					self.id_time[index][idx] = self.id_time[index][idx+1]
					self.id_time[index][idx+1] = temp

					temp2 = self.id_array_x[index][idx]
					self.id_array_x[index][idx] = self.id_array_x[index][idx+1]
					self.id_array_x[index][idx+1] = temp2

					temp3 = self.id_array_y[index][idx]
					self.id_array_y[index][idx] = self.id_array_y[index][idx+1]
					self.id_array_y[index][idx+1] = temp3

					os._exit(0)


	def get_bsm_array(self,bsmNew,bsmOld):
		rP = bsmNew['BsmPrint']['BsmCheck']['rP']
		pP = bsmNew['BsmPrint']['BsmCheck']['pP']
		sP = bsmNew['BsmPrint']['BsmCheck']['sP']
		pC = bsmNew['BsmPrint']['BsmCheck']['pC']
		sC = bsmNew['BsmPrint']['BsmCheck']['sC']
		psC = bsmNew['BsmPrint']['BsmCheck']['psC']
		psmC = bsmNew['BsmPrint']['BsmCheck']['psmC']
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


		if DeltaTime<0:
			print('Error')

		label = bsmNew['BsmPrint']['Metadata']['mbType']

		#label = 0
		if(label == 'Genuine'):
			numLabel = 0.0
		else:
			numLabel = 1.0

		zeros = 0
		if rP==0:
			zeros=zeros+1
		if pP==0:
			zeros=zeros+1
		if sP==0:
			zeros=zeros+1
		if pC==0:
			zeros=zeros+1
		if sC==0:
			zeros=zeros+1
		if psC==0:
			zeros=zeros+1
		if psmC==0:
			zeros=zeros+1
		if phC==0:
			zeros=zeros+1
		if bF==0:
			zeros=zeros+1
		if inT==0:
			zeros=zeros+1

		valuesArray = array([1-rP,1-pP,1-sP,1-pC,1-sC,1-psC,1-psmC,1-phC,1-sA,1-bF,1-inT, zeros, DeltaPos, PosConfidence, Speed, DeltaSpeed, SpeedConfidence, DeltaHeading, HeadingConfidence, DeltaTime])
		targetArray = array([numLabel])
		returnArray = array([valuesArray,targetArray])
		#print valuesArray
		#print "returnArray: " + str(returnArray)
		#returnArray = returnArray.astype(np.float)
		return returnArray

	def get_array(self, id):
		index = self.id_index.index(id)
		return array([self.id_array_x[index][-1],self.id_array_y[index][-1]])

	def get_array_MLP(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = self.id_array_x[index][-batch_size:]

		app_ary = array([[len(list_X)]])
		for i in range(0,len(list_X)-1):
			app_ary = np.append(app_ary.tolist(),[[len(list_X)]],axis=0)
		list_X = np.append(list_X,app_ary,axis=1)
		
		#ret_array = [sum(x) for x in zip(*list_X)]
		ret_array = np.append(list_X[:,:11].sum(axis=0),list_X[-1,11:])
		list_Y = self.id_array_y[index][-batch_size:]
		return array([array(ret_array),list_Y[-1]])

	def get_array_lstm(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = self.id_array_x[index][-batch_size:]
		if len(list_X)<batch_size:
			list_X_Ret = np.zeros(len(list_X[-1]))
			for i in range(0,batch_size-len(list_X) - 1):
				list_X_Ret = np.vstack((list_X_Ret,np.zeros(len(list_X[-1]))))
			list_X_Ret = np.vstack((list_X_Ret,list_X))
			list_X = list_X_Ret

		#nx , ny = np.array(list_X).shape
		#list_X = np.array(list_X).reshape((nx*ny))

		list_Y = self.id_array_y[index][-batch_size:]
		ret_array = array([list_X,list_Y[-1]])
		#print ret_array
		return ret_array

	def get_latest_array(self,id): 
		try:
			index = self.id_index.index(id)
			return self.id_array_x[index][-1]
		except ValueError:
			return 0

	def get_latest_time(self,id): 
		try:
			index = self.id_index.index(id)
			return self.id_time[index][-1]
		except ValueError:
			return 0

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
