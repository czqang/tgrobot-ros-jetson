#ifndef __CHASSIS_NODE_HPP_
#define __CHASSIS_NODE_HPP_

#include <ros/ros.h>
#include <serial/serial.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/LinearMath/Quaternion.h>
#include <nav_msgs/Odometry.h>
// #include <nav_msgs/Path.h>
#include <sensor_msgs/BatteryState.h>
#include <sensor_msgs/Imu.h>
#include <chassis_msgs/Ultrasonic.h>
#include <signal.h>

#define PI      3.1415926f

inline void mySigintHandler(int sig);

typedef struct __vel_chassis {
    float x, y, z,theta;
}Vel_Chassis;

typedef struct __pose_chassis {
    float x, y;
}Pose_Chassis;

typedef struct __odom_chassis {
    Vel_Chassis vel;
    Pose_Chassis pose;
}Odom_Chassis;

class Chassis 
{
    public:
        Chassis(ros::NodeHandle comm_nh, ros::NodeHandle param_nh);
        ~Chassis();

        void CMD_Vel_Callback(const geometry_msgs::Twist &twist_aux);
        bool SendCMD_WaitResponse(const uint8_t* w_data, uint8_t *r_data, uint8_t num);
        uint8_t Check_CRC(const uint8_t *data, uint8_t len);
        void BatteryPub_TimerCallback(const ros::TimerEvent &event);
        bool GetOdometer_toSensor(Odom_Chassis *odom);
        void OdomPub_TimerCallback(const ros::TimerEvent &event);
        void UltrasonicPub_TimerCallback(const ros::TimerEvent &event);
        void IMUdataPub_TimerCallback(const ros::TimerEvent &event);

    private:
        ros::NodeHandle node, pnode;
        serial::Serial serial_port;
        std::string serial_port_name, base_frame_id, odom_frame_id;
        int serial_baud_rate;
        ros::Subscriber twist_cmd_vel;

        bool pub_odometer, pub_battery, pub_ultrasonic, pub_imu;
        ros::Publisher battery_pub, odometer_pub, ultrasonic_pub, imu_pub;
        ros::Timer battery_timer, odometer_timer, ultrasonic_timer, imu_timer;
};

#endif