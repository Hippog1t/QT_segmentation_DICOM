# -*- coding: utf-8 -*-
"""
Created on Fri May  3 20:00:30 2019

@author: alexa
"""

import SimpleITK as sitk
import sys

#file = "./DATA/IM-0001-0182-0001.dcm"
#path = "./Out"

file = sys.argv[1]
path = sys.argv[2]

dcmkeys = [["0010|0010", "Patient Name"],
           ["0010|0020", "Patient ID"],
           ["0010|0030", "Patient Birth Date"],
           ["0008|0020", "Study Date"],
           ["0008|0030", "Study Time"],
           ["0008|0060", "Modality"],
           ["0008|0070", "Manufacturer"],
           ["0018|1030", "Protocol Name"],
           ["0008|0080", "Institution Name"],
           ["0008|1050", "Performing Physician Name"],
           ["0008|1030", "Study Description"],
           ["0008|103e", "SeriesDescription"],
           ]

img = sitk.ReadImage(file)

f= open(path+"/Out/dicomheader.txt","w+")

for k in img.GetMetaDataKeys():
    v = img.GetMetaData(k)
    for i in dcmkeys:
        if(k == i[0] and v!=""):
            f.write(i[1]+"\n"+v+"\n")

f.close()