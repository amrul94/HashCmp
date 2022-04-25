import json

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from helper import *

import typing


class AvalancheEffectStatistics:

    def __init__(self, js: dict, tests_dir_path: str | bytes):
        self.test_name = js['Test name']
        self.bits = js["Bits"]
        self.hist_path = tests_dir_path     # os.path.join(tests_dir_path, self.test_name)
        print('hist_path: ', self.hist_path)
        make_dir(tests_dir_path)
        make_dir(self.hist_path)

        self.x_bars = []
        self.hash_names = []
        self.y_min_dist = []     # worst_cases
        self.y_min_freq = []     # worst_cases
        self.y_max_dist = []     # best_cases
        self.y_max_freq = []     # best_cases
        self.y_avg_dist = []     # average_cases
        self.y_err_dist = [[], []]
        self.original_number = []
        self.original_hash = []
        self.modified_number = []
        self.modified_hash = []
        self.__fill_statistics(js["Avalanche effect"])

    def __fill_statistics(self, avalanche_statistics: dict):
        for hash_name, hash_statistics in avalanche_statistics.items():
            self.hash_names.append(hash_name)
            y_min = hash_statistics['Worst case']
            y_max = hash_statistics['Best case']
            y_avg = hash_statistics['Average case']
            self.y_min_dist.append(y_min['Distance'])
            self.y_min_freq.append(y_min['Frequency'])
            self.y_max_dist.append(y_max['Distance'])
            self.y_max_freq.append(y_min['Frequency'])
            self.y_avg_dist.append(y_avg)
            self.y_err_dist[0].append(abs(self.y_min_dist[-1] - y_avg))
            self.y_err_dist[1].append(abs(self.y_max_dist[-1] - y_avg))

            original_pair = hash_statistics['Original pair']
            modified_pair = hash_statistics['Modified pair']
            self.original_number.append(original_pair['Number'])
            self.original_hash.append(original_pair['Hash'])
            self.modified_number.append(modified_pair['Number'])
            self.modified_hash.append(modified_pair['Hash'])

    def __bar(self, ax, fig):
        rects = plt.bar(self.hash_names, self.y_avg_dist, yerr=self.y_err_dist, ecolor='red', capsize=6, bottom=0, color='b',
                alpha=0.3, edgecolor='blue')
        plt.xticks(rotation='vertical')
        fig.set_facecolor('floralwhite')
        ax.set_facecolor('seashell')

        max_height = self.bits
        print('bits: ', self.bits)
        ax.yaxis.set_major_locator(ticker.MultipleLocator(max_height / 4))
        ax.yaxis.set_minor_locator(ticker.MultipleLocator(max_height / 16))
        plt.grid(ls=':')
        plt.ylim(top=self.bits * 1.125, bottom=-1)

    def create_histogram(self, file_name: str):
        fig, ax = plt.subplots()
        file_name = file_name.split('.')[0]
        ax.set_title(self.test_name)
        plt.xlabel(f'Hashes')
        plt.ylabel('Hamming distance')
        fig.set_figwidth(15)
        fig.set_figheight(9)

        self.__bar(ax, fig)

        file_path = os.path.join(self.hist_path, file_name + '.png')
        print('file_path: ', file_path)
        fig.savefig(file_path, bbox_inches='tight')
        plt.cla()
        plt.clf()
        plt.close()

    def get_hamming_distance_table(self):
        table = [['Название функции',
                  'Минимальное расстояние Хемминга',
                  'Частота минимального расстояния',
                  'Максимальное расстояние Хемминга',
                  'Частота максимального расстояния',
                  'Среднее значение']]
        for name, min_dist, min_freq, max_dist, max_freq, avg in \
                zip(self.hash_names, self.y_min_dist, self.y_min_freq, self.y_max_dist, self.y_max_freq, self.y_avg_dist):
            table.append([name, min_dist, min_freq, max_dist, max_freq, avg])
        return table


def create_dict_tables():
    table = [['Название функции',
              'Минимальное расстояние Хемминга',
              'Частота минимального расстояния',
              'Максимальное расстояние Хемминга',
              'Частота максимального расстояния',
              'Среднее значение']]
    return table


def create_histogram(dir_path: str, file_name: str, save_path: str):
    path_to_file = os.path.join(dir_path, file_name)
    print('path_to_file: ', path_to_file)
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        ds = AvalancheEffectStatistics(js, save_path)
        ds.create_histogram(file_name)
        return ds.get_hamming_distance_table()


def process_avalanche_effect_statistics(tests_dir_name):
    report_heading = 'Таблицы лавинного эффекта'
    path_to_test_dir = get_cpp_report_path(tests_dir_name)
    print('path_to_test_dir: ', path_to_test_dir)
    test_name = "Avalanche effect tests"
    path_to_cur_test_dir = os.path.join(path_to_test_dir, test_name)
    print('path_to_cur_test_dir: ', path_to_cur_test_dir)
    save_path = get_python_report_path(tests_dir_name)
    make_dir(save_path)
    print('save_path: ', save_path)
    save_path = os.path.join(save_path, test_name)
    make_dir(save_path)
    print('save_path: ', save_path)
    list_of_files = os.listdir(path_to_cur_test_dir)
    report = dr.create_document(report_heading)
    for file_name in list_of_files:
        dict_tables = create_histogram(path_to_cur_test_dir, file_name, save_path)
        heading = file_name.split()[0]
        dr.add_table_to_report(heading, dict_tables, report)
    dr.save_document(report, save_path, 'report.docx')






