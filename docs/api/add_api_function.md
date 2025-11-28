# Добавление новых функций API

Добавление новых функций в API может понадобится при модернизации ядра или разработке драйвера для устройства, которое не было предусмотрено создателем ядра.

## Структура файлов с исходным кодом API

Здесь можно посмотреть назначение файлов с исходным кодом API ядра. Это важно, ведь для изменения API необходимо понимать куда вносить изменения.

Файлы с исходным кодом API ядра находятся по пути [`src/api/`](../../src/api/)

### [api.c](../../src/api/api.c) / [api.h](../../src/api/api.h) 
Предназначены для приёма системных вызовов из user-space. Вызывает внутреннюю функцию-интерфейс ядра исходя из номера системного вызова. Внутренние функции-интерфейсы для user-space находятся в файлах `userspace_if.c` / `userspace_if.h`

### [userspace_if.c](../../src/api/userspace_if.c) / [userspace_if.h](../../src/api/userspace_if.h)
Файлы содержат функции-интерфейсы для userspace. Эти принимают:
- Адрес структуры, которая содержит значения регистров, то есть аргументы
- Адрес структуры, которая содержит результат, который затем будет записан в регистры

Сигнатура структуры:
``` c
// Структура предназначенная для хранения регистров
struct registers_struct {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esi;
    unsigned int edi;
};
```

Сигнатура функции-интерфейса:
``` c
void sys_example_func(struct registers_struct* args, struct registers_struct* result);
```

Функция-интерфейс вызывает внутреннюю функцию ядра, передавая ей аргументы не в структуре, а уже как полноценные аргументы. После возврата внутренняя функция ядра может вернуть значение (*зависит от спецификации*), оно должно быть записно в определённый *спецификацией* регистр по адресу структуры в result

Рассмотрим пример:

Существует внутренняя функция ядра:

``` c
int _example_func(int arg1, void* arg2, char* arg3); // Возвращает число
```

Вот как она будет вызываться в функции-интерфейсе:

``` c
void sys_example_func(struct registers_struct* args, struct registers_struct* result){
    int arg1 = (int)args->ebx; // Число
    void* arg2 = (void*)args->ecx; // Адрес
    char* arg3 = (char*)args->edx; // Адрес на массив символов
    int func_result = _example_func(arg1, arg2, arg3); // Вызываем функцию, передаём ей аргументы, получаем значение
    result->ebx = func_result; // Записываем результат в регистр ebx
}
```

### [general_functions.c](../../src/api/general_functions.c) / [general_functions.h](../../src/api/general_functions.h)

Эти файлы содержат **общие** функции ядра, т.е предназначенные и для user-space и для компонентов ядра. 

### [kernel_functions.c](../../src/api/kernel_functions.c) / [kernel_functions.h](../../src/api/kernel_functions.h)

Эти файлы содержат функции ядра только **для компонентов ядра**.

## Добавление функции

Для начала нужно определиться: функция должна быть доступна **только для компонентов ядра (kernel-space)** или **должна быть общей (kernel-space и user-space)**

### Только для компонентов ядра (kernel-space)

Краткий план действий:
1. реализуем функцию в файлах `kernel_functions.c` и `kernel_functions.h`

> Желательно перед названием функции ставить символ `_`

### Общая (kernel-space и user-space)

Краткий план действий:
1. придумываем спецификацию для системного вызова
2. реализуем функцию в файлах `general_functions.c` и `general_functions.h`
3. добавляем функцию-интерфейс в файлы `userspace_if.c` и `userspace_if.h`
4. добавляем обработчик системного вызова по его номеру

В качестве примера возьмём функцию `_print_text`, которая выводит текст.

#### 1. Спецификация для системного вызова

Необходимо продумать как программы будут вызывать системный вызов: какие регистры необходимо использовать для передачи аргументов, какой номер системного вызова будет использоваться.

