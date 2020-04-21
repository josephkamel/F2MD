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

try:
    import SocketServer as socketserver
    from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler
except ImportError:
    import socketserver
    from http.server import HTTPServer, BaseHTTPRequestHandler
from MAMain import MaMain
from os import listdir
from os.path import isfile, join

from datetime import datetime

from threading import Timer
import time

version = 'NOVER'

class S(BaseHTTPRequestHandler):
	globalMaMain = MaMain()

	def setup(self):
		BaseHTTPRequestHandler.setup(self)
		self.request.settimeout(0.2)

	def _set_headers(self):
		self.send_response(200)
		self.send_header('Content-type', 'text/html')
		self.end_headers()

	def do_GET(self):
		self._set_headers()
		#self.globalMaMain.set_httpstring()
		self.globalMaMain.pingma()
		self.wfile.write(self.globalMaMain.get_httpstring().encode('utf-8'))			

	def do_HEAD(self):
		self._set_headers()
		
	def do_POST(self):
		'''
		Handle POST requests.
		'''
		#requestStr = urllib2.unquote((self.path));
		if not self.path.startswith('curTime:'):
			pred = self.globalMaMain.maMain(version, self.path)
			self.wfile.write(pred.encode('utf-8'))

		# the response

	def load_binary(self,file):
		with open(file, 'rb') as file:
			return file.read()

def run(server_class=HTTPServer, handler_class=S, port=9998):
	server_address = ('', port)
	httpd = server_class(server_address, handler_class)
	print('Starting MAServer...')
	print('Listening on port ' + str(port))
	httpd.serve_forever()

# duration is in seconds

if __name__ == "__main__":
	from sys import argv
	
	if len(argv) == 3:
		version = str(argv[2])
		run(port=int(argv[1]))
	elif len(argv) == 2:
		version = "V1"
		run(port=int(argv[1]))
	else:
		print('not enough argv')

