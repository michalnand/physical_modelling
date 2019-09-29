import sys
sys.path.append('..')
import libs_python.pyphy as pyphy


#convert dats files to json trajectory
dats_to_motion_tensor = pyphy.DatsToMotionTensor("testing_dats.json", "motion_tensor.json")

#save into "target_trajectory.json"
dats_to_motion_tensor.tensor().save_json("trajectory_result/target_trajectory.json")

#load trajectory json
target = pyphy.MotionTensor()
target.load_json("trajectory_result/target_trajectory.json")


#load trajectory result
result_net_0_single = pyphy.MotionTensor()
result_net_0_single.load_json("trajectory_result/net_1_single.json")

result_net_0_all = pyphy.MotionTensor()
result_net_0_all.load_json("trajectory_result/net_1_all.json")

#use only 200 points for render
visualisation = pyphy.MotionTensorVisualisation(200)

while True:
    visualisation.start()
    visualisation.render(target, 1.0, 0.0, 0.0)
    visualisation.render(result_net_0_single, 0.0, 0.0, 1.0)
    visualisation.render(result_net_0_all, 0.0, 1.0, 0.0)
    visualisation.finish()
