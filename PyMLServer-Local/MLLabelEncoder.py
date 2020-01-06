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
from sklearn.preprocessing import LabelEncoder

class MlLabelEncoder:

	def __init__(self, *args, **kwargs):
		self.dict_indexs = {}
		self.dict_labels = {}
		self.classes_ =[]

	def fit(self, labels):
		sklearn_le = LabelEncoder()
		sklearn_le.fit(labels)
		indexs = sklearn_le.transform(labels)
		self.classes_ = sklearn_le.classes_
		for i in range(0,len(indexs)):
			self.dict_labels[indexs[i]] = labels[i]
			self.dict_indexs[labels[i]] = indexs[i]

	def transform(self, label):
		return [self.dict_indexs[x] for x in label]

	def inverse_transform(self, i):
		return [self.dict_labels[x] for x in i]

	def transform_single(self, label):
		return self.dict_indexs[label]

	def inverse_transform_single(self, i):
		return self.dict_labels[i]

	def transform_h(self, label):
		if isinstance(label, (list)):
			return self.transform(label)
		else:
			return self.transform_single(label)

	def inverse_transform_h(self, i):
		if isinstance(i, (list)):
			return self.inverse_transform(i)
		else:
			return self.inverse_transform_single(i)

