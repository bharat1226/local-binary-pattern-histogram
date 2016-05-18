# local-binary-pattern-histogram

Local Binary Pattern (LBP) is a simple yet very efficient texture operator which labels the pixels of an image by thresholding the neighborhood of each pixel and considers the result as a binary number.

This project will implement to:

* get all image filenames from a directory  
* put all filenames into a list  
* sort list of filenames  
* for each filename
 * loads the image
 * computes the Local Binary Pattern (LBP) label for each pixel
 * computes the LBP histogram for the entire image
 * appends the filename and histogram to a file “HISTOGRAMS.txt” (located in the output directory)
*reloads the data in the “HISTOGRAMS.txt” file
* for each histogram
 * finds the histogram for the filename “droid_1.bmp”
 * gets the histogram distance of every histogram from the histogram for “droid_1.bmp”
 * output a file “DISTANCES.txt” that contains the filenames and the histogram distances
 
When your program is complete, there will be the following files in the output directory:  
* “HISTOGRAMS.txt” (ONE file for whole directory)
* “DISTANCES.txt” (again, ONE file for the whole directory)
