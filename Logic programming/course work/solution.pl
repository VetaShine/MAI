:- discontiguous father/2.
:- discontiguous mother/2.

:- include("My_family.pl").

% Определение пола индивидуума
female(Person) :- mother(Person,_), !.
male(Person) :- father(Person,_), !.


% Определение тёщи
mother_in_law(Mother_in_law, Person):-
    mother(Mother_in_law, Wife),
    mother(Wife, Child),
    father(Person, Child).

% Определение мужа
husband(Person, Husband) :- 
    mother(Person,Child), 
    father(Husband, Child),!.

% Определение жены
wife(Person, Wife) :- 
    father(Person, Child), 
    mother(Wife, Child), !.

% Определение сына
son(Person, Son) :- 
    father(Person, Son), 
    male(Son), !.

son(Person, Son) :- 
    mother(Person, Son), 
    male(Son), !.

% Определение дочери
daughter(Person, Daughter) :-  
    father(Person, Daughter), 
    female(Daughter), !.

daughter(Person, Daughter) :-  
    mother(Person, Daughter), 
    female(Daughter), !.

% Определение брата
brother(Person, Brother) :- 
    father(Father, Person), 
    father(Father, Brother), 
    Person \= Brother, 
    male(Brother).

brother(Person, Brother) :- 
    mother(Mother, Person), 
    mother(Mother, Brother),  
    Person \= Brother, 
    male(Brother).

% Определение сестры
sister(Person, Sister) :- 
    father(Father, Person), 
    father(Father, Sister), 
    Person \= Sister, 
    female(Sister).

sister(Person, Sister) :- 
    mother(Mother, Person), 
    mother(Mother, Sister), 
    Person \= Sister, 
    female(Sister).

% Определение дедушки
grandfather(Person, Grandfather) :- 
    father(Father, Person), 
    father(Grandfather, Father).

grandfather(Person, Grandfather) :- 
    mother(Mother, Person), 
    father(Grandfather, Mother). 

% Определение бабушки
grandmother(Person, Grandmother) :- 
    father(Father, Person), 
    mother(Grandmother, Father).

grandmother(Person, Grandmother) :- 
    mother(Mother, Person), 
    mother(Grandmother, Mother).


% Состояния
mother(mother).
father(father).
grandfather(grandfather).
grandmother(grandmother).
son(son).
daughter(daughter).
wife(wife).
husband(husband).
brother(brother).
sister(sister).
mother_in_law(mother_in_law).

% Логические шаги
move(First_person, Second_person, State) :- 
    mother(Second_person, First_person), 
    mother(State).
move(First_person, Second_person, State) :- 
    father(Second_person, First_person), 
    father(State).
move(First_person, Second_person, State) :- 
    grandfather(First_person, Second_person), 
    grandfather(State).
move(First_person, Second_person, State) :- 
    grandmother(First_person, Second_person), 
    grandmother(State).
move(First_person, Second_person, State) :- 
    husband(First_person, Second_person), 
    husband(State).
move(First_person, Second_person, State) :- 
    wife(First_person, Second_person), 
    wife(State).
move(First_person, Second_person, State) :- 
    brother(First_person, Second_person), 
    brother(State).
move(First_person, Second_person, State) :- 
    sister(First_person, Second_person), 
    sister(State).
move(First_person, Second_person, State) :- 
    son(First_person, Second_person), 
    son(State).
move(First_person, Second_person, State) :- 
    daughter(First_person, Second_person), 
    daughter(State).
move(First_person, Second_person, State) :- 
    mother_in_law(Second_person, First_person), 
    mother_in_law(State).


% Продление пути без зацикливания
prolong([X|T],[Y,X|T]):-
    move(X,Y,_),
    not(member(Y,[X|T])).

% Поиск в ширину	
bdth([[X|T]|_],X,[X|T]).

bdth([P|QI], X, R):-
    findall(Z, prolong(P, Z), T),
    append(QI, T, QQ), !,
    bdth(QQ, X, R).
	
bdth([_|T], Y, L):- bdth(T, Y, L).
	
bdth_search(X,Y,L):-
    bdth([[X]],Y,R), 
    reverse(R,L).

% Получение имени и фамилии из цепочки родства
convert([_|[]], []) :- !.

convert([First_person, Second_person|Tail], Result) :- 
    move(First_person, Second_person, State), 
    append(End,[State], Result), 
    convert([Second_person|Tail], End), !.

% Вывод цепочки родства
relative(L,X,Y) :- 
    bdth_search(X,Y,R), 
    convert(R,L).

individual(Person):- mother(Person, _); mother(_, Person).
individual(Person):- father(Person, _); father(_, Person).


% Естественно-языковой интерфейс
	
% Запоминание имени и фамили индивидуума, введеного последним 
writing_female(First_person, Second_person):- 
    female(Second_person), !, nb_setval(last_female, Second_person);
    female(First_person), nb_setval(last_female, First_person).

