from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
import SocketServer
import urllib2
from MAMain import MaMain
from os import listdir
from os.path import isfile, join

from datetime import datetime

version = 'NOVER'

class S(BaseHTTPRequestHandler):
	globalMaMain = MaMain()

	def _set_headers(self):
		self.send_response(200)
		self.send_header('Content-type', 'text/html')
		self.end_headers()

	def do_GET(self):
			self._set_headers()
			#self.globalMaMain.set_httpstring()
			self.wfile.write(self.globalMaMain.get_httpstring())


	def do_HEAD(self):
		self._set_headers()
		
	def do_POST(self):
		'''
		Handle POST requests.
		'''
		#print('The Request: %s' % (self.path))
		requestStr = urllib2.unquote((self.path));
		pred = self.globalMaMain.maMain(version, requestStr)

	   	# the response
		self.wfile.write(pred)

	def load_binary(self,file):
		with open(file, 'rb') as file:
			return file.read()




def run(server_class=HTTPServer, handler_class=S, port=9998):
	server_address = ('', port)
	httpd = server_class(server_address, handler_class)
	print 'Starting MAServer...'
	print 'Listening on port ' + str(port)
	httpd.serve_forever()

if __name__ == "__main__":
	from sys import argv
	
	if len(argv) == 3:
		version = str(argv[2])
		run(port=int(argv[1]))
	else:
		print 'not enough argv'
