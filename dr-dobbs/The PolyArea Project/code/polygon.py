"""The Polygon class
"""

import os
import sys
import math
import helpers
from helpers import intersect, round_points
from triangle import calc_triangle_area


class Polygon(object):
  def __init__(self, points):
    self.points = round_points(points)
    self.sort_points()
    self.invariant()
    
  def sort_points(self):
    def compare_points(p1, p2):
      if p1 == p2:
        return 0
      if p1[1] > p2[1]:
        return -1
      if p1[1] < p2[1]:
        return 1
      # Same y-coordinate
      assert p1[1] == p2[1]
      if p1[0] < p2[0]:
        return -1
      else:
        assert p1[0] != p2[0]
        return 1    
    self.sorted_points = sorted(self.points, cmp=compare_points)    
  
  def invariant(self):
    assert len(self.points) > 2
    for p in self.points:  
      assert len(p) == 2
      
    for i, p in enumerate(self.sorted_points[1:]):
      p2 = self.sorted_points[i]
      assert p2[1] >= p[1]
      if p[1] == p2[1]:
        assert p2[0] < p[0]
      
    # Make sure there are no duplicates
    assert len(self.points) == len(set(self.points))
    
    # Make sure there no 3 consecutive points with the same X or Y coordinate
    point_count = len(self.points)
    for i in xrange(point_count):
      p = self.points[i]
      p1  = self.points[(i + 1) % point_count]
      p2  = self.points[(i + 2) % point_count]
      assert not (p[0] == p1[0] == p2[0])
      assert not (p[1] == p1[1] == p2[1])
      
    # Make sure no vertex resides on a side
    sides = []
    for i in xrange(len(self.points)):
      sides.append((self.points[i], self.points[(i+1) % len(self.points)]))

    for p in self.points:
      for side in sides:
        if p != side[0] and p!= side[1]:
          assert not helpers.point_on_segment(p, side)
    
  def __repr__(self):
    s = '<' + ','.join('(%f, %f) ' % (p[0], p[1]) for p in self.points) + '>'
    return s

  def __str__(self):
    return self.__repr__()

  def find_second_top_point(self):
    """Find the point whose y co-ordinate is the second from the top.
    
    Note that there may be multiple points with the same Y co-ordinate. This
    applies both to top points and second top points. The returned second top
    point is the leftmost point. For example if the points are:
    (5,10), (6,10), (7,10), (8,4), (12, 4)
    
    Then there are 3 top points with Y=10 and 2 second top points with Y=4.
    The returned second top point will be (8,4) because it is to the left of
    (12, 4).
    
    The self.sorted_points attibute contains the polygon points already sorted
    first by their y coordinate and then by their x coordinate.
    
    if there are multiple second top points an at least one of them is internal
    then the first internal point will be returned.
    
    Otherwise, the leftmost second top point will be returned
    
    The function retuens the a pair that consists of the selected point
    and its kind (one of 'vertex', 'inside', 'outside')
    """
    top = self.sorted_points[0] 
    top_y = top[1]
    second_top_y = None
    second_top = None
    candidates = []
    # Find the Y co-ordinate of the second top point and all the candidates
    for p in self.sorted_points[1:]:
      if p[1] < top_y:
        if second_top_y is None:
          second_top_y = p[1]
        else:
          if p[1] < second_top_y:
            break # finished with second top candidates
        candidates.append(p)
                
    index = self.points.index(top)
    pred = self.points[index-1]
    post = self.points[(index+1) % len(self.points)]
    assert None not in (pred, post)
    
    # If both pred and post are candidates and there is another candidate
    # between them then pick the candidate in between as an 'inside' point
    if pred in candidates and post in candidates:
      pred_index = self.sorted_points.index(pred)
      post_index = self.sorted_points.index(post)
      if abs(post_index - pred_index) > 1:
        # there is a candidate between pred and post
        index = min(pred_index, post_index) + 1
        assert index < max(pred_index, post_index)
        p = self.sorted_points[index]
        assert p in candidates
        return (p, 'inside')

    # If either pred or post are candidates and there is another candidate
    # between them then pick the candidate in between as an 'inside' point
    if pred in candidates:
      # Find the point p on (top, post) where y = second_top. If there is a
      # candidate whose X coordinate is between pred.x and p.x then it is the
      # second top point and it's an 'inside' point'
      p = helpers.intersect_sweepline((top, post), second_top_y)
      if p is not None:
        left_x = min(pred[0], p[0])
        right_x = max(pred[0], p[0])
        for c in candidates:
          if left_x < c[0] < right_x:
            return (c, 'inside')

    if post in candidates:
      # Find the point p on (top, pred) where y = second_top. If there is a
      # candidate whose X coordinate is between post.x and p.x then it is the
      # second top point and it's an 'inside' point'
      p = helpers.intersect_sweepline((top, pred), second_top_y)
      if p is not None:
        left_x = min(post[0], p[0])
        right_x = max(post[0], p[0])
        for c in candidates:
          if left_x < c[0] < right_x:
            return (c, 'inside')

    second_top = candidates[0]
    assert second_top[1] < top_y
        
    # If the second top point is either pred or post then it is a 'vertex'
    if second_top in (pred, post):
      return (second_top, 'vertex')
    
    # If pred or post are at the same height as top then second top is 'outside'
    # if the second top is vertically between pred and post, otherwise it's
    # inside
    if max(pred[1], post[1]) == top_y:
      if min(pred[0], post[0]) < second_top[0] < max(pred[0], post[0]):
        return (second_top, 'inside')
      else:
        return (second_top, 'outside')

    
    # Check if pred or post are at the same height as the second top point.
    # If this is the case then the second top point must be outside.
    if second_top[1] in (pred[1], post[1]):
      return (second_top, 'outside')
      
    # If we got here then the second top point may be either 'inside'
    # or 'outside'. To figure it out we find the intersection of the
    # side lines of the top triangle with the sweepline that goes
    # through the Y co-ordinate of the second top point
    pred = helpers.intersect_sweepline((pred, top), second_top[1])
    assert pred is not None
    post = helpers.intersect_sweepline((top, post), second_top[1])
    assert post is not None
    
    # Pred and post are both fixed to be on the sweepline at this point
    # if they weren't already. Find their left and right X-coordinate
    left_x = min(pred[0], post[0])
    right_x = max(pred[0], post[0])

    # If the second_top_point is between post and pred it's internal
    # otherwise it's outside
    if left_x < second_top[0] < right_x:
      kind = 'inside'
    else:
      kind = 'outside'
    return (second_top, kind)
    
  def is_triangle(self):
    """Cechk if the polygon is a triangle"""
    return len(self.points) == 3

