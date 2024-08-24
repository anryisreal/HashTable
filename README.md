# HashTable

## Описание

Этот проект реализует хеш-таблицу на языке C++ с использованием шаблонов, что позволяет хранить ключи и значения любых типов. Хеш-таблица поддерживает вставку, удаление, поиск и обработку коллизий. Для управления памятью используются обычные указатели.

## Основные возможности

- **Вставка**: Добавление новых элементов в хеш-таблицу с автоматическим управлением коллизиями.
- **Удаление**: Удаление элементов по ключу (можно добавить по мере необходимости).
- **Поиск**: Доступ к элементам через оператор индексации `operator[]`.
- **Обработка коллизий**: Встроенная обработка коллизий.
- **Динамическое расширение**: Автоматическое изменение размера таблицы при заполнении.

## Использование

### Инициализация и вставка элементов

```cpp
#include "hash.h"

int main() {
    Hash::HashTable<int, std::string> table;
    
    table[1] = "Hello ";
    table[2] = "World!;

    std::cout << table[1] << table[2] << std::endl;
    
    return 0;
}
