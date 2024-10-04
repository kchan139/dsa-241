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
    size_t batch_size;
    bool shuffle;
    bool drop_last;

    xt::xarray<size_t> indexes;
    size_t curr_idx;
    /*TODO: add more member variables to support the iteration*/
public:
    DataLoader(Dataset<DType, LType> *ptr_dataset,
               int batch_size,
               bool shuffle = true,
               bool drop_last = false)
    : ptr_dataset(ptr_dataset), batch_size(batch_size), shuffle(shuffle), drop_last(drop_last), curr_idx(0)
    {
        /*TODO: Add your code to do the initialization */
        int dataset_len = ptr_dataset->len();

        if (drop_last)
            dataset_len = (dataset_len / batch_size) * batch_size;

        
        indexes = xt::arange<size_t>(0, dataset_len);
        if (shuffle) 
        {
            xt::random::default_engine_type engine(0);
            xt::random::shuffle(indexes, engine);
            // std::default_random_engine engine(0);
            // std::shuffle(indexes.begin(), indexes.end(), engine);
        }
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
        return Iterator(this, this->indexes.size());
    }

    class Iterator
    {
    private:
        DataLoader *loader;
        size_t index;
        size_t indexes_size;

    public:
        Iterator(DataLoader *loader, int index) : loader(loader), index(index) {
            indexes_size = loader->indexes.size();
        }

        Iterator &operator++()
        {
            if ((indexes_size - index) / loader->batch_size >= 2)
                index += loader->batch_size;
            else index = indexes_size;

            return *this;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            loader = iterator.loader;
            index = iterator.index;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator!=(const Iterator &other) const
        {
            return index != other.index;
        }

        Batch<DType, LType> operator*() const
        {
            size_t end = (indexes_size - index) / loader->batch_size >= 2 ? 
                index + loader->batch_size : indexes_size;
            size_t get_size = end - index;

            xt::svector<size_t> data_shape = loader->ptr_dataset->get_data_shape();
            xt::svector<size_t> label_shape = loader->ptr_dataset->get_label_shape();

            data_shape[0] = get_size;
            label_shape[0] = get_size;

            xt::xarray<DType> data = xt::xarray<DType>::from_shape(data_shape);
            xt::xarray<LType> label = xt::xarray<LType>::from_shape(label_shape);
            bool has_label = label.dimension();
            size_t batch_index = 0;

            for (size_t i = index; i < end; i++)
            {
                auto data_label = loader->ptr_dataset->getitem(loader->indexes(i));
                xt::view(data, batch_index, xt::all()) = data_label.getData();
                if (has_label) 
                    xt::view(label, batch_index, xt::all()) = data_label.getLabel();

                batch_index++;
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