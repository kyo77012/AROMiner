//============================================================================
// Name        : Hog_PLUG.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

# include <iostream>
# include <fstream>
# include <chrono>
# include <thread>
# include <vector>
# include <string>


# include <stdlib.h>
# include <time.h>
# include <math.h>


# include <windows.h>
# include <dinput.h>


# include <opencv2/core/core.hpp>
# include <opencv2/highgui/highgui.hpp>
# include <opencv2/opencv.hpp>


# define BACKGROUND 5


using namespace std;
using namespace cv;



bool TRAIN = false;

IplImage * trainPic = cvCreateImage( cvSize( 28, 28 ), 8, 3 );
HOGDescriptor hog( cvSize( 28, 28 ), cvSize( 14, 14 ), cvSize( 7, 7 ),cvSize( 7, 7 ), 9 );

CvSVM svm;
CvTermCriteria criteria = TermCriteria( CV_TERMCRIT_EPS, 1000, FLT_EPSILON );
CvSVMParams param = CvSVMParams( CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL, criteria );


bool TrainHog() {
  string ImgName;
  ifstream finSVM;
  finSVM.open( "C:\\Users\\kyo\\Desktop\\hog\\SVM_DATA.txt" );
  if ( finSVM.bad() ) {
    cout << "finPos bad." << endl;
    return false;
  } // if

  vector<string> all_pic_path;
  vector<int> all_pic_ctag;

  string pic_path;
  int pic_ctage;
  while( !finSVM.eof() ) {
    finSVM >> pic_path >> pic_ctage;

    all_pic_path.push_back( pic_path );
    all_pic_ctag.push_back( pic_ctage );
    cout << pic_path << " " << pic_ctage << endl;
  } // while

  finSVM.close();
  CvMat * data_mat, * res_mat;
  int posNum = all_pic_path.size();
  data_mat = cvCreateMat( posNum, 324, CV_32FC1 );
  cvSetZero( data_mat );
  res_mat = cvCreateMat( posNum, 1, CV_32FC1 );
  cvSetZero( res_mat );

  IplImage * srcPic;
  vector<float> descriptors;
  for ( int i = 0 ; i < all_pic_path.size() ; i++ ) {
    descriptors.clear();
    srcPic = cvLoadImage( all_pic_path[i].c_str(), CV_LOAD_IMAGE_ANYCOLOR );
    if ( srcPic == NULL ) {
      cout << "train stage: " << all_pic_path[i] << " load failed." << endl;
      continue;
    } // if

    cout << "handle " << all_pic_path[i] << endl;

    cvResize( srcPic, trainPic );
    hog.compute( trainPic, descriptors, Size( 1, 1 ), Size( 0, 0 ) );
    cout << "HOG dims:" << descriptors.size() << endl;

    for ( int j = 0 ; j < descriptors.size() ; j++ ) {
      cvmSet( data_mat, i, j, descriptors[j] );
    } // for

    cvmSet( res_mat, i, 0, all_pic_ctag[i] );
    cout << all_pic_path[i] << " done." << endl;

    cvReleaseImage( &srcPic );
  } // for

  cout << "開始訓練SVM分類器" << endl;
  svm.train( data_mat, res_mat, NULL, NULL, param );
  cout << "訓練完成" << endl;
  svm.save( "C:\\Users\\kyo\\Desktop\\data\\hog_data.xml" );

  cvReleaseMat( &data_mat );
  cvReleaseMat( &res_mat );

  return true;
} // TrainHog()


string MINICAP_HOME;
string CAP_MODE;
string CAP_REGION;
string FLAG;
string SAVE_PATH;
string PIC_NAME;

int region_left, region_top, region_right, region_bottom;

string cmd;

