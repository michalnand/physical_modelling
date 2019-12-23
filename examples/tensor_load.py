import json
import numpy

class TensorLoad:

    def __init__(self, file_name, load_start_offset = 0, load_reshaped = False):
        with open(file_name) as f:
            #print("loading ", file_name)
            json_tensor = json.load(f)

        if load_reshaped:
            self.tensor = self.__json_to_tensor_reshaped(json_tensor, load_start_offset)
        else:
            self.tensor = self.__json_to_tensor(json_tensor, load_start_offset)

    def __json_to_tensor(self, json_tensor, load_start_offset):
        self.width   = json_tensor["width"]//2
        self.height  = json_tensor["height"] - load_start_offset
        self.depth   = json_tensor["depth"]

        result_tensor = numpy.zeros((self.depth, self.height, self.width))

        self.min_x = json_tensor["extremes"][0]["min"]
        self.max_x = json_tensor["extremes"][0]["max"]
        self.min_y = json_tensor["extremes"][1]["min"]
        self.max_y = json_tensor["extremes"][1]["max"]
        self.min_z = json_tensor["extremes"][2]["min"]
        self.max_z = json_tensor["extremes"][2]["max"]

        for i in range(0, self.depth):
            for j in range(0,self.height):
                x_raw = json_tensor["values"][i][j + load_start_offset][0]
                y_raw = json_tensor["values"][i][j + load_start_offset][1]
                z_raw = json_tensor["values"][i][j + load_start_offset][2]

                x = (self.max_x - self.min_x)*x_raw + self.min_x
                y = (self.max_y - self.min_y)*y_raw + self.min_y
                z = (self.max_z - self.min_z)*z_raw + self.min_z

                result_tensor[i][j][0] = x
                result_tensor[i][j][1] = y
                result_tensor[i][j][2] = z

        return result_tensor

    def __json_to_tensor_reshaped(self, json_tensor, load_start_offset):
        self.width   = json_tensor["width"]//2
        self.height  = json_tensor["height"] - load_start_offset
        self.depth   = json_tensor["depth"]

        result_tensor = numpy.zeros((self.width, self.height, self.depth))

        self.min_x = json_tensor["extremes"][0]["min"]
        self.max_x = json_tensor["extremes"][0]["max"]
        self.min_y = json_tensor["extremes"][1]["min"]
        self.max_y = json_tensor["extremes"][1]["max"]
        self.min_z = json_tensor["extremes"][2]["min"]
        self.max_z = json_tensor["extremes"][2]["max"]

        for i in range(0, self.depth):
            for j in range(0,self.height):
                x_raw = json_tensor["values"][i][j + load_start_offset][0]
                y_raw = json_tensor["values"][i][j + load_start_offset][1]
                z_raw = json_tensor["values"][i][j + load_start_offset][2]

                x = (self.max_x - self.min_x)*x_raw + self.min_x
                y = (self.max_y - self.min_y)*y_raw + self.min_y
                z = (self.max_z - self.min_z)*z_raw + self.min_z

                result_tensor[0][j][i] = x
                result_tensor[1][j][i] = y
                result_tensor[2][j][i] = z

        return result_tensor


    def get(self):
        return self.tensor

    def get_width(self):
        return self.width

    def get_height(self):
        return self.height

    def get_depth(self):
        return self.depth

    def print_info(self):
        #print some info

        print("width = ", self.width)        #6 axis : 3 for positions, 3 for velocity
        print("height = ", self.height)      #count of time steps
        print("depth = ", self.depth)        #count of cells in file
        print("\n")

        print("min x = ", self.min_x)
        print("max x = ", self.max_x)
        print("min y = ", self.min_y)
        print("max y = ", self.max_y)
        print("min z = ", self.min_z)
        print("max z = ", self.max_z)
        print("\n")

        max     = self.tensor.max()
        min     = self.tensor.min()
        mean    = self.tensor.mean()
        sigma   = numpy.std(self.tensor)

        print("mean   ", mean)
        print("min    ", min)
        print("max    ", max)
        print("sigma  ", sigma)
        print("\n")

        print("\n")


'''


#print trajectory of selected cell
def print_cell_trajectory(tensor, cell_idx):

    height = tensor["height"]

    for time in range(0, height):
        #print only each 100 step
        if (time%100) == 0:

            x = tensor["values"][cell_idx][time][0] #x axis on index 0
            y = tensor["values"][cell_idx][time][1] #y axis on index 1
            z = tensor["values"][cell_idx][time][2] #z axis on index 2

            print(time, " : ", x, y, z)



def load_tensor_from_file(file_name):
    with open(file_name) as f:
        target_tensor = json.load(f)

    tensor = json_to_tensor(target_tensor)

    return tensor

def print_tensor_info(tensor):
    max     = tensor.max()
    min     = tensor.min()
    mean    = tensor.mean()
    sigma   = numpy.std(tensor)

    print("mean   ", mean)
    print("min    ", min)
    print("max    ", max)
    print("sigma  ", sigma)
    print()
'''
