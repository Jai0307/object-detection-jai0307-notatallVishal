// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from action_client:action/TrackObject.idl
// generated code does not contain a copyright notice

#ifndef ACTION_CLIENT__ACTION__DETAIL__TRACK_OBJECT__BUILDER_HPP_
#define ACTION_CLIENT__ACTION__DETAIL__TRACK_OBJECT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "action_client/action/detail/track_object__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace action_client
{

namespace action
{

namespace builder
{

class Init_TrackObject_Goal_object_name
{
public:
  Init_TrackObject_Goal_object_name()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::action_client::action::TrackObject_Goal object_name(::action_client::action::TrackObject_Goal::_object_name_type arg)
  {
    msg_.object_name = std::move(arg);
    return std::move(msg_);
  }

private:
  ::action_client::action::TrackObject_Goal msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::action_client::action::TrackObject_Goal>()
{
  return action_client::action::builder::Init_TrackObject_Goal_object_name();
}

}  // namespace action_client


namespace action_client
{

namespace action
{

namespace builder
{

class Init_TrackObject_Result_status_message
{
public:
  Init_TrackObject_Result_status_message()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::action_client::action::TrackObject_Result status_message(::action_client::action::TrackObject_Result::_status_message_type arg)
  {
    msg_.status_message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::action_client::action::TrackObject_Result msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::action_client::action::TrackObject_Result>()
{
  return action_client::action::builder::Init_TrackObject_Result_status_message();
}

}  // namespace action_client


namespace action_client
{

namespace action
{

namespace builder
{

class Init_TrackObject_Feedback_success
{
public:
  Init_TrackObject_Feedback_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::action_client::action::TrackObject_Feedback success(::action_client::action::TrackObject_Feedback::_success_type arg)
  {
    msg_.success = std::move(arg);
    return std::move(msg_);
  }

private:
  ::action_client::action::TrackObject_Feedback msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::action_client::action::TrackObject_Feedback>()
{
  return action_client::action::builder::Init_TrackObject_Feedback_success();
}

}  // namespace action_client


namespace action_client
{

namespace action
{

namespace builder
{

class Init_TrackObject_SendGoal_Request_goal
{
public:
  explicit Init_TrackObject_SendGoal_Request_goal(::action_client::action::TrackObject_SendGoal_Request & msg)
  : msg_(msg)
  {}
  ::action_client::action::TrackObject_SendGoal_Request goal(::action_client::action::TrackObject_SendGoal_Request::_goal_type arg)
  {
    msg_.goal = std::move(arg);
    return std::move(msg_);
  }

private:
  ::action_client::action::TrackObject_SendGoal_Request msg_;
};

class Init_TrackObject_SendGoal_Request_goal_id
{
public:
  Init_TrackObject_SendGoal_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TrackObject_SendGoal_Request_goal goal_id(::action_client::action::TrackObject_SendGoal_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_TrackObject_SendGoal_Request_goal(msg_);
  }

private:
  ::action_client::action::TrackObject_SendGoal_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::action_client::action::TrackObject_SendGoal_Request>()
{
  return action_client::action::builder::Init_TrackObject_SendGoal_Request_goal_id();
}

}  // namespace action_client


namespace action_client
{

namespace action
{

namespace builder
{

class Init_TrackObject_SendGoal_Response_stamp
{
public:
  explicit Init_TrackObject_SendGoal_Response_stamp(::action_client::action::TrackObject_SendGoal_Response & msg)
  : msg_(msg)
  {}
  ::action_client::action::TrackObject_SendGoal_Response stamp(::action_client::action::TrackObject_SendGoal_Response::_stamp_type arg)
  {
    msg_.stamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::action_client::action::TrackObject_SendGoal_Response msg_;
};

class Init_TrackObject_SendGoal_Response_accepted
{
public:
  Init_TrackObject_SendGoal_Response_accepted()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TrackObject_SendGoal_Response_stamp accepted(::action_client::action::TrackObject_SendGoal_Response::_accepted_type arg)
  {
    msg_.accepted = std::move(arg);
    return Init_TrackObject_SendGoal_Response_stamp(msg_);
  }

private:
  ::action_client::action::TrackObject_SendGoal_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::action_client::action::TrackObject_SendGoal_Response>()
{
  return action_client::action::builder::Init_TrackObject_SendGoal_Response_accepted();
}

}  // namespace action_client


namespace action_client
{

namespace action
{

namespace builder
{

class Init_TrackObject_GetResult_Request_goal_id
{
public:
  Init_TrackObject_GetResult_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::action_client::action::TrackObject_GetResult_Request goal_id(::action_client::action::TrackObject_GetResult_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::action_client::action::TrackObject_GetResult_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::action_client::action::TrackObject_GetResult_Request>()
{
  return action_client::action::builder::Init_TrackObject_GetResult_Request_goal_id();
}

}  // namespace action_client


namespace action_client
{

namespace action
{

namespace builder
{

class Init_TrackObject_GetResult_Response_result
{
public:
  explicit Init_TrackObject_GetResult_Response_result(::action_client::action::TrackObject_GetResult_Response & msg)
  : msg_(msg)
  {}
  ::action_client::action::TrackObject_GetResult_Response result(::action_client::action::TrackObject_GetResult_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::action_client::action::TrackObject_GetResult_Response msg_;
};

class Init_TrackObject_GetResult_Response_status
{
public:
  Init_TrackObject_GetResult_Response_status()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TrackObject_GetResult_Response_result status(::action_client::action::TrackObject_GetResult_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return Init_TrackObject_GetResult_Response_result(msg_);
  }

private:
  ::action_client::action::TrackObject_GetResult_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::action_client::action::TrackObject_GetResult_Response>()
{
  return action_client::action::builder::Init_TrackObject_GetResult_Response_status();
}

}  // namespace action_client


namespace action_client
{

namespace action
{

namespace builder
{

class Init_TrackObject_FeedbackMessage_feedback
{
public:
  explicit Init_TrackObject_FeedbackMessage_feedback(::action_client::action::TrackObject_FeedbackMessage & msg)
  : msg_(msg)
  {}
  ::action_client::action::TrackObject_FeedbackMessage feedback(::action_client::action::TrackObject_FeedbackMessage::_feedback_type arg)
  {
    msg_.feedback = std::move(arg);
    return std::move(msg_);
  }

private:
  ::action_client::action::TrackObject_FeedbackMessage msg_;
};

class Init_TrackObject_FeedbackMessage_goal_id
{
public:
  Init_TrackObject_FeedbackMessage_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TrackObject_FeedbackMessage_feedback goal_id(::action_client::action::TrackObject_FeedbackMessage::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_TrackObject_FeedbackMessage_feedback(msg_);
  }

private:
  ::action_client::action::TrackObject_FeedbackMessage msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::action_client::action::TrackObject_FeedbackMessage>()
{
  return action_client::action::builder::Init_TrackObject_FeedbackMessage_goal_id();
}

}  // namespace action_client

#endif  // ACTION_CLIENT__ACTION__DETAIL__TRACK_OBJECT__BUILDER_HPP_
