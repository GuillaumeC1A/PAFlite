#include <iostream>
#include <complex>
#include <vector>
#include <uhd.h>
#include <uhd/exception.hpp>
#include <uhd/types/time_spec.hpp>
#include <uhd/transport/udp_simple.hpp>
#include <uhd/types/tune_request.hpp>
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

using namespace std;

int main(int argc, char *argv[]) {

    cout << "\nHEllo World !\n";

    double freq(1000e6);
    double rate(3.84e6);
    double gain(2);
    double bw(3.84e6);
    int total_num_samps = 1000;


    rf device(rate, freq, gain, bw);
    uhd::usrp::multi_usrp::sptr usrp = device.usrp;

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
        size_t num_rx_samps = rx_stream->recv(&buff.front(), buff.size(), md);

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

        // send complex single precision floating point samples over udp
        for(int i = 0; i<buff.size(); i++){
           cout << buff[i];
        }

        num_acc_samps += num_rx_samps;
    }
    done_loop:

    // finished
    std::cout << std::endl << "Done!" << std::endl << std::endl;

    return EXIT_SUCCESS;
    
    

}