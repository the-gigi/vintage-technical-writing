from Evaluatr import evaluateMultipleValues

values = range(20)
expression = 'x*3/2'
result = evaluateMultipleValues(expression, values)

print zip(values, result)
print