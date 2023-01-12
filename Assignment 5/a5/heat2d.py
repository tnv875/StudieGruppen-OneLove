#!/usr/bin/env python3

import ctypes
import numpy as np
import matplotlib.pyplot as plt
import sys

# Set this to True to use the C-accelerated implementation.
use_c = True

# Prepare to load C library
heateqclib = None

def use_clib():
    global heateqclib
    heateqclib = ctypes.CDLL('./heat-equation.so')
    heateqclib.compute_delta.restype = ctypes.c_float


def pos(width, x, y):
    return y * width + x


def write_borders(data, width, height):
    # If we have the C library, use that
    if heateqclib is not None:
        heateqclib.write_borders(data.ctypes.data_as(ctypes.c_void_p), width, height)
        return

    # Write borders
    for n in range(0, width):
        data[pos(width, n,0)] = 20.0
        data[pos(width, n,height-1)] = -273.15
    for n in range(0, height):
        data[pos(width, 0,n)] = -273.15
        data[pos(width, width-1,n)] = -273.15


# Compute the new value for the given position
def stencil(data, width, x, y, alpha = 0.2):
    return alpha * (data[pos(width, x,y)] + data[pos(width, x-1,y)] + data[pos(width, x+1,y)] + data[pos(width, x,y-1)] + data[pos(width, x,y+1)])


# Runs a single simulated timestep
def apply_stencil(data, width, height, offset, alpha = 0.2):
    if heateqclib is not None:
        heateqclib.apply_stencil(data.ctypes.data_as(ctypes.c_void_p), width, height, offset, ctypes.c_float(alpha))
        return

    for x in range(1, width-1):
        for y in range(1 + ((x+offset)%2), height-1, 2):
            data[pos(width, x, y)] = stencil(data, width, x, y, alpha)

# Helper method to create visualization of the steps
def plot_image(data, width, height, iteration):
    plt.imshow(np.reshape(data, (width, height)), interpolation="none")
    plt.savefig("plots/fig-%sx%s-step%s.pdf" % (width, height, iteration))


# Computes the average elementwise difference between two arrays
# Same as:
#   np.sum(np.abs(prev-data)) / size
def compute_delta(data, prev, width, height):
    if heateqclib is not None:
        return heateqclib.compute_delta(data.ctypes.data_as(ctypes.c_void_p), prev.ctypes.data_as(ctypes.c_void_p), width, height)

    res = 0.0
    for x in range(0, width):
        for y in range(0, height):
            res += abs(prev[pos(width, x, y)]-data[pos(width, x, y)])

    return res / (width*height)


# Runs the simulation for a number of steps
def run_simulation(width, height, steps):
    size = width*height
    # Create empty buffer for computation and reference
    data = np.zeros(size, dtype=np.float32)
    prev = np.zeros(size, dtype=np.float32)

    # Prepare the buffer
    write_borders(data, width, height)
    #plot_image(data, width, height, 0)

    # Declare variable for use outside the loop
    delta = 0.0
    n = 0

    for n in range(0, steps):
        # Copy all items from data into prev
        # prev[:] = data
        np.copyto(prev, data)
        apply_stencil(data, width, height, n % 2)
        # Compute the elementwise difference, and sum the differences
        delta = compute_delta(data, prev, width, height)
        # Check the delta
        if delta < 0.001:
            break

    # Report the value
    print("After %d iterations, delta was: %f" % (n+1, delta))

    # Make 2D for plotting
    plt.imshow(np.reshape(data, (width, height)), interpolation="none")
    plt.show()

if __name__ == "__main__":
    if use_c:
        use_clib()

    n = 50 if len(sys.argv) < 2 else int(sys.argv[1])
    m = 50 if len(sys.argv) < 3 else int(sys.argv[2])
    s = 100 if len(sys.argv) < 4 else int(sys.argv[3])

    run_simulation(n, m, s)
