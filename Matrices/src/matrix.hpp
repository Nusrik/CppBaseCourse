#pragma once

#include <algorithm>
#include <tuple>

#include "myvector.hpp"

namespace linalg {

// TODO: Add checkers that DataType has required operators
template <typename DataType> struct SqMatrix final {
public:
  explicit SqMatrix(size_t n)
      : mData(mystd::MyVector<mystd::MyVector<DataType>>(
            n, mystd::MyVector<DataType>(n))) {}

public:
  size_t size() const { return mData.size(); }

  std::tuple<SqMatrix, bool> GaussElementaryTransform() const {
    SqMatrix<DataType> tmp = *this;
    bool inverse = false;

    for (size_t i = 0, end = mData.size(); i < mData.size(); ++i) {
      size_t row = i;

      const DataType &zeroElem = DataType(0);
      mystd::MyVector<DataType> &rowVec = tmp[i];
      auto it = std::find_if(
          rowVec.begin(), rowVec.end(),
          [&zeroElem](const DataType &elem) { return elem != zeroElem; });
      if (it != rowVec.end())
        row = it - rowVec.begin();

      if (i != row) {
        std::swap(tmp[i], tmp[row]);
        inverse = !inverse;
      }

      // Getting rid of all elements under current tmp[i, j];
      for (size_t j = i + 1, end = mData.size(); j < end; ++j) {
        if (tmp[j][i] == DataType(0))
          continue;
        DataType q = tmp[j][i] / tmp[i][i];
        for (size_t k = i, end = mData.size(); k < end; ++k)
          tmp[j][k] -= q * tmp[i][k];
      }
    }

    return std::make_tuple(tmp, inverse);
  }

  // TODO: Add size check to determine that this is square matrix
  DataType GaussDet() const {
    auto [tmpMtr, inverse] = GaussElementaryTransform();

    DataType det(1);
    if (inverse)
      det = -det;

    for (size_t i = 0, end = mData.size(); i < end; ++i)
      det *= tmpMtr[i][i];

    return det;
  }

public:
  mystd::MyVector<DataType> &operator[](size_t i) {
    if (i > mData.size())
      throw std::invalid_argument("Row index is out of range!");

    return mData[i];
  }

public:
  mystd::MyVector<mystd::MyVector<DataType>> mData;
};

template <typename DataType> SqMatrix<DataType> makeSqEMatrix(size_t n) {
  SqMatrix<DataType> tmp(n);
  for (size_t i = 0; i < n; ++i) {
    tmp[i] = mystd::MyVector<DataType>(n, DataType(0));
    tmp[i][i] = DataType(1);
  }
  return tmp;
}

} // namespace linalg
