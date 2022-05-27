import os

import docx_report


# Базовый класс для визуализации статистики из тестов
class CollisionsStatistics:
    def __init__(self, js: dict):
        self.graphics_path = 'graphics'
        self.bits = js['Bits']
        self.collisions = js["Collisions"]


# Проходит по вложенным каталогам результатов теста
def process_statistics(func: callable, report_heading: str,
                       root_path: str, save_path):
    report = docx_report.create_document(report_heading)
    dir_list = os.listdir(root_path)
    for sub_dir_name in dir_list:
        func(root_path, sub_dir_name, report, save_path)
    docx_report.save_document(report, save_path, 'report.docx')
