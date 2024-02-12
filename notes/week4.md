1 - Counting sort

Tie breaks on smallest indices
Needs two tables - Can use memcopy - O(n) but could be parallelized to -> O(n/p)
Framework for doing sorting. Kan ta utgangspunktet i ett av programmene vi har brukt i forelesning.
da får man bedre resultat. slipper dill dall programmering


2 - Monte carlo for pi - hvordan genererer man tilfeldige verdier? To verdier, x og y.
I lecture filene ligger filene som ble brukt i forelesning. der ligger det også threadsafe RNG.

Her kan jeg stresse litt at dette med seed verdier kan være bra for debugging (ha samme verdi for å få samme tall)
Bruker man klokkeslettet risikerer man at man får samme verdier. (Threads kjører på samme tid)



3 - Bestemme primtall i et gitt intervall fra a til b (dele fra to til kvadratroten av veriden (laveste hele primtall)), eller eratostenes soll
