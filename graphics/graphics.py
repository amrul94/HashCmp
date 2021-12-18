import os.path

import matplotlib.pyplot as plt

import distribution_statistics as ds
import gen_blocks_statistics as gbs
import eng_words_statistics as ews


def main():
    reports_dir_name = "Report v2.1. (12.12.2021)"
    test_numbers = [1, 2, 3]
    for test_number in test_numbers:
        if test_number == 1:
            ds.process_distribution_statistics(reports_dir_name)
        elif test_number == 2:
            gbs.process_collision_statistics(reports_dir_name)
        elif test_number == 3:
            ews.process_collision_statistics(reports_dir_name)


if __name__ == '__main__':
    plt.rcParams['figure.max_open_warning'] = 100
    main()
