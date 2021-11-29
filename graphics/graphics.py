import matplotlib.pyplot as plt

import distribution_statistics as ds
import gen_blocks_statistics as gbs
import eng_words_statistics as ews


def main():
    test_number = 1
    if test_number == 1:
        ds.process_distribution_statistics()
    elif test_number == 2:
        gbs.process_collision_statistics()
    elif test_number == 3:
        ews.process_collision_statistics()


if __name__ == '__main__':
    plt.rcParams['figure.max_open_warning'] = 100
    main()
