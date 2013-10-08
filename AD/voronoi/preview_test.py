"""
Preview of a Delaunay Triangulation and Voronoi Diagram using matplotlib.
"""
from __future__ import division
import pygame
import random
import time
import threading 
from delaunay import Delaunay
from preview import Preview 
from preview import Color 
from point2D import Point2D
from voronoi import VoronoiDiagram
from timer import Timer

class TestPainter():
    '''
    A simple painter
    '''
    counter = 0
    lock = threading.Lock()
   
    def __init__(self, number, view):
        self.number = number
        self.text = str(view)
        with TestPainter.lock:
            self.id = str(TestPainter.counter)
            TestPainter.counter += 1

    def __repr__(self):
        return 'Test #' + self.id

    def __str__(self):
        return self.__repr__() 

    
    def changed(self):
        return True
    
    def draw(self, panel, index):
        panel.clear()
        pos = (random.randint(1, 100), random.randint(1, 100))
        radius = random.randint(1, 10) 
        pygame.draw.circle(panel.surface, Color.BLUE, pos, radius)
        panel.draw_line(str(index) + "/" + str(self.number), Color.RED)
        panel.draw_line("FPS:" + str(panel.fps), Color.BLUE)
        panel.draw_line("Object:" + self.text, Color.BLUE)
        panel.draw_line("Test ID:" + self.id, Color.BLUE)

class TestFpsPainter():
    '''
    Test with random generated frames to tests FPS control
    '''
   
    def __init__(self, number, object):
        self.number = number
    
    def changed(self):
        return random.randint(0, 2) == 1
    
    def draw(self, panel, index):
        panel.clear()
        pos = (random.randint(1, 100), random.randint(1, 100))
        radius = random.randint(1, 10) 
        pygame.draw.circle(panel.surface, Color.RED, pos, radius)
        panel.draw_line(str(index) + "/" + str(self.number), Color.RED)
        panel.draw_line("FPS:" + str(panel.fps), Color.BLUE)


class SiteSet(object):
    '''
    Generate a random selection for testing the Voronoi Diagram Previewer
    '''
    
    def __init__(self, sites):
        self.sites = sites[:]
        self.included = []
        self.main_site = None
        
    def can_pick(self):
        if self.main_site is None:
            return len(self.included) < len(self.sites)
        return len(self.included) < (len(self.sites) - 1)

    def has_picked(self):
        return len(self.included) > 0
    
    def pick_index(self):
        return random.randint(0, len(self.sites) - 1)

    def is_valid_index(self, i):
        return (i < len(self.sites)) and (i >= 0)

    def is_main_site(self, i):
        assert self.is_valid_index(i)
        if self.main_site is None:
            return False
        return self.main_site == self.sites[i]

    def set_main(self, i):
        assert self.is_valid_index(i)
        if (self.main_site is None) and (not self.has_picked()):
            self.main_site = self.sites[i]
            return self.main_site
        return None

    def pick_main(self):
        return self.set_main(self.pick_index())

    def unset_main(self):
        if (self.main_site is not None) and (not self.has_picked()):
            site = self.main_site
            self.main_site = None
            return site
        return None
    
    def include(self, i):
        assert self.is_valid_index(i)
        if (i in self.included) or self.is_main_site(i):
            return False, None
        self.included.append(i)
        return True, self.sites[i]
    
    def pick(self):
        while self.can_pick():
            included, site = self.include(self.pick_index())
            if included: return site
        return None

    def remove(self, i):
        assert self.is_valid_index(i)
        if (i in self.included):
            self.included.remove(i)
            return True, self.sites[i]
        return False, None

    def unpick(self):
        while self.has_picked():
            removed, site = self.remove(self.pick_index())
            if removed: return site
        return None


