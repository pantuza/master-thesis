"""
Preview of a Delaunay Triangulation and Voronoi Diagram using matplotlib.
"""
import pygame
from delaunay import Delaunay
import threading


class Color(object):
    CYAN = pygame.Color(0, 183, 235)
    YELLOW = pygame.Color(255,255,0)
    MAGENT = pygame.Color(255,20,147)
    ORANGE = pygame.Color(255,165,0)
    RED = pygame.Color(255, 0, 0)
    BLUE = pygame.Color(0, 0, 255)
    GREEN = pygame.Color(0, 255, 0)
    GRAY = pygame.Color(105,105,105)
    BLACK = pygame.Color(0,0,0)
    WHITE = pygame.Color(255,255,255)


class Panel(object):
    
    def __init__(self, width = 800, height = 600):
        self.width = width
        self.height = height
        self.surface = pygame.Surface((self.width, self.height))
        self.pos = (0, 0) # test
        self.color = Color.WHITE
        self.alpha = []
    
    def set_position(self, pos):
        # copy to avoid reference leaking
        self.pos[0] = pos[0]
        self.pos[1] = pos[1]
        
    def push_alpha(self, opacity):
        self.alpha.append(self.surface.get_alpha())
        self.surface.set_alpha(opacity)

    def pop_alpha(self):
        opacity = self.alpha.pop()
        self.surface.set_alpha(opacity)


class Preview(threading.Thread): 

    STARTING = 0
    RUNNING = 1
    STOPPING = 2
    KILLED = 3

    preview = None

    def __init__(self, title = "Voronoi diagram", x = 800, 
                 y = 600):

        self.state = Preview.STARTING
        self.width = x
        self.height = y
        self.display = pygame.display.set_mode((x, y))
        self.siteRadius = 2
        self.panel = {}
        #self.addDiagram(diagram, x, y)
        self.set_title(title)
        pygame.init()
        threading.Thread.__init__(self)
        
    def __del__(self):
        if self.state != Preview.KILLED:
            pygame.quit()

    @staticmethod
    def start_voronoi():
         Preview.preview = Preview()
         Preview.preview.start()

    @staticmethod
    def new_diagram(diagram):
        Preview.preview.add_diagram(diagram)

    def start(self):
        threading.Thread.start(self)

    def run(self):
        self.clock = pygame.time.Clock()
        self.state = Preview.RUNNING
        while self.state == Preview.RUNNING:
            
            for diagram in self.panel.keys():
                self.draw_voronoi(diagram)
            self.show()
            self.clear()
            self.process_events()
            self.clock.tick(30)

    def add_diagram(self, diagram):
        if not isinstance(diagram, Delaunay):
            raise Exception("Object is not a Delaunay Triangulation.")
        self.panel[diagram] = Panel()

    def quit(self):
        return self.state == Preview.KILLED
        pygame.quit()
        
    def is_running(self):
        return self.state == Preview.RUNNING

    def _get_default(self, diagram):
        if diagram is None:
            return self.default, self.panel[self.default]
        return diagram, self.panel[diagram]

    def addDiagram(self, diagram, x, y):
        self.default = diagram
        panel = Panel(x,y)
        panel.surface.fill(panel.color)
        self.panel[diagram] = panel
        
    @staticmethod
    def distance(size):
        return int(size)
        
    @staticmethod
    def position(point):
        return (int(round(point[0])), int(round(point[1])))
        
    @staticmethod
    def voronoi(diagram, title = "Voronoi diagram"):
        preview = Preview(diagram, title)
        while preview.is_running():
            preview.draw_voronoi()
            preview.show()
            preview.clear()
            preview.process_events()

    def process_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.state = Preview.STOPPING
                return
    
    def set_title(self, title):
        if title is not None:
            pygame.display.set_caption(title)

    def interactive(self, interac = True):
        pass
    
    def clear(self, diagram = None):
        diagram, panel = self._get_default(diagram)
        panel.surface.fill(panel.color)

    def _apply(self, panel):
        # apply drawings
        self.display.blit(panel.surface, panel.pos)
    
    def draw(self):
        # plot control
        pass

    def show(self):
        pygame.display.flip()
        
    def setDefault(self, diagram):
        self.default = diagram

    def draw_voronoi(self, diagram = None):
        # plot the voronoi cells
        diagram, panel = self._get_default(diagram)
        cells = diagram.voronoi_cells()
        for site, cell in cells.items():
            # draw cell
            pygame.draw.aalines(panel.surface, Color.BLACK, True, cell)
            pos = Preview.position(site)
            pygame.draw.circle(panel.surface, Color.BLACK, pos, self.siteRadius)
        # apply drawings
        self._apply(panel)

    def draw_voronoi_cell(self, theSite, diagram = None):
        # plot the voronoi cells
        diagram, panel = self._get_default(diagram)
        cells = diagram.voronoi_cells()
        for site, cell in cells.items():
            if site == theSite:
                # draw cell
                pygame.draw.aalines(panel.surface, Color.BLACK, True, cell)
                pos = Preview.position(site)
                pygame.draw.circle(panel.surface, Color.BLACK, pos, self.siteRadius)
                self._apply(panel)
                return
        
    def draw_voronoi_cell_triangulation(self, site, diagram = None):
        # plot the voronoi cells
        diagram, panel = self._get_default(diagram)
        triangules, initial = self.delaunay.voronoi_cell_trinagulation(site)
        pygame.draw.circle(panel.surface, Color.RED, pos, self.siteRadius)
        panel.push_alpha(90)
        for triangle in triangules:
            pygame.draw.aalines(panel.surface, Color.GREEN, True, triangle)
        panel.surface.pop_alpha()
        self.draw_neighbors(initial, diagram, panel)
        self._apply(panel)

    def draw_triangulation(self, diagram = None):
        diagram, panel = self._get_default(diagram)
        panel.push_alpha(50)
        for triangle in diagram.neighborhood.keys():
            pygame.draw.aalines(panel.surface, Color.BLUE, True, triangle)
        panel.pop_alpha()
        self._apply(panel)

    def draw_circumcircles(self, diagram = None):
        diagram, panel = self._get_default(diagram)
        for triangle in diagram.neighborhood.keys():
            center = triangle.circumcenter()
            if (center[0] <= panel.width) and (center[1] <= self.height) and \
               (center[0] >= 0) and (center[1] >= 0):
                pos = Preview.position(center)
                radius = Preview.distance(triangle.circumcircle_radius())
                pygame.draw.circle(panel.surface, Color.YELLOW, pos, radius, 1)
        self._apply(panel) 

    def _draw_neighbors(self, triangle, diagram, panel):
        if triangle is not None:
            neighborhood = diagram.neighborhood[triangle]
            panel.push_alpha(70)
            for neighbor in neighborhood:
                pygame.draw.polygon(panel.surface, Color.BLUE, neighbor, 0)
            panel.surface.pop_alpha()
        
    
