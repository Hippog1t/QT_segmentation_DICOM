# -*- coding: utf-8 -*-
"""
Created on Tue Apr 30 21:28:43 2019

@author: alexa
"""

import SimpleITK as sitk
import matplotlib.image as mpimg
import numpy as np
import sys

inputFile = sys.argv[1]
outFile = sys.argv[2]+"/Out/waterShed.jpg"

img = sitk.ReadImage(inputFile)

mpimg.imsave("temp_lpe.jpg", np.squeeze(sitk.GetArrayFromImage(img)), cmap="gray")

temp_img = sitk.ReadImage("temp_lpe.jpg")

img = sitk.VectorIndexSelectionCast(temp_img,1)

feature_img = sitk.GradientMagnitudeRecursiveGaussian(img, sigma=2)

ws_img = sitk.MorphologicalWatershed(feature_img, level=4, markWatershedLine=False, fullyConnected=False)

seg = sitk.ConnectedComponent(ws_img!=ws_img[256,256])
final_img = sitk.LabelOverlay(img, seg)
mpimg.imsave(outFile, np.squeeze(sitk.GetArrayFromImage(final_img)))