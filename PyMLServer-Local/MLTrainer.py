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
from sklearn.svm import SVC
from sklearn import datasets
from sklearn.externals import joblib
from os import listdir
from os.path import isfile, join
import numpy as np

from keras.models import Sequential  
from keras.layers.core import Dense, Activation, Dropout  
from keras.layers.recurrent import LSTM

from keras.models import load_model

from keras.utils import to_categorical
from sklearn.utils.class_weight import compute_class_weight

from MLDataCollector import MlDataCollector

class MlTrainer:

	AIType = 'NotSet'

	valuesFileStr = 'notSet'
	targetFileStr = 'notSet'
	savePath = ''

	curDateStr = ''
	def setCurDateSrt(self, datastr):
		self.curDateStr = datastr

	def setSavePath(self, datastr):
		self.savePath = datastr

	def setAIType(self, datastr):
		self.AIType = datastr

	def train(self, dataCollector):

		if(self.AIType == 'SVM'):
			X, y = dataCollector.valuesCollection, dataCollector.targetCollection
			y = to_categorical(y)
			clf = SVC(gamma=0.001, C=100.)
			clf.fit(X, y)

		if(self.AIType == 'MLP_L1N15'):
			X, y = dataCollector.valuesCollection, dataCollector.targetCollection
			y = to_categorical(y)
			clf = MLPClassifier(solver='lbfgs', alpha=1e-5,hidden_layer_sizes=(15,), random_state=1)
			clf.fit(X, y)
		if(self.AIType == 'MLP_L3N25'):
			X, y = dataCollector.valuesCollection, dataCollector.targetCollection
			y = to_categorical(y)
			clf = MLPClassifier(solver='lbfgs', alpha=1e-5,hidden_layer_sizes=(25,25,25,), random_state=1)
			clf.fit(X, y)

		if(self.AIType == 'MLP_L4NV25'):
			X, y = dataCollector.valuesCollection, dataCollector.targetCollection
			y = to_categorical(y)
			clf = MLPClassifier(solver='lbfgs', alpha=1e-5,hidden_layer_sizes=(20,25,22,16,), verbose=True,random_state=1)
			clf.fit(X, y)

		if(self.AIType == 'LSTM'):

			print('Values_LSTM: ' + str(dataCollector.valuesCollection.shape))
			print('Targets_LSTM: ' + str(dataCollector.targetCollection.shape))
			X, y = dataCollector.valuesCollection, dataCollector.targetCollection

			y = np.reshape(y, (1,np.product(y.shape)))[0]

			classes_ = [0.0,1.0]

			le_classes_remove = []
			for i in range(0,len(classes_)):
				if classes_[i] not in y:
					le_classes_remove.append(i)
			le_classes_temp = np.delete(classes_, le_classes_remove)

			w = compute_class_weight('balanced', list(le_classes_temp), y)
			new_w = np.array([])
			int_i = 0
			for i in range(0,len(classes_)):
				if classes_[i] not in le_classes_temp:
					new_w = np.append(new_w,[1])
				else:
					new_w = np.append(new_w,w[int_i])
					int_i = int_i + 1

			d_weights = dict(enumerate(new_w))

			print("d_weights" + str(d_weights))

			y = to_categorical(y)
			clf = Sequential()
			clf.add(LSTM(256, return_sequences=True, input_shape=(X.shape[1], X.shape[2])))
			clf.add(LSTM(256, return_sequences=True))
			clf.add(Dropout(0.2))
			clf.add(BatchNormalization())
			clf.add(LSTM(256, return_sequences=False))
			clf.add(Dense(y.shape[1],activation='softmax'))
			clf.compile(loss='categorical_crossentropy', optimizer='adam',metrics=['accuracy'],weighted_metrics=['accuracy'])
			clf.fit(X, y,epochs=20, batch_size=512,class_weight=d_weights)

		joblib.dump(clf, self.savePath + '/clf_' + self.AIType + '_'+self.curDateStr+'.pkl')