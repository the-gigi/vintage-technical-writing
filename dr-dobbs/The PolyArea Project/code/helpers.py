"""Various helper functions for basic geometric operations

distance - 

"""

import os
import sys
import math

def calc_distance(p1, p2):
  """Calculate the distance between two points
  
  Uses the Pythagorean Theorem.
  
  See: http://en.wikipedia.org/wiki/Pythagorean_theorem
  """
  d = math.sqrt(((p1[0] - p2[0]) ** 2) + ((p1[1] - p2[1]) ** 2))
  return d

def find_line_coefficients(p1, p2):
  """Find the a and b coefficients of the line that goes through p1 and p2
  
  If the line is vertical return None otherwise return (a,b)
  """
  if p1[0] == p2[0]:
    return None
    
  a = float(p2[1] - p1[1]) / (p2[0] - p1[0])
  b = p1[1] - a * p1[0]
  
  return (a, b)


def round_points(points):
  """Round the co-ordinates of a sequence of points to 3 decimal digits"""
  return [tuple(round(float(x), 3) for x in p) for p in points]

def filter_consecutive_points(points):
  """Remove the middle point if there are 3 consecutive points
  on the same horizontal or vertical line
  """
  point_count = len(points)
  result = points[:]
  to_remove = []
  for i in range(point_count):
    p = points[i]
    p1  = points[(i + 1) % point_count]
    p2  = points[(i + 2) % point_count]
    if p[0] == p1[0] == p2[0]:
      to_remove.append(p1)
    if p[1] == p1[1] == p2[1]:
      to_remove.append(p1)
  
  for p in to_remove:
    result.remove(p)
    
  return result

def overlap(seg_1, seg_2):
  """Find if the segments overlap
  
  Segments overlap if they are on the same line and a common point
  """
  line_1 = find_line_coefficients(*seg_1)
  line_2 = find_line_coefficients(*seg_2)
  if line_1 != line_2:
    return set()

  # Vertical lines
  if line_1 == None:
    # Parallel
    if seg_1[0][0] != seg_2[0][0]:
      return set()
  
  min_x_1 = min(seg_1[0][0], seg_1[1][0])
  max_x_1 = max(seg_1[0][0], seg_1[1][0])
  min_y_1 = min(seg_1[0][1], seg_1[1][1])
  max_y_1 = max(seg_1[0][1], seg_1[1][1])
  
  min_x_2 = min(seg_2[0][0], seg_2[1][0])
  max_x_2 = max(seg_2[0][0], seg_2[1][0])
  min_y_2 = min(seg_2[0][1], seg_2[1][1])
  max_y_2 = max(seg_2[0][1], seg_2[1][1])

  # Find all the overlap points of segment end points
  overlaps = set()

  if line_1 == None:
    # Handle vertical lines
    if min_y_1 <= seg_2[0][1] <= max_y_1:
      overlaps.add(tuple(seg_2[0]))
  
    if min_y_1 <= seg_2[1][1] <= max_y_1:
      overlaps.add(tuple(seg_2[1]))
    
    if min_y_2 <= seg_1[0][1] <= max_y_2:
      overlaps.add(tuple(seg_1[0]))
  
    if min_y_2 <= seg_1[1][1] <= max_y_2:
      overlaps.add(tuple(seg_1[1]))
  else:
    # Handle all other lines (horizontal and diagonal)
    if min_x_1 <= seg_2[0][0] <= max_x_1:
      overlaps.add(tuple(seg_2[0]))
  
    if min_x_1 <= seg_2[1][0] <= max_x_1:
      overlaps.add(tuple(seg_2[1]))
    
    if min_x_2 <= seg_1[0][0] <= max_x_2:
      overlaps.add(tuple(seg_1[0]))
  
    if min_x_2 <= seg_1[1][0] <= max_x_2:
      overlaps.add(tuple(seg_1[1]))

  return overlaps


