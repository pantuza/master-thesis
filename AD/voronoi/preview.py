"""
Preview of a Delaunay Triangulation and Voronoi Diagram using matplotlib.
"""
from __future__ import division
import pygame
from delaunay import Delaunay
import threading
import time
from math import ceil
from timer import Timer

class Color(object):
    CYAN = pygame.Color(0, 183, 235, 255)
    YELLOW = pygame.Color(255,255,0, 255)
    MAGENT = pygame.Color(255,20,147, 255)
    ORANGE = pygame.Color(255,165,0, 255)
    RED = pygame.Color(255, 0, 0, 255)
    BLUE = pygame.Color(0, 0, 255, 255)
    GREEN = pygame.Color(0, 255, 0, 255)
    GRAY1 = pygame.Color(80,80,80, 255)
    GRAY2 = pygame.Color(120,120,120, 255)
    BLACK = pygame.Color(0,0,0, 255)
    WHITE = pygame.Color(255,255,255, 255)

class Painter():
    '''
    A simple painter
    A painter object must to have this interface:

    def __init__(self, number, view):
        """
        Receive a identification number and a object (view) to paint
        """
    def changed(self):
        """
        Informs if the state changed and need to draw a new version
        """
    def draw(self, panel, index):
        """
        self.viewsbject in this panel with this index (relative to previewer)
        """
    '''
   
    def __init__(self, number, view):
        self.number = number
        self.text = str(view) 
    
    def changed(self):
        return False
    
    def draw(self, panel, index):
        panel.clear()
        panel.draw_line(str(index) + "/" + str(self.number), Color.RED)
        panel.draw_line("FPS:" + str(panel.fps), Color.BLUE)
        panel.draw_line("Object:" + self.text, Color.BLUE)



class Panel(object):
    '''
    Panel object storages data about diagram window (surface)
    It's like a record structure
    '''
    
    def __init__(self, display, width, height, position):
        self.display = display
        self.size = (width, height)
        self.surface = pygame.Surface(self.size)
        self.position = position
        self.line = 1
        self.bgcolor = Color.WHITE
        self.frame_counter = 0
        self.current_time = time.clock() -1
        self.fps = 0
        # PyGame font resource
        pygame.font.init()
        self.font_size = 12
        self.font = pygame.font.Font(pygame.font.get_default_font(), 
                                     self.font_size)

    def new_frame(self):
        self.frame_counter += 1
        current_time = time.clock()
        elapsed_time = (current_time - self.current_time)
        if elapsed_time > 0:
            self.fps = int(round(1.0/elapsed_time))
        else:
            self.fps= -1
        self.current_time = current_time
        return True

    def draw_line(self, text, color = Color.BLACK):
        '''
         
        '''
        # _draw labels
        self.label = self.font.render(text, 1, color)
        self.surface.blit(self.label, (1, self.line))
        self.line += self.font_size + 2

    def draw(self):
        # apply drawings in correct position
        self.display.blit(self.surface, self.position)
        pygame.display.update(pygame.Rect(self.position, self.size))
        self.line = 1
        self.new_frame()

    def clear(self):
        '''
        Fill the diagram panel with background color 
        '''
        self.surface.set_alpha(255)
        self.surface.fill(self.bgcolor)

    def fill(self, color):
        '''
        Fill the diagram panel with specific color
        '''
        self.surface.set_alpha(255)
        self.surface.fill(color)


