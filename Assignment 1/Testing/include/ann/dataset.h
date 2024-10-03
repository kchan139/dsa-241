/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/*
 * File:   dataset.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 3:59 PM
 */

#ifndef DATASET_H
#define DATASET_H
#include "ann/xtensor_lib.h"
using namespace std;

template <typename DType, typename LType>
class DataLabel
{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;

public:
    DataLabel(xt::xarray<DType> data, xt::xarray<LType> label) : data(data), label(label) {}
    xt::xarray<DType> getData() const { return data; }
    xt::xarray<LType> getLabel() const { return label; }
};

template <typename DType, typename LType>
class Batch
{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;

public:
    Batch(xt::xarray<DType> data, xt::xarray<LType> label) : data(data), label(label) {}
    virtual ~Batch() {}
    xt::xarray<DType> &getData() { return data; }
    xt::xarray<LType> &getLabel() { return label; }
};

template <typename DType, typename LType>
class Dataset
{
private:
public:
    Dataset() {};
    virtual ~Dataset() {};

    virtual int len() = 0;
    virtual DataLabel<DType, LType> getitem(int index) = 0;
    virtual xt::svector<unsigned long> get_data_shape() = 0;
    virtual xt::svector<unsigned long> get_label_shape() = 0;
};

//////////////////////////////////////////////////////////////////////
template <typename DType, typename LType>
class TensorDataset : public Dataset<DType, LType>
{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
    xt::svector<unsigned long> data_shape, label_shape;

public:
    TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label)
    {
        this->data = data;
        this->label = label;
        this->data_shape = xt::svector<unsigned long>(this->data.shape().begin(), this->data.shape().end());
        if (this->label.size() > 0)
            this->label_shape = xt::svector<unsigned long>(this->label.shape().begin(), this->label.shape().end());
        else
            this->label_shape = xt::svector<unsigned long>();
    }

    int len()
    {
        return static_cast<int>(this->data_shape[0]);
    }

    DataLabel<DType, LType> getitem(int index)
    {
        if (index < 0 || index >= this->len())
            throw std::out_of_range("Index is out of range!");

        xt::xarray<DType> data_item;
        xt::xarray<LType> label_item;

        if (this->data.dimension() == 1)
            data_item = xt::view(this->data, index);
        else
            data_item = xt::view(this->data, index, xt::all());

        if (this->label.size() > 0) 
        {
            if (this->label.dimension() == 1)
                label_item = xt::view(this->label, index);
            else
                label_item = xt::view(this->label, index, xt::all());
        }

        return DataLabel<DType, LType>(data_item, label_item);
    }

    xt::svector<unsigned long> get_data_shape()
    {
        return this->data_shape;
    }

    xt::svector<unsigned long> get_label_shape()
    {
        return this->label_shape;
    }
};

#endif /* DATASET_H */

// TODO
// template <typename DType, typename LType>
// class ImageFolderDataset : Dataset<DType, LType>
// {
// private:

// public:
// };