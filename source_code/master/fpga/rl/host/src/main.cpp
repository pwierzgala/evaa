#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CL/opencl.h"
#include "AOCLUtils/aocl_utils.h"

using namespace aocl_utils;

// OpenCL runtime configuration
cl_platform_id platform = NULL;
unsigned num_devices = 0;
scoped_array<cl_device_id> device; // num_devices elements
cl_context context = NULL;
scoped_array<cl_command_queue> queue; // num_devices elements
cl_program program = NULL;
scoped_array<cl_kernel> kernel; // num_devices elements
#if USE_SVM_API == 0
scoped_array<cl_mem> input_a_buf; // num_devices elements
scoped_array<cl_mem> input_b_buf; // num_devices elements
scoped_array<cl_mem> output_buf; // num_devices elements
#endif /* USE_SVM_API == 0 */

// Problem data.
unsigned N = 1000000; // problem size
scoped_array<scoped_SVM_aligned_ptr<float> > input_a, input_b; // num_devices elements
scoped_array<scoped_SVM_aligned_ptr<float> > output; // num_devices elements
scoped_array<scoped_array<float> > ref_output; // num_devices elements
scoped_array<unsigned> n_per_device; // num_devices elements

// Function prototypes
float rand_float();
bool init_opencl();
void init_problem();
void run();
void cleanup();

// Entry point.
int main(int argc, char **argv) {

  // Initialize OpenCL.
  if(!init_opencl()) {
    return -1;
  }

  // Initialize the problem data.
  // Requires the number of devices to be known.
  init_problem();

  // Run the kernel.
  run();

  // Free the resources allocated
  cleanup();

  return 0;
}

float rand_float() {
  return float(rand()) / float(RAND_MAX) * 20.0f - 10.0f;
}

// Initializes the OpenCL objects.
bool init_opencl() {
  cl_int status;

  printf("Initializing OpenCL\n");

  if(!setCwdToExeDir()) {
    return false;
  }

  // Get the OpenCL platform.
  platform = findPlatform("Intel(R) FPGA SDK for OpenCL(TM)");
  if(platform == NULL) {
    printf("ERROR: Unable to find Intel(R) FPGA OpenCL platform.\n");
    return false;
  }

  // Query the available OpenCL device.
  device.reset(getDevices(platform, CL_DEVICE_TYPE_ALL, &num_devices));
  printf("Platform: %s\n", getPlatformName(platform).c_str());
  printf("Using %d device(s)\n", num_devices);
  for(unsigned i = 0; i < num_devices; ++i) {
    printf("  %s\n", getDeviceName(device[i]).c_str());
  }

  // Create the context.
  context = clCreateContext(NULL, num_devices, device, &oclContextCallback, NULL, &status);
  checkError(status, "Failed to create context");

  // Create the program for all device. Use the first device as the
  // representative device (assuming all device are of the same type).
  std::string binary_file = getBoardBinaryFile("reinforecment_learning", device[0]);
  printf("Using AOCX: %s\n", binary_file.c_str());
  program = createProgramFromBinary(context, binary_file.c_str(), device, num_devices);

  // Build the program that was just created.
  status = clBuildProgram(program, 0, NULL, "", NULL, NULL);
  checkError(status, "Failed to build program");

  // Create per-device objects.
  queue.reset(num_devices);
  kernel.reset(num_devices);
  n_per_device.reset(num_devices);

  for(unsigned i = 0; i < num_devices; ++i) {
    // Command queue.
    queue[i] = clCreateCommandQueue(context, device[i], CL_QUEUE_PROFILING_ENABLE, &status);
    checkError(status, "Failed to create command queue");

    // Kernel.
    const char *kernel_name = "reinforecment_learning";
    kernel[i] = clCreateKernel(program, kernel_name, &status);
    checkError(status, "Failed to create kernel");

    // Determine the number of elements processed by this device.
    n_per_device[i] = N / num_devices; // number of elements handled by this device

    // Spread out the remainder of the elements over the first
    // N % num_devices.
    if(i < (N % num_devices)) {
      n_per_device[i]++;
    }

    cl_device_svm_capabilities caps = 0;

    status = clGetDeviceInfo(
      device[i],
      CL_DEVICE_SVM_CAPABILITIES,
      sizeof(cl_device_svm_capabilities),
      &caps,
      0
    );
    checkError(status, "Failed to get device info");


 }

  return true;
}

