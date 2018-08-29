from sklearn import svm
from sklearn import datasets
from sklearn.externals import joblib
from os import listdir
from os.path import isfile, join
import numpy as np

class MlTrainer:
	valuesFileStr = 'notSet'
	targetFileStr = 'notSet'
	trainPath = ''

	valuesCollection = np.array([])
	targetCollection = np.array([])

	curDateStr = ''
	def setCurDateSrt(self, datastr):
		self.curDateStr = datastr

	def setTrainPath(self, datastr):
		self.trainPath = datastr

	def train(self):
		self.loadData()
		X, y = valuesCollection, targetCollection
		clf = svm.SVC(gamma=0.001, C=100.)
		clf.fit(X, y)
		joblib.dump(clf, trainPath + '/clf_'+curDateStr+'.pkl')

	def loadData(self):
		self.setFileNames()
		self.valuesCollection = np.load(trainPath + '/' +self.valuesFileStr)
		self.targetCollection = np.load(trainPath + '/' +self.targetFileStr)

	def setFileNames(self):
		filesNames = [f for f in listdir(self.trainPath) if isfile(join(self.trainPath, f))]
		for s in filesNames:
			if s.endswith(".npy"):
				if s.startswith("valuesSave_") and self.valuesFileStr == 'notSet' :
					self.targetFileStr = s
				if s.startswith("targetSave_") and self.targetFileStr == 'notSet' :
					self.targetFileStr = s

# load dataset
# digits = datasets.load_digits()
# X, y = digits.data, digits.target

# # setup classifier
# clf = svm.SVC(gamma=0.001, C=100.)

# # train classifier
# clf.fit(X, y)

# # save classifier into a file
# # os.chdir('../mlClassifier')
# print '========================================='
# #print os.path.abspath(os.curdir)
# os.chdir("/home/joseph/Projects/F2MD/VeinsMDV2/omnetpp-workspace/veins-veins-4.6/src/veins/modules/application/misbehaviorDetection/mdChecks/")
# print '========================================='
# joblib.dump(clf, 'mlClassifiers/clf.pkl')

