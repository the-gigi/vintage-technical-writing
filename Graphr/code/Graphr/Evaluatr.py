import os
from math import *

def evaluateSingleValue(expr, x):
  # replace the 'X' in the expression with the current value
  e = expr.replace("x", str(x))
    
  # evaluate the substituted expression string using the eval() function    
  return eval(e)


def evaluate(expr, values):
   """Evaluate an expression with an X variable over a range of X values
  
   expr - a string that represents a Python experession with a variable
   values - a list of floating point values
  
   return a generator that yields pairs of (x, eval(expr))
   """
   expr = expr.lower()
   for x in values:
     try:
       yield (x, evaluateSingleValue(expr, x))
     except:
       yield (None, None)








