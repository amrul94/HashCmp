import json
import os.path

import numpy as np

import my_histogram
from reports_dir import *


class SpeedStatistics:
    """Класс для визуализации статистики скорости хеш-функций"""

    def __init__(self, tests_data: dict, tests_dir_name: str):
        """
        :param tests_data: json-структура с результатами тестов;
        :param tests_dir_name: название папки, в которую будут сохранены графики
        """
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
        """
        Статический метод, который устанавливает скорость хеширования на гистограммах.
        :param ax: оси графиков;
        :param rects: контейнер со всеми гистограммами.
        :return: None
        """
        for rect in rects:
            height = np.around(rect.get_height(), 1)
            ax.text(rect.get_x() + rect.get_width() / 2., height + 0.3, height, ha='center', va='bottom')

    def create_histogram(self):
        """
        Построение гистограммы.
        :return: None
        """
        title = f'Скорость хеширования ({self.bits}-битные хеш функции)'
        legend_labels = []
        labels = my_histogram.Labels(title, 'Хеш функции', 'Число Время хеширования (в секундах)', legend_labels)
        file_path = os.path.join(self.hist_path, f'{self.bits} bits.png')
        my_histogram.histogram(self.speeds, labels, file_path, self.__auto_label)


def create_histogram(tests_dir_name: str, dir_path: str, file_name: str | bytes):
    """
    Чтение json-файлов и на их основе построение гистограмм
    :param tests_dir_name: название папки с тестами
    :param dir_path: путь к папке с тестами
    :param file_name: название json-файла с результатами тестов
    :return: None
    """
    path_to_file = os.path.join(dir_path, file_name)
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        ews = SpeedStatistics(js, tests_dir_name)
        ews.create_histogram()


def process_speed_statistics(tests_dir_name):
    """
    Обработки данных тестирования скорости хешировании
    :param tests_dir_name: название папки с результатами тестов
    :return: None
    """
    path_to_test_dir = get_cpp_report_path(tests_dir_name)
    path_to_eg_dir = os.path.join(path_to_test_dir, "Speed tests")
    list_of_files = os.listdir(path_to_eg_dir)
    for file_name in list_of_files:
        create_histogram(tests_dir_name, path_to_eg_dir, file_name)
