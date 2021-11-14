import json

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

from helper import *

PLOT_GEN_BLOCKS_PATH = 'graphics/gen_words/'


class GeneratedBlocksStatistics(CollisionsStatistics):
    def __init__(self, js: dict):
        CollisionsStatistics.__init__(self, js)
        self.mask = js['Mask']
        self.blocks_size = js['Blocks size']

        root_path = PLOT_GEN_BLOCKS_PATH
        blocks_dir_path = os.path.join(root_path, str(self.blocks_size))
        self.plot_path = os.path.join(blocks_dir_path, str(self.mask))

    @staticmethod
    def __plot_log_scale(ax):
        plt.xscale('log', base=2)
        plt.yscale('log', base=2)
        x_major = ticker.LogLocator(base=2.0, numticks=50)
        ax.xaxis.set_major_locator(x_major)
        y_major = ticker.LogLocator(base=2.0, numticks=50)
        ax.yaxis.set_major_locator(y_major)

    def __fill_plot_lines(self, hash_name: str) -> tuple[list, list]:
        count_words = []
        collisions = []
        hash_collisions = self.collisions[hash_name]
        for count in hash_collisions:
            count_words.append(int(count))
            collisions.append(hash_collisions[count])
        return count_words, collisions

    def __plot_hash_collisions(self, hash_name: str):
        fig, ax = plt.subplots()
        ax.set_title(hash_name)
        plt.xlabel('Count keys')
        plt.ylabel('Collisions')
        count_words, collisions = self.__fill_plot_lines(hash_name)

        plt.plot(count_words, collisions, color='r')
        plt.scatter(count_words, collisions, color='red', marker='o')

        self.__plot_log_scale(ax)

        ax.grid(which='major', color='gray', linestyle=':')
        if self.bits != self.mask:
            file_name = f'{hash_name} {self.bits} bits (mask {self.mask} bits) (log).png'
        else:
            file_name = f'{hash_name} {self.bits} bits (log).png'

        file_path = os.path.join(self.plot_path, file_name)
        fig.savefig(file_path)
        plt.cla()
        plt.clf()
        plt.close()

    def plot(self):
        for hash_name in self.collisions:
            print(hash_name, self.collisions[hash_name])
            self.__plot_hash_collisions(hash_name)

    def get_collisions(self):
        keys = list(self.collisions)
        collisions = [list(self.collisions[keys[0]])]
        collisions[0].insert(0, 'Название функции')
        for hash_name in self.collisions:
            collisions.append([hash_name])
            collisions[-1].extend(self.collisions[hash_name].values())
        return collisions


def plot_graphics(sub_dir_path: str, sub_dir_name, file_name: str, report: Document):
    path_to_file = os.path.join(sub_dir_path, file_name)
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        # временно
        js["Blocks size"] = sub_dir_name
        cs = GeneratedBlocksStatistics(js)
        cs.get_collisions()
        data_collisions = cs.get_collisions()
        heading = f'{cs.bits} bits ({cs.mask} bits) with {cs.blocks_size} bytes block size'
        dr.add_table_to_report(heading, data_collisions, report)


def open_sub_dir(root_path: str, sub_dir_name, report: Document):
    sub_dir_path = os.path.join(root_path, sub_dir_name)
    list_of_files = os.listdir(sub_dir_path)
    for file_name in list_of_files:
        plot_graphics(sub_dir_path, sub_dir_name, file_name, report)


def process_collision_statistics():
    report_heading = 'Таблицы коллизий'
    root_path = "reports/gen_words"
    process_statistics(open_sub_dir, report_heading, root_path, PLOT_GEN_BLOCKS_PATH)

