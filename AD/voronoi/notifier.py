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

    def initiator(self, node, message):
        pass
        print "init node: %s" % node
        print message
        if message.header == NodeAlgorithm.INI:

            # Send to every neighbor
            msg = Message(header='Information',
                          data=node.memory[self.informationKey])
            node.send(msg)
            node.status = 'DONE'

    def idle(self, node, message):
        pass
        print "init node: %s" % node
        print message
        if message.header == 'Information':

            node.memory[self.informationKey] = message.data
            destination_nodes = list(node.memory[self.neighborsKey])
            # send to every neighbor-sender
            destination_nodes.remove(message.source)

            if destination_nodes:
                msg = Message(destination=destination_nodes,
                              header='Information',
                              data=message.data)
                node.send(msg)
        node.status = 'DONE'

    def done(self, node, message):
        pass
