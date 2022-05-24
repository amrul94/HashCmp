import matplotlib.pyplot as plt

import avalanche_effect_statistics as avalanche_stat
import distribution_statistics as distribution_stat
import data_collisions_statistics as data_stat
import gen_blocks_statistics as gen_stat
import speed_statistics as speed_stat


def main():
    plt.rcParams['figure.max_open_warning'] = 100
    reports_dir_name = "Report 26.04.2022"
    test_numbers = [1, 2, 3, 4, 5, 6]
    for test_number in test_numbers:
        if test_number == 1:
            avalanche_stat.process_avalanche_effect_statistics(reports_dir_name)
        elif test_number == 2:
            distribution_stat.process_distribution_statistics(reports_dir_name)
        elif test_number == 3:
            data_stat.process_collision_statistics(reports_dir_name, "English words tests")
        elif test_number == 4:
            data_stat.process_collision_statistics(reports_dir_name, "Images tests")
        elif test_number == 5:
            gen_stat.process_collision_statistics(reports_dir_name)
        elif test_number == 6:
            speed_stat.process_speed_statistics(reports_dir_name)


if __name__ == '__main__':
    main()
