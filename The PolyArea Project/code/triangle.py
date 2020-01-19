import math
from helpers import calc_distance  

  
def herons_formula(a, b, c):
  """Calculate the area of a triangle whose sides are a, b and c
  
  Uses Heron's formula.
  
  See: http://en.wikipedia.org/wiki/Heron's_formula
  """
  s = float(a + b +c) / 2
  return math.sqrt(s * (s - a) * (s -b) * (s -c))

def calc_triangle_area(p1, p2, p3):
  """Calculate the area of a triangle whose vertices are: p1, p2 and p3
  
  The function finds the sides of the triangle by calculating the distance
  between each two vertices and then calls the herons_formula() function
  to calculate the actual area
  """
  a = calc_distance(p1, p2)
  b = calc_distance(p2, p3)
  c = calc_distance(p3, p1)
  area = herons_formula(a, b, c)
  return area
  
def test_herons_formula():
  a = 3
  b = 4
  c = 5
  assert herons_formula(a, b, c) == 6
  
def test_calc_triangle_area():
  p1 = (0, 0)
  p2 = (0, 3)
  p3 = (4, 0)
  assert calc_triangle_area(p1, p2, p3) == 6
  
if __name__=='__main__':
  test_herons_formula()
  test_calc_triangle_area()
  print 'Done.'
  