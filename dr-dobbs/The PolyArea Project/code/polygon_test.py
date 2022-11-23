from polygon import *
from pprint import pprint as pp
  
  
# o
# 
# 
# 
# o    o
triangle = ((0,0), (0,3), (4,0))

# o    o
#
#
#
# o    o
rectangle = ((0,0), (0,3), (4,3), (4,0))

#      o
#  o   
#     
#     o
# o
parallelogram = ((0,0), (3,1), (4,5), (1,4))
#    o
#
#
#    o
#
#
# o      o  
concave = ((0,0), (3,6), (6,0), (3,3))


#     o
#
#
#       o
#
#     o
# o     o  
concave2 = ((0,0), (4,6), (4,1), (5,3), (5,0))

#      o
#
#
#     o o
#
# o    o    o
concave3 = ((0,0), (3,4), (6,0), (4,2), (3,0), (2,2))

concave4 = ((0,0), (2,3), (4,1), (5,3), (5,0))

concave5 = ((0,0), (0, 4), (2,4), (2,2), (3,3), (3,0))

concave6 = ((0, 0), (6, 0), (6, 2), (5, 1), (5, 3), (0,3))

"""
              o (2,1)
             /| 
            / |    
           /  |
          /   |
         /    |
 (0,1)  o     o (2,1)
         \   /
          \ /
           o (1, 0)
""" 
concave7 = ((0, 1), (2,2), (2,1), (1,0))

"""
 o------------o
 |            |
 |            |
 o-----o      |
       |      |
       |      |
       o------o
"""      
concave8 = ((0,2), (2,2), (2,0), (1,0), (1,1), (0,1)) 

"""
          o
        / |
      /   |
    /     |
  /       |
o-----o   |
      |   |
      |   |
      o---o
      
"""
concave9 = ((2,2), (2,0), (1,0), (1,1), (0,1))


"""
      o------o
      |      |
      |      |  
o-----o      |
|            |
|            |
o------------o 
"""      
      
concave10 = ((0, 0), (0, 1), (1, 1), (1, 2), (2, 2), (2, 0))


"""
     o
   /   \
 /      \
o   o    o
 \  | \  |
  \ |  \ |
   o     o
   
"""
concave11 = ((0,1), (1, 2), (2, 1), (2, 0), (1, 1), (1, 0))


"""
     o
    /|
  /  |
 o o | 
  \|\|
   o o

"""
concave12 = (0,1), (1,0), (1,1), (2,0), (2, 2)


def test_is_triangle():
  """ """
  p = Polygon(triangle)
  assert p.is_triangle()
  
  p = Polygon(rectangle)
  assert not p.is_triangle()
  
  p = Polygon(parallelogram)
  assert not p.is_triangle()

  p = Polygon(concave)
  assert not p.is_triangle()

  p = Polygon(concave2)
  assert not p.is_triangle()

def test_split():
  p1, p2 = split(Polygon(concave11), ((1, 2), (1, 1)))
  assert p1.points == [(1.0, 2.0), (2.0, 1.0), (2.0, 0.0), (1.0, 1.0)]
  assert p2.points == [(1.0, 0.0), (0.0, 1.0), (1.0, 2.0)]
  
  p1, p2 = split(Polygon(rectangle), ((0,0), (4,3)))
  assert p1.points == [(0, 0), (0, 3), (4, 3)]
  assert p2.points == [(4, 3), (4, 0), (0, 0)]



  
def test_find_second_top_point():
  p = Polygon(triangle)
  assert p.find_second_top_point() == ((0.0, 0.0), 'vertex')
  
  p = Polygon(rectangle)
  assert p.find_second_top_point() == ((0.0, 0.0), 'vertex')
  
  p = Polygon(parallelogram)
  assert p.find_second_top_point() == ((1.0, 4.0), 'vertex')
  
  p = Polygon(concave)
  assert p.find_second_top_point() == ((3.0, 3.0), 'inside')

  p = Polygon(concave2)
  assert p.find_second_top_point() == ((5.0, 3.0), 'outside')

  p = Polygon(concave7)
  assert p.find_second_top_point() == ((0.0, 1.0), 'vertex')

