/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/*
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"

using namespace std;

template <typename DType, typename LType>
class DataLoader
{
public:
    class Iterator;

private:
    Dataset<DType, LType> *ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;

    /*TODO: add more member variables to support the iteration*/
    xt::xarray<unsigned long> indices;

public:
    DataLoader(Dataset<DType, LType> *ptr_dataset,
               int batch_size,
               bool shuffle = true,
               bool drop_last = false)
    {
        /*TODO: Add your code to do the initialization */

        this->ptr_dataset = ptr_dataset;
        this->batch_size = batch_size;
        this->shuffle = shuffle;
        this->drop_last = drop_last;
        this->indices = xt::arange<unsigned long>(0, ptr_dataset->len());
    }
    virtual ~DataLoader() {}

    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////

    /*TODO: Add your code here to support iteration on batch*/

    Iterator begin()
    {
        return Iterator(this, 0);
    }

    Iterator end()
    {
        size_t final_index = drop_last ? (indices.size() / batch_size) * batch_size : indices.size();
        return Iterator(this, final_index);
    }

    class Iterator
    {
    private:
        size_t index;
        DataLoader *loader;

    public:
        Iterator(DataLoader *loader, size_t index) : loader(loader), index(index) {}

        Iterator &operator++()
        {
            index += loader->batch_size;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            index += loader->batch_size;
            return temp;
        }

        bool operator!=(const Iterator &other) const
        {
            return index != other.index;
        }

        auto operator*() const
        {
            xt::xarray<DType> data;
            xt::xarray<LType> label;

            size_t end = min(index + loader->batch_size, loader->indices.size());

            for (size_t i = index; i < end; ++i)
            {
                size_t dataset_index = loader->indices[i];
                DataLabel<DType, LType> data_label = loader->ptr_dataset->getitem(dataset_index);
                data = xt::concatenate(xt::xtuple(data, data_label.getData()), 0);
                label = xt::concatenate(xt::xtuple(label, data_label.getLabel()), 0);
            }

            return Batch<DType, LType>(data, label);
        }
    };

    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};

#endif /* DATALOADER_H */