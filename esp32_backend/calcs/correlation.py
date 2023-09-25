from math import sqrt

x = [11.98, 9.28, 8.25, 7.62, 6.44, 5.66, 5.8, 13.34, 11.49, 10.12, 8.7, 6.87, 6.22, 6.11]

y = [11.36, 10.42, 9.47, 7.58, 5.59, 4.75, 5.22, 11.84, 10.89, 9.95, 8.53, 6.64, 5.69, 5.69]

def relation_coeeficient(x, y):
    if not (type(x) is list and type(y) is list):
        print("Parameters x and y are not tuples")
        return
    if len(x) != len(y):
        print("x and y don't have the same length")

    sum_xy = 0
    n = len(x)
    sum_x = sum(x) 
    sum_y = sum(y) 
    x_square = list(map(lambda x:pow(x,2), x))
    y_square = list(map(lambda y:pow(y,2), y))

    for i in range(len(x)):
        sum_xy += x[i]*y[i]

    r = (n*sum_xy-sum_x*sum_y)/sqrt((n*sum(x_square)-pow(sum(x),2))*(n*sum(y_square)-pow(sum(y),2)))

    return r


print(relation_coeeficient(x, y))
