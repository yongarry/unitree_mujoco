from pathlib import Path

from ament_index_python.packages import get_package_prefix, get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess, OpaqueFunction
from launch.substitutions import LaunchConfiguration


def launch_setup(context, *args, **kwargs):
    robot = LaunchConfiguration("robot").perform(context)
    model_package = LaunchConfiguration("model_package").perform(context)
    model_file = Path(LaunchConfiguration("model_file").perform(context))

    if not model_file.is_absolute():
        model_file = Path(get_package_share_directory(model_package)) / model_file

    unitree_mujoco = (
        Path(get_package_prefix("unitree_mujoco"))
        / "lib"
        / "unitree_mujoco"
        / "unitree_mujoco"
    )

    return [
        ExecuteProcess(
            cmd=[
                str(unitree_mujoco),
                "-r",
                robot,
                "-s",
                str(model_file),
            ],
            output="screen",
        )
    ]


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            "robot",
            default_value="g1",
            description="Unitree robot model name.",
        ),
        DeclareLaunchArgument(
            "model_package",
            default_value="g1_description",
            description="ROS package containing the MuJoCo XML model file.",
        ),
        DeclareLaunchArgument(
            "model_file",
            default_value="xml/g1_29dof.xml",
            description=(
                "MuJoCo XML model file. Relative paths are resolved from "
                "model_package share; absolute paths are used as-is."
            ),
        ),
        OpaqueFunction(function=launch_setup),
    ])
