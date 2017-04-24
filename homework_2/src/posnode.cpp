#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include <string>
#include <sstream>
#include <iostream>
#include <tf/transform_listener.h>


void distanceCallback(const sensor_msgs::LaserScan::ConstPtr& msg, ros::Publisher pub, tf::TransformListener* tl) {
	std_msgs::String s;
	std::stringstream ss;
	ros::Time t = ros::Time::now();
	tf::StampedTransform transform;
	tf::Quaternion q;
	float x, y, theta;
	
	
	if (tl->canTransform("/odom", "/base_laser_link", t, NULL)) {
		tl->waitForTransform("/odom", "/base_laser_link", t, ros::Duration(3.0));
		tl->lookupTransform("/odom", "/base_laser_link", t, transform);
		
		tf::Vector3 v = transform.getOrigin();
		x = v.getX();
		y = v.getY();
		q = transform.getRotation();
		theta = q.getAngle();
		
		ss << "Laser: [" << t.sec << "." <<t.nsec << "] " << x << " " << y << " " << theta;
		s.data = ss.str();
		pub.publish(s);
		
		ros::spinOnce();
	} else ROS_INFO("NO");
}

int main(int argc, char **argv) {

	ros::init(argc, argv, "posnode");
	tf::TransformListener tl;

	ros::NodeHandle n;  
	
	ros::Publisher pub = n.advertise<std_msgs::String>("postopic", 1000);
	ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("base_scan", 1000, boost::bind(distanceCallback, _1, pub, &tl));

	ros::spin();
	return 0;
}
