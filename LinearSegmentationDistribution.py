# -*- coding: utf-8 -*-

import SimpleITK as sitk
import matplotlib.image as mpimg
import numpy as np
import sys
import pydicom as dicom

inputFile = sys.argv[1]
outFile = sys.argv[2]+"/Out/lsd.jpg"
_colormap = sys.argv[3]


img = sitk.ReadImage(inputFile)

img = sitk.Cast(sitk.RescaleIntensity(img), sitk.sitkUInt8)

mpimg.imsave(outFile, np.squeeze(sitk.GetArrayFromImage(img)), cmap=_colormap)

"""

import SimpleITK as sitk
import matplotlib.image as mpimg
import numpy as np
import sys
import pydicom as dicom

inputFile = "C:/Users/xX_war_machine_Xx/Desktop/dicom_pics/MANON/SE_1/IM000126.dcm"
outFile = "C:/Users/xX_war_machine_Xx/Desktop/QT_segmentation_DICOM/Out/LSD.jpg"
img = sitk.ReadImage(inputFile)

img = sitk.Cast(sitk.RescaleIntensity(img), sitk.sitkUInt8)

mpimg.imsave("temp_lsd.jpg", np.squeeze(sitk.GetArrayFromImage(img)), cmap="seismic")

temp_img = sitk.ReadImage("temp_lsd.jpg")

mpimg.imsave(outFile, np.squeeze(sitk.GetArrayFromImage(temp_img)))

"""