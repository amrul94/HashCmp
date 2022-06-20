import matplotlib.pyplot as plt

import avalanche_effect_statistics as avalanche_stats
import distribution_statistics as dist_stats
import gen_blocks_statistics as gbs
import data_collisions_statistics as dcs
import speed_statistics as ss
import hashes_diagram as hd


def main():
    """
    Основная функция, которая запускает тесты
    :return: None
    """
    plt.rcParams['figure.max_open_warning'] = 100
    reports_dir_name = "Report 26.04.2022"
    test_numbers = {1, 2, 3, 6}
    counter = 0
    hashes_info = dict()
    for test_number in test_numbers:
        if test_number == 1:
            hashes_info['Avalanches'] = avalanche_stats.process_avalanche_effect_statistics(reports_dir_name)
            counter += 1
        elif test_number == 2:
            hashes_info['Distributions'] = dist_stats.process_distribution_statistics(reports_dir_name)
            counter += 1
        elif test_number == 3:
            hashes_info['Collisions'] = gbs.process_collision_statistics(reports_dir_name)
            counter += 1
        elif test_number == 4:
            dcs.process_collision_statistics(reports_dir_name, "English words tests")
        elif test_number == 5:
            dcs.process_collision_statistics(reports_dir_name, "Images tests")
        elif test_number == 6:
            hashes_info['Speeds'] = ss.process_speed_statistics(reports_dir_name)
            counter += 1
    if counter >= 2:
        hd.create_hashes_diagram(reports_dir_name, hashes_info, 32)
        hd.create_hashes_diagram(reports_dir_name, hashes_info, 64)


if __name__ == '__main__':
    main()
