# cpp-simple-vector
Разработанный собственный контейнер — вектор.

## Описание
Учебный проект, позволивший лучше разобраться в устройстве контейнеров C++ и эффективно их использовать.

### Функционал
 * Конструктор по умолчанию, который создаёт вектор;
 * Деструктор вектора удаляет все его элементы;
 * Метод `GetSize` количество элементов в массиве;
 * Метод `GetCapacity` возвращает вместимость массива;
 * Метод `IsEmpty` сообщает, пустой ли вектор;
 * Метод `PushBack` добавляет элемент в конец вектора, при нехватке места увеличивает вдвое вместимость вектора; 
 * Метод `PopBack` удаляет последний элемент вектора;
 * Метод `Clear` обнуляет размер массива, не изменяя его вместимость, не выбрасывает исключений;
 * Метод `Insert`, вставляет элемент в указанную позицию, возвращает итератор на вставленный элемент;
 * Метод `Erase` удаляет элемент вектора в указанной позиции, возвращает итератор на элемент, следующий за удалённым, не выбрасывает исключений;
 * Метод `Resize` изменяет размер массива;
 * Метод `Reserve` задает ёмкость вектора;
 * Метод `swap` обменивает содержимое векторов;
 * Реализованы операторы сравнения;
 * Реализованы методы `begin`, `end`, их константные и реверсивные версии.

## Сборка
Сборка с помощью любой IDE либо сборка из командной строки.

## Системные требования
Компилятор С++ с поддержкой стандарта C++17 или новее.
