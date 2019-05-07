# -*- coding: utf-8 -*-
"""
Created on Tue May  7 17:44:12 2019

@author: alexa
"""

import SimpleITK as sitk
import matplotlib.image as mpimg
import numpy as np
import sys, os

inputdir = "C:/Users/alexa/Documents/Projet_Segmentation/CodingZone/DATA"
output = "C:/Users/alexa/Documents/Projet_Segmentation/CodingZone/Out"

if(os.path.isdir(inputdir)):
    files = os.listdir(inputdir)
    i = 0
    for file in files :
        if(file.lower().endswith('.dcm')):
            print(file)
            img = sitk.ReadImage(inputdir+"/"+file)
            img_255 = sitk.Cast(sitk.RescaleIntensity(img), sitk.sitkUInt8)
            out_img = sitk.GetArrayFromImage(img_255)
            out_img = np.squeeze(out_img)
            mpimg.imsave(output+"/initial"+str(i)+".jpg", out_img, cmap="gray")
            i = i+1