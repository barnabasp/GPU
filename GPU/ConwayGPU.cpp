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

        /// Load program source
        std::ifstream source_file{ "C:\\Users\\220mp\\Documents\\ELTE_MSc\\GPU\\Project\\GPU\\funcs.cl" };
        if (!source_file.is_open())
            throw std::runtime_error{ std::string{ "Cannot open kernel source: " } + "funcs.cl" };
        
        /// Create program and kernel
        cl::Program program{ std::string{ std::istreambuf_iterator<char>{ source_file },
                             std::istreambuf_iterator<char>{} } };
        program.build({ device } );
        /*
        cl::Kernel applyRules(program, "applyRules");
        applyRules.setArg(0, buffer_A);
        applyRules.setArg(1, buffer_B);
        applyRules.setArg(2, buffer_C);
        queue.enqueueNDRangeKernel(applyRules,cl::NullRange,cl::NDRange(10),cl::NullRange);
        queue.finish();
        */
        //cl::Kernel kernel(program, "applyRules");
        auto saxpy = cl::KernelFunctor<cl_double, cl::Buffer, cl::Buffer>(program, "applyRules");

        // Init computation
        const std::size_t chainlength = std::size_t(std::pow(2u, 4u)); // 1M, cast denotes floating-to-integral conversion,
                                                                        //     promises no data is lost, silences compiler warning
        std::vector<cl_float> vec_x(chainlength),
                              vec_y(chainlength);
        cl_float a = 2.0;

        // Fill arrays with random values between 0 and 100
        auto prng = [engine = std::default_random_engine{},
                     distribution = std::uniform_real_distribution<cl_float>{ -100.0, 100.0 }]() mutable { return distribution(engine); };

        std::generate_n(std::begin(vec_x), chainlength, prng);
        std::generate_n(std::begin(vec_y), chainlength, prng);

        cl::Buffer buf_x{ std::begin(vec_x), std::end(vec_x), true },
                   buf_y{ std::begin(vec_y), std::end(vec_y), false };

        // Explicit (blocking) dispatch of data before launch
        cl::copy(queue, std::begin(vec_x), std::end(vec_x), buf_x);
        cl::copy(queue, std::begin(vec_y), std::end(vec_y), buf_y);

        // Launch kernels
        //queue.enqueueNDRangeKernel(saxpy, cl::NDRange{ chainlength }, a, buf_x, buf_y);
        saxpy(cl::EnqueueArgs{ queue, cl::NDRange{ chainlength } }, a, buf_x, buf_y);


        std::transform(
            vec_x.cbegin(),
            vec_x.cbegin(),
            vec_y.cbegin(),
            vec_x.begin(),
            [=](const cl_float& x, const cl_float& y){ return a * x + y; }
        );

        // (Blocking) fetch of results
        cl::copy(queue, buf_y, std::begin(vec_y), std::end(vec_y));

        // Validate (compute saxpy on host and match results)
        if (std::equal(vec_x.cbegin(), vec_x.cend(), vec_y.cbegin()))
            throw std::runtime_error{ "Validation failed." };
        cl::finish();
        /*
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
        */
    }    
    catch (cl::BuildError& error) // If kernel failed to build
    {
        std::cerr << error.what() << "(" << error.err() << ")" << std::endl;

        for (const auto& log : error.getBuildLog())
        {
            std::cerr <<
                "\tBuild log for device: " <<
                log.first.getInfo<CL_DEVICE_NAME>() <<
                std::endl << std::endl <<
                log.second <<
                std::endl << std::endl;
        }

        std::exit(error.err());
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