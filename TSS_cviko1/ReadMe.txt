Úprava jasu v spracovaní obrázkov (S použitím vlákien)

Táto funkcia umožňuje používateľom upraviť jas obrázka - variácie jasu (jasnejší alebo tmavší) a smeru (hore, dole, doľava, doprava). 
Spracovanie obrázkov prebieha v pozadí na samostatnom vlákne. Keď je efekt použitý na obrázok, uloží sa do cache, aby sa zabránilo 
opakovaniu rovnakého efektu. Cache uchováva kombináciu efektu (jas a smer) a výsledný upravený obrázok. 
Effect je navrhnutý na spracovanie až 8 kombinácií úpravy jasu a smeru:

Jasnejší + Hore
Jasnejší + Dole
Jasnejší + Doľava
Jasnejší + Doprava
Tmavší + Hore
Tmavší + Dole
Tmavší + Doľava
Tmavší + Doprava

Postup

1. Voľba používateľa
Používateľ vyberie obrázok a zvolí kombináciu jasu a smeru. Jas môže byť buď jasnejší alebo tmavší o 30%
Smer je jeden zo štyroch smerov: hore, dole, doľava alebo doprava. 
V Menu su jednotlivé kombinacie ošetrené, tak aby si používateľ mohol zvoliť iba jednu z 8 kombinacií.

2. Aplikácia efektu na pozadí
Po vybraní efektu systém skontroluje, či bola kombinácia efektu už aplikovaná a uložená do cache. 
Ak bola, použije sa uložený výsledok. Ak efekt ešte nebol aplikovaný: Základný obrázok sa skopíruje, 
aby sa zachoval pôvodný obrázok. Spustí sa na pozadí vlákno, ktoré aplikuje úpravu jasu podľa vybraného efektu.
Upravený obrázok sa uloží do cache pre budúce použitie.

3. Správa cache
Vektor v_bitmap_effects uchováva všetky uložené efekty pre obrázok. Každý záznam reprezentuje jednu z 8 možných kombinácií jasu a smeru. 
Každý objekt BitmapEffect uchováva upravený obrázok (bitmap_effect), efekt jasu (brightness - Brighter/Darker), 
smer efektu (direction - Up/Down/Left/Right) a premmenú (isEffectApplied), ktorá sleduje, či bol efekt aplikovaný. 
Ak používateľ vyberie efekt, ktorý už bol aplikovaný (t.j. je uložený v cache), systém preskočí opakované spracovanie efektu 
a použije uložený výsledok.

Algoritmus použitý na úpravu jasu 

Funkcia AdjustBrightness slúži na úpravu jasnosti pixelov obrázka na základe zadaného faktora 
a smerov (hore, dole, vľavo, vpravo). Umožňuje len špecifických oblastí obrázka 
(napr. horná polovica, dolná polovica, ľavá alebo pravá strana).

Algoritmus:

 - Premenná factor je obmedzená iba na kladné hodnoty. 
 - Pixel je upravený len vtedy, ak spadá do oblasti určenej smerovými parametrami. 
 - Hodnoty RGB jednotlivých pixelov sú vynásobené faktorom a následne obmedzené na rozsah 0–255, 
   aby nedošlo k prekročeniu farebného rozsahu. 
 - Upravené hodnoty RGB sa zapíšu späť do dát obrázka.

Referencie:

https://aryamansharda.medium.com/understanding-image-contrast-algorithms-8636723a0f05
https://lodev.org/cgtutor/color.html#Change_the_Brightness_