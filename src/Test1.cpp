#include <ros/ros.h>
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <sensor_msgs/image_encodings.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TransformStamped.h>
#include <rosgraph_msgs/Clock.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <stdio.h>
//Variables
int intOp;
float psi, x, y;
double C;



geometry_msgs::Twist emma_cmd;

void odom_callback(const nav_msgs::Odometry::ConstPtr& msg){
	x=msg->pose.pose.position.x;
	y=msg->pose.pose.position.y;
	psi=msg->pose.pose.orientation.z;
}

void clock_callaback(const rosgraph_msgs::Clock::ConstPtr& msg){
	C=msg->clock.toSec();
}


//Funciones Callback
void opt_callback(const std_msgs::Int32::ConstPtr& msg1){
	intOp=msg1->data;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "TEST1");
  ros::NodeHandle nodo;

	ros::Publisher steer_pub = nodo.advertise<std_msgs::Int16>("/steering", 1);
	ros::Publisher forward_pub = nodo.advertise<std_msgs::Int16>("/manual_control/speed", 1);


  //teclado	
  ros::Subscriber tec_sub = nodo.subscribe("/copt",10,opt_callback);
  ros::Subscriber tf_sub = nodo.subscribe("/odom",10,odom_callback);
  ros::Subscriber cl_sub =nodo.subscribe("/clock",1,clock_callaback);
  //Nodos de velocidad						
  std_msgs::Int16 FB;
	std_msgs::Int16 St;
  //Hover	
  std::cout<<"Master Control Node \n";
  FILE * PlotData = fopen("Datos.txt", "w");
  while(ros::ok()){

  	std::cout<<psi<<" "<<x<<" "<<y<<" "<<C<< "\n";

		if(C < 10.0 ){
			std::cout<<"Adelante \n";
			FB.data=50;
			forward_pub.publish(FB);
			St.data=90;
			steer_pub.publish(St);
		}else{
			if(C < 20.0 ){
				std::cout<<"Derecha \n";
				FB.data=50;
				forward_pub.publish(FB);
				St.data=120;
				steer_pub.publish(St);
			}
			else{
					if(C < 30.0 ){
						std::cout<<"Izquierda \n";
						FB.data=50;
						forward_pub.publish(FB);
						St.data=60;
						steer_pub.publish(St);
					}
					else{
						std::cout<<"Izquierda \n";
						FB.data=0;
						forward_pub.publish(FB);
						St.data=90;
						steer_pub.publish(St);
					}
			}
			
		}


  	

	fprintf(PlotData,"%f %f %f %f \n",x,y,psi,C);
	ros::spinOnce();
	//ros::spin();
  }
  return 0;	
}
