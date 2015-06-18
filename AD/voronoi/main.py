# -*- coding: utf-8 -*-

import pygame

from src.delaunay import Delaunay


class Window:

    WIDTH = 1024
    HEIGHT = 800
    SIZE = WIDTH, HEIGHT

    COLORS = {"green": pygame.Color(0, 255, 0),
              "blue": pygame.Color(0, 0, 255),
              "red": pygame.Color(255, 0, 0),
              "yellow": pygame.Color(255, 255, 0),
              "white": pygame.Color(255, 255, 255),
              "black": pygame.Color(0, 0, 0)}

    def __init__(self):
        pygame.init()

        self.screen = pygame.display.set_mode(self.SIZE)
        self.surface = pygame.Surface((self.WIDTH-50, self.HEIGHT-50))

    def on_event(self, event):

        if event.type == pygame.QUIT:
            pygame.quit()

    def set_title(self, title):

        if title is None:
            raise ValueError("Title could not be None")
        pygame.display.set_caption(title)

    def circle(self, color, pos, radius):
        pygame.draw.circle(self.surface, color, pos, radius)

    def draw(self):
        self.surface.fill(self.COLORS['white'])

    def flip(self):
        self.screen.blit(self.surface, (25,25))
        pygame.display.flip()
