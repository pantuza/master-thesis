"""
Preview of a Delaunay Triangulation and Voronoi Diagram using pygame.
"""
from __future__ import division
import pygame
from delaunay import Delaunay
from preview import Preview
from preview import Color

class VoronoiDiagram(object):
    '''
    Painter of Voronoi Diagram by Delaunay Triangulation
    Must to have this interface:

    def __init__(self, number, view):
        """
        Receive a identification number and a object (view) to paint
        """
    def changed(self):
        """
        Informs if the state changed and need to draw a new version
        """
    def draw(self, panel):
        """
        Draw object in this panel
        """
    
    '''

    always_paint = False
    draw_circumcircles = False
    draw_triangulation = False
    draw_cell = False
    draw_cell_triangulation = False
    draw_frame_counter = True
    draw_fps = True
    draw_diagram_version = True
    # radius of Voronoi site (in pixels)
    site_radius = 2
    
    preview = None

    @staticmethod
    def start(**args):
        '''
        Automatic create a default previewer object
        '''
        VoronoiDiagram.preview = Preview(painter_class=VoronoiDiagram, **args)
        VoronoiDiagram.preview.start()

    @staticmethod
    def stop():
        '''
        Stop the default previewer
        '''
        VoronoiDiagram.preview.stop()

    @staticmethod
    def new_diagram(diagram, label=''):
        '''
        Include a diagram into the default previewer
        '''
        print "new_diagram call: label=%s" % label
        VoronoiDiagram.preview.add_view(diagram, label=label)

    @staticmethod
    def in_state(state):
        '''
        Checks the state of the default preview
        '''
        if VoronoiDiagram.preview is None: 
            return state == Preview.NONE
        return state == VoronoiDiagram.preview.state

    @staticmethod
    def show(diagram, title = ''):
        preview = Preview(title = title, lines=1, columns=1)
        preview.add_view(diagram, VoronoiDiagram, title)
        preview.interactive(True)
        preview.start()
        while preview.step():
            pass

    @staticmethod
    def panel_dim():
        return VoronoiDiagram.preview.panel_dim()
    
    def __init__(self, number, view, label=''):
        self.number = number
        self.diagram = view
        self.version = -1
        self.label = label
    
    def changed(self):
        if (self.diagram.version != self.version):
            self.version = self.diagram.version
            return True
        return self.always_paint

    def _draw_voronoi(self, panel):
        # plot the voronoi cells
        cells = self.diagram.voronoi_cells()
        for site, cell in cells.items():
            # _draw cell
            if (len(cell) > 1):
                pygame.draw.aalines(panel.surface, Color.BLACK, True, cell)
            pos = int(round(site[0])), int(round(site[1]))
            # _draw site
            if (self.diagram.main_site is not None) \
            and (self.diagram.main_site == site):
                pygame.draw.circle(panel.surface, Color.RED, 
                                   pos, VoronoiDiagram.site_radius * 2)
            else:
                pygame.draw.circle(panel.surface, Color.BLACK, 
                                   pos, VoronoiDiagram.site_radius)

    def _draw_triangulation(self, panel):
        panel.surface.set_alpha(50)
        for triangle in self.diagram.neighborhood.keys():
            pygame.draw.aalines(panel.surface, Color.CYAN, True, triangle)

    def _draw_voronoi_cell(self, panel, the_site):
        # plot the voronoi cells
        cells = self.diagram.voronoi_cells()
        for site, cell in cells.items():
            if site == the_site:
                #self._draw(panel)
                panel.surface.set_alpha(50)
                color = pygame.Color(255, 255, 0, 50)
                # Polygon method has problem when point is out of screen. 
                # It can lead to a improper cell drawing of site in convex hull
                pygame.draw.polygon(panel.surface, color, cell, 0)
                pos = int(round(site[0])), int(round(site[1]))
                pygame.draw.circle(panel.surface, Color.BLACK, pos,
                                   VoronoiDiagram.site_radius)
                return
        
    def _draw_voronoi_cell_triangulation(self, panel, site, tri = None):
        # plot the voronoi cells
        triangules, initial = self.diagram.voronoi_cell_trinagulation(site)
        panel.surface.set_alpha(90)
        for triangle in triangules:
            pygame.draw.polygon(panel.surface, Color.MAGENT, triangle, 2)
            pos = triangle.circumcenter()
            pos = int(round(pos[0])), int(round(pos[1]))
            radius = int(round(triangle.circumcircle_radius()))
            pygame.draw.circle(panel.surface, Color.RED, pos, radius, 1)
        if (tri is None):
            self._draw_neighbors(panel, initial)
        else:
            self._draw_neighbors(panel, tri)
        #self._draw(panel)
        pos = int(round(site[0])), int(round(site[1]))
        pygame.draw.circle(panel.surface, Color.BLUE, pos, 
                           VoronoiDiagram.site_radius * 3)

    def _draw_circumcircles(self, panel):
        for triangle in self.diagram.neighborhood.keys():
            center = triangle.circumcenter()
            #if (center[0] <= panel.width) and (center[1] <= self.height) \
            #and (center[0] >= 0) and (center[1] >= 0):
            pos = int(round(center[0])), int(round(center[1]))
            radius = int(round(triangle.circumcircle_radius()))
            pygame.draw.circle(panel.surface, Color.GREEN, pos, radius, 1)

    def _draw_neighbors(self, panel, triangle):
        if triangle is not None:
            neighborhood = self.diagram.neighborhood[triangle]
            #self._draw(panel)
            panel.surface.set_alpha(70)
            color = pygame.Color(255, 0, 0, 50)
            pygame.draw.polygon(panel.surface, color, triangle, 0)
            #self._draw(panel)
            for neighbor in neighborhood:
                pygame.draw.aalines(panel.surface, Color.RED, True, neighbor)

    def draw(self, panel, index):
        '''
        Automatically draw the diagram to its panel
        '''
        # clear panel (pixel buffer)
        panel.clear()
        # _draw Delaunay
        if VoronoiDiagram.draw_circumcircles:
            self._draw_circumcircles(panel)
        if VoronoiDiagram.draw_triangulation:
            self._draw_triangulation(panel)
        ## _draw diagram to panel
        self._draw_voronoi(panel)
        if self.diagram.fail_site is not None:
            self._draw_circumcircles(panel)
            self._draw_triangulation(panel)
            self._draw_voronoi_cell(panel, self.diagram.fail_site)
            self._draw_voronoi_cell_triangulation(panel,
                                                  self.diagram.fail_site, 
                                                  self.diagram.fail_triangle)
        elif VoronoiDiagram.draw_cell and self.diagram.main_site is not None:
            self._draw_voronoi_cell(panel, self.diagram.main_site)
            if panel.draw_cell_triangulation:
                self._draw_voronoi_cell_triangulation(panel, 
                                                      self.diagram.main_site)
        # draw labels
        panel.draw_line(str(index) + "/" + str(self.number), Color.RED)
        panel.draw_line("node:" + str(self.label), Color.ORANGE)
        if VoronoiDiagram.draw_fps:
            panel.draw_line("FPS:" + str(panel.fps), Color.BLUE)
        if VoronoiDiagram.draw_frame_counter:
            panel.draw_line("#" + str(panel.frame_counter), Color.GREEN)
        if VoronoiDiagram.draw_diagram_version:
            panel.draw_line("@" + str(self.version), Color.ORANGE)


