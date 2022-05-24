import json

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

from base_statistics import *
from reports_dir import *


# Класс для визуализации статистики коллизий при хешировании
# случайных блоков данных
class GeneratedBlocksStatistics(CollisionsStatistics):
    # Конструктор класса
    def __init__(self, js: dict, tests_dir_path: str):
        CollisionsStatistics.__init__(self, js)
        self.mask = js['Mask']
        self.blocks_size = js['Blocks size']

        blocks_dir_path = os.path.join(tests_dir_path, str(self.blocks_size))
        make_dir(blocks_dir_path)

        self.plot_path = os.path.join(blocks_dir_path, str(self.bits))
        make_dir(self.plot_path)

    # Устанавливает параметры построения графика в логарифмическом масштабе
    @staticmethod
    def __plot_log_scale(ax):
        plt.xscale('log', base=2)
        plt.yscale('log', base=2)
        x_major = ticker.LogLocator(base=2.0, numticks=50)
        ax.xaxis.set_major_locator(x_major)
        y_major = ticker.LogLocator(base=2.0, numticks=50)
        ax.yaxis.set_major_locator(y_major)

    # Заполняет списки для построения графика
    def __fill_plot_lines(self, hash_name: str) -> tuple[list, list]:
        count_words = []    # Число хэшируемых блоков
        collisions = []     # Число коллизий
        hash_collisions = self.collisions[hash_name]
        # заполнение списков
        for count in hash_collisions:
            count_words.append(int(count))
            collisions.append(hash_collisions[count])
        return count_words, collisions

    # Построение графика коллизий для одной хеш-функции
    def __plot_hash_collisions(self, hash_name: str):
        fig, ax = plt.subplots()
        ax.set_title(hash_name)
        plt.xlabel('Число хешируемых данных')
        plt.ylabel('Число коллизий')
        count_words, collisions = self.__fill_plot_lines(hash_name)

        plt.plot(count_words, collisions, color='black')
        plt.scatter(count_words, collisions, color='black', marker='o')

        self.__plot_log_scale(ax)

        ax.grid(which='major', color='gray', linestyle=':')
        if self.bits != self.mask:
            file_name = f'{hash_name} {self.bits} bits (mask {self.mask} bits) (log).png'
        else:
            file_name = f'{hash_name} {self.bits} bits (log).png'

        file_path = os.path.join(self.plot_path, file_name)
        fig.savefig(file_path)
        plt.cla()
        plt.clf()
        plt.close()

    # Построение графиков коллизий для всех хеш-функций
    def plot(self):
        for hash_name in self.collisions:
            self.__plot_hash_collisions(hash_name)

    # Формирует списки для печати таблицы с информацией о коллизиях
    def get_collisions(self):
        keys = list(self.collisions)
        collisions = [list(self.collisions[keys[0]])]
        collisions[0].insert(0, 'Название функции')
        for hash_name in self.collisions:
            collisions.append([hash_name])
            collisions[-1].extend(self.collisions[hash_name].values())
        return collisions


# Чтение json-файлов и на их основе построение гистограмм и таблиц
def plot_graphics(sub_dir_path: str, sub_dir_name, file_name: str, report, save_path: str):
    path_to_file = os.path.join(sub_dir_path, file_name)
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        # временно
        js["Blocks size"] = sub_dir_name
        cs = GeneratedBlocksStatistics(js, save_path)
        cs.plot()
        cs.get_collisions()
        data_collisions = cs.get_collisions()
        heading = f'{cs.bits} bits ({cs.mask} bits) with {cs.blocks_size} bytes block size'
        docx_report.add_table_to_report(heading, data_collisions, report)


# Проход по вложенному каталогу результатов теста коллизий
def open_sub_dir(root_path: str, sub_dir_name, report, save_path: str):
    sub_dir_path = os.path.join(root_path, sub_dir_name)
    list_of_files = os.listdir(sub_dir_path)
    for file_name in list_of_files:
        plot_graphics(sub_dir_path, sub_dir_name, file_name, report, save_path)


# Обработки данных тестирования устойчивости к коллизиям
# при хешировании случайных блоков данных
def process_collision_statistics(tests_dir_name):
    report_heading = 'Таблицы коллизий'
    path_to_test_dir = get_cpp_report_path(tests_dir_name)
    test_name = "Generated blocks tests"
    path_to_gen_dir = os.path.join(path_to_test_dir, test_name)
    save_path = get_python_report_path(tests_dir_name)
    make_dir(save_path)
    save_path = os.path.join(save_path, test_name)
    make_dir(save_path)
    process_statistics(open_sub_dir, report_heading, path_to_gen_dir, save_path)
