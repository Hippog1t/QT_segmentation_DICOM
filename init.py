# -*- coding: utf-8 -*-
"""
Created on Mon Apr 29 17:14:07 2019

@author: Alexandre
"""

import SimpleITK as sitk
import matplotlib.image as mpimg
import numpy as np
import sys

file = sys.argv[1]
outfile = sys.argv[2]
img = sitk.ReadImage(file)
img_255 = sitk.Cast(sitk.RescaleIntensity(img), sitk.sitkUInt8)
out_img = sitk.GetArrayFromImage(img_255)
out_img = np.squeeze(out_img)

mpimg.imsave(outfile, out_img, cmap="gray")