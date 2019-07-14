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
    this->testing_count             = testing_count;

    std::cout << "creating dataset\n";

    /*
    if (testing_count == 0)
        create_testing_all(testing_tensor_interface);
    else
        create_testing(testing_tensor_interface, testing_count);
    */

    create_testing(testing_tensor_interface, 10);

    training_count = testing_tensor_interface.get_max_y_offset()*testing_tensor_interface.get_max_z_offset();

    auto item = get_random_training();


    set_output_shape(Shape(1, 1, item.output.size()));

    /*
    training.resize(1);
    print();
    */

    print();
}

DatasetTrajectoryRuntime::~DatasetTrajectoryRuntime()
{

}

unsigned int DatasetTrajectoryRuntime::get_training_count()
{
  return training_count;
}

sDatasetItem DatasetTrajectoryRuntime::get_random_training()
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

    return training_tensor->get();
}

void DatasetTrajectoryRuntime::create_testing(TensorInterface &tensor, unsigned int count)
{
    while (count > 0)
    {
        unsigned int y_offset = rand()%tensor.get_max_y_offset();   //random time step
        unsigned int z_offset = rand()%tensor.get_max_z_offset();   //random cell

        if (tensor.create(y_offset, z_offset) == 0)
        {
            auto item = tensor.get();

            add_testing(item.input, item.output);
            count--;
        }
    }
}


void DatasetTrajectoryRuntime::create_testing_all(TensorInterface &tensor)
{
    for (unsigned int y_offset = 0; y_offset < tensor.get_max_y_offset(); y_offset++)
        for (unsigned int z_offset = 0; z_offset < tensor.get_max_z_offset(); z_offset++)
            if (tensor.create(y_offset, z_offset) == 0)
            {
                auto item = tensor.get();
                add_testing(item.input, item.output);
            }
}

unsigned int DatasetTrajectoryRuntime::get_testing_count()
{
    return testing_count;
}

sDatasetItem DatasetTrajectoryRuntime::get_testing(unsigned int idx)
{
    (void)idx;

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

    return testing_tensor->get();
}
