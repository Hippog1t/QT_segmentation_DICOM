# -*- coding: utf-8 -*-
"""
Created on Mon Apr 29 17:14:07 2019

@author: Alexandre
"""

import SimpleITK as sitk
import matplotlib.image as mpimg
import numpy as np
import sys

pathOut = sys.argv[1]

numberFile = 0

for i in range(2, len(sys.argv)):
    file = sys.argv[i]
    img = sitk.ReadImage(file)
    img_255 = sitk.Cast(sitk.RescaleIntensity(img), sitk.sitkUInt8)
    out_img = sitk.GetArrayFromImage(img_255)
    out_img = np.squeeze(out_img)
    mpimg.imsave(pathOut+"/Out/initial"+str(numberFile)+".jpg", out_img, cmap="gray")
    numberFile += 1