class ScrollControl(object):
    '''
    Control the scrolling, independent from direction
    '''
    
    def __init__(self, size, buttom_size):
        assert size > 0 and buttom_size > 0
        self.size = int(size) - 1
        self.buttom_min_size = int(buttom_size)
        self.buttom_size = self.buttom_min_size
        self.selected = 0
        self.capacity = 0
        self.last_selected = 0
        self.position = 0
        self.capacity_per_panel = 0
        self.set_capacity(0, 0)

    def set_capacity(self, capacity, capacity_per_panel):
        self.capacity = int(ceil(capacity))
        self.capacity_per_panel = int(capacity_per_panel)
        if self.capacity == 0 or self.capacity_per_panel == 0 \
        or self.capacity < self.capacity_per_panel:
            self.buttom_size = self.size
            self.unit_size = 0
            self.inverse_unit_size = 0
            self.max_selectable = 0
        else:
            self.unit_size = float(self.size) / float(self.capacity)
            self.inverse_unit_size = float(self.capacity) / float(self.size) 
            self.buttom_size = self.unit_size * self.capacity_per_panel
            self.max_selectable = self.capacity - self.capacity_per_panel
        if self.buttom_size < self.buttom_min_size:
             self.buttom_size = self.buttom_min_size
        self.last_selected = -1
        self.select(self.position)

    def _update_selection(self):
        assert self.selected >= 0
        if self.selected != self.last_selected:
            if self.selected > self.max_selectable:
                self.top_selected = self.max_selectable
            else:
                self.top_selected = self.selected
            self.top_position = self.top_selected * self.unit_size
            if self.top_position > (self.size - self.buttom_size):
                self.top_position = (self.size - self.buttom_size)
            self.last_selected = self.selected
            return True
        return False

    def select(self, position):
        assert position <= self.size and position >= 0
        self.position = position
        self.selected = int(self.position * self.inverse_unit_size)
        return self._update_selection()

    def shift(self, lines):
        assert lines != 0
        if (self.selected + lines) > self.max_selectable:
            self.selected = self.max_selectable
        elif (self.selected + lines) < 0:
            self.selected = 0
        else:
            self.selected += lines
        return self._update_selection()


class VerticalScroll(object):
    '''
    Vertical scrollbar
    '''
    
    BUTTOM_SIZE = 16

    def __init__(self, display, 
                 buttom_width = BUTTOM_SIZE, 
                 buttom_height = BUTTOM_SIZE):
        self.display = display
        self.left = display.get_width() - buttom_width
        self.top = 0
        self.width = buttom_width
        self.height = display.get_height()
        self.rect = pygame.Rect(self.left, self.top, self.width, self.height)
        self.buttom_rect = self.rect
        self.buttom_top = 0
        self.color = Color.GRAY1
        self.buttom_color = Color.GRAY2
        self.bgcolor = Color.BLACK
        self.control = ScrollControl(self.height, buttom_height)
        self.configure(0, 0)
    
    def configure(self, capacity, capacity_per_panel):
        self.control.set_capacity(capacity, capacity_per_panel)
        self.buttom_height = self.control.buttom_size
        self.buttom_top = self.control.top_position
        self.draw()

    def selected(self):
        return self.control.top_selected

    def change_selection(self, point):
        if self.rect.collidepoint(point):
            if self.control.select(point[1]):
                self.buttom_top = self.control.top_position
                self.draw()
                return True
        return False
    
    def shift(self, lines):
        if self.control.shift(lines):
            self.buttom_top = self.control.top_position
            self.draw()
            return True
        return False

    def draw(self):
        # draw scroller
        pygame.draw.rect(self.display, self.bgcolor, self.rect, 0)
        pygame.draw.rect(self.display, self.color, self.rect, 1)
        # draw buttom
        self.buttom_rect = pygame.Rect(self.left, self.buttom_top,
                                       self.width, self.buttom_height)
        pygame.draw.rect(self.display, self.buttom_color, self.buttom_rect, 0)
        pygame.display.update(self.rect)
        pygame.display.flip()
        self.changed = False


