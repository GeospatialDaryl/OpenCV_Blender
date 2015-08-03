/*
This code implements simple opencv pipeline that demonstrates 
how to read, process and display image using OpenCV library
*/
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

/// Global Variables
const int alpha_slider_max = 150;
const int p2_slider_max = 150;
int alpha_slider;
int p2_slider;
double alpha;
double beta;
double p2;
double p3;

int ps_array[2];

/// Matrices to store images
Mat src;
//Mat src2;
//Mat dst;
UMat imgInp;    // to store input image
UMat imgGray;   // to store intermediate grayscale image
UMat imgOut;    // to store final processing result
UMat imgOut2;

/**
* @function on_trackbar
* @brief Callback for trackbar
*/
void on_trackbar(int, void*)
{
	alpha = (double)alpha_slider / alpha_slider_max;
	beta = (1.0 - alpha);

	//addWeighted(src1, alpha, src2, beta, 0.0, dst);
	addWeighted(imgGray, alpha, imgInp, beta, 0.0, imgOut2);

	imshow("Linear Blend", imgOut2);
}

void on_trackbar_Canny(int, void*)
{
	alpha = (double)alpha_slider ;
	beta = (double)p2_slider;

	// run canny processing on grayscale image
	Canny(imgGray, imgOut2, alpha, beta);

	// show the result on the screen using opencv function
	// this call creates window named "Canny" and draw imgOut inside the window
	imshow("Linear Bleand", imgOut2);

}

int main(int argc, const char** argv)
{
    
        // get next frame from input stream
        //cap >> imgInp;
	    src = imread(argv[1], CV_LOAD_IMAGE_ANYDEPTH);
		src.convertTo(imgInp, CV_8UC1, 0.00390625);
		//src.copyTo(imgInp);

		std::istringstream iss(argv[1]);
		std::vector<std::string> tokens;
		std::string token;
		while (std::getline(iss, token, '.')) {
			if (!token.empty())
				tokens.push_back(token);
		}
			

        // check read result
        // in case of reading from file the loop will be break after last frame is read and processed
        // in case of camera this condition is always false until something wrong with camera 

        // show the input image on the screen using opencv function
        // this call creates window named "Input" and draws imgInp inside the window
        imshow("Input", imgInp);

        // convert input image into intermediate grayscale image
		if (imgInp.channels() > 1)
		{
			  printf("Preceding with blanks: %10d \n", imgInp.channels());
              cvtColor(imgInp, imgGray, COLOR_BGR2GRAY);
			  Mat spl;
			  split(imgInp, spl);
			  //imshow("spl1", spl[0]);//b
			  //imshow("spl2", spl[1]);//g
			 // imshow("spl3", spl[2]);//r

		}
		else
		{
			printf("Preceding with blanks: %10d \n", imgInp.channels());
		 imgGray = imgInp; 
		}
        
        
		/// Initialize values
		alpha_slider = 0;
		p2_slider = 0;
		// run canny processing on grayscale image
        //Canny(imgGray, imgOut, 50, 150);

        // show the result on the screen using opencv function
        // this call creates window named "Canny" and draw imgOut inside the window
        //imshow("Canny", imgOut);
		/// Create Windows
		namedWindow("Linear Blend",WINDOW_NORMAL);

		/// Create Trackbars
		char TrackbarName[50];
		char TrackbarName2[50];
		sprintf(TrackbarName, "Thresh#1 /n x %d", alpha_slider_max);
		sprintf(TrackbarName2, "Thresh#2 /n 2 x %d", alpha_slider_max);
		//createTrackbar(TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar);
		createTrackbar(TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar_Canny);
		createTrackbar(TrackbarName2, "Linear Blend", &p2_slider, p2_slider_max, on_trackbar_Canny);

		ps_array[0] = alpha_slider;
		ps_array[1] = p2_slider;
		for (;;)
		{
			/// Show some stuff
			on_trackbar_Canny(0, 0);
			//on_trackbar(alpha_slider, 0);

			// the waitKey function is called for 2 reasons
			// 1. detect when ESC key is pressed
			// 2. to allow "Input" and "Canny" windows to plumb messages. It allows user to manipulate with "Input" and "Canny" windows
			// 10ms param is passed to spend only 10ms inside the waitKey function and then go to further processing
			int key = waitKey(100);
			//exit if ESC is pressed
		if (key == 27)
			break;
		}
	

    return EXIT_SUCCESS;
}