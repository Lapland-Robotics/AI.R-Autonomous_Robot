#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/header.hpp>
#include <chrono>
#include <functional>
#include <memory>

using namespace std::chrono_literals;

class LatencyTestRemoteNode : public rclcpp::Node {

public:

    LatencyTestRemoteNode() : Node("latency_remote_node") {
        
        // publisher
        publisher_ = this->create_publisher<std_msgs::msg::Header>("/latency_halfway", 10);
        
        // subscriber
        subscription_ = this->create_subscription<std_msgs::msg::Header>("/latency_fullway",10,
            std::bind(&LatencyTestRemoteNode::latencyCallback, this, std::placeholders::_1));
        
        // timer 10Hz
        timer_ = this->create_wall_timer(100ms,std::bind(&LatencyTestRemoteNode::publishTimestamp, this));
    }

private:

    // Timer callback
    void publishTimestamp() {
        std_msgs::msg::Header msg;
        msg.stamp = this->now();
        publisher_->publish(msg);
    }
    
    // latency calculation
    void latencyCallback(const std_msgs::msg::Header::SharedPtr msg) {
        rclcpp::Time now = this->now();
        double latency_ms = (now - msg->stamp).seconds() * 1000.0;
        RCLCPP_INFO(this->get_logger(), "Latency (rtt): %.3f ms", latency_ms);
    }

    rclcpp::Publisher<std_msgs::msg::Header>::SharedPtr publisher_;
    rclcpp::Subscription<std_msgs::msg::Header>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<LatencyTestRemoteNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}