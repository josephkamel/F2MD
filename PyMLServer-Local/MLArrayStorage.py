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
import math
import itertools
import operator
from sys import getsizeof


APPEND_ZEROS = False

class MlArrayStorage:

	def __init__(self, *args, **kwargs):
		self.id_index = []
		self.id_time = []
		self.id_bsm = []
		self.id_array_x = []
		self.id_array_y = []

	def add_bsm(self, id, time, cur_bsm, batch_size,cur_label):
		if id in self.id_index:
			index = self.id_index.index(id)
			cur_array = [self.get_bsm_array(cur_bsm,self.id_bsm[index]),cur_label]
		else:
			cur_array = [self.get_bsm_array(cur_bsm,cur_bsm),cur_label]
		self.add_array(id,time,cur_array,cur_bsm,batch_size)

	def filter_array(self, curtime ,deltatime):
		for i in range(len(self.id_index)-1,-1,-1):
			if (curtime - self.id_time[i][-1]) > deltatime:
				del self.id_index[i]
				del self.id_bsm[i]
				for j in range(len(self.id_time[i])-1,-1,-1):
					del self.id_time[i][j]
					del self.id_array_x[i][j]
					del self.id_array_y[i][j]
				del self.id_time[i]
				del self.id_array_x[i]
				del self.id_array_y[i]

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
		bF = bsmNew['BsmPrint']['BsmCheck']['bF']
		kPACS = bsmNew['BsmPrint']['BsmCheck']['kPACS']
		kPCS = bsmNew['BsmPrint']['BsmCheck']['kPCS']
		kPSCP = bsmNew['BsmPrint']['BsmCheck']['kPSCP']
		kPSCS = bsmNew['BsmPrint']['BsmCheck']['kPSCS']
		kPSCSP = bsmNew['BsmPrint']['BsmCheck']['kPSCSP']
		kPSCSS = bsmNew['BsmPrint']['BsmCheck']['kPSCSS']
		kSCC = bsmNew['BsmPrint']['BsmCheck']['kSCC']
		inT = 1
		for x in bsmNew['BsmPrint']['BsmCheck']['inT']:
			if inT>x['uVal']:
				inT = x['uVal']

		DeltaPos = self.get_distance(bsmNew['BsmPrint']['BSMs'][0]['Pos'], bsmOld['BsmPrint']['BSMs'][0]['Pos'])
		PosConfidence = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['PosConfidence'])
		Speed = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['Speed'])
		PosX = bsmNew['BsmPrint']['BSMs'][0]['Pos'][0]
		PosY = bsmNew['BsmPrint']['BSMs'][0]['Pos'][1]
		Heading = self.get_angle(bsmNew['BsmPrint']['BSMs'][0]['Heading'])
		DeltaSpeed = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['Speed']) - self.get_speed(bsmOld['BsmPrint']['BSMs'][0]['Speed'])
		SpeedConfidence = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['SpeedConfidence'])
		Accel = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['Accel'])
		DeltaAccel = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['Accel']) - self.get_speed(bsmOld['BsmPrint']['BSMs'][0]['Accel'])
		AccelConfidence = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['AccelConfidence'])
		DeltaHeading = self.get_angle_delta(bsmNew['BsmPrint']['BSMs'][0]['Heading'], bsmOld['BsmPrint']['BSMs'][0]['Heading'])
		HeadingConfidence = self.get_speed(bsmNew['BsmPrint']['BSMs'][0]['HeadingConfidence'])
		DeltaTime = bsmNew['BsmPrint']['Metadata']['generationTime'] - bsmOld['BsmPrint']['Metadata']['generationTime']

		if DeltaTime<0:
			print('Error')

		velAry1 = np.array([1.0-rP,1.0-pP,1.0-sP,1.0-pC,1.0-sC,1.0-psC,1.0-psmC,1.0-phC,1.0-sA,1.0-bF,1.0-kPACS,1.0-kPCS,1.0-kPSCP,1.0-kPSCS,1.0-kPSCSP,1.0-kPSCSS,1.0-kSCC,1.0-inT],dtype=np.float32)
		nzeros = np.count_nonzero(velAry1 == 1.0)
		#velAry2 = [zeros,DeltaPos, PosConfidence, Speed, DeltaSpeed, SpeedConfidence,Accel,DeltaAccel,AccelConfidence, DeltaHeading, HeadingConfidence, DeltaTime]
		
		velAry2 = [nzeros,DeltaPos, DeltaSpeed, DeltaAccel, DeltaHeading, DeltaTime]#24
		velAry3 = [PosX, PosY, Speed, Accel, Heading]#29

		#velAry2 = [nzeros,DeltaPos, DeltaPos, DeltaSpeed, Accel, DeltaAccel, DeltaHeading, DeltaTime]#24
		#velAry3 = []#29

		velAry = velAry1.tolist() + velAry2 + velAry3
		valuesArray = np.array(velAry,dtype=np.float32)

		return valuesArray

	def get_array(self, id):
		index = self.id_index.index(id)
		return [self.id_array_x[index][-1][:24],self.id_array_y[index][-1]]
    
	def get_array_features(self, id):
		index = self.id_index.index(id)
		return [self.id_array_x[index][-1][:18],self.id_array_y[index][-1]]

	def get_ave_array_features(self, array_x):
		#npsum=np.sum(array_x[0:18], axis=0)
		npmean=np.mean(array_x[0:18], axis=0)
		#npstd=np.std(array_x[0:18], axis=0)
		npmin=np.min(array_x[0:18], axis=0)
		#npmax=np.max(array_x[0:18], axis=0)
		newArray = []
		for i in range(0,18):
			#newArray.append(npsum[i])
			newArray.append(npmin[i])
			#newArray.append(npmax[i])
			newArray.append(npmean[i])
			#newArray.append(npstd[i])
		return np.array(newArray,dtype=np.float32)

	def get_array_MLP_features(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = self.id_array_x[index][-batch_size:]
		ret_array = self.get_ave_array_features(list_X)
		return [ret_array,self.id_array_y[index][-1]]
    
	def get_ave_array(self, array_x):
		#npsum=np.sum(array_x, axis=0)
		npmean=np.mean(array_x, axis=0)
		#npstd=np.std(array_x, axis=0)
		npmin=np.min(array_x, axis=0)
		#npmax=np.max(array_x, axis=0)
		newArray = []
		for i in range(0,18):
			#newArray.append(npsum[i])
			newArray.append(npmin[i])
			#newArray.append(npmax[i])
			newArray.append(npmean[i])
			#newArray.append(npstd[i])
		newArray.append(len(array_x))
		newArray.extend(array_x[-1][18:24])
		return np.array(newArray,dtype=np.float32)

	def get_array_MLP(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = self.id_array_x[index][-batch_size:]
		ret_array = self.get_ave_array(list_X)
		return [ret_array,self.id_array_y[index][-1]]

	def get_array_lstm(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = self.id_array_x[index][-batch_size:]
		list_X =np.array(list_X)[:,-5:]
		if APPEND_ZEROS:
			if len(list_X)<batch_size:
				list_X_Ret = np.zeros(len(list_X[-1]),dtype=np.float32)
				for i in range(0,batch_size-len(list_X) - 1):
					list_X_Ret = np.vstack((list_X_Ret,np.zeros(len(list_X[-1]),dtype=np.float32)))
				list_X_Ret = np.vstack((list_X_Ret,list_X))
				list_X = list_X_Ret
		#nx , ny = np.array(list_X).shape
		#list_X = np.array(list_X).reshape((nx*ny))
		list_Y = self.id_array_y[index][-batch_size:]
		ret_array = [list_X,list_Y[-1]]
		#print ret_array
		return ret_array
    
	def get_array_lstm_feat(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = self.id_array_x[index][-batch_size:]
		list_X =np.array(list_X)[:,:18]
		if APPEND_ZEROS:
			if len(list_X)<batch_size:
				list_X_Ret = np.zeros(len(list_X[-1]),dtype=np.float32)
				for i in range(0,batch_size-len(list_X) - 1):
					list_X_Ret = np.vstack((list_X_Ret,np.zeros(len(list_X[-1]),dtype=np.float32)))
				list_X_Ret = np.vstack((list_X_Ret,list_X))
				list_X = list_X_Ret
		#nx , ny = np.array(list_X).shape
		#list_X = np.array(list_X).reshape((nx*ny))
		list_Y = self.id_array_y[index][-batch_size:]
		ret_array = [list_X,list_Y[-1]]
		#print ret_array
		return ret_array
    
	def get_array_lstm_sin(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = self.id_array_x[index][-batch_size:]
		list_X =np.array(list_X)[:,:24]
		if APPEND_ZEROS:
			if len(list_X)<batch_size:
				list_X_Ret = np.zeros(len(list_X[-1]),dtype=np.float32)
				for i in range(0,batch_size-len(list_X) - 1):
					list_X_Ret = np.vstack((list_X_Ret,np.zeros(len(list_X[-1]),dtype=np.float32)))
				list_X_Ret = np.vstack((list_X_Ret,list_X))
				list_X = list_X_Ret
		#nx , ny = np.array(list_X).shape
		#list_X = np.array(list_X).reshape((nx*ny))
		list_Y = self.id_array_y[index][-batch_size:]
		ret_array = [list_X,list_Y[-1]]
		#print ret_array
		return ret_array
    
	def get_array_lstm_all(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = self.id_array_x[index][-batch_size:]
		if APPEND_ZEROS:
			if len(list_X)<batch_size:
				list_X_Ret = np.zeros(len(list_X[-1]),dtype=np.float32)
				for i in range(0,batch_size-len(list_X) - 1):
					list_X_Ret = np.vstack((list_X_Ret,np.zeros(len(list_X[-1]),dtype=np.float32)))
				list_X_Ret = np.vstack((list_X_Ret,list_X))
				list_X = list_X_Ret
		#nx , ny = np.array(list_X).shape
		#list_X = np.array(list_X).reshape((nx*ny))
		list_Y = self.id_array_y[index][-batch_size:]
		ret_array = [list_X,list_Y[-1]]
		#print ret_array
		return ret_array
    
    
	def get_array_lstm_mix(self, id, batch_size):
		index = self.id_index.index(id)
		list_X = self.id_array_x[index][-batch_size:]
		list_X =np.array(list_X)[:,-10:]
		if APPEND_ZEROS:
			if len(list_X)<batch_size:
				list_X_Ret = np.zeros(len(list_X[-1]),dtype=np.float32)
				for i in range(0,batch_size-len(list_X) - 1):
					list_X_Ret = np.vstack((list_X_Ret,np.zeros(len(list_X[-1]),dtype=np.float32)))
				list_X_Ret = np.vstack((list_X_Ret,list_X))
				list_X = list_X_Ret
		#nx , ny = np.array(list_X).shape
		#list_X = np.array(list_X).reshape((nx*ny))
		list_Y = self.id_array_y[index][-batch_size:]
		ret_array = [list_X,list_Y[-1]]
		#print ret_array
		return ret_array

	def get_array_combined(self, id, batch_size):
		index = self.id_index.index(id)
		list_X_Orig = self.id_array_x[index][-batch_size:]
		list_Y = self.id_array_y[index][-batch_size:]
		list_X =np.array(list_X_Orig)[:,-10:]
		if APPEND_ZEROS:
			if len(list_X)<batch_size:
				list_X_Ret = np.zeros(len(list_X[-1]),dtype=np.float32)
				for i in range(0,batch_size-len(list_X) - 1):
					list_X_Ret = np.vstack((list_X_Ret,np.zeros(len(list_X[-1]),dtype=np.float32)))
				list_X_Ret = np.vstack((list_X_Ret,list_X))
				list_X = list_X_Ret
		list_X_1 = self.get_ave_array_features(list_X_Orig)
		return [[list_X,list_X_1],list_Y[-1]]
    
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
    
	def get_angle(self,angl2):
		deltaX = angl2[0] - 1;
		deltaY = angl2[1];
		rad = math.atan2(deltaY, deltaX);
		deg = rad * (180 / math.pi)
		return deg
    
	def get_angle_delta(self,angl1,angl2):
		deltaX = angl2[0] - angl1[0];
		deltaY = angl2[1] - angl1[1];
		rad = math.atan2(deltaY, deltaX);
		deg = rad * (180 / math.pi)
		return deg
