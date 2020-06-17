#!/usr/bin/env python
# -*- coding: utf-8 -*- 

"""
/*******************************************************************************
 * @author  Joseph Kamel
 * @email   josephekamel@gmail.com
 * @date	28/11/2018
 * @version 2.0
 *
 * SCA (Secure Cooperative Autonomous systems)
 * Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
 * All rights reserved.
 *******************************************************************************/
"""

from sklearn.neural_network import MLPClassifier
from sklearn.svm import SVC, LinearSVC
from sklearn.linear_model import SGDClassifier, LogisticRegression
from sklearn import datasets
import joblib
from os import listdir
from os.path import isfile, join
import numpy as np
import os
import umap
import matplotlib as mpl
import random 


from MulticoreTSNE import MulticoreTSNE as TSNE

from keras.models import Sequential  
from keras.layers.core import Dense, Activation, Dropout 
from keras.layers import TimeDistributed, CuDNNGRU, Input, Bidirectional, concatenate
from keras.layers.recurrent import LSTM, GRU,SimpleRNN
from keras.layers.normalization import BatchNormalization
from keras.models import Model
from keras.models import load_model
from keras.optimizers import Adam
from keras.callbacks import ReduceLROnPlateau
from keras.utils import to_categorical

from keras.utils import to_categorical
from sklearn.utils.class_weight import compute_class_weight
from sklearn.model_selection import train_test_split

from sklearn.metrics import classification_report
from sklearn.metrics import confusion_matrix
import seaborn as sns
import matplotlib.pyplot as plt
import time
import pandas as pd
import xgboost as xgb
from xgboost import XGBClassifier, XGBRegressor
from xgboost import plot_importance,  plot_tree
from matplotlib import pyplot

from tqdm import tqdm

