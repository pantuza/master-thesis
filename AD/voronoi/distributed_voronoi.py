# -*- coding: utf-8 -*-


from pymote.message import Message
from pymote.sensor import NeighborsSensor
from pymote.algorithm import NodeAlgorithm

#from scipy.spatial import Voronoi as SciVoronoi
from notifier import Notifier
from delaunay import Delaunay
from point2D import Point2D
from voronoi import VoronoiDiagram


class DistributedVoronoi(Notifier):
    """ Implements a distributed algorithm for solving the
    voronoi diagram problem.
    """

    def initializer(self):

        for node in self.network.nodes():
            node.memory['neighbors_data'] = {}
            node.points = [(int(node.network.pos[node][0]),
                            int(node.network.pos[node][1]))]
            node.voronoi = Delaunay()
            VoronoiDiagram.new_diagram(node.voronoi, str(node.id))
            node.voronoi.include(node.points[0])

        super(DistributedVoronoi, self).initializer()

    def step(self, node):
        msg = node.receive()

        try:

            if msg is not None and msg.source != node:

                node.memory['neighbors_data'][msg.source.id] = msg.data
                node.points.append(msg.data)

                # adds new point and recalculates voronoi
                node.voronoi.include(msg.data)

                # notifies the node neighbors about the message received
                self.notify(node, msg.source, msg.data)

        except Exception as e:
            raise Exception("Erro during processing message from node %s"
                            % node)

    def notify(self, node, source, data):

        # Starts communication through neighbors nodes
        for neighbor in node.memory[self.neighborsKey]:
            msg = Message(header=NodeAlgorithm.INI,
                          source=source,
                          destination=neighbor,
                          data=data)
            node.send(msg)
