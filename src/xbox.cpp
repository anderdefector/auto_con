#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <iostream>
#include <std_msgs/Int16.h>
#include <stdio.h>
uint8_t battery;
float y=0.0, z=0.0, az=0.0, volante=0.0, xv=0.0, zv=0.0, reversa, acelerador;
int A=0, B=0, X=0,OP=0, st,sp;

using namespace std;

float vx=0.0,vy=0.0,vz=0.0,vaz=0.0;
geometry_msgs::Twist emma_cmd;

std_msgs::Int16 Sp;
std_msgs::Int16 St;

void c_vel_dir (int st, int sp);

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy){
  	volante=joy->axes[0];
	reversa=joy->axes[2];
	acelerador=joy->axes[5];
  	xv=joy->axes[1];
	zv=joy->axes[4];
	A=joy->buttons[0];
	B=joy->buttons[1];
	X=joy->buttons[2];
	if(A==1){ OP=0;	}	
	if(X==1){ OP=1; }
	if(B==1){ OP=2; }
	if(xv==1){ OP=3;}
	if(xv==-1){ OP=4;}
	//if(yv==1){ OP=5;}
	//if(yv==-1){ OP=6;}
	if(zv==1){ OP=7;}
	if(zv==-1){ OP=8;}
		
}


int main(int argc, char** argv)
{
	
  	ros::init(argc, argv, "xbox_con");
	ros::NodeHandle n;
	
	ros::Publisher steer_pub = n.advertise<std_msgs::Int16>("/steering", 1);	
	ros::Publisher speed_pub = n.advertise<std_msgs::Int16>("/manual_control/speed", 1);
	
	ros::Subscriber cxbox_sub=n.subscribe<sensor_msgs::Joy>("/joy",10,joyCallback);

	std_msgs::Empty takeoff_cmd;
 	std_msgs::Empty land_cmd;
	
	cout<<"XBOX"<<endl;
  	while(ros::ok()){
		
		//Control de giro
		if(volante == 0.0){
			st = 90;
		}else{
			if(volante > 0.0){
				if(volante > 0.9){
					st = 0;
				}else{
					st = int(90-(volante*100));
				}
			}else{
				if(volante < 0.0){
					if(volante < -0.9){
						st = 180;
					}else{
						st = int(90-(volante*100));
					}
				}else{
					st = st;
				}
			}
		}
		
		/*if(reversa< 0.0){
			sp=-200;
		}else{
			sp= int(-1*(200-((reversa*100)*2)));
		}*/
		
		if(acelerador < 0.0){
			sp=200;
		}else{
			sp= int(200-((acelerador*100)*2));
		}
		
		c_vel_dir(st,sp);
		speed_pub.publish(Sp);
		steer_pub.publish(St);
		cout<<"Steering: "<<st<<" Speed: "<<sp<<endl;

		ros::spinOnce();
	}
	return 0;  	
}

void c_vel_dir (int st, int sp){
	
	Sp.data=sp;
	St.data=st;

}
