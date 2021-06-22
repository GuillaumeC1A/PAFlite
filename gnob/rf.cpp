#include "rf.h"
#include <uhd.h>
#include <boost/format.hpp>
#include <iostream>
#include <complex>
#include <vector>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <mutex>
#include <unistd.h>


rf::rf(double sample_rate, double center_frequency, double gain, double bandwidth,
               std::string subdev, std::string antenna_mode, std::string ref, std::string device_args) {

    //uhd::set_thread_priority_safe();

    this->sample_rate = sample_rate;
    this->center_frequency = center_frequency;
    this->gain = gain;
    this->bandwidth = bandwidth;
    this->device_args = device_args;
    this->subdev = subdev;
    this->antenna_mode = antenna_mode;
    this->ref = ref;

    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(device_args);
    this->usrp = usrp;
    this->usrp->set_clock_source(this->ref);
    this->usrp->set_rx_subdev_spec(this->subdev );
    this->usrp->set_rx_rate(this->sample_rate);
    uhd::tune_request_t tune_request(this->center_frequency);
    this->usrp->set_rx_freq(tune_request);
    this->usrp->set_rx_gain(this->gain);
    this->usrp->set_rx_bandwidth(this->bandwidth);
    this->usrp->set_rx_antenna(this->antenna_mode);

    this->usrp->set_tx_subdev_spec(this->subdev );
    this->usrp->set_tx_rate(this->sample_rate);
    this->usrp->set_tx_freq(tune_request);
    this->usrp->set_tx_gain(this->gain);
    this->usrp->set_tx_bandwidth(this->bandwidth);
    this->usrp->set_tx_antenna(this->antenna_mode);
}