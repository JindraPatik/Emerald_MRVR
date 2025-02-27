# Emerald_MRVR

EKGame
Herní žánr:

Real time strategie, Tower Defense, 1vs.1 player - Worms, Micro souboje, Výběr jednotek do hry jako balíček u karetní hry

Téma:

Post-apocalyptic sci-fi
Zdecimovaná rozvinutá humanoidní civilizace
Rozvoj civilizace umožnil vysoce efektivní zdroj enrgie na kterém se stali postupně závislí
Zdroj začal ubývat. Jednotlivé vyspělé národy o něj začali válčit až se téměř zničily navzájem až najednou zdroj úplně zmizel. 
Zbytky národů se uchýlily k hibernaci pomocí starých nevýkonných zdrojů energie (vizuál různě improvizovaně pobastlený jako Fallout) a čekali až znovu objeví naleziště zdroje energie.
Hra začíná ve chvíli kdy se zdroj objeví => Národy o něj okamžitě začínají bojovat

MENU
Single player - Planetky z hexagonů
cíl dobýt všechny políčka na planetce a jít na další

Multi player - Zatím ne

Asi ve VR.
Hráč by si měl navolit do slotů jednotky se kterými chce vstoupit do souboje.

Gameplay:
AR scéna. 
Hrací plocha je levitující ostrov/ostrovy uvnitř místnosti
Na začátku hry hráč stojí vně ostrova, 
Na ostrově jsou proti sobě dvě naspawnované dvě vojenské základy. Hráčova a protihráčova.
Gameplay probíhá mezi základnami.
Mezi základnami rostou v náhodných intervalech krystaly - zdroje energie (které zároveň slouží jako platidlo ve hře)
Krystal zůstává ve hře dokud ho někdo nesebere.
Úkolem hráčů je krystaly sbírat a přivážet zpět do základny. Tímto získávají suroviny navíc.
Krystaly lze směnit za nákup vojenských jednotek které hráč posílá směrem k soupeřově základně. 
Cílem hry je soupeřovu základnu dobýt. 
Hráč si před každým herním soubojem navolí se kterými jednotkami vstupuje do hry.
Boj probíhá ve dvou úrovních. Pozemní a vzdušné.

Ovládání
Hráč stojí v místnosti. Hrací plochu (ostrov) má před sebou jako otočný stůl.
Ovládání pomocí Controllerů.
Z obou controllerů míří paprsky. 
Pravým controllerem hráč zajišťuje posílání jednotek, levým se stará o Power Upy (možnost prohodit?)
Výběr jednotky probíhá pomocí namíření na modul ve scéně. Po stisknutí triggeru se jednotka vybere a nad pravým ovladačem se vytvoří náhledový hologram.
Stisknutím tlačítka B se vybraná jednotka vypustí (pokud na ni hráč má zdroje) a dále se chová autonomně.
Ideálně nastavit selekci jednotek i na klobouček pravého controlleru a selekci power upů na klobouček levého controlleru. 
Otáčení scény ideálně nabindovat na boční tlačítko prostředníčku.
S ostrovem by mělo jít otáčet o 360. Hráč by měl mít možnost přibližovat a oddalovat ostrov a nastavovat si jeho výšku v prostoru.

Hráč
Obsahuje HealthComponent na sledování životů. Pokud se protihráčově jednotce podaří dostat se až na úroveň základny provede útok a životy ubere

Obsahuje ResourcesComponent - Ten spravuje hráčovy dostupné zdroje sesbíraných crystalů

Obsahuje MilitaryStationComponent- Spawnuje instanci hráčovy military base


Vojenská základna
Zároveň hodnota zdrojů nepatrně lineárně stále roste aby nedošlo k situaci že hráč nemá zdroje na nákup sběračů.

Souboj
Hráč vysílá jednotky které má k dispozici.
Jednotky po spawnu směřují konstantní rychlostí směrem k soupeřově základně.
Pokud se po cestě střetnou se soupeřovou jednotkou dochází k souboji. 

Jednotky mají útočná čísla (1, 2, 3…)
Souboj probíhá podle jednoduchých pravidel:
	Vyšší zabíjí nižší
	Jednotky se stejným útočným číslem se zabijí navzájem.

Jednotka se vždy na souboj na chvíli zastaví a pokud vyhraje rozjede se znovu směrem k soupeřově základně.

Jednotky se nijak nepoškozují. Například pokud jednotka s útočným číslem 3 potká na cestě za sebou několik jednotek s číslem 2 zneškodní je všechny.

Pokud se jednotce podaří projet až k soupeřově základně, zaútočí na ni a ubere ji životy.
Cílem je bránit základnu a nenechat soupeřovy jednotky přijet až k ní.

Cílem je aby hráč dokázal jednoduše a rychle vyhodnotit kterou jednotku musí poslat a nemusel odhadovat jak dlouho mu ještě která vydrží.

Jednotky:
Základní vlastnosti:
Jméno - Pojmenování jednotky

Druh jednotky: Pozemní/Létací

Cena - Kolik je potřeba krystalů k nákupu jednotky

Rychlost - Jakou rychlostí se pohybuje jednotka směrem k soupeřově základně

Doba trvání souboje - Doba na kterou se jednotka zastaví podobu souboje než se znovu 
rozjede

Síla - Útočné číslo pro boj s ostatními jednotkami

Damage - Množství HP které ubere soupeřově základně pokud se mu na ni podaří úspěšně zaútočit


Pozemní
Harvester
Modul: Mine
Nebojová jednotka.
Jednotka sloužící ke sběru krystalů.