string PIC_PATH;
/*
bool ReadMiniCapConfig() {
  string tag, value;
  ifstream ifs;
  ifs.open( "minicap_config.txt", ios::in );
  if ( ifs.fail() ) return false;

  while ( !ifs.eof() && getline( ifs, tag ) ) {
    if ( tag.compare( "MINICAP_HOME" ) == 0 ) {
      getline( ifs, MINICAP_HOME );
    } // if
    else if ( tag.compare( "CAP_MODE" ) == 0 ) {
      getline( ifs, CAP_MODE );
    } // else if
    else if ( tag.compare( "CAP_REGION" ) == 0 ) {
      ifs >> region_left >> region_top >> region_right >> region_bottom;
    } // else if
    else if ( tag.compare( "FLAG" ) == 0 ) {
      getline( ifs, FLAG );
    } // else if
    else if ( tag.compare( "SAVE_PATH" ) == 0 ) {
      getline( ifs, SAVE_PATH );
    } // else if
    else if ( tag.compare( "PIC_NAME" ) == 0 ) {
      getline( ifs, PIC_NAME );
    } // else if
  } // while



  PIC_PATH = SAVE_PATH + "\\" + PIC_NAME;
  cmd = "\"\"" + MINICAP_HOME + "\" " + CAP_MODE + " " +
        to_string( region_left ) + " " + to_string( region_top ) + " " +
        to_string( region_right ) + " " + to_string( region_bottom ) +
        " " + FLAG + " -save \"" +  PIC_PATH + "\"\"";
  cout << cmd << endl;


  ifs.close();
  return true;
} // ReadMiniCapConfig()
*/

/*
bool Snapshot() {
  if ( system( cmd.c_str() ) != 0 ) return false;
  return true;
} // Snapshot()
*/

/*

HWND hDesktopWnd;
HDC hWndDC, hWndCompatibleDC;
BITMAPINFOHEADER bmpInfoHead;
RECT windowSize;
Size bmpSize;
Mat printScreen;
CvRect imgRect;
CvSize imgSize;


void Init_GetScreen() {
  hDesktopWnd = GetDesktopWindow();
  hWndDC = GetDC( hDesktopWnd );
  hWndCompatibleDC = CreateCompatibleDC( hWndDC );
  SetStretchBltMode( hWndCompatibleDC, COLORONCOLOR );

  GetClientRect( hDesktopWnd, &windowSize );


  bmpSize.width = windowSize.right;
  bmpSize.height = windowSize.bottom;

  printScreen.create( bmpSize.height, bmpSize.width, CV_8UC3 );

  bmpInfoHead.biSize = sizeof( BITMAPINFOHEADER );
  bmpInfoHead.biWidth = bmpSize.width;
  bmpInfoHead.biHeight = -bmpSize.height;
  bmpInfoHead.biPlanes = 1;
  bmpInfoHead.biBitCount = 24;
  bmpInfoHead.biCompression = BI_RGB;
  bmpInfoHead.biSizeImage = 0;
  bmpInfoHead.biXPelsPerMeter = 0;
  bmpInfoHead.biYPelsPerMeter = 0;
  bmpInfoHead.biClrUsed = 0;
  bmpInfoHead.biClrImportant = 0;

  imgRect.x = region_left;
  imgRect.y = region_top;
  imgRect.width = region_right - region_left;
  imgRect.height = region_bottom - region_top;

  imgSize.width = imgRect.width;
  imgSize.height = imgRect.height;
} // Init_GetScreen()


IplImage * GetScreen() {
  HBITMAP hbWnd = CreateCompatibleBitmap( hWndDC, windowSize.right, windowSize.bottom );

  SelectObject( hWndCompatibleDC, hbWnd );
  StretchBlt( hWndCompatibleDC, 0, 0, bmpSize.width, bmpSize.height,
              hWndDC, 0, 0, windowSize.right, windowSize.bottom, SRCCOPY );
  GetDIBits( hWndCompatibleDC, hbWnd, 0, bmpSize.height, printScreen.data,
             ( BITMAPINFO * ) &bmpInfoHead, DIB_RGB_COLORS );
  DeleteObject( hbWnd );


  IplImage temp( printScreen );
  IplImage * img = cvCreateImage( imgSize, temp.depth, temp.nChannels );
  cvSetImageROI( &temp, imgRect );
  cvCopy( &temp, img );

  return img;
} // GetScreen()

*/



