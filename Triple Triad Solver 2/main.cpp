#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <vector>
#include <iostream>
#include <cstdlib>  // For rand()
#include <ctime>    // For time()
#include <numeric>
#include <tuple>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <unordered_set>
#include <chrono>
#include <iomanip>

enum Edge {
    TOP = 0, RIGHT = 1, BOTTOM = 2, LEFT = 3
};

// Player Enum
enum Player {
    PLAYER_RED,
    PLAYER_BLUE,
    PLAYER_NONE
};

using ID = uint16_t;
using CardContainer = std::vector<ID>;

struct Card {
    uint8_t attribute[4] = { 0, 0, 0, 0 };
    ID id = 0;
    uint8_t stars = 0;
    Player controllingColor = PLAYER_NONE;

    Card() = default;

    Card(ID id, uint8_t stars, uint8_t top, uint8_t right, uint8_t bottom, uint8_t left, Player controllingColor)
        : id(id), stars(stars), controllingColor(controllingColor), attribute{ top, right, bottom, left } {
    }

    uint8_t operator[](Edge dir) const {
        return attribute[static_cast<size_t>(dir)];
    }

    bool isEnemy(const Card& other) const {
        return other.controllingColor != PLAYER_NONE && controllingColor != PLAYER_NONE && other.controllingColor != controllingColor;
    }
};

static constexpr uint8_t STRENGTH_MAX = 255;

static char attributeToChar(uint8_t value) {
    switch (value) {
    case STRENGTH_MAX:
        return 'A';
    case 0:
        return '0';
    case 1:
        return '1';
    case 2:
        return '2';
    case 3:
        return '3';
    case 4:
        return '4';
    case 5:
        return '5';
    case 6:
        return '6';
    case 7:
        return '7';
    case 8:
        return '8';
    case 9:
        return '9';
    default:
        std::cout << "attributeToChar() received an input value that isn't a valid attribute number: " << std::endl;
        return '%';
    }
}

static char colorToChar(Player color) {
    return color == PLAYER_RED ? 'R' :
        color == PLAYER_BLUE ? 'B' : 'N';
}

namespace CardCollection {
    static std::vector<Card> cards;
    static std::vector<std::string> names;

    // Adds a manually defined card from the game into the global collection
    static void add(const std::string& name, uint8_t stars,
        uint8_t top, uint8_t right, uint8_t bottom, uint8_t left) {
        const ID id = cards.size();
        cards.emplace_back(id, stars, top, right, bottom, left, Player::PLAYER_NONE);
        names.push_back(name);
    }

    // Initalize all 435 triple triad cards currently in ffxiv
    static void init() {
        // Card Stat Ordering: Top, Right, Bottom, Left (Clockwise)
        // ______ EMPTY CARD _________
        add("EMPTY CARD", 0, 0, 0, 0, 0); // #0

        // ______ 1-STAR CARDS _______
        uint8_t stars = 1;
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
        add("PuPu", stars, 5, 7, 4, 3); // #389
        add("Moblin", stars, 5, 5, 4, 5); // #393
        add("Branchbearer", stars, 6, 5, 3, 4); // #394
        add("Rroneek", stars, 3, 5, 4, 6); // #395
        add("Sentry R8", stars, 4, 6, 4, 5); // #396
        add("Outrunner", stars, 5, 3, 6, 5); // #397
        add("Overseer Kanilokka", stars, 6, 4, 5, 4); // #411

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
		add("Arenvald Lentinus ", stars, 8, 4, 8, 2); // #217   
		add("Lupin", stars, 3, 3, 8, 7); // #218
		add("Qitana Dasheng", stars, 3, 8, 4, 8); // #224
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
        add("Arc Angel MR", stars, 7, 8, 4, 4); // #412
        add("Arc Angel TT", stars, 8, 4, 4, 7); // #413
        add("Arc Angel GK", stars, 4, 4, 7, 8); // #414
        add("Arc Angel HM", stars, 4, 7, 4, 8); // #415
        add("Arc Angel EV", stars, 4, 7, 8, 4); // #416

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
        add("Squall Leonhard", stars, 6, STRENGTH_MAX, STRENGTH_MAX, 1); // #428
        add("Zidane Tribal", stars, 5, STRENGTH_MAX, 6, 8); // #429
        add("Tidus", stars, STRENGTH_MAX, 7, 1, 9); // #430
        add("Shantotto", stars, 4, 9, 7, 9); // #431
        add("Vaan", stars, 1, 7, STRENGTH_MAX, 9); // #432
        add("Lightning", stars, 9, 1, 7, STRENGTH_MAX); // #433
        add("Noctis Lucis Caelum", stars, 7, STRENGTH_MAX, 9, 1); // #434
        add("Clive Rosfield", stars, STRENGTH_MAX, 7, 8, 5); // #435
    }

    static Card getEmpty() {
        return cards.front(); // The first element of cards is an empty card
    }

    static ID getRandomID() {
        return 1 + std::rand() % (cards.size() - 1);
    }

    static Card getRandomCard() {
        return cards[getRandomID()];
    }

    static std::string name(ID nameID) {
        return CardCollection::names[nameID];
    }

    static ID getIDfromName(const std::string& name) {
        auto it = std::find(names.begin(), names.end(), name);
        if (it != names.end())
            return std::distance(names.begin(), it);

        std::cout << "Error: A name wasn't found in CardCollection::getIDfromName()" << std::endl;
    }

    static void print(ID id) {
        const auto& card = cards[id];
        for (size_t edge = 0; edge < 4; ++edge)
            std::cout << attributeToChar(card.attribute[edge]);
        std::cout << std::endl;
    }
}

static constexpr uint8_t HAND_SIZE = 5;
static constexpr uint8_t DECK_SIZE = 5;

