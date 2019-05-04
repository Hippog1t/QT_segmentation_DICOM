# -*- coding: utf-8 -*-
"""
Created on Fri May  3 20:00:30 2019

@author: alexa
"""

import SimpleITK as sitk

file = "./DATA/IM-0001-0182-0001.dcm"

dcmkeys = [["0010|0010", "Patient Name"],
           ["0010|0020", "Patient ID"],
           ["0010|0030", "Patient Birth Date"],
           ["0008|0020", "Study Date"],
           ["0008|0030", "Study Time"],
           ["0008|0060", "Modality"]
        ]

reader = sitk.ImageFileReader()
reader.SetFileName(file)
reader.LoadPrivateTagsOn();

reader.ReadImageInformation();
f= open("dicomhead.txt","w+")

for k in reader.GetMetaDataKeys():
    print(reader.GetMetaDataKeys())
    v = reader.GetMetaData(k)
    f.write(k+" = "+v+"\n")

print("Image Size: {0}".format(reader.GetSize()));
print("Image PixelType: {0}".format(sitk.GetPixelIDValueAsString(reader.GetPixelID())));

f.close()

"""
0018|1030 5.69 COEUR SEGMENT  66-74 BPM    IMC < 25 
0018|0022 HELICAL MODE
0008|0060 CT
0008|0070 GE MEDICAL SYSTEMS
0008|0080 G.I.E SCANNER CREIL 
0008|0090 DAVID^JEAN-MARIE
0008|1010 ct01
0008|1030 ANGIO-SCANNER CORONAIRE 
0008|103e Coeur Segment + 
0008|1050 NIKKHOU^SHAHIN

tags_to_copy = ["0010|0010", # Patient Name
   69                 "0010|0020", # Patient ID
   70                 "0010|0030", # Patient Birth Date
   71                 "0020|000D", # Study Instance UID, for machine consumption
   72                 "0020|0010", # Study ID, for human consumption
   73                 "0008|0020", # Study Date
   74                 "0008|0030", # Study Time
   75                 "0008|0050", # Accession Number
   76                 "0008|0060"  # Modality
   77 ]

"""