#include "DatasetTrajectoryRuntime.h"

#include <iostream>

DatasetTrajectoryRuntime::DatasetTrajectoryRuntime(
                                                    TensorInterface &training_tensor_interface,
                                                    TensorInterface &testing_tensor_interface,
                                                    unsigned int testing_count
                                                )
{
    set_input_shape(Shape(training_tensor_interface.input_width(), training_tensor_interface.input_height(), training_tensor_interface.input_depth()));

    this->training_tensor           = &training_tensor_interface;
    this->testing_tensor            = &testing_tensor_interface;
    this->training_count            = testing_tensor_interface.get_max_y_offset()*testing_tensor_interface.get_max_z_offset();
    this->testing_count             = testing_count;


    std::cout << "creating dataset\n";


    set_random_training_idx();
    get_testing_input(0);
    set_output_shape(Shape(1, 1, testing_item.output.size()));

    print();
}

DatasetTrajectoryRuntime::~DatasetTrajectoryRuntime()
{

}

unsigned int DatasetTrajectoryRuntime::get_training_count()
{
    return training_count;
}

unsigned int DatasetTrajectoryRuntime::get_testing_count()
{
    return testing_count;
}


void DatasetTrajectoryRuntime::set_random_training_idx()
{
    bool created = false;
    while (created != true)
    {
        unsigned int y_offset = rand()%training_tensor->get_max_y_offset();        //random time step
        unsigned int z_offset = rand()%training_tensor->get_max_z_offset();       //random cell

        if (training_tensor->create(y_offset, z_offset) == 0)
        {
            created = true;
        }
    }

    training_item = training_tensor->get();
}

std::vector<float>& DatasetTrajectoryRuntime::get_training_input()
{
    return training_item.input;
}

std::vector<float>& DatasetTrajectoryRuntime::get_training_output()
{
    return training_item.output;
}

std::vector<float>& DatasetTrajectoryRuntime::get_testing_input(unsigned int idx)
{
    bool created = false;

    while (created != true)
    {
        unsigned int y_offset = rand()%testing_tensor->get_max_y_offset();        //random time step
        unsigned int z_offset = rand()%testing_tensor->get_max_z_offset();       //random cell

        if (testing_tensor->create(y_offset, z_offset) == 0)
        {
            created = true;
        }
    }

    testing_item = testing_tensor->get();
    return testing_item.input;
}

std::vector<float>& DatasetTrajectoryRuntime::get_testing_output(unsigned int idx)
{
    return testing_item.output;
}
