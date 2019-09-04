//
// Created by acco on 3/1/18.
//
// Simple raw memory allocator.
//

#ifndef XSTTRP_MM_HPP
#define XSTTRP_MM_HPP

#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>

/*
* Memory management template functions.
*/
namespace mm {

    /*
    * Declare an efficient 2d matrix of contiguous memory of primitive data types.
    * Cannot be used for objects that requires allocation with new.
    * It should be used for up to average sized matrices since memory is contiguous.
    * Matrix could be accessed in the classical way matrix[i][j]
    */
    template<class T>
    static inline T ** request_raw_contiguous_memory(const size_t rows, const size_t columns) {

        assert(rows > 0);
        assert(columns > 0);

        // allocate space for:
        // the data: columns * rows * sizeof(T)
        // the row pointers: rows * sizeof(T*)
        // [ROW POINTERS] [---------------- DATA -------------]
        const size_t size = columns * rows * sizeof(T) + rows * sizeof(T*);
        auto ** matrix = static_cast<T**>(malloc(size));

        if(!matrix) {
            std::cout << "Fatal error while requesting memory. Aborting.\n";
            exit(1);
        }

        assert(matrix);
        memset(matrix, 0, size);

        // the initial space contains the row pointers. Let make them point to the right data
        // baseAddress contains the address of the memory after the row pointers
        // [ROW POINTERS] [---------------- DATA -------------]
        // baseAddress-->|
        // then each chunk of data contains "columns" columns
        auto * baseAddress = reinterpret_cast<T*>(&matrix[rows]);
        for (size_t i = 0; i < rows; i++) {
            matrix[i] = baseAddress + i * columns;
        }

        return matrix;
    }

    template<class T>
    static inline T * request_raw_contiguous_memory(const size_t elements) {

        assert(elements > 0);
        auto * ptr = static_cast<T*>(calloc(elements, sizeof(T)));
        if(!ptr) {
            std::cout << "Fatal error while requesting memory. Aborting.\n";
            exit(1);
        }
        assert(ptr);
        return ptr;
    }

    template <class T>
    static inline void release_raw_contiguous_memory(T *&ptr) {
        free(ptr);
        ptr = nullptr;
    }

}

#endif //XSTTRP_MM_HPP
