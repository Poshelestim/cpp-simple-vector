#pragma once

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <stdexcept>

#include "array_ptr.h"

class ReserveProxyObj
{
public:

    ReserveProxyObj() = delete;

    explicit ReserveProxyObj(size_t capacity_to_reserve);

    [[nodiscard]] size_t GetNewCapacity() const;
private:

    size_t capacity_ = 0;
};

ReserveProxyObj::ReserveProxyObj(size_t capacity_to_reserve)
    : capacity_(capacity_to_reserve)
{

}

size_t ReserveProxyObj::GetNewCapacity() const
{
    return capacity_;
}

ReserveProxyObj Reserve(size_t capacity_to_reserve)
{
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector
{
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size);

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type &value);

    SimpleVector(size_t size, Type &&value);

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init);

    SimpleVector(const SimpleVector &other);

    SimpleVector(SimpleVector &&other);

    SimpleVector(const ReserveProxyObj &capacity_to_reverse);

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept;

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept;

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept;

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item);

    void PushBack(Type&& item);


    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type &value);

    Iterator Insert(ConstIterator pos, Type &&value);

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept;

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos);

    SimpleVector& operator=(const SimpleVector &rhs) noexcept;

    SimpleVector& operator=(SimpleVector &&rhs) noexcept;

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept;

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept;

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index);

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const;

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept;

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию
    // для типа Type
    void Resize(size_t new_size);

    // Задает ёмкость вектора
    void Reserve(size_t new_capacity);

    void swap(SimpleVector<Type> &other) noexcept;

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept;

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept;

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept;

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept;

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept;

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept;

private:

    ArrayPtr<Type> items_ = {};
    size_t size_ = 0;
    size_t capacity_ = 0;
};


template<typename Type>
SimpleVector<Type>::SimpleVector(size_t size)
{
    assert(size_ == 0 && capacity_ == 0);

    SimpleVector tmp(size, Type{});
    swap(tmp);
}

template<typename Type>
SimpleVector<Type>::SimpleVector(size_t size, const Type &value)
{
    assert(size_ == 0 && capacity_ == 0);

    ArrayPtr<Type> tmp(size);
    std::fill(tmp.Get(), tmp.Get() + size, value);
    items_.swap(tmp);
    capacity_ = size;
    size_ = capacity_;
}

template<typename Type>
SimpleVector<Type>::SimpleVector(size_t size, Type &&value)
{
    assert(size_ == 0 && capacity_ == 0);

    ArrayPtr<Type> tmp(size);

    for (auto it = tmp.Get(); it != tmp.Get() + size; ++it)
    {
        *it = std::move(value);
    }

    items_.swap(tmp);
    capacity_ = size;
    size_ = capacity_;
}

template<typename Type>
SimpleVector<Type>::SimpleVector(std::initializer_list<Type> init)
{
    assert(size_ == 0 && capacity_ == 0);

    SimpleVector<Type> tmp(init.size());
    std::copy(init.begin(), init.end(), tmp.begin());
    swap(tmp);
}

template<typename Type>
SimpleVector<Type>::SimpleVector(const SimpleVector &other)
{
    assert(size_ == 0 && capacity_ == 0);

    SimpleVector<Type> new_vector(other.GetSize());
    new_vector.capacity_ = other.capacity_;

    std::copy(other.begin(), other.end(), new_vector.begin());
    swap(new_vector);
}

template<typename Type>
SimpleVector<Type>::SimpleVector(SimpleVector &&other)
{
    assert(size_ == 0 && capacity_ == 0);

    SimpleVector<Type> new_vector(other.GetSize());
    new_vector.capacity_ = other.capacity_;

    std::move(other.begin(), other.end(), new_vector.begin());
    swap(new_vector);
    other.Clear();
}

template<typename Type>
SimpleVector<Type>::SimpleVector(const ReserveProxyObj &capacity_to_reverse)
{
    assert(size_ == 0 && capacity_ == 0);

    SimpleVector<Type> tmp(capacity_to_reverse.GetNewCapacity());
    swap(tmp);
    size_ = 0;
}

template<typename Type>
size_t SimpleVector<Type>::GetSize() const noexcept
{
    return size_;
}

template<typename Type>
size_t SimpleVector<Type>::GetCapacity() const noexcept
{
    return capacity_;
}

template<typename Type>
bool SimpleVector<Type>::IsEmpty() const noexcept
{
    return size_ == 0U;
}

template<typename Type>
void SimpleVector<Type>::PushBack(const Type &item)
{
    if (size_ < capacity_)
    {
        (*this)[size_++] = item;
    }
    else
    {
        Resize(++size_);
        (*this)[size_ - 1] = item;
    }
}

template<typename Type>
void SimpleVector<Type>::PushBack(Type &&item)
{
    if (size_ < capacity_)
    {
        (*this)[size_++] = std::move(item);
    }
    else
    {
        Resize(++size_);
        (*this)[size_ - 1] = std::move(item);
    }
}

template<typename Type>
typename SimpleVector<Type>::Iterator
SimpleVector<Type>::Insert(ConstIterator pos, const Type &value)
{
    if (pos < begin() || pos > end())
    {
        throw std::out_of_range("out of range");
    }

    int distance = pos - begin();

    if (size_ == capacity_)
    {
        size_ != 0 ? Reserve(2 * capacity_) : Reserve(1);
    }
    std::copy_backward(begin() + distance, end(), end() + 1);
    *(begin() + distance) = value;

    return begin() + distance;
}

