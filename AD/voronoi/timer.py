'''
Created on 28/09/2013

@author: Gustavo Pantuza
'''
import time

class Timer(object):
    '''
    Simple timer control
    '''
    
    def __init__(self, delay):
        self.current_time = 0
        self.set_delay(delay)

    def pause(self, pause):
        if pause >= self.delay:
            self.current_time = time.clock()
            self.next_time = self.current_time + pause

    def set_delay(self, delay):
        assert delay >= 0
        self.delay = delay
        if self.delay == 0:
            self.next_time = self.current_time
        else:
            self.current_time = time.clock()
            self.next_time = self.current_time + self.delay
        
    def idle(self):
        '''
        Verify if the timer is idle
        '''
        self.current_time = time.clock()
        ## if next frame occurs in the future, now it's idle time
        if self.next_time > self.current_time:
            return True
        # if pass more than one delay time, synchronize it
        if (self.current_time - self.next_time) > self.delay:
            self.next_time = self.current_time + self.delay
        else:
            self.next_time += self.delay
        return False
