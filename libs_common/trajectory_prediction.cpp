#include "trajectory_prediction.h"

#include <iostream>
#include <math.h>

TrajectoryPrediction::TrajectoryPrediction(MotionTensor &initial_conditions)
{
  this->initial_conditions = &initial_conditions;

  extremes = this->initial_conditions->get_extremes();
}

TrajectoryPrediction::~TrajectoryPrediction()
{

}


unsigned int TrajectoryPrediction::process( std::string network_file_name,
                                            TensorInterface &tensor_interface,
                                            unsigned int prediction_offset,
                                            std::vector<float> trajectory_modulo)
{
  Shape input_shape(tensor_interface.input_width(), tensor_interface.input_height(), tensor_interface.input_depth());
  Shape output_shape(tensor_interface.output_width(), tensor_interface.output_height(), tensor_interface.output_depth());

  CNN nn(network_file_name, input_shape, output_shape);

  nn.print();

  result.init(initial_conditions->width(), initial_conditions->height(), initial_conditions->depth());
  result.clear();

  auto extremes = initial_conditions->get_extremes();
  result.set_extremes(extremes);


  for (unsigned int y = 0; y < prediction_offset; y++)
  for (unsigned int z = 0; z < result.depth(); z++)
  for (unsigned int x = 0; x < result.width(); x++)
  {
    float v = initial_conditions->get(x, y, z);
    result.set(x, y, z, v);
  }



  unsigned int time_max = initial_conditions->height() - prediction_offset;

  for (unsigned int time_idx = 0; time_idx < time_max; time_idx++)
  {
      for (unsigned int particle_idx = 0; particle_idx < result.depth(); particle_idx++)
      {
        std::vector<float> res;

        res = predict(nn, tensor_interface, time_idx, particle_idx, prediction_offset);


        if (res.size() == 0)
        {
            res.resize(3);

          res[0] = result.get(0, time_idx + prediction_offset - 1, particle_idx);
          res[1] = result.get(1, time_idx + prediction_offset - 1, particle_idx);
          res[2] = result.get(2, time_idx + prediction_offset - 1, particle_idx);

          /*
        for (unsigned int k = 0; k < trajectory_modulo.size(); k++)
            res[k] = fmod(res[k], trajectory_modulo[k]);
            */

          result.set(0, time_idx + prediction_offset, particle_idx, res[0]);
          result.set(1, time_idx + prediction_offset, particle_idx, res[1]);
          result.set(2, time_idx + prediction_offset, particle_idx, res[2]);
       }

      }

      /*
      unsigned int particle_idx = 10;

      unsigned int width = initial_conditions->width()/2;

      float error = 0.0;
      float base  = 0.0000000001;


      for (unsigned int x = 0; x < width; x++)
      {
        error+= pow(initial_conditions->get(x, time_idx, particle_idx) - result.get(x, time_idx, particle_idx), 2.0);
        base+= pow(initial_conditions->get(x, time_idx, particle_idx), 2.0);
      }

      float relative_error = 100.0*sqrt(error/base);

      std::cout << time_idx << " > ";

      for (unsigned int x = 0; x < width; x++)
        std::cout << initial_conditions->get(x, time_idx, particle_idx) << " ";

      std::cout << " : ";


      for (unsigned int x = 0; x < width; x++)
        std::cout << result.get(x, time_idx, particle_idx) << " ";

      std::cout << " : " << relative_error << "% ";

      std::cout << "\n";
      */


      if ((time_idx%100) == 0)
      {
        float done = time_idx*100.0/time_max;
        std::cout << "done = " << done << "%\n";
      }

  }


  return result.height();

}

MotionTensor& TrajectoryPrediction::get_result()
{
  return result;
}





float TrajectoryPrediction::map_to(float source_min, float source_max, float dest_min, float dest_max, float x)
{
  float k = (dest_max - dest_min)/(source_max - source_min);
  float q = dest_max - k*source_max;

  return k*x + q;
}


std::vector<float> TrajectoryPrediction::predict( CNN &nn, TensorInterface &tensor_interface,
                                                  unsigned int time_idx,
                                                  unsigned int particle_idx,
                                                  unsigned int prediction_offset
                                                )
{
  std::vector<float> predict_result;

  int ti_res = tensor_interface.create(time_idx, particle_idx, result);

  if (ti_res != 0)
  {
      std::cout << "tensor interface returned " << ti_res << " at [" << time_idx << ", " << particle_idx << "]\n";
      return predict_result;
  }

  auto dataset_item = tensor_interface.get();

  unsigned int output_size = dataset_item.output.size();


  std::vector<float> nn_output(output_size);
  predict_result.resize(output_size);

  nn.forward(nn_output, dataset_item.input);

  for (unsigned int x = 0; x < output_size; x++)
  {
    float pos_norm = result.get(x, time_idx + prediction_offset-1, particle_idx);

    //float v_norm = initial_conditions->get(x + output_size, time_idx + prediction_offset, particle_idx);

    float v_norm   = nn_output[x];


    float v_orig, pos_orig;

    v_orig = map_to(0.0, 1.0,
                    extremes[output_size + x].min, extremes[output_size + x].max,
                    v_norm);

    pos_orig = map_to(0.0, 1.0,
                      extremes[x].min, extremes[x].max,
                      pos_norm);



    float predicted_orig = pos_orig + v_orig;

    float predicted = map_to(extremes[x].min, extremes[x].max, 0.0, 1.0, predicted_orig);

    result.set(x, time_idx + prediction_offset, particle_idx, predicted);
    result.set(x + output_size, time_idx + prediction_offset, particle_idx, v_norm);

    predict_result[x] = predicted;
  }

  return predict_result;
}
