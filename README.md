NVENC 4.0 SDK Readme and Getting Started Guide

System Requirements

* NVIDIA Kepler based GPU - Refer to the NVIDIA NVENC developer site for GPUs that support NVENC
* NVIDIA drivers can be obtained from the website here:
  http://www.nvidia.com
* Windows: Driver version 340.52 or higher 
* Linux: Driver from R340 or higher
* CUDA 5.0 Toolkit

[Windows Configuration Requirements]
The following environment variables need to be set to build this sample application 
* For Windows
  - DXSDK_DIR: pointing to the DirectX SDK root directory
    You can download the latest SDK from Microsoft's DirectX website
* For Windows
  - The CUDA 5.0 Toolkit must also be installed (see below on how to get it)
  - CUDA toolkit required to enable CUDA interop support with NVENC hardware

[Linux Configuration Requirements]    
* For Linux
  - The CUDA 5.0 Toolkit must also be installed (see below on how to get it)
  - NVENC SDK 4.0 for Linux requires a CUDA 5.0 toolkit and R340 driver to build and run.
  - For the driver, please use a R340 driver with this package.
    Please refer to the CUDA 5.0 installation guides on for details on installing the toolkit

[Common to all OS platforms]
To download the CUDA 5.0 toolkit, you can get it here for your linux distribution.

http://developer.nvidia.com/cuda/cuda-toolkit

Getting started:

1) Windows Building and running the project.  Open the VS2008 solution file in this path.

      {your_installation_path}\Samples\nvEncodeApp\nvEncoder_vs2008.vcproj

   In VS2008, build the project, it will create an exectuable in
   
      {your_installation_path}\Samples\nvEncodeApp\nvEncoder.exe
   
   There are also included two included YUV source files and scripts so you can test the encoder application after it is built.
     
2) Linux Building and running the project.  Change to this folder in a terminal window

   cd {your_installation_path}
   make 

   To run the project:

   cd {your_installation_path}

      ./nvEncoder