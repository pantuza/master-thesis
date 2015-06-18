'''
Created on 22/09/2013

@author: Gustavo Pantuza
'''
from __future__ import division
class Point2D(tuple):
    '''
    Tuple representing a 2D point 
    '''

    @staticmethod
    def __new__(cls, x, y):
        return tuple.__new__(cls, (float(x), float(y)))
        #return tuple.__new__(cls, (x, y))

    def __repr__(self):
        return '(' + str(self[0]) + ',' + str(self[1]) + ')'

    def __str__(self):
        return self.__repr__()

