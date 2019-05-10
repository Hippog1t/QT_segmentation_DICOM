# -*- coding: utf-8 -*-
"""
Created on Fri Apr 12 10:17:23 2019

@author: Alexandre
"""

import SimpleITK as sitk
import sys
import matplotlib.image as mpimg
import numpy as np

filename = sys.argv[1]
outputfile = sys.argv[2]+"/Out/threshold.jpg"
threshold = int(sys.argv[3])

img = sitk.ReadImage(filename)
img_255= sitk.Cast(sitk.RescaleIntensity(img), sitk.sitkUInt8)

seg = img>threshold

imgtest = np.squeeze(sitk.GetArrayFromImage(sitk.LabelOverlay(img_255, seg)))

mpimg.imsave(outputfile, imgtest)