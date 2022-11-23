"""This module implements the UI of the poly area program

It is also the entry point (the module you need to run to execute the program).
derived from BaseObject and provides hooks (overriable methods) for event
handling and collision checking.
"""

import sys
import polygon
from mainloop import BaseObject, MainLoop
import helpers
from pygame_objects import *
from functools import partial
from pprint import pprint as pp
  
def round_int(n, r):
  """Rounds an integer n to the closest multiple of an integer r"""
  return int(n + r / 2) // r * r
  
def verifyGridPoint(p, dx, dy):
  assert (p[0] % dx == 0) and (p[1] % dy == 0)

def window2grid(x, grid):
  """Convert an object from from the main window coordinate system to the grid
  
  The object may be a point, a segment or a sequence
  of segments.
  """
  # Check if it's a point
  if (len(x) == 2 and
      type(x[0]) in (int, long, float) and
      type(x[1]) in (int, long, float)):
    return (x[0] - grid.rect[0], x[1] - grid.rect[1])
    
  return type(x)(window2grid(o) for o in x)
  
def grid2window(point, grid):
  """Convert a point from the grid co-ordinate system to the main window"""
  return (point[0] + grid.rect[0], point[1] + grid.rect[1])

def snapToGrid(point, dx, dy, grid):
  """Move a point to the nearest grid point"""
  p = [round_int(point[0], dx),
       round_int(point[1], dy)]
  return tuple(p)

