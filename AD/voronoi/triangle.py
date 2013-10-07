'''
Created on 22/09/2013

@author: Gustavo Pantuza
'''
from __future__ import division
from numpy.linalg import det 
from math import fabs, sqrt
from segment import Segment

class Triangle(tuple):
    '''
    classdocs
    '''
    # float point precision
    EPSILON = 0.0000000001

    @staticmethod
    def orientation(p1, p2, p3):
        '''
        
        '''
        # Test the relation of the points using determinat
        d = det(((p1[0], p1[1], 1),
                 (p2[0], p2[1], 1),
                 (p3[0], p3[1], 1)))
        # check float point precision
        if fabs(d) < Triangle.EPSILON:
            return 0.0
        else:
            return d

    @staticmethod
    def __new__(cls, p1, p2, p3):
        # make the last be the 'bigger' point
        if p1 > p2:
            if p1 > p3:
                tp = p3
                p3 = p1
                p1 = tp
        else:
            if p2 > p3:
                tp = p3
                p3 = p2
                p2 = tp
        # Test the relation of the points using determinat
        d = Triangle.orientation(p1, p2, p3)
        # Collinear
        if (d == 0):
            raise Exception("Can't create a triangle with collinear points!")
        # ClockWise (default)
        elif (d < 0): 
            return tuple.__new__(cls, (p1, p2, p3))
        # Counter ClockWise (invert)
        else:
            return tuple.__new__(cls, (p2, p1, p3))

    def __init__(self, p1, p2, p3):
        # precalculate center
        if not self._pre_circumcenter():
            raise Exception("Too small or float point precision limit (r=%f)."
                            % self.radius ) 
        # postpone calculation of matrix whitch 
        # verify if point is inside circle
        #self.m = None

    def __repr__(self):
        return 'Triangle[' + str(self[0]) + \
            ',' + str(self[1]) + ',' + str(self[2]) + ']'

    def __str__(self):
        return self.__repr__() 

    def contains(self, p):
        return (self[0] == p) or \
            (self[1] == p) or (self[2] == p)

    '''
    def x__eq__(self, t):
        # No need, if the points remain previously ordered
        # in this case, hash comparison is more efficient 
    '''

    def _pre_circumcenter(self):
        '''
        calculate the circumscribe center and the radius r
        NOTE: A point on the edge is inside the circumscribe
        http://paulbourke.net/papers/triangulate/
        '''
        x1 = self[0][0]
        y1 = self[0][1]
        x2 = self[1][0]
        y2 = self[1][1]
        x3 = self[2][0]
        y3 = self[2][1]
        #double m1,m2,mx1,mx2,my1,my2;
        #double dx,dy,drsqr;
        fabsy1y2 = fabs(y1 - y2)
        fabsy2y3 = fabs(y2 - y3)

        # Check for coincident points */
        if (fabsy1y2 < Triangle.EPSILON and fabsy2y3 < Triangle.EPSILON):
            return False

        if (fabsy1y2 < Triangle.EPSILON):
            m2 = - (x3-x2) / (y3-y2)
            mx2 = (x2 + x3) / 2.0
            my2 = (y2 + y3) / 2.0
            self.cx = (x2 + x1) / 2.0
            self.cy = m2 * (self.cx - mx2) + my2
        elif (fabsy2y3 < Triangle.EPSILON):
            m1 = - (x2-x1) / (y2-y1)
            mx1 = (x1 + x2) / 2.0
            my1 = (y1 + y2) / 2.0
            self.cx = (x3 + x2) / 2.0
            self.cy = m1 * (self.cx - mx1) + my1
        else:
            m1 = - (x2-x1) / (y2-y1)
            m2 = - (x3-x2) / (y3-y2)
            mx1 = (x1 + x2) / 2.0
            mx2 = (x2 + x3) / 2.0
            my1 = (y1 + y2) / 2.0
            my2 = (y2 + y3) / 2.0
            try:
                self.cx = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2)
            except:
                self.cx = 0.0
            if (fabsy1y2 > fabsy2y3):
                self.cy = m1 * (self.cx - mx1) + my1
            else:
                self.cy = m2 * (self.cx - mx2) + my2
        
        dx = x2 - self.cx
        dy = y2 - self.cy
        self.radius = sqrt(dx*dx + dy*dy)
        
        return (self.radius > Triangle.EPSILON)

    def circumcenter(self):
        return (self.cx, self.cy)

    def circumcircle_radius(self):
        return self.radius

    """
    def circumscribe(self, point):
        '''
        Verify if triangle circumscribe the point
        via orientation of matrix m 
        '''
        if self.m is None:
        # precalculate matrix for test point inside circle
            self.m = []
            for i in (0,1,2):
                x = self[i][0]
                y = self[i][1]
                dist2 = x*x + y*y
                self.m.append([x, y, dist2, 1])
            self.m.append([])
        # calculate the orientation of matrix m
        self.m[3] = [point[0], point[1], 
                     point[0]*point[0] + point[1]* point[1], 1]
        d = det(self.m)
        # check float point precision
        if fabs(d) < Triangle.EPSILON:
            d = 0.0
        return d < 0
    """
    
    def circumscribe(self, point):
        '''
        Verify if triangle circumscribe the point
        via distance between point and the circumcenter 
        '''
        dx = self.cx - point[0]
        dy = self.cy - point[1]
        dist = sqrt(dx*dx + dy*dy)
        # check float point precision
        if fabs(dist) < Triangle.EPSILON:
            dist = 0.0
        return dist < self.radius

    def opposite_facet(self, point):
        '''
        gets the opposite facet of a triangle vertex
        '''
        if point == self[0]:
            return Segment(self[1], self[2])
        elif point == self[1]:
            return Segment(self[0], self[2])
        elif point == self[2]:
            return Segment(self[0], self[1])
        else:
            raise Exception("Point is not a triangle vertex")

    def is_neighbor(self, triangle):
        '''
        Check if the triangles has a common facet
        '''
        count = 0
        for vertex in self:
            if (not triangle.contains(vertex)):
                count += 1
        return count == 1

    def next_vertex_except(self, exclude):
        '''
        Get the next vertex of a triangle, considering exceptions
        PS: must guarantee the order (CW or CCW)
        '''
        for point in self:
            if point not in exclude:
                return point
        return None

    @staticmethod
    def _sign(p1, p2, p3):
        return (p1[0] - p3[0]) * (p2[1] - p3[1]) - \
               (p2[0] - p3[0]) * (p1[1] - p3[1])

    def collide(self, point):
        '''
        Check if a point is inside a triangle area
        '''
        b1 = Triangle._sign(point, self[0], self[1]) < 0.0
        b2 = Triangle._sign(point, self[1], self[2]) < 0.0
        b3 = Triangle._sign(point, self[2], self[0]) < 0.0
        return ((b1 == b2) and (b2 == b3))
    '''
    # Planning: use to accelerate "remove(site)" algorithm
    # see reference...
    def getPower(self, point):
        self.m[3] = [point[0], point[1], 
                     point[0]*point[0] + point[1]* point[1], 1]
        h = det(self.m)
        if fabs(d) < Triangle.EPSILON:
            d = 0.0
        d = self.getDeterminant()
        return (h / d)
    '''