HWND AROWnd;
HDC AROWndDC, AROWndCompatibleDC;
BITMAPINFOHEADER ARObmpInfoHead;
RECT AROwindowSize;
Size ARObmpSize;
Mat AROprintScreen;
CvRect AROimgRect;
CvSize AROimgSize;


bool Init_GetAROScreen() {
  wchar_t windowName[512];
  GetWindowTextW( GetForegroundWindow(), windowName, 512 );
  AROWnd = FindWindow( NULL, "Astral Realm Online                     " );
  if ( AROWnd == NULL ) {
    cout << "ARO is not running!" << endl;
    return false;
  } // if
  AROWndDC = GetDC( AROWnd );
  AROWndCompatibleDC = CreateCompatibleDC( AROWndDC );
  SetStretchBltMode( AROWndCompatibleDC, COLORONCOLOR );


  ShowWindow( AROWnd, SW_SHOWNORMAL );
  GetClientRect( AROWnd, &AROwindowSize );

  ARObmpSize.width = AROwindowSize.right;
  ARObmpSize.height = AROwindowSize.bottom;

  cout << "ARObmpSize.width:" << ARObmpSize.width << endl;
  cout << "ARObmpSize.height:" << ARObmpSize.height << endl;

  AROprintScreen.create( ARObmpSize.height, ARObmpSize.width, CV_8UC3 );

  ARObmpInfoHead.biSize = sizeof( BITMAPINFOHEADER );
  ARObmpInfoHead.biWidth = ARObmpSize.width;
  ARObmpInfoHead.biHeight = -ARObmpSize.height;
  ARObmpInfoHead.biPlanes = 1;
  ARObmpInfoHead.biBitCount = 24;
  ARObmpInfoHead.biCompression = BI_RGB;
  ARObmpInfoHead.biSizeImage = 0;
  ARObmpInfoHead.biXPelsPerMeter = 0;
  ARObmpInfoHead.biYPelsPerMeter = 0;
  ARObmpInfoHead.biClrUsed = 0;
  ARObmpInfoHead.biClrImportant = 0;

  AROimgRect.x = ARObmpSize.width / 6;
  AROimgRect.y = ARObmpSize.height / 4;
  AROimgRect.width = ARObmpSize.width / 6 * 4;
  AROimgRect.height = ARObmpSize.height / 2;

  AROimgSize.width = AROimgRect.width;
  AROimgSize.height = AROimgRect.height;
  return true;
} // Init_GetAROScreen()


IplImage * GetAROScreen() {
  HBITMAP ARObWnd = CreateCompatibleBitmap( AROWndDC, AROwindowSize.right, AROwindowSize.bottom );
  ShowWindow( AROWnd, SW_SHOWNORMAL );
  SelectObject( AROWndCompatibleDC, ARObWnd );
  StretchBlt( AROWndCompatibleDC, 0, 0, ARObmpSize.width, ARObmpSize.height,
              AROWndDC, 0, 0, AROwindowSize.right, AROwindowSize.bottom, SRCCOPY );
  GetDIBits( AROWndCompatibleDC, ARObWnd, 0, ARObmpSize.height, AROprintScreen.data,
             ( BITMAPINFO * ) &ARObmpInfoHead, DIB_RGB_COLORS );
  DeleteObject( ARObWnd );


  IplImage temp( AROprintScreen );
  IplImage * img = cvCreateImage( AROimgSize, temp.depth, temp.nChannels );
  cvSetImageROI( &temp, AROimgRect );
  cvCopy( &temp, img );
  return img;
} // GetAROScreen()





INPUT v_keyboard;

void v_keyboard_init() {
  v_keyboard.type = INPUT_KEYBOARD;
  v_keyboard.ki.wScan = 0;
  v_keyboard.ki.time = 0;
  v_keyboard.ki.dwExtraInfo = 123;
} // v_keyboard_init()