def intersect_vert(seg, vert_seg):
  """Find the intersection of a line segment with a vertical line segment
  
  The line segment MUST not be vertical, but it may be a point.
  """
  assert vert_seg[0][0] == vert_seg[1][0]

  # If it's not a point it must NOT be vertical unless the vert_seg is a point
  if seg[0] != seg[1]:
      assert seg[0][0] != seg[1][0]
  
  vert_x = vert_seg[0][0]
  left_x = min(seg[0][0], seg[1][0])
  right_x = max(seg[0][0], seg[1][0])
  if vert_x < left_x or vert_x > right_x:
    return None
    
  a, b = find_line_coefficients(*seg)
  y = a * vert_x + b
  
  top_y = min(seg[0][1], seg[1][1])
  bottom_y = max(seg[0][1], seg[1][1])

  top_vert_y = min(vert_seg[0][1], vert_seg[1][1])
  bottom_vert_y = max(vert_seg[0][1], vert_seg[1][1])

  if (top_y <= y <= bottom_y) and (top_vert_y <= y <= bottom_vert_y):
    return (vert_x, y)
  
  return None

def is_point(seg):
  """Check if the segment has identical end points"""
  return seg[0] == seg[1]

def point_on_segment(p, seg):
  """Find if point p resides on segment seg"""
  
  # If the segment is a point then p must be equal to it.
  if is_point(seg):
    return p == srg[0]
    
  #
  coefficients = find_line_coefficients(*seg)
  # Vertical segment
  if coefficients is None:
    if p[0] != seg[0][0]:
      return False
    if p[1] > max(seg[0][1], seg[1][1]):
      return False
    if p[1] < min(seg[0][1], seg[1][1]):
      return False
    return True
  else:
    return intersect((p, p), seg)
    

def intersect(seg_1, seg_2):
  """Find the intersection of two line segments
  
  If the line segment intersect return the intersection point
  If they don't return None.
  
  Each line segment is a pair of points, where each point os a pair of integers.
  Line segments are always part of an infinite line.
  
  The function determines if the segments are vertical or not. For non-vertical
  segments it calculates the a and b coefficients in their line equation:
  (y = ax + b)
  
  If the line segments are parallel then they don't intersect. If they are not
  parallel it calculates the intersection point of their lines
  
  """
  assert seg_1 != seg_2
  for p in seg_1 + seg_2:
    assert p[0] is not None and p[1] is not None
  #seg_1 = round_points(seg_1)
  #seg_2 = round_points(seg_2)
  
  line_1 = find_line_coefficients(*seg_1)
  line_2 = find_line_coefficients(*seg_2)
  
  # Both lines are vertical (no intersection or one of them is a point)
  if line_1 is None and line_2 is None:
    # seg_1 is a point
    if seg_1[0] == seg_1[1]:
      # Seg 2 has a different x coordinate, no intersection
      if seg_2[0][0] != seg_1[0][0]:
        return None
      else: # The point (seg_1) has the same x co-ordinate as seg_2
        # Find out if the seg_1 point is vertically between
        # the min and max of the seg_2 end points
        min_y = min(seg_2[0][1], seg_2[1][1])
        max_y = max(seg_2[0][1], seg_2[1][1])
        return min_y <= seg_1[0][1] <= max_y
    # seg_2 is a point
    elif seg_2[0] == seg_2[1]:
      # Seg 1 has a different x coordinate, no intersection
      if seg_1[0][0] != seg_2[0][0]:
        return None
      else: # The point (seg_2) has the same x co-ordinate as seg_1
        # Find out if the seg_2 point is vertically between
        # the min and max of the seg_1 end points
        min_y = min(seg_1[0][1], seg_1[1][1])
        max_y = max(seg_1[0][1], seg_1[1][1])
        return min_y <= seg_2[0][1] <= max_y
      
    # Both seg 1 and seg 2 are not points. No intersection
    # (ignoring the case of vertical segments touching at one end point)
    else:
      return None
  
  # Line 1 is vertical and line 2 is not
  if line_1 is None:
    x = seg_1[0][0]
    if x < min(seg_2[0][0], seg_2[1][0]):
      return None
      
    if x > max(seg_2[0][0], seg_2[1][0]):
      return None
    
    # If its not a point find the vertical intersection
    return intersect_vert(seg_2, seg_1)

  # Line 2 is vertical and line 1 is not
  if line_2 is None:
    x = seg_2[0][0]
    if x < min(seg_1[0][0], seg_1[1][0]):
      return None
      
    if x > max(seg_1[0][0], seg_1[1][0]):
      return None
    
    return intersect_vert(seg_1, seg_2)

  # Both lines are not vertical
  a1, b1 = line_1
  a2, b2 = line_2
  
  # Check if parallel lines
  if a1 == a2:
    return None
  
  x = (b2 - b1) / (a1 - a2)
  y = a1 * x + b1
  x = round(x, 3)
  y = round(y, 3)
  
  if x < min(seg_1[0][0], seg_1[1][0]):
    return None

  if x < min(seg_2[0][0], seg_2[1][0]):
    return None

  if x > max(seg_1[0][0], seg_1[1][0]):
    return None

  if x > max(seg_2[0][0], seg_2[1][0]):
    return None

  if y < min(seg_1[0][1], seg_1[1][1]):
    return None

  if y < min(seg_2[0][1], seg_2[1][1]):
    return None

  if y > max(seg_1[0][1], seg_1[1][1]):
    return None

  if y > max(seg_2[0][1], seg_2[1][1]):
    return None

  return x, y
    
