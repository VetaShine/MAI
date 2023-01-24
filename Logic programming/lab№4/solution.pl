% Алфавит 
element(Element):-
    number(Element).
operator('+').
operator('-').
high_operator('*').
high_operator('/').
left_parenthesis('(').
right_parenthesis(')').

% Конкатенация списков в результирующий список
myappend([], List, List).
myappend([Element|List1], List2, [Element|List3]) :-
    myappend(List1, List2, List3).

% Вставка элемента в конец списка
insert_last_element([], Element, [Element]).
insert_last_element([Head|Tail], Element, [Head|TailWithElement]):-
    insert_last_element(Tail, Element, TailWithElement).

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

calculate(Start, Result):-
    reverse(Start, Interval),
    prefix(Interval, [], [], Upshot),
    reverse(Upshot, Result), !.
