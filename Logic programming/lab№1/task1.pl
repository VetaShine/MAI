% Определение длины списка
mylen([], 0).
mylen([_|T], N) :-
    mylen(T, N1),
    N is N1+1, !.

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
mysublist(L, T) :-
    myappend(Sublist, _, T),
    myappend(_, L, Sublist), !.

% Замена N-го элемента на указанный (на X) без стандартных предикатов
replace(X,0,[_|L],Res):-
    myappend([X],L,Res),!.
replace(X,N,[H|L],Res):-
    N1 is N - 1,
    replace(X,N1,L,R),
    myappend([H],R,Res),!.

% Замена N-го элемента на указанный (на X) со стандартными предикатами:
stdreplace(X, N, L, Res) :-
    N>=0,
    length(HelpL, N),
    append(HelpL, Tail, L),
    [_|TTail]=Tail,
    append(HelpL, [X|TTail], Res).

% Разделение списка на два относительно первого элемента (по принципу «больше-меньше») без стандартных предикатов
separation(_, [], [], []).
separation(X, [H|L], Less, More) :-
    (   H>X,
        separation(X, L, Less, More1), !,
        myappend([H], More1, More)
    ;   H<X,
        separation(X, L, Less1, More), !,
        myappend([H], Less1, Less)
    ;   H is X,
        separation(X, L, Less, More)
    ).

separation([X|L], Less, More) :-
    separation(X, L, Less, More).

% Разделение списка на два относительно первого элемента (по принципу «больше-меньше») со стандартными предикатами:
stdseparation([X|L],Less,More):-
    findall(N,(member(N,L),N < X),Less),
    findall(N,(member(N,L),N > X),More).
