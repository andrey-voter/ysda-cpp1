# Any

В этой задаче вам предстоит реализовать аналог класса [std::any](https://en.cppreference.com/w/cpp/utility/any) из стандарта C++17.
Данный класс позволяет инкапсулировать в себе значение любого типа, например
```c++
Any a = 5;
Any b = std::string{"hello, world"};
Any c = std::vector{1, 2, 3};
c = 2.2;
b = a;
```

Интерфейс класса приведен в файле `any.h`, некоторые особенности:

* Метод `Empty` должен возвращать true, если Any не содержит объекта. Это происходит, например,
при конструировании Any по умолчанию или после вызова `Clear`.
В данном задании требуется, чтобы перемещение из объекта Any оставляло его в пустом состоянии.
* Метод `GetValue` возвращает содержимое `Any`, возвращаемый тип, разумеется, должен указывать сам пользователь.
При этом если запрашиваемый тип не совпадает с тем, который хранится в объекте `Any`, метод должен бросить исключение `std::bad_cast`.
Примеры использования можно посмотреть в тестах.

### Примечания

* Использование `std::any`, `void*` и умных указателей запрещено.
* В решении вам понадобится техника type erasure, прочитать о которой можно [здесь](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Erasure).
* Задача разобрана на семинаре.