namespace DeckStats {
    static constexpr size_t MATCHES_TO_PLAY = 420;
    static constexpr size_t MATCHES_THRESHOLD = 100;

    enum Result {
        WIN, DRAW, LOSS, NONE
    };

    struct Stats {

        ID deckId;
        size_t wins, draws, losses;
        std::unordered_map<ID, Result> playedAgainst;

        Stats() = default;

        bool hasPlayedAgainst(ID targetDeck) const {
            return playedAgainst.find(targetDeck) != playedAgainst.end();
        }

        Result resultAgainst(ID targetDeck) const {
            auto it = playedAgainst.find(targetDeck);
            return (it != playedAgainst.end()) ? it->second : Result::NONE;
        }

        size_t matchesPlayed() const {
            return wins + losses + draws;
        }

        float winrate() const {
            if (!(matchesPlayed() < MATCHES_THRESHOLD)) // Avoids division by zero
                return (float(wins) / float(matchesPlayed()));
            return -1.f;
        }
    };

    static std::vector<CardContainer> decks;
    static std::vector<Stats> stats;

    static void recordMatchResult(ID redDeck, ID blueDeck, Player winner) {
        Result redResult, blueResult;

        switch (winner) {
        case PLAYER_RED:
            redResult = Result::WIN;
            blueResult = Result::LOSS;
            stats[redDeck].wins++;
            stats[blueDeck].losses++;
            break;
        case PLAYER_BLUE:
            redResult = Result::LOSS;
            blueResult = Result::WIN;
            stats[redDeck].losses++;
            stats[blueDeck].wins++;
            break;
        case PLAYER_NONE:
            redResult = Result::DRAW;
            blueResult = Result::DRAW;
            stats[redDeck].draws++;
            stats[blueDeck].draws++;
            break;
        }

        stats[redDeck].playedAgainst[blueDeck] = redResult;
        stats[blueDeck].playedAgainst[redDeck] = blueResult;
    }

    static ID bestPerformingID() {
        size_t bestID = 0;
        float bestWinrate = -1.f;
        for (size_t i = 0; i < stats.size(); i++)
            if (stats[i].winrate() > bestWinrate) {
                bestWinrate = stats[i].winrate();
                bestID = i;
            }
        return bestID;
    }

    static ID getID(const CardContainer& deck) {
        for (size_t i = 0; i < stats.size(); i++)
            if (decks[i] == deck)
                return i;
        std::cout << "DeckStats::getID() failed to find any deck which matched" << std::endl;
    }

    static void sort(CardContainer& deck) {
        std::sort(deck.begin(), deck.end());
    }

    static bool hasDuplicates(const CardContainer& deck) {
        for (size_t i = 0; i < DECK_SIZE; i++)
            for (size_t j = i + 1; j < DECK_SIZE; j++)
                if (deck[i] == deck[j])
                    return true;
        return false;
    }

    static bool isLegal(const CardContainer& deck) {
        // Deck rules: no more than 1 card of 5-star rarity per deck, 
        // and no more than 2 cards of 4-star rarity or higher per deck
        // Also no duplicates

        if (hasDuplicates(deck))
            return false;

        int fourStarCount = 0, fiveStarCount = 0;
        for (size_t i = 0; i < DECK_SIZE; i++)
            if (CardCollection::cards[deck[i]].stars == 4)
                fourStarCount++;
            else if (CardCollection::cards[deck[i]].stars == 5)
                fiveStarCount++;

        return (fourStarCount + fiveStarCount) <= 2
            && fiveStarCount < 2 && deck.size() == DECK_SIZE;
    }

    static size_t getRandomID() {
        return std::rand() % stats.size();
    }

    static CardContainer createRandom() {
        CardContainer tmpDeck;
        do {
            tmpDeck.clear();
            for (size_t j = 0; j < DECK_SIZE; j++) {
                tmpDeck.push_back(CardCollection::getRandomID());
            }
        } while (!isLegal(tmpDeck));
        std::sort(tmpDeck.begin(), tmpDeck.end());
        return tmpDeck;
    }

    static bool matchFound(const CardContainer& targetDeck) {
        // Assumes targetDeck is sorted
        for (size_t i = 0; i < stats.size(); i++)
            if (targetDeck == decks[i])
                return true;
        return false;
    }

    static void addIfUnique(CardContainer deck) {
        sort(deck);
        if (!matchFound(deck)) {
            decks.push_back(deck);
            stats.emplace_back();
        }
    }

    static void initRandoms(size_t count) {
        while (stats.size() < count)
            addIfUnique(createRandom());
    }

    static void initAllPossible() {
        CardContainer deck(DECK_SIZE);
        std::vector<uint16_t> cards;
        std::iota(cards.begin(), cards.end(), 1);  // Fill with {1, 2, 3, ..., TRIPLE_TRIAD_CARD_COUNT}

        // Generate all unique 5-card combinations
        std::vector<bool> selector(cards.size(), false);
        std::fill(selector.begin(), selector.begin() + DECK_SIZE, true);  // First DECK_SIZE elements = true

        do {
            size_t index = 0;
            for (size_t i = 0; i < cards.size(); i++)
                if (selector[i]) deck[index++] = cards[i];

            addIfUnique(deck);
        } while (std::prev_permutation(selector.begin(), selector.end()));
    }

    static void print(const ID id) {
        std::cout << std::endl;
        std::cout << "Winrate: " << stats[id].winrate() * 100 << "%" << std::endl;
        std::cout << "Best Win-Loss-Draw: " << std::endl;
        std::cout << "W: " << stats[id].wins << " | L: " << stats[id].losses << " | D: " << stats[id].draws << std::endl;
        std::cout << "Deck List: " << std::endl;

        for (size_t i = 0; i < decks[id].size(); i++)
            std::cout << CardCollection::names[decks[id][i]] << " (" << int(CardCollection::cards[decks[id][i]].stars) << "-Star), ";
        std::cout << std::endl;
    }

