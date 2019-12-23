import json
import numpy
import tensor_load

def compute_axis_error(target_tensor, computed_tensor):
    error = target_tensor - computed_tensor

    eps = 10**-20

    max     = target_tensor.max()
    min     = target_tensor.min()
    mean    = error.mean()
    sigma   = numpy.std(error)

    rms             = numpy.sqrt(numpy.mean(numpy.square(error)))
    rms_relative    = 100.0*rms/(numpy.absolute(max - min) + eps)

    absolute        = numpy.mean(numpy.absolute(error))

    decimal_places  = 2
    mean            = round(mean, decimal_places)
    sigma           = round(sigma, decimal_places)
    absolute        = round(absolute, decimal_places)
    rms             = round(rms, decimal_places)
    rms_relative    = round(rms_relative, decimal_places)


    #return [rms, rms_relative]

    return [mean, sigma, absolute, rms, rms_relative]


def compute_errors(target_tensor, computed_tensor, verbose = False):

    json_result = {}

    result_x = compute_axis_error(target_tensor[0], computed_tensor[0])
    result_y = compute_axis_error(target_tensor[1], computed_tensor[1])
    result_z = compute_axis_error(target_tensor[2], computed_tensor[2])
    result_total = compute_axis_error(target_tensor, computed_tensor)

    if verbose:
        print(result_x[0], result_x[1], result_x[2], result_x[3], result_x[4])
        print(result_y[0], result_y[1], result_y[2], result_y[3], result_y[4])
        print(result_z[0], result_z[1], result_z[2], result_z[3], result_z[4])
        print(result_total[0], result_total[1], result_total[2], result_total[3], result_total[4], end=" ")
        print()

    json_result["x"] = {}
    json_result["x"]["mean"] = result_x[0]
    json_result["x"]["sigma"] = result_x[1]
    json_result["x"]["error_absolute"] = result_x[2]
    json_result["x"]["rms"] = result_x[3]
    json_result["x"]["rms_relative"] = result_x[4]

    json_result["y"] = {}
    json_result["y"]["mean"] = result_y[0]
    json_result["y"]["sigma"] = result_y[1]
    json_result["y"]["error_absolute"] = result_y[2]
    json_result["y"]["rms"] = result_y[3]
    json_result["y"]["rms_relative"] = result_y[4]

    json_result["z"] = {}
    json_result["z"]["mean"] = result_z[0]
    json_result["z"]["sigma"] = result_z[1]
    json_result["z"]["error_absolute"] = result_z[2]
    json_result["z"]["rms"] = result_z[3]
    json_result["z"]["rms_relative"] = result_z[4]

    json_result["total"] = {}
    json_result["total"]["mean"] = result_total[0]
    json_result["total"]["sigma"] = result_total[1]
    json_result["total"]["error_absolute"] = result_total[2]
    json_result["total"]["rms"] = result_total[3]
    json_result["total"]["rms_relative"] = result_total[4]

    return json_result


def compute_error_field(target_tensor, computed_tensor, decimation, file_name, reshaped):
    error = target_tensor - computed_tensor


    decimal_places = 3

    file = open(file_name, "w")
    if reshaped:

        dim   = len(error)
        time_steps  = len(error[0])
        cells_count   = len(error[0][0])

        #print(">>>>> ", dim, time_steps, cells_count)

        for time in range(0, time_steps):
                for cell in range(0, cells_count):
                    if time%decimation == 0:

                        err = 0.0
                        err+= error[0][time][cell]*error[0][time][cell]
                        err+= error[1][time][cell]*error[1][time][cell]
                        err+= error[2][time][cell]*error[2][time][cell]
                        err = err**0.5

                        result = ""
                        result+= str(round(target_tensor[0][time][cell], decimal_places)) + " "
                        result+= str(round(target_tensor[1][time][cell], decimal_places)) + " "
                        result+= str(round(target_tensor[2][time][cell], decimal_places)) + " "

                        result+= str(round(computed_tensor[0][time][cell], decimal_places)) + " "
                        result+= str(round(computed_tensor[1][time][cell], decimal_places)) + " "
                        result+= str(round(computed_tensor[2][time][cell], decimal_places)) + " "

                        result+= str(round(error[0][time][cell], decimal_places)) + " "
                        result+= str(round(error[1][time][cell], decimal_places)) + " "
                        result+= str(round(error[2][time][cell], decimal_places)) + " "

                        result+= str(round(err, decimal_places)) + "\n"

                        file.write(result);
