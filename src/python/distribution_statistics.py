import json

import my_histogram
from base_statistics import *
from reports_dir import *


# Класс для визуализации статистики распределительных свойств хеш-функций
class DistributionStatistics:
    def __init__(self, js: dict, tests_dir_path: str):
        self.hash_name = js['Hash name']
        self.bits = js["Bits"]
        self.mode = js["Mode"]
        self.bin_size = js["Bin size"]
        self.bar_count = js["Bar count"]

        # значения гистограммы по оси X
        self.x_bars = [i + 1 for i in range(self.bar_count)]

        # значения гистограммы по оси Y
        self.y_mean = js["Y mean"]
        self.x_ranges = ["{:,}".format(number) for number in js["X ranges"]]
        self.y_err = [js["Y err min"], js["Y err max"]]
        self.y_min = js["Y min"]
        self.y_max = js["Y max"]

        hist_dir_name = f'{self.bits} ({self.mode})'
        self.hist_path = os.path.join(tests_dir_path, hist_dir_name)
        make_dir(self.hist_path)

    def create_histogram(self, file_name: str):
        # Названия на гистограмме
        x_label = 'Хеш-значения' if self.bin_size == 1 else f'Хеш-значения (бин = {self.bin_size})'
        labels = my_histogram.Labels(self.hash_name, x_label, 'Количество хешей в бине')

        # путь по которому будет построена гистограмма
        file_path = os.path.join(self.hist_path, file_name + '.png')
        # построение гистограммы
        my_histogram.histogram_with_errors(self.x_ranges, self.y_mean, self.y_err, labels, file_path)

    # возвращает минимальное и максимальное значение счетчика хешей в бинах
    def get_err(self):
        min_counter = min(self.y_min)
        max_counter = max(self.y_max)
        return [self.hash_name, min_counter, max_counter]


# Создает списки, в которые будет записана информация
# распределительных свойствах хеш-функций.
# На основе этих таблиц составляется таблица в docx
def create_dict_tables(bits: int):
    table = [['Название функции',
              'Минимальное значение',
              'Максимальное значение']]
    if bits > 32:
        return {"Bins": table[:], "Mask": table[:]}
    else:
        return {'Normal': table[:]}


# Чтение json-файлов и на их основе построение гистограмм и таблиц
def create_histogram(sub_dir_path: str, file_name: str, dict_tables, save_path: str):
    path_to_file = os.path.join(sub_dir_path, file_name)
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        ds = DistributionStatistics(js, save_path)
        ds.create_histogram(file_name.split()[0])
        dict_tables[js['Mode']].append(ds.get_err())


# Проход по вложенному каталогу результатов теста распределения
def open_sub_dir(root_path: str, sub_dir_name, report, save_path: str):
    dict_tables = create_dict_tables(int(sub_dir_name))
    sub_dir_path = os.path.join(root_path, sub_dir_name)
    list_of_files = os.listdir(sub_dir_path)
    for file_name in list_of_files:
        create_histogram(sub_dir_path, file_name, dict_tables, save_path)
    for mode in dict_tables:
        heading = f'{sub_dir_name} ({mode})'
        docx_report.add_table_to_report(heading, dict_tables[mode], report)


# Обработки данных тестирования распределения
def process_distribution_statistics(tests_dir_name):
    report_heading = 'Таблицы распределений'
    path_to_test_dir = get_cpp_report_path(tests_dir_name)
    test_name = "Distribution tests"
    path_to_dist_dir = os.path.join(path_to_test_dir, test_name)
    save_path = get_python_report_path(tests_dir_name)
    make_dir(save_path)
    save_path = os.path.join(save_path, test_name)
    make_dir(save_path)
    process_statistics(open_sub_dir, report_heading, path_to_dist_dir, save_path)
