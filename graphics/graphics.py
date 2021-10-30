import sys
import os
import json

import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.scale
import matplotlib.ticker as ticker

import numpy as np


def print_start_end_method(func: callable):
    def wrapped(instance, dir_name: str, file_name: str, bits: int):
        print(f'start {file_name}')
        func(instance, dir_name, file_name, bits)
        print(f'end {file_name}\n')
    return wrapped


def print_start_end_func(func: callable):
    def wrapped(dir_name: str, file_name: str, bits: int):
        print(f'start {file_name}')
        func(dir_name, file_name, bits)
        print(f'end {file_name}\n')
    return wrapped


class HistCheckDist:
    def __init__(self, js: dict):
        self.bits = js["Bits"]
        self.mode = js["Mode"]
        self.bin_size = js["Bin size"]
        self.bar_count = js["Bar count"]
        self.x_bars = [i + 1 for i in range(self.bar_count)]
        self.y_mean = js["Y mean"]
        self.x_ranges = ["{:,}".format(number) for number in js["X ranges"]]
        self.y_err = [js["Y err min"], js["Y err max"]]
        self.y_min = js["Y min"]
        self.y_max = js["Y max"]

    def _log(self):
        print(f'bin_size = {self.bin_size}')
        print(f'x_bars = {self.x_bars}')
        print(f'x_ranges = {self.x_ranges}')
        print(f'y_mean = {self.y_mean}')
        print(f'y_err_min = {self.y_err[0]}')
        print(f'y_err_max = {self.y_err[1]}')
        print(f'y_min = {self.y_min}')
        print(f'y_max = {self.y_max}')

    def _bar(self, ax, fig):
        plt.bar(self.x_bars, self.y_mean, yerr=self.y_err, ecolor='red', capsize=6, bottom=0, color='b', alpha=0.3,
                edgecolor='blue')

        if self.bits != 64:
            plt.xticks(self.x_bars, self.x_ranges)
        else:
            plt.xticks(self.x_bars, self.x_ranges, fontsize=9)

        if self.bin_size == 1:
            plt.xlabel(f'Hashes')
        else:
            tmp = "{:,}".format(self.bin_size)
            plt.xlabel('Hashes (bin = ' + tmp + ')')
        plt.ylabel('Count')

        if self.bits != 64:
            #  Устанавливаем интервал основных делений:
            ax.xaxis.set_major_locator(ticker.MultipleLocator(4))
        else:
            #  Устанавливаем интервал основных делений:
            ax.xaxis.set_major_locator(ticker.MultipleLocator(8))

        #  Устанавливаем интервал вспомогательных делений:
        ax.xaxis.set_minor_locator(ticker.MultipleLocator(1))

        fig.set_facecolor('floralwhite')
        ax.set_facecolor('seashell')

    def create_histogram(self, file_name: str):
        # file_name = file_name.split('.')[0]

        fig, ax = plt.subplots()
        func_name = file_name.split('.')[0]
        ax.set_title(func_name)

        self._log()
        self._bar(ax, fig)

        fig.savefig(f'graphics/check_dist/{self.bits} ({self.mode})/{func_name}.png')
        # plt.show()
        plt.cla()
        plt.clf()
        plt.close()


class PlotRandomWordsCollisions:
    def __init__(self, js: dict):
        self.bits = js['Bits']
        self.hashes_collisions = js["Collisions"]

    def _plot_linear_scale(self, ax):
        if self.bits == 16:
            ax.xaxis.set_major_locator(ticker.MultipleLocator(8192))
            ax.yaxis.set_major_locator(ticker.MultipleLocator(4000))
        if self.bits == 24:
            ax.xaxis.set_major_locator(ticker.MultipleLocator(2097152))
            ax.yaxis.set_major_locator(ticker.MultipleLocator(1000000))

    @staticmethod
    def _plot_log_scale(ax):
        plt.xscale('log', base=2)
        plt.yscale('log', base=2)
        x_major = matplotlib.ticker.LogLocator(base=2.0, numticks=16)
        ax.xaxis.set_major_locator(x_major)
        y_major = matplotlib.ticker.LogLocator(base=2.0, numticks=8)
        ax.yaxis.set_major_locator(y_major)

    def _fill_plot_lines(self, hash_name: str) -> tuple[list, list]:
        count_words = []
        collisions = []
        hash_collisions = self.hashes_collisions[hash_name]
        for count in hash_collisions:
            count_words.append(int(count))
            collisions.append(hash_collisions[count])
        return count_words, collisions

    def _plot_hash_collisions(self, hash_name: str, log_scale: bool):
        fig, ax = plt.subplots()
        ax.set_title(hash_name)
        plt.xlabel('Count words')
        plt.ylabel('Collisions')
        count_words, collisions = self._fill_plot_lines(hash_name)

        plt.plot(count_words, collisions, color='r')
        plt.scatter(count_words, collisions, color='red', marker='o')

        if log_scale:
            self._plot_log_scale(ax)
        else:
            self._plot_linear_scale(ax)

        ax.grid(which='major', color='gray', linestyle=':')

        if log_scale:
            fig.savefig(f'histograms/gen words/{hash_name} {self.bits} bits (log).png')
        else:
            fig.savefig(f'histograms/gen words/{hash_name} {self.bits} bits.png')

        plt.cla()
        plt.clf()
        plt.close()

    def plot(self, log_scale):
        for hash_name in self.hashes_collisions:
            print(hash_name, self.hashes_collisions[hash_name])
            self._plot_hash_collisions(hash_name, log_scale)


@print_start_end_func
def create_histogram(dir_name: str, file_name: str, bits: int):
    path_to_file = dir_name + f'/{bits}/' + file_name
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        hcd = HistCheckDist(js)
        hcd.create_histogram(file_name)


def hist_dist_open_dir(dir_name, bits):
    list_of_files = os.listdir(dir_name + f'/{bits}')
    for file in list_of_files:
        create_histogram(dir_name, file, bits)


def hist_dist():
    dir_name = "reports/check_dist"
    hist_dist_open_dir(dir_name, 16)
    hist_dist_open_dir(dir_name, 24)
    hist_dist_open_dir(dir_name, 32)
    hist_dist_open_dir(dir_name, 64)


def plot_gen_words():
    dir_name = "reports/gen words"
    list_of_files = os.listdir(dir_name)
    for file_name in list_of_files:
        with open(dir_name + '/' + file_name, 'r') as file:
            js = json.load(file)
            plt_coll = PlotRandomWordsCollisions(js)
            plt_coll.plot(True)
            plt_coll.plot(False)


def main():
    test_number = 1
    if test_number == 1:
        hist_dist()
    elif test_number == 2:
        plot_gen_words()


if __name__ == '__main__':
    mpl.pyplot.rcParams['figure.max_open_warning'] = 100
    main()
