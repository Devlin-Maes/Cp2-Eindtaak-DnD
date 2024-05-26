Schijf een programma dat de inventory (list of equipment cards) voor een D&D 5e simuleert a.d.h.v. D&D 5e API 

De applicatie kan opgeroepen worden via cli op volgende manier :

Usage:   Inventory.exe equipment-files [number-of-items] [-w max-weight] [-m money] [-c camp-file]
Options:
    number-of-items      Optional per file to define the number in the inventory﻿
    -w max-weight        Maximum weight before becoming encumbered
    -m money             List of coins and types (cp, sp, ep, gp, pp)
    -c camp-file         Optional camp file for all discovered items during play that stay in camp
 
Example: Inventory.exe -w 180.75 -m 4gp 42sp 69cp greatsword.json explorers-pack.json small-knife.json 2 waterskin.json leather-armor.json -c camp.log


Na het inladen van de equipment (cards) kan een speler circulair door de lijst met objecten stappen en bij elk object detail informatie opvragen indien gewenst en/of verplaatsen naar camp. Steeds wordt de totale waarde inclusief coins berekend en weergegeven en het totaal gewicht berekend en aangegeven of de speler emcumbered is.



JSON-files zijn te downloaden via https://www.dnd5eapi.co/api/equipment en afgeleiden.
