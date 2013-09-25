'''
Created on 22/09/2013

@author: Gustavo Pantuza
'''
from __future__ import division
from numpy.linalg import det 
from math import fabs, sqrt
from segment import Segment
from point2D import Point2D

class Triangle(tuple):
    '''
    classdocs
    '''
    # float point precision
    EPSILON = 0.000001

    @staticmethod
    def __new__(cls, p1, p2, p3):
        # the last is the 'bigger' point
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
        d = det([[p1[0], p1[1], 1],
                 [p2[0], p2[1], 1],
                 [p3[0], p3[1], 1]])
        # Collinear
        if (d == 0):
            raise Exception("Can't create a triangle with collinear points!")
        # ClockWise
        elif (d > 0): 
            return tuple.__new__(cls, (p2, p1, p3))
        # Counter ClockWise
        else:
            return tuple.__new__(cls, (p1, p2, p3))

    def __init__(self, p1, p2, p3):
        # precalculate center
        if not self._circumCenter():
            raise Exception("Too small or float point precision limit (r=%f)." 
                            % self.radius ) 
        # precalculate matrix for test point inside circle
        self.m = []
        for i in [0,1,2]:
            x = float(self[i][0])
            y = float(self[i][1])
            dist2 = float(x*x) + float(y*y)
            self.m.append([x, y, dist2, 1])
        self.m.append([])

    def __repr__(self):
        return 'Triangle' + str(self._id) + '[' + str(self[0]) + \
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

    def _circumCenter(self):
        '''
        calculate the circumcircle center and the radius r
        NOTE: A point on the edge is inside the circumcircle
        http://paulbourke.net/papers/triangulate/
        '''
        x1 = float(self[0][0])
        y1 = float(self[0][1])
        x2 = float(self[1][0])
        y2 = float(self[1][1])
        x3 = float(self[2][0])
        y3 = float(self[2][1])
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
            except ZeroDivisionError:
                self.cx = 0.1

            if (fabsy1y2 > fabsy2y3):
                self.cy = m1 * (self.cx - mx1) + my1
            else:
                self.cy = m2 * (self.cx - mx2) + my2
        
        dx = x2 - self.cx
        dy = y2 - self.cy
        self.radius = sqrt(dx*dx + dy*dy)
        
        return (self.radius > Triangle.EPSILON)

    def _circumcircle(self, p):
        '''
        Return TRUE if a point (xp,yp) is inside the circumcircle made up
        of the points (x1,y1), (x2,y2), (x3,y3)
        http://paulbourke.net/papers/triangulate/
        '''
        dx = p[0] - self.cx;
        dy = p[1] - self.cy;
        test_radius = sqrt(dx*dx + dy*dy);
        # Proposed by Chuck Morris
        return ((test_radius - self.radius) <= Triangle.EPSILON)

    def getCircumcenter(self):
        return Point2D(self.cx, self.cy)

    def getCircumcircleRadius(self):
        return self.radius

    def circumcircle(self, point):
        self.m[3] = [point[0], point[1], 
                     point[0]*point[0] + point[1]* point[1], 1]
        return det(self.m) < 0

    def getOppositeFacet(self, point):
        if point == self[0]:
            return Segment(self[1], self[2])
        elif point == self[1]:
            return Segment(self[0], self[2])
        elif point == self[2]:
            return Segment(self[0], self[1])
        else:
            raise Exception("Point is not a triangle vertex")

    def isNeighbor(self, triangle):
        count = 0
        for vertex in self:
            if (not triangle.contains(vertex)):
                count += 1
        return count == 1

    def getNextVertexExcept(self, exclude):
        for point in self:
            if point not in exclude:
                return point
        return None

if __name__ == '__main__':

    from point2D import Point2D
    #http://www.icoachmath.com/math_dictionary/Circumcenter.html
    t = Triangle(Point2D(-1, -2),
                 Point2D( 2,  3),
                 Point2D( 3, -3));
    print "xc = ", ("{0:.15f}".format(t.cx))
    print "yc = ", ("{0:.15f}".format(t.cy))
    print "r  = ", ("{0:.15f}".format(t.radius))

    print "Center = " + str(t.getCircumcenter())
    print "Radius = " + str(t.getCircumcircleRadius())
    
    print t.circumcircle(Point2D(1,0))
    print t.circumcircle(Point2D(-1,-1))
    print t.circumcircle(Point2D(0,0))
    print t.circumcircle(Point2D(-3,-2))
    print t.circumcircle(Point2D(4,4))
    print t.circumcircle(Point2D(-4,-4))
    
    t2 = Triangle(Point2D(-10.0,-10.0), Point2D(0.0,10.0), Point2D(0.0,0.0))
    print t2.circumcircle(Point2D(0,1))
    
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
