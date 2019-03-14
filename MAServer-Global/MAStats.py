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

class MaStats:

	trueDetection = 0
	falseDetection = 0
	detectionRate = 0

	trueDetectionAgr = 0
	falseDetectionAgr = 0
	detectionRateAgr = 0

	trueDetection_inst = 0
	falseDetection_inst = 0

	trueDetectionAgr_inst = 0
	falseDetectionAgr_inst = 0

	detectionRate_cur = 0
	detectionRateAgr_cur = 0

	def update_detection(self,pred_str,real_str):
		if  pred_str == real_str:
			self.trueDetection = self.trueDetection + 1
		else:
			self.falseDetection = self.falseDetection + 1
		self.detectionRate = 100*self.trueDetection/(self.trueDetection+self.falseDetection)
		#print (pred_str + '/' + real_str)
		#print(str(self.trueDetection) + '/' + str(self.falseDetection) +' '+ str(self.detectionRate) + '%')

	def update_detection_agr(self,pred_str,real_str):
		if  pred_str == real_str:
			self.trueDetectionAgr = self.trueDetectionAgr + 1
		else:
			self.falseDetectionAgr = self.falseDetectionAgr + 1
		self.detectionRateAgr = 100*self.trueDetectionAgr/(self.trueDetectionAgr+self.falseDetectionAgr)
		#print (pred_str + '/' + real_str)
		#print (str(self.trueDetectionAgr) + '/' + str(self.falseDetectionAgr) +' '+ str(self.detectionRateAgr) + '%') 

	def get_detectionRate(self):
		return int(self.detectionRate)

	def get_detectionRateAgr(self):
		return self.detectionRateAgr

	def get_detectionInfo_inst(self):
		trueDetection_cur = self.trueDetection - self.trueDetection_inst
		falseDetection_cur = self.falseDetection - self.falseDetection_inst
		if (falseDetection_cur + trueDetection_cur) > 0:
			self.detectionRate_cur = 100*trueDetection_cur/(trueDetection_cur+falseDetection_cur)
		self.trueDetection_inst = self.trueDetection
		self.falseDetection_inst = self.falseDetection
		return [trueDetection_cur,falseDetection_cur,self.detectionRate_cur]

	def get_detectionInfoAgr_inst(self):
		trueDetection_cur = self.trueDetectionAgr - self.trueDetectionAgr_inst
		falseDetection_cur = self.falseDetectionAgr - self.falseDetectionAgr_inst
		if (falseDetection_cur + trueDetection_cur) > 0:
			self.detectionRateAgr_cur = 100*trueDetection_cur/(trueDetection_cur+falseDetection_cur)
		self.trueDetectionAgr_inst = self.trueDetectionAgr
		self.falseDetectionAgr_inst = self.falseDetectionAgr
		return [trueDetection_cur,falseDetection_cur,self.detectionRateAgr_cur]