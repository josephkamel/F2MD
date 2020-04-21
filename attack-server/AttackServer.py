try:
	import SocketServer as socketserver
	from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler
except ImportError:
	import socketserver
	from http.server import HTTPServer, BaseHTTPRequestHandler

from AttackMain import AttackMain
from os import listdir
from os.path import isfile, join
from datetime import datetime

version = 'NOVER'

class S(BaseHTTPRequestHandler):
	globalAttackMain = AttackMain()

	def setup(self):
		self.globalAttackMain.setup()
		BaseHTTPRequestHandler.setup(self)
		self.request.settimeout(0.5)

	def _set_headers(self):
		self.send_response(500)
		self.send_header('Content-type', 'text/html')
		self.end_headers()

	def do_GET(self):
		self._set_headers()
		self.wfile.write("test".encode('utf-8'))

	def do_HEAD(self):
		self._set_headers()

	def do_POST(self):
		'''
		Handle POST requests.
		'''
		#print('The Request: %s' % (self.path))

		#requestStr = urllib2.unquote((self.path));
		#print self.path
		pred = self.globalAttackMain.collect_data()

		self.wfile.write(pred.encode('utf-8'))

	def load_binary(self,file):
		with open(file, 'rb') as file:
			return file.read()


def run(server_class=HTTPServer, handler_class=S, port=9998):
	server_address = ('', port)
	httpd = server_class(server_address, handler_class)
	print('Starting AttackServer...')
	print('Listening on port ' + str(port))
	httpd.serve_forever()

if __name__ == "__main__":
	from sys import argv

	if len(argv) == 2:
		run(port=int(argv[1]))
	else:
		print('not enough argv')
