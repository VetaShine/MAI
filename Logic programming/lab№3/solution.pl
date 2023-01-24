% Определение принадлежности элемента списку
mymember(X, [_|T]) :-
    mymember(X, T), !.
mymember(H, [H|_]). 

% Конкатенация списков в результирующий список
myappend([], L, L).
myappend([X|L1], L2, [X|L3]) :-
    myappend(L1, L2, L3).

% Логические шаги
act(A,B):-
    myappend(S, ["_","w"|T], A),
    myappend(S, ["w","_"|T], B).

act(A,B):-
    myappend(S, ["b","_"|T], A),
    myappend(S, ["_","b"|T], B).

act(A,B):-
    myappend(S, ["_","b","w"|T], A),
    myappend(S, ["w","b","_"|T], B).

act(A,B):-
    myappend(S, ["b","w","_"|T], A),
    myappend(S, ["_","w","b"|T], B).

% Вывод решения
print_solution([]).
print_solution([B|T]):-
    print_solution(T),
    write(B), nl.

% Продление пути без зацикливания
prolong([X|T], [Y,X|T]) :-
    act(X, Y),
    not(mymember(Y, [X|T])). 

% Поиск в глубину
dpth([[X|T]|_], X, [X|T]).

dpth([P|QI], X, R):- findall(Z, prolong(P, Z), T),
                   myappend(T, QI, QO), !,
                   dpth(QO, X, R).

dpth([_|T], Y, L):- dpth(T, Y, L).


% Поиск в ширину
bdth([[X|T]|_], X, [X|T]).

bdth([P|QI], X, R):- findall(Z, prolong(P, Z), T),
                   myappend(QI, T, QO), !,
                   bdth(QO, X, R).

bdth([_|T], Y, L):- bdth(T, Y, L).

% Поиск в глубину с итеративным погружением
dpth_id([X|T], X, [X|T], 0).

dpth_id(X, Y, P, N) :- 
    N > 0, 
    prolong(X, XX), 
    N1 is N - 1, 
    dpth_id(XX, Y, P, N1).

for(X, X, _).
for(I, X, Y):-
    X < Y,
    X1 is X + 1,
    for(I, X1 ,Y).

solve(S, Res):-
    write('ITERATIVE-DEEPENING DEPTH-FIRST SEARCH'), nl,
    get_time(DPTH_ID),
    for(N, 1, 20),
    dpth_id([S], Res, W, N),
    print_solution(W),
    get_time(DPTH_ID1),
    T is DPTH_ID1 - DPTH_ID,
    write('TIME: '), write(T), nl, nl,
    
    write('DEPTH-FIRST SEARCH'), nl,
    get_time(DPTH),
    dpth([[S]], Res, W),
    print_solution(W),
    get_time(DPTH1),
    T1 is DPTH1 - DPTH,
    write('TIME: '), write(T1), nl, nl,
    
    write('BREADTH-FIRST SEARCH'), nl,
    get_time(BDTH),
    bdth([[S]], Res, W), 
    print_solution(W),
    get_time(BDTH1),
    T2 is BDTH1 - BDTH,
    write('TIME: '), write(T2), nl, nl,
    !.
