#№ Отчет по лабораторной работе №4
## по курсу "Логическое программирование"

## Обработка естественного языка

### студент: Минеева Светлана Алексеевна

## Результат проверки

| Преподаватель     | Дата         |  Оценка       |
|-------------------|--------------|---------------|
| Сошников Д.В. |              |               |
| Левинская М.А.|              |               |

> *Комментарии проверяющих (обратите внимание, что более подробные комментарии возможны непосредственно в репозитории по тексту программы)*


## Введение

Обработка естественных и искусственных языков очень актуальна в наше время. Обрабатывать искусственный язык проще, ведь он обладает строгой структурой и в нем чётко определены правила семантики, синтаксиса. Большей проблемой является обработка естественного языка, поскольку он является сложной системой с интересной, гибкой структурой. Существует два метода к обработке естественного языка: статистический и лингвистический. Статистический метод основывается на том, что смысл текста отражается часто встречающимися словами, в данном подходе связность текста не учитывается. Лингвистический метод основывается на разбиении на уровни, что помогает учитывать связи слов и предложений.

Язык программирования Prolog является удобным средством для решения таких задач, поскольку его основа – перебор вариантов, в данном случае грамматики языка, его фактов и правил. Так, Prolog позволяет создавать синтаксические анализаторы, текстовые редакторы, трансляторы языков и многое другое.

## Задание

4. Реализовать синтаксический анализатор арифметического выражения для перевода его в префиксную форму. В выражении допустимы операции +, -, *, /. 

Пример:
```prolog
?- calculate([5, '+', 2, '*', 3], X).
X = [+, 5, *, 2, 3]
```

## Принцип решения 

Сначала вводим алфавит, состоящий из чисел, операторов сложения, вычитания, умножения, деления, а также из знаков левой и правой скобок. Операторы разделяем на две группы (operator, high_operator) в зависимости от приоритета их выполнения. 

```prolog
% Алфавит
element(Element):-
    number(Element).
operator('+').
operator('-').
high_operator('*').
high_operator('/').
left_parenthesis('(').
right_parenthesis(')').
```
Для решения задачи понадобится функция добавления элемента в конец списка:

```prolog
% Вставка элемента в конец списка
insert_last_element([], Element, [Element]).
insert_last_element([Head|Tail], Element, [Head|TailWithElement]):-
    insert_last_element(Tail, Element, TailWithElement).
```

Также понадобится функция реверса списка. Важным моментом в этой функции для данной задачи является то, что при реверсе выражения знак левой скобки должен меняться на знак правой скобки, а знак правой скобки – на знак левой скобки.

```prolog
% Реверс списка
inverter([], X, Y):-
    myappend([], X, Y).
inverter([Head|Tail], X, Y):-
    not(left_parenthesis(Head)),
    not(right_parenthesis(Head)),
    inverter(Tail, [Head|X], Y).
inverter([Head|Tail], X, Y):-
    left_parenthesis(Head),
    right_parenthesis(Element),
    inverter(Tail, [Element|X], Y).
inverter([Head|Tail], X, Y):-
    right_parenthesis(Head),
    left_parenthesis(Element),
    inverter(Tail, [Element|X], Y).

reverse(List1, List2):-
    inverter(List1, [], List2).
```

Сама идея перевода выражения в префиксную форму заключается в применении алгоритма Дейкстры, однако перед применением алгоритма выражение нужно реверсировать и реверсировать после его применения. 

Алгоритм Дейкстры:
1)	Числа заносятся в выходную строку;
2)	Первый оператор заносится в стек. Последующие операторы выталкивают в выходную строку операторы, имеющие приоритет выше рассматриваемого, после рассматриваемый оператор заносится в стек;
3)	Открывающаяся скобка заносится в стек;
4)	Закрывающаяся скобка выталкивает из стека в выходную строку все операторы до открывающейся скобки, открывающаяся скобка удаляется из стека.

Реализация алгоритма для данной задачи:

```prolog
% Алгоритм Дейкстры 
prefix([Head|Tail], Result, Stack, Y):-
    element(Head),
    insert_last_element(Result, Head, Output),
    prefix(Tail, Output, Stack, Y).
prefix([Head|Tail], Result, [], Y):-
    (operator(Head); high_operator(Head); left_parenthesis(Head)),
    prefix(Tail, Result, [Head], Y).
prefix([Head|Tail], Result, Stack, Y):-
    left_parenthesis(Head),
    prefix(Tail, Result, [Head|Stack], Y).
prefix([Head|Tail], Result, [Top|Stack], Y):-
    operator(Head),
    (operator(Top); high_operator(Top)),
    insert_last_element(Result, Top, Output),
    prefix([Head|Tail], Output, Stack, Y).
prefix([Head|Tail], Result, [Top|Stack], Y):-
    operator(Head),
    left_parenthesis(Top),
    prefix(Tail, Result, [Head, Top|Stack], Y).
prefix([Head|Tail], Result, [Top|Stack], Y):-
    high_operator(Head),
    high_operator(Top),
    insert_last_element(Result, Top, Output),
    prefix([Head|Tail], Output, Stack, Y).
prefix([Head|Tail], Result, [Top|Stack], Y):-
    high_operator(Head),
    (operator(Top); left_parenthesis(Top)),
    prefix(Tail, Result, [Head, Top|Stack], Y).
prefix([Head|Tail], Result, [Top|Stack], Y):-
    right_parenthesis(Head),
	not(left_parenthesis(Top)),
    insert_last_element(Result, Top, Output),
    prefix([Head|Tail], Output, Stack, Y).
prefix([Head|Tail], Result, [Top|Stack], Y):-
    right_parenthesis(Head),
    left_parenthesis(Top),
    prefix(Tail, Result, Stack, Y).
prefix([], Result, [], Y):-
    myappend(Result, [], Y).
prefix([], Result, [Top|Stack], Y):-
    insert_last_element(Result, Top, Output),
    prefix([], Output, Stack, Y).
```

## Результаты

```prolog
?- calculate([5, '+', 2, '*', 3], X).
X = [+, 5, *, 2, 3]

?- calculate(['(', 14, '+', 1, ')', '*', 2], X).
X = [*, +, 14, 1, 2]

?- calculate(['(', 1, '+', 2, ')', '*', '(', 3, '+', 4, ')'], X).
X = [*, +, 1, 2, +, 3, 4]
```

## Выводы

Сфера обработки естественных языков сейчас является крайне актуальной. На данный момент идёт бурное развитие голосовых помощников, ботов, машины становятся искусными собеседниками для человека. Решение подобной задачи по силам и языку программирования Prolog.

При выполнении данной лабораторной работы я убедилась, что благодаря языку программирования Prolog можно весьма просто и компактно реализовать синтаксический анализатор выражений, ведь в Prolog можно в простой форме записать характеристики языка, что обеспечивает последующую простую работу с ними. Prolog является удобным средством для решения задач, связанных с обработкой естественных и искусственных языков.
