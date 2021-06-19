#include <CL/cl2.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <array>
#include <numeric>
//parallelisation
#include <thread>
#include <mutex>
#include <atomic>
//time
#include <chrono>

int main()
{
    try
    {
        /// GPU usual inits: queue, device, platform, context
        cl::CommandQueue queue = cl::CommandQueue::getDefault();
        cl::Device device = queue.getInfo<CL_QUEUE_DEVICE>();
        cl::Context context = queue.getInfo<CL_QUEUE_CONTEXT>();
        cl::Platform platform{device.getInfo<CL_DEVICE_PLATFORM>()};

        /// Load kernel source file
        std::ifstream source_file{ "C:\\Users\\220mp\\Documents\\ELTE_MSc\\GPU\\Project\\GPU\\funcs.cl" };
        if (!source_file.is_open())
            throw std::runtime_error{ std::string{ "Cannot open kernel source: " } + "funcs.cl" };
        
        /// Create cl::Program from kernel and build it for the device
        cl::Program program{ std::string{ std::istreambuf_iterator<char>{ source_file },
                             std::istreambuf_iterator<char>{} } };
        program.build({ device });

        size_t N = 6;
        size_t M = 6;

        std::array<std::vector<cl_int>, 2> grids;
        unsigned int generation = 10;

        grids[0].resize(N * M);
        grids[1].resize(N * M);
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dis(0.0, 1.0);
        for (int i = 0; i < (N * M); i++)
        {
                grids[0][i] = (dis(gen) > 0.5 ? 0 : 1);
                //grids[0][i] = 0;
        }
    }    
    catch (cl::Error& error) // If any OpenCL error occurs
    {
        std::cerr << error.what() << "(" << error.err() << ")" << std::endl;
        std::exit(error.err());
    }
    catch (std::exception& error) // If STL/CRT error occurs
    {
        std::cerr << error.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}