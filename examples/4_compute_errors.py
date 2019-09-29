import sys
sys.path.append('..')

import numpy
import tensor_load
import libs_compute_errors


#program starts here :

load_start_offset   = 800
load_reshaped       = True
decimation = 100


#load target values -> ground truth (real trajectory)
print("loading target")
target_tensor = tensor_load.TensorLoad("trajectory_result/target_trajectory.json", load_start_offset, load_reshaped)
target_tensor.print_info()


print("loading prediction result")
experiment_tensor_net_0_single = tensor_load.TensorLoad("trajectory_result/net_0_single.json", load_start_offset, load_reshaped)
experiment_tensor_net_0_single.print_info()

print("loading prediction result")
experiment_tensor_net_0_all = tensor_load.TensorLoad("trajectory_result/net_0_all.json", load_start_offset, load_reshaped)
experiment_tensor_net_0_all.print_info()

print("loading prediction result")
experiment_tensor_net_1_single = tensor_load.TensorLoad("trajectory_result/net_1_single.json", load_start_offset, load_reshaped)
experiment_tensor_net_1_single.print_info()

print("loading prediction result")
experiment_tensor_net_1_all = tensor_load.TensorLoad("trajectory_result/net_1_all.json", load_start_offset, load_reshaped)
experiment_tensor_net_1_all.print_info()


print("computing errors")


errors = libs_compute_errors.compute_errors(target_tensor.get(), experiment_tensor_net_0_single.get())
print(errors["total"], "\n")

errors = libs_compute_errors.compute_errors(target_tensor.get(), experiment_tensor_net_0_all.get())
print(errors["total"], "\n")

errors = libs_compute_errors.compute_errors(target_tensor.get(), experiment_tensor_net_1_single.get())
print(errors["total"], "\n")

errors = libs_compute_errors.compute_errors(target_tensor.get(), experiment_tensor_net_1_all.get())
print(errors["total"], "\n")


print("program done")
