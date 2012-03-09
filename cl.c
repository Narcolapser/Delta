/***********************************************************
 *Program:	OpenCL wrapper library for Epsilon
 *Programmer:	Toben "Littlefoot" Archer
 *Date:		10/31/11
 *Copy Right:	Creative Commons Attribution-Noncommercial-Share Alike 3.0 License.
 *Purpose:	Simply the ussage of openCl to my specific needs for epsilon.
 ***************/

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <stdio.h>


typedef struct CTEX{
	cl_platform_id platform_id;	    // compute platform ID
	cl_device_id device_id;		    // compute device ID
	cl_context context;		    // compute context
	cl_command_queue commands;	    // compute command queue
}cTex;

void initGPU(cTex* val)
{
	int err;
	int platsAvaliable;

	//get platform IDs
	cl_platform_id platform_id;	    // compute platform ID
	err = clGetPlatformIDs(1,&platform_id,&platsAvaliable);

	//get device Ids
	cl_device_id device_id;		    // compute device ID
	err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

	//instantiate the context
	cl_context context;		    // compute context
	context = clCreateContext(0,1, &device_id, NULL, NULL, &err);

	//instantiate the command queue
	cl_command_queue commands;	    // compute command queue
	commands = clCreateCommandQueue(context,device_id, 0, &err);

	//and make the struct!
	cTex temp = cTex{platform_id,device_id,context,commands};
	val = &temp;
}

int main()
{
	return(0);
}
