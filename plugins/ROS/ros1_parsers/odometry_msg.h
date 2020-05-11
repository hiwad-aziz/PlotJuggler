#pragma once

#include <nav_msgs/Odometry.h>
#include "pose_msg.h"
#include "twist_msg.h"
#include "ros1_parser.h"



class OdometryMsgParser: public BuiltinMessageParser<nav_msgs::Odometry>
{
public:

    OdometryMsgParser(const std::string& topic_name, PlotDataMapRef& plot_data):
        BuiltinMessageParser<nav_msgs::Odometry>(topic_name, plot_data),
        _pose_parser(topic_name + "/pose", plot_data),
        _twist_parser(topic_name + "/twist", plot_data)
    {
      _data.push_back( &getSeries(plot_data, topic_name + "/header/stamp/sec"));
      _data.push_back( &getSeries(plot_data, topic_name + "/header/stamp/nanosec"));
    }

    void parseMessageImpl(const nav_msgs::Odometry& msg,
                          double timestamp) override
    {
        if( _use_header_stamp )
        {
            timestamp = double(msg.header.stamp.sec) +
                        double(msg.header.stamp.nsec)*1e-9;
        }

        _data[0]->pushBack( {timestamp, double(msg.header.stamp.sec)} );
        _data[1]->pushBack( {timestamp, double(msg.header.stamp.nsec)} );

        _pose_parser.parseMessageImpl( msg.pose, timestamp);
        _twist_parser.parseMessageImpl( msg.twist, timestamp);
    }

private:
    PoseCovarianceMsgParser  _pose_parser;
    TwistCovarianceMsgParser _twist_parser;
    std::vector<PlotData*> _data;
};
