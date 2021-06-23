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
#include <iostream>
#include <complex>
#include <vector>
#include <uhd.h>
#include <uhd/exception.hpp>
#include <uhd/transport/udp_simple.hpp>
#include <uhd/types/tune_request.hpp>
#include <uhd/types/time_spec.hpp>
#include <uhd/types/device_addr.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/types/stream_cmd.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/utils/thread.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <chrono>
#include <complex>
#include <iostream>
#include <thread>
#include "rf.h"
#include <string>



rf::rf(double sample_rate, double center_frequency,
                                double gain,
                                double bandwidth) {

    

    std::string device_serial = "3167783";


    std::string device_args("serial=" + device_serial );
    device_args +=", master_clock_rate=30.72e6, recv_frame_size=7976, ";
    device_args+="send_frame_size=7976, num_recv_frames=256, num_send_frames=256";
    this->device_args = device_args;
    this->subdev = "A:A";
    this->ref = "internal";
    this->antenna_mode = "TX/RX";
    
    //this->rf_buff = rf_buff;

    this->sample_rate = sample_rate;
    this->center_frequency = center_frequency;
    this->gain = gain;
    this->bandwidth = bandwidth;

    this->usrp = uhd::usrp::multi_usrp::make(device_args);//device_args
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

std::vector<std::complex<float>> rf::start_receiving(int total_num_samps ) const {
    // create a receive streamer

    uhd::stream_args_t stream_args("fc32"); // complex floats
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);


    // setup streaming
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    stream_cmd.num_samps  = total_num_samps;
    stream_cmd.stream_now = true;
    rx_stream->issue_stream_cmd(stream_cmd);

    // loop until total number of samples reached
    size_t num_acc_samps = 0; // number of accumulated samples
    uhd::rx_metadata_t md;
    std::vector<std::complex<float>> buff(rx_stream->get_max_num_samps());


    while (num_acc_samps < total_num_samps) {
        size_t num_rx_samps = rx_stream->recv(&buff.front(), buff.size(), md, 1);

        // handle the error codes
        switch (md.error_code) {
            case uhd::rx_metadata_t::ERROR_CODE_NONE:
                break;

            case uhd::rx_metadata_t::ERROR_CODE_TIMEOUT:
                if (num_acc_samps == 0)
                    continue;
                std::cout << boost::format("Got timeout before all samples received, "
                                           "possible packet loss, exiting loop...")
                          << std::endl;
                goto done_loop;

            default:
                std::cout << boost::format("Got error code 0x%x, exiting loop...")
                             % md.error_code
                          << std::endl;
                goto done_loop;
        }




        num_acc_samps += num_rx_samps;
    }
    done_loop:

    // finished
    std::cout << std::endl << "Done!" << std::endl << std::endl;

    return buff;
}