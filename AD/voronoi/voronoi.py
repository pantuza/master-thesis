# -*- coding: utf-8 -*-


from pymote.message import Message
from pymote.sensor import NeighborsSensor
from pymote.algorithm import NodeAlgorithm
from random import randint

from scipy.spatial import Voronoi as SciVoronoi
from notifier import Notifier



class Voronoi(Notifier):
    """ Implements a distributed algorithm for solving the 
    voronoi diagram problem. 
    """

    def initializer(self):
        
        for node in self.network.nodes():
            node.memory['neighbors_data'] = {}
            node.points = [[int(node.network.pos[node][0]),
                    int(node.network.pos[node][1])]]
        super(Voronoi, self).initializer()

    def step(self, node):
        msg = node.receive()
        
        if msg:

            print msg.data

            if msg.source is None or msg.source != node:
                node.memory['neighbors_data'][msg.source.id] = msg.data
                node.points.append(msg.data)
            print node.memory['neighbors_data']

            # Minimun points in SciVoronoi to construct initial simplex
            if len(node.points) == 4:
                node.voronoi = SciVoronoi(node.points, incremental=True)
            
            elif len(node.points) > 4:
                node.voronoi.add_points([msg.data])

