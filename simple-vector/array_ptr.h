#include <cassert>
#include <cstdlib>
#include <utility>

template <typename Type>
class ArrayPtr
{
public:
    // Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле raw_ptr_ должно быть равно nullptr
    explicit ArrayPtr(size_t size) :
        size_(size)
    {
        if (size == 0)
        {
            raw_ptr_ = nullptr;
            return;
        }
        raw_ptr_ = new Type[size]{};
    }

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr, size_t size) noexcept :
        raw_ptr_(raw_ptr),
        size_(size)
    {

    }

    // Запрещаем копирование
    ArrayPtr(const ArrayPtr&) = delete;

    // Запрещаем присваивание
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    //Разрешаем перемещение
    ArrayPtr(ArrayPtr &&other)
    {
        assert(size_ == 0 && raw_ptr_ == nullptr);
        ArrayPtr tmp(other.size_);
        tmp.swap(other);
        swap(tmp);
        other.Clear();
    }

    ArrayPtr& operator=(ArrayPtr &&rhs)
    {
        if (this != &rhs)
        {
            ArrayPtr tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    ~ArrayPtr()
    {
        Clear();
    }

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept
    {
        Type *ptr_to_return = this->raw_ptr_;
        this->raw_ptr_ = nullptr;
        this->size_ = 0;
        return ptr_to_return;
    }

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept
    {
        assert(index < size_);
        return raw_ptr_[index];
    }

    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept
    {
        assert(index < size_);
        return raw_ptr_[index];
    }

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const
    {
        return this->raw_ptr_ != nullptr;
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept
    {
        return raw_ptr_;
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept
    {
        std::swap(this->raw_ptr_, other.raw_ptr_);
        std::swap(this->size_, other.size_);
    }

    void Clear()
    {
        delete[] raw_ptr_;
        raw_ptr_ = nullptr;
        size_ = 0;
    }

private:
    Type* raw_ptr_ = nullptr;
    size_t size_ = 0;
};
