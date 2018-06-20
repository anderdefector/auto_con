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
#include <cmath> 
#include <stdio.h>

using namespace std;
//Variables
int intOp=0;
float psi, x, y, e;
float cpAx=4.44, cpBth=0.70, cpCy=2.4, cpDx=3.7, cpEx=2.38, cpFy=2.4, cpGy=1.51, cpHth = 0.0;
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
	ros::init(argc, argv, "TEST2");
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
	FILE * PlotData = fopen("Datos7.txt", "w");
	while(ros::ok()){

  	//std::cout<<psi<<" "<<x<<" "<<y<<" "<<C<< "\n";

	switch(intOp){
		case 0:
            e = abs(cpAx-x);
			if(e >= 0.2 ){
				std::cout<<"CPA Avanza Error: "<<e<<" x: "<<x<<" y: "<<y<<" psi: "<<psi<<" "<<C<< "\n";
				c_vel_dir(90,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);           
				intOp=0;
			}else{
				intOp=1;
			}
			break;
		case 1:
            e = abs(cpBth-psi);
			if(e >= 0.01 ){ //y=1.4
                std::cout<<"CPB GiraIzq Error: "<<e<<" x: "<<x<<" y: "<<y<<" psi: "<<psi<<" "<<C<< "\n";
				c_vel_dir(40,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);
				intOp=1;
			}else{
				intOp=2;
			}
			break;
		case 2:
            e = abs(cpCy-y);
			if(e >= 0.2 ){
				std::cout<<"CPC Avanza Error: "<<e<<" x: "<<x<<" y: "<<y<<" psi: "<<psi<<" "<<C<< "\n";
				c_vel_dir(90,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);
				intOp=2;
			}else{
				intOp=3;
			}
			break;
		case 3:
            e = abs(cpDx-x);
			if(e >= 0.2 ){
				std::cout<<"CPD GiraIzq Error: "<<e<<" x: "<<x<<" y: "<<y<<" psi: "<<psi<<" "<<C<<"\n";
				c_vel_dir(40,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);
				intOp=3;
			}else{
				intOp=4;
			}
			break;
		case 4:
            e = abs(cpEx-x);
			if(e >= 0.2){
				std::cout<<"CPF GiraDer Error: "<<e<<" x: "<<x<<" y: "<<y<<" psi: "<<psi<<" "<<C<< "\n";
				c_vel_dir(130,70);
				speed_pub.publish(Sp);
				steer_pub.publish(St);

				intOp=4;
			}else{
				intOp=5;
			}
			break;
		case 5:
            e = abs(cpFy-y);
			if(e >= 0.2 ){
				std::cout<<"CPB GiraIzq Error: "<<e<<" x: "<<x<<" y: "<<y<<" psi: "<<psi<<" "<<C<<"\n";
				c_vel_dir(40,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);
				intOp=5;
			}else{
				intOp=6;
			}
			break;
		case 6:
            e = abs(cpGy-y);
			if(e >= 0.1 ){
				std::cout<<"CPG GiraIzq Error: "<<e<<" x: "<<x<<" y: "<<y<<" psi: "<<psi<<" "<<C<<"\n";
				c_vel_dir(90,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);
				intOp=6;
			}else{
				intOp=7;
			}
			break;
		case 7:
            e = abs(cpHth-psi);
			if(e >= 0.01 ){
				std::cout<<"CPH GiraIzq Error: "<<e<<" x: "<<x<<" y: "<<y<<" psi: "<<psi<<" "<<C<<"\n";
				c_vel_dir(40,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);
				intOp=7;
			}else{
				intOp=0;
			}
			break;
		default:
			c_vel_dir(90,0);
			speed_pub.publish(Sp);
			steer_pub.publish(St);			
			break;
	}

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