class PolyMainLoop(MainLoop):
  def __init__(self, screen_size, caption, background_color=(255, 255, 255)):
    MainLoop.__init__(self, screen_size, caption, background_color)
    self.prevPos = None
    self.startpos = None
    self.oldLine = None
    self.polygon_complete = False
    self.area = 0
    self.firstPoint = None
    self.triangles = []
        
  def fixPoint(self, point):
    """Fix the point so it doesn't cross any polygon line and respects the grid
    
    The target point is the end point of the next segment of the polygon
    (the start point is either the end point of the previous segment or the
    first point if its the first segment).
    
    This segment must not intersect with any other segment and also must
    not overlap with any other segment. 
    
    """
    # Restrict the point to grid points
    grid = self.objects[0]
    dx = grid.delta_x
    dy = grid.delta_y

    s2g = partial(snapToGrid, dx=dx, dy=dy, grid=grid)

    # Normalize the point to the grid
    w2g = window2grid
    sp = w2g(self.startpos, grid) if self.startpos else None
    p = w2g(point, grid)
    
    # It's the first point
    if not sp or not self.firstPoint:
      return grid2window(s2g(p), grid)
    
    # Find all the polygon lines  
    poly_lines = [x for x in self.objects if isinstance(x, Line)]
    # Remove the oldLine
    if self.oldLine in poly_lines:
      poly_lines.remove(self.oldLine)
    # Convert polylines to grid co-ordinates  
    poly_lines = [(w2g(line.startpos, grid), w2g(line.endpos, grid))
                  for line in poly_lines]

    if self._checkEndPoint(sp, p, poly_lines, s2g):
      return grid2window(s2g(p), grid)
    
    intersections = self._findIntersections(sp, p, poly_lines, s2g)
            
    # If there are any real intersections
    if intersections:
      # There were intersections. Find the nearest one and restrict the point
      # to the nearest grid point that doesn't intersect.
      nearest = None
      distance = sys.maxint
      
      # Find the nearest intersection by iterating over
      # all the intersection points and keeping the intersection point
      # whose distance from the start position is the shortest.
      for intersection in intersections:
        d = helpers.calc_distance(intersection, sp)
        if d < distance:
          distance = d
          nearest = intersection
                
      xp = s2g(nearest)
            
      #Find all potential 8 neighbors
      neighbours = [
        (xp[0] - dx, xp[1] - dy),
        (xp[0], xp[1] - dy),
        (xp[0] + dx, xp[1] - dy),
        (xp[0] - dx, xp[1]),
        (xp[0] + dx, xp[1]),
        (xp[0] - dx, xp[1] + dy),
        (xp[0], xp[1] + dy),
        (xp[0] + dx, xp[1] + dy),
        ]

      # Eliminate neighbours outside the grid (if xp is on edge or corner)
      all_neighbours = neighbours[:]
      neighbours = [n for n in neighbours if
                    (0 <= n[0] < grid.rect[2] - grid.rect[0]) and
                    (0 <= n[1] < grid.rect[3] - grid.rect[1])]
      
      
      assert neighbours != []
      # Calculate the distance of each neighbour from the start point
      # and pick the closest one
      p = None
      selected_seg = None
      distance = sys.maxint
      
      for i, n in enumerate(neighbours):
        #d = helpers.calc_distance(n, sp)
        d = helpers.calc_distance(n, point)
        if d < distance:
          if self._checkEndPoint(sp, n, poly_lines, s2g):
            distance = d
            p = n

        
      if not p:
        return None
         
    p = s2g(p)
    # Offset the result point back to the main window coordinate system
    p = grid2window(p, grid)
    
    return p
        
  def _checkEndPoint(self, sp, p, poly_lines, s2g):
    """Check if the current segment from start point to end point is valid
    
    The current segment is the segment (self.startpos, point). It is valid
    if it doesn't intersect with or overlap any existing polygon line.
    
    The function also tests the segment (self.startPos, snapToGrid(p))
    """
    if len(poly_lines) == 0:
      return True
            
    seg = (sp, p)
    seg2 = (sp, s2g(p))
    
    # 0 length segments don't count
    if seg[0] == seg[1]:
      return False
    
    if seg2[0] == seg2[1]:
      return False
              
    # Check intersecton with all poly lines except the last
    for line in poly_lines[:-1]:
      if helpers.intersect(seg, line):
        return False
            
      if helpers.intersect(seg2, line):
        return False

    # Check overlap with first and last lines only
    for line in poly_lines:
      if helpers.overlap(seg, line):
        return False
            
      if helpers.overlap(seg2, line):
        return False

    return True
  
  def _findIntersections(self, sp, p, poly_lines, s2g):
    """Find all intersection points between the segment (sp, p) and poly_lines

    It also finds overlaps and touches (any common point)    
    """
    intersections = []
    seg_1 = (sp, p)
    seg_2 = (sp, s2g(p))
    for line in poly_lines[:-1]:
      # Detect intersection
      intersection = helpers.intersect(seg_1, line)
      if intersection:
        intersections.append(intersection)
        
      intersection = helpers.intersect(seg_2, line)
      if intersection and not intersection in intersections:
        intersections.append(intersection)

    pp = s2g(self.firstPoint)
    for i in intersections:
      if abs(pp[0] - i[0]) < 0.001 and abs(pp[1] - i[1]) < 0.001:
        intersections.remove(i)
        break

    for line in poly_lines:
      # Detect overlaps (use snap to grid point)
      overlaps = helpers.overlap(seg_2, line)
      intersections += list(overlaps)
      
    return intersections

    
  def _do_algorithm(self):
    """ """
    points = []
    lines = [o for o in self.objects if isinstance(o, Line)]
    points = [line.startpos for line in lines]
    index = points.index(self.firstPoint)
    points = points[index:]
    
    polygon.calc_polygon_area([polygon.Polygon(points)], self._on_triangle_found)
    
  def _paint_next_triangle(self):
    if self.triangles == []:
      return
    
    triangle = self.triangles[0]
    self.triangles = self.triangles[1:]
    
    # Add current triangle to mainloop's objects
    self.objects.append(Triangle(triangle[0]))
    
    # Update accumulated area and update text
    self.area += triangle[1]
    area = round(self.area, 3)
    if area == int(area):
      area = int(area)
    self.objects[1].text = 'Area: ' + str(area)
    
  def _on_triangle_found(self, triangle, area):
    if triangle is not None:
      self.triangles.append((triangle, area / (grid_delta_x * grid_delta_y)))
      triangle = [(int(p[0] / grid_delta_x), int(p[1] / grid_delta_y))
                   for p in triangle]
      print triangle, self.triangles[-1]
      print
      
  def _reset(self, remove_triangles=True):
    self.polygon_complete = False
    self.area = 0
    if remove_triangles:
      self.triangles = []
    if self.oldLine in self.objects:
      self.objects.remove(self.oldLine)
    self.prevPos = None
    self.startpos = None
    self.firstPoint = None
    self.oldLine = None
    self.objects[1].text = 'Area: ???'
        
  def _onMouseDown(self, event):
    """ """
    if self.polygon_complete:
      return
    
    # Ignore repeated clicks on the same spot
    if self.prevPos == event.pos:
      return
    
    self.prevPos = event.pos
    
    sp = self.fixPoint(event.pos)
    print '*** _onMouseDown()', event.pos, self.startpos
    if not sp:
      return
    # Ignore repeatd clicks on the first point
    if self.oldLine is not None:
      if sp == self.oldLine.startpos == self.oldLine.endpos:
        return
            
    self.startpos = sp
    
    if self.firstPoint is None:
      self.firstPoint = sp
    
    if self.oldLine is not None:
      line = Line(color=(0, 255, 0),
                  startpos=self.oldLine.startpos,
                  endpos=self.startpos,
                  width=3)
      self.objects.append(line)
    #if self.oldLine is not None:
      if self.startpos == self.firstPoint:
        self.objects.remove(self.oldLine)
        self.oldLine = None
        self.polygon_complete = True
        self._do_algorithm()
  
  def _onMouseMove(self, event):
    """ """
    if self.polygon_complete:
      return
    if self.startpos is None:
      return
    
    # Remove old line
    if self.oldLine is not None and self.oldLine in self.objects:
      self.objects.remove(self.oldLine)
    bounds = self.screen.get_rect()
    assert 0 <= event.pos[0] < bounds[2]
    assert 0 <= event.pos[1] < bounds[3]
    end_pos = self.fixPoint(event.pos)    
    if end_pos is None:
      return
  
    #print '---------'
    #p1 = (event.pos[0] - 10, event.pos[1] - 10)
    #p2 = (end_pos[0] - 10, end_pos[1] - 10)
    #print  'event.pos:', p1[0] / 30, p1[1] / 30
    #print  'end_pos  : ', p2[0] / 30, p2[1] / 30
  
    # Add current line to mainloop's objects
    line = Line(color=(255, 0, 0),
                startpos=self.startpos,
                endpos=end_pos,
                width=3)
    self.objects.append(line)
    
    # Remember the current line so it can be removed when the mouse moves
    self.oldLine = line
  
  def handle_events(self, events):
    for event in events: 
      if event.type == pygame.QUIT: 
        sys.exit() 
      elif event.type == pygame.MOUSEBUTTONDOWN:
        self._onMouseDown(event)
      elif event.type == pygame.MOUSEMOTION:
        self._onMouseMove(event)
      elif event.type == pygame.KEYDOWN:
        if event.key == pygame.K_SPACE:
          self._paint_next_triangle()
        elif event.key == pygame.K_n:
          # Keep just the grid and the area text
          self.objects = self.objects[:2]
          self._reset()
        #elif event.key == pygame.K_n:
        #  self._reset()
        


def nop(*args, **kw):
  pass

def main(render_func=nop):
  mainloop = PolyMainLoop(main_window_size,
                          main_window_title,
                          main_window_bg_color)
  # Create grid object
  delta_x = 30
  delta_y = 30
  padding = 10
  grid = Grid(rect=pygame.rect.Rect(grid_padding,
                                    grid_padding,
                                    main_window_size[0] - 2 * grid_padding,
                                    main_window_size[1] - 2 * padding),
              delta_x=grid_delta_x,
              delta_y=grid_delta_y,
              background_color=grid_bg_color,
              border_color=grid_border_color,
              line_color=grid_line_color )
  # Add grid to the main loop's list of objects
  mainloop.objects.append(grid)
  mainloop.objects.append(Text('Area: ???',
                               size=text_size,
                               color=text_color,
                               pos=text_pos))
  
  # Call the custom render function
  render_func(mainloop)
  
  # Run the main loop
  mainloop.run()

  
if __name__=='__main__':
  main()