def intersect_sweepline(seg, y):
  """Find the intersection point of seg with the sweepline like y"""
  # Check if segment is vertical first
  if seg[0][0] == seg[1][0]:
    top = min(seg[0][1], seg[1][1])
    bottom = max(seg[0][1], seg[1][1])
    if top <= y <= bottom:
      return (seg[0][0], y)
  
  sweepline = ((seg[0][0], y), (seg[1][0], y))
  return intersect(seg, sweepline)
  
def classify_point(p1, p2, target):
  """Determine if the target point is between p1 and p2
  
  p1, p2 and target are points that have the same Y co-ordinate
  
  If target is either p1 or p2 return     'vertex'
  If the target is between p1 and p2 return 'inside'
  Otherwise return 'outside'
  """
  assert p1 != p2
  assert p1[1] == p2[1] == target[1]
  if target in (p1, p2):
    return 'vertex'

  left_x = min(p1[0], p2[0])
  right_x = max(p1[0], p2[0])
  
  if left_x < target[0] < right_x:
    return 'inside'
  else:
    return 'outside'
  
def point_on_degment(p, segment):
  """Determine if the point lies on the segment"""
  return intersect((p, p), segment)
  
def test_calc_distance():
  p1 = (0, 0)
  p2 = (6, 8)
  assert calc_distance(p1, p2) == 10
  
def test_find_line_coefficients():
  line = ((1,0), (1,3))
  assert find_line_coefficients(*line) == None
    
  line = ((0,0), (5,5))
  assert find_line_coefficients(*line) == (1, 0)

  line = ((4,5), (5,5))
  assert find_line_coefficients(*line) == (0, 5)
  
def test_intersect_vert():
  v1 = ((1,8), (1,5))
  v2 = ((2,2), (2, 20))
  
  s1 = ((0, 7), (100, 7))
  s2 = ((-100, 7), (-50, 7))
  
  s3 = ((6,10), (0,4))
  
  try:
    intersect_vert(v1, v2)
    assert False
  except AssertionError:
    pass
  
  assert intersect_vert(s1, v1) == (1,7)
  assert intersect_vert(s2, v1) == None
  assert intersect_vert(s3, v1) == (1,5) 


def test_point_on_segment():
  p = (0, 0)
  s = ([-5, 0], [5, 0])
  assert point_on_segment(p, s)

  p = (0, 0)
  s = ([0, -5], [0, 5])
  assert point_on_segment(p, s)

  p = (0, 0)
  s = ([-5, -5], [5, 5])
  assert point_on_segment(p, s)

  p = (0, 1)
  s = ([-5, 0], [5, 0])
  assert not point_on_segment(p, s)

  p = (1, 0)
  s = ([0, -5], [0, 5])
  assert not point_on_segment(p, s)

  p = (6, 6)
  s = ([-5, -5], [5, 5])
  assert not point_on_segment(p, s)
  

