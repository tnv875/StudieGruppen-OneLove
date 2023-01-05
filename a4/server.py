
#!/bin/python3
import datetime
import locale
import math
import os
import re
import socketserver
import struct

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

        self.status = STATUS_OK
        self.message = ""
        
        try:
            # Read message
            bytes_message: bytes = self.request.recv(MSG_MAX)
            string_message: str = bytes_message.decode('utf-8')
            split_message = string_message.split(sep="\r\n")

            # Get request_lines using custom function
            self.method, self.url, self.protocol = self._get_request_lines()

            # Get header_lines using custom function
            header_lines, entity_body_i = self._get_header_lines()
            
            # TODO: Handle individual headers
            header_response = self.handle_headers(header_lines)

            # Define final lines as entity_body
            entity_body = split_message[entity_body_i:]

            # TODO: Handle entity_body if entity_body is not empty.

            # Build and send HTTP_response
            if self.method == "GET":
                self._handle_GET()
            self._send_response()
        
 
        # Always generate a response, this is the fallback for if all other
        # validation and handling fails. This is acceptable as a last resort,
        # but were possible more helpful feedback and responses should be 
        # generated.
        except Exception as e:
            self.status = STATUS_OTHER        

    # TODO: Might need more asserts
    def _get_request_lines(self, split_message):
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

        # - url exists
        if not os.path.exists(url):
            self.status = 400
            self.handle_error()
            return

        # - protocol is HTTP/1.1
        if protocol != "HTTP/1.1":
            self.status = 400
            self.handle_error()
            return
        
        return method, url, protocol


    def _get_header_lines(self, split_message):
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
        
        # TODO: Update handle_*() methods to correspond to individual headers. Check capitalization of header fields.
        if "host" not in header_dict:
            self.handle_error(STATUS_BAD_REQUEST, f"Missing a Host header field")
        else:
            self.handle_Host(header_dict.get("host"))
        if "accept" in header_dict:
            self.handle_accept(header_dict.get("accept"))
        if "accept-encoding" in header_dict:
            self.handle_Host(header_dict.get("host"))
        if "Connection" in header_dict:
            self.handle_Connection(header_dict.get("Connection"))
        if "If-Modified-Since" in header_dict:
            self.handle_If_Modified_Since(header_dict.get("If-Modified-Since"))
        if "if-Unmodified-since" in header_dict:
            self.handle_If_Unmodified_Since(header_dict.get("If-Unmodified-Since"))
        if "user-agent" in header_dict:
            self.handle_user_agent(header_dict.get("user-agent"))


    def handle_Host(self, host: str):
        """
        Custom function to handle header Host
        """
        # TODO: Could be upgraded to support servers that are not hosted locally
        LOCAL_HOST = "127.0.0.1"
        if host != LOCAL_HOST:
            self.status = 400
            self.handle_error()


    # TODO: Handle Accept
    def handle_accept(self, accept: str):
        """
        Method to handle accept in header. Assumes an accept header is present in the file
        """

        # From docs:
        '''
        If an Accept header field is present,
        and if the server cannot send a response which is acceptable
        according to the combined Accept field value, then the server SHOULD
        send a 406 (not acceptable) response.'''

        types = accept.split(sep= ", ")

        types_dict = {}
        for type_ in types:
            key, value = type_.split(sep=";", maxsplit=1)
            types_dict.update({key: value})

        types_subtypes = {"*": "*", "image": ["jpeg", "png", "svg+xml"], "text": ["html", "plain", "csv", "css"], "application": ["xhtml+xml", "xml"]}
        accepted_list = []
        for type_subtype in types_dict.keys():
            MIME_type, MIME_subtype = type_subtype.split(sep="/")
            if MIME_type in types_subtypes:
                if MIME_subtype in types_subtypes[MIME_type]:
                    #Make a list of all matches --> Sorted List
                    accepted_list.append(MIME_type+"/"+MIME_subtype)
                else:
                    next
            else:
                next
        if accepted_list == []:
            self.status = 406
            self._handle_error()
        else:
            final_dict = {}
            for elem in accepted_list:
                value = types_dict[elem].split(sep="=")[1]
                final_dict.update({elem: value})
            sorted_final_list_tuples = sorted(final_dict.items(), key= lambda x:x[1], reverse=True)
            
            # Search for file ending in sorted order from URL and self 
            for best_option in sorted_final_list_tuples:
                if os.path.exists(self.url + "." + best_option[0]):
                    self.url = self.url+"."+best_option[0]
                else:
                     next
            return

    # TODO: Might be expanded to support any comma-separated list of HTTP headers
    def handle_Connection(self, Connection):
        """
        Custom function to handle Connection header type.
        Sets self.server_should_close to True. Caller should use this
        variable to close the connection after response has been processed.
        """
        self.Connection = Connection


    def handle_If_Modified_Since(self, If_Modified_Since: str):
        """
        Custom function to handle If-Modified-Since header type.
        If succesful sets self.status to 200. This should be included
        in response to client.
        """
        last_modified_secs = os.path.getmtime(self.url)
        last_modified_date = datetime.fromtimestamp(last_modified_secs)
        condition_date = datetime.strptime(If_Modified_Since, '%a, %d %b %Y %H:%M:%S GMT')

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
        condition_date = datetime.strptime(If_Unmodified_Since, '%a, %d %b %Y %H:%M:%S GMT')

        # If modification date is later than condition
        if last_modified_date > condition_date:
            self.status = 412
            self.handle_error()
            return
        else:
            self.status = 200 # OK


    def handle_user_agent(self, user_agent: str):
        """
        Method for handleing user-agent header type.
        Will always succeed.
        """
        #user_agent is nearly always  == "Mozilla/5.0" at least for all testcases, and as such we will always return success.
        return


    def _humanStatus(self):
        """
        Get human readable status message from statuscode 
        """
        status_messages = {
            "200": "OK",
            "301": "Moved Permanently",
            "304": "Not Modified",
            "404": "Not found",
            "406": "Not Acceptable",
            "412": "Modified"
        }
        return status_messages[str(self.status)]

    def _handle_GET(self, request:bytes) -> None:
        """
        Function to handle a 'get file' type request.
        request(bytes): The request message body

        A data file is read and sent back to the requestee. A response is 
        always generated, either the data file or an error message explaining 
        what went wrong.
        """

        # Get file data as bytes
        with open(self.url) as requested_file:
            data = requested_file.read()

        statusline = self._statusline()

        # TODO: figure out the headers to include in response (pass is a placeholder)
        self.response_headers_list = []
        response_headers = '\r\n'.join(self.response_headers_list)

        self.message = '\r\n'.join([statusline, response_headers, '', data])

        # Send a response
        print(f'Sending requested data from {self.url}')
        self._send_response()
        return

    def _statusline(self):
        return f"HTTP/1.1 {self.status} {self._humanStatus(self.status)}"

    def _send_response(self):
        bytes_message = bytes(self.message, 'utf-8')
        self.request.sendall(bytes_message)

    def _handle_error(self):
        self.message = self._statusline() + "\r\n\r\n"
        self._send_response()
        return

if __name__ == "__main__":
    configs = {
        "server_ip": "127.0.0.1",
        "server_port": 12345,
    }    
    
    # This check is not a formal requirement of the system but should let you 
    # know if you've started your server in the wrong place. Do feel free to 
    # change or remove this if you are wanting to use different data files
    for f in ["tiny.txt", "hamlet.txt"]:
        if not os.path.isfile(f): 
            print(f"Server can't find expected data file {f}. Make sure you " + 
                  "are starting the server with the files next to it in the " + 
                  "directory")
            exit(1)

    print(f"Starting server at: {configs['server_ip']}:"
        f"{configs['server_port']}")
    with HTTPServer(configs, RequestHandler) as exercise_server:
        try:
            exercise_server.serve_forever()
        finally:
            exercise_server.server_close()