void SendInput_keydown( int vk_keyCode ) {
  // v_keyboard.ki.wVk = vk_keyCode;
  v_keyboard.ki.wScan = vk_keyCode;
  v_keyboard.ki.dwFlags = KEYEVENTF_SCANCODE;
  SendInput( 1, &v_keyboard, sizeof( INPUT ) );
} // SendInput_keydown()

void SendInput_keyup( int vk_keyCode ) {
  // v_keyboard.ki.wVk = vk_keyCode;
  v_keyboard.ki.wScan = vk_keyCode;
  v_keyboard.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
  SendInput( 1, &v_keyboard, sizeof( INPUT ) );
} // SendInput_keyup()



CvMemStorage * storage = cvCreateMemStorage( 0 );
CascadeClassifier cascade;
vector<Rect> numbers;
IplImage * img;
Rect tempRect;

bool detectMultiScale_done;

IplImage * test;
vector<string> all_test_path;
vector<float> descriptors;
CvMat * SVMtrainMat;
CvRect test_area;
int result;
vector<int> resultSet;



void detectMultiScale_with_timeout() {
  cascade.detectMultiScale( img, numbers, 1.1, 1, CASCADE_SCALE_IMAGE, Size( 28, 28 ), Size( 84, 84 ) );
  detectMultiScale_done = true;
} // detectMultiScale_with_timeout()

