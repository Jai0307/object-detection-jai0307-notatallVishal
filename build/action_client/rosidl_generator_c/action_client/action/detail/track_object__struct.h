// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from action_client:action/TrackObject.idl
// generated code does not contain a copyright notice

#ifndef ACTION_CLIENT__ACTION__DETAIL__TRACK_OBJECT__STRUCT_H_
#define ACTION_CLIENT__ACTION__DETAIL__TRACK_OBJECT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'object_name'
#include "rosidl_runtime_c/string.h"

/// Struct defined in action/TrackObject in the package action_client.
typedef struct action_client__action__TrackObject_Goal
{
  rosidl_runtime_c__String object_name;
} action_client__action__TrackObject_Goal;

// Struct for a sequence of action_client__action__TrackObject_Goal.
typedef struct action_client__action__TrackObject_Goal__Sequence
{
  action_client__action__TrackObject_Goal * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} action_client__action__TrackObject_Goal__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'status_message'
// already included above
// #include "rosidl_runtime_c/string.h"

/// Struct defined in action/TrackObject in the package action_client.
typedef struct action_client__action__TrackObject_Result
{
  rosidl_runtime_c__String status_message;
} action_client__action__TrackObject_Result;

// Struct for a sequence of action_client__action__TrackObject_Result.
typedef struct action_client__action__TrackObject_Result__Sequence
{
  action_client__action__TrackObject_Result * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} action_client__action__TrackObject_Result__Sequence;


// Constants defined in the message

/// Struct defined in action/TrackObject in the package action_client.
typedef struct action_client__action__TrackObject_Feedback
{
  bool success;
} action_client__action__TrackObject_Feedback;

// Struct for a sequence of action_client__action__TrackObject_Feedback.
typedef struct action_client__action__TrackObject_Feedback__Sequence
{
  action_client__action__TrackObject_Feedback * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} action_client__action__TrackObject_Feedback__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
#include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'goal'
#include "action_client/action/detail/track_object__struct.h"

/// Struct defined in action/TrackObject in the package action_client.
typedef struct action_client__action__TrackObject_SendGoal_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
  action_client__action__TrackObject_Goal goal;
} action_client__action__TrackObject_SendGoal_Request;

// Struct for a sequence of action_client__action__TrackObject_SendGoal_Request.
typedef struct action_client__action__TrackObject_SendGoal_Request__Sequence
{
  action_client__action__TrackObject_SendGoal_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} action_client__action__TrackObject_SendGoal_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in action/TrackObject in the package action_client.
typedef struct action_client__action__TrackObject_SendGoal_Response
{
  bool accepted;
  builtin_interfaces__msg__Time stamp;
} action_client__action__TrackObject_SendGoal_Response;

// Struct for a sequence of action_client__action__TrackObject_SendGoal_Response.
typedef struct action_client__action__TrackObject_SendGoal_Response__Sequence
{
  action_client__action__TrackObject_SendGoal_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} action_client__action__TrackObject_SendGoal_Response__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"

/// Struct defined in action/TrackObject in the package action_client.
typedef struct action_client__action__TrackObject_GetResult_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
} action_client__action__TrackObject_GetResult_Request;

// Struct for a sequence of action_client__action__TrackObject_GetResult_Request.
typedef struct action_client__action__TrackObject_GetResult_Request__Sequence
{
  action_client__action__TrackObject_GetResult_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} action_client__action__TrackObject_GetResult_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'result'
// already included above
// #include "action_client/action/detail/track_object__struct.h"

/// Struct defined in action/TrackObject in the package action_client.
typedef struct action_client__action__TrackObject_GetResult_Response
{
  int8_t status;
  action_client__action__TrackObject_Result result;
} action_client__action__TrackObject_GetResult_Response;

// Struct for a sequence of action_client__action__TrackObject_GetResult_Response.
typedef struct action_client__action__TrackObject_GetResult_Response__Sequence
{
  action_client__action__TrackObject_GetResult_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} action_client__action__TrackObject_GetResult_Response__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'feedback'
// already included above
// #include "action_client/action/detail/track_object__struct.h"

/// Struct defined in action/TrackObject in the package action_client.
typedef struct action_client__action__TrackObject_FeedbackMessage
{
  unique_identifier_msgs__msg__UUID goal_id;
  action_client__action__TrackObject_Feedback feedback;
} action_client__action__TrackObject_FeedbackMessage;

// Struct for a sequence of action_client__action__TrackObject_FeedbackMessage.
typedef struct action_client__action__TrackObject_FeedbackMessage__Sequence
{
  action_client__action__TrackObject_FeedbackMessage * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} action_client__action__TrackObject_FeedbackMessage__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ACTION_CLIENT__ACTION__DETAIL__TRACK_OBJECT__STRUCT_H_
