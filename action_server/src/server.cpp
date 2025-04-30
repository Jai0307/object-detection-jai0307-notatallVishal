/*
*
Action Server Specs
- Gets a goal request with a specified object to track. ✅ (Written)
- It accepts the client's request if the object is one 
	of the 80 object classes from the COCO dataset. Otherwise, it rejects it. ✅ (Written)
	
- After the request is accepted and the object is already 
	in the RealSense camera's field of view (FOV), 
	the server starts tracking it and sending feedback to the client. 
	The feedback should consist of short messages about the general location of the object: 
	left, right, center, ahead, top left, bottom right, etc. 
	
- If the object box center is anywhere around the center of the camera FOV 
	(within ±30 pixel radius) and about 20 cm away from the robot, 
	then the action server will complete the action and return 
	the result message "Tracking Successful!" to the client. 
	
- If the object bbox center disappears from the camera FOV 
	anytime after the start of tracking, then the action 
	server will terminate the actions and return the message "Tracking Failed."
	
- If the requested object is not initially in the camera FOV, 
	the action server will wait indefinitely until it appears. 
	We will change this behavior in Part 2.
    While waiting, the action server will send the feedback message to the client: "Searching."
*
*/
 
#include <functional>
#include <memory>
#include <thread>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

#include "vision_msgs/msg/Detection2DArray.hpp"
#include "action_client/action/trackObject.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "std_msgs/msg/bool.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"


enum trackingState_t = { /*States to determine movement */
	T_WAIT,
	T_SEARCH,
	T_TRACK, /* */
	T_STOP /* Complete State */
	};

