# -*- coding: utf-8 -*-


from pymote.message import Message
from pymote.sensor import NeighborsSensor
from pymote.algorithm import NodeAlgorithm

#from scipy.spatial import Voronoi as SciVoronoi
from notifier import Notifier
from voronoi import Voronoi


class DistributedVoronoi(Notifier):
    """ Implements a distributed algorithm for solving the
    voronoi diagram problem.
    """

    def initializer(self):

        for node in self.network.nodes():
            node.memory['neighbors_data'] = {}
            node.points = [[int(node.network.pos[node][0]),
                            int(node.network.pos[node][1])]]
            node.voronoi = Voronoi(node.points)
        super(Voronoi, self).initializer()

    def step(self, node):
        msg = node.receive()

        try:

            if msg.source is None or msg.source != node:
                node.memory['neighbors_data'][msg.source.id] = msg.data
                node.points.append(msg.data)

                # adds new point and recalculates voronoi
                node.voronoi.add(msg.data)
        except:
            raise Exception("Erro during processing message from node %s" 
                            % node)