Funkcionalita:
Harvester po vypuštění směřuje směrem od vlastní základny. Pokud po cestě narazí na Crystal, sebere jej a vrací se s ním nepatrně pomaleji než jel nenaložený zpět do své základny. Pokud se mu podaří krystal doručït, přičte si ResourcesComponent hodnotu doručeného crystalu a navýší tak zdroje které jsou hráči k dispozici.
Při doručení harvester zanikne.

Pokud se po cestě tam nebo zpět střetne s nepřátelskou jednotkou probíhá klasický souboj dle pravidel.

Pokud je zničen naložený harvester, jeho náklad je ztracen.


Thief
Modul: Thief
Nebojová jednotka.
Zloděj slouží k získávání zdrojů jinak než těžbou krystalů.
Po vypuštění směřuje směrem od vlastní základny. Pokud se po cestě střetne s jakoukoli jinou soupeřovou jednotkou než je naložený Harvester nebo Collaborant, je zničen.

Pokud potká soupeřův naložený Harvester vracející se zpět do své základny, přepadne, jej, sebere mu náklad, otočí se a zpomaleně se s lupem vrací zpět do své základny. Pokud se mu podaří lup doručit přičte si tyto zdroje ResourcesComponent a má je hned k dispozici jako při doručení krystalu harvesterem.

Pokud se zloději podaří dostat až k soupeřově základně, neútčí na ni ale ukradne ze soupeřovy ResourcesComponenty určený obnos a vrací se zpět stejně jako by přepadl Harvester. Pokud Soupeř neměl dostatek surovin kolik měl Thief ukrást, ukradne Thief jen co má. Nikdy nekrade do mínusu.

Při doručení thief zanikne.

PowerUpPicker
Modul: Bank
Nebojová jednotka.
Jednotka určená ke sběru power-upů.
Power-up nelze sebrat žádnou jinou jednotkou. Sběr probíhá stejným způsobem jako harvester sbírá Crystal.
Střet s jakoukoli nepřátelskou jednotku znamená pro Boost Picker zánik.

Při doručení power-upu Picker zanikne.

Collaborator
Modul: UVKSC
Nebojová jednotka.
Collaborant při kontaktu s protihráčouvou jednotkou tuto jednotku konvertuje na svoji. Ta se otočí opačným směrem a chová se jako by byla vlastní. Útočí proti původnímu majiteli.

Pokud se střetnou dva collaboranti přemění se navzájm a jedou zpět do svých základen.

Poté co Collaborant přemění jednotku odlétá random směrem a postupně se vrací na bod návratu.

Voyager
Modul: Microsoft
Bojová jednotka
Rychlá průzkumná jednotka s nízkým útočným číslem a nízkým damage.
Levná. Ideální na dohánění a likvidaci soupeřových Harvesterů

Brigade
Modul: Barracs
Bojová jednotka
Středně silná pozemní jednotka. Pomalejší ale silnější než Explorel.

Tank
Modul: Garage
Bojová jednotka
Nejsilnější pozemní jednotka. Pomalá ale težce zlikvidovatelná. Velký damage.

ATACSM
Modul: LPlatform
Bojová jednotka
Jednotka schopná zaútočit z dálky. Má soubojový trigger box před sebou. Při souboji zastaví a vystřelí rakety směrem k cíli z určité vzdálenosti.
Mortar
Modul: Minecraft
Bojová jednotka
Po vypuštění směřuje směrem od vlastní základny. V náhodném časovém intervalu se zastaví, rozloží se a přemění se na minu. Tato mina poté při kontaktu s cizí jednotkou exploduje. Do chvíle než se jednotka přemění na minu je velmi zranitelná. 



Létací
Létací jednotky se při souboji nezastavují jako ty pozemní
Helicopter
Modul: Heliport
Bojová jednotka
Rychlá průzkumná jednotka s nízkým útočným číslem a nízkým damage.
Létací obdoba Explorelu

Jet
Modul: Runway
Bojová jednotka
Stíhací letoun. 
Rychlý, silnější než helicopter, dražší.

Zeppelin
Modul: Aerial
Bojová jednotka
Pomalá těžká jednotka. Vysoké útočné číslo. Vysoký damage.
B52
Modul: Hangar
Bojová jednotka
Provádí line trace smerem před sebe/pod sebe. Pokud identifikuje soupeřovu jednotku shazuje postupně v náhodném intervalu tři bomby. Bomby při zásahu jakékoli jednotky (i vlastní) jednotku zničí. Po shození bomb odlétá a vrací se na svůj bod návratu.

Rocket
Modul: Rocket Launcher
Bojová jednotka
Raketa hledá soupeřovu leteckou jednotku. Pokud má k dispozici cíl zamíří k němu a zničí jej. pokud ne, vyletí kolmo vzhůru a po určitou dobu čeká na validní target. Pokud se neobjeví včas, provede autodestrukci.
Raketa je velmi drahá, ale obsahuje kompenzační bonus. Pokud protihráč hráči zničí po sobě čtyři jednotky za sebou aktivuje se hráči na omezenou dobu 100% sleva na raketu.
Power-Ups

PowerUpy shazuje v náhodných intervalech v bednách meteorologický balón (podobně jako ve Worms).
Padají do prostoru mezi základny. 
Obsah bedny je náhodný.
Bednu může sebrat a doručit pouze Boost Picker. 
Hráč má k dispozici tři sloty na uchování power upů. Pokud sebere čtvrtý nahradí tím ten který sebral jako první.
Po aktivaci power upu se slot uvolní.

Typy boosterů
Štít - hráč aktivuje na nějaký čas štít který při kontaktu s nepřátelskou jednotkou jednotku zničí
HP - lékárnička
Dočasný zrychlovač jednotek
Dočasný  zpomalovač soupeřových jednotek
Dočasný bonus na doručené suroviny
Následující jednotka zdarma
Konvertor jednotek
Dočasná mlha







