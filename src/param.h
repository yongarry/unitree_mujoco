#pragma once

#include <iostream>
#include <boost/program_options.hpp>
#include <filesystem>

namespace param
{

inline struct SimulationConfig
{
    std::string robot = "g1";
    std::filesystem::path robot_scene = "g1_29dof.xml";

    int domain_id = 0;
    std::string interface = "lo";

    int use_joystick = 0;
    std::string joystick_type = "xbox";
    std::string joystick_device = "/dev/input/js0";
    int joystick_bits = 16;

    int print_scene_information = 0;

    int enable_elastic_band = 0;
    int band_attached_link = 0;

    // MJCF <keyframe>: apply on each model load (-1 = off). Name overrides index when set.
    int initial_keyframe_index = 0;
    std::string initial_keyframe_name = "";
} config;

/* ---------- Command Line Parameters ---------- */
namespace po = boost::program_options;

//※ This function must be called at the beginning of main() function
inline po::variables_map helper(int argc, char** argv)
{
    po::options_description desc("Unitree Mujoco");
    desc.add_options()
        ("help,h", "Show help message")
        ("domain_id,i", po::value<int>(&config.domain_id), "DDS domain ID; -i 0")
        ("network,n", po::value<std::string>(&config.interface), "DDS network interface; -n eth0")
        ("robot,r", po::value<std::string>(&config.robot), "Robot type; -r g1")
        ("scene,s", po::value<std::filesystem::path>(&config.robot_scene), "MuJoCo XML model file; -s g1_29dof.xml")
        ("model_file", po::value<std::filesystem::path>(&config.robot_scene), "MuJoCo XML model file")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        exit(0);
    }

    return vm;
}

}