class MlTrainer:

	AIType = 'NotSet'

	SavedModel = None
	SavedModelSet = False

	saveData = None

	valuesFileStr = 'notSet'
	targetFileStr = 'notSet'
	savePath = ''

	curDateStr = ''
    
	save_y = None
	batch_size = 512
	train_split = 0.8

	cur_index = -1

	NewValuesData = []
	NewTargetData = []


	def setCurDateSrt(self, datastr):
		self.curDateStr = datastr

	def setSavePath(self, datastr):
		self.savePath = datastr

	def setAIType(self, datastr):
		self.AIType = datastr

	def setSavedModel(self, SavedModel):
		self.SavedModel = SavedModel
		self.SavedModelSet = True


	def train(self, data, le):
		data.prepare_arrays()
		self.saveData = data
            
		if(self.AIType == 'COMBINED_LSTM10_DENSE36_DENSE24'):
			print('Training: ' + self.AIType)
			y = data.TargetData
			y = np.reshape(y, (1,np.product(y.shape)))[0]
			d_weights = self.get_d_weights(y,le)
			y = to_categorical(y)

			self.save_y = y

			print(data.ValuesData[0].shape)
			print(data.ValuesData[1].shape)
			print(data.ValuesData[0][0].shape)
			print(data.ValuesData[1][0].shape)
			self.generate_values_for_train_generator_1()

			input_lstm = Input(shape=(None, data.ValuesData[0][0].shape[1]),name='lstm')
			lstm = Bidirectional(LSTM(10,return_sequences=False))(input_lstm)
			#lstm = Activation(activation='relu')(lstm)
			#lstm = (BatchNormalization())(lstm)
			lstm = Dropout(0.3) (lstm)
			#lstm = GlobalMaxPooling1D()(lstm)

			input_mlp = Input(shape=(data.ValuesData[1].shape[1],),name='mlp')
			mlp = (Dense(36))(input_mlp)
			mlp = Activation(activation='relu')(mlp)
			#mlp = (BatchNormalization())(mlp)
			mlp = Dropout(0.3) (mlp)

			combined = concatenate([lstm, mlp])
			combined = Dense(24)(combined)
			combined = Activation(activation='relu')(combined)
			#combined = (BatchNormalization())(combined)
			combined = Dropout(0.3) (combined)
			combined = Dense(y.shape[1], activation='softmax')(combined)

			opt=Adam(lr=0.005)
			reduce_lr = ReduceLROnPlateau(monitor='val_loss', factor=0.7,patience=4, min_lr=0.0005)
			clf = Model(inputs=[input_lstm,input_mlp], outputs=combined)
			clf.compile(loss='categorical_crossentropy', optimizer=opt,metrics=['accuracy'],weighted_metrics=['accuracy'])

			if self.SavedModelSet :
				clf.set_weights(self.SavedModel.get_weights())

			steps_per_epoch_calc = int(self.train_split*len(self.NewValuesData))
			steps_per_epoch_val = len(self.NewValuesData) - steps_per_epoch_calc
			clf.fit_generator(self.train_generator_0(), steps_per_epoch=steps_per_epoch_calc, epochs=10,class_weight=d_weights,verbose=1,callbacks=[reduce_lr], validation_data=self.train_generator_0(),validation_steps= steps_per_epoch_val)
		joblib.dump(clf, self.savePath + '/clf_'+self.AIType + '_'+self.curDateStr+'.pkl')
        
		print("Saved " + self.savePath + '/clf_'+self.AIType + '_'+self.curDateStr+'.pkl')
		

		print("Predicting ...")
		y_test = clf.predict(data.ValuesData,batch_size=16384)
		pred=np.argmax(y_test,axis=1)
		print("Predicting Finished!")
		ground=np.argmax(data.TargetData,axis=1)

		res=classification_report(ground, pred)
		print(le.dict_labels)
		print(res)
		conf_mat = confusion_matrix(ground, pred)
		fig, ax = plt.subplots(figsize=(14,14))
		sns.heatmap(conf_mat, annot=True, fmt='d',xticklabels=le.classes_, yticklabels=le.classes_)
		plt.ylabel('ground truth')
		plt.xlabel('prediction')
		plt.show()
		plt.savefig(self.savePath+'/fig_'+self.AIType + '_'+self.curDateStr+'.png', dpi=fig.dpi)
		plt.clf()


		
	def generate_values_for_train_generator_1(self):

		sequence_length_indexes = {}

		for cur_index in tqdm(range(0,self.saveData.ValuesData[0].shape[0])):
			sequence_length = self.saveData.ValuesData[0][cur_index].shape[0]
			seq_index = 0
			if sequence_length in sequence_length_indexes.keys():
				seq_index = sequence_length_indexes[sequence_length]
				if(len(self.NewValuesData[seq_index][0]) >= self.batch_size):
					self.NewValuesData.append([[],[]])
					self.NewTargetData.append([])
					sequence_length_indexes[sequence_length] = len(self.NewValuesData) - 1
					seq_index = len(self.NewValuesData) - 1
			else:
				self.NewTargetData.append([])
				self.NewValuesData.append([[],[]])
				sequence_length_indexes[sequence_length] = len(self.NewValuesData) - 1
				seq_index = len(self.NewValuesData) - 1

			self.NewValuesData[seq_index][0].append(self.saveData.ValuesData[0][cur_index])
			self.NewValuesData[seq_index][1].append(self.saveData.ValuesData[1][cur_index])
			self.NewTargetData[seq_index].append(self.save_y[cur_index])

		for val_index in tqdm(range(0,len(self.NewValuesData))):
			'''
			if len(self.NewValuesData[val_index][0]) <=self.batch_size:

				temp_index = 0
				max_index = len(self.NewValuesData[val_index][0])

				while len(self.NewValuesData[val_index][0])<self.batch_size:
					self.NewValuesData[val_index][0].append(self.NewValuesData[val_index][0][temp_index])
					self.NewValuesData[val_index][1].append(self.NewValuesData[val_index][1][temp_index])
					self.NewTargetData[val_index].append(self.NewTargetData[val_index][temp_index])
					temp_index = temp_index + 1
					if temp_index >= max_index:
						temp_index = 0
			'''

			self.NewValuesData[val_index][0] = np.array(self.NewValuesData[val_index][0])
			self.NewValuesData[val_index][1] = np.array(self.NewValuesData[val_index][1])
			self.NewTargetData[val_index] = np.array(self.NewTargetData[val_index])

	def train_generator_0(self):

		steps_per_epoch_calc = int(self.train_split*len(self.NewValuesData))
		steps_per_epoch_val = len(self.NewValuesData) - steps_per_epoch_calc
		cur_index_list = list(range(steps_per_epoch_val, len(self.NewValuesData)))
		random.shuffle(cur_index_list)
		cur_index_list_test = list(range(steps_per_epoch_val))
		random.shuffle(cur_index_list_test)
		cur_index_list.extend(cur_index_list_test)

		
		while True:
			if self.cur_index<1:
				self.cur_index = len(self.NewValuesData) - 1
			else:
				self.cur_index = self.cur_index - 1
			local_index = cur_index_list[self.cur_index]
			yield self.NewValuesData[local_index], self.NewTargetData[local_index]

	def get_d_weights(self, y,le):
		y_train_str = le.inverse_transform(y)
		le_classes_remove = []
		for i in range(0,len(le.classes_)):
			if le.classes_[i] not in y_train_str:
				le_classes_remove.append(i)
		le_classes_temp = np.delete(le.classes_, le_classes_remove)
		w = compute_class_weight('balanced', list(le_classes_temp), y_train_str)
		new_w = np.array([])
		int_i = 0
		for i in range(0,len(le.classes_)):
			if le.classes_[i] not in le_classes_temp:
				new_w = np.append(new_w,[1])
			else:
				new_w = np.append(new_w,w[int_i])
				int_i = int_i + 1
		d_weights = dict(enumerate(new_w))
		gen_index = le.transform(['Genuine'])[0]
		d_weights[gen_index] = (len(le.classes_)-1) * d_weights[gen_index]
		print("d_weights" + str(d_weights))
		return d_weights