class DiagramTest(threading.Thread):
    '''
    Generate a mono and mult thread test for Voronoi Diagram Previewer
    '''
    STARTING = 1
    RUNNING = 2
    STOPPING = 3
    
    num_threads = 0
    lock = threading.Lock()
    
    def __init__(self, sites, delay):
        with DiagramTest.lock:
            self.id = DiagramTest.num_threads
            DiagramTest.num_threads += 1
            self.diagram = Delaunay()
            self.site_set = SiteSet(sites)
        self.delay = delay
        self.big_delay = 2.0 if delay < 2.0 else delay
        self.state = DiagramTest.STARTING
        threading.Thread.__init__(self)
        self.counter = 0
        

    def start(self):
        threading.Thread.start(self)

    def stop(self):
        self.state = DiagramTest.STOPPING
        
    def must_exit(self):
        self.counter += 1
        threading.Event().wait(self.delay)
        return self.state != DiagramTest.RUNNING or self.diagram.has_error()

    def must_exit_delay(self, delay):
        self.counter += 1
        threading.Event().wait(delay)
        return self.state != DiagramTest.RUNNING or self.diagram.has_error()

    def run(self):
        self.state = DiagramTest.RUNNING
        while True:
            #self.diagram.include_as_main(self.site_set.pick_main())
            if self.must_exit(): break
            while self.site_set.can_pick():
                self.diagram.include(self.site_set.pick())
                if self.must_exit(): break
            while self.site_set.has_picked():
                self.diagram.remove(self.site_set.unpick())
                if self.must_exit(): break
            #self.diagram.remove(self.site_set.unset_main())
            if self.must_exit(): break
        # keep running until receive a stop sign
        while self.state == DiagramTest.RUNNING:
            pass

class DiagramTestNear(DiagramTest):

    def run(self):
        self.state = DiagramTest.RUNNING
        while True:
            if self.site_set.can_pick():
                main = self.site_set.pick_main()
                self.diagram.include(main)
            if self.must_exit(): break
            while self.site_set.can_pick():
                self.diagram.include_near(self.site_set.pick(), main)
                if self.must_exit(): break
            self.diagram.remove_far_sites(main)
            if self.must_exit_delay(self.big_delay): break
            while self.site_set.has_picked():
                self.diagram.remove(self.site_set.unpick())
                if self.must_exit(): break
            self.diagram.remove(self.site_set.unset_main())
            if self.must_exit(): break
        # keep running until receive a stop sign
        while self.state == DiagramTest.RUNNING:
            pass

