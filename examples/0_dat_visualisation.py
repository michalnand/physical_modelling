import sys
sys.path.append('..')
import libs_python.pyphy as pyphy

print("init done")

#convert dats files to json trajectory
dats_to_motion_tensor = pyphy.DatsToMotionTensor("testing_dats.json", "motion_tensor.json")

print("loading done")

#save into "target_trajectory.json"
dats_to_motion_tensor.tensor().save_json("trajectory_result/target_trajectory.json")

print("saving done")

#load trajectory json
target = pyphy.MotionTensor()
target.load_json("trajectory_result/target_trajectory.json")

#use only 200 points for render
visualisation = pyphy.MotionTensorVisualisation(200)

while True:
    visualisation.start()
    visualisation.render(target, 1.0, 0.0, 0.0)
    visualisation.finish()
