NUME: Ciobanu Andrei-Bogdan
GRUPA: 312 CB

Tema 1 - A trip throughout TV Series?

Part 1 - Bits & Pieces
    Part 1.1 - Bit by Bit
        Pentru implementarea funcțiilor de la acest task, s-au folosit măștile
        și operatorii pe biți.

    Part 1.2 - One gate to rule them all
        Acest task a fost rezolvat urmând schemele din enunț. Unde a fost
        cazul, s-au utilizat porțile definite la primul task.

    Part 1.3 - Just Carry the Bit
        1 Bit Full Adder:
        Formula pentru sumă este următoarea:
            suma = a ^ b ^ c
        Formula pentru carry a fost aflată cu ajutorul tabelului de adevăr:
            carry = b*c + a*c + a*b
        Pentru codificare, biții au fost setați astfel: bitul de pe poziția 0
        reprezintă bitul de sumă, iar cel de pe poziția 1 reprezintă bitul de
        carry.

        Ripple Carry Adder:
        Se parcurg cei doi operanzi bit cu bit, iar pentru adunarea biților se
        utilizează 1 Bit Full Adder. Foarte important, dacă în urma ultimei
        adunări bitul de carry este setat, atunci adunarea celor doi operanzi
        produce „overflow”. În acest caz, rezultatul este 0.


Part 2 - Shut up Morty!
    Part 2.1 - The Beginning
        Acest task se bazează pe funcțiile send_squanch() și recv_squanch().
        Codificarea sau decodificarea unui caracter depinde de poziția acestuia
        față de caracterul 'A' în ASCII, UTF-8 etc. Caracterele de la 'A' la
        'Z' sunt consecutive în aceste sisteme de codificare uzuale.

    Part 2.2 - Waiting for the Message
        Codificarea și decodificarea caractelor au loc precum în taskul
        precedent. Spre deosebire de primul task, înainte de a trimite mesajul,
        trebuie trimisă lungimea acestuia, codificată. Codificarea sau
        decodificarea lungimii se face prin deplasare de biți, ținând cont de
        imaginea din enunț.

    Part 2.3 - In the Zone
        Pentru codificarea mesajului, se extrag rând pe rând biți din cele două
        caractere și se plasează în noul octet pe poziția corespunzătoare.
        La decodificare, octetul se parcurge din doi în doi biți. De fiecare
        dată, bitul de pe poziția curentă aparține caracterului al doilea, iar
        bitul de pe poziția următoare aparține primului caracter. Se extrag
        biții prin deplasări la dreapta și la stânga, apoi se plasează în noul
        octet pe poziția potrivită, în funcție de caracterul de care „aparțin”.
        Primii patru biți sunt destinați celui de-al doilea caracter, iar
        ceilalți patru biți sunt destinați primului caracter, conform imaginii
        din enunț.


Part 3 - Toss a coin to your Witcher
    Part 3.1 - Axii
        Pentru a găsi vraja criptată, se parcurge memoria. După identificarea
        grupului de cinci biți consecutivi setați, următorii 16 biți reprezintă
        vraja.
        Pentru a găsi cheia de decriptare, se parcurge memoria. Se contorizează
        poziția curentă în memorie. După identificarea grupului de trei biți
        consecutivi setați, poziția curentă este cea a ultimului bit consecutiv
        setat, iar poziția din care începe cheia de decriptare este cu 18 biți
        în urmă.
        Decriptarea se face conform formulei:
            spell_encrypted = spell_plaintext ^ key

    Part 3.2 - Sword Play
        Se numără biții setați din variabila enemy și se determină tipul de
        inamic. Se configurează tipul de sabie în funcție de natura inamicului,
        prin setarea celor mai semnificativi patru biți.
        Dacă inamicul este om, valoarea formată din primii 16 biți trebuie să
        fie (~enemy + 1).
        (~enemy + enemy) face ca toți cei 16 biți să fie 1.
        (~enemy + 1 + enemy) face ca toți cei 16 biți să fie 0. (17th bit == 1)
        Dacă inamicul este monstru, valoarea formată din primii 16 biți trebuie
        să fie (Enemy & (1 - Enemy)), căci ieșirea unei porți XOR este 0 dacă
        și numai dacă cei doi operanzi au aceeași valoare (0 sau 1).

    Part 3.3 - The Witcher Trials
        Trial of the Grasses:
        Din proprietatea a doua (antibodies_low & antibodies_high = 0) rezultă
        că antibodies_low și antibodies_high nu prezintă biți de 1 comuni.
        Din proprietatea a treia (antibodies_low | antibodies_high = cocktail)
        rezultă că cele două componente, împreună, formează anticorpi egali cu
        cocktail-ul ce trebuie neutralizat.
        Prima proprietate se poate prelucra. Primul operand se poate reduce la
        antibodies_high. Al doilea operand se poate reduce la cocktail.
        Astfel, prima proprietate se transformă în:
            antibodies_high ^ cocktail = 0
        Pentru ca proprietatea cu XOR să aibă la ieșire valoare 0, trebuie ca
        cei doi operanzi să aibă aceeași valoare. Așadar:
            antibodies_high = cocktail
        Totodată, (antibodies_low & antibodies_high = 0). De aici reiese:
            antibodies_low = 0
        Se generează anticorpii conform celor de mai sus.

        Trial of Forest Eyes:
        Pentru Wolven Glade, variabila map trebuie să aibă toți biții setați.
        Negarea fiecărui bit din map (~map) conduce la (~map == 0) și astfel
        se identifică pădurea Wolven Glade.
        Pentru Hindar, se creează o mască care are numai doi biți setați în
        mijloc. Dacă map coincide cu masca, pădurea Hindar este identificată.
        Pentru a distinge Brokilon de Caed Dhu, se verifică toate grupurile de
        biți consecutivi setați. Dacă se găsește un grup care nu este format
        din strict patru biți setați, pădurea este Caed Dhu. Dacă se găsesc
        numai grupuri de patru biți setați sau nu există biți setați în map,
        pădurea este Brokilon.

        Trial of the Dreams:
        Se parcurge map bit cu bit. Se caută primul bit setat, care reprezintă
        poziția candidatului. Distanța începe să crească din acel moment, până
        când se găsește poziția portalului, adică al doilea bit setat.
