import json

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from helper import *

HIST_CHECK_DIST_PATH = 'graphics/check_dist/'


class DistributionStatistics:
    def __init__(self, js: dict, tests_dir_path: str):
        self.hash_name = js['Hash name']
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

        hist_dir_name = f'{self.bits} ({self.mode})'
        self.hist_path = os.path.join(tests_dir_path, hist_dir_name)
        make_dir(self.hist_path)

    def __bar(self, ax, fig):
        plt.bar(self.x_bars, self.y_mean, yerr=self.y_err, ecolor='red', capsize=6, bottom=0, color='b', alpha=0.3,
                edgecolor='blue')
        print(f'{self.hash_name}: x_ranges_size - {len(self.x_ranges)}')
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

    def create_histogram(self, file_name: str):
        fig, ax = plt.subplots()
        func_name = file_name.split()[0]
        ax.set_title(self.hash_name)

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


def create_dict_tables(bits: int):
    table = [['Название функции',
              'Минимальное значение',
              'Максимальное значение']]
    if bits > 24:
        return {"Bins": table[:], "Mask": table[:]}
    else:
        return {'Normal': table[:]}


def create_histogram(sub_dir_path: str, file_name: str, dict_tables, save_path:str):
    path_to_file = os.path.join(sub_dir_path, file_name)
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        ds = DistributionStatistics(js, save_path)
        ds.create_histogram(file_name)
        dict_tables[js['Mode']].append(ds.get_err())


def open_sub_dir(root_path: str, sub_dir_name, report: Document, save_path: str):
    dict_tables = create_dict_tables(int(sub_dir_name))
    sub_dir_path = os.path.join(root_path, sub_dir_name)
    list_of_files = os.listdir(sub_dir_path)
    for file_name in list_of_files:
        create_histogram(sub_dir_path, file_name, dict_tables, save_path)
    for mode in dict_tables:
        heading = f'{sub_dir_name} ({mode})'
        dr.add_table_to_report(heading, dict_tables[mode], report)


def process_distribution_statistics(tests_dir_name):
    report_heading = 'Таблицы распределений'
    path_to_test_dir = get_report_path(tests_dir_name)
    test_name = "Distribution tests"
    path_to_dist_dir = os.path.join(path_to_test_dir, test_name)
    save_path = os.path.join('graphics', tests_dir_name)
    make_dir(save_path)
    save_path = os.path.join(save_path, test_name)
    make_dir(save_path)
    print(save_path)
    process_statistics(open_sub_dir, report_heading, path_to_dist_dir, save_path)
