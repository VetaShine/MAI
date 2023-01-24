% Определение длины списка
mylen([], 0).
mylen([_|T], N) :-
    mylen(T, N1),
    N is N1+1.

% Определение принадлежности элемента списку
mymember(X, [_|T]) :-
    mymember(X, T), !.
mymember(H, [H|_]).

% Конкатенация списков в результирующий список
myappend([], L, L).
myappend([X|L1], L2, [X|L3]) :-
    myappend(L1, L2, L3).

% Удаление элемента из списка и помещение результирующего списка в новый список
rm(X, [X|T], T).
rm(X, [M|T], [M|T1]) :-
    rm(X, T, T1), !.

% Перестановка элементов списка
mypermute([], []).
mypermute([X|T], R) :-
    rm(X, R, T1),
    mypermute(T, T1).

% Проверка – является ли один список подсписком другого
mysublist(List, T) :-
    myappend(Sublist, _, T),
    myappend(_, List, Sublist), !.

%-------------------------------------------------------------------------------

gods([truth, lie, dipl]). % Список всех богов

godsay(pos(left, truth)).  % Бог слева сказал, что посередине бог правды
godsay(pos(center, dipl)). % Бог посередине сказал, что он - бог дипломатии
godsay(pos(right, lie)).  % Бог справа сказал, что посередине бог лжи

writepos(left, V, [V, _, _]).
writepos(center, V, [_, V, _]).
writepos(right, V, [_, _, V]).

change(left, right).
change(right, left).

solve :-
    gods(GodList),
    List=[_, _, _],
    godsay(pos(center, Center)),  
    (   Center==truth   % Посередине может быть любой бог
    ;   Center==lie, % То, что он - бог лжи, может сказать только бог дипломатии
        writepos(center, dipl, List),
        godsay(pos(Truth, dipl)), % Известно, что посередине бог дипломатии, тот, кто назвал его богом дипломатии, бог правды
        writepos(Truth, truth, List),
        godsay(pos(Truth, TrueGod)),
        writepos(center, TrueGod, List)
    ;   Center==dipl,
        (   godsay(pos(X, truth)), % Посередине точно не бог правды, тот, кто назвал бога посередине богом правды, лжёт
            change(X, Y),
            writepos(Y, truth, List),
            godsay(pos(Y, TrueGod)),
            writepos(center, TrueGod, List)
        ;   mypermute([lie, dipl], [God, _]),
            writepos(center, God, List) % Нельзя определить богов однозначно, можно лишь предположить варианты их расположения
        )
    ),
    mypermute(GodList, List), !,
    writeln(List).
