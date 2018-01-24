#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include "distance.h"

Distance dist;

// callback function
void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
  //looks at sensor values from 45 degrees to 135 degrees 
  //in front of the turtlebot
  for(unsigned int i=180;i<540;i++){
    //the minimum distance value is updated if the scan value is smaller than it,
    //and then moves on to the next angle 
    if(scan->ranges[i]<dist.min_dist_out()){
      dist.set_min(scan->ranges[i]);   
      i++;
    }
    
  //checks which side of the robot the obstacle is closer to
  dist.set_sides(scan->ranges[180], scan->ranges[540]);
   
  }
  //print the distance to an obstacle in front of the robot
  ROS_INFO("%f", scan->ranges[360]); 

}

int main(int argc, char** argv) {

  ros::init(argc, argv, "laser_subscriber");
  ros::NodeHandle nh;
  
  //publisher publishes messages of type Twist to /cmd_vel topic
  ros::Publisher vel_pub=nh.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop",1);
  //subscriber subscribes to the laser/scan/ topic
  ros::Subscriber sub = nh.subscribe("/scan", 10000, scanCallback);
  //instantiate Twist message
  geometry_msgs::Twist vel;
  
  ros::Rate loop_rate(5); 
  //set initial minimum distance

  dist.set_min(5.0);
  
  while(ros::ok()){
    //if distnce is above threshold, move forward
    if(dist.min_dist_out()>=1.0){
      vel.linear.x = 0.3;
      vel.angular.z = 0.0;
      vel_pub.publish(vel);
//      ROS_INFO("%f", distance);    
    }
    //if distance is below threshold, and there is more distance in the left side
    //than the right side, turn anticlockwise
     if(dist.min_dist_out()<=1.0 & dist.clockwise_out()==0){
       vel.linear.x = 0.0;
       vel.angular.z = 0.5;
       vel_pub.publish(vel);
     }
    //if distance is below threshold, and there is more distance in the right side
    //than the left side, turn clockwise
     if(dist.min_dist_out()<=1.0 & dist.clockwise_out()==1){
       vel.linear.x = 0.0;
       vel.angular.z = -0.5;
       vel_pub.publish(vel);
     }
    //reset distance value to something above threshold
   dist.set_min(5.0);
    ros::spinOnce();
    //sleep at the set loop_rate of 5 hz
    loop_rate.sleep();
    
  }
    
  return 0;
}