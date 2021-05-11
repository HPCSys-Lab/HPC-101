import numpy as np
import matplotlib.pyplot as plt
import argparse
import os

parser = argparse.ArgumentParser(description='How to use this program')
parser.add_argument("--file", type=str, required=True, help="Path to the wavefield file")
parser.add_argument("--name", type=str, required=False, default="wavefield", help="Name of the image to be generated")
args = parser.parse_args()

# create the destination dir
os.system('mkdir -p plots')

# read the input (wavefield N x M) file
input = np.loadtxt(args.file)

# process data e generate the plot
plt.imshow(input)
plt.savefig('plots/{}.png'.format(args.name), format='png')

print("Plot saved in plots/{}.png".format(args.name))