// Initialize the data for the problem. Requires num_devices to be known.
void init_problem() {
  if(num_devices == 0) {
    checkError(-1, "No devices");
  }

  input_a.reset(num_devices);
  input_b.reset(num_devices);
  output.reset(num_devices);
  ref_output.reset(num_devices);

  // Generate input vectors A and B and the reference output consisting
  // of a total of N elements.
  // We create separate arrays for each device so that each device has an
  // aligned buffer.
  for(unsigned i = 0; i < num_devices; ++i) {
    input_a[i].reset(context, n_per_device[i]);
    input_b[i].reset(context, n_per_device[i]);
    output[i].reset(context, n_per_device[i]);
    ref_output[i].reset(n_per_device[i]);

    cl_int status;

    status = clEnqueueSVMMap(queue[i], CL_TRUE, CL_MAP_WRITE,
        (void *)input_a[i], n_per_device[i] * sizeof(float), 0, NULL, NULL);
    checkError(status, "Failed to map input A");
    status = clEnqueueSVMMap(queue[i], CL_TRUE, CL_MAP_WRITE,
        (void *)input_b[i], n_per_device[i] * sizeof(float), 0, NULL, NULL);
    checkError(status, "Failed to map input B");

    for(unsigned j = 0; j < n_per_device[i]; ++j) {
      input_a[i][j] = rand_float();
      input_b[i][j] = rand_float();
      ref_output[i][j] = input_a[i][j] + input_b[i][j];
    }

    status = clEnqueueSVMUnmap(queue[i], (void *)input_a[i], 0, NULL, NULL);
    checkError(status, "Failed to unmap input A");
    status = clEnqueueSVMUnmap(queue[i], (void *)input_b[i], 0, NULL, NULL);
    checkError(status, "Failed to unmap input B");
  }
}

void run() {
  cl_int status;

  const double start_time = getCurrentTimestamp();

  // Launch the problem for each device.
  scoped_array<cl_event> kernel_event(num_devices);
  scoped_array<cl_event> finish_event(num_devices);

  for(unsigned i = 0; i < num_devices; ++i) {

    // Set kernel arguments.
    unsigned argi = 0;

    status = clSetKernelArgSVMPointer(kernel[i], argi++, (void*)input_a[i]);
    checkError(status, "Failed to set argument %d", argi - 1);

    status = clSetKernelArgSVMPointer(kernel[i], argi++, (void*)input_b[i]);
    checkError(status, "Failed to set argument %d", argi - 1);

    status = clSetKernelArgSVMPointer(kernel[i], argi++, (void*)output[i]);
    checkError(status, "Failed to set argument %d", argi - 1);
    const size_t global_work_size = n_per_device[i];
    printf("Launching for device %d (%zd elements)\n", i, global_work_size);

    status = clEnqueueNDRangeKernel(queue[i], kernel[i], 1, NULL,
        &global_work_size, NULL, 0, NULL, &kernel_event[i]);

    checkError(status, "Failed to launch kernel");

    status = clEnqueueSVMMap(queue[i], CL_TRUE, CL_MAP_READ,
        (void *)output[i], n_per_device[i] * sizeof(float), 0, NULL, NULL);
    checkError(status, "Failed to map output");
	clFinish(queue[i]);
  }

  // Wait for all devices to finish.
  clWaitForEvents(num_devices, finish_event);

  const double end_time = getCurrentTimestamp();

  // Wall-clock time taken.
  printf("\nTime: %0.3f ms\n", (end_time - start_time) * 1e3);

  // Get kernel times using the OpenCL event profiling API.
  for(unsigned i = 0; i < num_devices; ++i) {
    cl_ulong time_ns = getStartEndTime(kernel_event[i]);
    printf("Kernel time (device %d): %0.3f ms\n", i, double(time_ns) * 1e-6);
  }

  // Release all events.
  for(unsigned i = 0; i < num_devices; ++i) {
    clReleaseEvent(kernel_event[i]);
    clReleaseEvent(finish_event[i]);
  }

  // Verify results.
  bool pass = true;
  for(unsigned i = 0; i < num_devices && pass; ++i) {
    for(unsigned j = 0; j < n_per_device[i] && pass; ++j) {
      if(fabsf(output[i][j] - ref_output[i][j]) > 1.0e-5f) {
        printf("Failed verification @ device %d, index %d\nOutput: %f\nReference: %f\n",
            i, j, output[i][j], ref_output[i][j]);
        pass = false;
      }
    }
  }

  printf("\nVerification: %s\n", pass ? "PASS" : "FAIL");
}

// Free the resources allocated during initialization
void cleanup() {
  for(unsigned i = 0; i < num_devices; ++i) {
    if(kernel && kernel[i]) {
      clReleaseKernel(kernel[i]);
    }
    if(queue && queue[i]) {
      clReleaseCommandQueue(queue[i]);
    }
    if(input_a[i].get())
      input_a[i].reset();
    if(input_b[i].get())
      input_b[i].reset();
    if(output[i].get())
      output[i].reset();
  }

  if(program) {
    clReleaseProgram(program);
  }
  if(context) {
    clReleaseContext(context);
  }
}

