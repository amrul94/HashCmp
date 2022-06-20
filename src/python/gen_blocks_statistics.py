import json

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

from base_statistics import *
from reports_dir import *


class GeneratedBlocksStatistics(CollisionsStatistics):
    """Класс для визуализации статистики коллизий при хешировании случайных блоков данных"""

    def __init__(self, js: dict, tests_dir_path: str):
        """
        :param js: json-структура с результатами тестов.
        :param tests_dir_path: путь к папке, в которую сохраняются результаты выполнения программы
        """
        CollisionsStatistics.__init__(self, js)
        self.mask = js['Mask']
        self.blocks_size = js['Blocks size']

        blocks_dir_path = os.path.join(tests_dir_path, str(self.blocks_size))
        make_dir(blocks_dir_path)

        self.plot_path = os.path.join(blocks_dir_path, str(self.bits))
        make_dir(self.plot_path)

    @staticmethod
    def __plot_log_scale(ax):
        """
        Устанавливает параметры построения графика в логарифмическом масштабе.
        :param ax: оси графика
        :return: None
        """
        plt.xscale('log', base=2)
        plt.yscale('log', base=2)
        x_major = ticker.LogLocator(base=2.0, numticks=50)
        ax.xaxis.set_major_locator(x_major)
        y_major = ticker.LogLocator(base=2.0, numticks=50)
        ax.yaxis.set_major_locator(y_major)

    def __fill_plot_lines(self, hash_name: str) -> tuple[list, list]:
        """
         Заполняет списки для построения графика
        :param hash_name: название хеш функции
        :return: tuple[list, list] - списки с числом хешируемых блоков и числом коллизий
        """
        count_words = []    # Число хэшируемых блоков
        collisions = []     # Число коллизий
        hash_collisions = self.collisions[hash_name]
        # заполнение списков
        for count in hash_collisions:
            count_words.append(int(count))
            collisions.append(hash_collisions[count])
        return count_words, collisions

    def __plot_hash_collisions(self, hash_name: str):
        """
        Построение графика коллизий для одной хеш функции.
        :param hash_name: название хеш функции
        :return: None
        """
        fig, ax = plt.subplots()
        if self.bits != self.mask:
            title = hash_name.split('(')[0] + '(with 32 bits xor-fold-mask)'
            ax.set_title(title)
        else:
            ax.set_title(hash_name)
        plt.xlabel('Число хешируемых данных')
        plt.ylabel('Число коллизий')
        count_words, collisions = self.__fill_plot_lines(hash_name)

        plt.plot(count_words, collisions, color='orange')
        plt.scatter(count_words, collisions, color='orange', marker='o')
        if self.bits >= 32:
            plt.ylim([1, 1 << 20])

        self.__plot_log_scale(ax)
        ax.grid(which='major', color='gray', linestyle=':')

        file_path = os.path.join(self.plot_path, f'{hash_name}.png')
        fig.savefig(file_path)
        plt.cla()
        plt.clf()
        plt.close()

    def plot(self):
        """
        Построение графиков коллизий для всех хеш функций.
        :return: None
        """
        for hash_name in self.collisions:
            self.__plot_hash_collisions(hash_name)

    def get_all_collisions(self):
        """
        Формирует списки для печати таблицы с информацией о коллизиях.
        :return: списки для печати таблицы
        """
        keys = list(self.collisions)
        collisions = [list(self.collisions[keys[0]])]
        collisions[0].insert(0, 'Название функции')
        for hash_name in self.collisions:
            collisions.append([hash_name])
            collisions[-1].extend(self.collisions[hash_name].values())
        return collisions

    def get_max_collisions(self):
        max_collisions = dict()
        for key, values in self.collisions.items():
            hash_name = key.split(' (')[0]
            collisions = [x for x in values.values()]
            max_collisions[hash_name] = collisions[-1]
        return max_collisions


def plot_graphics(sub_dir_path: str, sub_dir_name, file_name: str, report, save_path: str, collisions: dict):
    """
    Чтение json-файлов и на их основе построение гистограмм и таблиц
    :param sub_dir_path: путь к папке с json-файлами
    :param sub_dir_name: название папки с json-файлами
    :param file_name: название json-файла с результатами тестов
    :param report: docx-документ, в который будут сохранены таблицы с результатами тестов
    :param save_path: путь к папке, в которую будут сохранены графики.
    :param collisions: словарь, в который записываются коллизии 32 и 64 битных функций
    :return: None
    """
    path_to_file = os.path.join(sub_dir_path, file_name)
    with open(path_to_file, 'r') as file:
        js = json.load(file)
        # временно
        js["Blocks size"] = sub_dir_name
        cs = GeneratedBlocksStatistics(js, save_path)
        cs.plot()
        data_collisions = cs.get_all_collisions()
        if cs.bits == 32 or cs.bits == 64:
            collisions[cs.bits] = cs.get_max_collisions()
        heading = f'{cs.bits} bits ({cs.mask} bits) with {cs.blocks_size} bytes block size'
        docx_report.add_table_to_report(heading, data_collisions, report)


def open_sub_dir(root_path: str, sub_dir_name, report, save_path: str):
    """
    Проход по вложенному каталогу результатов теста коллизий
    :param root_path: путь к папке с результатами тестов
    :param sub_dir_name: название папки с результатами тестов
    :param report: docx-документ, в который будут сохранены таблицы с результатами тестов
    :param save_path: путь к папке, в которую будут сохранены графики.
    :return: словарь, в который записываются коллизии 32 и 64 битных функций
    """
    sub_dir_path = os.path.join(root_path, sub_dir_name)
    list_of_files = os.listdir(sub_dir_path)
    collisions = dict()
    for file_name in list_of_files:
        plot_graphics(sub_dir_path, sub_dir_name, file_name, report, save_path, collisions)
    return collisions


def process_statistics(report_heading: str, root_path: str, save_path):
    """
    Проходит по вложенным каталогам результатов теста.
    :param report_heading: заголовок документа docx
    :param root_path: путь к папке с результатами тестов;
    :param save_path: путь к папке, в которую будут сохранены графики и документ.
    :return: словарь, в который записываются коллизии 32 и 64 битных функций
    """
    report = docx_report.create_document(report_heading)
    dir_list = os.listdir(root_path)
    collisions = None
    for sub_dir_name in dir_list:
        collisions = open_sub_dir(root_path, sub_dir_name, report, save_path)
    docx_report.save_document(report, save_path, 'report.docx')
    return collisions


def process_collision_statistics(tests_dir_name):
    """Обработка данных тестирования устойчивости к коллизиям при хешировании случайных блоков данных.
    :param tests_dir_name: название папки с результатами тестов.
    :return: словарь, в который записываются коллизии 32 и 64 битных функций
    """
    report_heading = 'Таблицы коллизий'
    path_to_test_dir = get_cpp_report_path(tests_dir_name)
    test_name = "Generated blocks tests"
    path_to_gen_dir = os.path.join(path_to_test_dir, test_name)
    save_path = get_python_report_path(tests_dir_name)
    make_dir(save_path)
    save_path = os.path.join(save_path, test_name)
    make_dir(save_path)
    return process_statistics(report_heading, path_to_gen_dir, save_path)
