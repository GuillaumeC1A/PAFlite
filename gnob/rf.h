#ifndef FREE5GRAN_RF_H
#define FREE5GRAN_RF_H

#include <string>
#include <uhd.h>
#include <uhd/usrp/multi_usrp.hpp>
#include <complex>
#include <mutex>

class rf {
    /*
     * rf class implements the exchanges between NRPhy and USRP based on UHD lib (https://files.ettus.com/manual/index.html).
     */

private:
    double sample_rate;
    double center_frequency;
    double gain;
    double bandwidth;
    std::string device_args;
    std::string subdev;
    std::string antenna_mode;
    std::string ref;
    
public:
    rf(
            double sample_rate, double center_frequency,
                                double gain,
                                double bandwidth);
    
    uhd::usrp::multi_usrp::sptr usrp;
    std::vector<std::complex<float>> start_receiving(int total_num_samps) const;


};


#endif