% Импорт данных
:- [‘two.pl’].

% Возвращение списка всех предметов
subjects(L) :- setof(Subject, A^B^C^grade(A, B, Subject, C), L).

% Подсчёт суммы элементов списка
sum([], 0).
sum([H|T], Sum) :- sum(T, Sum1), Sum is Sum1 + H.

% Нахождение среднего значения списка
average(L, X) :- sum(L, Sum), length(L, Length), X is Sum / Length.

% Нахождение среднего балла N по предмету S
average_grade(S, N) :- findall(M, grade(_, _, S, M), L),  average(L, N).

% Вывод среднего балла по всем предметам
print_average_grades(Subject, Average) :- subjects(Subjects), member(Subject, Subjects), average_grade(Subject, Average).

% Нахождение и вывод количества не сдавших студентов группы G
print_failed_exam(G, N) :- setof(Student, A^grade(G, Student, A, 2), L), length(L, N).

% Нахождение количсетва студентов, не сдавших предмет S
failed_subject(S, N) :- findall(Student, grade(_, Student, S, 2), L), length(L, N).

% Вывод списка всех предметов и количества студентов, не сдавших этот предмет
print_failed_subject(Subject, Number) :- subjects(Subjects), member(Subject, Subjects), failed_subject(Subject, Number).
