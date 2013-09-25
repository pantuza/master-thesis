# -*- coding: utf-8 -*-

try:
    from sys import path
    from os.path import abspath

    path.append(abspath("."))
except:
    raise EnvironmentError("Can't append voronoi on python path :/ ")


from pymote.networkgenerator import NetworkGenerator
from pymote.simulation import Simulation
from pymote.npickle import write_npickle

#from scipy.spatial import voronoi_plot_2d
import matplotlib.pyplot as plt

from distributed_voronoi import DistributedVoronoi
from point2D import Point2D


# generates the network with 10 hosts
net_gen = NetworkGenerator(n_count=6, n_min=1, n_max=7)
net = net_gen.generate_random_network()


# Defines the network algorithm
net.algorithms = ((DistributedVoronoi, {'informationKey':'axis'}),)


# Assign to node memory its position
for node in net.nodes():
    node.memory['axis'] = Point2D(int(net.pos[node][0]), int(net.pos[node][1]))

# Creates and starts the simulation
sim = Simulation(net)
sim.run()

# Show the State of the Voronoi Algorith execution
print net.algorithmState


# Plot voronoi diagram for each node
for node in net.nodes():
    
    try:
        #voronoi_plot_2d(node.voronoi)
        #plt.show(block=False)
        Preview.draw(node.voronoi, width = 200, height = 200)
    except AttributeError:
        print "%s Insufficient number of nodes to compute voronoi" % node

# Plot the network image
net.show()
# Writes
#write_npickle(net, "net.gz")
sim.reset()
