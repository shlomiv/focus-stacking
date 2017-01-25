#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

/// Global variables


void calcFocusMLAP(InputArray image, OutputArray focus_map, int window_size) {
  
  Mat gray_image;
  cvtColor(image , gray_image, CV_BGR2GRAY);

  //M = [-1 2 -1];        
  //Lx = imfilter(Image, M, 'replicate', 'conv');
  //Ly = imfilter(Image, M', 'replicate', 'conv');
  //FM = abs(Lx) + abs(Ly);
  //FM = mean2(FM);

  Mat C = (Mat_<double>(1,3) << -1.0,2.0,-1.0);
  Mat Lx,Ly;
  int ddepth=CV_32F,delta=0;


  filter2D(gray_image, Lx, ddepth, C,     Point(-1,-1),delta,BORDER_REPLICATE);
  filter2D(gray_image, Ly, ddepth, C.t(), Point(-1,-1),delta,BORDER_REPLICATE);

  Mat l = abs(Lx)+abs(Ly);
  
  // calculate mean

  //blur(l, focus_map, Size(window_size, window_size), Point(-1,-1), BORDER_REPLICATE);
  GaussianBlur(l, focus_map, Size(window_size, window_size), 0.1,0, BORDER_REPLICATE);
} 

/**
 * @function main
 */
int main( int argc, char** argv )
{

  vector<Mat> focusMaps;
  vector<Mat> images;
  
  cout << "Starting..." << endl;

  Mat image = imread(argv[1]);;
  Mat focused = image.clone();

  if( !image.data )
      { printf(" No data! -- Exiting the program \n");
        exit(-1);}

  Mat mlap_max;
  int window_size = 1;
  calcFocusMLAP(image, mlap_max, window_size);
  
  for(int i=2;i<argc-1;i++){
    
    Mat image = imread(argv[i]);;

    if( !image.data )
      { printf(" No data! -- Exiting the program \n");
        exit(-1);}

    Mat mlap;
    calcFocusMLAP(image, mlap, window_size);

    image.copyTo(focused, mlap_max>mlap);
    mlap.copyTo(mlap_max, mlap_max>mlap);
    
  }

  //  cout << "Done create focus maps, merging" << endl;


  // cout << "Done merging!" << endl;
  
  namedWindow( "Focused", WINDOW_OPENGL|WINDOW_NORMAL );// Create a window for display.
  imshow( "Focused", focused );                   

  waitKey(0);
  return 0;
 
}

// would love to use it as a function, but cant seem to make it work! :/

/*void reconstruct_focus(vector<InputArray> images, vector<InputArray> focusMaps, int stack_size, OutputArray focused) {
  int rows = images[0].rows;
  int cols = images[0].cols;

  for (int j=0;j<cols;j++) {
    for (int i=0;i<rows;i++) {
      // find most focused pixel
      int most_focused_layer=0;
      float max_focus = 0;
      
      for (int k=0; k<stack_size; k++) {
        float curr_focus = focusMaps[k].at<float>(i,j);
        if (curr_focus > max_focus) {
          max_focus=curr_focus;
          most_focused_layer = k;
        }
      }

      focused.at<Vec3b>(i,j)=images[most_focused_layer].at<Vec3b>(i,j);
    }
  }
}

/*
  int stack_size = argc-1;
  int rows = images[0].rows;
  int cols = images[0].cols;
  Mat focused = images[0].clone();

  for (int j=0;j<cols;j++) {
    for (int i=0;i<rows;i++) {
      // find most focused pixel
      int most_focused_layer=0;
      float max_focus = 0;
      
      for (int k=0; k<stack_size; k++) {
        float curr_focus = focusMaps[k].at<float>(i,j);
        if (curr_focus > max_focus) {
          max_focus=curr_focus;
          most_focused_layer = k;
        }
      }
      focused.at<Vec3b>(i,j)=images[most_focused_layer].at<Vec3b>(i,j);
    }
  }
*/
