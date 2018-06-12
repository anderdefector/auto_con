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

/*Unicamente se buscaba que el carro avanzara durante 10 segundos en linea recta
posteriormente se hacia la medición de la distancia recorrida. Las RPM se fueron variando
de 0 a 200 en incrementos de 25. :) XoXo
*/

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
	FILE * PlotData = fopen("Datos.txt", "w");
	while(ros::ok()){

  	std::cout<<psi<<" "<<x<<" "<<y<<" "<<C<< "\n";

	switch(intOp){
		case 0:
			if(x < 4.2 ){
				std::cout<<"Adelante \n";
				c_vel_dir(90,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);
				intOp=0;
			}else{
				intOp=1;
			}
			break;
		case 1:
			if(psi < 0.68 ){ //y=1.4
				std::cout<<"Giro Izquierda \n";
				c_vel_dir(50,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);
				intOp=1;
			}else{
				intOp=2;
			}
			break;
		case 2:
			if(y < 2.2 ){
				std::cout<<"Adelante \n";
				c_vel_dir(90,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);
				intOp=2;
			}else{
				intOp=3;
			}
			break;
		case 3:
			if(x > 3.8 ){
				std::cout<<"Giro Izquierda \n";
				c_vel_dir(50,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);
				intOp=3;
			}else{
				intOp=4;
			}
			break;
		case 4:
			if(x > 3.18 ){
				std::cout<<"Giro Derecha \n";
				c_vel_dir(160,70);
				speed_pub.publish(Sp);
				steer_pub.publish(St);

				intOp=4;
			}else{
				intOp=5;
			}
			break;
		case 5:
			if(y > 3.0 ){
				std::cout<<"Giro Izquierda \n";
				c_vel_dir(50,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);
				intOp=5;
			}else{
				intOp=6;
			}
			break;
		case 6:
			if(y > 2.0 ){
				std::cout<<"Adelante \n";
				c_vel_dir(90,150);
				speed_pub.publish(Sp);
				steer_pub.publish(St);
				intOp=6;
			}else{
				intOp=7;
			}
			break;
		case 7:
			if(psi < 0.0 ){
				std::cout<<"Giro Izquierda \n";
				c_vel_dir(50,150);
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