# -*- coding: utf-8 -*-
"""
Created on Mon Apr 29 08:13:39 2019

@author: Alexandre
"""

import SimpleITK as sitk
import matplotlib.image as mpimg
import numpy as np
import sys
    
x_seed = int(sys.argv[1])
y_seed = int(sys.argv[2])

inputFile = sys.argv[3]
outputFile = sys.argv[4]+"/Out/regionGrow.jpg"

img = sitk.ReadImage(inputFile)

seed = (x_seed,y_seed,0)   
    
img_255 = sitk.Cast(sitk.RescaleIntensity(img), sitk.sitkUInt8)
    
seg_conf = sitk.ConfidenceConnected(img_255,
                                    seedList = [seed],
                                    numberOfIterations = 1,
                                    multiplier = 2.5,
                                    initialNeighborhoodRadius = 1,
                                    replaceValue = 1)

seg_conf = sitk.BinaryDilate(seg_conf, 3)
    
out_img = sitk.GetArrayFromImage(sitk.LabelOverlay(img_255, seg_conf))
out_img = np.squeeze(out_img)

mpimg.imsave(outputFile, out_img)