'''
Created on 22/09/2013

@author: Gustavo Pantuza
'''

class Segment(tuple):
    '''
    Line segment representing a polygon side
    '''

    @staticmethod
    def __new__(cls, p1, p2):
        if (p1 < p2):
            return tuple.__new__(cls, (p1, p2))
        else:
            return tuple.__new__(cls, (p2, p1))

    def contains(self, p):
        return (p == self[0]) or (p == self[1])
        

    def __repr__(self):
        return '(' + str(self[0]) + ',' + str(self[1]) + ')'

    def __str__(self):
        return self.__repr__() 
    '''
    def __eq__(self, s):
        # No need, if the points remain previously ordered
        # in this case, hash comparison is more efficient 
        if (self[0] == s[0]):
            return self[1] == s[1]
        elif (self[0] == s[1]):
            return self[1] == s[0]
        return False
    '''

