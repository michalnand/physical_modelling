import sys
sys.path.append('..')
import libs_python.pyphy as pyphy

dats_to_motion_tensor = pyphy.DatsToMotionTensor("testing_dats.json", "motion_tensor.json")
#dats_to_motion_tensor.tensor().save_json("trajectory_result/target_trajectory.json")

prediction_offset = 800

def process_trajectory(tensor_config, network_config, result_file_name):

    print("processing")
    print("tensor config : ", tensor_config)
    print("network config : ", network_config)
    print("result file name : ", result_file_name)

    tensor_interface = pyphy.TensorSpatial(tensor_config, dats_to_motion_tensor.tensor())

    prediction = pyphy.TrajectoryPrediction(dats_to_motion_tensor.tensor())

    prediction.process( network_config, tensor_interface, prediction_offset)
    prediction.get_result().save_json(result_file_name)



process_trajectory("spatial_tensor_single.json", "networks/net_0_single/trained/network_config.json", "trajectory_result/net_0_single.json")
process_trajectory("spatial_tensor_all.json", "networks/net_0_all/trained/network_config.json", "trajectory_result/net_0_all.json")
process_trajectory("spatial_tensor_single.json", "networks/net_1_single/trained/network_config.json", "trajectory_result/net_1_single.json")
process_trajectory("spatial_tensor_all.json", "networks/net_1_all/trained/network_config.json", "trajectory_result/net_1_all.json")


print("program done")
