import sys
import os.path

import matplotlib.pyplot as plt

import avalanche_effect_statistics as ae
import distribution_statistics as ds
import gen_blocks_statistics as gbs
import data_collisions_statistics as dcs
import speed_statistics as ss


def main():
    plt.rcParams['figure.max_open_warning'] = 100
    reports_dir_name = "Report 26.04.2022"
    test_numbers = [1, 2, 3, 4, 5, 6]
    for test_number in test_numbers:
        if test_number == 1:
            ae.process_avalanche_effect_statistics(reports_dir_name)
        elif test_number == 2:
            ds.process_distribution_statistics(reports_dir_name)
        elif test_number == 3 or test_number == 4:
            gbs.process_collision_statistics(reports_dir_name)
        elif test_number == 5:
            dcs.process_collision_statistics(reports_dir_name, "English words tests")
        elif test_number == 6:
            dcs.process_collision_statistics(reports_dir_name, "Images tests")
        elif test_number == 7:
            ss.process_speed_statistics(reports_dir_name)


if __name__ == '__main__':
    main()
