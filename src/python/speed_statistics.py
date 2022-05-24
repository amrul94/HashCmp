import json
import os.path

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

import my_histogram
from reports_dir import *


# частично этот класс совпадает с классом коллизий
class SpeedStatistics:
    def __init__(self, tests_data: dict, tests_dir_name: str):
        self.graphics_path = 'graphics'
        self.bits = tests_data['Bits']
        self.speeds = tests_data["Speed"]
        self.tests_dir_path = get_python_report_path(tests_dir_name)
        self.test_name = tests_data["Test name"]
        self.hist_path = os.path.join(self.tests_dir_path, self.test_name)
        make_dir(self.tests_dir_path)
        make_dir(self.hist_path)

    @staticmethod
    def __auto_label(ax, rects):
        for rect in rects:
            height = np.around(rect.get_height(), 1)
            ax.text(rect.get_x() + rect.get_width() / 2., height + 0.3, height, ha='center', va='bottom')

    def __bar(self, ax, fig):
        ax.set_title(f'Скорость хеширования ({self.bits}-битные хеш функции)', fontsize=18)
        plt.xlabel('Хеш функции', fontsize=16)
        plt.ylabel('Время хеширования (в секундах)', fontsize=16)

        fig.set_figwidth(12)
        fig.set_figheight(8)

        x = self.speeds.keys()
        height = self.speeds.values()
        rects = plt.bar(x, height, color='grey', edgecolor='black', linewidth=1, alpha=0.5)
        plt.xticks(rotation='vertical')
        self.__auto_label(ax, rects)

        max_height = max([int(np.around(rect.get_height() + 1, -1)) for rect in rects])
        ax.yaxis.set_major_locator(ticker.MultipleLocator(max_height / 5))
        ax.yaxis.set_minor_locator(ticker.MultipleLocator(max_height / 25))
        plt.grid(ls=':')
        plt.ylim(top=max_height * 1.2)

    def create_histogram(self):
        title = f'Скорость хеширования ({self.bits}-битные хеш функции)'
        legend_labels = []
        labels = my_histogram.Labels(title, 'Хеш функции', 'Число Время хеширования (в секундах)', legend_labels)
        file_path = os.path.join(self.hist_path, f'{self.bits} bits.png')
        my_histogram.histogram(self.speeds, labels, file_path, self.__auto_label)


def create_histogram(tests_dir_name: str, dir_path: str, file_name: str | bytes):
    path_to_file = os.path.join(dir_path, file_name)
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        ews = SpeedStatistics(js, tests_dir_name)
        ews.create_histogram()


def process_speed_statistics(tests_dir_name):
    path_to_test_dir = get_cpp_report_path(tests_dir_name)
    path_to_eg_dir = os.path.join(path_to_test_dir, "Speed tests")
    list_of_files = os.listdir(path_to_eg_dir)
    for file_name in list_of_files:
        create_histogram(tests_dir_name, path_to_eg_dir, file_name)
