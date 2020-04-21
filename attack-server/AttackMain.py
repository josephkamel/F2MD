import os
import sys
from os import listdir
from os.path import isfile, join
import datetime
import warnings
from tqdm import tqdm
import time, threading
import tkinter as tk
from tkinter import *
from functools import partial
from math import sqrt


class AttackMain:

	all_labels = [ "ConstPos", "ConstPosOffset", "RandomPos", "RandomPosOffset",
			"ConstSpeed", "ConstSpeedOffset", "RandomSpeed", "RandomSpeedOffset","StaleMessages",
			"EventualStop", "Disruptive", "DataReplay",
			"DoS", "DoSRandom", "DoSDisruptive",
			"GridSybil", "DataReplaySybil", "DoSRandomSybil",
			"DoSDisruptiveSybil"]

	thread_wait_time = 0.1
	initiated = False

	attack_list = []
	inst_list = []
	instanttk = True

	def setup(self):
		if not self.initiated:
			print('AttackServer Starting ...')
			self.tkinter_thread()
			self.parsing_thread()
			self.initiated = True
			print('AttackServer Started!')

	def collect_data(self):
		return_str = ""
		for i in range(0,len(self.attack_list)):
			return_str = return_str + self.inst_list[i] + ";"
			return_str = return_str + self.attack_list[i] + ";"
		self.attack_list = []
		self.inst_list = []
		return return_str

	def tkinter_thread(self):
		if self.initiated:
			root = tk.Tk()
			root.title("Attack Server")
			label = tk.Label(root, fg="dark green",font=('Gotham', '12'))
			label.config(text="Choose Attack:")
			label.grid(row=0, column=0)
			#label.pack()

			midnum = sqrt(len(self.all_labels))

			button = tk.Button(root, text="instant",font=('Gotham', '12'), width=24, command=self.inverse_instanttk)
			button.grid(row=0, column=1)

			label2 = tk.Label(root, fg="dark green",font=('Gotham', '12'))
			label2.config(text="Instant: " + str(self.instanttk))
			label2.grid(row=0, column=2)
			self.update_label(label2)

			if (midnum-int(midnum))<0.5:
				maxrow = int(midnum) 
				maxcol = int(midnum) + 1
			else:
				maxrow = int(midnum) + 1
				maxcol = int(midnum) + 1

			for i in range(0,len(self.all_labels)):
				action_with_arg = partial(self.add_attack, self.all_labels[i])
				button = tk.Button(root, text=self.all_labels[i],font=('Gotham', '12'), width=24, command=action_with_arg)
				button.grid(row=int(1+i/maxrow), column=int(i%maxrow))
				#button.pack()

			
			root.mainloop()
			
		threading.Timer(self.thread_wait_time, self.tkinter_thread).start()

	def parsing_thread(self):
		if self.initiated:
			txt = input("Attack Name: ")
			textlist = txt.split(',')
			if len(textlist)<=1:
				if textlist[0] in self.all_labels:
					self.attack_list.append(textlist[0])
					self.inst_list.append("no")
			elif len(textlist)==2:
				if textlist[0] in self.all_labels:
					self.attack_list.append(textlist[0])
					if textlist[1] == 'i':
						self.inst_list.append('instant')
					else:
						self.inst_list.append('no')
			elif len(textlist)==3:
				if textlist[0] in self.all_labels:
					for j in range(0,int(textlist[2])):
						self.attack_list.append(textlist[0])
						if textlist[1] == 'i':
							self.inst_list.append('instant')
						else:
							self.inst_list.append('no')

		#print(time.ctime())
		threading.Timer(self.thread_wait_time, self.parsing_thread).start()

	def inverse_instanttk(self):
		if self.instanttk:
			self.instanttk = False
		else:
			self.instanttk = True

	def add_attack(self,strattack):
		self.attack_list.append(strattack)
		if self.instanttk:
			self.inst_list.append("instant")
		else:
			self.inst_list.append("no")

	def update_label(self,label):
		def update():
			label.config(text="Instant: " + str(self.instanttk))
			label.after(10, update)
		update()
