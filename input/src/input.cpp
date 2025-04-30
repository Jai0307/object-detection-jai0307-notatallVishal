
#include <rclcpp/rclcpp.hpp>
#include <thread>
#include <iostream>
#include <string>

class InputMonitorNode : public rclcpp::Node
{
public:
  InputMonitorNode() : Node("input_monitor_node")
  {
    RCLCPP_INFO(this->get_logger(), "Node started. Type input below:");
    input_thread_ = std::thread(&InputMonitorNode::readInput, this);
  }

  ~InputMonitorNode()
  {
    keep_running_ = false;
    if (input_thread_.joinable()) {
      input_thread_.join();
    }
  }

private:
  void readInput()
  {
    std::string input;
    while (rclcpp::ok() && keep_running_) {
      std::cout << ">>> ";
      std::getline(std::cin, input);
      if (!input.empty()) {
        RCLCPP_INFO(this->get_logger(), "User typed: '%s'", input.c_str());
        // You could publish this, call a service, etc.
      }
    }
  }

  std::thread input_thread_;
  bool keep_running_ = true;
};


int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<SpinSomeNode>();
  rclcpp::ExecutorOptions options;
  rclcpp::executors::SingleThreadedExecutor executor(options);
  executor.add_node(node);

  // Main loop with spin_some
  while (rclcpp::ok() && other_flags) {
    executor.spin_some();  // Process any ready callbacks

    // You can do other stuff here such as read user input and set other_flags
  }

  rclcpp::shutdown();
  return 0;
}