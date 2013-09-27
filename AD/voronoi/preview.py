"""
Demo of a PathPatch object.
"""
import matplotlib.path as mpath
import matplotlib.patches as mpatches
import matplotlib.pyplot as plt
from delaunay import Delaunay

class Preview(object): 

    def __init__(self, delaunay):
        if not isinstance(delaunay, Delaunay):
            raise Exception("Object is not a Delaunay Triangulation.")
        self.delaunay = delaunay
        self.siteRadius=2
        self.max_x = 400
        self.max_y = 400
        self.min_x = 0
        self.min_y = 0
        self.clear()
        self.ion = True
        plt.ion()
        

    @staticmethod
    def voronoi(delaunay, title = "Voronoi diagram", x = 200, y = 200):
        preview = Preview(delaunay)
        preview.max_x = x
        preview.max_y = y
        preview.drawVoronoi()
        preview.draw()
        preview.setTitle(title)
        preview.interactive(False)
        preview.show()

    def setTitle(self, title):
        if title is not None:
            plt.gcf().canvas.set_window_title(title)
        
    def interactive(self, interac = True):
        self.ion = interac
        if self.ion:
            plt.ion()
        else:
            plt.ioff()
            
    def clear(self):
        self.fig, self.ax = plt.subplots()
        self.Path = mpath.Path

    def _apply(self, path_data, **style):
        # apply drawings
        codes, verts = zip(*path_data)
        path = mpath.Path(verts, codes)
        patch = mpatches.PathPatch(path, **style)
        self.ax.add_patch(patch)

    def draw(self):
        # plot control 
        self.ax.set_ylim([self.min_y, self.max_y])
        self.ax.set_xlim([self.min_y, self.max_x]) 
        plt.draw()

    def show(self):
        plt.show()

    def drawVoronoi(self):
        path_data = []
        # plot the voronoi cells
        cells = self.delaunay.getVoronoiCells()
        for site, cell in cells.items():
            # draw cell
            path_data.append((self.Path.MOVETO, cell[len(cell) - 1]))
            for point in cell:
                path_data.append((self.Path.LINETO, point))
            #draw site
            self.drawPoint(site)
        # apply drawings
        self._apply(path_data, facecolor='w', alpha=0.5)

    def drawVoronoiCell(self, theSite):
        path_data = []
        # plot the voronoi cells
        cells = self.delaunay.getVoronoiCells()
        for site, cell in cells.items():
            if site == theSite:
                # draw cell
                path_data.append((self.Path.MOVETO, cell[len(cell) - 1]))
                for point in cell:
                    path_data.append((self.Path.LINETO, point))
                    #draw site
                    self.drawPoint(site)
                # apply drawings
                self._apply(path_data, facecolor='r', alpha=0.5)
                return

    def drawVoronoiCellTriangulation(self, site):
        path_data = []
        # plot the voronoi cells
        triangules, initial = self.delaunay.getVoronoiCellTrinagulation(site)
        opacity = 0.35
        self.drawPoint(site, color='r')
        for triangle in triangules:
            self.drawTriangle(triangle, facecolor='g', alpha=opacity)
        self.drawTriangle(initial, facecolor='b', alpha=opacity)
        self.drawNeighbors(initial)

    def drawTriangle(self, triangle, **style):
        path_data = [(self.Path.MOVETO, triangle[2])]
        for point in triangle:
            path_data.append((self.Path.LINETO, point))
        self._apply(path_data, **style)

    def drawTriangulation(self):
        for triangle in self.delaunay.neighborhood.keys():
            self.drawTriangle(triangle, color='b', fill=False, alpha=0.2)

    def drawCircumcircles(self):
        for triangle in self.delaunay.neighborhood.keys():
            center = triangle.getCircumcenter()
            if (center[0] <= self.max_x) and (center[1] <= self.max_y) and \
               (center[0] >= self.min_x) and (center[1] >= self.min_y):
                self.drawCircle(center, triangle.getCircumcircleRadius(), color = 'y')

    def drawCircle(self, point, radius, color='g'):
        circle=plt.Circle(point, radius, color=color, fill=False, alpha=0.8)
        self.fig.gca().add_artist(circle)

    def drawPoint(self, point, color='b'):
        circle=plt.Circle(point, self.siteRadius, color=color, fill=True)
        self.fig.gca().add_artist(circle)

    def drawNeighbors(self, triangle):
        #triangle = self.delaunay._findTriangleContains(test)
        if triangle is not None:
            neighborhood = self.delaunay.neighborhood[triangle]
            for neighbor in neighborhood:
                self.drawTriangle(neighbor, color='r', alpha=0.3)
    
if __name__ == '__main__':

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

    max = 200
    
    # test mode: manual cleanup: clear_main_site()
    d = Delaunay()
    d.addMainSite(Point2D(100, 100))
    d.add(Point2D(100, 100))
    for site in sites:
        d.add(site)
    d.clear_main_site()
    Preview.voronoi(d, title = "Manual 'main site' cleanup mode")
    
    # test mode: automatic cleanup: addSurrounding(site)
    d = Delaunay()
    d.addMainSite(Point2D(100, 100))
    d.add(Point2D(100, 100))
    for site in sites:
        d.addSurrounding(site)
    Preview.voronoi(d, title = "Automatic 'main site' cleanup mode")
    
    
    d = Delaunay()
    d.addMainSite(Point2D(100, 100))
    d.add(Point2D(100, 100))
    for site in sites:
        d.add(site)

    preview = Preview(d)
    preview.interactive(False) 
    preview.max_x = max
    preview.max_y = max
    #preview.min_x = -max
    #preview.min_y = -max
    preview.setTitle("Simulation of remove(site)")
    preview.draw()
    preview.drawVoronoi()
    preview.drawTriangulation()
    preview.drawCircumcircles()
    #preview.drawVoronoiCellTriangulation(Point2D(100.0,100.0))
    preview.show()
    

    preview.interactive(True) 
    import random
    deleted = []
    while len(sites) > 0:
        i = random.randint(0, len(sites) - 1)
        if i in deleted:
            continue
        deleted.append(i)
        preview.drawVoronoi()
        #preview.drawTriangulation()
        #preview.drawCircumcircles()
        preview.drawVoronoiCell(sites[i])
        preview.draw()
        preview.clear()
        d.remove(sites[i])
