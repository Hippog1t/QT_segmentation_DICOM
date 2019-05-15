# QT_segmentation_DICOM\n
Dicom image processing using Qt (5.12.3, plus use Mingw 64 v11) interface and python (3.7) in backend\n

Welcome to this project!

If you are reading this with the intention of using or improving the project:

  -first of all good luck. DICOM is a dark place where nobody want's to go.
  
  -this project is mainly commented, it should make things easier but if you have a problem feel free to contact me. 
  
  -You WILL NEED DICOM FILES with the .DCM windows extention. Some of them are lacking the '.dcm' sometimes, run 
  ```
    ren \*.\* \*.dcm 
  ``` 
    in the command prompt in the folder, it should do the trick.
    
    There should be an example for you to load in the Ressources files
    
  -You also need to install SimpleITK, matplotlib, pillow with your python version 
  
  ```
  pip install *stuff*
  ```
  
  or 
  ```
  conda install *stuff*
  ```
  should work on your python command prompt.\n
