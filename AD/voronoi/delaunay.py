'''
Created on 22/09/2013

@author: Gustavo Pantuza
'''
from point2D import Point2D;
from triangle import Triangle;
from sys import maxint

class Delaunay(object):
    '''
    Delaunay Triangulation
    ref: http://www.cs.cornell.edu/home/chew/Delaunay.html
    '''


    def __init__(self, inf = maxint):
        
        try:
            self._initTriangle = Triangle(Point2D(-inf, -inf), 
                                          Point2D(inf, -inf), 
                                          Point2D(0, inf))
            self._triangles = {self._initTriangle: {}}
        except TypeError:
            import ipdb
            ipdb.set_trace()
        

    def __repr__(self):
        return 'Triangulation with ' + str(len(self._triangles)) + ' triangles'

    def __str__(self):
        return self.__repr__() 

    def add(self, site):
        triangle = self._getTriangleOn(site)
        if not triangle.contains(site):
            cavity = self._getCavity(site, triangle)
            self._insert(site, cavity)
            return True
        return False
    
    def _getTriangleOn(self, site):
        '''
        Search the triangle that match the point
        '''
        for triangle in self._triangles:
            if triangle.circumcircle(site):
                return triangle
        raise Exception("Site out of the valid area.")
    
    def _getCavity(self, site, triangle):
        '''
        Determine the cavity caused by site.
        '''
        encroached = {}
        toBeChecked = [triangle]
        marked = {triangle: None}
        while (len(toBeChecked) > 0):
            triangle = toBeChecked.pop()
            # Site outside triangle => triangle not in cavity
            if not triangle.circumcircle(site):
                continue
            encroached[triangle] = None
            # Check the neighbors
            for neighbor in self._triangles[triangle]:
                if neighbor not in marked:
                    marked[neighbor] = None
                    toBeChecked.append(neighbor)
        return encroached

    def _addTriangle(self, t1, t2):
        if t1 not in self._triangles:
            self._triangles[t1] = {t2}
        else:
            self._triangles[t1][t2] = None

    def _delTriangle(self, triangle):
        del self._triangles[triangle]
        for neighbor in self._triangles.values():
            if triangle in neighbor:
                del neighbor[triangle]

    def _linkTriangles(self, t1, t2):
        self._addTriangle(t1, t2)
        self._addTriangle(t2, t1)

    def _insert(self, site, cavity):
        '''
        Update the triangulation by removing the cavity triangles and then
        filling the cavity with new triangles.
        '''
        boundary = {}
        triangles = {}

        # Find boundary facets and adjacent triangles
        for triangle in cavity:
            for neighbor in self._triangles[triangle]:
                # Adj triangles only
                if neighbor not in cavity:
                    triangles[neighbor] = None
            for point in triangle:
                facet = triangle.getOppositeFacet(point);
                if facet in boundary:
                    del boundary[facet]
                else:
                    boundary[facet] = None

        # Remove the cavity triangles from the triangulation
        for triangle in cavity:
            self._delTriangle(triangle)

        # Build each new triangle and add it to the triangulation
        newTriangles = {}
        for facet in boundary.keys():
            newTriangle = Triangle(facet[0], facet[1], site)
            self._triangles[newTriangle] = {}
            newTriangles[newTriangle] = None
            # Adj triangle + new triangles
            triangles[newTriangle] = None

        # Update the graph links for each new triangle
        for triangle in newTriangles:
            for other in triangles:
                if triangle.isNeighbor(other):
                    self._linkTriangles(triangle, other)

    def neighborOpposite(self, site, triangle):
        '''
        Report neighbor opposite the given vertex of triangle.
        '''
        if site not in triangle:
            raise Exception("Vertex not in triangle.")
        for neighbor in self._triangles[triangle]:
            if not neighbor.contains(site):
                return neighbor
        return None

    def surroundingTriangles(self, site, triangle):
        '''
        Report triangles surrounding site in order (cw or ccw).
        '''
        if site not in triangle:
            raise Exception("Site not in triangle.")
        triangles = []
        start = triangle
        # cw or cww (pay attention)
        guide = triangle.getNextVertexExcept([site])
        while True:
            triangles.append(triangle)
            previous = triangle
            triangle = self.neighborOpposite(guide, triangle)
            guide = previous.getNextVertexExcept([site, guide])
            if (triangle == start):
                break
        return triangles

if __name__ == '__main__':

    v = Delaunay()
    sites= [Point2D(100, 100),
            Point2D(150, 200),
            Point2D(300, 150),
            Point2D(250, 200)]
    for s in sites:
        v.add(s)
    print "DelaunayTriangulation created: Triangulation with " + \
        str(len(v._triangles)) + " triangles"
    c = 0
    for t in v._triangles:
        c += 1
        print str(c) + ":" + str(t)
        for n in v._triangles[t]:
            print "--> " + str(n)
    '''
    v2 = Delaunay(10)
    v2.add(Point2D(0,0))
    v2.add(Point2D(1,0))
    v2.add(Point2D(0,1))
    print "DelaunayTriangulation created: Triangulation with " + \
        str(len(v2._triangles)) + " triangles"
    c = 0
    for t in v2._triangles:
        c += 1
        print str(c) + ":" + str(t)
    '''
        
