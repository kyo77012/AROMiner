//============================================================================
// Name        : GetPicSize.cpp
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


int main( int argc, char * argv[] ) {
  if ( argc < 2 ) return 0;
  string outputName( argv[1] );
  system( "dir /b *.bmp *.jpg > picNames.txt" );
  ifstream picNames_in;
  ofstream output_out;

  picNames_in.open( "picNames.txt", ios::in );
  if ( !picNames_in.is_open() ) {
    cerr << "picNames open failed." << endl;
    return 0;
  } // if

  output_out.open( outputName.c_str(), ios::out );
  if ( !picNames_in.is_open() ) {
    cerr << "output open failed." << endl;
    return 0;
  } // if

  string picName;
  int i = 1, j;
  string newPicName;
  string subname;
  string ren_cmd;
  while ( !picNames_in.eof() && getline( picNames_in, picName ) ) {
    IplImage * img = cvLoadImage( picName.c_str(), CV_LOAD_IMAGE_ANYCOLOR );
    if ( img != NULL ) {
      newPicName = argv[2] + to_string( i );
      for ( j = picName.length() - 1 ; j >= 0 ; j -- ) {
        if ( picName.at( j ) == '.' ) break;
      } // for

      if ( j >= 0 ) {
        for ( ; j < picName.length() ; j++ )
          newPicName += picName.at( j );
      } // if
      ren_cmd = "ren \"" + picName + "\" \"" + newPicName + "\"";
      output_out << newPicName << " 1 0 0 " << img->width << " " << img->height << endl;
      system( ren_cmd.c_str() );
      img->~IplImage();
      i++;
    } // if
  } // while

  picNames_in.close();
  output_out.close();

  return 0;
} // main()
