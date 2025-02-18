#pragma once
#include "defs.hpp"
#include "helpers.hpp"
#include "Card.hpp"
#include <string>
#include <iostream>

class CardCollection {
private:
    inline static std::vector<Card> cards;
    inline static std::vector<std::string> names;

    // Adds a manually defined card from the game into the global collection
    static void add(const std::string& name, int stars,
        int top, int right, int bottom, int left) {
        const ID id = cards.size();
        cards.emplace_back(id, stars, top, right, bottom, left, Player::PLAYER_NONE);
        names.push_back(name);
    }
public:
    static int cardCount() {
        return cards.size();
    }

    static Card card(const ID id) {
        return cards[id];
    }

    // Initalize all 435 triple triad cards currently in ffxiv
    static void init() {
        // Card Stat Ordering: Top, Right, Bottom, Left (Clockwise)
        // ______ EMPTY CARD _________
        add("EMPTY CARD", 0, 0, 0, 0, 0); // #0

        // ______ 1-STAR CARDS _______
        int stars = 1;
        add("Dodo", stars, 4, 2, 3, 4); // #1
        add("Tonberry", stars, 2, 2, 7, 2); // #2
        add("Sabotender", stars, 4, 3, 3, 3); // #3
        add("Spriggan", stars, 2, 3, 4, 4); // #4
        add("Pudding", stars, 2, 4, 3, 5); // #5
        add("Bomb", stars, 3, 4, 3, 3); // #6
        add("Mandragora", stars, 4, 2, 5, 3); // #7
        add("Coblyn", stars, 3, 3, 3, 4); // #8
        add("Morbol", stars, 5, 2, 5, 2); // #9
        add("Coeurl", stars, 2, 5, 2, 5); // #10
        add("Ahriman", stars, 5, 5, 2, 2); // #11
        add("Goobbue", stars, 2, 5, 5, 2); // #12
        add("Chocobo", stars, 3, 7, 2, 1); // #13
        add("Amalj'aa", stars, 1, 4, 7, 1); // #14
        add("Ixal", stars, 6, 1, 3, 4); // #15
        add("Sylph", stars, 2, 4, 5, 4); // #16
        add("Kobold", stars, 2, 2, 4, 6); // #17
        add("Sahagin", stars, 4, 5, 3, 3); // #18
        add("Tataru Taru", stars, 7, 2, 3, 2); // #19
        add("Moogle", stars, 2, 1, 3, 7); // #20
        add("Gaelicat", stars, 4, 1, 1, 7); // #81
        add("Deepeye", stars, 1, 3, 7, 2); // #101
        add("Apkallu", stars, 3, 4, 4, 1); // #154
        add("Colibri", stars, 6, 1, 4, 1); // #155
        add("Magitek Death Claw", stars, 4, 3, 2, 3); // #156
        add("Opo-opo", stars, 1, 4, 2, 6); // #169
        add("Namazu", stars, 1, 6, 1, 5); // #183
        add("Mossling", stars, 5, 2, 5, 1); // #203
        add("Koja", stars, 3, 2, 1, 6); // #211
        add("Wanyudo & Katasharin", stars, 6, 1, 1, 5); // #221
        add("Karakuri Hanya", stars, 7, 1, 1, 4); // #231
        add("Stormblood Tataru Taru", stars, 1, 4, 1, 7); // #241
        add("Amaro", stars, 1, 7, 2, 3); // #253
        add("Evil Weapon", stars, 7, 2, 2, 2); // #254     
        add("Lord and Lady Chai", stars, 5, 1, 6, 1); // #255
        add("Hobgoblin", stars, 2, 7, 2, 2); // #273   
        add("Porxie", stars, 1, 2, 6, 4); // #274
        add("Flower Basket", stars, 2, 2, 2, 7); // #283
        add("Qitari", stars, 4, 6, 2, 2); // #284 
        add("Dwarf", stars, 2, 4, 6, 2); // #294 
        add("Great Azuro", stars, 4, 7, 1, 1); // #305
		add("Troll", stars, 4, 4, 3, 4); // #327    
		add("Pisaca", stars, 3, 4, 4, 4); // #328   
		add("Ea", stars, 5, 4, 1, 4); // #329   
		add("Arkasodara", stars, 4, 4, 7, 4); // #330
		add("Rampart", stars, 1, 6, 5, 1); // #347
		add("Hippo Cart", stars, 1, 1, 7, 4); // #348 
		add("N-7000", stars, 1, 5, 1, 6); // #357
		add("Clockwork Onmyoji & Clockwork Yojimbo", stars, 6, 3, 1, 3); // #368
		add("Dreamingway", stars, 2, 5, 5, 2); // #369
		add("Okuri Chochin", stars, 4, 5, 1, 4); // #378
		add("Ketuduke", stars, 2, 3, 4, 3); // #389
		add("Pelupelu", stars, 5, 3, 4, 2); // #406  
		add("Alpaca", stars, 3, 5, 3, 3); // #407 

        // ______ 2-STAR CARDS _______
        stars = 2;
        add("Siren", stars, 3, 6, 7, 2); // #21
        add("Ultros & Typhon", stars, 2, 3, 6, 7); // #22
        add("Demon Wall", stars, 6, 7, 2, 3); // #23
        add("Succubus", stars, 7, 3, 2, 6); // #24
        add("Chimera", stars, 7, 7, 2, 2); // #25
        add("Blue Dragon", stars, 2, 2, 7, 7); // #26
        add("Scarface Bugaal Ja", stars, 6, 6, 2, 6); // #27
        add("Momodi Modi", stars, 7, 5, 5, 3); // #28
        add("Baderon Tenfingers", stars, 3, 7, 5, 5); // #29
        add("Mother Miounne", stars, 5, 5, 3, 7); // #30
        add("Livia sas Junius", stars, 3, 7, 7, 1); // #31
        add("Rhitahtyn sas Arvina", stars, 7, 1, 3, 7); // #32
        add("Biggs & Wedge", stars, 5, 3, 7, 5); // #33
        add("Gerolt", stars, 1, 7, 3, 7); // #34
        add("Frixio", stars, 6, 2, 6, 6); // #35
        add("Mutamix Bubblypots", stars, 2, 6, 6, 6); // #36
        add("Memeroon", stars, 6, 6, 6, 2); // #37
        add("Vanu Vanu", stars, 2, 6, 4, 7); // #82
        add("Gnath", stars, 6, 3, 7, 3); // #83
        add("Yugiri Mistwalker", stars, 7, 7, 1, 3); // #84
        add("Fat Chocobo", stars, 5, 5, 5, 5); // #85
        add("Archaeornis", stars, 5, 4, 6, 5); // #102
        add("Paissa", stars, 4, 6, 3, 6); // #103
        add("Dhalmel", stars, 6, 5, 3, 5); // #104
        add("Bandersnatch", stars, 6, 6, 3, 4); // #105
        add("Crawler", stars, 7, 7, 3, 2); // #106
        add("Poroggo", stars, 6, 3, 7, 3); // #107
        add("Honoroit", stars, 7, 2, 7, 2); // #121
        add("Hoary Boulder & Coultenet", stars, 2, 7, 2, 7); // #122
        add("Lolorito Nanarito", stars, 4, 3, 7, 3); // #141
        add("Gibrillont", stars, 2, 6, 3, 6); // #142
        add("Laniaitte de Haillenarte", stars, 7, 6, 3, 3); // #143
        add("Rhoswen", stars, 5, 6, 4, 6); // #144
        add("Carvallain de Gorgagne", stars, 2, 7, 7, 2); // #145
        add("Liquid Flame", stars, 4, 3, 6, 6); // #157
        add("Lost Lamb", stars, 7, 3, 4, 5); // #158
        add("Delivery Moogle", stars, 5, 5, 6, 3); // #159
        add("Magitek Colossus", stars, 6, 3, 6, 3); // #160
        add("Adamantoise", stars, 5, 7, 4, 4); // #170
        add("Magitek Vanguard", stars, 3, 5, 4, 7); // #171
        add("Magitek Gunship", stars, 3, 5, 5, 5); // #172
        add("Gold Saucer Attendant", stars, 4, 7, 1, 6); // #173
        add("Kojin", stars, 4, 5, 5, 4); // #184    
        add("Ananta", stars, 3, 7, 5, 2); // #185   
        add("Mnaago", stars, 4, 1, 4, 7); // #186   
        add("Kotokaze", stars, 5, 6, 2, 6); // #187
        add("Chapuli", stars, 7, 2, 2, 6); // #204   
        add("Qiqirn Meateater", stars, 1, 5, 7, 5); // #205
        add("Ango", stars, 4, 2, 7, 4); // #212
        add("Guidance Node", stars, 6, 4, 4, 4); // #213
        add("Tansui", stars, 6, 2, 3, 7); // #214
        add("Hatamoto", stars, 2, 7, 5, 6); // #222
        add("Yukinko", stars, 3, 7, 3, 5); // #223
        add("Muud Suud", stars, 3, 7, 1, 7); // #232
        add("Dvergr", stars, 4, 5, 3, 7); // #242 
        add("Ejika Tsunjika", stars, 7, 1, 1, 7); // #243
        add("Gigantender", stars, 7, 4, 4, 4); // #256     
        add("Feo UI", stars, 7, 5, 5, 1); // #257   
        add("Runar", stars, 6, 4, 7, 1); // #258  
        add("Grenoldt", stars, 1, 3, 7, 7); // #259
        add("Iguana", stars, 3, 5, 6, 5); // #275   
        add("Nu Mou", stars, 7, 6, 2, 4); // #276    
        add("Fuath", stars, 7, 4, 6, 2); // #277 
        add("Gnoll", stars, 2, 4, 7, 6); // #285
        add("Lizbeth", stars, 5, 5, 6, 4); // #286
        add("Rolling Tankard", stars, 6, 4, 2, 6); // #296
        add("Ehll Tou", stars, 1, 7, 1, 7); // #295
        add("Mother Porxie", stars, 3, 3, 7, 6); // #306
        add("Moblin", stars, 5, 5, 4, 5); // #393
        add("Branchbearer", stars, 6, 5, 3, 4); // #394
        add("Rroneek", stars, 3, 5, 4, 6); // #395
        add("Sentry R8", stars, 4, 6, 4, 5); // #396
        add("Outrunner", stars, 5, 3, 6, 5); // #397
        add("Overseer Kanilokka", stars, 6, 4, 5, 4); // #411
		add("Keeper of the Keys", stars, 5, 4, 4, 7); // #317
		add("Loporrit", stars, 4, 6, 6, 4); // #331 
		add("Argos", stars, 7, 3, 7, 1); // #332 
		add("Gajasura", stars, 4, 7, 4, 4); // #349
		add("Geryon the Steer", stars, 2, 4, 6, 6); // #358   
		add("Gladiator of Sildih", stars, 7, 2, 5, 5); // #359  
		add("Thorne Knight", stars, 4, 7, 2, 6); // #360
		add("Felicitous Furball", stars, 5, 5, 7, 1); // #370  
		add("Anden", stars, 4, 1, 6, 7); // #371 
		add("Gorai the Uncaged", stars, 7, 2, 6, 4); // #378 
		add("Moko the Restless", stars, 7, 3, 3, 8); // #380
		add("Lala", stars, 5, 6, 2, 3); // #390      
		add("Statice", stars, 4, 4, 5, 5); // #391
		add("PuPu", stars, 5, 7, 4, 3); // #392
		add("Moblin", stars, 5, 5, 4, 5); // #408     
		add("Branchbearer", stars, 6, 5, 3, 4); // #409  
		add("Rroneek", stars, 3, 5, 4, 6); // #410  
		add("Sentry R8", stars, 4, 6, 4, 5); // #411 
		add("Outrunner", stars, 7, 3, 3, 8); // #412 
		add("Overseer Kanilokka", stars, 6, 4, 5, 4); // #426

        // ______ 3-STAR CARDS _______
        stars = 3;
        add("Behemoth", stars, 7, 8, 4, 2); // #38
        add("Gilgamesh & Enkidu", stars, 8, 3, 7, 3); // #39
        add("Ifrit", stars, 7, 1, 6, 7); // #40
        add("Titan", stars, 1, 7, 7, 6); // #41
        add("Garuda", stars, 7, 6, 1, 7); // #42
        add("Good King Moggle Mog XII", stars, 7, 6, 7, 1); // #43
        add("Raya-O-Senna & A-Ruhn-Senna", stars, 5, 6, 6, 6); // #44
        add("Godbert Manderville", stars, 6, 6, 5, 6); // #45
        add("Thancred", stars, 2, 3, 8, 7); // #46
        add("Nero tol Scaeva", stars, 4, 1, 8, 7); // #47
        add("Papalymo & Yda", stars, 3, 7, 8, 2); // #48
        add("Yshtola", stars, 7, 8, 1, 4); // #49
        add("Urianger", stars, 8, 1, 4, 7); // #50
        add("Griffin", stars, 5, 1, 7, 8); // #86
        add("Tioman", stars, 1, 5, 8, 7); // #87
        add("Estinien", stars, 8, 8, 2, 3); // #88
        add("Lucia goe Junius", stars, 8, 2, 3, 8); // #89
        add("Ysayle", stars, 4, 8, 8, 1); // #90
        add("Hilda", stars, 1, 4, 8, 8); // #91
        add("Matoya", stars, 8, 3, 8, 1); // #92
        add("Count Edmont de Fortemps", stars, 1, 8, 3, 8); // #93
        add("Byblos", stars, 8, 4, 1, 7); // #94
        add("Haurchefant", stars, 7, 1, 5, 8); // #95
        add("Vedrfolnir", stars, 8, 2, 7, 4); // #108
        add("Coeurlregina", stars, 5, 5, 5, 8); // #109
        add("Progenitrix", stars, 6, 6, 3, 6); // #110
        add("Belladonna", stars, 3, 7, 3, 7); // #111
        add("Echidna", stars, 6, 4, 7, 4); // #112
        add("Pipin Tarupin", stars, 6, 5, 6, 6); // #113
        add("Julyan Manderville", stars, 6, 6, 5, 7); // #114
        add("Moglin", stars, 8, 5, 4, 5); // #115
        add("Charibert", stars, 7, 8, 4, 3); // #116
        add("Roundrox", stars, 2, 2, 8, 8); // #117
        add("Brachiosaur", stars, 8, 4, 5, 4); // #123
        add("Darkscale", stars, 2, 8, 4, 7); // #124
        add("Fenrir", stars, 7, 3, 6, 4); // #125
        add("Kraken", stars, 3, 6, 6, 6); // #126
        add("Vicegerent to the Warden", stars, 7, 1, 7, 5); // #127
        add("Manxome Molaa Ja Ja", stars, 3, 7, 2, 8); // #128
        add("Ferdiad", stars, 4, 6, 8, 4); // #129
        add("Calcabrina", stars, 6, 6, 6, 3); // #130
        add("Kuribu", stars, 8, 3, 2, 7); // #131
        add("Phlegethon", stars, 1, 7, 6, 7); // #132
        add("Artoirel de Fortemps", stars, 4, 7, 3, 7); // #133
        add("Emmanellain de Fortemps", stars, 7, 4, 7, 3); // #134
        add("Kal Myhk", stars, 1, 8, 4, 8); // #146
        add("Waukkeon", stars, 1, 6, 7, 7); // #147
        add("Curator", stars, 4, 6, 4, 7); // #148
        add("Mistbeard", stars, 5, 6, 7, 6); // #149
        add("Strix", stars, 5, 7, 1, 7); // #161
        add("Tozol Huatotl", stars, 7, 6, 6, 2); // #162
        add("Alexander Prime", stars, 7, 3, 2, 8); // #163
        add("Brendt, Brennan, & Bremondt", stars, 4, 7, 6, 5); // #164
        add("Lava Scorpion", stars, 3, 8, 5, 5); // #174
        add("Magitek Predator", stars, 5, 7, 4, 5); // #175
        add("Magitek Sky Armor", stars, 6, 2, 6, 7); // #176
        add("The Griffin", stars, 8, 8, 4, 1); // #177
        add("Roland", stars, 2, 7, 8, 3); // #178
        add("Mammoth", stars, 6, 3, 8, 3); // #188 
        add("Phoebad", stars, 8, 8, 3, 1); // #189  
        add("Susano", stars, 2, 8, 3, 8); // #190   
        add("Lakshmi", stars, 3, 7, 7, 5); // #191  
        add("Grynewaht", stars, 7, 4, 7, 4); // #192 
        add("Rasho", stars, 4, 7, 8, 2); // #193    
        add("Cirina", stars, 3, 5, 6, 8); // #194   
        add("Magnai", stars, 6, 7, 4, 4); // #195   
        add("Sadu", stars, 6, 6, 7, 4); // #196
        add("Hrodric Poisontongue", stars, 2, 7, 6, 7); // #206
        add("Fordola rem Lupis", stars, 5, 8, 6, 3); // #207
        add("Rofocale", stars, 4, 7, 1, 8); // #208
        add("Genbu", stars, 7, 7, 1, 7); // #215     
        add("Byakko", stars, 7, 1, 7, 7); // #216     
        add("Arenvald Lentinus", stars, 8, 4, 8, 2); // #217   
        add("Lupin", stars, 3, 3, 8, 7); // #218
        add("Qitian Dasheng", stars, 3, 8, 4, 8); // #224
        add("Hiruko", stars, 4, 7, 5, 7); // #225
        add("Happy Bunny", stars, 3, 8, 7, 3); // #226
        add("Louhi", stars, 5, 8, 2, 7); // #227
        add("Tokkapchi", stars, 2, 6, 6, 7); // #233    
        add("Mist Dragon", stars, 5, 8, 5, 5); // #234    
        add("Suzaku", stars, 1, 7, 7, 7); // #235        
        add("Asahi sas Brutus", stars, 8, 4, 1, 8); // #238
        add("Pazuzu", stars, 4, 8, 4, 7); // #236     
        add("Penthesilea", stars, 7, 2, 8, 5); // #237 
        add("Prometheus", stars, 6, 5, 8, 3); // #244      
        add("Seiryu", stars, 7, 7, 7, 1); // #246   
        add("Alpha", stars, 6, 6, 6, 6); // #247
        add("Provenance Watcher", stars, 7, 4, 4, 8); // #245
        add("Philia", stars, 2, 6, 8, 6); // #260   
        add("Titania", stars, 8, 6, 6, 2); // #261     
        add("Eros", stars, 6, 2, 6, 8); // #262   
        add("Storge", stars, 6, 8, 2, 6); // #263   
        add("Formidable", stars, 8, 5, 5, 5); // #264    
        add("Lyna", stars, 1, 8, 8, 3); // #265    
        add("Jongleurs of Eulmore", stars, 4, 4, 7, 6); // #266
        add("Leannan Sith", stars, 8, 7, 2, 4); // #278 
        add("Seeker of Solitude", stars, 4, 6, 6, 8); // #279
        add("Batsquatch", stars, 6, 2, 8, 6); // #287   
        add("Forgiven Obscenity", stars, 8, 7, 1, 6); // #288 
        add("Huaca", stars, 7, 5, 8, 2); // #289     
        add("Unknown", stars, 5, 6, 3, 8); // #290
        add("Lugus", stars, 5, 4, 7, 7); // #297
        add("Dawon", stars, 8, 4, 6, 6); // #298  
        add("Adrammelech", stars, 6, 1, 7, 8); // #299 
        add("Black Cat", stars, 7, 3, 3, 8); // #421
        add("Ark Angel MR", stars, 7, 8, 4, 4); // #412
        add("Ark Angel TT", stars, 8, 4, 4, 7); // #413
        add("Ark Angel GK", stars, 4, 4, 7, 8); // #414
        add("Ark Angel HM", stars, 4, 7, 4, 8); // #415
        add("Ark Angel EV", stars, 4, 7, 8, 4); // #416
		add("Trinity Seeker", stars, 6, 1, 8, 7); // #307    
		add("Trinity Avowed", stars, 8, 7, 6, 1); // #308    
		add("Azulmagia", stars, 4, 8, 6, 6); // #309    
		add("Siegfried", stars, 6, 6, 4, 8); // #310    
		add("Gogo, Master of Mimicry", stars, 8, 6, 4, 6); // #311
		add("Lunar Bahamut", stars, 8, 2, 8, 4); // #318      
		add("Valens van Varro", stars, 7, 5, 8, 3); // #319
		add("Lunar Ifrit", stars, 8, 3, 4, 7); // #320      
		add("4th-make Shemhazai", stars, 1, 6, 7, 8); // #321   
		add("4th-make Cuchulainn", stars, 8, 1, 7, 6); // #322
		add("Hermes", stars, 6, 8, 7, 1); // #333      
		add("Amon the Undying", stars, 8, 7, 3, 3); // #334     
		add("Magus Sisters", stars, 4, 1, 8, 8); // #336      
		add("Meteion", stars, 7, 1, 8, 6); // #337      
		add("Fourchenault Leveilleur", stars, 7, 7, 3, 7); // #338
		add("Erichthonios", stars, 5, 8, 5, 6); // #335 
		add("Rhalgr", stars, 6, 8, 1, 7); // #350       
		add("Azeyma", stars, 7, 6, 1, 8); // #351        
		add("Nald", stars, 8, 1, 6, 7); // #352          
		add("Thal", stars, 6, 7, 8, 1); // #353
		add("Beatrice", stars, 6, 7, 7, 4); // #361      
		add("Byregot", stars, 1, 6, 8, 7); // #362  
		add("Shadowcaster Zeless Gah", stars, 2, 8, 8, 2); // #363
		add("Menphina", stars, 1, 7, 6, 8); // #374 
		add("Suprae-Lugae", stars, 5, 7, 5, 6); // #372  
		add("Gancanagh", stars, 8, 3, 2, 8); // #373 
		add("Octomammoth", stars, 6, 8, 4, 5); // #381     
		add("Proto-Carbuncle", stars, 3, 7, 5, 7); // #382
		add("Goldor", stars, 4, 2, 8, 8); // #383     
		add("Shishio", stars, 7, 5, 2, 8); // #384 
		add("Nophica", stars, 8, 6, 7, 1); // #394     
		add("Althyk", stars, 1, 7, 8, 6); // #395      
		add("Nymeia", stars, 1, 8, 6, 7); // #396     
		add("Halone", stars, 1, 8, 7, 6); // #397      
		add("Thaliak", stars, 7, 8, 6, 1); // #398     
		add("Llymlaen", stars, 7, 8, 1, 6); // #399   
		add("Oschon", stars, 7, 6, 8, 1); // #400      
		add("Durante", stars, 7, 6, 6, 6); // #401 
		add("Loquloqui", stars, 5, 5, 7, 5); // #393 
		add("Gulool Ja Ja", stars, 7, 3, 3, 8); // #413  
		add("Drowsie", stars, 3, 7, 5, 6); // #414     
		add("Ryoqor Terteh", stars, 6, 6, 4, 6); // #415   
		add("Maulskull", stars, 7, 7, 4, 4); // #416      
		add("Ambrose", stars, 5, 3, 7, 6); // #417

        // ______ 4-STAR CARDS _______
        stars = 4;
        add("Ultima Weapon", stars, 7, 8, 9, 1); // #51
        add("Odin", stars, 8, 8, 1, 8); // #52
        add("Ramuh", stars, 8, 1, 8, 8); // #53
        add("Leviathan", stars, 8, 8, 8, 1); // #54
        add("Shiva", stars, 1, 8, 8, 8); // #55
        add("Minfilia", stars, 9, 8, 3, 5); // #56
        add("Lahabrea", stars, 4, 9, 4, 8); // #57
        add("Cid Garlond", stars, 5, 9, 9, 2); // #58
        add("Alphinaud & Alisaie", stars, 9, 3, 3, 9); // #59
        add("Louisoix Leveilleur", stars, 9, 4, 9, 3); // #60
        add("Aymeric", stars, 1, 5, 9, 9); // #96
        add("Ravana", stars, 9, 7, 8, 1); // #97
        add("Bismarck", stars, 1, 9, 7, 8); // #98
        add("Senor Sabotender", stars, 9, 5, 7, 6); // #118
        add("Xande", stars, 9, 4, 6, 7); // #135
        add("Brute Justice", stars, 7, 7, 7, 7); // #136
        add("Sephirot", stars, 6, 8, 8, 6); // #137
        add("Flhaminn", stars, 9, 7, 3, 7); // #138
        add("Vidofnir", stars, 8, 6, 8, 6); // #139
        add("Unei & Doga", stars, 6, 8, 6, 8); // #150
        add("Tiamat", stars, 9, 6, 5, 6); // #151
        add("Calofisteri", stars, 5, 8, 7, 8); // #152
        add("Heavensward Thancred", stars, 8, 1, 7, 9); // #165
        add("Heavensward Yshtola", stars, 9, 9, 2, 5); // #166
        add("Nael van Darnus", stars, 3, 9, 3, 9); // #167
        add("Diabolos Hollow", stars, 4, 4, 8, 9); // #179
        add("Armored Weapon", stars, 3, 5, 9, 7); // #180
        add("Gigi", stars, 6, 8, 4, 7); // #181
        add("Shinryu", stars, 7, 8, 8, 2); // #197 
        add("Yotsuyu", stars, 9, 2, 8, 6); // #198  
        add("Krile", stars, 2, 8, 8, 7); // #199    
        add("Lyse", stars, 6, 9, 1, 8); // #200   
        add("Argath Thadalfus", stars, 9, 2, 5, 8); // #209 
        add("Hancock", stars, 8, 9, 1, 7); // #219
        add("Tsukuyomi", stars, 2, 7, 7, 8); // #228
        add("Great Gold Whisker", stars, 6, 9, 6, 7); // #248
        add("Stormblood Gilgamesh", stars, 9, 4, 8, 4); // #249
        add("Innocence", stars, 8, 3, 8, 8); // #267     
        add("Shadowbringers Yshtola", stars, 2, 9, 5, 9); // #268  
        add("Shadowbringers Urianger", stars, 9, 2, 5, 9); // #269   
        add("Ranjit", stars, 1, 9, 9, 5); // #270
        add("Oracle of Light", stars, 7, 7, 9, 4); // #280
        add("Ruby Weapon", stars, 8, 7, 1, 9); // #291
        add("Elidibus", stars, 8, 4, 9, 4); // #300   
        add("Shadowbringers Thancred", stars, 7, 9, 8, 1); // #301  
        add("Sapphire Weapon", stars, 1, 9, 8, 7); // #302
        add("Emerald Weapon", stars, 7, 1, 9, 8); // #312   
        add("Ryne", stars, 7, 9, 7, 1); // #313        
        add("Gaia", stars, 5, 9, 1, 9); // #314 
        add("Cahciua", stars, 8, 8, 5, 5); // #416
        add("Ttokrrone", stars, 9, 7, 7, 2); // #417
        add("Mica the Magical Mu", stars, 8, 2, 9, 6); // #418
        add("Prishe of the Distant Chains", stars, 6, 8, 8, 5); // #419
		add("G-Warrior", stars, 9, 1, 7, 8); // #323        
		add("Diamond Weapon", stars, 9, 8, 7, 1); // #324   
		add("Diablo Armament", stars, 4, 4, 9, 9); // #325     
		add("Anima", stars, 5, 1, 9, 9); // #339         
		add("Quintus van Cinna", stars, 9, 3, 6, 8); // #340   
		add("Endwalker Alphinaud & Alisaie", stars, 4, 9, 9, 3); // #341    
		add("Hythlodaeus", stars, 7, 9, 6, 6); // #342     
		add("Vrtra", stars, 9, 3, 7, 7); // #343     
		add("Chi", stars, 8, 1, 9, 7); // #354           
		add("Daivadipa", stars, 7, 8, 1, 9); // #355     
		add("Scarmiglione", stars, 7, 6, 8, 7); // #364  
		add("Barbariccia", stars, 6, 7, 7, 8); // #365   
		add("Chief Keyward Lahabrea", stars, 5, 9, 5, 8); // #366   
		add("Cagnazzo", stars, 8, 6, 7, 7); // #375      
		add("Rubicante", stars, 7, 8, 6, 7); // #376    
		add("Themis", stars, 5, 5, 9, 8); // #385       
		add("Enenra", stars, 8, 5, 5, 9); // #386      
		add("Eulogia", stars, 6, 9, 6, 7); // #402     
		add("Asura", stars, 9, 5, 7, 6); // #403       
		add("Valigarmanda", stars, 7, 5, 6, 8); // #418    
		add("Otis", stars, 5, 7, 8, 6); // #422    
		add("Cahciua", stars, 8, 8, 5, 5); // #423  
		add("Prishe of the Distant Chains", stars, 6, 8, 8, 5); // #434        
		add("Mica the Magical Mu", stars, 8, 2, 9, 6); // #433 

        // ______ 5-STAR CARDS _______
        stars = 5;
        add("Bahamut", stars, 9, 5, 9, 6); // #61
        add("Hildibrand & Nashu Mhakaracca", stars, 1, 8, STRENGTH_MAX, 8); // #62
        add("Nanamo UI Namo", stars, STRENGTH_MAX, 6, 4, 8); // #63
        add("Gaius van Baelsar", stars, 4, STRENGTH_MAX, 5, 9); // #64
        add("Merlwyb Bloefhiswyn", stars, 5, 9, STRENGTH_MAX, 3); // #65
        add("Kan-E-Senna", stars, 9, STRENGTH_MAX, 1, 7); // #66
        add("Raubahn Aldynn", stars, 6, 2, 9, STRENGTH_MAX); // #67
        add("Nidhogg", stars, STRENGTH_MAX, 7, 3, 8); // #99
        add("Midgardsormr", stars, 3, 8, STRENGTH_MAX, 7); // #100
        add("Regula van Hydrus", stars, 8, 8, 3, 8); // #119
        add("Archbishop Thordan VII", stars, 7, 1, STRENGTH_MAX, 9); // #120
        add("Cloud of Darkness", stars, 7, STRENGTH_MAX, 3, 8); // #140
        add("Hraesvelgr", stars, 7, 7, STRENGTH_MAX, 6); // #153
        add("Sophia", stars, STRENGTH_MAX, 8, 5, 6); // #168
        add("Zurvan", stars, 3, 7, 8, STRENGTH_MAX); // #182
        add("Zenos yae Galvus", stars, 6, 6, 7, STRENGTH_MAX); // #201 
        add("Hien", stars, 2, STRENGTH_MAX, 5, STRENGTH_MAX); // #202   
        add("Raubahn & Pipin", stars, 1, STRENGTH_MAX, STRENGTH_MAX, 6); // #210
        add("Hisui & Kurenai", stars, STRENGTH_MAX, 2, 7, 9); // #220
        add("Yiazmat", stars, 7, 6, STRENGTH_MAX, 7); // #229
        add("Gosetsu", stars, STRENGTH_MAX, 5, STRENGTH_MAX, 2); // #230
        add("Omega", stars, 6, 9, 3, STRENGTH_MAX); // #240
        add("Yojimbo & Daigoro", stars, STRENGTH_MAX, 8, 1, 8); // #239
        add("Ultima, the High Seraph", stars, 6, STRENGTH_MAX, 7, 7); // #250
        add("Stormblood Alphinaud & Alisaie", stars, 6, 8, 8, 8); // #251 
        add("Hades", stars, 8, 6, 6, STRENGTH_MAX); // #271   
        add("Ardbert", stars, 1, 9, 9, 9); // #272
        add("Archaeotania", stars, STRENGTH_MAX, 9, 7, 1); // #281 
        add("9S", stars, 4, 8, 6, STRENGTH_MAX); // #282 
        add("Therion", stars, 9, 9, 2, 9); // #292
        add("Varis yae Galvus", stars, STRENGTH_MAX, STRENGTH_MAX, 4, 1); // #293 
        add("2P", stars, 4, 8, STRENGTH_MAX, 6); // #303    5 Star
        add("Shadowbringers Warrior of Light", stars, 2, STRENGTH_MAX, STRENGTH_MAX, 5); // #304
        add("Edens Promise", stars, STRENGTH_MAX, 5, 8, 6); // #315   
        add("Phoenix", stars, 5, 9, 6, 9); // #316 
        add("Squall Leonhart", stars, 6, STRENGTH_MAX, STRENGTH_MAX, 1); // #428
        add("Zidane Tribal", stars, 5, STRENGTH_MAX, 6, 8); // #429
        add("Tidus", stars, STRENGTH_MAX, 7, 1, 9); // #430
        add("Shantotto", stars, 4, 9, 7, 9); // #431
        add("Vaan", stars, 1, 7, STRENGTH_MAX, 9); // #432
        add("Lightning", stars, 9, 1, 7, STRENGTH_MAX); // #433
        add("Noctis Lucis Caelum", stars, 7, STRENGTH_MAX, 9, 1); // #434
        add("Clive Rosfield", stars, STRENGTH_MAX, 7, 8, 5); // #435
		add("2B", stars, 6, STRENGTH_MAX, 4, 8); // #326     
		add("Zodiark", stars, 5, STRENGTH_MAX, 3, STRENGTH_MAX); // #344     
		add("Hydaelyn", stars, STRENGTH_MAX, 3, STRENGTH_MAX, 5); // #345   
		add("Zenos Galvus", stars, 6, 6, 9, 9); // #346     
		add("Endsinger", stars, 7, STRENGTH_MAX, 2, 9); // #356    
		add("Hephaistos", stars, 9, 4, STRENGTH_MAX, 5); // #367     
		add("Venat", stars, 4, 8, 7, STRENGTH_MAX); // #377   
		add("Golbez", stars, STRENGTH_MAX, 5, 2, STRENGTH_MAX); // #387    
		add("Athena", stars, 9, 5, STRENGTH_MAX, 4); // #388  
		add("Zeromus", stars, 9, 9, 9, 1); // #404    
		add("Zoraal Ja", stars, 9, 8, 5, 7); // #419      
		add("Queen Eternal", stars, 3, STRENGTH_MAX, 9, 5); // #420   
		add("Wuk Lamat", stars, 5, 5, STRENGTH_MAX, 9); // #424      
		add("Sphene", stars, 9, STRENGTH_MAX, 6, 3); // #425         
		add("Cloud Strife", stars, 9, 3, 9, 8); // #74         
		add("Terra Branford", stars, STRENGTH_MAX, STRENGTH_MAX, 2, 5); // #73   
		add("Bartz Klauser", stars, 4, 4, STRENGTH_MAX, STRENGTH_MAX); // #72    
		add("Cecil Harvey", stars, 4, STRENGTH_MAX, 4, STRENGTH_MAX); // #71     
		add("Onion Knight", stars, 8, 2, 8, STRENGTH_MAX); // #70    
		add("Firion", stars, STRENGTH_MAX, 5, STRENGTH_MAX, 1); // #69     
		add("Warrior of Light", stars, STRENGTH_MAX, 2, 5, STRENGTH_MAX); // #68   
		add("Shadow Lord", stars, STRENGTH_MAX, STRENGTH_MAX, 4, 4); // #435 
    }

    static Card getEmpty() {
        return cards.front(); // The first element of cards is an empty card
    }

    static ID randomID() {
        return 1 + std::rand() % (cards.size() - 1);
    }

    static Card random() {
        return cards[randomID()];
    }

    static ID randomWithStarsID(const int numStars) {
        Card starCard = random();
        while (starCard.stars() != numStars)
            starCard = random();
        return starCard.id();
    }

    static std::string name(const ID id) {
        return names[id];
    }

    static ID idFromName(const std::string& name) {
        auto it = std::find(names.begin(), names.end(), name);
        if (it != names.end())
            return std::distance(names.begin(), it);

        std::cout << "Error: A name wasn't found in CardCollection::idFromName()" << std::endl;
    }
};