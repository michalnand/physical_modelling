#include "dats_load.h"
#include <json_config.h>
#include <iostream>

DatsLoad::DatsLoad()
{
  dat_count    = 0;
  column_count = 0;
  lines_count  = 0;
}

DatsLoad::DatsLoad(std::string json_config_file_name)
{
  load(json_config_file_name);
}


DatsLoad::DatsLoad(DatsLoad& other)
{
  copy(other);
}

DatsLoad::DatsLoad(const DatsLoad& other)
{
  copy(other);
}

DatsLoad::~DatsLoad()
{

}

DatsLoad& DatsLoad::operator= (DatsLoad& other)
{
  copy(other);

  return *this;
}

DatsLoad& DatsLoad::operator= (const DatsLoad& other)
{
  copy(other);

  return *this;
}

void DatsLoad::copy(DatsLoad& other)
{
  dat = other.dat;
}

void DatsLoad::copy(const DatsLoad& other)
{
  dat = other.dat;
}

void DatsLoad::load(std::string json_config_file_name)
{
    dat.clear();
    JsonConfig json(json_config_file_name);

    values_modulo.resize(json.result["values modulo"].size());
    for (unsigned int i = 0; i < values_modulo.size(); i++)
        values_modulo[i] = json.result["values modulo"][i].asFloat();

    for (unsigned int i = 0; i < json.result["dat files"].size(); i++)
    {
        std::string file_name = json.result["dat files"][i].asString();
        dat.push_back(DatLoad(file_name));

        if (values_modulo.size() != 0)
            dat[i].process_modulo(values_modulo);
    }

    compute_metric();
    find_extreme();
    print();
}


void DatsLoad::print()
{
  std::cout << "dat count " << get_dat_count() << "\n";
  std::cout << "columns count " << get_columns_count() << "\n";
  std::cout << "lines count " << get_lines_count() << "\n";

  std::cout << "min " << get_min() << "\n";
  std::cout << "max " << get_max() << "\n";

  std::cout << "min ";
  for (unsigned int i = 0; i < get_columns_count(); i++)
    std::cout << get_min_column(i) << " ";
  std::cout << "\n";

  std::cout << "max ";
  for (unsigned int i = 0; i < get_columns_count(); i++)
    std::cout << get_max_column(i) << " ";
  std::cout << "\n\n";
}

void DatsLoad::save(std::string file_name_prefix)
{
  for (unsigned int i = 0; i < dat.size(); i++)
  {
    std::string file_name = file_name_prefix + "_" + std::to_string(i) + ".log";
    dat[i].save(file_name);
  }
}

float DatsLoad::get(unsigned int dat_idx, unsigned int column_idx, unsigned int line_idx)
{
  return dat[dat_idx].get(column_idx, line_idx);
}

void DatsLoad::set(unsigned int dat_idx, unsigned int column_idx, unsigned int line_idx, float value)
{
  dat[dat_idx].set(column_idx, line_idx, value);
}

unsigned int DatsLoad::get_dat_count()
{
  return dat_count;
}

unsigned int DatsLoad::get_columns_count()
{
  return column_count;
}

unsigned int DatsLoad::get_lines_count()
{
  return lines_count;
}

float DatsLoad::get_max()
{
  return extremes.max;
}

float DatsLoad::get_max_column(unsigned int column)
{
  return extremes.max_column[column];
}

float DatsLoad::get_min()
{
  return extremes.min;
}

float DatsLoad::get_min_column(unsigned int column)
{
  return extremes.min_column[column];
}


void DatsLoad::compute_metric()
{
  dat_count = dat.size();

  if (dat_count == 0)
    return;

  column_count = dat[0].get_columns_count();
  lines_count = dat[0].get_lines_count();

  for (unsigned int i = 0; i < dat.size(); i++)
  {
    if (dat[i].get_columns_count() < column_count)
      column_count = dat[i].get_columns_count();

    if (dat[i].get_lines_count() < lines_count)
      lines_count = dat[i].get_lines_count();
  }
}

void DatsLoad::find_extreme()
{
  for (unsigned int i = 0; i < get_dat_count(); i++)
    dat[i].find_extreme();

  extremes.min = dat[0].get_min();
  extremes.max = dat[0].get_max();

  for (unsigned int i = 0; i < get_dat_count(); i++)
  {
    if (dat[i].get_min() < extremes.min)
      extremes.min = dat[i].get_min();

    if (dat[i].get_max() > extremes.max)
      extremes.max = dat[i].get_max();
  }

  extremes.max_column.resize(get_columns_count());
  extremes.min_column.resize(get_columns_count());

  for (unsigned int j = 0; j < get_columns_count(); j++)
  {
    extremes.min_column[j] = dat[0].get_min_column(j);
    extremes.max_column[j] = dat[0].get_max_column(j);
  }

  for (unsigned int j = 0; j < get_columns_count(); j++)
  for (unsigned int i = 0; i < get_dat_count(); i++)
  {
    if (dat[i].get_min_column(j) < extremes.min_column[j])
      extremes.min_column[j] = dat[i].get_min_column(j);

    if (dat[i].get_max_column(j) > extremes.max_column[j])
      extremes.max_column[j] = dat[i].get_max_column(j);
  }
}

void DatsLoad::normalise_column(float min, float max, bool find_own_extreme)
{
  std::cout << "normalising into " << min << " " << max << "\n";

  if (find_own_extreme)
    find_extreme();

  for (unsigned int j = 0; j < get_columns_count(); j++)
  {
    float k = 0.0;
    float q = 0.0;

    if (extremes.max_column[j] > extremes.min_column[j])
    {
      k = (max - min)/(extremes.max_column[j] - extremes.min_column[j]);
      q = max - k*extremes.max_column[j];
    }

    for (unsigned int i = 0; i < get_dat_count(); i++)
      dat[i].normalise_column_kq(j, k, q);
  }


  find_extreme();

  std::cout << "normalising done\n";
}

std::vector<float>& DatsLoad::get_values_modulo()
{
    return values_modulo;
}
