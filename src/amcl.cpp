#include <boost/shared_ptr.hpp>

// Signal handling
#include <signal.h>

// roscpp
#include "ros/ros.h"

#include "amcl/AmclNode.h"

boost::shared_ptr<AmclNode> mixmcl_node_ptr;

void sigintHandler(int sig)
{
  // Save latest pose as we're shutting down.
  mixmcl_node_ptr->savePoseToServer();
  ros::shutdown();
}

int
main(int argc, char** argv)
{
  ros::init(argc, argv, "mixmcl");
  ros::NodeHandle nh;

  // Override default sigint handler
  signal(SIGINT, sigintHandler);

  // Make our node available to sigintHandler
  mixmcl_node_ptr.reset(new AmclNode());

  if (argc == 1)
  {
    // run using ROS input
    ROS_INFO("ros::spin() in main");
    ros::spin();
  }
  else if ((argc >= 3) && (std::string(argv[1]) == "--run-from-bag"))
  {
    ROS_INFO("runFromBag in main");
    mixmcl_node_ptr->runFromBag(argv[2]);
  }
  else
  {
    ROS_INFO("something wrong in main");
    for( int i = 0; i < argc ; i++)
    {
      ROS_INFO("%d %s", i, argv[i]);
    }
  }
  // Without this, our boost locks are not shut down nicely
  mixmcl_node_ptr.reset();

  // To quote Morgan, Hooray!
  return(0);
}
