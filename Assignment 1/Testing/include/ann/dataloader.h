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

    xt::xarray<unsigned long> indices;

public:
    DataLoader(Dataset<DType, LType> *ptr_dataset,
            int batch_size,
            bool shuffle = true,
            bool drop_last = false)
    : ptr_dataset(ptr_dataset), batch_size(batch_size), shuffle(shuffle), drop_last(drop_last)
    {
        size_t dataset_size = ptr_dataset->len();
        this->indices = xt::arange<unsigned long>(0, dataset_size);
        if (shuffle) 
        {
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(this->indices.begin(), this->indices.end(), g);
        }
    }
    virtual ~DataLoader() {}

    Iterator begin()
    {
        return Iterator(this, 0);
    }

    Iterator end()
    {
        size_t dataset_size = ptr_dataset->len();
        size_t final_index;
        if (drop_last)
            final_index = (dataset_size / batch_size) * batch_size;
        else
            final_index = dataset_size;
        return Iterator(this, final_index);
    }

    class Iterator
    {
    private:
        size_t index;
        DataLoader *loader;
        size_t dataset_size;

    public:
        Iterator(DataLoader *loader, size_t index) : loader(loader), index(index) 
        {
            dataset_size = loader->ptr_dataset->len();
        }

        Iterator& operator++()
        {
            if (index < dataset_size) 
            {
                index += loader->batch_size;
                if (index > dataset_size && !loader->drop_last)
                    index = dataset_size;
            }
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
            size_t batch_start = index;
            size_t batch_end = std::min(index + loader->batch_size, dataset_size);
            size_t actual_batch_size = batch_end - batch_start;

            if (actual_batch_size == 0)
                throw std::out_of_range("");

            xt::xarray<DType> batch_data;
            xt::xarray<LType> batch_label;

            auto data_shape = loader->ptr_dataset->get_data_shape();
            auto label_shape = loader->ptr_dataset->get_label_shape();

            std::vector<size_t> batch_data_shape = {actual_batch_size};
            batch_data_shape.insert(batch_data_shape.end(), data_shape.begin() + 1, data_shape.end());

            batch_data.resize(batch_data_shape);

            for (size_t i = 0; i < actual_batch_size; ++i) 
            {
                size_t idx = loader->indices[batch_start + i];
                auto item = loader->ptr_dataset->getitem(idx);
                
                if (data_shape.size() > 1)
                    xt::view(batch_data, i) = item.getData();
                else
                    batch_data(i) = item.getData()(0);
            }

            if (!label_shape.empty()) 
            {
                std::vector<size_t> batch_label_shape = {actual_batch_size};
                batch_label_shape.insert(batch_label_shape.end(), label_shape.begin() + 1, label_shape.end());
                batch_label.resize(batch_label_shape);

                for (size_t i = 0; i < actual_batch_size; ++i) 
                {
                    size_t idx = loader->indices[batch_start + i];
                    auto item = loader->ptr_dataset->getitem(idx);
                    
                    if (label_shape.size() > 1)
                        xt::view(batch_label, i) = item.getLabel();
                    else
                        batch_label(i) = item.getLabel()(0);
                }
            }

            return Batch<DType, LType>(batch_data, batch_label);
        }
    };
};

#endif /* DATALOADER_H */