writing_male(First_person, Second_person):- 
    male(Second_person), !, nb_setval(last_male, Second_person);
    male(First_person), nb_setval(last_male, First_person).


% Получение имени и фамилии
getting(Pronoun,Pronoun_list):- 
    member(Pronoun,[her]), !, 
    nb_getval(last_female, Pronoun_list).

getting(Pronoun,Pronoun_list):- 
    member(Pronoun,[his]), !, 
    nb_getval(last_male, Pronoun_list).


% Перевод в единственное число
singular(mothers_in_law, mother_in_law).
singular(husbands, husband).
singular(wifes, wife).
singular(sons, son).
singular(daughters, daughter).
singular(brothers, brother).
singular(sisters, sister).
singular(mothers, mother).
singular(grandfathers, grandfather).
singular(grandmothers, grandmother).

change(he, his).
change(she, her).


% Проверка корректности вывода
write_n(X, Y, N):- 
    N > 1, singular(X, Y); 
    X = Y, !.


subject(Slovo) --> person(Slovo).
subject(V) --> pronoun(First_slovo, pnoun), {change(First_slovo, Third_slovo), getting(Third_slovo, V)}.

object(Slovo, Second_slovo, N) --> person(Slovo), ["'s"], relation(Second_slovo, N).
object(V, Slovo, N)--> pronoun(First_slovo, prnoun),{getting(First_slovo, V)}, relation(Slovo, N) .
object(V, Slovo, N)--> pronoun(First_slovo, pnoun), {change(First_slovo, Third_slovo), getting(Third_slovo, V)}, relation(Slovo, N).
object(Slovo) --> preposition, person(Slovo).
object(V) --> preposition, pronoun(First_slovo, prnoun), {getting(First_slovo, V)}.

verb(N) --> to_be(N).
verb(N) --> be(N).

word --> [who].
word -->['Who'].
words --> [how], [many].
words --> ['How'], [many].
mark --> [?].
preposition --> [for].
pronoun(Slovo, prnoun) --> {member(Slovo, [his, her])}, [Slovo].
pronoun(Slovo, pnoun)--> {member(Slovo, [he, she])}, [Slovo].
have(say) --> [has].
have(speech) --> [have].
to_be(say) --> [is].
to_be(say) --> ['Is'].
to_be(speech) --> [are].
to_be(speech) --> ['Are'].
be(say) --> [does].
be(speech) --> [do].


relation(Slovo, say) --> {member(Slovo, [father, mother, wife, husband, sister, brother, grandfather, grandmother, 
    son, daughter, mother_in_law])}, [Slovo].
relation(Slovo, speech) --> {member(Slovo, [fathers, mothers, wifes, husbands, sisters, brothers, grandfathers, 
    grandmothers, sons, daughters, mothers_in_law])}, [Slovo].


person(X) --> {setof(Y, individual(Y), L), member(X, L)}, [X].


solution(Sentence):- solution(Sentence,[]).

solution --> to_be(N), subject(Slovo), object(X, Y, N), mark, 
             {first_answer(Slovo, X, Y)}, !, {writing_female(Slovo, X), writing_male(Slovo, X)}.

solution --> word, to_be(N), subject(First_person), object(Second_person), mark, 
             {second_answer(First_person, Second_person, 0)}, !, 
             {writing_female(First_person, Second_person), writing_male(First_person, Second_person)}.

solution --> word, to_be(N), object(X, Y, N), mark,
             {second_answer(0, X, Y)}, !, {writing_female(_, X)}.
            
solution --> words, relation(Word, speech), verb(N), subject(First_person), have(speech), mark,
             {singular(Word, Singular), third_answer(Singular, First_person)}, !, 
             {writing_female(_, First_person)}.


first_answer(First_person, Second_person, Result) :- 
    relative([Result], Second_person, First_person), !, 
    write("Yes, "), write(First_person), write(" is "), write(Second_person), write("'s "), write(Result), write("."); 
	write("No, "), write(First_person), write(" is not "), write(Second_person), write("'s "), write(Result), write(".").
			 
second_answer(0, Second_person, Result) :- 
    relative([Result], Second_person, First_person), !,
    write(First_person), write(" is "), write(Second_person), write("'s "), write(Result), write(".");
    write("We can't find "), write(Second_person), write("'s "), write(Result), writeln(".").
			  
second_answer(First_person, Second_person, 0):- 
    relative([Result], Second_person, First_person), !,
    write(First_person), write(" is "), write(Second_person), write("'s "), write(Result), write(".");
    write("We don't have enough information about "), write(First_person), write(" and "), write(Second_person), write(" to answer this solution.").						  
						  
third_answer(R, Person) :- 
    setof(First_person, relative([R], Person, First_person), L), length(L, N), !, 
    write(Person), write(" has "), write(N), write_n(X, R, N), write(" "), write(X), write(".");
    write(Person), write(" don't have any "), singular(X, R), write(X), write(".").
