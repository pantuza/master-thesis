# -*- coding: utf-8 -*-


from pymote.algorithms.broadcast import Flood
from pymote.message import Message
from pymote.sensor import NeighborsSensor
from random import randint

class Voronoi(Flood):
    """ Implements a distributed algorithm for solving the 
    voronoi diagram problem. It uses Flood class to make the 
    communication between hosts
    """

    msg_counter = 0

    def initializer(self):

        super(Voronoi, self).initializer()

    def step(self, node):
        msg = node.receive()
        
        if msg:
            print node
            print node.outbox
            print node.memory
            print node.status
            print msg
            if msg.destination.id in node.memory['Neighbors']:
                super(Voronoi, self).step(node)
                self.msg_counter += 1
        print self.msg_counter
        
