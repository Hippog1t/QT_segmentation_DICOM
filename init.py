# -*- coding: utf-8 -*-
"""
Created on Mon Apr 29 17:14:07 2019

@author: Alexandre
"""

import SimpleITK as sitk
import matplotlib.image as mpimg
import numpy as np
import sys, os

In = sys.argv[1]
pathOut = sys.argv[2]

if(os.path.isfile(In)):
    if(In.lower().endswith('.dcm')):
        img = sitk.ReadImage(In)
        img_255 = sitk.Cast(sitk.RescaleIntensity(img), sitk.sitkUInt8)
        out_img = sitk.GetArrayFromImage(img_255)
        out_img = np.squeeze(out_img)
        mpimg.imsave(pathOut+"/Out/initial0.jpg", out_img, cmap="gray")
    elif(In.lower().endswith('.jpg', '.png')):
        mpimg.imsave(pathOut+"/Out/initial0.jpg", In, cmap="gray")

elif(os.path.isdir(In)):
    files = os.listdir(In)
    files.sort(key = lambda s: len(s))
    f= open(pathOut+"/Out/directory.txt","w+")
    for i in files:
        if(i.lower().endswith('.dcm')):
            f.write(i+"\n")
    print(files)
    f.close()
       
    i = 0
    for file in files :
        if(file.lower().endswith('.dcm')):
            #print(file)
            img = sitk.ReadImage(In+"/"+file)
            img_255 = sitk.Cast(sitk.RescaleIntensity(img), sitk.sitkUInt8)
            out_img = sitk.GetArrayFromImage(img_255)
            out_img = np.squeeze(out_img)
            mpimg.imsave(pathOut+"/Out/initial"+str(i)+".jpg", out_img, cmap="gray")
            i = i+1