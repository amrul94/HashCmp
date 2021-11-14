import os

from docx import Document

import docx_report as dr


class CollisionsStatistics:
    def __init__(self, js: dict):
        self.bits = js['Bits']
        self.collisions = js["Collisions"]


def process_statistics(func: callable, report_heading: str,
                       root_path: str, save_path: str):
    report = dr.create_document(report_heading)
    dir_list = os.listdir(root_path)
    for sub_dir_name in dir_list:
        func(root_path, sub_dir_name, report)
    dr.save_document(report, save_path, 'report.docx')