    static void printBest() {
        print(bestPerformingID());
    }
}

static Player otherPlayer(Player player) {
    if (player == PLAYER_RED)
        return PLAYER_BLUE;
    else
        return PLAYER_RED;
}

struct Board {
    enum Border { TOP_BORDER = 0, RIGHT_BORDER = 2, BOTTOM_BORDER = 2, LEFT_BORDER = 0 };
    static constexpr uint8_t PLAYER_COUNT = 2;
    static constexpr uint8_t WIDTH = 3;
    static constexpr uint8_t HEIGHT = 3;

    std::vector<CardContainer> hand = std::vector<CardContainer>(PLAYER_COUNT);
    std::vector<ID> deckIDs = std::vector<ID>(PLAYER_COUNT);

    Player currentPlayer;
    Card cards[WIDTH][HEIGHT];

    struct PossibleMove {
        uint8_t col, row;
        ID card;

        PossibleMove(uint8_t _col, uint8_t _row, ID _card)
            : col(_col), row(_row), card(_card) {
        }
    };

    Board(ID redDeck, ID blueDeck) {
        currentPlayer = PLAYER_RED;

        // Initialize the board with empty cards
        for (size_t row = 0; row < HEIGHT; row++)
            for (size_t col = 0; col < WIDTH; col++)
                cards[col][row] = Card();
        deckIDs[PLAYER_RED] = redDeck;
        deckIDs[PLAYER_BLUE] = blueDeck;
        hand[PLAYER_RED] = DeckStats::decks[redDeck];
        hand[PLAYER_BLUE] = DeckStats::decks[blueDeck];
    }

    size_t hash() const {
        size_t h = 0;
        std::hash<int> hasher;

        // Hash board state (card IDs + controlling colors)
        for (size_t row = 0; row < HEIGHT; ++row)
            for (size_t col = 0; col < WIDTH; ++col)
                h ^= hasher(cards[row][col].id) ^ (hasher(cards[row][col].controllingColor) << 1);

        // Hash player hands (requires hands to be sorted)
        for (size_t player = 0; player < PLAYER_COUNT; ++player)
            for (size_t card = 0; card < hand[player].size(); ++card)
                h ^= hasher(hand[player][card]) << 3;

        return h;
    }


    void initPlayerByPickingRandomDeck(Player player) {
        const size_t deckID = DeckStats::getRandomID();
        deckIDs[player] = deckID;
        hand[player] = DeckStats::decks[deckID];
    }

    std::vector<PossibleMove> getAllPossibleMoves() const {
        const uint8_t handSize = hand[currentPlayer].size();
        std::vector<PossibleMove> possibleMoves;
        for (uint8_t col = 0; col < WIDTH; col++)
            for (uint8_t row = 0; row < HEIGHT; row++)
                if (isEmpty(col, row))
                    for (size_t i = 0; i < handSize; i++)
                        possibleMoves.emplace_back(col, row, hand[currentPlayer][i]);

        return possibleMoves;
    }

    void flip(Card& card) {
        card.controllingColor = otherPlayer(card.controllingColor);
    }

    void removeCardFromHand(ID cardID) {
        hand[currentPlayer].erase(
            std::remove(hand[currentPlayer].begin(), hand[currentPlayer].end(), cardID), hand[currentPlayer].end());
    }

    void sortHand() {
        std::sort(hand[currentPlayer].begin(), hand[currentPlayer].end(), std::greater<ID>());
    }

    void playCard(PossibleMove move) {
        placeCard(move.card, move.col, move.row);  // Place the card on the board
        removeCardFromHand(move.card);  // Remove the card from the hand
        sortHand(); // This should make transposition-hashing faster
        currentPlayer = otherPlayer(currentPlayer);
    }

    void placeCard(ID cardID, int col, int row) {
        Card placedCard = CardCollection::cards[cardID];
        placedCard.controllingColor = currentPlayer;
        cards[col][row] = placedCard;

        // Check Top For Flips
        if (row > TOP_BORDER) {
            Card& adjacentCard = cards[col][row - 1];
            if (adjacentCard.isEnemy(placedCard)
                && adjacentCard.attribute[BOTTOM] < placedCard.attribute[TOP])
                flip(adjacentCard);
        }

        // Check Right For Flips
        if (col < RIGHT_BORDER) {
            Card& adjacentCard = cards[col + 1][row];
            if (adjacentCard.isEnemy(placedCard)
                && adjacentCard.attribute[LEFT] < placedCard.attribute[RIGHT])
                flip(adjacentCard);
        }

        // Check Bottom For Flips
        if (row < BOTTOM_BORDER) {
            Card& adjacentCard = cards[col][row + 1];
            if (adjacentCard.isEnemy(placedCard)
                && adjacentCard.attribute[TOP] < placedCard.attribute[BOTTOM])
                flip(adjacentCard);
        }

        // Check Left For Flips
        if (col > LEFT_BORDER) {
            Card& adjacentCard = cards[col - 1][row];
            if (adjacentCard.isEnemy(placedCard)
                && adjacentCard.attribute[RIGHT] < placedCard.attribute[LEFT])
                flip(adjacentCard);
        }
    }

    bool matchEnded() const {
        for (size_t col = 0; col < WIDTH; col++)
            for (size_t row = 0; row < HEIGHT; row++)
                if (cards[col][row].controllingColor == PLAYER_NONE)
                    return false;
        return true; // The game is over when all spaces have been filled
    }

    bool isEmpty(int col, int row) const {
        return cards[col][row].controllingColor == PLAYER_NONE;
    }

