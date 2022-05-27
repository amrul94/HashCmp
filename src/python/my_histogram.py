import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np


class Labels:
    def __init__(self, title, x_label, y_label, legend_labels=None):

        self.title = title
        self.x_label = x_label
        self.y_label = y_label
        self.legend_labels = legend_labels


class Limits:
    def __init__(self, top, bottom=0):
        self.top = top
        self.bottom = bottom


class Locators:
    def __init__(self, major, minor):
        self.major = major
        self.minor = minor


def histogram_with_errors(x, height, errors, labels, file_path, locators=None, limits=None, plot_points=None):
    fig, ax = plt.subplots()
    ax.set_title(labels.title, fontsize=18)
    plt.xlabel(labels.x_label, fontsize=16)
    plt.ylabel(labels.y_label, fontsize=16)

    fig.set_figwidth(12)
    fig.set_figheight(8)

    if plot_points is not None:
        # ax.plot(x, plot_points, 'xk', markersize=7)
        ax.plot(x, plot_points, 'xk', markersize=7)
    # ax.bar(x, height, yerr=errors, ecolor='black', capsize=6, bottom=0, color='grey', alpha=0.5, linewidth=1)
    ax.bar(x, height, yerr=errors, edgecolor='red', ecolor='darkred', capsize=6, bottom=0, color='orange', linewidth=1)

    if labels.legend_labels is not None:
        ax.legend(labels=labels.legend_labels, fontsize=10)

    plt.xticks(rotation='vertical')
    if locators is not None:
        ax.yaxis.set_major_locator(ticker.MultipleLocator(locators.major))
        ax.yaxis.set_minor_locator(ticker.MultipleLocator(locators.minor))

    plt.grid(ls=':')
    if limits is not None:
        plt.ylim(top=limits.top, bottom=limits.bottom)

    fig.savefig(file_path, bbox_inches='tight')
    plt.cla()
    plt.clf()
    plt.close()


def histogram(bars, labels, file_path, auto_label, is_log_scale=False):
    fig, ax = plt.subplots()
    ax.set_title(labels.title, fontsize=18)
    plt.xlabel(labels.x_label, fontsize=16)
    plt.ylabel(labels.y_label, fontsize=16)

    fig.set_figwidth(12)
    fig.set_figheight(8)

    x = bars.keys()
    height = bars.values()
    # rects = plt.bar(x, height, color='grey', alpha=0.5, edgecolor='black', linewidth=1)
    rects = plt.bar(x, height, color='orange', edgecolor='red', linewidth=1)
    plt.xticks(rotation='vertical')
    auto_label(ax, rects)

    if is_log_scale:
        ax.set_yscale('log', base=2)
    else:
        max_height = max([np.around(rect.get_height()) for rect in rects])
        if max_height > 1:
            max_ticket = int(np.around(max_height, -1))
            ax.yaxis.set_major_locator(ticker.MultipleLocator(max_ticket * 0.2))
            ax.yaxis.set_minor_locator(ticker.MultipleLocator(max_ticket * 0.04))
            plt.ylim(top=max_height * 1.125, bottom=0)
        else:
            plt.ylim(bottom=0)
    plt.grid(ls=':')

    fig.savefig(file_path, bbox_inches='tight')
