#include "DatasetVelocitySimple.h"

#include <iostream>

DatasetVelocitySimple::DatasetVelocitySimple(
                                              std::string training_dats_json_file_name,
                                              std::string testing_dats_json_file_name,
                                              std::string motion_tensor_file_name,
                                              unsigned int training_count,
                                              unsigned int testing_count
                                            )
{
    std::cout << "loading configs : \n";
    std::cout << training_dats_json_file_name << "\n";
    std::cout << testing_dats_json_file_name << "\n";
    std::cout << motion_tensor_file_name << "\n";

    training_motion_tensor.load(training_dats_json_file_name, motion_tensor_file_name);
    testing_motion_tensor.load(testing_dats_json_file_name, motion_tensor_file_name, training_motion_tensor.tensor());

    set_input_shape(Shape(1, 1, 3));
    set_output_shape(Shape(1, 1, 3));

    std::cout << "creating dataset\n";

    create(training_motion_tensor.tensor(), false, training_count);
    create(testing_motion_tensor.tensor(), true, testing_count);

    print();
}

DatasetVelocitySimple::~DatasetVelocitySimple()
{

}


void DatasetVelocitySimple::create(MotionTensor &tensor, bool put_to_testing, unsigned int count)
{
    std::vector<float> input, output;
    input.resize(3);
    output.resize(3);

    for (unsigned int z = 0; z < tensor.depth(); z++)
    for (unsigned int y = 0; y < tensor.height(); y++)
    {
        input[0] = tensor.get(0, y, z);
        input[1] = tensor.get(1, y, z);
        input[2] = tensor.get(2, y, z);

        output[0] = tensor.get(3, y, z);
        output[1] = tensor.get(4, y, z);
        output[2] = tensor.get(5, y, z);

        if (put_to_testing)
            add_testing(input, output);
        else
            add_training(input, output);
  }

}
