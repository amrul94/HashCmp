import os.path

import matplotlib.pyplot as plt

import distribution_statistics as ds
import gen_blocks_statistics as gbs
import data_collisions_statistics as dcs
import speed_statistics as ss


def main():
    reports_dir_name = "Fri Apr  8 15.48.46 2022"
    test_numbers = [4]
    for test_number in test_numbers:
        if test_number == 1:
            ds.process_distribution_statistics(reports_dir_name)
        elif test_number == 2:
            gbs.process_collision_statistics(reports_dir_name)
        elif test_number == 3:
            dcs.process_collision_statistics(reports_dir_name, "English words tests")
        elif test_number == 4:
            ss.process_collision_statistics(reports_dir_name)
        elif test_number == 6:
            dcs.process_collision_statistics(reports_dir_name, "Images tests")


if __name__ == '__main__':
    plt.rcParams['figure.max_open_warning'] = 100
    main()
