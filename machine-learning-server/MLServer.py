
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
from MLMain import MlMain
from os import listdir
from os.path import isfile, join

version = 'NOVER'

class S(BaseHTTPRequestHandler):
	globalMlMain = MlMain()

	def setup(self):
		BaseHTTPRequestHandler.setup(self)
		self.request.settimeout(0.2)

	def _set_headers(self):
		self.send_response(200)
		self.send_header('Content-type', 'text/html')
		self.end_headers()

	def do_GET(self):
		self.wfile.write("Get Request".encode('utf-8'))

	def do_HEAD(self):
		self._set_headers()

	def do_POST(self):
		'''
		Handle POST requests.
		'''
		#print('The Request: %s' % (self.path))
		#requestStr = urllib2.unquote((self.path));
		#requestStr = unquote(self.path)

		pred = self.globalMlMain.mlMain(version,self.path, 'COMBINED_LSTM10_DENSE36_DENSE24')
		
	   	# the response
		self.wfile.write(pred.encode('utf-8'))



def run(server_class=HTTPServer, handler_class=S, port=9998):
	server_address = ('', port)
	httpd = server_class(server_address, handler_class)
	print('Starting MLServer...')
	print('Listening on port ' + str(port))
	httpd.serve_forever()



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