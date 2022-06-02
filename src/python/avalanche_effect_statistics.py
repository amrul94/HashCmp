import json

import my_histogram
from base_statistics import *
from reports_dir import *


class AvalancheEffectStatistics:
    """Класс для визуализации статистики лавинного эффекта"""

    def __init__(self, js: dict, tests_dir_path: str | bytes):
        """
        :param js: json-структура с результатами тестов;
        :param tests_dir_path: путь к папке, в которую сохраняются результаты выполнения программы
        """
        self.test_name = js['Test name']
        self.bits = js["Bits"]
        self.hist_path = tests_dir_path
        make_dir(tests_dir_path)
        make_dir(self.hist_path)

        self.hash_names = []
        # списки параметров для построения гистограммы и таблицы
        self.minimum_distances = []
        self.minimum_frequencies = []
        self.maximum_distances = []
        self.maximum_frequencies = []
        self.average_distance = []
        self.median_distances = []
        self.error_distances = [[], []]
        self.original_number = []
        self.original_hash = []
        self.modified_number = []
        self.modified_hash = []
        # заполнение списков параметров
        self.__fill_statistics(js["Avalanche effect"])

    def __fill_statistics(self, avalanche_statistics: dict):
        """
        Метод для заполнения списков параметров.
        :param avalanche_statistics: результаты тестирования лавинного эффекта
        """
        for hash_name, hash_statistics in avalanche_statistics.items():
            self.hash_names.append(hash_name)
            y_min = hash_statistics['Worst case']
            y_max = hash_statistics['Best case']
            y_avg = hash_statistics['Average case']
            y_med = hash_statistics['Median case']
            self.minimum_distances.append(y_min['Distance'])
            self.minimum_frequencies.append(y_min['Frequency'])
            self.maximum_distances.append(y_max['Distance'])
            self.maximum_frequencies.append(y_max['Frequency'])
            self.average_distance.append(y_avg)
            self.median_distances.append(y_med)
            self.error_distances[0].append(abs(self.minimum_distances[-1] - y_avg))
            self.error_distances[1].append(abs(self.maximum_distances[-1] - y_avg))

            original_pair = hash_statistics['Original pair']
            modified_pair = hash_statistics['Modified pair']
            self.original_number.append(original_pair['Number'])
            self.original_hash.append(original_pair['Hash'])
            self.modified_number.append(modified_pair['Number'])
            self.modified_hash.append(modified_pair['Hash'])

    def create_histogram(self, file_name: str):
        """
        Построение гистограммы.
        :param file_name: название файла с графиком
        """

        # Названия на гистограмме
        title = f'Лавинный эффект ({self.bits}-битные хеш функции)'
        legend_labels = ['Медианное расстояние Хемминга', 'Среднее расстояние Хемминга']
        labels = my_histogram.Labels(title, 'Хеш функции', 'Расстояние Хемминга', legend_labels)

        # путь по которому будет построена гистограмма
        file_path = os.path.join(self.hist_path, file_name + '.png')
        # пределы по оси Y
        limits = my_histogram.Limits(self.bits * 1.125, -1)
        # Шаг сетки
        locators = my_histogram.Locators(self.bits >> 2, self.bits >> 4)
        # построение гистограммы
        my_histogram.histogram_with_errors(self.hash_names, self.average_distance, self.error_distances, labels,
                                           file_path, locators, limits, self.median_distances)

    def get_hamming_distance_table(self):
        """
        Заполняет списки для построения таблицы расстояния Хемминга в docx
        :return: list[list[str] | list] - списки для построения таблицы
        """
        table = [['Название функции',
                  'Минимальное расстояние Хемминга',
                  'Частота минимального расстояния',
                  'Максимальное расстояние Хемминга',
                  'Частота максимального расстояния',
                  'Среднее расстояние Хемминга',
                  'Медианное расстояние Хемминга']]
        for name, min_dist, min_freq, max_dist, max_freq, avg, median in \
                zip(self.hash_names, self.minimum_distances, self.minimum_frequencies, self.maximum_distances,
                    self.maximum_frequencies, self.average_distance, self.median_distances):
            table.append([name, min_dist, min_freq, max_dist, max_freq, avg, median])
        return table


def create_histogram(dir_path: str, file_name: str, save_path: str):
    """
    Чтение json-файлов и на их основе построение гистограмм и таблиц.
    :param dir_path: путь к папке с результатами тестов;
    :param file_name: название json-файла с результатами тестов;
    :param save_path: путь к папке, в которую будут сохранены графики.
    :return: list[list[str] | list] - списки для построения таблицы
    """
    path_to_file = os.path.join(dir_path, file_name)
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        ds = AvalancheEffectStatistics(js, save_path)
        ds.create_histogram(file_name.split('.')[0])
        return ds.get_hamming_distance_table()


def process_avalanche_effect_statistics(tests_dir_name: str):
    """
    Обработки данных тестирования лавинного эффекта.
    :param tests_dir_name: название папки с результатами тестов.
    :return: None
    """
    report_heading = 'Таблицы лавинного эффекта'
    path_to_test_dir = get_cpp_report_path(tests_dir_name)
    test_name = "Avalanche effect tests"
    path_to_cur_test_dir = os.path.join(path_to_test_dir, test_name)
    save_path = get_python_report_path(tests_dir_name)
    make_dir(save_path)
    save_path = os.path.join(save_path, test_name)
    make_dir(save_path)
    list_of_files = os.listdir(path_to_cur_test_dir)
    report = docx_report.create_document(report_heading)
    for file_name in list_of_files:
        dict_tables = create_histogram(path_to_cur_test_dir, file_name, save_path)
        heading = file_name.split('.')[0]
        docx_report.add_table_to_report(heading, dict_tables, report)
    docx_report.save_document(report, save_path, 'report.docx')

