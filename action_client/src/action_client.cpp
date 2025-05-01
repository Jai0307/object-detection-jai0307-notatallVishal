/*
*

Action Client Specs
- Asks for user input about the object to track (see below).
- Sends a goal to the action server with the specified object.
- Prints all feedback messages received from the action server
	during tracking.
- Prints the result message from the action server: 
	"Tracking Successful!" or "Tracking Failed."
- After receiving a result message from the server, whether successful or not, 
  the client asks for new user input for an object to track and requests that 
  the server track the new object. 
*
*/

#include <inttypes.h>
#include <memory>
#include <string>
#include <iostream>

#include "custom_interfaces/action/detect.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "std_msgs/msg/string.hpp"

class TrackObjectClient : public rclcpp::Node
{
public:

  using Detect = custom_interfaces::action::Detect;
  using GoalHandleDetect = rclcpp_action::ClientGoalHandle<Detect>;

  explicit TrackObjectClient(const rclcpp::NodeOptions & node_options = rclcpp::NodeOptions())
  : Node("action_client", node_options), goal_done_(false)
  {
    using namespace std::placeholders;

    this->client_ptr_ = rclcpp_action::create_client<Detect>(
      this->get_node_base_interface(),
      this->get_node_graph_interface(),
      this->get_node_logging_interface(),
      this->get_node_waitables_interface(),
      "action_client");


	subscription_ = this->create_subscription<std_msgs::msg::String>("input_topic", 10, 
					std::bind(&TrackObjectClient::input_callback, this, _1));
					
    this->timer_ = this->create_wall_timer(
      std::chrono::milliseconds(500),
      std::bind(&TrackObjectClient::send_goal, this));
  }

  bool is_goal_done() const
  {
    return this->goal_done_;
  }

  void send_goal()
  {
    using namespace std::placeholders;

    this->timer_->cancel();

    this->goal_done_ = false;

    if (!this->client_ptr_) {
      RCLCPP_ERROR(this->get_logger(), "Action client not initialized");
    }

    while (!this->client_ptr_->wait_for_action_server(std::chrono::seconds(10) )) {
      RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
      this->goal_done_ = true;
      // sleep(1)
      return;
    }
    // while(objectName == ""){
    //   this->client_ptr_->wait_for_action_server(std::chrono::seconds(10)
    // }
    auto goal_msg = Detect::Goal();
	
    goal_msg.object_name = objectName;


    RCLCPP_INFO(this->get_logger(), "Sending goal");

    auto send_goal_options = rclcpp_action::Client<Detect>::SendGoalOptions();
                
    send_goal_options.goal_response_callback =
      std::bind(&TrackObjectClient::goal_response_callback, this, _1);

    send_goal_options.feedback_callback =
      std::bind(&TrackObjectClient::feedback_callback, this, _1, _2);

    send_goal_options.result_callback =
      std::bind(&TrackObjectClient::result_callback, this, _1);
      
    auto goal_handle_future = this->client_ptr_->async_send_goal(goal_msg, send_goal_options);
  }

private:
  rclcpp_action::Client<Detect>::SharedPtr client_ptr_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  std::string objectName;
  rclcpp::TimerBase::SharedPtr timer_;
  bool goal_done_;
  
  void input_callback(const std_msgs::msg::String::SharedPtr msg) {
	RCLCPP_INFO(this->get_logger(), "Got %s\n", msg->data.c_str());
	objectName = msg->data;
  }

  void goal_response_callback(const GoalHandleDetect::SharedPtr & goal_handle)
  {
    if (!goal_handle) {
      RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
    } else {
      RCLCPP_INFO(this->get_logger(), "Goal accepted by server, waiting for result");
    }
  }

  void feedback_callback(
    GoalHandleDetect::SharedPtr goal_handler,
    const std::shared_ptr<const Detect::Feedback> feedback)
  {
    RCLCPP_INFO(
      this->get_logger(), "Feedback received: %s", feedback->feedback.c_str());
	  
	 if(strcmp(feedback->feedback.c_str(), "Terminate.")){
		auto canceler = client_ptr_->async_cancel_goal(goal_handler);
	 }
  }

  void result_callback(const GoalHandleDetect::WrappedResult & result)
  {
    this->goal_done_ = true;
    switch (result.code) {
      case rclcpp_action::ResultCode::SUCCEEDED:
		RCLCPP_ERROR(this->get_logger(), "Tracking Successful!!!!");
        break;
      case rclcpp_action::ResultCode::ABORTED:
        RCLCPP_ERROR(this->get_logger(), "Tracking Failed.");
        return;
      case rclcpp_action::ResultCode::CANCELED:
        RCLCPP_ERROR(this->get_logger(), "Action Cancelled. Tracking Failed.");
        return;
      default:
        RCLCPP_ERROR(this->get_logger(), "Unknown result code");
        return;
    }

    RCLCPP_INFO(this->get_logger(), "Result received: %s", result.result->result.c_str());

  }
};  // class TrackObjectClient

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto action_client = std::make_shared<TrackObjectClient>();
    
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(action_client);

  while (!action_client->is_goal_done()) {
    executor.spin();
  }

  rclcpp::shutdown();
  return 0;
}
