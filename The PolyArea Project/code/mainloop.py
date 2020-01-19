"""This module define a simple high-level PyGame framework

It defines a BaseObject and MainLoop classes. The MainLoop manages objects
derived from BaseObject and provides hooks (overriable methods) for event
handling and collision checking.
"""

import sys
import pygame
import pygame.locals

class BaseObject(object):
  """A base class for visual objects managed by the main loop
  
  Each object has an __init__(), update() and render() methods.
  To use it sub-class BaseObject implement the methods and add
  your object to the main loop.
  """
  def __init__(self):
    """ """

  def update(self):
    """ """
    
  def render(self, screen):
    """ """

class MainLoop(object):
  """A class that manages a list of objects and implements the PyGame main loop.
  
  It initializes the PyGame main window, maintains a list of objects and in each
  iteration fo the main loop it handles events, updates the various objects,
  handles collisions and renders all the objects.
  
  The class can be used as is for simple programs that just draw on the screen
  and don't care about events. You can just instantiate it, add objects and call
  the run() method, which will continue the update() and render() your objects
  in each iteration.
  
  The class can also be used as a base class for more complicsted programs. You
  should subclass it with your your own class and implement the handle_events()
  and handle_collisions() methods.  
  """
  
  def __init__(self, screen_size, caption, background_color=(255, 255, 255)):
    """Initialize PyGame's main window display and the object list
  
    - screen_size : tuple of 2 integers
      the width and height of the screen (e.g. (800, 600))
      
    - caption: string
      the caption of the main window
      
    - background color: tuple of 3 integers in the range 0..255
      the background color of the main window (defaults to white)
    """
    self.objects = []
    pygame.init()
    self.screen = pygame.display.set_mode(screen_size)
    self.screen.fill(background_color)
    pygame.display.set_caption(caption)
    
  def run(self):
    """Run PyGame's event loop
    
    - Handle events
    - Update objects
    - Handle collisions
    - Render objects
    """
    while True:
      self.handle_events(pygame.event.get())
      self._update_objects()
      self.handle_collisions()
      self._render_objects()
      
  def handle_events(self, events):
    """ """
    if pygame.locals.QUIT in events:
      sys.exit()

  def handle_collisions(self):
    """ """

  def _update_objects(self):
    """ """
    for o in self.objects:
      o.update()
        
  def _render_objects(self):
    """ """
    for o in self.objects:
      o.render(self.screen)
    pygame.display.flip()
