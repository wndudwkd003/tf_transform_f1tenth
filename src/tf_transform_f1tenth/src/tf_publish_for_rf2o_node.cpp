#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/static_transform_broadcaster.h"
#include "tf2/LinearMath/Quaternion.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "sensor_msgs/msg/imu.hpp"

using std::string;

class StaticTFPublisher : public rclcpp::Node {
public:
  StaticTFPublisher()
  : Node("static_tf_publisher_node") {
    static_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);

    // Static transforms
    publish_static_tf("world", "map");
    publish_static_tf("map", "odom");
    publish_static_tf("base_link", "scan");
    publish_static_tf("base_link", "imu_link");

    // LiDAR Sub & Pub
    lidar_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
        "/autodrive/roboracer_1/lidar", 10,
        std::bind(&StaticTFPublisher::lidar_callback, this, std::placeholders::_1)
    );

    scan_pub_ = this->create_publisher<sensor_msgs::msg::LaserScan>(
        "/scan", 10
    );

    imu_sub_ = this->create_subscription<sensor_msgs::msg::Imu>(
      "/autodrive/roboracer_1/imu", 10,
      std::bind(&StaticTFPublisher::imu_callback, this, std::placeholders::_1)
    );

    imu_pub_ = this->create_publisher<sensor_msgs::msg::Imu>(
      "/imu", 10
    );

    RCLCPP_INFO(this->get_logger(), "Published static transforms");
  }

private:
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> static_broadcaster_;
  
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr lidar_sub_;
  rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub_;

  rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_pub_;
  rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_pub_;

  void publish_static_tf(const string &parent_frame, const string &child_frame) {
    geometry_msgs::msg::TransformStamped tf_msg;
    tf_msg.header.stamp = this->get_clock()->now();
    tf_msg.header.frame_id = parent_frame;
    tf_msg.child_frame_id = child_frame;

    // 고정 위치 (0,0,0)
    tf_msg.transform.translation.x = 0.0;
    tf_msg.transform.translation.y = 0.0;
    tf_msg.transform.translation.z = 0.0;

    // 회전 없음
    tf2::Quaternion q;
    q.setRPY(0.0, 0.0, 0.0);
    tf_msg.transform.rotation.x = q.x();
    tf_msg.transform.rotation.y = q.y();
    tf_msg.transform.rotation.z = q.z();
    tf_msg.transform.rotation.w = q.w();

    static_broadcaster_->sendTransform(tf_msg);
  }

  void lidar_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
    auto new_msg = *msg;
    new_msg.header.frame_id = "scan";
    scan_pub_->publish(new_msg);
  }

  void imu_callback(const sensor_msgs::msg::Imu::SharedPtr msg) {
    auto new_msg = *msg;
    new_msg.header.frame_id = "imu_link";
    imu_pub_->publish(new_msg);
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<StaticTFPublisher>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
