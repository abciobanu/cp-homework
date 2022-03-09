NUME: Ciobanu Andrei-Bogdan
GRUPA: 312 CB

Tema 2 - A Game of Scrabble

    Matricea pentru tabla de joc este declarată în main() și ulterior populată,
fiind nevoie de ea în rezolvarea tuturor cerințelor.

    Pentru completarea task-ului 0, se afișează tabla de joc cu print_board().
Pentru celelalte task-uri, se apelează read_lines().

    Funcția read_lines() citește datele de intrare, linie cu linie, și le
procesează. Pentru cerința 3, se citesc substringurile cu read_substrings() și
se memorează într-un vector. În continuare, se citesc cele N linii, N fiind dat
la intrare, și se interpretează.

    Coordonatele pentru cuvânt, direcția de scriere și cuvântul propriu-zis
sunt extrase cu get_word_attributes(). Cuvântul este plasat în matrice, apoi
este trecut în vectorul de cuvinte folosite pe tabla de joc. Pentru a calcula
punctajul (cu bonus), se folosește funcția process_word().

    get_bonus_type() returnează tipul de bonus pentru cuvântul dat ca argument,
iar check_bonus() returnează multiplicatorul de bonus, în funcție de poziția
în tabla de bonusuri și tipul de bonus aflat anterior.

    process_word() returnează punctajul necesar în actualizarea punctajelor
jucătorilor - la cerințele 2, 3 și 5. În cazul cerinței 6, liniile citite sunt
pentru Player 1. În acest sens, se actualizează numai punctajul pentru Player 1
și se caută următorul cuvânt ce trebuie jucat de Player 2 cu find_next_word().

    După ce au fost procesate toate datele de intrare, pentru task-ul 1 se
afișează tabla de joc cu print_board(), pentru task-urile 2 și 3 se afișează
punctajele în formatul specificat în enunț cu print_score(), pentru task-urile
4 și 5 se găsește următorul cuvânt cu find_next_word(), apoi se afișează
matricea, iar în cazul cerinței 6, se afișează tabla de joc, urmată de
câștigător, în formatul cerut.

    find_next_word() este folosită pentru task-urile 4, 5 și 6. Se caută în
lista de cuvinte words[] din scrabble.h cuvântul potrivit, în funcție de
condițiile din enunțul fiecărei cerințe. De fiecare dată când se trece la un
nou cuvânt din words[], se verifică dacă a mai fost folosit în matricea de joc.
Funcția check_space() verifică suprapunerea cu alte cuvinte și ieșirea de pe
tabla de joc.

    Pentru cerința 4, primul cuvânt eligibil găsit este trecut în matrice cu
ajutorul lui place_word(). Pentru task-urile 5 și 6, se caută cuvântul optim.
Punctajul unui cuvânt se obține precum în read_lines(), cu process_word().
Pentru a determina dacă un cuvânt găsit este optim până la acel moment, se
folosește funcția is_best(). Aceasta verifică condițiile de cuvânt optim de la
cerințele 5 și 6.

    Dacă cel mai bun cuvânt găsit pentru task-ul 5 nu conduce la un punctaj cel
puțin egal cu cel al Player-ului 2, atunci se afișează „Fail!”. În caz contrar,
cuvântul optim este trecut pe tabla de joc. În cazul cerinței 6, cel mai bun
cuvânt găsit este trecut în lista de cuvinte găsite pentru Player 2, punctajul
pentru jucător este actualizat, iar mai apoi matricea este actualizată cu noul
cuvânt.
