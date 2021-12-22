import os

from docx import Document

import docx_report as dr


def get_report_path(reports_dir_name):
    reports_path = 'reports'
    result = os.path.join(reports_path, reports_dir_name)
    return result


def make_dir(path_to_dir):
    if not os.path.exists(path_to_dir):
        os.mkdir(path_to_dir)


class CollisionsStatistics:

    def __init__(self, js: dict):
        self.graphics_path = 'graphics'
        self.bits = js['Bits']
        self.collisions = js["Collisions"]


def process_statistics(func: callable, report_heading: str,
                       root_path: str, save_path):
    report = dr.create_document(report_heading)
    dir_list = os.listdir(root_path)
    for sub_dir_name in dir_list:
        func(root_path, sub_dir_name, report, save_path)
    dr.save_document(report, save_path, 'report.docx')
