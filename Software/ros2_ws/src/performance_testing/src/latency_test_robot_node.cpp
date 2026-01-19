#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/header.hpp>
#include <functional>
#include <memory>

class LatencyTestRobotNode : public rclcpp::Node
{
public:

    LatencyTestRobotNode() : Node("LatencyTestRobotNode") {
        // subscriber
        subscription_ = this->create_subscription<std_msgs::msg::Header>("/latency_halfway", 10, 
            std::bind(&LatencyTestRobotNode::forwardCallback, this, std::placeholders::_1));
        
        // publisher
        publisher_ = this->create_publisher<std_msgs::msg::Header>("/latency_fullway", 10);
    }

private:

    void forwardCallback(const std_msgs::msg::Header::SharedPtr msg) {
        publisher_->publish(*msg);
    }

    rclcpp::Publisher<std_msgs::msg::Header>::SharedPtr publisher_;
    rclcpp::Subscription<std_msgs::msg::Header>::SharedPtr subscription_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<LatencyTestRobotNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}