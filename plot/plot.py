from os.path import dirname, realpath, join
from os import listdir, makedirs
import sys
import csv
import re
import time


use_png = (len(sys.argv) >= 2 and sys.argv[1] == "png")
def get_extension():
    if use_png:
        return ".png"
    else:
        return ".svg"

import matplotlib
if not use_png:
    matplotlib.use('SVG')
import matplotlib.pyplot as plt

from matplotlib import rcParams
rcParams['axes.titlepad'] = 17
rcParams['axes.titlesize'] = 10

class Distribution:
    def __init__(self, path):
        self.path_to_save = path
        self.samples = []
        self.num_nodes = ""
        self.gossip_factor = ""
        self.corrupt = ""
        self.delay = ""
        self.latency = ""
        self.percent = ""

    def add(self, sample):
        self.samples.append(sample)

    def set_title_parts(self, num_nodes, gossip_factor, corrupt, delay, latency, percent):
        self.num_nodes = num_nodes
        self.gossip_factor = gossip_factor
        self.corrupt = corrupt
        self.delay = delay
        self.latency = latency
        self.percent = percent

    def get_title(self):
        return self.num_nodes + " nodes, Gossip=" + self.gossip_factor + ", FailRate=" + self.corrupt + ", Processing=" + self.delay + "ms, Reach=" + self.percent

    def get_file_name(self):
        return self.get_title()

    def save_plot(self):
        num_samples = str(len(self.samples))

        if len(self.samples) == 0:
            return num_samples

        plt.hist(self.samples, list(range(int(min(self.samples)) - 5, int(max(self.samples)) + 10)))

        plt.xlabel('Avg Time (ms)')
        plt.ylabel('Number of simulations (Out of a total of ' + num_samples + ')')
        plt.title(self.get_title())

        # plt.axis([min(samples) - 5, max(samples) + 5, 0, 100])
        plt.grid(True)

        plt.savefig(join(self.path_to_save, self.percent + get_extension()))
        plt.clf()

        return num_samples


def get_data(path, n, k, c, t, l):
    reach_enumeration=["50%", "66%", "75%", "90%", "99%", "99.9%", "100.0%"]

    file_path = join(path, "data.csv")
    try:
        f = open(file_path, "r")
    except:
        return []
    reader = csv.reader(f, delimiter=",")
    distributions = [Distribution(path) for _ in range(len(reach_enumeration))]
    for i, row in enumerate(reader):
        for j in range(len(reach_enumeration)):
            try:
                distributions[j].add(float(row[j]))
            except:
                pass
    for i, percent in enumerate(reach_enumeration):
        distributions[i].set_title_parts(n, k, c, t, l, percent)
    return distributions


def create_images(path, n, k, c, t, l):
    distributions = get_data(path, n, k, c, t, l)
    for distribution in distributions:
        distribution.save_plot()
    return bool(len(distributions))


def dump():
    logs_folder = join(dirname(dirname(realpath('__file__'))), "logs")
    makedirs(logs_folder, exist_ok=True)
    simulations = listdir(logs_folder)
    pattern = re.compile(r"n=(\d+),k=(\d+),c=(\d+%),t=(\d+),l=(\d+)")
    for simulation in simulations:
        match = pattern.match(simulation)
        if match:
            path = join(logs_folder, simulation)
            args = match.group(1, 2, 3, 4, 5)
            created = create_images(path, *args)
            if created:
                print("Created images for parameters", simulation)
            else:
                print("No data file for parameters", simulation, ", empty folder instead")


while True:
    print("\rMaking a dump now...")
    dump()
    print("\rSuccessfully made a full gallery")
    time.sleep(15)
