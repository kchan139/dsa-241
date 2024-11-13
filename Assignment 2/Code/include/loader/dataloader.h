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
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"

using namespace std;

template<typename DType, typename LType>
class DataLoader{
public:
    class Iterator; //forward declaration for class Iterator
    
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int nbatch;
    ulong_tensor item_indices;
    int m_seed;
    
public:
    DataLoader(Dataset<DType, LType>* ptr_dataset, 
            int batch_size, bool shuffle=true, 
            bool drop_last=false, int seed=-1)
                : ptr_dataset(ptr_dataset), 
                batch_size(batch_size), 
                shuffle(shuffle),
                m_seed(seed){
            nbatch = ptr_dataset->len()/batch_size;
            item_indices = xt::arange(0, ptr_dataset->len());
            if(shuffle)
            {
                if(seed >= 0)
                {
                    xt::random::seed(seed);
                    xt::random::shuffle(item_indices);
                }
            else
                {
                    xt::random::shuffle(item_indices);
                }
            }
    }
    virtual ~DataLoader(){}
    
    //New method: from V2: begin
    int get_batch_size(){ return batch_size; }
    int get_sample_count(){ return ptr_dataset->len(); }
    int get_total_batch(){return int(ptr_dataset->len()/batch_size); }
    
    //New method: from V2: end
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
public:
    Iterator begin(){
        //YOUR CODE IS HERE
        return Iterator(this, true);
    }
    Iterator end(){
        //YOUR CODE IS HERE
        return Iterator(this, false);
    }
    
    //BEGIN of Iterator

    //YOUR CODE IS HERE: to define iterator
    class Iterator{
    private:
        DataLoader<DType, LType>* ptr_loader;
        int batch_index;
    public:
        Iterator(DataLoader<DType, LType>* Data = nullptr, bool begin = true) 
        : ptr_loader(Data), batch_index(!begin && ptr_loader ?
                                         ptr_loader->nbatch :
                                        0)
        {};

        Iterator &operator=(const Iterator &iterator)
        {
            this->ptr_loader = iterator.ptr_loader;
            this->batch_index = iterator.batch_index;
            return *this;
        }

        Batch<DType, LType> operator*() const
        {
            int start = batch_index * ptr_loader->batch_size;
            int end = start + ptr_loader->batch_size;

            int dataset_len = ptr_loader->ptr_dataset->len();

            if(batch_index == ptr_loader->nbatch - 1 &&
               !ptr_loader->drop_last                       ) 
                end = dataset_len;

            int actual_batch_size = end - start;

            auto data_shape = ptr_loader->ptr_dataset->get_data_shape();
            auto label_shape = ptr_loader->ptr_dataset->get_label_shape();
            data_shape[0] = actual_batch_size;

            xt::xarray<DType> batch_data = xt::xarray<DType>::from_shape(data_shape);
            xt::xarray<LType> batch_label = xt::xarray<LType>::from_shape(label_shape.empty() ?
                                            xt::dynamic_shape<unsigned long>{0} : label_shape);

            if (!label_shape.empty()) 
            {
                label_shape[0] = actual_batch_size;
                batch_label = xt::xarray<LType>::from_shape(label_shape);
            }
            

            for(int i = 0; i < actual_batch_size; i++) 
            {
                
                DataLabel<DType, LType> item = 
                ptr_loader->ptr_dataset->getitem(ptr_loader->item_indices[start + i]);
                
                xt::view(batch_data, i) = item.getData();
                
                if (!label_shape.empty()) 
                {
                    xt::view(batch_label, i) = item.getLabel();
                }
            }

            if (!label_shape.empty()) 
            {
                return Batch<DType, LType>(batch_data, batch_label);
            } 
            else 
            {
                return Batch<DType, LType>(batch_data, xt::xarray<LType>());
            }
        }

        bool operator!=(const Iterator &iterator)
        {
            return this->batch_index != iterator.batch_index 
                || this->ptr_loader  != iterator.ptr_loader;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            ++batch_index;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;    
        }
    };
    //END of Iterator
    
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};


#endif /* DATALOADER_H */

