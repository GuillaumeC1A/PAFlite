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
    uhd::usrp::multi_usrp::sptr usrp1 = device.usrp;


    return EXIT_SUCCESS;
    
    

}