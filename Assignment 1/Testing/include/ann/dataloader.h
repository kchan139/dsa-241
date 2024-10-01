
#ifndef DATALOADER_H
#define DATALOADER_H
#include "ann/dataset.h"
#include "ann/xtensor_lib.h"

using namespace std;

template <typename DType, typename LType>
class DataLoader {
 public:
 private:
  Dataset<DType, LType>* ptr_dataset;
  int batch_size;
  bool shuffle;
  bool drop_last;
  // TODO : add more member variables to support the iteration
 public:
  DataLoader(Dataset<DType, LType>* ptr_dataset, int batch_size,
             bool shuffle = true, bool drop_last = false) {
    // TODO implement
  }
  virtual ~DataLoader() {
    // TODO implement
  }

  Iterator begin() {
    // TODO implement
  }

  Iterator end() {
    // TODO implement
  }

  // TODO implement forech
  class Iterator {
   public:
    // TODO implement contructor

    Iterator& operator=(const Iterator& iterator) {
      // TODO implement
    }

    Iterator& operator++() {
      // TODO implement
    }

    Iterator operator++(int) {
      // TODO implement
    }

    bool operator!=(const Iterator& other) const {
      // TODO implement
    }

    Batch<DType, LType> operator*() const {
      // TODO implement
    }

   private:
    // TODO implement
  };
};

#endif /* DATALOADER_H */