class Grid(object):
    '''
    Grid control with scrollbar
    '''
    
    def __init__(self, display, lines, columns):
        self.diaplay = display
        self.scroll = VerticalScroll(display)
        self.width = display.get_width() - self.scroll.width
        self.height = display.get_height()
        self.set_grid(columns, lines)
        self.painters = []
        self.first_visible_painter = 0
        self.painted_panels = 0
        self.set_grid(columns, lines)
        self.panel_index = -1
        self.selected_painter = None

    def set_grid(self, columns, lines):
        self.columns = columns
        self.lines = lines
        # size of panels
        self.panel_width = int(self.width / self.columns)
        self.panel_height = int(self.height / self.lines)
        self.panels = []
        self.number_of_panels = (self.columns * self.lines)
        for panel_index in xrange(0, self.number_of_panels):
            x = int(panel_index % self.columns) * self.panel_width
            y = int(panel_index / self.columns) * self.panel_height
            self.panels.append(Panel(self.diaplay,
                                     self.panel_width - 1,
                                     self.panel_height - 1,
                                     (x, y)))
        self.painted_panels = 0

    def add_painter(self, painter):
        self.painters.append(painter)
        self.scroll.configure(len(self.painters) / float(self.columns), 
                              self.lines)
        self._update()
        return True

    def del_painter(self, painter):
        try:
            painter_index = self.painters.index(painter)
        except:
            return False
        del self.painters[painter_index]
        self.scroll.configure(len(self.painters) / float(self.columns), 
                              self.lines)
        self._update()
        return True

    def select(self, pos):
        if self.scroll.change_selection(pos): self._update()

    def line_down(self):
        if self.scroll.shift(1): self._update()

    def line_up(self):
        if self.scroll.shift(-1): self._update()

    def page_down(self):
        if self.scroll.shift(self.lines): self._update()

    def page_up(self):
        if self.scroll.shift(-self.lines): self._update()
            
    def _update(self):
        self.first_visible_painter = self.scroll.selected() * self.columns
        self.painted_panels = 0

    def next(self):
        # next panel
        self.panel_index = (self.panel_index + 1) % self.number_of_panels
        # get the index of panel painter
        self.painter_index = self.first_visible_painter + self.panel_index
        assert self.painter_index >= 0
        # verify if painter exists
        if self.painter_index < len(self.painters):
            self.selected_painter = self.painters[self.painter_index]
        else:
            self.selected_painter = None
        # increase the controller of panel drawing 
        # used to update after scrolling
        # by forcing to draw at least the number of visible panels
        self.painted_panels += 1
        return self.selected_painter, self.panels[self.panel_index],\
               self.painter_index, self.painted_panels <= self.number_of_panels

        
