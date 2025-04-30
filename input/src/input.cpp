
    #include <rclcpp/rclcpp.hpp>
    #include <thread>
    #include <iostream>
    #include <string>
    
    class InputMonitorNode : public rclcpp::Node
    {
    public:
      InputMonitorNode() : Node("input_monitor_node")
      {
		publisher_ = this->create_publisher<std_msgs::msgs:String>("input_topic", 10);
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
		auto message = std_msgs::msgs:String; 
        std::string input;
        while (rclcpp::ok() && keep_running_) {
          std::cout << ">>> ";
          std::getline(std::cin, input);
          if (!input.empty()) {
            RCLCPP_INFO(this->get_logger(), "User typed: '%s'", input.c_str());
            // You could publish this, call a service, etc.
			message.data = input;
			publisher_->publish(message);
          }
        }
      }
    
      std::thread input_thread_;
      bool keep_running_ = true;
    };
    
    int main(int argc, char *argv[])
    {
      rclcpp::init(argc, argv);
      auto node = std::make_shared<InputMonitorNode>();
      rclcpp::spin(node);
      rclcpp::shutdown();
      return 0;
    }