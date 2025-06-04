
 #include <pluginlib/class_list_macros.hpp>

 #include <point_cloud_transport/publisher_plugin.hpp>
 #include <point_cloud_transport/subscriber_plugin.hpp>
 
 #include <diane_point_cloud_transport/diane_publisher.hpp>
 #include <diane_point_cloud_transport/diane_subscriber.hpp>
 
 PLUGINLIB_EXPORT_CLASS(
   diane_point_cloud_transport::DianePublisher,
   point_cloud_transport::PublisherPlugin)
 PLUGINLIB_EXPORT_CLASS(
   diane_point_cloud_transport::DianeSubscriber,
   point_cloud_transport::SubscriberPlugin)
 