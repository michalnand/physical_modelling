#ifndef _DATASET_TRAJECTORY_RUNTIME_H_
#define _DATASET_TRAJECTORY_RUNTIME_H_

#include <tensor_interface.h>
#include <dataset_interface.h>

class DatasetTrajectoryRuntime: public DatasetInterface
{
    public:
        DatasetTrajectoryRuntime(
                                    TensorInterface &training_tensor_interface,
                                    TensorInterface &testing_tensor_interface,
                                    unsigned int testing_count
                                );

        virtual ~DatasetTrajectoryRuntime();

        unsigned int get_training_count();
        unsigned int get_testing_count();


        void set_random_training_idx();
        std::vector<float>& get_training_input();
        std::vector<float>& get_training_output();

        std::vector<float>& get_testing_input(unsigned int idx);
        std::vector<float>& get_testing_output(unsigned int idx);

    private:
        sDatasetItem training_item, testing_item;

        unsigned int training_count;
        unsigned int testing_count;
        TensorInterface *training_tensor;
        TensorInterface *testing_tensor;

};

#endif