if __name__ == '__main__':

    import random
    import time
    from point2D import Point2D
    
    sites= [
            Point2D(50, 75),
            Point2D(75, 50),
            Point2D(125,50),
            Point2D(150, 75),
            Point2D(150, 125),
            Point2D(125, 150),
            Point2D(75, 150),
            Point2D(50, 125),
            Point2D(10, 100),
            Point2D(190, 100),
            Point2D(100, 10),
            Point2D(100, 190),
            Point2D(30, 30),
            Point2D(170, 170),
            Point2D(170, 30),
            Point2D(30, 170)
            ]

    # test manual cleanup: 
    d = Delaunay()
    #d.add_as_main(Point2D(100, 100))
    d.add(Point2D(100, 100))
    for site in sites:
        d.add(site)
    #d.clear_far_sites()
    Preview.voronoi(d, title = "Manual 'main site' cleanup mode")
    
    # test automatic cleanup: 
    d = Delaunay()
    d.add_as_main(Point2D(100, 100))
    d.add(Point2D(100, 100))
    for site in sites:
        d.add_near(site)
    Preview.voronoi(d, title = "Automatic 'main site' cleanup mode")
    
    d = Delaunay(10000)
    d.add(Point2D(100, 100))
    for site in sites:
        d.add(site)

    # test site deletion: 
    preview = Preview(d)
    preview.interactive(True) 
    preview.set_title("Simulation of remove(site)")
    preview.draw_voronoi()
    preview.draw_triangulation()
    preview.draw_circumcircles()
    preview.show()
    preview.clear()
    preview.process_events()
    time.sleep(5)

    deleted = []
    while (preview.is_running()) and (len(sites) > 0):
        i = random.randint(0, len(sites) - 1)
        if i in deleted:
            continue
        deleted.append(i)
        preview.draw_voronoi()
        preview.draw_voronoi_cell(sites[i])
        preview.show()
        preview.clear()
        preview.process_events()
        time.sleep(1)
        d.remove(sites[i])