Основная спецификация системных вызовов [находится здесь](syscalls.md#детали) - для аргументов необходимо использовать те регистры, которые приведены в ней.

Номер системного вызова должен быть уникальным, и не совпадать с уже существующими номерами. Номера существующих системных функций можно посмотреть [в таблице](syscalls.md#таблица-системных-вызовов)

Пример:

Функция `_print_text` имеет номер системного вызова `0x0`. 

Регистр `ebx` используется для передачи адреса строки, `ecx` - количество байт для вывода, `dl` - координата X, `dh` - координата Y, `si` - цвет символов, `edi` - индекс устройства.

Спецификацию `_print_text` можно найти [здесь](syscall_functions.md#print_text)

#### 2. Реализация  функции в `general_functions.c/h`

Здесь реализовывается функция, которая должна принимать аргументы, и может вызываться как из kernel-space так и из user-space (через системные вызовы).

Желательно, чтобы имя функции начиналось с символа `_`.

Пример:

Функция `_print_text` имеет следующую реализацию:
``` c
void _print_text(unsigned char* text, unsigned int size, unsigned char x, unsigned char y, unsigned char font_color, unsigned char bkgr_color, unsigned int device_index){

    struct dev_info* devices = _get_device_info(); // Получение адреса массива устройств
    struct dev_info* device = &devices[device_index]; // Получение конкретного устройства по его индексу, который передаётся в аргументах

    // Здесь происходит получение драйвера устройства.
    // Дальше получаем функцию драйвера PRINT_TEXT - данная функция расположена по индексу 0 (но здесь используется объект enum).
    void (*driver_print_text)(struct dev_info*, unsigned char*, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char) = (void (*)(struct dev_info*, unsigned char*, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char))(device->driver->funcs[PRINT_TEXT]);

    // Вызываем полученную функцию и передаем аргументы.
    driver_print_text(device, text, size, x, y, font_color, bkgr_color);

    // Обратите внимание: расположение функций драйверов в массиве, а также принимаемые аргументы - определены в спецификации драйверов
}
```

#### 3. Реализация  функции-интерфейса в `userspace_if.c/h`

Смысл функции-интерфейса: принять регистры, правильно передать аргументы (по спецификации функции), вернуть результат в регистрах

Функция-интерфейс должна принимать два аргумента:
``` c
void sys_example_func(struct registers_struct* args, struct registers_struct* result);
```

Структура выглядит следующим образом:
``` c
// Структура предназначенная для хранения регистров
struct registers_struct {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esi;
    unsigned int edi;
};
```

Функция-интерфейс должна правильно передать значения регистров из структуры в аргументы функции (по спецификации функции):
``` c
void sys_example_func(struct registers_struct* args, struct registers_struct* result){
    int arg1 = (int)args->ebx; // Число
    void* arg2 = (void*)args->ecx; // Адрес
    char* arg3 = (char*)args->edx; // Адрес на массив символов
    int func_result = _example_func(arg1, arg2, arg3); // Вызываем функцию, передаём ей аргументы, получаем значение
    result->ebx = func_result; // Записываем результат в регистр ebx
}
```

А также должна записать результат функции в структуру по адресу из `result`

Желательно, чтобы имя функции начиналось с `sys_`.

Пример:

Функция `_print_text` вызывается следующим образом:
``` c
void sys_print_text(struct registers_struct* args, struct registers_struct* result){
    _print_text(
                args->ebx,
                args->ecx,
                ((args->edx) & 0xFF),
                ((args->edx >> 8) & 0xFF),
                ((args->esi) & 0xFF),
                ((args->esi >> 8) & 0xFF),
                args->edi
                );
}
```
Обратите внимание на то, что соблюдается [спецификация аргументов](syscall_functions.md#print_text)

#### 4. Обработчик системного вызова по его номеру

В файле `api.c` необходимо в секции `Вызов определённой функции исходя по номеру системного вызова` в конструкцию `switch-case` добавить еще одно условие в таком виде:
``` c
case НОМЕР_СИСТЕМНОГО_ВЫЗОВА:
            sys_НАЗВАНИЕ_ФУНКЦИИ(&args, &result);
            break;
```

Пример:

Функция `_print_text` вызывается следующим образом:

``` c
case 0x0:
    sys_print_text(&args, &result);
    break;
```
