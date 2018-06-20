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
int intOp=0;
float psi, x, y;
double C;


std_msgs::Int16 Sp;
std_msgs::Int16 St;

geometry_msgs::Twist emma_cmd;

void c_vel_dir (int st, int sp);

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
	ros::init(argc, argv, "TEST4");
	ros::NodeHandle nodo;

	ros::Publisher steer_pub = nodo.advertise<std_msgs::Int16>("/steering", 1);
	ros::Publisher speed_pub = nodo.advertise<std_msgs::Int16>("/manual_control/speed", 1);


	//teclado	
	ros::Subscriber tec_sub = nodo.subscribe("/copt",10,opt_callback);
	ros::Subscriber tf_sub = nodo.subscribe("/odom",10,odom_callback);
	ros::Subscriber cl_sub =nodo.subscribe("/clock",1,clock_callaback);
	//Nodos de velocidad						
  
  //Hover	
	std::cout<<"Master Control Node \n";
	FILE * PlotData = fopen("Datos5.txt", "w");
	while(ros::ok()){

  	std::cout<<psi<<" "<<x<<" "<<y<<" "<<C<< "\n";

  	//st 0-80
  	//speed 50-200 (50)
  	c_vel_dir(80,200);
	speed_pub.publish(Sp);
	steer_pub.publish(St);
	
	fprintf(PlotData,"%f %f %f %f \n",x,y,psi,C);
	ros::spinOnce();
	//ros::spin();
  }
  return 0;	
}

void c_vel_dir (int st, int sp){
	
	Sp.data=sp;
	St.data=st;

}

