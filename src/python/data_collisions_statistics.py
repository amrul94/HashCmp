import json
import os.path

import my_histogram
from base_statistics import *
from reports_dir import *


class DataCollisionsStatistics(CollisionsStatistics):
    """Класс для визуализации статистики коллизий при хешировании изображений и английских слов."""

    def __init__(self, tests_data: dict, tests_dir_name: str):
        """
        :param tests_data: json-структура с результатами тестов;
        :param tests_dir_name: путь к папке, в которую сохраняются результаты выполнения программы
        """
        CollisionsStatistics.__init__(self, tests_data)
        self.tests_dir_path = get_python_report_path(tests_dir_name)
        self.test_name = tests_data["Test name"]
        self.hist_path = os.path.join(self.tests_dir_path, self.test_name)
        make_dir(self.tests_dir_path)
        make_dir(self.hist_path)

    @staticmethod
    def __auto_label(ax, rects):
        """
        Статический метод, который устанавливает число коллизий на гистограммах.
        :param ax: оси графиков;
        :param rects: контейнер со всеми гистограммами.
        :return: None
        """
        for rect in rects:
            height = rect.get_height()
            ax.text(rect.get_x() + rect.get_width() / 2., 1.01 * height,
                    height, ha='center', va='bottom')

    def create_histogram(self):
        """
        Построение гистограммы.
        :return: None
        """

        # Названия на гистограмме
        title = f'Гистограмма коллизий ({self.bits}-битные хеш функции)'
        legend_labels = []
        labels = my_histogram.Labels(title, 'Хеш функции', 'Число коллизий', legend_labels)

        # путь по которому будет построена гистограмма
        file_path = os.path.join(self.hist_path, f'{self.bits} bits.png')
        is_32_bits_images_test = self.bits == 32 and self.test_name == 'Images tests'
        # построение гистограммы
        my_histogram.histogram(self.collisions, labels, file_path, self.__auto_label, is_32_bits_images_test)


# Чтение json-файлов и на их основе построение гистограмм и таблиц
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
        ews = DataCollisionsStatistics(js, tests_dir_name)
        ews.create_histogram()


def process_collision_statistics(tests_dir_name: str, test_name: str):
    """
    Обработки данных тестирования устойчивости к коллизиям при хешировании изображений и английских слов.
    :param tests_dir_name: название папки с результатами тестов;
    :param test_name: название теста.
    :return: None
    """
    path_to_tests_dir = get_cpp_report_path(tests_dir_name)
    path_to_cur_test_dir = os.path.join(path_to_tests_dir, test_name)
    list_of_files = os.listdir(path_to_cur_test_dir)
    for file_name in list_of_files:
        create_histogram(tests_dir_name, path_to_cur_test_dir, file_name)
