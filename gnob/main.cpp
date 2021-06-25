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
#include <boost/thread.hpp>
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
    double rate(30.72e6);
    double gain(10);
    double bw(3.84e6);
    int total_num_samps = 256*5;
    uhd::time_spec_t start_time(double(3));


    rf device(rate, freq, gain, bw);
    uhd::usrp::multi_usrp::sptr usrp = device.usrp;

    usrp->set_time_next_pps(uhd::time_spec_t(0.0));


    cout << usrp->get_time_now().to_ticks(rate)
         <<endl;



    boost::thread recv_thread([total_num_samps, device=device, start_time] {
        vector<complex<float>> buff = device.start_receiving(total_num_samps, start_time);
        for(int i = 0; i<buff.size(); i++){
            cout << buff[i];
        }
    });

    vector<complex<float>> table(280, complex<float>(10,10));

    boost::thread transmit_thread([table, total_num_samps, device=device, start_time] {
        device.start_transmitting(table,total_num_samps, start_time);

    });

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    recv_thread.join();

    transmit_thread.join();


    
    

}