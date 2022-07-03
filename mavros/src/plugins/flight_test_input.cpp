#include <mavros/mavros_plugin.h>
// #include <pluginlib/class_list_macros.h>
#include <pluginlib/class_list_macros.h>
#include <mavros_msgs/FlightTestInput.h>

namespace mavros {
namespace extra_plugins {


/**
 * @brief IMU data publication plugin
 */
class FlightTestInputPlugin : public plugin::PluginBase {
public:

	FlightTestInputPlugin() : PluginBase(),
		imu_nh("~flight_test_input"),
		has_flight_test_input(false)
	{ };

	void initialize(UAS &uas_)
	{
		PluginBase::initialize(uas_);

		flight_test_input_pub = imu_nh.advertise<mavros_msgs::FlightTestInput>("",10);

		// reset has_* flags on connection change
		enable_connection_cb();
	}

	Subscriptions get_subscriptions() {
		return {
           make_handler(&FlightTestInputPlugin::handle_flight_test_input),
		};
	}

private:
	ros::NodeHandle imu_nh;
	ros::Publisher flight_test_input_pub;

	bool has_flight_test_input;

	void handle_flight_test_input(const mavlink::mavlink_message_t *msg, mavlink::common::msg::FLIGHT_TEST_INPUT &flight_test)
	{

		ROS_INFO_COND_NAMED(!has_flight_test_input, "imu", "FlightTest: FlightTest ALPHA/OMEGA detected!");
		has_flight_test_input = true;

		ROS_INFO("FlightTest WAS HERE");

		mavros_msgs::FlightTestInput _msg_flight_test_input;
		// _msg_flight_test_input.fti_mode = flight_test.flight_test_alpha.fti_mode;

		_msg_flight_test_input.fti_mode = flight_test.fti_mode;
		_msg_flight_test_input.fti_state = flight_test.fti_state;
		
		_msg_flight_test_input.fti_sweep_time_segment_pct = flight_test.fti_sweep_time_segment_pct;
		_msg_flight_test_input.fti_sweep_frequency  = flight_test.fti_sweep_frequency ;
		_msg_flight_test_input.fti_sweep_amplitude  = flight_test.fti_sweep_amplitude ;
		
		_msg_flight_test_input.fti_injection_input  = flight_test.fti_injection_input ;
		_msg_flight_test_input.fti_injection_output  = flight_test.fti_injection_output ;
		_msg_flight_test_input.fti_raw_output  = flight_test.fti_raw_output ;
		_msg_flight_test_input.fti_injection_point  = flight_test.fti_injection_point ;

		flight_test_input_pub.publish(_msg_flight_test_input);

	}

	void connection_cb(bool connected) override
	{
    has_flight_test_input = false;
	}

};
}	// namespace std_plugins
}	// namespace mavros


PLUGINLIB_EXPORT_CLASS(mavros::extra_plugins::FlightTestInputPlugin, mavros::plugin::PluginBase)
// #include <pluginlib/class_list_macros.h>
// PLUGINLIB_EXPORT_CLASS(mavros::std_plugins::FlightTestInputPlugin, mavros::plugin::PluginBase)