//============================================================================
// Name        : PLUG.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

# include <windows.h>

# include <opencv2/core/core.hpp>
# include <opencv2/highgui/highgui.hpp>
# include <opencv2/opencv.hpp>

# include <stdlib.h>
# include <iostream>
# include <fstream>
# include <chrono>
# include <thread>
# include <vector>
# include <string>

using namespace std;
using namespace cv;

int main() {
  IplImage * img = cvLoadImage( "C:\\Users\\kyo\\Desktop\\131.jpg", CV_LOAD_IMAGE_ANYCOLOR );
  String cascade_name = "C:\\Users\\kyo\\Desktop\\data4\\data\\cascade.xml";
  // String cascade_name = "C:\\Users\\kyo\\Desktop\\haar\\data1\\data.xml";
  CvMemStorage * storage = cvCreateMemStorage( 0 );


  CascadeClassifier cascade;
  cascade.load( cascade_name.c_str() );
  vector<Rect> numbers;
  cascade.detectMultiScale( img, numbers, 1.1, 3, CASCADE_SCALE_IMAGE, Size( 28, 28 ), Size( 84, 84 ) );
  for ( int i = 0 ; i < numbers.size() ; i++ ) {
    Rect * number_rect = &numbers[i];
    cvRectangle( img,
                 cvPoint( number_rect->x, number_rect->y ),
                 cvPoint( ( number_rect->x + number_rect->width ),
                          ( number_rect->y + number_rect->height ) ),
                 CV_RGB( 255, 0, 0 ),
                 3 );
    cout << number_rect->width << " " << number_rect->height << endl;
  } // for


  /*
  CvHaarClassifierCascade * cascade = ( CvHaarClassifierCascade * ) cvLoad( cascade_name.c_str() );
  CvSeq * numbers = cvHaarDetectObjects( img, cascade, storage, 1.1, 138, 0, cvSize( 28, 28 ) );
  for ( int i = 0 ; i < numbers.size() ; i++ ) {
    CvRect * number_rect = ( CvRect * ) cvGetSeqElem( numbers, i );
    cvRectangle( img,
    		         cvPoint( number_rect->x, number_rect->y ),
                 cvPoint( ( number_rect->x + number_rect->width ),
                          ( number_rect->y + number_rect->height ) ),
                 CV_RGB( 255, 0, 0 ),
                 3 );
  } // for
  */
  namedWindow( "MyWindow", CV_WINDOW_AUTOSIZE );
  cvShowImage( "MyWindow", img );
  waitKey( 0 );
  destroyWindow( "MyWindow" );
  return 0;
} // main()
