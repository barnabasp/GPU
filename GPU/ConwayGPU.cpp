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

void writeGrid(std::vector<int> grid, unsigned int gen, time_t genTime, unsigned int N, unsigned int M);
void dumpGrid(std::vector<int> grid, unsigned int gen, time_t genTime, unsigned int N, unsigned int M, bool print);

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
        auto conway = cl::KernelFunctor<cl::Buffer, cl::Buffer, cl_int, cl_int, cl_int>(program, "applyRules");

        // Init computation
        /*
        const std::size_t chainlength = std::size_t(std::pow(2u, 4u)); // 1M, cast denotes floating-to-integral conversion,
                                                                        //     promises no data is lost, silences compiler warning
        std::vector<cl_float> vec_x(chainlength),
                              vec_y(chainlength);
        cl_float a = 2.0;
        */
        // Fill arrays with random values between 0 and 100
        /*
        auto prng = [engine = std::default_random_engine{},
                     distribution = std::uniform_real_distribution<cl_float>{ -100.0, 100.0 }]() mutable { return distribution(engine); };
        */
        unsigned int N = 1e3;
        unsigned int M = 1e3;
        unsigned int rule = 2;
        std::array<std::vector<cl_int>, 2> grids;
        unsigned int generation = 10;

        grids[0].resize(N * M);
        grids[1].resize(N * M);
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dis(0.0, 1.0);
        for (unsigned int i = 0; i < (N * M); i++)
        {
                grids[0][i] = (dis(gen) > 0.5 ? 0 : 1);
                //grids[0][i] = 0;
        }
        //blinker oscillator
        /*
        grids[0][2 + M * 2] = 1;
        grids[0][2 + M * 3] = 1;
        grids[0][2 + M * 4] = 1;
        */
        //glider spaceship:
        /*
        grids[0][2 + M*2] = 1;
        grids[0][3 + M*3] = 1;
        grids[0][1 + M*4] = 1;
        grids[0][2 + M*4] = 1;
        grids[0][3 + M*4] = 1;
        */
        //container for time measurement
        //not necessarily needed a vector, but it's stored always
        std::vector<time_t> timeMeasure;
        
        std::cout << "Executing game of life with rule: " << rule << " on a grid " << N << "x" << M << std::endl;
        //loop over the generations
        for(unsigned int gen = 0; gen < generation; gen++)
        {
            //create a buffer with the grids
            cl::Buffer buf_x{ std::begin(grids[0]), std::end(grids[0]), true },
                       buf_y{ std::begin(grids[1]), std::end(grids[1]), false };

            // Explicit (blocking) dispatch of data before launch
            cl::copy(queue, std::begin(grids[0]), std::end(grids[0]), buf_x);
            cl::copy(queue, std::begin(grids[1]), std::end(grids[1]), buf_y);

            // Launch time measurement
            auto t0 = std::chrono::high_resolution_clock::now();
            //launch the kernel with the given arguments
            //the dimension is set to N,M and we are treating it as a 1D vector
            conway(cl::EnqueueArgs{ queue, cl::NDRange{ N,M } }, buf_x, buf_y, N, M, rule);
            auto t1 = std::chrono::high_resolution_clock::now();
            timeMeasure.push_back(std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count());


            // (Blocking) fetch of results
            cl::copy(queue, buf_y, std::begin(grids[0]), std::end(grids[0])); //basically copy the buffer for the next status to the current status;
            //grids[0] = grids[1]; 
            dumpGrid(grids[0], gen, timeMeasure[gen], N, M, false);
            std::cout << '\n';
        }

        cl::finish();
    }    
    //+-+-+-+-+-+-+-+-+-
    //ERROR HANDLING
    //+-+-+-+-+-+-+-+-+-
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
//+-+-+-+-+-+-+-+-+-
//Functions to write out and dump the current status
//+-+-+-+-+-+-+-+-+-
void writeGrid(std::vector<int> grid, unsigned int gen, time_t genTime, unsigned int N, unsigned int M)
{
    //seting up the string name for the file
    std::stringstream fileName(""); fileName << "C:\\Users\\220mp\\Documents\\ELTE_MSc\\GPU\\Project\\GPU\\outputs\\" << "-"<< N << "x" << N << "-gen_" << gen  << ".txt";
    std::ofstream file(fileName.str().c_str());
    //header
    file << "#T " << genTime << std::endl;
    //body
    unsigned int idx;
    for (unsigned int i_row = 0; i_row < N; i_row++)
    {
        for (unsigned int i_col = 0; i_col < M; i_col++)
        {
            idx = i_row * M + i_col;
            if (grid[idx] == 1)
                file << "+,";
            else
                file << "-,";
        }
        file << "\n";
    }
    file.close();

}
void dumpGrid(std::vector<int> grid, unsigned int gen, time_t genTime, unsigned int N, unsigned int M, bool print) //dumps the status inserted to console
{
    unsigned int idx;
    std::cout << "gen: " << gen << "\ttime in us: " << genTime << std::endl;
    if(print)
    {
        for (unsigned int i_row = 0; i_row < N; i_row++)
        {
            for (unsigned int i_col = 0; i_col < M; i_col++)
            {
                idx = i_row * M + i_col;
                std::cout << grid[idx];
            }
            std::cout << "\n";
        }
    }
}