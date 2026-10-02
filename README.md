# Stack with canaries and hash protection

Cтек на C с защитой от порчи памяти: канарейки, хеши и отладочный лог.

## Структура

| Файл | Назначение |
|---|---|
| `stackHeader.h` | Типы, константы, прототипы функций |
| `stackWithCanary.cpp` |Основные функции стека |
| `main.cpp` | Пример использования |


## Сборка и запуск

```bash
g++ main.cpp 
./a DEBUG.txt 
```
Имя файла может отличаться. Вы сами выбираете, куда будут сохранены логи ошибок.


## `MAKE_DEBUG(arg)`

### Что передаётся в скобках

В `arg` указывается имя файла отладочного лога, то есть строка типа `const char*`. Она попадает в поле `fileName` структуры `debug_info_t`, и именно этот файл открывает `makeDebugLog` при любой ошибке.

Остальные поля заполняются автоматически:
### Куда направить лог

Файл лога выбирается тем, что вы укажете в скобках. Это не обязательно `argv[1]`:

```c
// имя файла из первого аргумента командной строки
stackPush(&stk, 78 MAKE_DEBUG(argv[1]));

// из второго аргумента
stackPush(&stk, 78 MAKE_DEBUG(argv[2]));

// константная строка
stackPush(&stk, 78 MAKE_DEBUG("errors.txt"));
```

Для разных стеков или разных операций можно указывать разные файлы, и ошибки каждого стека лягут в свой лог:

| Функция | Описание |
|---|---|
| `stackInit(stk, capacity)` | Инициализирует стек. `capacity` от 1 до `CAPACITY_MAX`. |
| `destroyStack(stk)` | Освобождает буфер и обнуляет поля. |
| `stackPush(stk, value)` | Кладёт значение. При необходимости увеличивает ёмкость. |
| `stackPop(stk)` | Снимает и возвращает верхний элемент. При ошибке возвращает `POISON_VALUE`. |
| `stackTop(stk)` | Возвращает верхний элемент без удаления. При ошибке или пустом стеке возвращает `POISON_VALUE`. |
| `printStackConsole(stk)` | Печатает размер, ёмкость и элементы. |
| `changeMemoryUpStack(stk, newCapacity)` | Увеличивает ёмкость до `newCapacity` (не меньше текущей и не больше `CAPACITY_MAX`). |

Внутренние функции: `mainVerifier`, `canaryChecker`, `cashChecker`, `changeCash`, `canaryAdder`, `pushRealloc`, `popRealloc`, `djb2_hash_stackData`, `djb2_hash_stackStruc`, `makeDebugLog`, `getErrorName`.



## Коды ошибок (`errors_t`)

| Код | Значение |
|---|---|
| `noProblem` | Ошибок нет |
| `callocError` | Буфер не выделен |
| `capacityProblem` | `capacity` вне допустимых пределов |
| `sizeProblem` | `size` больше `capacity` или `CAPACITY_MAX` |
| `alreadyInit` | Стек уже инициализирован (зарезервирован) |
| `initProblem` | Ошибка инициализации (зарезервирован) |
| `nullProblem` | Передан `NULL` |
| `canaryLeftDead` / `canaryRightDead` | Повреждена левая / правая канарейка буфера |
| `canaryUpDead` / `canaryDownDead` | Повреждена верхняя / нижняя канарейка структуры |
| `hashError` | Хеш не совпал |