if __name__ == '__main__':

    from point2D import Point2D
    #http://www.icoachmath.com/math_dictionary/Circumcenter.html
    t = Triangle(Point2D(-1, -2),
                 Point2D( 2,  3),
                 Point2D( 3, -3));
    print "Center = " + str(t.circumcenter())
    print "Radius = " + str(t.circumcircle_radius())
    
    print "circumscribe:"
    print t.circumscribe(Point2D(1,0))
    print t.circumscribe(Point2D(-1,-1))
    print t.circumscribe(Point2D(0,0))
    print t.circumscribe(Point2D(-3,-2))
    print t.circumscribe(Point2D(4,4))
    print t.circumscribe(Point2D(-4,-4))
    print "collide:"
    print t.collide(Point2D(1,0))
    print t.collide(Point2D(-1,-1))
    print t.collide(Point2D(0,0))
    print t.collide(Point2D(-3,-2))
    print t.collide(Point2D(4,4))
    print t.collide(Point2D(-4,-4))
    print t.collide(Point2D(2,2))
    print "-------------"
    
    t2 = Triangle(Point2D(-10.0,-10.0), Point2D(0.0,10.0), Point2D(0.0,0.0))
    print t2.circumscribe(Point2D(0,1))
    
    t3 = Triangle(Point2D(2,3), Point2D(3,-3), Point2D(-1, -2))
    t4 = Triangle(Point2D(3,-3), Point2D(-1, -2), Point2D(2,3))
    if (t3 == t):
        print "OK!"
    else:
        print "ERRO? " + str(t3) + " != " + str(t)
        
    if (t4 == t3):
        print "OK!"
    else:
        print "ERRO? " + str(t4) + " != " + str(t3)

    ta = Triangle(Point2D(150.0,75.0),
                  Point2D(150.0,125.0),
                  Point2D(125.0,150.0))
    print "Center a= " + str(ta.circumcenter())
    print "Radius a= " + str(ta.circumcircle_radius())
    print ta.circumscribe(Point2D(50.0,125.0))

    print Triangle.orientation((30.0,170.0),(75.0,150.0),(150.0,125.0))
    
    tx = Triangle((125.0,150.0), (30.0,170.0), (100.0,190.0))
    print tx.radius
    p = (0.0,10000.0)
    print tx.circumscribe(p)
    print str(sqrt((tx.cx - p[0])*(tx.cx - p[0])+(tx.cy-p[1])*(tx.cy-p[1])))