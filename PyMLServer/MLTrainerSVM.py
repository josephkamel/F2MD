from sklearn import svm
from sklearn import datasets
from sklearn.externals import joblib
from os import listdir
from os.path import isfile, join
import numpy as np

class MlTrainer:
	valuesFileStr = 'notSet'
	targetFileStr = 'notSet'
	savePath = ''

	valuesCollection = np.array([])
	targetCollection = np.array([])

	curDateStr = ''
	def setCurDateSrt(self, datastr):
		self.curDateStr = datastr

	def setSavePath(self, datastr):
		self.savePath = datastr

	def setValuesCollection(self, datacol):
		self.valuesCollection = datacol

	def setTargetCollection(self, datacol):
		self.targetCollection = datacol

	def train(self):
		X, y = self.valuesCollection, self.targetCollection
		clf = svm.SVC(gamma=0.001, C=100.)
		clf.fit(X, y)
		joblib.dump(clf, self.savePath + '/clf_'+self.curDateStr+'.pkl')

	def loadData(self):
		self.valuesCollection = np.load(self.savePath + '/' +self.valuesFileStr)
		self.targetCollection = np.load(self.savePath + '/' +self.targetFileStr)

	def setFileNames(self):
		filesNames = [f for f in listdir(self.savePath) if isfile(join(self.savePath, f))]
		for s in filesNames:
			if s.endswith(".npy"):
				if s.startswith("valuesSave_") and self.valuesFileStr == 'notSet' :
					self.targetFileStr = s
				if s.startswith("targetSave_") and self.targetFileStr == 'notSet' :
					self.targetFileStr = s