template<typename Type>
typename SimpleVector<Type>::Iterator
SimpleVector<Type>::Insert(ConstIterator pos, Type &&value)
{
    if (pos < begin() || pos > end())
    {
        throw std::out_of_range("out of range");
    }

    int distance = std::distance(cbegin(), pos);

    if (size_ == capacity_)
    {
        size_ != 0 ? Reserve(2 * capacity_) : Reserve(1);
    }
    std::move_backward(begin() + distance, end(), end() + 1);
    items_[distance] = std::move(value);
    ++size_;
    value = std::move(Type{});
    return begin() + distance;
}

template<typename Type>
void SimpleVector<Type>::PopBack() noexcept
{
    if (!IsEmpty())
    {
        --size_;
    }
}

template<typename Type>
typename SimpleVector<Type>::Iterator
SimpleVector<Type>::Erase(ConstIterator pos)
{
    if (pos < begin() || pos >= end())
    {
        throw std::out_of_range("out of range");
    }

    int distance = std::distance(cbegin(), pos);

    std::move(begin() + distance + 1, end(), begin() + distance);
    --size_;
    return begin() + distance;
}

template<typename Type>
SimpleVector<Type> &SimpleVector<Type>::operator=(const SimpleVector &rhs) noexcept
{
    if (this != &rhs)
    {
        SimpleVector<Type> tmp(rhs);
        swap(tmp);
    }
    return *this;
}

template<typename Type>
SimpleVector<Type> &SimpleVector<Type>::operator=(SimpleVector &&rhs) noexcept
{
    if (this != &rhs)
    {
        SimpleVector<Type> tmp(rhs);
        swap(tmp);
    }
    return *this;
}

template<typename Type>
Type &SimpleVector<Type>::operator[](size_t index) noexcept
{
    assert(index < size_);
    return items_[index];
}

template<typename Type>
const Type &SimpleVector<Type>::operator[](size_t index) const noexcept
{
    assert(index < size_);
    return items_[index];
}

template<typename Type>
Type &SimpleVector<Type>::At(size_t index)
{
    if (index >= size_)
    {
        throw std::out_of_range("Index must be less than vector size");
    }
    return (*this)[index];
}

template<typename Type>
const Type &SimpleVector<Type>::At(size_t index) const
{
    if (index >= size_)
    {
        throw std::out_of_range("Index must be less than vector size");
    }
    return (*this)[index];
}

template<typename Type>
void SimpleVector<Type>::Clear() noexcept
{
    size_ = 0;
}

template<typename Type>
void SimpleVector<Type>::Resize(size_t new_size)
{
    if (new_size < size_)
    {
        size_ = new_size;
    }
    else if (new_size < capacity_)
    {
        //        std::fill(begin() + size_, begin() + new_size, Type{});
        for (auto it = begin() + size_; it != begin() + new_size; ++it)
        {
            *it = std::move(Type{});
        }
        size_ = new_size;
    }
    else
    {
        ArrayPtr<Type> new_vector(new_size * 2);
        if (capacity_ != 0)
        {
            std::move(begin(), end(), new_vector.Get());
        }

        for (auto it = new_vector.Get() + size_;
             it != new_vector.Get() + new_size;
             ++it)
        {
            *it = std::move(Type{});
        }
        //        std::fill(new_vector.Get() + size_, new_vector.Get() + new_size, Type{});
        items_.swap(new_vector);
        size_ = new_size;
        capacity_ = new_size * 2;
    }
}

template<typename Type>
void SimpleVector<Type>::Reserve(size_t new_capacity)
{
    if (capacity_ >= new_capacity)
    {
        return;
    }

    ArrayPtr<Type> new_vector(new_capacity);
    if (capacity_ != 0)
    {
        //        std::copy(begin(), end(), new_vector.Get());
        std::move(begin(), end(), new_vector.Get());
    }

    for (auto it = new_vector.Get() + size_;
         it != new_vector.Get() + new_capacity;
         ++it)
    {
        *it = std::move(Type{});
    }

    //    std::fill(new_vector.Get() + size_, new_vector.Get() + new_capacity, Type{});
    items_.swap(new_vector);
    capacity_ = new_capacity;
}

template<typename Type>
void SimpleVector<Type>::swap(SimpleVector<Type> &other) noexcept
{
    items_.swap(other.items_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template<typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::begin() noexcept
{
    return items_.Get();
}

template<typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::end() noexcept
{
    return items_.Get() + size_;
}

template<typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::begin() const noexcept
{
    return cbegin();
}

template<typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::end() const noexcept
{
    return cend();
}

template<typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::cbegin() const noexcept
{
    return items_.Get();
}

template<typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::cend() const noexcept
{
    return items_.Get() + size_;
}

template <typename Type>
void swap(SimpleVector<Type>& lhs, SimpleVector<Type>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename Type>
SimpleVector<Type> Reserve(size_t new_capacity) noexcept
{
    SimpleVector<Type> to_return = {};
    to_return.Reserve(new_capacity);
    return to_return;
}

template <typename Type>
bool operator==(const SimpleVector<Type>& lhs,
                const SimpleVector<Type>& rhs)
{
    if (&lhs == &rhs)
    {
        return true;
    }

    return  std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SimpleVector<Type>& lhs,
                const SimpleVector<Type>& rhs)
{
    return !operator==(lhs, rhs);
}

template <typename Type>
bool operator<(const SimpleVector<Type>& lhs,
               const SimpleVector<Type>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end());
}

template <typename Type>
bool operator>(const SimpleVector<Type>& lhs,
               const SimpleVector<Type>& rhs)
{
    return !operator<(lhs, rhs) && operator!=(lhs, rhs);
}

template <typename Type>
bool operator<=(const SimpleVector<Type>& lhs,
                const SimpleVector<Type>& rhs)
{
    return operator<(lhs, rhs) || operator==(lhs, rhs);
}

template <typename Type>
bool operator>=(const SimpleVector<Type>& lhs,
                const SimpleVector<Type>& rhs)
{
    return !operator<(lhs, rhs);
}
