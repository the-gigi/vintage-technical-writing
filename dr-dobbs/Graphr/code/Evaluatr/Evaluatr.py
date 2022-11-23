import os

def evaluateSingleValue(expr, x):
  # replace the 'X' in the expression with the current value
  e = expr.replace("x", str(x))
    
  # evaluate the substituted expression string using the eval() function    
  return eval(e)


def evaluateMultipleValues(expr, values):
  for x in values:
    yield evaluateSingleValue(expr, x)

def evaluateFile(inputFile, outputFile=None):
  # open the input and output file
  print 'inputFile:', inputFile
  i = open(inputFile)
  o = open(outputFile, "w") if outputFile else None
  
  output = []
  
  # read the first line from the input file (the expression)
  expr = i.readline().lower().strip()
  
  # read and iterate over the rest of the lines
  lines = i.readlines()
  for line in lines:
    x = int(line.strip())
    y = evaluateSingleValue(expr, x)  
    output.append((x, y))
    
    # write the value and the evaluated expression to the output file
    if o:
        o.write('%d,%d\n' % (x, y))
  return output
  
class A(object):
  def __init__(self, x):
    self.x = x
   
  def foo(self, y):
    return self.x + y








