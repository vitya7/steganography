# steganography

## Кодирование файла

закодировать файл text.txt в tiger.bmp и записать результат в tiger_with_text.bmp :

`steg -encode -i tiger.bmp -src text.txt -o tiger_with_text.bmp -bits 5 -skip 31`

декодировать файл из tiger_with_text.bmp и записать результат в text_from_tiger.txt :

`steg -decode src -i tiger_with_text.bmp -o text_from_tiger.txt -bits 5 -skip 31`


* `-encode` : режим кодирования
* `-decode src` : режим декодирования файла

* `-i` : файл в который кодировать
* `-src` : файл который кодировать
* `-o` : результат кодирования

* `-bits 5` : задействовать 5 младших битов
* `-skip 31` : задействовать каждый 32 байт

## Кодирование текста

закодировать текст "some text" в tiger.bmp и записать результат в tiger_with_text.bmp :

`steg -encode -i tiger.bmp -t "some text" -o tiger_with_text.bmp -bits 5 -skip 31`

декодировать текст из tiger_with_text.bmp и вывести результат :

`steg -decode t -i tiger_with_text.bmp -bits 5 -skip 31`

* `-decode t` : режим декодирования текста

* `-t` : текст для кодирвания
