import pygame
from mainloop import BaseObject
from config import *
  
def round_int(n, r):
  """Rounds an integer n to the closest multiple of an integer r"""
  return int(n + r / 2) // r * r
  
class Grid(BaseObject):
  def __init__(self, rect, delta_x, delta_y,
               background_color, border_color, line_color):
    self.rect = rect
    self.delta_x = delta_x
    self.delta_y = delta_y
    self.background_color = background_color
    self.border_color = border_color
    self.line_color = line_color
        
  def render(self, screen):
    """Draws a grid on the screen"""
  
    def dashed_line(screen, line_color, start_pos, end_pos):
      """Draw a dashed line (must be horizontal or verticsl)"""
      horiz_line_count = (end_pos[0] - start_pos[0])/10
      vert_line_count = (end_pos[1] - start_pos[1])/10
      if vert_line_count == 0:
        #draw horizontal lines
        for i in range(horiz_line_count):
          y = start_pos[1]
          x1 = i * 10 + start_pos[0]
          x2 = x1 + 5
          pygame.draw.line(screen, line_color, (x1, y), (x2, y))
      elif horiz_line_count == 0:
       #draw vertical lines
       for i in range(vert_line_count):
          x = start_pos[0]
          y1 = i * 10 + start_pos[1]
          y2 = y1 + 5
          pygame.draw.line(screen, line_color, (x, y1), (x, y2))
      else:
        raise Exception('Line must be horizontal or vertical')  
    
    # Erase the contents of the grid
    pygame.draw.rect(screen, self.background_color, self.rect)
    
    
    # Draw the grid's border
    pygame.draw.rect(screen, self.border_color, self.rect, 3)
    
    # Draw the grid lines
    y = self.rect.top + self.delta_y
    x = self.rect.left + self.delta_x
    while y < self.rect.bottom:
      #pygame.draw.line(screen, line_color, (rect.left, y), (rect.right, y))
      dashed_line(screen, self.line_color, (self.rect.left, y), (self.rect.right, y))
      y += self.delta_y
    while x < self.rect.right:
      dashed_line(screen, self.line_color, (x, self.rect.top), (x, self.rect.bottom))
      x += self.delta_x
    
class Line(BaseObject):
  def __init__(self, color, startpos, endpos, width=1):
    self.startpos = startpos
    self.endpos = endpos
    self.color = color
    self.width = width
    
  def render(self, screen):
    pygame.draw.line(screen, self.color, self.startpos, self.endpos, self.width)

class Triangle(BaseObject):
  def __init__(self,
               points,
               color=triangle_border_color,
               bg_color=triangle_bg_color, width=1):
    self.points = points
    self.color = color
    self.bg_color = bg_color
    self.width = width
    
  def render(self, screen):
    # Draw the filled interior first
    pygame.draw.polygon(screen, self.bg_color, self.points, 0)
    # Draw the border
    pygame.draw.polygon(screen, self.color, self.points, self.width)

class Text(BaseObject):
  def __init__(self, text, font=None, size=36, color=(0, 0, 0), pos=(0, 0)):
    self.text= text
    self.font = pygame.font.Font(font, size)
    self.color = color
    self.pos = pos
    
  def render(self, screen):  
    text = self.font.render(self.text, 1, self.color)
    screen.blit(text, self.pos)


