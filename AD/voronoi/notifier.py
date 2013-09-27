# -*- coding: utf-8 -*-

from pymote.algorithm import NodeAlgorithm
from pymote.message import Message


class Notifier(NodeAlgorithm):
    """ It uses NodeAlgorithm class to make the communication between hosts """

    STATUS = {'INITIATOR': 'initiator',
              'IDLE': 'idle',
              'DONE': 'done'}

    required_params = ('informationKey',)
    default_params = {'neighborsKey': 'Neighbors'}

    def initializer(self):

        for node in self.network.nodes():

            # Fills node neighbors
            node.memory[self.neighborsKey] = \
                node.compositeSensor.read()['Neighbors']
            node.status = 'IDLE'

            # Starts communication through neighbors nodes
            for neighbor in node.memory[self.neighborsKey]:
                msg = Message(header=NodeAlgorithm.INI,
                              source=node,
                              destination=neighbor,
                              data=node.memory['axis'])
                node.send(msg)
