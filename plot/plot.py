import os
import csv
import time

import matplotlib.pyplot as plt


class Distribution:
    def __init__(self):
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
        return "n=" + self.num_nodes + ", k=" + self.gossip_factor + ", corrupt=" + self.corrupt + ", delay=" + self.delay + "ms, ping=" + self.latency + "ms" + ",r=" + self.percent

    def get_file_name(self):
        return self.get_title()

    def save_plot(self):
        num_samples = str(len(self.samples))

        plt.hist(self.samples, list(range(int(min(self.samples)) - 5, int(max(self.samples)) + 10)))

        plt.xlabel('Avg Time')
        plt.ylabel('Number of simulations (total=' + num_samples + ')')
        plt.title(self.get_title())

        # plt.axis([min(samples) - 5, max(samples) + 5, 0, 100])
        plt.grid(True)

        plt.savefig("plots/" + self.get_file_name() + ".png")
        plt.clf()

        return num_samples


def get_data():
    file_path = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath('__file__'))), "transmission.csv")
    f = open(str(file_path), "r")
    reader = csv.reader(f, delimiter=",")
    distributions = [Distribution() for _ in range(6)]
    num_nodes, gossip_factor, corrupt, delay, latency = [""] * 5
    for i, row in enumerate(reader):
        if i == 0:
            num_nodes, gossip_factor, corrupt, delay, latency = row
        else:
            for j in range(6):
                distributions[j].add(float(row[j]))
    for i, percent in enumerate(["50%", "66%", "75%", "90%", "99%", "100%"]):
        distributions[i].set_title_parts(num_nodes, gossip_factor, corrupt, delay, latency, percent)
    return distributions


def dump():
    distributions = get_data()
    num_simulations = -1
    for distribution in distributions:
        num_simulations = max(num_simulations, distribution.save_plot())

    return num


while True:
    num = dump()
    print("Made a dump for", num, "simulations")
    time.sleep(15)