class trackObjectServer : public rclcpp::Node
 {
public:
  using trackObject = action_client::action::trackObject;
  using GoalHandleTrackObject = rclcpp_action::ClientGoalHandle<trackObject>;

  explicit trackObjectServer(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
  : Node("track_object_server", options)
  {
    using namespace std::placeholders;

    this->action_server_ = rclcpp_action::create_server<trackObject>(
      this,
      "move_robot",
      std::bind(&trackObjectServer::handle_goal, this, _1, _2),
      std::bind(&trackObjectServer::handle_cancel, this, _1),
      std::bind(&trackObjectServer::handle_accepted, this, _1));

    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
	
	/* Subscribe to detections_output and depth */
	
	cameraSubscription_ = this->create_subscription<vision_msgs::msgs::Detection2DArray>(
		"/detections_output", 10, std::bind(&trackObjectServer::camera_callback), this, _1);
	
	
	subscription_ = this->create_subscription<:: ::>(
        "/dep_dist", 10, std::bind(&trackObjectServer::depth_callback, this, _1));

  }

private:
  static const std::set<std::string> COCO_ObjSet = {
        "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
        "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
        "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
        "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
        "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
        "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
        "potted plant", "bed", "dining table", "toilet", "TV", "laptop", "mouse", "remote", "keyboard", "cell phone",
        "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
        "hair drier", "toothbrush"
    };
	
  static const std::map<int, std::string> CocoLabelMap = {
	  {  0, "person"         }, {  1, "bicycle"        },
	  {  2, "car"            }, {  3, "motorcycle"    },
	  {  4, "airplane"       }, {  5, "bus"            },
	  {  6, "train"          }, {  7, "truck"          },
	  {  8, "boat"           }, {  9, "traffic light"  },
	  { 10, "fire hydrant"   }, { 11, "stop sign"      },
	  { 12, "parking meter"  }, { 13, "bench"          },
	  { 14, "bird"           }, { 15, "cat"            },
	  { 16, "dog"            }, { 17, "horse"          },
	  { 18, "sheep"          }, { 19, "cow"            },
	  { 20, "elephant"       }, { 21, "bear"           },
	  { 22, "zebra"          }, { 23, "giraffe"        },
	  { 24, "backpack"       }, { 25, "umbrella"       },
	  { 26, "handbag"        }, { 27, "tie"            },
	  { 28, "suitcase"       }, { 29, "frisbee"        },
	  { 30, "skis"           }, { 31, "snowboard"      },
	  { 32, "sports ball"    }, { 33, "kite"           },
	  { 34, "baseball bat"   }, { 35, "baseball glove" },
	  { 36, "skateboard"     }, { 37, "surfboard"      },
	  { 38, "tennis racket"  }, { 39, "bottle"         },
	  { 40, "wine glass"     }, { 41, "cup"            },
	  { 42, "fork"           }, { 43, "knife"          },
	  { 44, "spoon"          }, { 45, "bowl"           },
	  { 46, "banana"         }, { 47, "apple"          },
	  { 48, "sandwich"       }, { 49, "orange"         },
	  { 50, "broccoli"       }, { 51, "carrot"         },
	  { 52, "hot dog"        }, { 53, "pizza"          },
	  { 54, "donut"          }, { 55, "cake"           },
	  { 56, "chair"          }, { 57, "couch"          },
	  { 58, "potted plant"   }, { 59, "bed"            },
	  { 60, "dining table"   }, { 61, "toilet"         },
	  { 62, "tv"             }, { 63, "laptop"         },
	  { 64, "mouse"          }, { 65, "remote"         },
	  { 66, "keyboard"       }, { 67, "cell phone"     },
	  { 68, "microwave"      }, { 69, "oven"           },
	  { 70, "toaster"        }, { 71, "sink"           },
	  { 72, "refrigerator"   }, { 73, "book"           },
	  { 74, "clock"          }, { 75, "vase"           },
	  { 76, "scissors"       }, { 77, "teddy bear"     },
	  { 78, "hair drier"     }, { 79, "toothbrush"     }
	};


  rclcpp_action::Server<Move>::SharedPtr action_server_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;


  void camera_callback(const /* whatever the type is */ msg) {

  }
  
  void depth_callback(const /* whatever the type is */ msg) {
	  
  }
  
  rclcpp_action::GoalResponse handle_goal(
    const rclcpp_action::GoalUUID & uuid,
    std::shared_ptr<const trackObject::Goal> goal)
  {
	
	if(COCO_ObjSet.find(goal->object_name) == COCO_ObjSet.end()) {
		RCLCPP_WARN(this->get_logger(), "Rejected request: '%s' is not a COCO dataset object.", goal->object_name.c_str());
            return rclcpp_action::GoalResponse::REJECT;
	}
	
    (void)uuid;
	RCLCPP_INFO(this->get_logger(), "Accepted request to track: '%s'", goal->object_name.c_str());
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  rclcpp_action::CancelResponse handle_cancel(
    const std::shared_ptr<GoalHandleTrackObject> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "Received request to cancel goal.");
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  void handle_accepted(const std::shared_ptr<GoalHandleTrackObject> goal_handle)
  {
    using namespace std::placeholders;
    // this needs to return quickly to avoid blocking the executor, so spin up a new thread
    std::thread{std::bind(&trackObjectServer::execute, this, _1), goal_handle}.detach();
  }

  void 

  void execute(const std::shared_ptr<GoalHandleTrackObject> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "[LOG] Executing goal");
    const auto goal = goal_handle->get_goal();
    auto feedback = std::make_shared<trackObject::Feedback>();
    auto & message = feedback->feedback;
    message = "Starting movement...";
    auto result = std::make_shared<trackObject::Result>();
    auto move = geometry_msgs::msg::Twist();
	    
	rclcpp::Rate loop_rate(50);
	
	while(1) {
		
		loop_rate.sleep();
	}
		
	if (rclcpp::ok()) {
      result->result = "Mission Accomplished";
      goal_handle->succeed(result);
      RCLCPP_INFO(this->get_logger(), "Goal succeeded");
    }
	
  }
};  // class trackObjectServer

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto action_server = std::make_shared<trackObjectServer>();
    
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(action_server);
  executor.spin();

  rclcpp::shutdown();
  return 0;
}