# Спецификация дополнительных функций драйверов


## Display


#### 0 - вывод текста

```
void print_text(struct dev_info* device, unsigned char*, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char)
```

#### 1 - новая строка

```
void new_line(struct dev_info* device)
```

#### 2 - обновить курсор

```
void cursor_update(struct dev_info* device)
```

#### 3 - очистить дисплей

```
void clear_display(struct dev_info* device)
```

#### 4 - получить текущий символ (можно со смещением)

```
unsigned char get_current_symbol(struct dev_info* device, unsigned int)
```

#### 5 - удалить текущий символ (можно со смещением)

```
unsigned char delete_current_symbol(struct dev_info* device, unsigned int)
```

#### 6 - получить значение координаты X

```
unsigned char get_display_cursor_pos_x(struct dev_info* device)
```

#### 7 - получить значение координаты Y

```
unsigned char get_display_cursor_pos_y(struct dev_info* device)
```

#### 8 - установить значение координаты X

```
void set_display_cursor_pos_x(struct dev_info* device, unsigned char)
```

#### 9 - установить значение координаты Y

```
void set_display_cursor_pos_y(struct dev_info* device, unsigned char)
```



## Keyboard


#### 0 - получить адрес буфера клавиатуры

```
void* get_keyboard_buffer(struct dev_info* device)
```

#### 1 - получить индекс (текущее положение) буфера клавиатуры

```
unsigned int get_keyboard_buffer_ptr(struct dev_info* device)
```

#### 2 - получить флаг: нажата ли клавиша shift

```
unsigned char get_keyboard_shift_pressed(struct dev_info* device)
```

#### 3 - получить флаг: нажата ли клавиша ctrl

```
unsigned char get_keyboard_ctrl_pressed(struct dev_info* device)
```

#### 4 - получить флаг: нажата ли клавиша alt

```
unsigned char get_keyboard_alt_pressed(struct dev_info* device)
```
