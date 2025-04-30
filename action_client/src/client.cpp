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

#include "action_client/action/trackObject.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

class trackObjectClient : public rclcpp::Node
{
public:
  using trackObject = action_client::action::trackObject;
  using GoalHandleTrackObject = rclcpp_action::ClientGoalHandle<trackObject>;

  explicit trackObjectClient(const rclcpp::NodeOptions & node_options = rclcpp::NodeOptions())
  : Node("track_object_client", node_options), goal_done_(false)
  {
    this->client_ptr_ = rclcpp_action::create_client<trackObject>(
      this->get_node_base_interface(),
      this->get_node_graph_interface(),
      this->get_node_logging_interface(),
      this->get_node_waitables_interface(),
      "track_object_client");


	subscription_ = this->create_subscription<std_msgs::msgs::String>("input_topic", 10, 
					std::bind(&trackObjectClient::input_callback, this, _1);
					
    this->timer_ = this->create_wall_timer(
      std::chrono::milliseconds(500),
      std::bind(&trackObjectClient::send_goal, this));
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

    if (!this->client_ptr_->wait_for_action_server(std::chrono::seconds(10))) {
      RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
      this->goal_done_ = true;
      return;
    }

    auto goal_msg = trackObject::Goal();
	

    goal_msg.time_goal = 5;
    goal_msg.move_goal_lin_x = 0.3;
    goal_msg.move_goal_ang_z = 0.2;

    RCLCPP_INFO(this->get_logger(), "Sending goal");

    auto send_goal_options = rclcpp_action::Client<trackObject>::SendGoalOptions();
                
    send_goal_options.goal_response_callback =
      std::bind(&MyActionClient::goal_response_callback, this, _1);

    send_goal_options.feedback_callback =
      std::bind(&MyActionClient::feedback_callback, this, _1, _2);

    send_goal_options.result_callback =
      std::bind(&MyActionClient::result_callback, this, _1);
      
    auto goal_handle_future = this->client_ptr_->async_send_goal(goal_msg, send_goal_options);
  }

private:
  rclcpp_action::Client<trackObject>::SharedPtr client_ptr_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  std::string objectName;
  rclcpp::TimerBase::SharedPtr timer_;
  bool goal_done_;
  
  void input_callback(const std_msgs::msg::String::SharedPtr msg) {
	RCLCPP_INFO(this->get_logger(), "Got %s\n", msg->data.c_str());
	objectName = msg->data;
  }

  void goal_response_callback(const GoalHandleTrackObject::SharedPtr & goal_handle)
  {
    if (!goal_handle) {
      RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
    } else {
      RCLCPP_INFO(this->get_logger(), "Goal accepted by server, waiting for result");
    }
  }

  void feedback_callback(
    GoalHandleTrackObject::SharedPtr goal_handler,
    const std::shared_ptr<const trackObject::Feedback> feedback)
  {
    RCLCPP_INFO(
      this->get_logger(), "Feedback received: %s", feedback->feedback.c_str());
	  
	 if(strcmp(feedback->feedback.c_str(), "Terminate.")){
		auto canceler = client_ptr_->async_cancel_goal(goal_handler);
	 }
  }

  void result_callback(const GoalHandleTrackObject::WrappedResult & result)
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
};  // class MyActionClient

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto action_client = std::make_shared<MyActionClient>();
    
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(action_client);

  while (!action_client->is_goal_done()) {
    executor.spin();
  }

  rclcpp::shutdown();
  return 0;
}