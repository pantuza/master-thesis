"""
Demo of a PathPatch object.
"""
import matplotlib.path as mpath
import matplotlib.patches as mpatches
import matplotlib.pyplot as plt

class Preview(object): 

    @staticmethod
    def draw(voronoi, siteRadius=0.5, width = 400, height = 400):
        fig, ax = plt.subplots()
        Path = mpath.Path
        path_data = []
        # plot the voronoi cells
        cells = voronoi.getCells()
        for site, cell in cells.items():
            path_data.append((Path.MOVETO, cell[len(cell) - 1]))
            for point in cell:
                path_data.append((Path.LINETO, point))
            circle=plt.Circle(site, siteRadius, color='b', fill=True)
            fig.gca().add_artist(circle)
        # apply drawings
        codes, verts = zip(*path_data)
        path = mpath.Path(verts, codes)
        patch = mpatches.PathPatch(path, facecolor='w', alpha=0.5)
        ax.add_patch(patch)
        # plot control 
        ax.set_ylim([0, height])
        ax.set_xlim([0, width])
        plt.show()

if __name__ == '__main__':

    from voronoi import Voronoi
    from point2D import Point2D
    
    v = Voronoi()
    sites= [Point2D(20, 40),
            Point2D(40, 50),
            Point2D(40, 100),
            Point2D(60, 60),
            Point2D(80, 90),
            Point2D(100, 20),
            Point2D(30, 150),
            Point2D(125, 125),
            Point2D(100, 100),
            Point2D(100, 150),
            Point2D(150, 100),
            Point2D(150, 150),
            Point2D(130, 50)
            ]
    for s in sites:
        v.add(s)

    Preview.draw(v, width = 200, height = 200)

        