def split(poly, diagonal):
  """Split a polygon into two polygons along a diagonal
  
  poly: the target simple polygon
  diagonal: a line segment that connects two vertices of the polygon
  
  The polygon will be split along the diagonal. The diagonal vertices will
  be part of both new polygons
  
  Return the two new polygons.
  """
  assert type(diagonal) in (list, tuple)
  assert len(diagonal) == 2
  assert diagonal[0] in poly.points
  assert diagonal[1] in poly.points
  assert diagonal[0] != diagonal[1]
  
  index = poly.points.index(diagonal[0])
  poly1 = [diagonal[0]]
  for i in range(index + 1, len(poly.points) + index):
    p = poly.points[i % len(poly.points)]
    poly1.append(p)
    if p == diagonal[1]:
      break;
    
  index = poly.points.index(diagonal[1])
  poly2 = [diagonal[1]]
  for i in range(index + 1, len(poly.points) + index):
    p = poly.points[i % len(poly.points)]
    poly2.append(p)
    if p == diagonal[0]:
      break;
  
  poly1 = helpers.filter_consecutive_points(poly1)
  poly2 = helpers.filter_consecutive_points(poly2)
  
  return [Polygon(poly1), Polygon(poly2)]  

def remove_top_triangle(poly, second_top_point, kind):
  """Remove the top triangle of a polygon and return it
    
  There are two cases:
  
  1. The top triangle has one top point. For example:
  
        o
      /  |_
     /_____|
  
  2. The top triangle has two top points. For example:
  
    o__o
    \  \
    |___\
  
  
  If there are two top points, the main top point will be the one with lower
  x coordinate (the leftmost one). In both cases a sweepline is used to find
  the y coordinate of the highest point if all the top points are removed.
  
  In case 1, the top triangle formed by the top point and the two points
  that are the intersection of the sweepline with the line segments to
  the left and right of the top point.
  
  In case 2, the top triangle is formed by both top points and the
  intersection of the line segment to the left of the main top point
  with the sweepline.
  
  This top triangle is removed from the polygon simply by removing the top
  point and the triangle is returned.
  """
  poly.invariant()
  assert not poly.is_triangle(), 'Polygon cannot be a triangle'
  assert second_top_point in poly.points
  assert kind in ('vertex', 'outside'), 'Inside second top point is not allowed'
  
  
  # Get the top point and its index  
  top_point = poly.sorted_points[0]
  index = poly.points.index(top_point)
  
  # Make sure the top point is really above the second top point
  assert top_point[1] > second_top_point[1]
  
  # Create the sweepline
  x1 = -sys.maxint
  x2 = sys.maxint
  second_top = second_top_point[1]
  sweepline = ((x1, second_top), (x2, second_top))
  
  next_point = poly.points[(index + 1) % len(poly.points)]
  prev_point = poly.points[index - 1]
  
  # check if we are in case 1 (two consecutive top points) or 2
  new_points = []
  
  if max(next_point[1], prev_point[1]) == top_point[1]:
    # Case 1 - platuae
    
    # The new point is the intersection of the segment from the non-platuae
    # point with the sweepline (if the other point is the second top point it
    # stays as is).
    p = next_point if next_point[1] < prev_point[1] else prev_point
    other_point = prev_point if next_point[1] < prev_point[1] else next_point
    segment = (p, top_point)
    new_point = helpers.intersect(segment, sweepline)
    assert new_point is not None
    new_points = [new_point, other_point]    
  else: 
    # Case 2 - single top point
    
    # The new points are intersection of the prev and next segments with
    # the sweepline (if one of the points is a second top point it stays as is).
    for p in prev_point, next_point:
      if p[1] == second_top_point[1]:
        new_point = p
      else:
        segment = (p, top_point)
        new_point = helpers.intersect(segment, sweepline)
        assert new_point is not None
  
      new_points.append(new_point)
  
    
  # The triangle to be removed is simply the top point + the new points
  assert len(new_points) == 2
  triangle = new_points + [top_point]
  
  # The new points that should be added to the polygon after removing the
  # top triangle.
  new_points = round_points(new_points)
  to_add = [p for p in new_points if p not in poly.points]
  
  # Add all the points before the top point (that was removed) and
  # the new points that need to be added and all the points that
  # follow the top point.
  poly.points = poly.points[:index] + to_add + poly.points[index+1:]

  # Find the polygon sides
  sides = []
  for i in range(0, len(poly.points) - 1):
    sides.append((poly.points[i], poly.points[i+1]))
  sides.append((poly.points[-1], poly.points[0]))

  # Remove left vertex of triangle if it has a horizontal bottom and it is
  # on a polygon side
  if new_points[0][1] == new_points[1][1]:
    left_vertex = (min(new_points[0][0], new_points[1][0]), new_points[0][1])
    right_vertex = (max(new_points[0][0], new_points[1][0]), new_points[0][1])

    for side in sides:    
      if helpers.point_on_segment(right_vertex, side):
        if right_vertex not in side:
          poly.points.remove(left_vertex)

  poly.points = helpers.filter_consecutive_points(poly.points)
      
  # If there are points that are in the middle of a side remove them
  to_remove = []
  # Find the polygon sides
  sides = []
  for i in range(0, len(poly.points) - 1):
    sides.append((poly.points[i], poly.points[i+1]))
  sides.append((poly.points[-1], poly.points[0]))

  # Iterate over all the polygon points and find the points
  # that intersect with polygon sides (but not end points of course)
  for i in range(0, len(poly.points)):
    p = poly.points[i]
    for side in sides:
    # If p is on segment s then should be removed
      if p != side[0] and p != side[1]:
        if helpers.point_on_segment(p, side):
          to_remove.append(p)
    
  for p in to_remove:
    poly.points.remove(p)
  
  #print 'triangle: ', triangle
  #print 'after: ', poly.points
  #print '*' * 20
  
  poly.sort_points()

  poly.invariant()  
  return triangle

def calc_polygon_area(polygons, callback):  
  while polygons:
    poly = polygons[0]
    poly.invariant()    
    if poly.is_triangle():
      polygons = polygons[1:]
      triangle = poly.points
    else:
      second_top_point, kind = poly.find_second_top_point()
      if kind == 'inside':
        # need to split the polygon along the diagonal from top to second top
        new_polygons = split(poly, (poly.sorted_points[0], second_top_point))
        polygons = new_polygons + polygons[1:]
        
        # No callback in this iteration because the first polygon was split
        # but no triangle was removed.
        continue
        
      # If got here then the target polygon (poly) has a top triangle that can be
      # removed
      triangle = remove_top_triangle(poly, second_top_point, kind)
      
    # Call the callback with the current triangle
    triangle = round_points(triangle)
    callback(triangle, calc_triangle_area(*triangle))
  
  # No more polygons to process. Call the callback with None, None
  callback(None, None)

  
  
  