class TestCases(threading.Thread):

    @staticmethod
    def displace(pointSet, dx = 0, dy = 0):
        points = []
        for p in pointSet:
            site = Point2D(p[0]+dx, p[1]+dy)
            points.append(site)
        return points

    @staticmethod
    def random_points(min, max, sx = 150, sy = 150):
        points = {}
        n = random.randint(min, max)
        for i in xrange(0, n):
            p = Point2D(random.randint(0,sx), random.randint(0,sy))
            if p not in points:
                points[p] = i
        return points.keys()

    @staticmethod
    def simple(sites, main_index):
        # simple test without cleanup:
        assert len(sites) > 1 
        d = Delaunay()
        d.include(sites[main_index])
        for site in sites:
            if site == sites[main_index]: continue
            d.include(site)
            if not d.fail_site is None:
                Preview.voronoi(d, title = "Fail")
                
        VoronoiDiagram.show(d, title = "Without cleanup")

    @staticmethod
    def auto_cleanup(sites, main_index):
        # test automatic cleanup: 
        assert len(sites) > 1 
        d = Delaunay()
        d.include(sites[main_index])
        for site in sites:
            if site == sites[main_index]: continue
            d.include_near(site, sites[main_index])
            
        VoronoiDiagram.show(d, title = "Automatic 'main site' cleanup mode")

    @staticmethod
    def manual_cleanup(sites, main_index):
        # test manual cleanup: 
        assert len(sites) > 1 
        d = Delaunay()
        d.include(sites[main_index])
        for site in sites:
            if site == sites[main_index]: continue
            d.include(site)
        d.remove_far_sites(sites[main_index])
        
        VoronoiDiagram.show(d, title = "Manual 'main site' cleanup mode")

    @staticmethod
    def multi_thread(points, delay, n, lines, columns, fps = 0, 
                     test_class = DiagramTest):
        # test multi-thread execution: 
        VoronoiDiagram.start(title = test_class.__name__ + " - Multi-thread test", 
                             lines = lines, columns = columns, fps_limit = fps)
        # configure
        threads = []
        # create tests in different threads
        for i in xrange(0,n):
            test = test_class(points, delay)
            threads.append(test)
            VoronoiDiagram.new_diagram(test.diagram)
            test.start()
        # waiting until previewer terminates
        while VoronoiDiagram.in_state(Preview.RUNNING):
            pass
        # stop tests threads
        for t in threads:
            t.stop()

    @staticmethod
    def mono_thread(points, delay, n, lines, columns, fps = 250,
                    test_class = DiagramTest):
        # test mono-thread execution (previewer run as main thread): 
        preview = Preview(title = test_class.__name__ + " - Mono-thread test", 
                          lines = lines, columns = columns, fps_limit = fps,
                          painter_class = VoronoiDiagram)
        preview.interactive(True)
        preview.start()
        threads = []
        # create tests in different threads
        for i in xrange(0,n):
            test = test_class(points, delay)
            threads.append(test)
            preview.add_view(test.diagram)
            test.start()
            if not preview.step():
                break

        while preview.step():
            pass

        # stop tests threads
        for t in threads:
            t.stop()

    @staticmethod
    def painter_test(n, lines, columns, fps = 25000):
        # test mono-thread execution (previewer run as main thread): 
        preview = Preview(title = "Mono-thread test without diagram", 
                          lines = lines, columns = columns, fps_limit = fps,
                          painter_class = TestPainter)
        
        preview.interactive(True)
        preview.start()
        # create tests in different threads
        for i in xrange(0,n):
            preview.add_view(i)
            if not preview.step():
                break

        while preview.step():
            pass

    @staticmethod
    def painter_test2(n, lines, columns, fps = 25000):
        # test mono-thread execution (previewer run as main thread): 
        preview = Preview(title = "Mono-thread test without diagram", 
                          lines = lines, columns = columns, fps_limit = fps,
                          painter_class = TestPainter)
        timer = Timer(0.1)
        preview.interactive(True)
        preview.start()
        step = 0
        # create tests in different threads
        while True:
            step += 1
            preview.set_title("Test #" + str(step) + " ADD-del inverse order")
            painters = []
            for i in xrange(0,n):
                painters.append(preview.add_view(i))
                while timer.idle(): 
                    if not preview.step():
                        return
            preview.set_title("Test #" + str(step) + " add-DEL inverse order")
            while len(painters) > 0:
                preview.del_view(painters.pop())
                while timer.idle(): 
                    if not preview.step():
                        return
            preview.set_title("Test #" + str(step) + " ADD-del direct order")
            painters = []
            for i in xrange(0,n):
                painters.append(preview.add_view(i))
                while timer.idle(): 
                    if not preview.step():
                        return
            preview.set_title("Test #" + str(step) + " add-DEL direct order")
            for i in xrange(0,n):
                preview.del_view(painters[i])
                while timer.idle(): 
                    if not preview.step():
                        return
            preview.set_title("Test #" + str(step) + " ADD-del random order")
            painters = []
            for i in xrange(0,n):
                painters.append(preview.add_view(i))
                while timer.idle(): 
                    if not preview.step():
                        return
            preview.set_title("Test #" + str(step) + " add-DEL random order")
            while len(painters) > 0:
                i = random.randint(0, len(painters) - 1)
                preview.del_view(painters[i])
                del painters[i]
                while timer.idle(): 
                    if not preview.step():
                        return


    @staticmethod
    def mono_thread_fps(points, delay, n, lines, columns, fps = 250, 
                        max_threads = 10, test_class = DiagramTest):
        # test mono-thread execution (previewer run as main thread): 
        preview = Preview(title = test_class.__name__ + 
                          " - Mono-thread test mixed with diagram (FPS limited)", 
                          lines = lines, columns = columns, fps_limit = fps)

        preview.interactive(True)
        preview.start()
        # create tests 
        threads = []
        for i in xrange(0,n):
            if len(threads) < max_threads and random.randint(1, 5) == 1:
                test = test_class(points, delay)
                threads.append(test) 
                preview.add_view(test.diagram, VoronoiDiagram)
                test.start()
            elif random.randint(1, 2) == 1:
                preview.add_view(i, TestFpsPainter)
            else:
                preview.add_view(i, TestPainter)
            if not preview.step():
                break

        while preview.step():
            pass

        # stop tests threads
        for t in threads:
            t.stop()

    @staticmethod
    def random_points_test(min, max, delay, n, lines, columns, max_count,
                           fps, test_class = DiagramTest):
        # test mono-thread execution (previewer run as main thread): 
        preview = Preview(title = test_class.__name__ + 
                          " - Mono-thread test with random points", 
                          lines = lines, columns = columns, fps_limit = fps,
                          painter_class = VoronoiDiagram)

        x, y = preview.panel_dimensions()
        preview.interactive(True)
        preview.start()
        # create tests 
        threads = []
        remove = None
        while True:
            
            if len(threads) < n:
                test = test_class(TestCases.random_points(min, max, x, y), 
                                   delay)
                threads.append(test)
                test.start()
                preview.add_view(test.diagram)

            for i in xrange(0, len(threads)):
                if threads[i].counter >= max_count:
                    preview.del_view(threads[i].diagram)
                    threads[i].stop()
                    del threads[i]
                    break
                    
            if not preview.step():
                break

        # stop tests threads
        for t in threads:
            t.stop()

    def __init__(self):
        self.delay = 0
        self.pointSet = []
        self.main_index = 0
        self.fps = 0
        self.force_multi_thread_execution = False

    def set_cenario(self):
        self.pointSet = [
            Point2D(50, 75),  #0
            Point2D(75, 50),  #1
            Point2D(125,50),  #2
            Point2D(150, 75), #3
            Point2D(150, 125),#4
            Point2D(125, 150),#5
            Point2D(75, 150), #6
            Point2D(50, 125), #7
            Point2D(10, 100), #8
            Point2D(190, 100),#9
            Point2D(100, 10), #10
            Point2D(100, 190),#11
            Point2D(30, 30),  #12
            Point2D(170, 170),#13
            Point2D(170, 30), #14
            Point2D(30, 170), #15
            Point2D(100, 100) #16
            ]
        self.main_index = 16
        
        self.painters = 10000
        self.painters_add_del = 30
        self.mixed_diagrams = 10
        self.diagrams = 15
        self.lines = 4
        self.columns = 3
        self.min_sites = 5
        self.max_sites = 20 
        self.max_diagram_version = 80

    def do_basic_tests(self, dx, dy):
        points = TestCases.displace(self.pointSet, dx, dy)
        TestCases.simple(points, self.main_index)
        TestCases.manual_cleanup(points, self.main_index)
        TestCases.auto_cleanup(points, self.main_index)
    
    def do_painter_tests(self):
        TestCases.painter_test(self.painters, 
                               self.lines, self.columns, self.fps)
        TestCases.painter_test2(self.painters_add_del, 
                                self.lines, self.columns, self.fps)

    def do_thread_tests(self, test_class = DiagramTest):
        
        TestCases.mono_thread(self.pointSet, self.delay, self.diagrams, 
                              self.lines, self.columns, self.fps,
                              test_class)

        TestCases.mono_thread_fps(self.pointSet, self.delay, 
                                  self.painters, 
                                  self.lines, self.columns, self.fps, 
                                  self.mixed_diagrams, test_class)
        
        TestCases.random_points_test(self.min_sites, self.max_sites, 
                                     self.delay,
                                     self.diagrams, self.lines, self.columns, 
                                     self.max_diagram_version, 
                                     self.fps, test_class)
        import platform
        if platform.system() == "Linux" or self.force_multi_thread_execution:
            TestCases.multi_thread(self.pointSet, self.delay, 
                                   self.diagrams, self.lines, self.columns, 
                                   self.fps, test_class)

    def do_exaustive_test(self):
        TestCases.random_points_test(min_sites, max_sites, self.delay, 
                                     diagrams, lines, columns, 
                                     max_diagram_version, 
                                     self.fps, test_class)
        
if __name__ == '__main__':

    test = TestCases()
    test.set_cenario()
    test.do_basic_tests(300, 300)
    test.do_painter_tests()
    test.do_thread_tests(DiagramTest)
    test.do_thread_tests(DiagramTestNear)
    