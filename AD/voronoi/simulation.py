# -*- coding: utf-8 -*-

from pymote.networkgenerator import NetworkGenerator
from pymote.simulation import Simulation
from pymote.npickle import write_npickle
from pymote.conf import global_settings

from distributed_voronoi import DistributedVoronoi
from point2D import Point2D
from voronoi import VoronoiDiagram


VoronoiDiagram.start()
global_settings.ENVIRONMENT2D_SHAPE = VoronoiDiagram.panel_dim()


# generates the network with 10 hosts
net_gen = NetworkGenerator(n_count=14, n_min=1, n_max=15)
net = net_gen.generate_random_network()


# Defines the network algorithm
net.algorithms = ((DistributedVoronoi, {'informationKey':'axis'}),)


# Assign to node memory its position
for node in net.nodes():
    node.memory['axis'] = (int(net.pos[node][0]), int(net.pos[node][1]))


# Creates and starts the simulation
sim = Simulation(net)
sim.run()

# Show the State of the Voronoi Algorith execution
#print net.algorithmState


# Plot voronoi diagram for each node
for node in net.nodes():
    
    try:
        while VoronoiDiagram.preview.step(): pass
    except AttributeError:
        print "%s Insufficient number of nodes to compute voronoi" % node


VoronoiDiagram.stop()
# Writes
#write_npickle(net, "net.gz")
sim.reset()
