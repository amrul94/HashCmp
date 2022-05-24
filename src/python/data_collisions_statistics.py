import json
import os.path

import my_histogram
from base_statistics import *
from reports_dir import *


# Класс для визуализации статистики коллизий при хешировании
# изображений и английских слов
class DataCollisionsStatistics(CollisionsStatistics):
    # Конструктор класса
    def __init__(self, tests_data: dict, tests_dir_name: str):
        CollisionsStatistics.__init__(self, tests_data)
        self.tests_dir_path = get_python_report_path(tests_dir_name)
        self.test_name = tests_data["Test name"]
        self.hist_path = os.path.join(self.tests_dir_path, self.test_name)
        make_dir(self.tests_dir_path)
        make_dir(self.hist_path)

    # Статический метод, который устанавливает число коллизий на гистограммах
    @staticmethod
    def __auto_label(ax, rects):
        for rect in rects:
            height = rect.get_height()
            ax.text(rect.get_x() + rect.get_width() / 2., 1.01 * height,
                    height, ha='center', va='bottom')

    # Построение гистограммы
    def create_histogram(self):
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
    path_to_file = os.path.join(dir_path, file_name)
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        ews = DataCollisionsStatistics(js, tests_dir_name)
        ews.create_histogram()


# Обработки данных тестирования устойчивости к коллизиям
# при хешировании изображений и английских слов
def process_collision_statistics(tests_dir_name: str, test_name: str):
    path_to_tests_dir = get_cpp_report_path(tests_dir_name)
    path_to_cur_test_dir = os.path.join(path_to_tests_dir, test_name)
    list_of_files = os.listdir(path_to_cur_test_dir)
    for file_name in list_of_files:
        create_histogram(tests_dir_name, path_to_cur_test_dir, file_name)
