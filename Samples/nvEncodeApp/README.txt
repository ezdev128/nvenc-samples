Prerequisites on Windows to build and run sample application

1) Install latest DirectX SDK and NVIDIA CUDA Toolkit
2) Add system environment variables DXSDK_DIR and CUDA_PATH to point to directX SDK and CUDA Toolkit. Reboot required.

Prerequisites on Linux to build and run sample application

You will need to install NVIDIA CUDA toolkit (CUDA 5.0 Production) for Linux to build and run
this sample application.

Once the CUDA toolkit is installed, update environment variable PATH to
correctly point to a location where CUDA binaries are installed.
    e.g. export PATH=$PATH:/usr/local/cuda/bin

The application no longer requires a GUID to initialize the NVENC encoder, the
driver performs the validation.  NVENC is officially supported on GPU hardware
based on Quadro, Tesla, GeForce GRID, or VGX.  NVENC on GeForce is not officially
supported.

In order to build and run the application on linux, ensure the CUDA library
path is included in the environment variable LD_LIBRARY_PATH. e.g.

For 32-bit system:
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/lib
For 64-bit system:
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/lib64
