
#!/bin/python3
import locale
import math
import os
import re
import socketserver
import struct
from datetime import datetime

# This is quick hack to get relative imports of a higher file working 
if __package__ is None:
    import sys
    sys.path.append(
        os.path.dirname(os.path.dirname( os.path.abspath(__file__)))
    )

from shared import *


class HTTPServer(socketserver.ThreadingTCPServer):
    def __init__(self, configs: dict, 
            request_handler_class: socketserver.StreamRequestHandler):
        """
        Constructor for HTTPServer.

        configs(dict): Configuration dictionary of for the server. Must include
            entires for 'server_ip', and 'server_port'.
        request_handler_class(socketserver.StreamRequestHandler): A handler to
            reespond to any messages sent to this server.

        This server inherits from the socketserver.ThreadingTCPServer. It makes
        no changes and is just here as a reference
        """
        super().__init__((configs['server_ip'], configs['server_port']), 
            request_handler_class)


class RequestHandler(socketserver.StreamRequestHandler):
    """
    Custom handler to handle any inbound messages. Any input message will 
    ALWAYS receive an appropriate response.
    """
        

    def handle(self):
        """
        Function to initially handle any requests received. 
        
        This will read a request, perform some curseory validation before 
        calling more specific handling functions. Nothing is returned.
        """

        try:
            self.status = 200 # OK
            self.response_headers = []
            self.message = ""
            self.data = ""
            self.connection = ""
            self.encoding = ""

            # Add date and server IP to Response headers
            self.gen_date()
            self.gen_server()

            # Read message
            bytes_message: bytes = self.request.recv(256*256)
            string_message: str = bytes_message.decode('utf-8')
            split_message = string_message.split(sep="\r\n")

            # Decompose into method, url and protocol using custom function
            self.method, self.url, self.protocol = self.get_request_lines(split_message)
            self.url = '.' + self.url

            # Get header_lines using custom function
            header_lines, entity_body_i = self.get_header_lines(split_message)
            
            # Handle headers
            self.handle_headers(header_lines)

            entity_body = split_message[entity_body_i:]

            # Build and send HTTP_response
            if self.method == "GET":
                self.handle_GET()

                print(f'Sending requested data from {self.url}')
                self.build_and_send_response()

            elif self.method == "Head":
                self.build_and_send_response(should_send_data=False)

            else:
                self.handle_error()

            print('Made it to the very end! <3\n')

 
        # Always generate a response, this is the fallback for if all other
        # validation and handling fails. This is acceptable as a last resort,
        # but were possible more helpful feedback and responses should be 
        # generated.
        except Exception as e:
            self.status = 400
            self.handle_error()        


    def gen_date(self):
        self._date_format = '%a, %d %b %Y %H:%M:%S GMT'
        now = datetime.now()
        date = now.strftime(self._date_format)
        self.response_headers.append(f'Date: {date}')


    def gen_server(self):
        self.ip = '127.0.0.1'
        self.response_headers.append(f'Server: {self.ip}')


    def get_request_lines(self, split_message):
        """
        Custom function to get request_lines from split_message. 
        Asserts that method, url and protocol are supported.
        """
        request_lines = split_message[0]

        # Decompose into method, url and protocol using .split() to split by space character
        method, url, protocol = request_lines.split()

        # ASSERTS
        # - method is supported
        if method not in ["GET", "HEAD"]:
            self.status = 400
            self.handle_error()
            return

        # - protocol is HTTP/1.1
        if protocol != "HTTP/1.1":
            self.status = 505
            self.handle_error()
            return
        
        return method, url, protocol


    def get_header_lines(self, split_message):
        """
        Custom function to get header_lines from split_message.
        Returns tuple containing header_lines and i. i is used by caller
        for getting entity_body if it exists.
        """
        header_lines = []
        i = 1
        while split_message[i] != "":
            header_lines.append(split_message[i])
            i += 1
        return (header_lines,i)


    def handle_headers(self, header_lines):
        header_dict = {}
        for element in range(0, len(header_lines)):
            name, value = header_lines[element].split(sep=": ")
            header_dict.update({name: value})
        
        if "Host" not in header_dict:
            self.status = 400
            self.handle_error()
            print('Handled Host error')
        else:
            self.handle_Host(header_dict.get("Host"))
            print('Handled Host')

        if "Accept" in header_dict:
            try:
                self.handle_Accept(header_dict.get("Accept"))
                print('Handled Accept')
            except Exception as e:
                print('Could not handle Accept')
           
        if "Accept-Encoding" in header_dict:
            try:
                self.handle_Accept_Encoding(header_dict.get("Accept-Encoding"))
            except Exception as e:
                print('Could not handle Accept-Encoding')

        if "Connection" in header_dict:
            self.handle_Connection(header_dict.get("Connection"))
            print('Handled Connection')

        # Note: This header is currently not included in test client request,
        # thus it is correct to not handle right now.
        if "If-Modified-Since" in header_dict:
            try:
                self.handle_If_Modified_Since(header_dict.get("If-Modified-Since"))
                print('Handled If-Modified-Since')
            except Exception as e:
                print('Could not handle If-Modified-Since')

         # Note: This header is currently not included in test client request,
        # thus it is correct to not handle right now.
        if "if-Unmodified-since" in header_dict:
            self.handle_If_Unmodified_Since(header_dict.get("If-Unmodified-Since"))
            print('Handled If-Unmodified-Since')
        
        if "User-Agent" in header_dict:
            self.handle_User_Agent(header_dict.get("User-Agent"))
            print('Handled User-agent')


    def handle_Host(self, host: str):
        """
        Custom function to handle header Host
        """
        # TODO: Could be upgraded to support servers that are not hosted locally
        if host != self.ip:
            self.status = 400
            self.handle_error()


    # TODO: Handle Accept
    def handle_Accept(self, accept: str):
        """
        Method to handle accept in header. Assumes an accept header is present in the file
        """

        # IF URL end with a dash, this is handled another place
        if self.url[-1] == "\\":
            return

        types = accept.split(sep= ", ")

        types_dict = {}
        for type_ in types:
            try:
                if ";" in type_:
                    key, value = type_.split(sep=";", maxsplit=1)
                    variable, value = value.split(sep="=", maxsplit=1)
                    types_dict.update({key: value})
                else:
                    types_dict.update({type_: "1.1"})
            except:
                next

        types_subtypes = {"*": "*", "image": ["jpeg", "jpg", "png", "svg+xml"], "text": ["html", "plain", "csv", "css"], "application": ["xhtml+xml", "xml"]}
        accepted_list = []
        for type_subtype in types_dict.keys():
            MIME_type, MIME_subtype = type_subtype.split(sep="/")
            if MIME_type in types_subtypes:
                if MIME_subtype in types_subtypes[MIME_type] or MIME_type == "*":
                    #Make a list of all matches --> Sorted List
                    accepted_list.append(MIME_type+"/"+MIME_subtype)
                else:
                    next
            else:
                next
        if accepted_list == []:
            self.status = 406
            self.handle_error()
        else:
            final_dict = {}
            for elem in accepted_list:
                value = types_dict[elem]
                final_dict.update({elem: value})
            sorted_final_list_tuples = sorted(final_dict.items(), key= lambda x:x[1], reverse=True)
            
            # Search for file ending in sorted order from URL and self 
            for best_option in sorted_final_list_tuples:
                if best_option[0].split(sep="/")[1] =="*":
                    if best_option[0].split(sep="/")[0] == "*":
                        for type in types_subtypes:
                            for elem in	types_subtypes[type]:
                                if os.path.exists(self.url + "." + elem):
                                    MIME_type = type
                                    MIME_subtype = best_option[0].split(sep="/")[1]
                                    self.response_headers.append(f"Content-Type: {MIME_type}/{MIME_subtype}")
                                    self.url = self.url + "." + elem
                                    return
                    else:
                        for elem in	types_subtypes[best_option[0].split(sep="/")[0]]:
                            if os.path.exists(self.url + "." + elem):
                                MIME_type = elem
                                MIME_subtype = best_option[0].split(sep="/")[1]
                                self.response_headers.append(f"Content-Type: {MIME_type}/{MIME_subtype}")
                                self.url = self.url + "." + best_option[0].split(sep="/")[1]
                                return		
                elif os.path.exists(self.url + "." + best_option[0].split(sep="/")[1]):
                    for type in types_subtypes:
                            for elem in	types_subtypes[type]:
                                if elem == best_option[0].split(sep="/")[1]:
                                    MIME_type = type
                                    break
                    MIME_subtype = best_option[0].split(sep="/")[1]
                    self.response_headers.append(f"Content-Type: {MIME_type}/{MIME_subtype}")
                    self.url = self.url + "." + best_option[0].split(sep="/")[1]
                    return
                else:
                    next

    def handle_Accept_Encoding(self, Accept_Encoding: str):
        """
        Custom function to handle Accept_Encoding header type.
        Is only called if header lines include Accept_Encoding
        Our implementation only supports gzip.
        """

        accepted = Accept_Encoding.split(', ')
        if 'gzip' in accepted:
            self.encoding = 'gzip'
        else:
            self.status = 406 # Encoding not acceptable
            self.handle_error()
        return

    def handle_Connection(self, Connection):
        """
        Custom function to handle Connection header type.
        Sets self.server_should_close to True. Caller should use this
        variable to close the connection after response has been processed.
        """
        self.connection = Connection
        
        self.response_headers.append(f'Connection: {Connection}')


    def handle_If_Modified_Since(self, If_Modified_Since: str):
        """
        Custom function to handle If-Modified-Since header type.
        If succesful sets self.status to 200. This should be included
        in response to client.
        """
        last_modified_secs = os.path.getmtime(self.url)
        last_modified_date = datetime.fromtimestamp(last_modified_secs)
        condition_date = datetime.strptime(If_Modified_Since, self._date_format)

        # If modification date is older than condition
        if last_modified_date < condition_date:
            self.status = 304
            self.handle_error()
            return
            
        else:
            self.status = 200 # OK


    def handle_If_Unmodified_Since(self, If_Unmodified_Since: str):
        """
        Method for handleing If_Unmodified_Since header type.
        Will succeed if the recourse has not been modified since the date specified in the HTTP request. 
        If it has changed, the response will be a 412 precondition failed error.
        """
        last_modified_secs = os.path.getmtime(self.url)
        last_modified_date = datetime.fromtimestamp(last_modified_secs)
        condition_date = datetime.strptime(If_Unmodified_Since, self._date_format)

        # If modification date is older than condition
        if last_modified_date > condition_date:
            self.status = 412
            self.handle_error()
            return
            
        else:
            self.status = 200 # OK


    def handle_User_Agent(self, user_agent: str):
        """
        Method for handleing user-agent header type.
        Will always succeed.
        """
        #user_agent is nearly always  == "Mozilla/5.0" at least for all testcases, and as such we will always return success.
        return


    def human_status(self):
        """
        Get human readable status message from statuscode 
        """
        status_messages = {
              4: "Other",
            200: "OK",
            301: "Moved Permanently",
            304: "Not Modified",
            400: "Bad Request",
            404: "Not Found",
            406: "Not Acceptable",
            412: "Modified",
            505: "HTTP Version Not Supported"
        }

        if self.status in status_messages:
            return status_messages[self.status]
        else:
            return "Unknown status code"

    def handle_GET(self) -> None:
        """
        Function to handle a 'GET' type request.

        A data file is read and sent back to the requestee. A response is 
        always generated, either the data file or an error message explaining 
        what went wrong.
        """
        if self.url[-1] == '/':
            # If folder is requested, return index.html if it exists
            if os.path.exists(self.url + 'index.html'):
                self.url += 'index.html'
                self.response_headers.append('Content-Type: text/html')

            # Otherwise list contents of requested folder 
            # and skip trying to read non-existant file
            else:
                self.data = '\n'.join(os.listdir())
                self.response_headers.append('Content-Type: text/plain')
                return
        try:                    
            with open(self.url, 'rb') as requested_file: 
                self.data = requested_file.read()
                if type(self.data) != bytes:
                    bytes(self.data, 'utf-8')
            print(f'Succesfully managed to read file at {self.url}')
        except:
            print('Could not read url')
            self.status = 400
            self.handle_error()
        return

    def gen_statusline(self):
        return f"HTTP/1.1 {self.status} {self.human_status()}"

    def build_and_send_response(self, should_send_data = True):
        statusline = bytes(self.gen_statusline(), 'utf-8')

        if should_send_data:
            if self.encoding == "gzip":
                import gzip
                self.data = gzip.compress(self.data)
                self.response_headers.append("Content-Encoding: gzip")

            # Content length response header
            content_length = len(self.data)
            if content_length != 0:
                self.response_headers.append(f'Content-Length: {content_length}')

        else:
            self.data = b""

        headers = bytes('\r\n'.join(self.response_headers), 'utf-8')
        self.message = b'\r\n'.join([statusline, headers, b'', self.data])

        self.request.sendall(self.message)

    def handle_error(self):
        self.build_and_send_response(should_send_data=False)
        print(f"Handled error: {self.status, self.human_status()}")
        sys.exit()

if __name__ == "__main__":
    configs = {
        "server_ip": "127.0.0.1",
        "server_port": 12345,
    }    
    
    print(f"Starting server at: {configs['server_ip']}:"
        f"{configs['server_port']}")
    with HTTPServer(configs, RequestHandler) as exercise_server:
        try:
            exercise_server.serve_forever()
        finally:
            print('Closing server')
            exercise_server.server_close()
