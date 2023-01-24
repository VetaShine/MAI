:- discontiguous father/2.
:- discontiguous mother/2.

:- include("My_family.pl").

mother_in_law(Mother_in_law, Person):-
    mother(Mother_in_law, Wife),
    mother(Wife, Child),
    father(Person, Child).
