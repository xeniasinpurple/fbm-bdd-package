# Ordnerstruktur
In den Unterordner dieses Verzeichnises findet ihr Schaltkreise im Aiger Format, welche ihr verifzieren sollt.
## UnkownCircuits
Der Ordner 'UnkownCircuits' enthält die Funktionen, für welche nicht klar ist welche Variablenordnung ihr verwenden könnt. 
Diese sind aufgeteilt in die Unterordner 'ref' für die Referenzschaltkreise sowie 'opt' für die optimierten Schaltkreise.
## Adders
Der Ordner 'Adders' enthält auschließlich Addierer. Die Dateien beginnen alle mit 'u_' das es sich um 'unsigned' Addierer handelt.
Dann folgt eine Abkürzung für die verwendete Architektur gefolgt von einer Zahl, welche die bit-Breite der Addierer ausweist.
Ihr sollt für jede bit-Breite den 'u_rca{n}.aag' als Referenz für alle anderen Architekturen der selben bit-Breite verwenden.
Die ersten {n} Inputs des Schaltkreises repräsentieren den Operand A, die zweiten {n} Inputs des Schalkreises den Operand B der Addition A+B.