int main() {
  /*
  char windowName[512];
  while( true ) {
    GetWindowText( GetForegroundWindow(), windowName, sizeof( windowName ) );
    cout << windowName << endl;
    Sleep( 350 );
  }
  */

  /*
  if ( !ReadMiniCapConfig() ) {
    cout << "ReadMiniCapConfig failed." << endl;
    return 0;
  } // if
  */
  if ( !Init_GetAROScreen() ) {
    cout << "Init_GetAROScreen failed!" << endl;
    return 0;
  } // if
  // Init_GetScreen();

  /*
  IplImage * test1 = GetScreen();
  Sleep( 1000 );
  namedWindow( "MyWindow", CV_WINDOW_AUTOSIZE );
  cvShowImage( "MyWindow", test1 );
  waitKey( 0 );
  destroyWindow( "MyWindow" );

  printScreen.release();
  DeleteDC( hWndDC );
  ReleaseDC( hDesktopWnd, hWndDC );
  return 0;
  */

  v_keyboard_init();

  if ( TRAIN ) {
    if ( !TrainHog() ) return 0;
  } // if

  svm.load( "hog_data.xml" );

  String cascade_name = "cascade.xml";
  cascade.load( cascade_name.c_str() );

  test_area.width = 28;
  test_area.height = 28;


  /*
  HWND aro = FindWindow( NULL, "Astral Realm Online                        " );
  if ( aro == NULL ) {
    cout << "Astral Realm Online not found." << endl;
    return 0;
  } // if
  else {
    Init_GetAROScreen();
    IplImage * AROimg = GetAROScreen();
    namedWindow( "test", CV_WINDOW_AUTOSIZE );
    cvShowImage( "test", AROimg );
    waitKey( 0 );
    destroyWindow( "test" );
    return 0;
  } // else
  */

  time_t begin_time;
  time_t end_time;
  srand( time( NULL ) );
  int pressTime;

  HWND tempWND;

  cout << "set press time(ms):";
  cin >> pressTime;
  while ( true ) {
    Sleep( 350 );
    /*
    if ( !Snapshot() ) {
      cout << "Snapshot failed." << endl;
      continue;
    } // if
    */

    begin_time = time( NULL );

    // img = cvLoadImage( PIC_PATH.c_str(), CV_LOAD_IMAGE_ANYCOLOR );
    // img = GetScreen();
    img = GetAROScreen();

    if ( img == NULL ) {
      cout << PIC_PATH.c_str() << " image load failed." << endl;
      continue;
    } // if

    numbers.clear();
    cascade.detectMultiScale( img, numbers, 1.1, 3, CASCADE_SCALE_IMAGE, Size( 28, 28 ), Size( 84, 84 ) );
    // cascade.detectMultiScale( img, numbers, 1.1, 60, CASCADE_SCALE_IMAGE, Size( 28, 28 ), Size( 84, 84 ) );
    for ( int i = 0 ; i < numbers.size() - 1 ; i++ ) {
      for ( int j = i + 1 ; j < numbers.size() ; j++ ) {
        if ( numbers[i].x > numbers[j].x ) {
          tempRect = numbers[i];
          numbers[i] = numbers[j];
          numbers[j] = tempRect;
        } // if
      } // for
    } // for

    end_time = time( NULL );
    cout << "haar time consumed: " << end_time - begin_time << endl;

    if ( numbers.size() <= 0 ) {
      cout << "none" << endl;
      continue;
    } // if
    else {
      cout << "total number: " << numbers.size() << endl;
      resultSet.clear();
      for ( int i = 0 ; i < numbers.size() ; i++ ) {
        descriptors.clear();

        test_area.x = numbers[i].x + ( numbers[i].width - test_area.width ) * 0.6;
        test_area.y = numbers[i].y + ( numbers[i].height - test_area.height ) * 0.8;

        cvSetImageROI( img, test_area );
        test = cvCreateImage( cvSize( test_area.width, test_area.height ), img->depth, img->nChannels );
        cvCopy( img, test );

        hog.compute( test, descriptors, Size( 1, 1 ), Size( 0, 0 ) );
        SVMtrainMat = cvCreateMat( 1, descriptors.size(), CV_32FC1 );
        for ( int j = 0 ; j < descriptors.size() ; j++ ) {
          cvmSet( SVMtrainMat, 0, j, descriptors[j] );
        } // for

        result = svm.predict( SVMtrainMat );
        if ( result != BACKGROUND ) {
          resultSet.push_back( result );
          cout << "number " << i + 1 << ": " << result << endl;
        } // if
        else cout << "number " << i + 1 << ": BACKGROUND" << endl;
        // cout << "number " << i + 1 << ": " << result << endl;

        cvReleaseMat( &SVMtrainMat );
        cvReleaseImage( &test );
        cvResetImageROI( img );
      } // for


      if ( resultSet.size() == 0 ) {
        cout << "hog + svm: no number found." << endl;
      } // if
      else {
        cout << "hog + svm: " ;
        for ( int i = 0 ; i < resultSet.size() ; i++ ) {
          cout << resultSet[i];
        } // for
        cout << endl;

        for ( int i = 0 ; i < resultSet.size() ; i++ ) {
          Sleep( 30 );
          pressTime = ( rand() % 25 ) + pressTime;
          tempWND = GetActiveWindow();
          SetActiveWindow( AROWnd );
          if ( resultSet[i] == 1 ) {
            SendInput_keydown( DIK_1 );
            Sleep( pressTime );
            SendInput_keyup( DIK_1 );
          } // if
          else if ( resultSet[i] == 2 ) {
            SendInput_keydown( DIK_2 );
            Sleep( pressTime );
            SendInput_keyup( DIK_2 );
          } // else if
          else if ( resultSet[i] == 3 ) {
            SendInput_keydown( DIK_3 );
            Sleep( pressTime );
            SendInput_keyup( DIK_3 );
          } // else if
          else if ( resultSet[i] == 4 ) {
            SendInput_keydown( DIK_4 );
            Sleep( pressTime );
            SendInput_keyup( DIK_4 );
          } // else if
          SetActiveWindow( tempWND );
        } // for
      } // else
    } // else

    cvReleaseImage( &img );

    end_time = time( NULL );
    cout << "haar + hog + svm time consumed: " << end_time - begin_time << endl;
    cout << "-----------------------------------" << endl;
  } // while


  /*
  printScreen.release();
  DeleteDC( hWndDC );
  ReleaseDC( hDesktopWnd, hWndDC );
  */
  AROprintScreen.release();
  DeleteDC( AROWndDC );
  DeleteDC( AROWndCompatibleDC );
  ReleaseDC( AROWnd, AROWndDC );
	return 0;
} // main()
