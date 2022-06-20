import json
import math
from statistics import mean
import my_histogram
from base_statistics import *
from reports_dir import *


class DistributionStatistics:
    """Класс для визуализации статистики распределительных свойств хеш-функций"""

    def __init__(self, js: dict, tests_dir_path: str):
        """
        :param js: json-структура с результатами тестов;
        :param tests_dir_path: путь к папке, в которую сохраняются результаты выполнения программы
        """
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

    def create_histogram(self):
        """
        Построение гистограммы.
        :return: None
        """
        # Названия на гистограмме
        labels = my_histogram.Labels(self.hash_name, 'Хеш-значения', 'Значение счетчика хеш-значений')

        # путь по которому будет построена гистограмма
        file_path = os.path.join(self.hist_path, self.hash_name + '.png')
        # построение гистограммы
        my_histogram.histogram_with_errors(self.x_ranges, self.y_mean, self.y_err, labels, file_path)

    def get_err(self):
        """
        :return: минимальное и максимальное значение счетчика хешей в бинах
        """
        min_counter = min(self.y_min)
        max_counter = max(self.y_max)
        return [self.hash_name, min_counter, max_counter]


def create_dict_tables(bits: int):
    """
    :param bits: число бит хеш функции.
    :return: списки для построения таблицы с результатами тестов
    """
    table = [['Название функции',
              'Минимальное значение',
              'Максимальное значение']]
    if bits > 32:
        return {"Bins": table[:], "Mask": table[:]}
    else:
        return {'Normal': table[:]}


def create_histogram(sub_dir_path: str, file_name: str, dict_tables, save_path: str):
    """
    Чтение json-файлов и на их основе построение гистограмм и таблиц
    :param sub_dir_path: путь к папке с json-файлами
    :param file_name: json-файл с результатами тестов
    :param dict_tables: списки для построения таблицы с результатами тестов
    :param save_path: путь к папке, в которую будут сохранены графики.
    :return: None
    """
    path_to_file = os.path.join(sub_dir_path, file_name)
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        ds = DistributionStatistics(js, save_path)
        ds.create_histogram()
        minmax = ds.get_err()
        dict_tables[js['Mode']].append(minmax)
    return minmax


def open_sub_dir(root_path: str, sub_dir_name, report, save_path: str, distributions):
    """
    Проход по вложенному каталогу результатов теста распределения
    :param root_path: путь к папке с результатами тестов
    :param sub_dir_name: название папки с результатами тестов
    :param report: docx-документ, в который будут сохранены таблицы с результатами тестов
    :param save_path: путь к папке, в которую будут сохранены графики.
    :param distributions: словарь, в который записываются распределения 32 и 64 битных функций
    :return: None
    """
    num_bits = int(sub_dir_name)
    dict_tables = create_dict_tables(int(sub_dir_name))
    sub_dir_path = os.path.join(root_path, sub_dir_name)
    list_of_files = os.listdir(sub_dir_path)
    for file_name in list_of_files:
        minmax = create_histogram(sub_dir_path, file_name, dict_tables, save_path)
        if num_bits == 32 or num_bits == 64:
            distributions[num_bits][minmax[0]] = math.log2(mean(minmax[1:]))
    for mode in dict_tables:
        heading = f'{sub_dir_name} ({mode})'
        docx_report.add_table_to_report(heading, dict_tables[mode], report)


def process_statistics(report_heading: str, root_path: str, save_path):
    """
    Проходит по вложенным каталогам результатов теста.
    :param report_heading: заголовок документа docx
    :param root_path: путь к папке с результатами тестов;
    :param save_path: путь к папке, в которую будут сохранены графики и документ.
    :return: None
    """
    report = docx_report.create_document(report_heading)
    dir_list = os.listdir(root_path)
    distributions = dict()
    for sub_dir_name in dir_list:
        distributions[int(sub_dir_name)] = dict()
        open_sub_dir(root_path, sub_dir_name, report, save_path, distributions)
    docx_report.save_document(report, save_path, 'report.docx')
    return distributions


def process_distribution_statistics(tests_dir_name):
    """
    Обработки данных тестирования распределения
    :param tests_dir_name: название папки с результатами тестов.
    :return: None
    """
    report_heading = 'Таблицы распределений'
    path_to_test_dir = get_cpp_report_path(tests_dir_name)
    test_name = "Distribution tests"
    path_to_dist_dir = os.path.join(path_to_test_dir, test_name)
    save_path = get_python_report_path(tests_dir_name)
    make_dir(save_path)
    save_path = os.path.join(save_path, test_name)
    make_dir(save_path)
    return process_statistics(report_heading, path_to_dist_dir, save_path)
