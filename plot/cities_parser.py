import os
import csv

file_path = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath('__file__'))), "cities.csv")
f = open(str(file_path), "r", encoding='latin-1')

reader = csv.reader(f, delimiter=",")
columns = []
cities = []
empty_cities = 0
total_population = 0
smallest = 100000
for i, line in enumerate(reader):
    if i == 0:
        columns = line
    else:
        try:
            line[4] = int(line[4])
            smallest = min(smallest, line[4])
        except:
            line[4] = 0
            empty_cities += 1
        total_population += line[4]
        cities.append([str(line[4]), line[5], line[6]])
print("Total population", total_population)
print("Smallest city registered", smallest)
print("Empty cities", empty_cities)
g = open(os.path.join(os.path.dirname(os.path.realpath('__file__')), "cities.txt"), "w")
for city in cities:
    for i, c in enumerate(city):
        city[i] = bytes(city[i], "latin-1")
        city[i] = str(city[i], "ascii", "ignore")
    g.write(" ".join(city) + "\n")
g.flush()
