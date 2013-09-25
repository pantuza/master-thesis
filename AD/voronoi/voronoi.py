'''
Created on 24/09/2013

@author: Gustavo Pantuza
'''

from delaunay import Delaunay

class Voronoi(Delaunay):
    '''
    Voronoi Diagram
    '''

    def __init__(self, *args):
        Delaunay.__init__(self, *args)

    def getCells(self):
        cells = {}
        ignore = {x:None for x in self._initTriangle}
        for triangle in self._triangles.keys():
            for site in triangle:
                if site in ignore:
                    continue;
                ignore[site] = None
                surrounding = self.surroundingTriangles(site, triangle)
                cell = []
                for tri in surrounding:
                    cell.append(tri.getCircumcenter())
                cells[site] = cell 
        return cells

