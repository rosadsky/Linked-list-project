# Spájaný zoznam filmov

### Projekt 2 | Predmet:  Základy procedurálneho programovania 2

## Funkcionalita

```c 
FILM *nacitaj(FILM *filmy)
```
-   načítanie a vytvorenie spájaného zoznamu zo súboru filmy.txt.
```c 
void  vypis(FILM *filmy)
```
-   výpis celého zoznamu filmov aj so zoznamami hercov.
```c 
void  pridaj(FILM *filmy)
```
-  pridanie filmu aj so zoznamom hercov na koniec zoznamu
```c 
void  vymaz (FILM *filmy)
```
-   vymazanie filmu aj s príslušným zoznamom hercov
```c 
void  najdiFilmy (FILM *filmy)
```
-   výpis filmov, v ktorých hral daný herec
```c 
void  najdiHercov (FILM *filmy)
```
-   výpis hercov, ktorí hrali v oboch z 2 vybraných filmoch
```c 
void  rok (FILM *filmy) 
```
-   vytvorenie, výpis a následné zmazanie usporiadaného zoznamu hercov, ktorí hrali v niektorom z filmov zadaného roku
```c 
void  koniec(FILM *filmy_vymaz)
```
-   ukončenie programu, pričom sa uvoľní všetka dynamicky vytvorená pamäť
