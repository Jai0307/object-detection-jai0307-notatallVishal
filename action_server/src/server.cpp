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
#include <inttypes.h>

#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include <vision_msgs/msg/detection2_d_array.hpp>
#include <vision_msgs/msg/detection2_d.hpp>
#include <vision_msgs/msg/object_hypothesis_with_pose.hpp>
#include "custom_interfaces/action/detect.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/bool.hpp"
#include "sensor_msgs/msg/image.hpp"

#define SCREENWIDTH imageX
#define SCREENHEIGHT imageY

static const std::set<std::string> COCO_ObjSet = {
	"person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
	"fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
	"elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
	"skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
	"tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
	"sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
	"potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
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

static const std::map<std::string, std::string> FlippedCocoLabelMap = {
  { "person", "0" },         { "bicycle", "1" },
  { "car", "2" },            { "motorcycle", "3" },
  { "airplane", "4" },       { "bus", "5" },
  { "train", "6" },          { "truck", "7" },
  { "boat", "8" },           { "traffic light", "9" },
  { "fire hydrant", "10" },  { "stop sign", "11" },
  { "parking meter", "12" }, { "bench", "13" },
  { "bird", "14" },          { "cat", "15" },
  { "dog", "16" },           { "horse", "17" },
  { "sheep", "18" },         { "cow", "19" },
  { "elephant", "20" },      { "bear", "21" },
  { "zebra", "22" },         { "giraffe", "23" },
  { "backpack", "24" },      { "umbrella", "25" },
  { "handbag", "26" },       { "tie", "27" },
  { "suitcase", "28" },      { "frisbee", "29" },
  { "skis", "30" },          { "snowboard", "31" },
  { "sports ball", "32" },   { "kite", "33" },
  { "baseball bat", "34" },  { "baseball glove", "35" },
  { "skateboard", "36" },    { "surfboard", "37" },
  { "tennis racket", "38" }, { "bottle", "39" },
  { "wine glass", "40" },    { "cup", "41" },
  { "fork", "42" },          { "knife", "43" },
  { "spoon", "44" },         { "bowl", "45" },
  { "banana", "46" },        { "apple", "47" },
  { "sandwich", "48" },      { "orange", "49" },
  { "broccoli", "50" },      { "carrot", "51" },
  { "hot dog", "52" },       { "pizza", "53" },
  { "donut", "54" },         { "cake", "55" },
  { "chair", "56" },         { "couch", "57" },
  { "potted plant", "58" },  { "bed", "59" },
  { "dining table", "60" },  { "toilet", "61" },
  { "tv", "62" },            { "laptop", "63" },
  { "mouse", "64" },         { "remote", "65" },
  { "keyboard", "66" },      { "cell phone", "67" },
  { "microwave", "68" },     { "oven", "69" },
  { "toaster", "70" },       { "sink", "71" },
  { "refrigerator", "72" },  { "book", "73" },
  { "clock", "74" },         { "vase", "75" },
  { "scissors", "76" },      { "teddy bear", "77" },
  { "hair drier", "78" },    { "toothbrush", "79" }
};


enum trackingState_t { /*States to determine movement */
	T_WAIT = 0,
	T_LOOK = 1,
	T_MOVE = 2,
	T_STOP = 3
	};

class TrackObjectServer : public rclcpp::Node
 {
public:
  using Detect = custom_interfaces::action::Detect;
  using GoalHandleDetect = rclcpp_action::ServerGoalHandle<Detect>;

  explicit TrackObjectServer(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
  : Node("track_object_server", options)
  {
    using namespace std::placeholders;

    this->action_server_ = rclcpp_action::create_server<Detect>(
      this,
      "detect_action",
      std::bind(&TrackObjectServer::handle_goal, this, _1, _2),
      std::bind(&TrackObjectServer::handle_cancel, this, _1),
      std::bind(&TrackObjectServer::handle_accepted, this, _1));

    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
	
	/* Subscribe to detections_output and depth */
	
	cameraSubscription_ = this->create_subscription<vision_msgs::msg::Detection2DArray>(
		"/detections_output", 10, std::bind(&TrackObjectServer::camera_callback, this, _1));
	
	
	depthSubscription_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/depth", 10, std::bind(&TrackObjectServer::depth_callback, this, _1));

  }

private:
  


  /* Camera Stored variables */
  std::string reqObjectName;
  vision_msgs::msg::Detection2D reqObject;
  float objX;
  float objY;
  float sizeX;
  float sizeY;

  float imageX, imageY;

  bool objFound;
  bool objFoundOnce = false;
  bool headOn;
  bool input_recieved = false;
  /* depth Stored variables */
  uint8_t avgDepth_BB; 

  rclcpp_action::Server<Detect>::SharedPtr action_server_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Subscription<vision_msgs::msg::Detection2DArray>::SharedPtr cameraSubscription_;
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr depthSubscription_;


  /* left is negative */
  float deltaXfromCenter(float centerX) {
	  return SCREENWIDTH/2 - centerX;
  }

  void camera_callback(const vision_msgs::msg::Detection2DArray::SharedPtr msg) {
	  if(!input_recieved) return;
    objFound = false; 
	  objX = -1;
	  objY = -1;
	  headOn = false;
	  for(const auto &detection : msg->detections) {
		  std::string detected_object = detection.results[0].hypothesis.class_id;

            if (detected_object == FlippedCocoLabelMap.at(reqObjectName)) {
              if(!objFoundOnce) {(this->get_logger(), "Requested object '%s' found!", reqObjectName.c_str());}
				reqObject = detection;
                objFound = true;
                objFoundOnce = true;
				auto bbox = detection.bbox;
				objX = bbox.center.position.x;
				objY = bbox.center.position.y;
				sizeX = bbox.size_x;
				sizeY = bbox.size_y;

				headOn = std::abs(deltaXfromCenter(objX))/SCREENWIDTH < 0.2;
                return;  // Exit loop once found
            }


	  }
	  
	  return;
  }
  

  uint8_t findAvgDepthBBOX(float size_x, float size_y, float objX, float objY, std::vector<uint8_t> depthData) {
	float avg = 0.0f;

	int start, end;
	start = objX - size_x/2 + SCREENWIDTH * (objY - size_y/2);
	end = objX + size_x/2 + SCREENWIDTH * (objY + size_y/2);

  if(start <= 0) start = 0;
  if(end >= SCREENHEIGHT*SCREENWIDTH) end = SCREENHEIGHT*SCREENWIDTH - 1;

	for(int i = start; i < end; i++) avg += depthData[i];

	avg /= (end-start);

	return avg;
  }

  
  void depth_callback(const sensor_msgs::msg::Image::SharedPtr msg) {
	  std::vector<uint8_t> data(msg->data.begin(), msg->data.begin() + 307200);


    imageX = msg->width;
    imageY = msg->height;
	  avgDepth_BB = findAvgDepthBBOX(sizeX, sizeY, objX, objY, data);

	  return;
  }
  
  rclcpp_action::GoalResponse handle_goal(
    const rclcpp_action::GoalUUID & uuid,
    std::shared_ptr<const Detect::Goal> goal)
  {
    RCLCPP_INFO(this->get_logger(), "Recieved client ping");
    if(COCO_ObjSet.find(goal->object_name) == COCO_ObjSet.end()) {
      RCLCPP_WARN(this->get_logger(), "Rejected request: '%s' is not a COCO dataset object.", goal->object_name.c_str());
              return rclcpp_action::GoalResponse::REJECT;
	}
	
    (void)uuid;
	RCLCPP_INFO(this->get_logger(), "Accepted request to track: '%s'", goal->object_name.c_str());
	reqObjectName = goal->object_name;
    input_recieved = true;
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  rclcpp_action::CancelResponse handle_cancel(
    const std::shared_ptr<GoalHandleDetect> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "Received request to cancel goal.");
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  void handle_accepted(const std::shared_ptr<GoalHandleDetect> goal_handle)
  {
    using namespace std::placeholders;
    // this needs to return quickly to avoid blocking the executor, so spin up a new thread
    std::thread{std::bind(&TrackObjectServer::execute, this, _1), goal_handle}.detach();
  }

  int open_robot_port(){
	const char* port = "/dev/ttyACM0";  // Change as needed
    int fd = open(port, O_RDWR | O_NOCTTY);

    if (fd < 0) {
        std::cerr << "Failed to open port: " << strerror(errno) << std::endl;
        return -1;
    }

    // Configure serial: 115200 baud
    termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        std::cerr << "tcgetattr error: " << strerror(errno) << std::endl;
        close(fd);
        return -1;
    }

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    
    // Configure 8N1
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    tty.c_cflag |= CREAD | CLOCAL;                  // Turn on READ and ignore modem ctrl lines
    tty.c_cflag &= ~(PARENB | PARODD);              // No parity
    tty.c_cflag &= ~CSTOPB;                         // 1 stop bit
    tty.c_cflag &= ~CRTSCTS;                        // No hardware flow control

    // Raw input/output mode
    tty.c_lflag = 0;                                // No signaling chars, no echo
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // No software flow control
    tty.c_oflag = 0;                                // No remapping, raw output

    tty.c_cc[VMIN]  = 1;                            // Read blocks until 1 char
    tty.c_cc[VTIME] = 1;                            // 0.1s read timeout

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error from tcsetattr: " << strerror(errno) << std::endl;
        close(fd);
        return -1;
    }
	return fd;
  }
  void execute(const std::shared_ptr<GoalHandleDetect> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "[LOG] Executing goal");
    const auto goal = goal_handle->get_goal();
    auto feedback = std::make_shared<Detect::Feedback>();
    auto & message = feedback->feedback;
    
    auto result = std::make_shared<Detect::Result>();
    auto move = geometry_msgs::msg::Twist();
	    
	rclcpp::Rate loop_rate(50);
	
	trackingState_t currState, nextState;
	
	int rfd = open_robot_port();
	bool complete = false;
	currState = T_WAIT;
	while(!complete) {
		
		/* State Transition Logic & Output Logic */
		switch (currState) {
			case T_WAIT:
      message = "Looking for Object";
				nextState = objFound ? T_LOOK : T_WAIT;
				stop_robot(rfd);
				break;
			case T_LOOK:
				/* rotate based on the location */
				spin_towards_obj(rfd);
        message = "Rotating";
				if(headOn && (avgDepth_BB < 20) && (avgDepth_BB != 0)) nextState = T_MOVE;
				break; 
			case T_MOVE:
				/* move forward */
				if(avgDepth_BB) move_forward(rfd);
        message = "Moving Forward";
				if(!headOn) nextState = T_LOOK;

				if((avgDepth_BB != 0) && (avgDepth_BB < 53) && headOn) nextState = T_STOP;
				break;
			case T_STOP:
				stop_robot(rfd);
        message = "I am done";
				complete = true;
				break;
		}
		currState = nextState;
    RCLCPP_INFO(this->get_logger(), "Looking for %s", reqObjectName.c_str());
    RCLCPP_INFO(this->get_logger(), "currState %d | nextState %d", currState, nextState);
    RCLCPP_INFO(this->get_logger(), "bbX : %f --- bbY: %f   depthinfo: %hhu", objX, objY, avgDepth_BB);
		/* Updating Client with Feedback */
		goal_handle->publish_feedback(feedback);

		loop_rate.sleep();
	}
		
	if (rclcpp::ok()) {
      result->result = "Mission Accomplished";
      goal_handle->succeed(result);
      RCLCPP_INFO(this->get_logger(), "Goal succeeded");
	  stop_robot(rfd);
	  close(rfd);
    }
	
  }

  void send_json_info_base(int fd, float x, float z){
	std::string json = "\n";
    ssize_t written = write(fd, json.c_str(), json.size());

    // Forming a JSON string with numberic value that can be passed in as agrument.
    json = R"({"T":13,"X":)" + std::to_string(x) + R"(,"Z":)" + std::to_string(z) + R"(})";
    json = json + "\n";

    // Send JSON string
    written = write(fd, json.c_str(), json.size());

	if (written < 0) {
        std::cerr << "Write failed: " << strerror(errno) << std::endl;
    } else {
        std::cout << "JSON sent to serial port.\n";
    }
	return;
  }
  //spins towards obj based on detected object
  void spin_towards_obj(int fd)
  {
        float x, z;
	x = 0.1f;
	z = 0.1f;	
	if(!objFound) return;
	int dist = deltaXfromCenter(objX);
	if(dist>0){send_json_info_base(fd, x, z); return;}
	send_json_info_base(fd, x, -1.0f * z);
	return;
  }

  //moves robot forward default vel
  //only call when head on
  void move_forward(int fd)
  {
	send_json_info_base(fd, 0.4, 0.0);
  }

  void stop_robot(int fd){
	send_json_info_base(fd, 0,0);
  }

  void get_depth(){

  }
};  // class TrackObjectServer

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto action_server = std::make_shared<TrackObjectServer>();
    
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(action_server);
  executor.spin();

  rclcpp::shutdown();
  return 0;
}
