import json

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from helper import *

import typing


class AvalancheEffectStatistics:
    L_DISTANCE: typing.Final = 'Distance'
    L_FREQUENCY: typing.Final = 'Frequency'
    L_NUMBER: typing.Final = 'Number'
    L_HASH: typing.Final = 'Hash'
    L_ERR: typing.Final = 'Err'

    def __init__(self, js: dict, tests_dir_path: str | bytes):
        self.test_name = js['Test name']
        self.bits = js["Bits"]
        self.hist_path = os.path.join(tests_dir_path, self.test_name)
        make_dir(tests_dir_path)
        make_dir(self.hist_path)



        avalanche_statistics = js["Avalanche effect"]
        self.worst_cases = {}
        self.best_cases = {}
        self.average_cases = {}
        self.original_pair = {}
        self.modified_pair = {}

        """
        self.bin_size = js["Bin size"]
        self.bar_count = js["Bar count"]
        self.x_bars = [i + 1 for i in range(self.bar_count)]
        self.y_mean = js["Y mean"]
        self.x_ranges = ["{:,}".format(number) for number in js["X ranges"]]
        self.y_err = [js["Y err min"], js["Y err max"]]
        self.y_min = js["Y min"]
        self.y_max = js["Y max"]

        hist_dir_name = f'{self.bits}'
        self.hist_path = os.path.join(tests_dir_path, hist_dir_name)
        make_dir(self.hist_path)
        """

    def __bar(self, ax, fig):
        plt.bar(self.x_bars, self.y_mean, yerr=self.y_err, ecolor='red', capsize=6, bottom=0, color='b', alpha=0.3,
                edgecolor='blue')
        if self.bits < 48:
            plt.xticks(self.x_bars, self.x_ranges)
        else:
            plt.xticks(self.x_bars, self.x_ranges, fontsize=9)

        if self.bin_size == 1:
            plt.xlabel(f'Hashes')
        else:
            tmp = "{:,}".format(self.bin_size)
            plt.xlabel('Hashes (bin = ' + tmp + ')')
        plt.ylabel('Count')

        if self.bits < 48:
            ax.xaxis.set_major_locator(ticker.MultipleLocator(8))
        else:
            ax.xaxis.set_major_locator(ticker.MultipleLocator(8))
        ax.xaxis.set_minor_locator(ticker.MultipleLocator(1))

        fig.set_facecolor('floralwhite')
        ax.set_facecolor('seashell')

    def fill_statistics(self, avalanche_statistics: dict):
        for hash_name, avalanche_statistics in avalanche_statistics:
            self.worst_cases[hash_name] = avalanche_statistics['Worst case']
            self.best_cases[hash_name] = avalanche_statistics['Best case']
            self.average_cases[hash_name] = self.bits / 2    # заглушка
            self.worst_cases[hash_name][L_ERR]
            self.original_pair[hash_name] = avalanche_statistics['Original pair']
            self.modified_pair[hash_name] = avalanche_statistics['Modified pair']

    def create_histogram(self, file_name: str):
        fig, ax = plt.subplots()
        func_name = file_name.split()[0]
        ax.set_title(self.test_name)

        self.__bar(ax, fig)

        file_path = os.path.join(self.hist_path, func_name + '.png')
        fig.savefig(file_path)
        plt.cla()
        plt.clf()
        plt.close()

    def get_err(self):
        min_counter = min(self.y_min)
        max_counter = max(self.y_max)
        return [self.hash_name, min_counter, max_counter]


def create_dict_tables():
    table = [['Название функции',
              'Минимальное расстояние Хемминга',
              'Частота минимального расстояния',
              'Максимальное расстояние Хемминга',
              'Частота максимального расстояния',
              'Среднее значение']]
    return table


def create_histogram(dir_path: str, file_name: str, dict_tables, save_path: str):
    path_to_file = os.path.join(dir_path, file_name)
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        ds = AvalancheEffectStatistics(js, save_path)
        ds.create_histogram(file_name)
        dict_tables.append(ds.get_err())


def process_avalanche_effect_statistics(tests_dir_name):
    report_heading = 'Таблицы лавинного эффекта'
    path_to_test_dir = get_cpp_report_path(tests_dir_name)
    test_name = "Avalanche effect tests"
    path_to_cur_test_dir = os.path.join(path_to_test_dir, test_name)
    save_path = get_python_report_path(tests_dir_name)
    make_dir(save_path)
    save_path = os.path.join(save_path, test_name)
    make_dir(save_path)
    list_of_files = os.listdir(path_to_cur_test_dir)
    report = dr.create_document(report_heading)
    for file_name in list_of_files:
        dict_tables = create_dict_tables()
        create_histogram(tests_dir_name, path_to_cur_test_dir, dict_tables, file_name)
        heading = f'{file_name})'
        dr.add_table_to_report(heading, dict_tables, report)
    dr.save_document(report, save_path, 'report.docx')



