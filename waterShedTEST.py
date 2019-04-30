# -*- coding: utf-8 -*-
"""
Created on Tue Apr 30 19:34:00 2019

@author: alexa
"""

import SimpleITK as sitk
import matplotlib.image as mpimg
import numpy as np

from myshow import myshow

inputFile = "DATA/IM-0001-0180-0001.dcm"

img = sitk.ReadImage(inputFile)
img_255 = sitk.Cast(sitk.RescaleIntensity(img), sitk.sitkUInt8)
img_255 = sitk.GetArrayFromImage(img_255)
img_255 = np.squeeze(img_255)

mpimg.imsave("test.jpg", np.squeeze(sitk.GetArrayFromImage(img)), cmap="gray")

rgb_img = sitk.ReadImage("test.jpg")
myshow(rgb_img, "bob")
img = sitk.VectorIndexSelectionCast(rgb_img,1)
myshow(img, "TesT")

feature_img = sitk.GradientMagnitudeRecursiveGaussian(img, sigma=1.5)
myshow(feature_img)

ws_img = sitk.MorphologicalWatershed(feature_img, level=4, markWatershedLine=False, fullyConnected=False)
myshow(sitk.LabelToRGB(ws_img), "Watershed Over Segmentation")
#myshow(sitk.LabelOverlay(img_255, ws_img))

"""
seg = sitk.ConnectedComponent(ws_img!=ws_img[256,256])
myshow(sitk.LabelOverlay(img, seg), "Foreground Components")

filled = sitk.BinaryFillhole(seg!=0)
d = sitk.SignedMaurerDistanceMap(filled, insideIsPositive=False, squaredDistance=False, useImageSpacing=False)
myshow(d, "Inside Distance Map")

ws = sitk.MorphologicalWatershed( d, markWatershedLine=False, level=1)
myshow(sitk.LabelOverlay(img, ws))

seg = ws
radius=10
bd_img = sitk.BinaryDilate(seg!=0, radius)
myshow(bd_img, "Binary Dilate")

dist_img = sitk.SignedMaurerDistanceMap(seg!=0, insideIsPositive=False, squaredDistance=False, useImageSpacing=False)
wsd_img = sitk.MorphologicalWatershedFromMarkers(dist_img, seg, markWatershedLine=False)
myshow(sitk.LabelOverlay(img,wsd_img))

md_img = sitk.Mask(wsd_img,bd_img)
myshow(sitk.LabelToRGB(md_img), "Multi-label Dilate")

e_img=sitk.BinaryErode(md_img!=0, radius)
mo_img=sitk.Mask(md_img, e_img)
myshow(sitk.LabelOverlay(img, mo_img), "Multi-label Closing")
"""