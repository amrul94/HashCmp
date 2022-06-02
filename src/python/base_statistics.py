import os

import docx_report


class CollisionsStatistics:
    """ Базовый класс для визуализации статистики из тестов"""

    def __init__(self, js: dict):
        """
        :param js: json-структура с результатами тестов.
        """
        self.graphics_path = 'graphics'
        self.bits = js['Bits']
        self.collisions = js["Collisions"]


def process_statistics(func: callable, report_heading: str,
                       root_path: str, save_path):
    """
    Проходит по вложенным каталогам результатов теста.
    :param func: функция, которая работает с файлами во вложенных папках
    :param report_heading: заголовок документа docx
    :param root_path: путь к папке с результатами тестов;
    :param save_path: путь к папке, в которую будут сохранены графики и документ.
    :return: None
    """
    report = docx_report.create_document(report_heading)
    dir_list = os.listdir(root_path)
    for sub_dir_name in dir_list:
        func(root_path, sub_dir_name, report, save_path)
    docx_report.save_document(report, save_path, 'report.docx')