def test_intersect():

  s1 = ((0.667, 1.0), (4.0, 3.0))
  s2 = ((-2147483647, 1.0), (2147483647, 1.0))
  assert intersect(s1, s2) == (0.667, 1.0)

  s1 = ((0,0), (10, 5))
  s2 = ((9,6), (3,0))
  assert intersect(s1, s2) == (6,3)
  
  s1 = ([130, 70], (434, 36))
  s2 = ([580, 220], [310, 310])
  assert intersect(s1, s2) == None
  
    
def test_intersect_sweepline():
  s1 = ((0,0), (10, 5))
  y = 3
  assert intersect_sweepline(s1, y) == (6,3)
  
  s1 = ((5,1), (5,5))
  y = 4
  assert intersect_sweepline(s1, y) == (5,4)
  
def test_classify_point():
  x = [(0, 5), (1, 5), (2, 5)]
  assert classify_point(x[0], x[1], x[2]) == 'outside'
  assert classify_point(x[1], x[0], x[2]) == 'outside'
  assert classify_point(x[2], x[1], x[0]) == 'outside'
  assert classify_point(x[1], x[2], x[0]) == 'outside'
  assert classify_point(x[2], x[1], x[1]) == 'vertex'
  assert classify_point(x[1], x[2], x[1]) == 'vertex'
  assert classify_point(x[2], x[1], x[2]) == 'vertex'
  assert classify_point(x[1], x[2], x[2]) == 'vertex'
  assert classify_point(x[0], x[2], x[1]) == 'inside'
  assert classify_point(x[2], x[0], x[1]) == 'inside'
  
  
def test_overlap():
  s1 = ((0,0), (10, 0))
  s2 = ((11, 0), (13, 0))
  assert overlap(s1, s2) == set()

  s1 = ((0,0), (10, 0))
  s2 = ((0,3), (13, 3))
  assert overlap(s1, s2) == set()

  s1 = ((5,5), (5, 10))
  s2 = ((5, 20), (5, 30))
  assert overlap(s1, s2) == set()

  s1 = ((5,5), (5, 10))
  s2 = ((3, 5), (3, 30))
  assert overlap(s1, s2) == set()

  s1 = ((0,0), (5, 5))
  s2 = ((0, 5), (5, 0))
  assert overlap(s1, s2) == set()

  s1 = ((0,0), (5, 5))
  s2 = ((6, 6), (10, 10))
  assert overlap(s1, s2) == set()
  
  s1 = ((0,0), (10, 0))
  s2 = ((10,0), (13, 1))
  assert overlap(s1, s2) == set()  

  s1 = ((0,0), (10, 0))
  s2 = ((10,0), (13, 0))
  assert overlap(s1, s2) == set([(10, 0)])  

  s1 = ((0,0), (12, 0))
  s2 = ((10,0), (13, 0))
  assert overlap(s1, s2) == set([(10, 0), (12, 0)])  

  s1 = ((0,0), (0, 10))
  s2 = ((0, 10), (0, 13))
  assert overlap(s1, s2) == set([(0, 10)])  

  s1 = ((0,0), (0, 12))
  s2 = ((0, 10), (0, 13))
  assert overlap(s1, s2) == set([(0, 10), (0, 12)])  

  s1 = ((0,0), (10, 10))
  s2 = ((10, 10), (13, 13))
  assert overlap(s1, s2) == set([(10, 10)])  

  s1 = ((0,0), (12, 12))
  s2 = ((10, 10), (13, 13))
  assert overlap(s1, s2) == set([(10, 10), (12, 12)])  


def test():
  test_calc_distance()
  test_find_line_coefficients()
  test_point_on_segment()
  test_intersect_vert()
  test_intersect()
  test_intersect_sweepline()
  test_classify_point()
  test_overlap()
  print 'Done.'
  
if __name__ == '__main__':
  test()