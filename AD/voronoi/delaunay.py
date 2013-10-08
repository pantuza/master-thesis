'''
Created on 22/09/2013

@author: Gustavo Pantuza
'''
from __future__ import division
from point2D import Point2D
from triangle import Triangle
from sys import maxint
from numpy.linalg import det
import threading;

class Delaunay(object):
    '''
    Delaunay Triangulation
    ref: http://www.cs.cornell.edu/home/chew/Delaunay.html
    '''

    # INT => MAX=+(pow(2,31) - 1)(2147483647) & MIN=-pow(2,31)(-2147483648)
    # inf = 1000000 was empirically limited due to pygame simulator
    # inf = 2^16 was empirically limited due to float point precision

    def __init__(self, inf = 65536, test = False):
        self._initTriangle = Triangle(Point2D(-inf, -inf), 
                                      Point2D(inf, -inf), 
                                      Point2D(0, inf))
        self.neighborhood = {self._initTriangle: {}}
        self.lock = threading.RLock()
        self.main_site = None
        self.fail_site = None
        self.fail_triangle = None
        self.fail_witness = None
        self.version = 0
        self.check_triangulation = test

    def __repr__(self):
        return 'Triangulation with ' + str(len(self.neighborhood)) + \
            ' triangles'

    def __str__(self):
        return self.__repr__() 

    def _new_version(self):
        self.version += 1
        # cicle version
        if self.version > 2147483640:
            self.version = 0

    def updated_since(self, version):
        return (version != self.version)

    def has_error(self):
        return self.fail_site is not None

    def include(self, site):
        '''
        Add a site to triangulation
        '''
        if self.fail_site is not None:
            return False
        with self.lock:
            triangle = self._find_triangle_circumscribe(site)
            return self._add(site, triangle)
    
    def remove_far_sites(self, from_site):
        '''
        Remove all sites that do not affect a "from site" cell
        Returns the number of removed sites
        '''
        if self.fail_site is not None:
            return False
        with self.lock:
            from_triangle = self._find_triangle_by_vertex(from_site)
            if (from_triangle is None):
                return 0
            triangles, points = \
                    self.surrounding_triangles(from_site, from_triangle)
            # search irrlevant sites
            dispensables = []
            for triangle in self.neighborhood.keys():
                if (not triangle in triangles):
                    dispensable = True
                    for vertex in triangle:
                        if not vertex in points:
                            dispensables.append(vertex)
            # remove irrlevant sites
            removed = 0
            for site in dispensables:
                if not self._initTriangle.contains(site):
                    self.remove(site)
                    removed += 1
            return removed

    def include_near(self, site, from_site):
        '''
        Add a site only if it will affect a "from site" cell
        '''
        if self.fail_site is not None:
            return False
        with self.lock:
            triangle = self._find_triangle_by_vertex(from_site)
            if (triangle is None):
                raise Exception("'From site' don't found.")
            triangles, points = \
                    self.surrounding_triangles(from_site, triangle)
            # verify site influence
            for triangle in triangles:
                if triangle.circumscribe(site):
                    return self._add(site, triangle)
            return False

    def remove(self, site):
        '''
        Remove a site from triagulation
        '''
        if self.fail_site is not None:
            return False
        with self.lock:
            triangle = self._find_triangle_by_vertex(site)
            if triangle is not None:
                triangles, points = self.surrounding_triangles(site, triangle)
                self._remove(site, points, triangles)
                return True
            return False

    def _find_triangle_circumscribe(self, site):
        '''
        Search the triangle that circumcircle the point
        '''
        for triangle in self.neighborhood:
            if triangle.circumscribe(site) or triangle.contains(site):
                return triangle
        raise Exception("Site out of the valid area.")
    
    def _find_triangle_by_vertex(self, site):
        '''
        Search the triangle that contains the point as a vertex
        '''
        for triangle in self.neighborhood.keys():
            if triangle.contains(site):
                return triangle
        return None
    
    def _cavity(self, site, triangle):
        '''
        Determine the cavity caused by site.
        '''
        encroached = {}
        toBeChecked = [triangle]
        marked = {triangle: None}
        while (len(toBeChecked) > 0):
            triangle = toBeChecked.pop()
            # Site outside triangle => triangle not in cavity
            if not triangle.circumscribe(site):
                continue
            encroached[triangle] = None
            # Check the neighborhood
            for neighbor in self.neighborhood[triangle]:
                if neighbor not in marked:
                    marked[neighbor] = None
                    toBeChecked.append(neighbor)
        return encroached

    def _add(self, site, triangle):
        '''
        Add a site to triangulation with specific triangle
        '''
        if not triangle.contains(site):
            cavity = self._cavity(site, triangle)
            self._insert(site, cavity)
            return True
        return False

    def _add_triangle(self, t1, t2):
        '''
        Add a triangle to neighborhood control
        '''
        if t1 not in self.neighborhood:
            self.neighborhood[t1] = {t2}
        else:
            self.neighborhood[t1][t2] = None

    def _del_triangle(self, triangle):
        '''
        Remove a triangle from neighborhood control
        '''
        del self.neighborhood[triangle]
        for neighbor in self.neighborhood.values():
            if triangle in neighbor:
                del neighbor[triangle]
        
    def _link_triangles(self, t1, t2):
        '''
        Include mutual neighbor triangles
        '''
        self._add_triangle(t1, t2)
        self._add_triangle(t2, t1)

    def _insert(self, site, cavity):
        '''
        Update the triangulation by removing the cavity triangles and then
        filling the cavity with new triangles.
        '''
        boundary = {}
        triangles = {}

        # Find boundary facets and adjacent triangles
        for triangle in cavity:
            for neighbor in self.neighborhood[triangle]:
                # Adj triangles only
                if neighbor not in cavity:
                    triangles[neighbor] = None
            for point in triangle:
                facet = triangle.opposite_facet(point)
                if facet in boundary:
                    del boundary[facet]
                else:
                    boundary[facet] = None

        # Remove the cavity triangles from the triangulation
        for triangle in cavity:
            self._del_triangle(triangle)

        # Build each new triangle and include it to the triangulation
        new_triangles = {}
        for facet in boundary.keys():
            newTriangle = Triangle(facet[0], facet[1], site)
            self.neighborhood[newTriangle] = {}
            new_triangles[newTriangle] = None
            # Adj triangle + new triangles
            triangles[newTriangle] = None

        # Update the graph links for each new triangle
        for triangle in new_triangles:
            for other in triangles:
                if triangle.is_neighbor(other):
                    self._link_triangles(triangle, other)
                    
        if self.main_site is None:
            self.main_site = site
        self._new_version()
        if self.check_triangulation:
            if not self.valid():
                print "### Including failure"

    def _remove(self, site, points, triangles):
        '''
        Update the triangulation by removing the surrounding triangles 
        and then filling this cavity with new Delaunay triangles.
        Challenge: Maintain neighborhood control.
        Ref: Mir Abolfazl Mostafavi, Christopher Gold, and Maciej Dakowicz. 
             2003. Delete and insert operations in Voronoi/Delaunay methods 
             and applications. Comput. Geosci. 29, 4 (May 2003), 523-530. 
             DOI=10.1016/S0098-3004(03)00017-7 
             http://dx.doi.org/10.1016/S0098-3004(03)00017-7
        '''
        # verify compatibility of surrounding points and old triangles
        if len(points) != len(triangles):
            raise Exception("Triangulation has different sizes.")
        # verify is the triangulation is empty (or insufficient?)
        if len(points) < 3:
            return
        # initialize new triangulation controls
        i = -1
        new_triangles = {}
        # while there are points to form more than the last triangle
        while len(points) > 3:
            # checks the possible triangles considering 
            # all three consecutive points (i, i1, i2 in a cycle) 
            # of surrounding points of the site
            i += 1
            npoints = len(points)
            if (i >= npoints):
                raise Exception("Inexists a valid ear? Is it possible?")
            i1 = (i + 1) % npoints
            i2 = (i1 + 1) % npoints
            # verify if points represent a valid triangle to site,
            # like a ear listen to the site:
            # 1: gets triangle orientation (CW or CCW)
            o_ear = Triangle.orientation(points[i], points[i1], points[i2])
            # 2: gets direction of triangle to the site (CW or CCW)
            o_ear_site = Triangle.orientation(points[i], points[i2], site)
            # 3: if points are collinear, try another edge as a reference
            # ??why don't take this edge at first place??
            if o_ear_site == 0:
                o_ear_site = Triangle.orientation(points[i], points[i1], site)
            # 4: the directions is the same?
            if (o_ear * o_ear_site) > 0:
                # if so, this a valid ear (possible triangulation)
                valid_ear = Triangle(points[i], points[i1], points[i2])
                # verify if this ear is a Delaunay Triangulation
                ear_is_delaunay = True
                # 1. for all other surrounding points
                for p in points:
                    # 1.1: is this other point (not in ear)?
                    if not valid_ear.contains(p):
                        # verify if ear won't circumcircle it
                        if valid_ear.circumscribe(p):
                            # if circumcircle, ear is not a Delaunay triangle
                            ear_is_delaunay = False
                            break
                # if it is a Delaunay triangle...
                if ear_is_delaunay:
                    # include to new triangle control
                    new_triangles[valid_ear] = None
                    # include to neighborhood control
                    self.neighborhood[valid_ear] = {}
                    # link to the opposite triangles from the removed vertices
                    self._link_ear(site, valid_ear, triangles[i], 
                                   new_triangles)
                    self._link_ear(site, valid_ear, triangles[i1], 
                                   new_triangles)
                    # change triangle related to vertex by the new one
                    # remove old triangle by switching the diagonal
                    triangles[i] = valid_ear
                    # remove middle point (leave the corners) 
                    del points[i1]
                    del triangles[i1]
                    # restart cycle of surrounding points
                    i = -1
        # if has only three neighbours remaining in the surrounding points, 
        # merged these three points (triangles) into last triangulation
        last_ear = Triangle(points[0], points[1], points[2])
        self.neighborhood[last_ear] = {}
        new_triangles[last_ear] = None
        # last triangle closes the triangulation and 
        # needs update the link with all sides (neighborhood)
        self._link_ear(site, last_ear, triangles[0], new_triangles)
        self._link_ear(site, last_ear, triangles[1], new_triangles)
        self._link_ear(site, last_ear, triangles[2], new_triangles)

        if self.main_site is not None:
            if self.main_site == site:
                self.main_site = None
        self._new_version()
        if self.check_triangulation:
            if not self.valid():
                print "### Removing failure"

    def _link_ear(self, site, ear, triangle, new_triangles):
        # check if the triangle has neighbor that is opposite from the site
        neighbor = self.neighbor_opposite(site, triangle)
        # if not, it's a external triangulation (from initial triangle)
        # or it's a new triangulation...
        if neighbor is None:
            # if the triangle related to the vertex is new
            if triangle in new_triangles:
                # update neighborhood with the new triangle
                self._link_triangles(ear, triangle)
            else:
                # otherwise, old triangle doesn't had a neighbor: 
                # ignore its neighborhood and delete it
                self._del_triangle(triangle)
        else:
            # update neighborhood of new triangle (ear)
            self._link_triangles(ear, neighbor)
            # delete old voronoi cell triangle
            self._del_triangle(triangle)

    def neighbor_opposite(self, site, triangle):
        '''
        Report neighbor opposite the given vertex of triangle.
        '''
        if site not in triangle:
            return None
        with self.lock:
            for neighbor in self.neighborhood[triangle]:
                if not neighbor.contains(site):
                    return neighbor
            return None

    def surrounding_triangles(self, site, triangle):
        '''
        Report triangles and points surrounding site in order (cw or ccw).
        '''
        if site not in triangle:
            raise Exception("Site not in triangle.")
        with self.lock:
            points = []
            triangles = []
            start = triangle
            # cw or cww (pay attention)
            guide = triangle.next_vertex_except({site})
            while triangle is not None:
                triangles.append(triangle)
                points.append(guide)
                previous = triangle
                triangle = self.neighbor_opposite(guide, triangle)
                guide = previous.next_vertex_except({site, guide})
                if (triangle == start):
                    break
            return triangles, points

    def voronoi_cells(self):
        '''
        Report polygons of each voronoi cell
        '''
        with self.lock:
            cells = {}
            ignore = {} #x:None for x in self._initTriangle}
            for triangle in self.neighborhood.keys():
                for site in triangle:
                    if site in ignore:
                        continue
                    ignore[site] = None
                    triangles, points = self.surrounding_triangles(site, 
                                                                   triangle)
                    cell = []
                    for tri in triangles:
                        cell.append(tri.circumcenter())
                    cells[site] = cell 
            return cells

    def voronoi_cell_trinagulation(self, site):
        '''
        Test purpose only
        '''
        with self.lock:
            triangle = self._find_triangle_by_vertex(site)
            triangles, points = self.surrounding_triangles(site, triangle)
            return triangles, triangle

    def valid(self):
        for triangle in self.neighborhood.keys():
            for other in self.neighborhood.keys():
                if (other != triangle):
                    for vertex in other:
                        if not triangle.contains(vertex):
                            if triangle.circumscribe(vertex):
                                self.fail_site = vertex
                                self.fail_triangle = triangle
                                self.fail_witness = other
                                print "Triangle     : " + str(triangle)
                                print "Other        : " + str(other)
                                print "Other vertex : " + str(vertex)
                                triangle.debug_distance(vertex)
                                return False
        return True
            

if __name__ == '__main__':

    v = Delaunay(1000)
    sites= [Point2D(100, 100),
            Point2D(150, 200),
            Point2D(300, 150),
            Point2D(250, 200)]
    for s in sites:
        v.include(s)
    print "Delaunay Triangulation created: Triangulation with " + \
        str(len(v.neighborhood)) + " triangles"
    c = 0
    for t in v.neighborhood:
        c += 1
        print str(c) + ":" + str(t)
        for n in v.neighborhood[t]:
            print "--> " + str(n)

    v2 = Delaunay(100)
    v2.include(Point2D(0,0))
    v2.include(Point2D(1,0))
    v2.include(Point2D(0,1))
    print "Delaunay Triangulation created: Triangulation with " + \
        str(len(v2.neighborhood)) + " triangles"
    c = 0
    for t in v2.neighborhood.keys():
        c += 1
        print str(c) + ":" + str(t)
        for n in v2.neighborhood[t]:
            print "-->" + str(n)