class Preview(threading.Thread): 
    '''
    Preview is a thread object that act as a pygame display control
    PyGame issue on Windows (and possibly other OS than Linux):
        In Windows OS, pygame just collects events in a main therad.
        Then, the application must instatiate the Preview 
        and maint it's controler
    '''

    NONE = -1
    STARTING = 0
    RUNNING = 1
    STOPPING = 2
    KILLED = 3
    
    preview = None

    def __init__(self, title = "Grid visualizer", 
                 width = 1000, height = 720, 
                 lines = 2, columns = 2, fps_limit = 0,
                 painter_class = Painter):
        # state control
        self.state = Preview.STARTING
        self.default_painter_class = painter_class
        # thread control
        self.threading = True
        # Window app control
        self.width = width
        self.height = height
        self.bgcolor = Color.BLACK
        self.display = pygame.display.set_mode((width + 
                                                VerticalScroll.BUTTOM_SIZE, 
                                                height), 
                                               pygame.HWSURFACE | 
                                               pygame.DOUBLEBUF)
        pygame.display.set_caption(title)
        # diagram selector
        self.grid = Grid(self.display, lines, columns)
        self.mouse_drag = False
        self.key_pressed = None
        # timer control (to pause, FPS, keyboard)
        self.fps_limit = fps_limit
        self.frame_timer = Timer(0)
        if self.fps_limit > 0:
            self.frame_timer.set_delay(1.0 / self.fps_limit)
        self.key_repeat_delay = 1.0 / 2.0
        self.key_repeat_rate  = 1.0 / 10.0
        self.keyboard_timer = Timer(0)
        # count number of painters/objects
        self.count_painters = 0
        # map object to painter
        self.views = dict()
        # multi-thread control
        self.lock = threading.Lock()
    
    def __del__(self):
        '''
        Automatic object destruction handler
        '''
        pygame.quit()

    def panel_dimensions(self):
        '''
        Get panel dimensions
        '''
        return self.grid.panel_width, self.grid.panel_height

    def set_title(self, title):
        '''
        Change app window caption
        '''
        pygame.display.set_caption(title)
    
    def add_view(self, view, painter_class = None):
        '''
        Include a diagram in previewer
        '''
        assert view is not None
        with self.lock:
            # prevent drawing duplication of same object 
            if view in self.views:
                return False
            # create a painter to object
            if painter_class is None:
                painter = self.default_painter_class(self.count_painters, 
                                                     view)
            else:
                painter = painter_class(self.count_painters, view)
            self.count_painters += 1
            # make control 
            index = self.grid.add_painter(painter)
            self.views[view] = painter
            return view
        
    def del_view(self, view):
        assert view is not None
        with self.lock:
            if view in self.views:
                painter = self.views[view]
                self.grid.del_painter(painter)
                del self.views[view]

    def pause(self, delay = 0.0):
        '''
        Defines a idle time to previewer
        '''
        self.frame_timer.pause(delay)

    def quit(self):
        '''
        Request the termination of panel
        '''
        return self.state == Preview.KILLED
        pygame.quit()

    def is_running(self):
        '''
        Verify if previewer state is RUNNING
        '''
        return self.state == Preview.RUNNING

    def interactive(self, interact):
        '''
        Defines if previewer works as a separate thread or not
        '''
        if self.state == Preview.STARTING:
            self.threading = not interact

    def start(self):
        '''
        Starts previewer
        '''
        # Initialization
        pygame.init()
        self.state = Preview.RUNNING
        if self.threading:
            threading.Thread.__init__(self)
            threading.Thread.start(self)

    def run(self):
        '''
        Run previewer continuously until it's received a termination sign
        '''
        while self.step():
            pass

    def step(self):
        '''
        Executes a step (maybe with a frame drawing) and responds to the events
        Useful in mono threading applications (or main thread), 
        which other class controls the execution and calls this method
        '''
        # exit if don't run
        if self.state != Preview.RUNNING:
            return False
        # simple concurrence control
        with self.lock:
            # pre-process the buffered events 
            self._process_events()
            # if don't need to force, verify timer (FPS limit)
            if self.frame_timer.idle(): 
                return True
            # select a panel
            diagram, panel, index, changed = self.grid.next()
            # if selector change, force to draw
            if changed:
                # if diagram is None, just clear the panel
                if diagram is None:
                    panel.fill(self.bgcolor)
                else:
                    diagram.draw(panel, index)
            else:
                # if diagram is None, there's nothing to do
                if diagram is None:
                    return True
                else:
                    # diagram was changed since last frame?
                    if diagram.changed():
                        diagram.draw(panel, index)
                    else:
                        return True  # go to next step
            panel.draw()
            pygame.display.flip()
            return True

    def _process_events(self):
        '''
        Take actions in response to the events
        '''
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.state = Preview.STOPPING
            elif event.type == pygame.MOUSEBUTTONDOWN:
                self.mouse_drag = True
                if self.grid.select(event.pos):
                    self.update_selector()
            elif  event.type == pygame.MOUSEMOTION:
                if self.mouse_drag:
                    if self.grid.select(event.pos):
                        self.update_selector()
            elif event.type == pygame.MOUSEBUTTONUP:
                self.mouse_drag = False
            elif event.type == pygame.KEYDOWN:
                self.key_pressed = event.key
                self.keyboard_timer.set_delay(self.key_repeat_delay)
                self._process_key(self.key_pressed)
            elif event.type == pygame.KEYUP:
                self.key_pressed = None
                self.keyboard_timer.set_delay(0)
        # process key events
        if self.key_pressed is not None and not self.keyboard_timer.idle():
            self.keyboard_timer.set_delay(self.key_repeat_rate)
            self._process_key(self.key_pressed)
            
    def _process_key(self, key):
        if key == pygame.K_DOWN:
            self.grid.line_down()
        elif key == pygame.K_UP:
            self.grid.line_up()
        elif key == pygame.K_PAGEDOWN:
            self.grid.page_down()
        elif key == pygame.K_PAGEUP:
            self.grid.page_up()