    void printColors() const {
        for (uint8_t row = 0; row < HEIGHT; row++) {
            for (uint8_t col = 0; col < WIDTH; col++) {
                std::cout << colorToChar(cards[col][row].controllingColor) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    Player winningPlayer() {
        if (!matchEnded())
            std::cout << "Error: Tried to call winningPlayer() on an unfinished game" << std::endl;

        size_t cardsControlled[PLAYER_COUNT] = { 0 };
        for (size_t col = 0; col < WIDTH; col++)
            for (size_t row = 0; row < HEIGHT; row++) {
                Player controllingPlayer = cards[col][row].controllingColor;
                if (controllingPlayer != PLAYER_NONE)
                    cardsControlled[controllingPlayer]++;
            }

        cardsControlled[PLAYER_BLUE]++; // Blue gets a handicap of 1 card for going second
        if (cardsControlled[PLAYER_RED] > cardsControlled[PLAYER_BLUE])
            return PLAYER_RED;
        if (cardsControlled[PLAYER_BLUE] > cardsControlled[PLAYER_RED])
            return PLAYER_BLUE;
        return PLAYER_NONE; // Tie
    }

    CardContainer cardContainerFrom3x3() const {
        CardContainer IDs;
        IDs.reserve(3 * 3); // Optimize by reserving space for 9 elements
        for (size_t row = 0; row < 3; row++)    // Iterate rows first
            for (size_t col = 0; col < 3; col++) // Then columns
                IDs.push_back(cards[row][col].id); // Maintain row-major order
        return IDs;
    }
};

struct TranspositionEntry {
    Player result;  // Stores the determined game outcome (PLAYER_RED, PLAYER_BLUE, or PLAYER_NONE)
    int depth;      // Depth at which this position was analyzed
};

static std::unordered_map<size_t, TranspositionEntry> transpositionTable;

Player alphaBeta(Board& thisBoard, Player maximizingPlayer, int depth = 1) {
    /* size_t boardHash = thisBoard.hash();

    Check transposition table
    auto it = transpositionTable.find(boardHash);
    if (it != transpositionTable.end() && it->second.depth >= depth) {
        return it->second.result;  // Use cached result if available at sufficient depth
    }
    */

    if (thisBoard.matchEnded()) {
        return thisBoard.winningPlayer();  // Returns PLAYER_RED, PLAYER_BLUE, or PLAYER_NONE
    }

    Player bestOutcome = (maximizingPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED;  // Worst case scenario

    const auto& possibleMoves = thisBoard.getAllPossibleMoves();
    for (const auto& move : possibleMoves) {
        Board childBoard = thisBoard;
        childBoard.playCard(move);

        Player eval = alphaBeta(childBoard, (maximizingPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED, depth + 1);

        // Maximizing Player (RED)
        if (maximizingPlayer == PLAYER_RED) {
            if (eval == PLAYER_RED) {
                transpositionTable[childBoard.hash()] = { PLAYER_RED, depth };  // Store correct hash
                return PLAYER_RED;  // Immediate win, prune further
            }
            if (eval == PLAYER_NONE) bestOutcome = PLAYER_NONE;  // Prefer a draw over losing
        }
        // Minimizing Player (BLUE)
        else {
            if (eval == PLAYER_BLUE) {
                transpositionTable[childBoard.hash()] = { PLAYER_BLUE, depth };
                return PLAYER_BLUE;  // Immediate win for opponent, prune
            }
            if (eval == PLAYER_NONE) bestOutcome = PLAYER_NONE;  // Prefer a draw over losing
        }
    }

    // Store result in transposition table using `childBoard.hash()`
    //transpositionTable[boardHash] = { bestOutcome, depth };
    return bestOutcome;
}


Board findBestMove(Board& board) {
    Board bestMove = board;  // Default to current board
    Player currentPlayer = board.currentPlayer;
    Player bestOutcome = PLAYER_BLUE;  // Assume worst case for PLAYER_RED (losing)

    if (currentPlayer == PLAYER_BLUE) {
        bestOutcome = PLAYER_RED;  // Assume worst case for PLAYER_BLUE (losing)
    }

    const auto& possibleMoves = board.getAllPossibleMoves();
    for (const auto& move : possibleMoves) {
        Board newBoard = board;
        newBoard.playCard(move);

        Player eval = alphaBeta(newBoard, (currentPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED);

        if (eval == currentPlayer) {  // Winning move found
            return newBoard;
        }
        if (eval == PLAYER_NONE && bestOutcome != currentPlayer) {  // Prefer a draw over a loss
            bestOutcome = PLAYER_NONE;
            bestMove = newBoard;
        }
        else if (bestOutcome != currentPlayer && eval != currentPlayer) {
            // If still in worst case, update to the first available move
            bestMove = newBoard;
        }
    }

    return bestMove;
}

static size_t square(const size_t value) {
    return value * value;
}

// ---------------------- MATCH GAMEPLAY HAPPENS BELOW THUS SDL DRAW FUNCTIONS ARE BELOW --------------------

static void findBestCounter(ID targetDeck) {
    std::vector<ID> decksWhichWonAgainst;

    // --- Play Matches As PLAYER_RED into targetDeck ---
    for (size_t i = 0; i < DeckStats::stats.size(); i++) {
        Board board = Board(i, targetDeck);
        transpositionTable.clear();

        while (!board.matchEnded()) {
            Board bestMove = findBestMove(board);
            board = bestMove;
        }

        if (board.winningPlayer() == PLAYER_RED)
            decksWhichWonAgainst.push_back(board.deckIDs[PLAYER_RED]);
    }

    // --- Play Matches As PLAYER_BLUE into targetDeck ---
    std::vector<ID> finalistDecks;
    for (size_t i = 0; i < decksWhichWonAgainst.size(); i++) {
        Board board = Board(targetDeck, i);
        transpositionTable.clear();

        while (!board.matchEnded()) {
            Board bestMove = findBestMove(board);
            board = bestMove;
        }

        // Filter Out Losses From Finalist Decks
        if (board.winningPlayer() == PLAYER_BLUE || board.winningPlayer() == PLAYER_NONE)
            finalistDecks.push_back(board.deckIDs[PLAYER_BLUE]);
    }

    std::cout << "Finalist Count: " << finalistDecks.size() << std::endl << std::endl;

    // Print The Results
    const size_t finalistDeckCount = finalistDecks.size();
    for (size_t i = 0; i < finalistDeckCount; i++)
        DeckStats::print(finalistDecks[i]);

    std::cout << std::endl;
    std::cout << (float(finalistDeckCount * 100) / float(DeckStats::stats.size())) << "% Of Decks Were Able To Win-Win or Win-Draw Against It" << std::endl;
}

namespace Clock {
    static std::chrono::steady_clock::time_point timeLast;

    static void init() {
        timeLast = std::chrono::steady_clock::now();
    }

    static bool intervalPassed(int intervalInSeconds) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::seconds interval(intervalInSeconds);  // Convert seconds to chrono duration
        return (now - timeLast) >= interval;
    }

    static void intervalReset() {
        timeLast = std::chrono::steady_clock::now();
    }

    static void printProgressEveryXseconds(float currentValue, float maxValue, size_t timeInSeconds) {
        if (intervalPassed(timeInSeconds)) {
            std::cout << "Progress: " << std::fixed << std::setprecision(2)
                << ((currentValue / maxValue) * 100) << "%" << std::endl;
            intervalReset();
        }
    }
}

static SDL_Window* WINDOW = nullptr;
static SDL_Renderer* RENDERER = nullptr;
static constexpr size_t WINDOW_WIDTH = 3440;
static constexpr size_t WINDOW_HEIGHT = 1440;

namespace Color {
    static constexpr SDL_Color WHITE = { 255, 255, 255, 255 };
    static constexpr SDL_Color LIGHT_GRAY = { 211, 211, 211, 255 };
    static constexpr SDL_Color DARK_GRAY = { 170, 170, 170, 255 };
    static constexpr SDL_Color BLUE = { 55, 55, 175, 255 };
    static constexpr SDL_Color BLACK = { 0, 0, 0, 255 };
    static constexpr SDL_Color RED = { 175, 55, 55, 255 };
    static constexpr SDL_Color GREEN = { 100, 255, 0, 255 };
}

namespace GraphicsSDL {
    // Text Display Settings
    static constexpr const char* FONT_PATH = "Transformers Movie.ttf";
    static constexpr SDL_Color TEXT_COLOR = Color::BLACK;
    static constexpr float FONT_SIZE = 48; // 12pt
    static TTF_Font* FONT;

    // ----------------------------------------------------------------------
    // Custom overloaded wrapper functions for readability
    // These assume things like the renderer from global scope and minimize argument variables
    static void SetRenderDrawColor(const SDL_Color color) {
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
    }

    static void RenderClear() {
        SDL_RenderClear(RENDERER);
    }

    static void RenderTexture(SDL_Texture* texture, SDL_FRect* dstRect) {
        SDL_RenderTexture(RENDERER, texture, nullptr, dstRect);
    }

    static void RenderPresent() {
        SDL_RenderPresent(RENDERER);
    }

    static SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface) {
        return SDL_CreateTextureFromSurface(RENDERER, surface);
    }

    // End of wrapper functions ---------------------------------------------

    static void text(float x, float y, float pt, bool centered, SDL_Color color, const char* text) {
        if (strlen(text) == 0)
            std::cout << "GrpaphicsSDL::text() Tried using text with 0 length" << std::endl;

        SDL_Surface* textSurface = TTF_RenderText_Solid(FONT, text, strlen(text), color);
        if (!textSurface)
            std::cout << "GraphicsSDL::text() Failure to create surface from font. " << SDL_GetError() << std::endl;

        SDL_Texture* textTexture = CreateTextureFromSurface(textSurface);
        if (!textTexture)
            std::cout << "GraphicsSDL::text() Failure to create texture from surface. " << SDL_GetError() << std::endl;

        const int textWidth = textSurface->w;
        const int textHeight = textSurface->h;
        SDL_DestroySurface(textSurface);

        if (centered) { // Center the text
            x -= textWidth / 2; 
            y -= textHeight / 2;
        }
        SDL_FRect destRect = { x, y, textWidth, textHeight };
        RenderTexture(textTexture, &destRect);
        SDL_DestroyTexture(textTexture);
    }

    static void init() {
        if (!SDL_Init(SDL_INIT_VIDEO))
            std::cout << "GraphicsSDL::init() SDL_Init failed: " << SDL_GetError() << std::endl;

        WINDOW = SDL_CreateWindow("Triple Triad Solver", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
        if (!WINDOW)
            std::cout << "GraphicsSDL::init() SDL_CreateWindow failed: " << SDL_GetError() << std::endl;

        RENDERER = SDL_CreateRenderer(WINDOW, NULL);
        if (!RENDERER)
            std::cout << "GraphicsSDL::init() SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;

        if (!TTF_Init())
            std::cout << "GraphicsSDL::init() TTF_Init failed: " << SDL_GetError() << std::endl;

        FONT = TTF_OpenFont(FONT_PATH, FONT_SIZE);
        if (!FONT)
            std::cout << "GraphicsSDL::init() TTF_OpenFont failed: " << SDL_GetError() << std::endl;
    }

    static void eventLoop() {
        bool running = true;
        SDL_Event event;
        while (running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT)
                    running = false;
            }
        }
    }

    static void cleanup() {
        TTF_CloseFont(FONT);
        SDL_DestroyRenderer(RENDERER);
        SDL_DestroyWindow(WINDOW);
        SDL_Quit();
    }
}

/* For a simple graphics abstraction layer for SDL, providing basic
drawing utilities to separate rendering logic from game logic */
namespace Graphics {
    static float LINE_THICKNESS, MARGIN;

    // A global allocation buffer which holds and manages all image textures
    namespace TextureCache {
        static constexpr const char* MISSING_TEXTURE_FILEPATH = "MISSING CARD.png";
        static constexpr float SCALE_FACTOR = 2.f; // Scale card images to 2x size
        static constexpr float CARD_WIDTH = 100.f * SCALE_FACTOR;
        static constexpr float CARD_HEIGHT = 126.f * SCALE_FACTOR;
        static constexpr float CARD_HALF_WIDTH = CARD_WIDTH / 2.f;
        static constexpr float CARD_HALF_HEIGHT = CARD_HEIGHT / 2.f;

        static std::vector<SDL_Texture*> card_images;
        static std::vector<bool> missingTextures;
        static ID RED_CARD;
        static ID BLUE_CARD;
        static bool cardSizeInitalized = false;

        static void textureAddCard(const std::string& filePath) {
            if (filePath == "RED CARD.png")
                RED_CARD = card_images.size();
            else if (filePath == "BLUE CARD.png")
                BLUE_CARD = card_images.size();

            bool textureMissing = false;
            SDL_Texture* newTexture = IMG_LoadTexture(RENDERER, filePath.c_str());
            if (!newTexture) {
                std::cout << "Error: Failed to load texture in TextureCache::textureAddCard()" << std::endl;
                std::cout << "Error Message: " << SDL_GetError() << std::endl;
                newTexture = IMG_LoadTexture(RENDERER, MISSING_TEXTURE_FILEPATH);
                textureMissing = true;
                if (!newTexture) {
                    std::cout << "Error: Also failed to load missing texture in TextureCache::textureAddCard()" << std::endl;
                    std::cout << "Error Message: " << SDL_GetError() << std::endl;
                }
            }

            if (textureMissing)
                missingTextures.push_back(true);
            else
                missingTextures.push_back(false);

            card_images.emplace_back(newTexture);
        }

        static void textureClear(SDL_Texture* texture) {
            if (!texture)
                std::cout << "Error: tried to clear invalid texture in TextureCache::textureClear()" << std::endl;
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        static void clear() {
            for (auto& image : card_images)
                textureClear(image);
            card_images.clear();
        }

        // Draw an image from the pool using it's ID
        static void draw(ID imgID, float x, float y) {
            if (imgID < 0 || imgID >= card_images.size() || !card_images[imgID])
                std::cout << "Error: Image ID out of bounds or texture not loaded in TextureCache::draw()" << std::endl;

            SDL_Texture* texture = card_images[imgID];
            float imgWidth, imgHeight;
            SDL_GetTextureSize(texture, &imgWidth, &imgHeight);
            SDL_FRect dstRect = { x, y, imgWidth, imgHeight };
            GraphicsSDL::RenderTexture(texture, &dstRect);
        }

        static void init() {
            for (size_t i = 0; i < CardCollection::cards.size(); i++)
                textureAddCard(CardCollection::names[i] + ".png");
            textureAddCard("RED CARD.png");
            textureAddCard("BLUE CARD.png");
        }
    }

    static void background() {
        GraphicsSDL::SetRenderDrawColor(Color::BLACK);
    }

    static void line(float x1, float y1, float x2, float y2) {
        GraphicsSDL::SetRenderDrawColor(Color::WHITE);
        SDL_FRect rect = { x1, y1, x2 - x1, y2 - y1 };
        SDL_RenderFillRect(RENDERER, &rect);
    }

    static void image(const ID imageID, const float x, const float y) {
        if (!TextureCache::card_images[imageID])
            std::cout << "Error: Attempted to render an image with an invalid texture in Graphics::image()" << std::endl;
        SDL_FRect destRect = { x, y, TextureCache::CARD_WIDTH, TextureCache::CARD_HEIGHT };
        SDL_RenderTexture(RENDERER, TextureCache::card_images[imageID], nullptr, &destRect);
    }

    // String Version
    static void textCentered(float x, float y, float pt, SDL_Color color, std::string& str) {
        GraphicsSDL::text(x, y, pt * TextureCache::SCALE_FACTOR, true, color, str.c_str());
    }

    // Single Char Version
    static void textCentered(const float x, const float y, const float pt, SDL_Color color, char ch) {
        GraphicsSDL::text(x, y, pt * TextureCache::SCALE_FACTOR, true, color, &ch);
    }

    namespace Attributes {
        static constexpr float OFFSET_MULTIPLIER = 0.6f; // 60% of the way to each edge
        static float VERTICAL_OFFSET, HORIZONTAL_OFFSET;
        static bool initalized = false;

        // Initialize edge offsets based on half-card width and height
        static void init() {
            VERTICAL_OFFSET = OFFSET_MULTIPLIER * TextureCache::CARD_HALF_HEIGHT;
            HORIZONTAL_OFFSET = OFFSET_MULTIPLIER * TextureCache::CARD_HALF_WIDTH;
            initalized = true;
        }

        static void draw(int *attribute, Player color, float x, float y) {
            if (!initalized)
                std::cout << "Graphics::Attribute::draw() was never initialized!" << std::endl;

            if (color != PLAYER_NONE)
                for (size_t edge = 0; edge < 4; edge++) {
                    float xAfterOffset = x;
                    float yAfterOffset = y;

                    // Apply vertical position modifications to vertical edges
                    if (edge == Edge::TOP)
                        yAfterOffset -= VERTICAL_OFFSET;
                    else if (edge == Edge::BOTTOM)
                        yAfterOffset += VERTICAL_OFFSET;
                    else if (edge == Edge::LEFT)
                        xAfterOffset -= HORIZONTAL_OFFSET;
                    else if (edge == Edge::RIGHT)
                        xAfterOffset += HORIZONTAL_OFFSET;

                    const char& c = attributeToChar(attribute[edge]);
                    textCentered(xAfterOffset, yAfterOffset, GraphicsSDL::FONT_SIZE, Color::WHITE, c);
                }
        }
    };

    struct CardGrid {
        enum Location {
            TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, CENTER
        };

        Location location;
        std::vector<CardContainer> cardIDs;
        std::vector<std::vector<Player>> owner;
        float xStart, yStart, drawWidth, drawHeight;
        size_t colCount, rowCount;

        CardGrid() = default;

        CardGrid(Location _location, std::vector<CardContainer> _cardIDs, std::vector<std::vector<Player>> owner)
            : location(_location), cardIDs(_cardIDs), owner(owner) {

            rowCount = cardIDs.at(0).size();
            colCount = cardIDs.size();

            drawWidth = TextureCache::CARD_WIDTH * rowCount;
            drawHeight = TextureCache::CARD_HEIGHT * colCount;

            switch (location) {
            case TOP_LEFT:
                xStart = 0;
                yStart = 0;
                break;
            case TOP_RIGHT:
                xStart = WINDOW_WIDTH - drawWidth;
                yStart = 0;
                break;
            case BOTTOM_LEFT:
                xStart = MARGIN;
                yStart = WINDOW_HEIGHT - drawHeight;
                break;
            case BOTTOM_RIGHT:
                xStart = WINDOW_WIDTH - drawWidth;
                yStart = WINDOW_HEIGHT - drawHeight;
                break;
            case CENTER:
                xStart = (WINDOW_WIDTH / 2.f) - (drawWidth / 2.f);
                yStart = (WINDOW_HEIGHT / 2.f) - (drawHeight / 2.f);
                break;
            }
        }

        static void init() {
            LINE_THICKNESS = 0.02f * TextureCache::CARD_WIDTH;  // Line thickness as a percentage of CARD_WIDTH
            MARGIN = LINE_THICKNESS * 3;  // Space between the cells and edges of the grid
        }

        void draw() {
            for (size_t col = 0; col < colCount; ++col) {
                for (size_t row = 0; row < rowCount; ++row) {
                    float cardX = xStart + (row * TextureCache::CARD_WIDTH);
                    float cardY = yStart + (col * TextureCache::CARD_HEIGHT);

                    // Determine card texture based on owner
                    const auto& cardTexture = (owner[col][row] == Player::PLAYER_RED)
                        ? TextureCache::RED_CARD
                        : TextureCache::BLUE_CARD;

                    // Draw the card
                    image(cardTexture, cardX, cardY);

                    // Calculate center position for attribute drawing
                    float xCenter = cardX + TextureCache::CARD_HALF_WIDTH;
                    float yCenter = cardY + TextureCache::CARD_HALF_HEIGHT;

                    // Retrieve card attributes in a structured way
                    const auto& card = CardCollection::cards[cardIDs[col][row]];
                    int attribute[4] = { card.attribute[0], card.attribute[1], card.attribute[2], card.attribute[3] };

                    // Draw the card image and its attributes
                    image(cardIDs[col][row], cardX, cardY);
                    Attributes::draw(attribute, owner[col][row], xCenter, yCenter);
                }
            }
        }

        static void drawGame(const Board& board) {
            GraphicsSDL::RenderClear();
            Graphics::background();

            std::vector<CardGrid> drawables;

            // Board
            std::vector<CardContainer> boardCardIDs(Board::WIDTH);
            std::vector<std::vector<Player>> boardOwnership(Board::WIDTH);

            for (size_t row = 0; row < Board::WIDTH; row++) {
                for (size_t col = 0; col < Board::HEIGHT; col++) {
                    boardCardIDs[row].emplace_back(board.cards[col][row].id);
                    boardOwnership[row].emplace_back(board.cards[col][row].controllingColor);
                }
            }
            drawables.push_back(CardGrid(CENTER, boardCardIDs, boardOwnership));

            // Red hand
            CardContainer redHand = board.hand[PLAYER_RED];
            while (redHand.size() < HAND_SIZE) {
                redHand.push_back(0);
            }
            std::vector<Player> redOwnership(HAND_SIZE, PLAYER_RED);
            drawables.push_back(CardGrid(BOTTOM_LEFT, { redHand }, { redOwnership }));

            // Blue hand
            CardContainer blueHand = board.hand[PLAYER_BLUE];
            while (blueHand.size() < HAND_SIZE) {
                blueHand.push_back(0);
            }
            std::vector<Player> blueOwnership(HAND_SIZE, PLAYER_BLUE);
            drawables.push_back(CardGrid(BOTTOM_RIGHT, { blueHand }, { blueOwnership }));

            // Decks
            drawables.push_back(CardGrid(TOP_LEFT, { DeckStats::decks[board.deckIDs[PLAYER_RED]] }, { redOwnership }));
            drawables.push_back(CardGrid(TOP_RIGHT, { DeckStats::decks[board.deckIDs[PLAYER_BLUE]] }, { blueOwnership }));

            // Draw elements
            for (size_t i = 0; i < drawables.size(); i++) {
                drawables[i].draw();
            }
        }
    };

    static void init() {
        GraphicsSDL::init();
        TextureCache::init();
        CardGrid::init();
        Attributes::init();
    }

    static void cleanup() {
        TextureCache::clear();
        GraphicsSDL::cleanup();
    }
}

/* Starts a match simulation loop where decks play against
 each other at random and winners are selected by winrate */
static void playRandomMatchups() {
    const size_t sufficientDecks = std::sqrt(DeckStats::MATCHES_TO_PLAY) * 2;
    if (DeckStats::stats.size() < sufficientDecks)
        std::cout << "Error: playRandomatchups() does not enough decks to satisfy MATCHES_TO_PLAY... OR, will be EXTREMELY slow" << std::endl;
    /* Decks aren't allowed to play against each other more than once, and since they are picked at random,
    the probability of finding an eligible match becomes increasingly more difficult the more matches are played,
    and in some cases impossible. This is intended to solve that, though could be improved */

    // Loop through matches
    size_t matchesPlayed = 0;
    while (matchesPlayed < DeckStats::MATCHES_TO_PLAY) {
        const ID redDeckID = DeckStats::getRandomID();
        const ID blueDeckID = DeckStats::getRandomID();
        Board board = Board(redDeckID, blueDeckID);

        // Prevents repeat matchups
        if (!DeckStats::stats[board.deckIDs[PLAYER_RED]].
            hasPlayedAgainst(board.deckIDs[PLAYER_BLUE])) {
            transpositionTable.clear();
            size_t movesPlayed = 0;

            while (!board.matchEnded()) {
                Board bestMove = findBestMove(board); board = bestMove;
                movesPlayed++;
            }

            DeckStats::recordMatchResult(board.deckIDs[PLAYER_RED],
                board.deckIDs[PLAYER_BLUE], board.winningPlayer());
            matchesPlayed++;
            Clock::printProgressEveryXseconds(matchesPlayed, DeckStats::MATCHES_TO_PLAY, 3);
        }
    }

    std::cout << "Games simulated: " << matchesPlayed << std::endl;
    DeckStats::printBest();
}

// Insert a deck of your choice here if you want to see what counters it best!
static void playMatchupsAgainstPreloadedDeck() {
    CardContainer targetDeck = {
        CardCollection::getIDfromName("Shantotto"),
        CardCollection::getIDfromName("Vaan"),
        CardCollection::getIDfromName("Lightning"),
        CardCollection::getIDfromName("Noctis Lucis Caelum"),
        CardCollection::getIDfromName("Clive Rosfield")
    };
    DeckStats::addIfUnique(targetDeck);
    findBestCounter(DeckStats::getID(targetDeck));
}

static void graphicallyResimulateMatch(ID redDeck, ID blueDeck) {
    Graphics::background();
    transpositionTable.clear();
    Board board = Board(redDeck, blueDeck);
    Graphics::CardGrid::drawGame(board);

    while (!board.matchEnded()) {
        Board bestMove = findBestMove(board);
        board = bestMove;
        SDL_Delay(16 * 360);  // Cap to 0.18ish fps
        Graphics::CardGrid::drawGame(board);
    }

    Graphics::CardGrid::drawGame(board);
}

static void graphicallyResimulateMatchManual(ID redDeck, ID blueDeck) {
    while (true) {  // Restart loop
        Graphics::background();
        transpositionTable.clear();
        Board board = Board(redDeck, blueDeck);
        Graphics::CardGrid::drawGame(board);
        GraphicsSDL::RenderPresent();  // Ensure first frame is visible

        while (!board.matchEnded()) {
            if (board.currentPlayer == PLAYER_RED) {
                bool validMove = false;
                int cardIndex, row, col;
                while (!validMove) {
                    std::cout << "Player RED, it's your turn!" << std::endl;
                    std::cout << "Select a card to play (1-" << board.hand[PLAYER_RED].size()
                        << ") followed by row(0-" << Board::WIDTH - 1 << ") and col(0-"
                        << Board::HEIGHT - 1 << "). Example: 412" << std::endl;

                    int input;
                    std::cin >> input;

                    // Extract values
                    cardIndex = input / 100 - 1;
                    row = (input / 10) % 10;
                    col = input % 10;

                    std::cout << "You selected card " << (cardIndex + 1)
                        << " to be placed at (" << row << ", " << col << ")" << std::endl;

                    if (cardIndex >= board.hand[PLAYER_RED].size() || cardIndex < 0)
                        std::cout << "Invalid card selection. Try again." << std::endl;
                    else if (row < 0 || row >= Board::WIDTH || col < 0 || col >= Board::HEIGHT)
                        std::cout << "Out-of-bounds placement. Try again." << std::endl;
                    else if (!board.isEmpty(col, row))
                        std::cout << "That spot is already occupied. Try again." << std::endl;
                    else
                        validMove = true;
                }

                auto move = Board::PossibleMove(col, row, board.hand[PLAYER_RED][cardIndex]);
                board.playCard(move);

                // Ensure player move is displayed
                Graphics::CardGrid::drawGame(board);
                GraphicsSDL::RenderPresent();
            }
            else {
                // Force render update before AI move
                Graphics::CardGrid::drawGame(board);
                GraphicsSDL::RenderPresent();
                SDL_Delay(100);  // Brief pause to ensure rendering

                // AI move
                Board bestMove = findBestMove(board);
                board = bestMove;

                // Ensure AI move is displayed
                Graphics::CardGrid::drawGame(board);
                GraphicsSDL::RenderPresent();
            }
        }

        // Final render
        Graphics::CardGrid::drawGame(board);
        GraphicsSDL::RenderPresent();

        // Wait for user input before restarting
        std::cout << "Match over! Press any key to restart..." << std::endl;
        std::cin.ignore();  // Clear newline if any left in buffer
        std::cin.get();  // Wait for user input
    }
}

// Main game loop
int main() {
    std::srand(static_cast<size_t>(std::time(0)));  // Seed RNG
    CardCollection::init();
    DeckStats::initRandoms(750);
    //playRandomMatchups();

    Graphics::init();

    // Watch a random match
    const ID redDeck = rand() % DeckStats::stats.size();
    const ID blueDeck = rand() % DeckStats::stats.size();
    //graphicallyResimulateMatchManual(redDeck, DeckStats::bestPerformingID());
    graphicallyResimulateMatchManual(redDeck, blueDeck);

    GraphicsSDL::eventLoop();
    Graphics::cleanup();

    return 0;
}
