#include <ros/ros.h>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <image_transport/image_transport.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/Int32.h>



using namespace std;
using namespace cv;

class Edge_Detector
{
   	ros::NodeHandle nh1_;
  	image_transport::ImageTransport it1_;
  	image_transport::Subscriber image_sub_;
  		
public:
	//Constructor por defecto de la clase con lista de constructores
  	Edge_Detector() : it1_(nh1_){

    image_sub_ = it1_.subscribe("/camera/rgb/image_raw", 1, &Edge_Detector::imageCb, this);///bebop/image_raw
	//image_sub_ = it1_.subscribe("/usb_cam/image_raw", 1, &Edge_Detector::imageCb, this);
    }

  	void imageCb(const sensor_msgs::ImageConstPtr& msg){

    		cv_bridge::CvImagePtr cv_ptr;
    		namespace enc = sensor_msgs::image_encodings;

   		 try{
      			cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    		}
    		catch (cv_bridge::Exception& e){
      			ROS_ERROR("cv_bridge exception: %s", e.what());
      			return;
    		}	

		detect_edges(cv_ptr->image);	
  	}


  	void detect_edges(cv::Mat img){
	
		cv::Mat src;
		img.copyTo(src);
		imshow("Original Image",src);
		waitKey(0);

	}	 
};

int main(int argc, char** argv)
{
	ros::init(argc, argv, "Edge_Detector");
	Edge_Detector ic;
  	ros::spin();
  	return 0;
}
