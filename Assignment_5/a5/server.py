#!/usr/bin/env python3

from http.server import *
import png
import numpy as np
import io
import base64
import colormap

import importlib
heat2d = importlib.import_module("heat2d")

# The size of the simulated grid.
width, height = (200, 200)

# The number of simulation steps to take per rendered frame.  Increase
# this to spend more time on compute and less time on graphics.  You
# should definitely make this 10 or 50 or some similar number once you
# have a parallel C implementation.
steps_per_frame = 1

# Set this to True to use the C-accelerated implementation.
use_c = False

class Simulation:
    def __init__(self, width=500, height=500):
        self.width = width
        self.height = height
        self.iteration = 0

        if use_c:
            heat2d.use_clib()

        self.size = self.width * self.height
        self.data = np.zeros(self.size, dtype=np.float32)
        heat2d.write_borders(self.data, self.width, self.height)

    def step(self):
        heat2d.apply_stencil(self.data, self.width, self.height, self.iteration % 2)
        self.iteration += 1

    def render(self):
        span = np.max(self.data) - np.min(self.data)
        scale = np.uint8((np.reshape(self.data, (self.width, self.height))-np.min(self.data))/span*255)
        return colormap.colormap[scale]

instance = Simulation(width, height)

class RequestHandler(SimpleHTTPRequestHandler):
    def __init__(self, request, client_address, server):
        self.pngwriter = png.Writer(width=instance.width, height=instance.height, alpha=False)
        super().__init__(request, client_address, server)

    def do_POST(self):
        for i in range(steps_per_frame):
            instance.step()
        img = instance.render()

        self.send_response(200)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Content-type','image/png')
        self.end_headers()
        out = io.BytesIO()
        self.pngwriter.write(out, img)
        self.wfile.write(base64.b64encode(out.getvalue()))

def run_server(server_address=('', 8000)):
    httpd = HTTPServer(server_address, RequestHandler)
    httpd.serve_forever()

if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='Visualisation server')
    parser.add_argument('--host', metavar='HOST', default='',
                        help='The hostname to listen from.')
    parser.add_argument('--port', metavar='PORT', type=int, default=8080,
                        help='The port to listen from.')
    args = parser.parse_args()

    run_server(server_address=(args.host, args.port))