triangles = []
def triangle_callback(triangle, area):
  if triangle is None or area is None:
    return
  print triangle
  triangles.append((triangle, area))

def test_calc_polygon_area():
  """ """    
  global triangles

  triangles = []
  calc_polygon_area([Polygon(concave11)], triangle_callback)
  assert len(triangles) == 3

  triangles = []
  calc_polygon_area([Polygon(concave10)], triangle_callback)
  assert len(triangles) == 4

  
  triangles = []
  calc_polygon_area([Polygon(triangle)], triangle_callback)
  assert len(triangles) == 1
  
  triangles = []
  calc_polygon_area([Polygon(rectangle)], triangle_callback)
  assert len(triangles) == 2
  
  triangles = []
  calc_polygon_area([Polygon(parallelogram)], triangle_callback)
  assert len(triangles) == 3

  triangles = []
  calc_polygon_area([Polygon(concave)], triangle_callback)
  assert len(triangles) == 2
  for t in triangles:
    assert round(float(t[1]), 3) == 4.5
  
  triangles = []
  calc_polygon_area([Polygon(concave2)], triangle_callback)
  assert len(triangles) == 6

  triangles = []
  calc_polygon_area([Polygon(concave4)], triangle_callback)
  assert len(triangles) == 4

  triangles = []
  calc_polygon_area([Polygon(concave8)], triangle_callback)
  assert len(triangles) == 5

  triangles = []
  calc_polygon_area([Polygon(concave10)], triangle_callback)
  assert len(triangles) == 4
  
  triangles = []
  calc_polygon_area([Polygon(concave12)], triangle_callback)
  assert len(triangles) == 3

def test_remove_top_triangle():
  """ """  
  try:
    remove_top_triangle(Polygon(triangle), (0, 0), 'vertex')
  except AssertionError, e:
    assert str(e) == 'Polygon cannot be a triangle'   

  try:  
    t = remove_top_triangle(Polygon(concave), (3, 3), 'inside')
  except AssertionError, e:
    assert str(e) == 'Inside second top point is not allowed'

  p = Polygon(rectangle)
  print p
  t = remove_top_triangle(p, (0, 0), 'vertex')
  print t
  print p
  assert t == [(0.0, 0.0), (4.0, 3.0), (0.0, 3.0)]
    
  t = remove_top_triangle(Polygon(parallelogram), (1, 4), 'vertex')
  assert t == [(3.75, 4.0), (1.0, 4.0), (4.0, 5.0)]

  t = remove_top_triangle(Polygon(concave2), (5, 3), 'outside')
  assert t == [(2.0, 3.0), (4.0, 3.0), (4.0, 6.0)]

  p = Polygon(concave4)
  t = remove_top_triangle(p, (4, 1), 'vertex')
  assert t == [(0.66700000000000004, 1.0), (4.0, 1.0), (2.0, 3.0)]

  p = Polygon(concave5)
  t = remove_top_triangle(p, (0, 0), 'vertex')
  assert t == [(0.0, 0.0), (2.0, 4.0), (0.0, 4.0)]

  p = Polygon(concave6)
  t = remove_top_triangle(p, (6, 2), 'outside')
  assert t == [(0.0, 2.0), (5.0, 3.0), (0.0, 3.0)]


  p = Polygon(concave8)
  t = remove_top_triangle(p, (0, 1), 'vertex')
  assert t == [(0.0, 1.0), (2.0, 2.0), (0.0, 2.0)]

  p = Polygon(concave9)
  t = remove_top_triangle(p, (0, 1), 'vertex')
  assert t == [(0.0, 1.0), (2.0, 1.0), (2.0, 2.0)]
  assert p.points == [(2.0, 1.0), (2.0, 0.0), (1.0, 0.0), (1.0, 1.0)]

def test():
  test_is_triangle()
  test_split()
  test_find_second_top_point()
  test_remove_top_triangle()
  test_calc_polygon_area()
  
if __name__=='__main__':
  #test_calc_polygon_area()
  test()
  print 'Done.'
