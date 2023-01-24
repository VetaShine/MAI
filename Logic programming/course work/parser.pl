% Преобразование файла в формате GEDCOM в набор утверждений на Prolog

% Получение имени и фамилии индивидуума
get_id_line(Number, [First_element, Second_element, Third_element|_], Id, End) :-
    First_element = "0",
    Third_element = "INDI",
    Second_element = Id,
    read_string(Number, "\n", "\r", _, _), % пропуск строки вида: имя /фамилия/
    read_string(Number, "\n", "\r", _, Name),
    split_string(Name, " ", "", Name_list),
    get_third_element(Name_list, Resulting_Name),
    read_string(Number, "\n", "\r", _, _), % пропуск строки с фамилией, которую индивидуум имел при рождении
    read_string(Number, "\n", "\r", _, Surname),
    split_string(Surname, " ", "", Surname_list),
    get_third_element(Surname_list, Resulting_Surname),
    string_concat(Resulting_Name, " ", Result),
    string_concat(Result, Resulting_Surname, End).

get_id_line(_, _, _, _).


get_id_file(Number, _, _) :-
    at_end_of_stream(Number), !.

get_id_file(Number, Id, End) :-
    read_string(Number, "\n", "\r", _, String),
    split_string(String, " ", "", Subject),
    get_id_line(Number, Subject, Id, End),
    get_id_file(Number, Id, End).


get_id(Id, End) :-
    open('/Users/macbookpro/Desktop/логическое программирование/My_family.ged', read, Number),
    get_id_file(Number, Id, End).


% Запись данных в формате предикатов father(отец, потомок) и mother(мать, потомок) 
print_result(_, _, [], _).

print_result(Husband_id, Wife_id, [Person_id|Child_id], Output) :-
    get_id(Person_id, H),
    get_id(Husband_id, Husband),
    get_id(Wife_id, Wife),
    write(Output, "father(\""), write(Output, Husband), write(Output, "\", \""), write(Output, H), write(Output, "\").\n"),
    write(Output, "mother(\""), write(Output, Wife), write(Output, "\", \""), write(Output, H), write(Output, "\").\n"),
    print_result(Husband_id, Wife_id, Child_id, Output).


% Проверка: является ли индивидуум ребенком
child([_, "CHIL", _]).


% Проверка: является ли строка информацией о родителях индивидуума
line_split(Input, Output, [First_element, _, Third_element|_]) :-
    First_element = "0",
    Third_element = "FAM",
    read_string(Input, "\n", "\r", _, Husband_string),
    split_string(Husband_string, " ", "", Husband_list),
    get_third_element(Husband_list, Husband),
    read_string(Input, "\n", "\r", _, Wife_string),
    split_string(Wife_string, " ", "", Wife_list),
    get_third_element(Wife_list, Wife),
    read_childs(Input, Child),
    print_result(Husband, Wife, Child, Output), !.

line_split(_, _, _).


% Результат - третий элемент списка
get_third_element([_, _, End], End).


% Получение элементов списка
read_childs(Input, Childs) :-
    read_string(Input, "\n", "\r", _, String),
    split_string(String, " ", "", String_list),
    child(String_list),
    get_third_element(String_list, Resulting_Child),
    read_childs(Input, Child_element),
    Childs = [Resulting_Child|Child_element].

read_childs(_, []).


% Разделение файла по строкам
file_split(Input, _) :-
    at_end_of_stream(Input), !.

file_split(Input, Output) :-
    read_string(Input, "\n", "\r", _, String),
    split_string(String, " ", "", Subject),
    line_split(Input, Output, Subject),
    file_split(Input, Output).


:-
    open('/Users/macbookpro/Desktop/логическое программирование/My_family.ged', read, Input),
    open('/Users/macbookpro/Desktop/логическое программирование/My_family1.pl', write, Output),
    file_split(Input, Output),
    close(Input),
    close(Output).
