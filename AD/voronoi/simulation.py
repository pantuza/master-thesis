# -*- coding: utf-8 -*-

try:
    from sys import path
    from os.path import abspath

    path.append(abspath("."))
except:
    raise EnvironmentError("Can't append voronoi to python path :/ ")


from random import randint

from pymote.networkgenerator import NetworkGenerator
from pymote.simulation import Simulation
from pymote.npickle import write_npickle

from voronoi import Voronoi


# generates the network with 10 hosts
net_gen = NetworkGenerator(n_count=3, n_min=1, n_max=4)
net = net_gen.generate_random_network()


# Defines the network algorithm
net.algorithms = ((Voronoi, {'informationKey':'axis'}),)


for node in net.nodes():
    node.memory['axis'] = 'x=%d, y=%d' % (randint(1, 100), randint(1, 100))

sim = Simulation(net)
sim.run()


#print net.algorithmState

#for node in net.nodes():
#    print node.id
#    print node.status
#    print node.memory
    #print node.msg_counter

print net.algorithmState

net.show()
write_npickle([path, net], "net.gz")
sim.reset()
#net.show()
