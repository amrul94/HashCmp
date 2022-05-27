import os


# Выдает путь к общей папке с отчетами
def get_reports_path(reports_dir_name):
    all_reports_path = '../../reports'
    return os.path.join(all_reports_path, reports_dir_name)


# Выдает путь к папке с отчетами тестирования
def get_cpp_report_path(reports_dir_name):
    current_reports_path = get_reports_path(reports_dir_name)
    return os.path.join(current_reports_path, 'cpp')


# Выдает путь к папке, в которую будут сохранены графики и таблицы
def get_python_report_path(reports_dir_name):
    current_reports_path = get_reports_path(reports_dir_name)
    return os.path.join(current_reports_path, 'python')


# Создает папку
def make_dir(path_to_dir):
    if not os.path.exists(path_to_dir):
        os.mkdir(path_to_dir)
