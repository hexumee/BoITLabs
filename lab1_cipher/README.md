![](https://github.com/hexumee/BoITLabs/blob/main/screenshots/lab1.png?raw=true)

------------

Пусть у нас есть некий алфавит и какое-то ключевое слово, а также заданы размеры будущей таблицы – так называемой таблицы Трисемуса.
<br/><br/>
### 1. Составляем таблицу Трисемуса
1) Выписываем построчно буквы ключевого слова, но, если текущая буква уже попала в таблицу, второй раз её не заносим. Например, в случае английского алфавита без J, ключа DEVELOPER и размера таблицы 5x5 в таблицу заносим D, E, V, L, O, P, R - буква E у нас встречается трижды, учитываем только первое её вхождение.
Порядок записи в таблицу – начинаем с левого верхнего угла, ячейки в строках заполняем слева направо, в случае если строка заполнена, переходим к следующей, которая под заполненной.

2) По порядку дописываем в таблицу буквы алфавита, пропуская те, что были в ключевом слове. При ключе в нашем примере заносим A, B, C, пропускаем D и E, заносим F, G и так до K, пропускаем L и т.д.
Пусть наш алфавит состоит из 25 английских букв, кроме J.
Ключевое слово – DEVELOPER. Тогда выходит такая таблица Трисемуса:

![](https://github.com/hexumee/BoITLabs/blob/main/screenshots/lab1_pic1.png?raw=true)

Зеленым выделено то, что попало в таблицу из ключевого слова.
Если в тексте есть пробелы и знаки препинания, то включаем их в алфавит и в таблицу тоже.

Размер таблицы может быть любым, при этом количество ячеек должно быть строго равно количеству символов алфавита. Так, если в алфавите 21 символ, годятся таблицы 3x7, 7x3. В выше приведённой из 25 ячеек - только 5x5.
<br/><br/>
### 2. Правило замены в шифре Трисемуса
Здесь один символ открытого текста всегда заменяется на один символ шифрованного. Сначала ищем символ в таблице.

Есть 2 случая:
1. Если символ не в нижней строке, он заменяется на тот, что
в таблице стоит сразу под ним.

![](https://github.com/hexumee/BoITLabs/blob/main/screenshots/lab1_pic2.png?raw=true)

2. Если символ в нижней строке, то он заменяется на символ, который находится в том же столбце верхней строки.

![](https://github.com/hexumee/BoITLabs/blob/main/screenshots/lab1_pic3.png?raw=true)

Для каждого символа открытого текста делаем такую замену. Например,
в нашей таблице слово UMBRELLA заменится на DUIGRBBH.

