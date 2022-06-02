import os


def get_reports_path(reports_dir_name):
    """
    Выдает путь к общей папке с отчетами.
    :param reports_dir_name: название папки с отчетами.
    :return: путь к общей папке с отчетами.
    """
    all_reports_path = '../../reports'
    return os.path.join(all_reports_path, reports_dir_name)


def get_cpp_report_path(reports_dir_name):
    """
    Выдает путь к папке с отчетами тестирования.
    :param reports_dir_name: название папки с отчетами.
    :return: путь к общей папке с отчетами тестирования
    """
    current_reports_path = get_reports_path(reports_dir_name)
    return os.path.join(current_reports_path, 'cpp')


#
def get_python_report_path(reports_dir_name):
    """
    Выдает путь к папке, в которую будут сохранены графики и таблицы.
    :param reports_dir_name: название папки с отчетами.
    :return: путь к папке, в которую будут сохранены графики и таблицы
    """
    current_reports_path = get_reports_path(reports_dir_name)
    return os.path.join(current_reports_path, 'python')


# Создает папку
def make_dir(path_to_dir):
    if not os.path.exists(path_to_dir):
        os.mkdir(path_to_dir)
