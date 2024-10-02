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
    {
        this->ptr_dataset = ptr_dataset;
        this->batch_size = batch_size;
        this->shuffle = shuffle;
        this->drop_last = drop_last;
        this->indices = xt::arange<unsigned long>(0, ptr_dataset->len());
    }
    virtual ~DataLoader() {}

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

        Batch<DType, LType> operator*() const
        {
            size_t remaining = loader->indices.size() - index/loader->batch_size;
            size_t actual_batch_size = std::min(static_cast<size_t>(loader->batch_size), remaining);
            cout << "loader->indices.size(): " << loader->indices.size() << endl;
            cout << "index: " << index << endl;
            cout << "remaining: " << remaining << endl;
            cout << "actual_batch_size: " << actual_batch_size << endl;
            cout << "loader->batch_size: " << loader->batch_size << endl;
            // cout << "ptr_dataset->len(): " << loader->ptr_dataset->len() << endl;
            cout << endl;

            if (actual_batch_size == 0) {
                // Return an empty batch if we've reached the end
                return Batch<DType, LType>(xt::xarray<DType>(), xt::xarray<LType>());
            }

            if (loader->shuffle) {
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(loader->indices.begin(), loader->indices.end(), g);
            }

            xt::xarray<DType> batch_data;
            xt::xarray<LType> batch_label;

            for (size_t i = 0; i < actual_batch_size; ++i) {
                size_t idx = loader->indices[index + i];
                cout << "idx: " << idx << endl;
                auto item = loader->ptr_dataset->getitem(idx);

                if (i == 0) {
                    auto data_shape = item.getData().shape();
                    auto label_shape = item.getLabel().shape();

                    std::vector<size_t> batch_data_shape = {actual_batch_size};
                    batch_data_shape.insert(batch_data_shape.end(), data_shape.begin(), data_shape.end());

                    std::vector<size_t> batch_label_shape = {actual_batch_size};
                    batch_label_shape.insert(batch_label_shape.end(), label_shape.begin(), label_shape.end());

                    batch_data = xt::empty<DType>(batch_data_shape);
                    batch_label = xt::empty<LType>(batch_label_shape);
                }

                xt::view(batch_data, i) = item.getData();
                xt::view(batch_label, i) = item.getLabel();
            }

            return Batch<DType, LType>(batch_data, batch_label);
        }
    };
};

#endif /* DATALOADER_H */
