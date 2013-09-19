# -*- coding: utf-8 -*-


from pymote.message import Message
from pymote.sensor import NeighborsSensor
from pymote.algorithm import NodeAlgorithm
from random import randint

from notifier import Notifier


class Voronoi(Notifier):
    """ Implements a distributed algorithm for solving the 
    voronoi diagram problem. 
    """

    def initializer(self):
        
        for node in self.network.nodes():
            node.memory['neighbors_data'] = {}
        super(Voronoi, self).initializer()

    def step(self, node):
        msg = node.receive()
        
        if msg:

            print msg.data
            
            if msg.source is None or msg.source != node:
                node.memory['neighbors_data'][msg.source] = msg.data
            print node.memory['neighbors_data']
