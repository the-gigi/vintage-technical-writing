def find_line_equation(p1, p2):
  """ """
  run = p2[0] - p1[0]
  rise = p2[1] - p1[1]
  if run == 0:
    return None
  slope = rise/run
  constant = p1[1] - slope*p1[0]
  return slope, constant

def find_line_test():
  p1 = [1,2]
  p2 = [3,4]
  conslope = find_line_equation(p1, p2)
  print conslope
  # test vertical line
  p1 = [1,1]
  p2 = [1,2]
  conslope = find_line_equation(p1, p2)
  print conslope
  
find_line_test()