
#ifndef __array2D_hpp__
#define __array2D_hpp__

#include "iOstream.hpp"

namespace pFlow
{

template<typename T, size_t nRow, size_t nCol>
struct array2D
{

    T   elements_[nRow][nCol];

    inline
    T& operator()(size_t i, size_t j) noexcept
    {
        return elements_[i][j];
    }

    inline
    const T& operator()(size_t i, size_t j)const noexcept
    {
        return elements_[i][j];
    }

    constexpr size_t size()const noexcept
    {
        return  nRow*nCol;
    }


    constexpr size_t nCols()const noexcept
    {
        return nCol;
    }

    constexpr size_t nRows()const noexcept
    {
        return nRow;
    }
    
};

template<typename T, size_t nRow, size_t nCol>
array2D<T, nRow, nCol> operator+(
    const array2D<T, nRow, nCol>& arr1, 
    const array2D<T, nRow, nCol>& arr2)
{
    array2D<T, nRow, nCol> res;

    for(size_t i=0; i<nRow; i++)
        for(size_t j=0; j<nCol; j++)
            res(i,j)= arr1(i,j)+arr2(i,j);
    return res;
}

template<typename T, size_t nRow, size_t nCol>
array2D<T, nRow, nCol> operator-(
    const array2D<T, nRow, nCol>& arr1, 
    const array2D<T, nRow, nCol>& arr2)
{
    array2D<T, nRow, nCol> res;

    for(size_t i=0; i<nRow; i++)
        for(size_t j=0; j<nCol; j++)
            res(i,j)= arr1(i,j)-arr2(i,j);
    return res;
}

template<typename T, size_t nRow, size_t nCol>
array2D<T, nRow, nCol> operator*(
    const array2D<T, nRow, nCol>& arr1, 
    const array2D<T, nRow, nCol>& arr2)
{
    array2D<T, nRow, nCol> res;

    for(size_t i=0; i<nRow; i++)
        for(size_t j=0; j<nCol; j++)
            res(i,j)= arr1(i,j)*arr2(i,j);
    return res;
}

template<typename T, size_t nRow, size_t nCol>
array2D<T, nRow, nCol> operator*(
    const T& s, 
    const array2D<T, nRow, nCol>& arr2)
{
    array2D<T, nRow, nCol> res;

    for(size_t i=0; i<nRow; i++)
        for(size_t j=0; j<nCol; j++)
            res(i,j)= s*arr2(i,j);
    return res;
}

template<typename T, size_t nRow, size_t nCol>
array2D<T, nRow, nCol> operator/(
    const array2D<T, nRow, nCol>& arr1, 
    const array2D<T, nRow, nCol>& arr2)
{
    array2D<T, nRow, nCol> res;

    for(size_t i=0; i<nRow; i++)
        for(size_t j=0; j<nCol; j++)
            res(i,j)= arr1(i,j)/arr2(i,j);
    return res;
}

template<typename T, size_t nRow, size_t nCol>
array2D<T, nRow, nCol> operator/(
    const T& val,
    const array2D<T, nRow, nCol>& arr2)
{
    array2D<T, nRow, nCol> res;

    for(size_t i=0; i<nRow; i++)
        for(size_t j=0; j<nCol; j++)
            res(i,j)= val/arr2(i,j);
    return res;
}

template <typename T, size_t nRow, size_t nInner, size_t nCol >
array2D<T, nRow, nCol> 
MatMul(
    const array2D<T, nRow, nInner>& A, 
    const array2D<T, nInner, nCol>& B)
{
    array2D<T, nRow, nCol> C;
	
    for (size_t row = 0; row < nRow; row++)
	{
		for (size_t col = 0; col < nCol; col++)
		{
			T sum = 0;
			for (size_t inner = 0; inner < nInner; inner++)
			{
				sum += A(row,inner) * B(inner,col);
			}
			C(row,col) = sum;
		}
	}

    return C;
}

template<typename T, size_t nRow, size_t nCol>
array2D<T, nCol, nRow> 
transpose(const array2D<T, nRow, nCol>& arr)
{
    array2D<T, nCol, nRow> tArr;

    for(size_t i=0; i<nRow; i++)
    {
        for(size_t j=0; j<nCol; j++)
        {
            tArr(j,i) = arr(i,j);
        }
    }
    return tArr;
}

template<typename T, size_t nRow, size_t nCol>
iOstream& operator<<(iOstream& os, const array2D<T, nRow, nCol>& arr)
{
    os<<'[';
    for(size_t i=0; i<nRow; i++)
    {
        os<<'[';
        for(size_t j=0; j<nCol-1; j++)
        {
            os<<arr(i,j)<<' ';
        }

        if(i < nRow-1)    
            os<<arr(i,nCol-1)<<"]\n";
        else
            os<<arr(i,nCol-1)<<"]";
    }
    os<<']';

    return os;
}

}

#endif //__array2D_hpp__