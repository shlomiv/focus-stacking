#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

/// Global variables


void getFocusedBlobs(InputArray image, OutputArray focus_map, int kernel_size) {

  int const max_lowThreshold = 100;
  int lowThreshold = 35;
  int ratio = 3;
  int erosion_size=45;

  Mat gray_image;
  cvtColor(image , gray_image, CV_BGR2GRAY);
  
  Mat detected_edges;
  /// Reduce noise with a kernel 3x3
  blur( gray_image, detected_edges, Size(1,1) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  Mat element = getStructuringElement(MORPH_RECT,
                                      Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                      Point( erosion_size, erosion_size ) );

  
  dilate(detected_edges, detected_edges, element);

 

  if (focus_map.rows() > 0) {
      cout << focus_map.size() << endl;
       Mat gray_focus;
       cvtColor(focus_map , gray_focus, CV_BGR2GRAY);

       Mat r = (gray_focus == 0)/254;
       Mat dst = Mat::zeros(detected_edges.rows, detected_edges.cols, detected_edges.type());
       
       detected_edges.copyTo(dst, r);
       //cout << dst << endl;
       Mat th3d;
       cvtColor(dst,th3d,CV_GRAY2BGR,3);

       image.copyTo(focus_map, dst);

  }else {
  
    image.copyTo(focus_map, detected_edges);}
} 

/**
 * @function main
 */
int main( int argc, char** argv )
{
  
  cout << "Starting..." <<  argc <<endl;

  Mat image = imread(argv[1]);;
  Mat focused;// = image.clone();

  if( !image.data )
      { printf(" No data! -- Exiting the program \n");
        exit(-1);}

  int kernel_size = 3;
  getFocusedBlobs(image, focused, kernel_size);
  
  for(int i=2;i<argc;i++){
    
    Mat image = imread(argv[i]);;

    if( !image.data )
      { printf(" No data! -- Exiting the program \n");
        exit(-1);}
    cout << "yaa" << endl;
    getFocusedBlobs(image, focused, kernel_size);
  }
  
  //  cout << "Done create focus maps, merging" << endl;


  // cout << "Done merging!" << endl;
  
  namedWindow( "Focused", WINDOW_OPENGL|WINDOW_NORMAL );// Create a window for display.
  imshow( "Focused", focused );                   

  waitKey(0);
  return 0;
 
}

