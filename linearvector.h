// Copyright 2017, Sergey Zagrebin
// This file is part of LinearVector.
//
// LinearVector is free software : you can redistribute it and / or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// LinearVector is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with LinearVector. If not, see <http://www.gnu.org/licenses/>.

// Author: zagrebin.dev@gmail.com (Sergey Zagrebin)


////////// Класс линейного вектора постоянной размерности //////////
//
// Обёртка над std::array для данных, по умолчанию double
// Запрет на тип указателя
// Исключения out_of_range при попытках доступа за пределами вектора
// Строгая политика доступа через operator[], бросающий исключения
// Перегрузка арифметических операторов:
// - Поэлементное сложение и вычитание для пар векторов, умножение и деление не определены
// - Сложение, вычитание, умножение и деление вектора на число применяются для каждого элемента вектора
// - Запрет на неявное преобразование типов в вызывающем коде

#ifndef _LINEARVECTOR_H
#define _LINEARVECTOR_H

#include <type_traits>
#include <initializer_list>
#include <assert.h>
#include <array>
#include <stdexcept>

template <size_t length, typename T = double>
class LinearVector {
private:
    static_assert(length > 0, "LinearVector length should be more than zero");
    static_assert(!std::is_pointer<T>::value, "LinearVector doesn't work with pointers");
    std::array<T,length> data_;

public:
    // Конструктор без аргументов, инициализирует значения нулями
    LinearVector() {
        for (size_t i = 0; i < length; ++i)
            data_[i] = T(0);
    }

    // Конструктор по списку инициализации (бросает исключение std::out_of_range)
    LinearVector(std::initializer_list<T> args) {
        assert(args.size() == length);
        if (args.size() != length) {
            throw std::out_of_range("incorrect size on initialiser list");
        }
        size_t i = 0;
        for (const auto& p : args) {
            data_.at(i) = p;
            ++i;
        }
    }

    // Конструктор копирования
    LinearVector(const LinearVector<length,T>& vec) {
        for (size_t i = 0; i < length; ++i)
            data_[i] = vec[i];
    }

    // Присваивание (бросает исключение std::out_of_range)
    T& operator[] (size_t i) {
        assert((i < length)&&(i >= 0));
        return data_.at(i);
    }

    // Доступ (бросает исключение std::out_of_range)
    const T& operator[] (size_t i) const {
        assert((i < length)&&(i >= 0));
        return data_.at(i);
    }

    //////////// Операции с другими векторами ////////////////
    void operator= (const LinearVector<length,T>& vec) {
        for (size_t i = 0; i < length; ++i)
            data_[i] = vec[i];
        return;
    }

    LinearVector& operator+= (const LinearVector<length,T>& vec) {
        for (size_t i = 0; i < length; ++i)
            data_[i] += vec[i];
        return *this;
    }

    LinearVector& operator-= (const LinearVector<length,T>& vec) {
        for (size_t i = 0; i < length; ++i)
            data_[i] -= vec[i];
        return *this;
    }

    ///////////////// Операции с числами ////////////////////////
    template <typename type>
    LinearVector& operator+= (const type& num) {
        static_assert(std::is_same<type,T>::value,"Implicit type conversions are forbiden");
        for (size_t i = 0; i < length; ++i)
            data_[i] += num;
        return *this;
    }

    template <typename type>
    LinearVector& operator-= (const type& num) {
        static_assert(std::is_same<type,T>::value,"Implicit type conversions are forbiden");
        for (size_t i = 0; i < length; ++i)
            data_[i] -= num;
        return *this;
    }

    template <typename type>
    LinearVector& operator*= (const type& num) {
        static_assert(std::is_same<type,T>::value,"Implicit type conversions are forbiden");
        for (size_t i = 0; i < length; ++i)
            data_[i] *= num;
        return *this;
    }

    template <typename type>
    LinearVector& operator/= (const type& num) {
        static_assert(std::is_same<type,T>::value,"Implicit type conversions are forbiden");
        for (size_t i = 0; i < length; ++i)
            data_[i] /= num;
        return *this;
    }
    /////////////////////////////////////////////////////////

};

/////////// Бинарные операции выражений ////////////////////
// Оба оператора - векторы
template <size_t length, typename T>
LinearVector<length,T> operator+ (const LinearVector<length,T>& left, const LinearVector<length,T>& right) {
    LinearVector<length,T> temp(left);
    return temp += right;
}

template <size_t length, typename T>
LinearVector<length,T> operator- (const LinearVector<length,T>& left, const LinearVector<length,T>& right) {
    LinearVector<length,T> temp(left);
    return temp -= right;
}

// Слева вектор, справа число
template <size_t length, typename T>
LinearVector<length,T> operator+ (const LinearVector<length,T>& vec, const T& num) {
    LinearVector<length,T> temp(vec);
    return temp += num;
}

template <size_t length, typename T>
LinearVector<length,T> operator- (const LinearVector<length,T>& vec, const T& num) {
    LinearVector<length,T> temp(vec);
    return temp -= num;
}

template <size_t length, typename T>
LinearVector<length,T> operator* (const LinearVector<length,T>& vec, const T& num) {
    LinearVector<length,T> temp(vec);
    return temp *= num;
}

template <size_t length, typename T>
LinearVector<length,T> operator/ (const LinearVector<length,T>& vec, const T& num) {
    LinearVector<length,T> temp(vec);
    return temp /= num;
}

// Слева число, справа вектор
template <size_t length, typename T>
LinearVector<length,T> operator+ (const T& num, const LinearVector<length,T>& vec) {
    return vec + num;
}

template <size_t length, typename T>
LinearVector<length,T> operator- (const T& num, const LinearVector<length,T>& vec) {
    return vec * T(-1) + num;
}

template <size_t length, typename T>
LinearVector<length,T> operator* (const T& num, const LinearVector<length,T>& vec) {
    return vec * num;
}

template <size_t length, typename T>
LinearVector<length,T> operator/ (const T& num, const LinearVector<length,T>& vec) {
    LinearVector<length,T> temp(vec);
    for (size_t i = 0; i < length; ++i){
        temp[i] = T(1) / temp[i];
    }
    return temp *= num;
}
//////////////////////////////////////////////////////////////
#endif // _LINEARVECTOR_H
