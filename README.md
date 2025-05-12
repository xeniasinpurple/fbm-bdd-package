# fbm-bdd-package

BDD Package für den Kurs Formale Beweismethoden, im Wintersemester 2024/2025 an der Universität Bremen.

**Anmerkung:** Es wurde keine effiziente, veränderbare Variablenordnung implementiert, weswegen alle BDDs und KFDDs eine Variablenordnung der Form $x_1 > x_2 > \cdots > x_{n-1} > x_n$ besitzen.

In der Datei ```main.c``` ist eine Beispielausführung, mit der ein BDD als ROBDD oder als KFDD erstellt werden kann und die Ausführungen des Aiger-Parsers für 8-Bit Addierer.

## Compiling & Ausführung
Nachdem der Source Code gedownloaded wurde, sollte der [Gnu C Compiler](https://gcc.gnu.org/) auf dem System installiert sein. Mit diesem wird der naive Compile Aufruf ```gcc ./src/*.c main.c -o bdd_pack -Wall``` ausgeführt.
Das Programm wird dann durch den Aufruf ```.\bdd_pack``` ausgeführt.

## Aufgabe 1
Um einen ROBDD zu erstellen müssen zuerst alle gewünschten Variablen erstellt werden in der Form ```struct bdd_node *node1 = bdd_create_node(size_t variable, struct bdd_node *low_child, struct bdd_node *high_child)```. Wobei alle Low Kinder zum Anfang an das Terminal Null sein sollten und alle High Kinder das Terminal Eins. Diese sind bereits vordefiniert.

Mehrere BDDs/BDD Nodes können mit den Funktionen ```bdd_and, bdd_nand, bdd_or, bdd_nor, bdd_xor, bdd_xnor und bdd_negate``` konkateniert oder negiert werden.

Um diese am Ende graphisch darzustellen, können diese mit der Funktion ```bdd_to_dot``` zu dot-Files konvertiert werden und vom User zu Bilddateien umgewandelt werden. In Visual Studio Code können die Dotfiles mit dem Plugin *Graphviz Preview* angezeigt werden.


## Aufgabe 2
Um ein ROBDD zu ein KFDD zu konvertieren muss zuerst eine Decomposition Type List erstellt werden mit ```dtl_create(size_t size)```.
Diese Liste kann befüllt werden mit der Funktion ```dtl_add(enum decomposition *e, size_t index, enum decomposition d)```. Hierbei steht ```d = 0``` für die Shannon-Zerlegung, ```d = 1``` für die positiv Davio-Zerlegung und ```d = 2``` für die negativ Davio-Zerlegung.

Um einen vorher erstellten ROBDD (siehe Aufgabe 1) nun zu einen KFDD zu konvertieren, muss die Funktion ```bdd_to_kfdd(struct bdd_node *f)``` aufgerufen werden. Diese verwandelt dann den eingegebenen ROBDD ```f``` in einen KFDD im Bezug auf die DTL.

## Aufgabe 3
Der implementierte Aiger Parser funktioniert für alle angegebenen 8-Bit Addierer in /benchmarks/Adders. Bei den Addieren mit höheren Bit-Werten wird es Stand jetzt (06.03.2025) zu langen Berechnungszeiten kommen.
Zudem gibt es bisher (06.04.2025) keine Möglichkeit die gesamte Knotenanzahl zu ermitteln, weswegen nur die Ausführungszeit am Ende ausgegeben wird.

### Aufruf
Ein Array an BDDs kann erzeugt werden durch den Aufruf von ```aiger_parser('directory', 'file_name')```.
Die einzelnen BDDs können dann durch die Funktion ```bdd_to_dot(bdd[i], 'output_directory/file_name.dot')``` als ein Dot-File ausgegeben werden und durch das Visual Studio Code Plugin *Graphviz Preview* als Bild im Code-Editor angezeigt werden.