
import random
import time
from point2D import Point2D
from preview import Preview
from delaunay import Delaunay


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

