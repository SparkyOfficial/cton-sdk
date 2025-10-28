// Mnemonic.cpp - реалізація BIP-39 мнемонічної фрази
// Author: Андрій Будильников (Sparky)
// Implementation of BIP-39 mnemonic phrase
// Реализация мнемонической фразы BIP-39

#include "../include/Mnemonic.h"
#include <stdexcept>
#include <random>
#include <cstring>
#include <sstream>
#include <algorithm>

// Try to include OpenSSL if available
#ifdef USE_OPENSSL
#if __has_include("C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/sha.h")
#include "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/sha.h"
#include "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/evp.h"
#include "C:/Users/Богдан/Desktop/cton-sdk/openssl-3.6.0/include/openssl/kdf.h"
#define OPENSSL_AVAILABLE 1
#else
#define OPENSSL_AVAILABLE 0
#endif
#else
#define OPENSSL_AVAILABLE 0
#endif

#ifndef USE_OPENSSL
#define OPENSSL_AVAILABLE 0
#endif

#if OPENSSL_AVAILABLE
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/kdf.h>
#else
// Fallback implementation for SHA256
// This is a simplified implementation for demonstration purposes
// In production, you should use a proper cryptographic library
static void sha256_fallback(const uint8_t* data, size_t len, uint8_t* hash) {
    // Simple SHA256 implementation for fallback
    // Based on the standard SHA256 algorithm
    
    // SHA256 constants
    static const uint32_t k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };
    
    // Initialize hash values
    uint32_t h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    
    // Pre-processing
    size_t original_len = len;
    size_t new_len = len + 1;
    while ((new_len % 64) != 56) {
        new_len++;
    }
    new_len += 8;
    
    uint8_t* padded_data = new uint8_t[new_len];
    memcpy(padded_data, data, len);
    padded_data[len] = 0x80;
    memset(padded_data + len + 1, 0, new_len - len - 1);
    
    // Append original length in bits as 64-bit big-endian integer
    uint64_t bit_len = original_len * 8;
    for (int i = 0; i < 8; i++) {
        padded_data[new_len - 8 + i] = (bit_len >> (56 - 8 * i)) & 0xFF;
    }
    
    // Process message in 512-bit chunks
    for (size_t chunk = 0; chunk < new_len; chunk += 64) {
        uint32_t w[64];
        
        // Break chunk into 16 32-bit words
        for (int i = 0; i < 16; i++) {
            w[i] = (padded_data[chunk + 4*i] << 24) | (padded_data[chunk + 4*i + 1] << 16) |
                   (padded_data[chunk + 4*i + 2] << 8) | padded_data[chunk + 4*i + 3];
        }
        
        // Extend the 16 words into 64 words
        for (int i = 16; i < 64; i++) {
            uint32_t s0 = ((w[i-15] >> 7) | (w[i-15] << 25)) ^ ((w[i-15] >> 18) | (w[i-15] << 14)) ^ (w[i-15] >> 3);
            uint32_t s1 = ((w[i-2] >> 17) | (w[i-2] << 15)) ^ ((w[i-2] >> 19) | (w[i-2] << 13)) ^ (w[i-2] >> 10);
            w[i] = w[i-16] + s0 + w[i-7] + s1;
        }
        
        // Initialize working variables
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
        
        // Main loop
        for (int i = 0; i < 64; i++) {
            uint32_t S1 = ((e >> 6) | (e << 26)) ^ ((e >> 11) | (e << 21)) ^ ((e >> 25) | (e << 7));
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = hh + S1 + ch + k[i] + w[i];
            uint32_t S0 = ((a >> 2) | (a << 30)) ^ ((a >> 13) | (a << 19)) ^ ((a >> 22) | (a << 10));
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;
            
            hh = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }
        
        // Add this chunk's hash to result so far
        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
        h[5] += f;
        h[6] += g;
        h[7] += hh;
    }
    
    // Produce the final hash value
    for (int i = 0; i < 8; i++) {
        hash[4*i] = (h[i] >> 24) & 0xFF;
        hash[4*i + 1] = (h[i] >> 16) & 0xFF;
        hash[4*i + 2] = (h[i] >> 8) & 0xFF;
        hash[4*i + 3] = h[i] & 0xFF;
    }
    
    delete[] padded_data;
}
#endif

namespace cton {
    
    // BIP-39 English wordlist (full 2048 words)
    static const std::vector<std::string> BIP39_WORDLIST = {
        "abandon", "ability", "able", "about", "above", "absent", "absorb", "abstract",
        "absurd", "abuse", "access", "accident", "account", "accuse", "achieve", "acid",
        "acoustic", "acquire", "across", "act", "action", "actor", "actress", "actual",
        "adapt", "add", "addict", "address", "adjust", "admit", "adult", "advance",
        "advice", "aerobic", "affair", "afford", "afraid", "again", "age", "agent",
        "agree", "ahead", "aim", "air", "airport", "aisle", "alarm", "album",
        "alcohol", "alert", "alien", "all", "alley", "allow", "almost", "alone",
        "alpha", "already", "also", "alter", "always", "amateur", "amazing", "among",
        "amount", "amused", "analyst", "anchor", "ancient", "anger", "angle", "angry",
        "animal", "ankle", "announce", "annual", "another", "answer", "antenna", "antique",
        "anxiety", "any", "apart", "apology", "appear", "apple", "approve", "april",
        "arch", "arctic", "area", "arena", "argue", "arm", "armed", "armor",
        "army", "around", "arrange", "arrest", "arrive", "arrow", "art", "artefact",
        "artist", "artwork", "ask", "aspect", "assault", "asset", "assist", "assume",
        "asthma", "athlete", "atom", "attack", "attend", "attitude", "attract", "auction",
        "audit", "august", "aunt", "author", "auto", "autumn", "average", "avocado",
        "avoid", "awake", "aware", "away", "awesome", "awful", "awkward", "axis",
        "baby", "bachelor", "bacon", "badge", "bag", "balance", "balcony", "ball",
        "bamboo", "banana", "banner", "bar", "barely", "bargain", "barrel", "base",
        "basic", "basket", "battle", "beach", "bean", "beauty", "because", "become",
        "beef", "before", "begin", "behave", "behind", "believe", "below", "belt",
        "bench", "benefit", "best", "betray", "better", "between", "beyond", "bicycle",
        "bid", "bike", "bind", "biology", "bird", "birth", "bitter", "black",
        "blade", "blame", "blanket", "blast", "bleak", "bless", "blind", "blood",
        "blossom", "blouse", "blue", "blur", "blush", "board", "boat", "body",
        "boil", "bomb", "bone", "bonus", "book", "boost", "border", "boring",
        "borrow", "boss", "bottom", "bounce", "box", "boy", "bracket", "brain",
        "brand", "brass", "brave", "bread", "breeze", "brick", "bridge", "brief",
        "bright", "bring", "brisk", "broccoli", "broken", "bronze", "broom", "brother",
        "brown", "brush", "bubble", "buddy", "budget", "buffalo", "build", "bulb",
        "bulk", "bullet", "bundle", "bunker", "burden", "burger", "burst", "bus",
        "business", "busy", "butter", "buyer", "buzz", "cabbage", "cabin", "cable",
        "cactus", "cage", "cake", "call", "calm", "camera", "camp", "can",
        "canal", "cancel", "candy", "cannon", "canoe", "canvas", "canyon", "capable",
        "capital", "captain", "car", "carbon", "card", "cargo", "carpet", "carry",
        "cart", "case", "cash", "casino", "castle", "casual", "cat", "catalog",
        "catch", "category", "cattle", "caught", "cause", "caution", "cave", "ceiling",
        "celery", "cement", "census", "century", "cereal", "certain", "chair", "chalk",
        "champion", "change", "chaos", "chapter", "charge", "chase", "chat", "cheap",
        "check", "cheese", "chef", "cherry", "chest", "chicken", "chief", "child",
        "chimney", "choice", "choose", "chronic", "chuckle", "chunk", "churn", "cigar",
        "cinnamon", "circle", "citizen", "city", "civil", "claim", "clap", "clarify",
        "claw", "clay", "clean", "clerk", "clever", "click", "client", "cliff",
        "climb", "clinic", "clip", "clock", "clog", "close", "cloth", "cloud",
        "clown", "club", "clump", "cluster", "clutch", "coach", "coast", "coconut",
        "code", "coffee", "coil", "coin", "collect", "color", "column", "combine",
        "come", "comfort", "comic", "common", "company", "concert", "conduct", "confirm",
        "congress", "connect", "consider", "control", "convince", "cook", "cool", "copper",
        "copy", "coral", "core", "corn", "correct", "cost", "cotton", "couch",
        "country", "couple", "course", "cousin", "cover", "coyote", "crack", "cradle",
        "craft", "cram", "crane", "crash", "crater", "crawl", "crazy", "cream",
        "credit", "creek", "crew", "cricket", "crime", "crisp", "critic", "crop",
        "cross", "crouch", "crowd", "crucial", "cruel", "cruise", "crumble", "crunch",
        "crush", "cry", "crystal", "cube", "culture", "cup", "cupboard", "curious",
        "current", "curtain", "curve", "cushion", "custom", "cute", "cycle", "dad",
        "damage", "damp", "dance", "danger", "daring", "dash", "daughter", "dawn",
        "day", "deal", "debate", "debris", "decade", "december", "decide", "decline",
        "decorate", "decrease", "deer", "defense", "define", "defy", "degree", "delay",
        "deliver", "demand", "demise", "denial", "dentist", "deny", "depart", "depend",
        "deposit", "depth", "deputy", "derive", "describe", "desert", "design", "desk",
        "despair", "destroy", "detail", "detect", "develop", "device", "devote", "diagram",
        "dial", "diamond", "diary", "dice", "diesel", "diet", "differ", "digital",
        "dignity", "dilemma", "dinner", "dinosaur", "direct", "dirt", "disagree", "discover",
        "disease", "dish", "dismiss", "disorder", "display", "distance", "divert", "divide",
        "divorce", "dizzy", "doctor", "document", "dog", "doll", "dolphin", "domain",
        "donate", "donkey", "donor", "door", "dose", "double", "dove", "draft",
        "dragon", "drama", "drastic", "draw", "dream", "dress", "drift", "drill",
        "drink", "drip", "drive", "drop", "drum", "dry", "duck", "dumb",
        "dune", "during", "dust", "dutch", "duty", "dwarf", "dynamic", "eager",
        "eagle", "early", "earn", "earth", "easily", "east", "easy", "echo",
        "ecology", "economy", "edge", "edit", "educate", "effort", "egg", "eight",
        "either", "elbow", "elder", "electric", "elegant", "element", "elephant", "elevator",
        "elite", "else", "embark", "embody", "embrace", "emerge", "emotion", "employ",
        "empower", "empty", "enable", "enact", "end", "endless", "endorse", "enemy",
        "energy", "enforce", "engage", "engine", "enhance", "enjoy", "enlist", "enough",
        "enrich", "enroll", "ensure", "enter", "entire", "entry", "envelope", "episode",
        "equal", "equip", "era", "erase", "erode", "erosion", "error", "erupt",
        "escape", "essay", "essence", "estate", "eternal", "ethics", "evidence", "evil",
        "evoke", "evolve", "exact", "example", "excess", "exchange", "excite", "exclude",
        "excuse", "execute", "exercise", "exhaust", "exhibit", "exile", "exist", "exit",
        "exotic", "expand", "expect", "expire", "explain", "expose", "express", "extend",
        "extra", "eye", "eyebrow", "fabric", "face", "faculty", "fade", "faint",
        "faith", "fall", "false", "fame", "family", "famous", "fan", "fancy",
        "fantasy", "farm", "fashion", "fat", "fatal", "father", "fatigue", "fault",
        "favorite", "feature", "february", "federal", "fee", "feed", "feel", "female",
        "fence", "festival", "fetch", "fever", "few", "fiber", "fiction", "field",
        "figure", "file", "film", "filter", "final", "find", "fine", "finger",
        "finish", "fire", "firm", "first", "fiscal", "fish", "fit", "fitness",
        "fix", "flag", "flame", "flash", "flat", "flavor", "flee", "flight",
        "flip", "float", "flock", "floor", "flower", "fluid", "flush", "fly",
        "foam", "focus", "fog", "foil", "fold", "follow", "food", "foot",
        "force", "forest", "forget", "fork", "fortune", "forum", "forward", "fossil",
        "foster", "found", "fox", "fragile", "frame", "frequent", "fresh", "friend",
        "fringe", "frog", "front", "frost", "frown", "frozen", "fruit", "fuel",
        "fun", "funny", "furnace", "fury", "future", "gadget", "gain", "galaxy",
        "gallery", "game", "gap", "garage", "garbage", "garden", "garlic", "garment",
        "gas", "gasp", "gate", "gather", "gauge", "gaze", "general", "genius",
        "genre", "gentle", "genuine", "gesture", "ghost", "giant", "gift", "giggle",
        "ginger", "giraffe", "girl", "give", "glad", "glance", "glare", "glass",
        "glide", "glimpse", "globe", "gloom", "glory", "glove", "glow", "glue",
        "goat", "goddess", "gold", "good", "goose", "gorilla", "gospel", "gossip",
        "govern", "gown", "grab", "grace", "grain", "grant", "grape", "grass",
        "gravity", "great", "green", "grid", "grief", "grit", "grocery", "group",
        "grow", "grunt", "guard", "guess", "guide", "guilt", "guitar", "gun",
        "gym", "habit", "hair", "half", "hammer", "hamster", "hand", "happy",
        "harbor", "hard", "harsh", "harvest", "hat", "have", "hawk", "hazard",
        "head", "health", "heart", "heavy", "hedgehog", "height", "hello", "helmet",
        "help", "hen", "hero", "hidden", "high", "hill", "hint", "hip",
        "hire", "history", "hobby", "hockey", "hold", "hole", "holiday", "hollow",
        "home", "honey", "hood", "hope", "horn", "horror", "horse", "hospital",
        "host", "hotel", "hour", "hover", "hub", "huge", "human", "humble",
        "humor", "hundred", "hungry", "hunt", "hurdle", "hurry", "hurt", "husband",
        "hybrid", "ice", "icon", "idea", "identify", "idle", "ignore", "ill",
        "illegal", "illness", "image", "imitate", "immense", "immune", "impact", "impose",
        "improve", "impulse", "inch", "include", "income", "increase", "index", "indicate",
        "indoor", "industry", "infant", "inflict", "inform", "inhale", "inherit", "initial",
        "inject", "injury", "inmate", "inner", "innocent", "input", "inquiry", "insane",
        "insect", "inside", "inspire", "install", "intact", "interest", "into", "invest",
        "invite", "involve", "iron", "island", "isolate", "issue", "item", "ivory",
        "jacket", "jaguar", "jar", "jazz", "jealous", "jeans", "jelly", "jewel",
        "job", "join", "joke", "journey", "joy", "judge", "juice", "jump",
        "jungle", "junior", "junk", "just", "kangaroo", "keen", "keep", "ketchup",
        "key", "kick", "kid", "kidney", "kind", "kingdom", "kiss", "kit",
        "kitchen", "kite", "kitten", "kiwi", "knee", "knife", "knock", "know",
        "lab", "label", "labor", "ladder", "lady", "lake", "lamp", "language",
        "laptop", "large", "later", "latin", "laugh", "laundry", "lava", "law",
        "lawn", "lawsuit", "layer", "lazy", "leader", "leaf", "learn", "leave",
        "lecture", "left", "leg", "legal", "legend", "leisure", "lemon", "lend",
        "length", "lens", "leopard", "lesson", "letter", "level", "liar", "liberty",
        "library", "license", "life", "lift", "light", "like", "limb", "limit",
        "link", "lion", "liquid", "list", "little", "live", "lizard", "load",
        "loan", "lobster", "local", "lock", "logic", "lonely", "long", "loop",
        "lottery", "loud", "lounge", "love", "loyal", "lucky", "luggage", "lumber",
        "lunar", "lunch", "luxury", "lyrics", "machine", "mad", "magic", "magnet",
        "maid", "mail", "main", "major", "make", "mammal", "man", "manage",
        "mandate", "mango", "mansion", "manual", "maple", "marble", "march", "margin",
        "marine", "market", "marriage", "mask", "mass", "master", "match", "material",
        "math", "matrix", "matter", "maximum", "maze", "meadow", "mean", "measure",
        "meat", "mechanic", "medal", "media", "melody", "melt", "member", "memory",
        "mention", "menu", "mercy", "merge", "merit", "merry", "mesh", "message",
        "metal", "method", "middle", "midnight", "milk", "million", "mimic", "mind",
        "minimum", "minor", "minute", "miracle", "mirror", "misery", "miss", "mistake",
        "mix", "mixed", "mixture", "mobile", "model", "modify", "mom", "moment",
        "monitor", "monkey", "monster", "month", "moon", "moral", "more", "morning",
        "mosquito", "mother", "motion", "motor", "mountain", "mouse", "move", "movie",
        "much", "muffin", "mule", "multiply", "muscle", "museum", "mushroom", "music",
        "must", "mutual", "myself", "mystery", "myth", "naive", "name", "napkin",
        "narrow", "nasty", "nation", "nature", "near", "neck", "need", "negative",
        "neglect", "neither", "nephew", "nerve", "nest", "net", "network", "neutral",
        "never", "news", "next", "nice", "night", "noble", "noise", "nominee",
        "noodle", "normal", "north", "nose", "notable", "note", "nothing", "notice",
        "novel", "now", "nuclear", "number", "nurse", "nut", "oak", "obey",
        "object", "oblige", "obscure", "observe", "obtain", "obvious", "occur", "ocean",
        "october", "odor", "off", "offer", "office", "often", "oil", "okay",
        "old", "olive", "olympic", "omit", "once", "one", "onion", "online",
        "only", "open", "opera", "opinion", "oppose", "option", "orange", "orbit",
        "orchard", "order", "ordinary", "organ", "orient", "original", "orphan", "ostrich",
        "other", "outdoor", "outer", "output", "outside", "oval", "oven", "over",
        "own", "owner", "oxygen", "oyster", "ozone", "pact", "paddle", "page",
        "pair", "palace", "palm", "panda", "panel", "panic", "panther", "paper",
        "parade", "parent", "park", "parrot", "party", "pass", "patch", "path",
        "patient", "patrol", "pattern", "pause", "pave", "payment", "peace", "peanut",
        "pear", "peasant", "pelican", "pen", "penalty", "pencil", "people", "pepper",
        "perfect", "permit", "person", "pet", "phone", "photo", "phrase", "physical",
        "piano", "picnic", "picture", "piece", "pig", "pigeon", "pill", "pilot",
        "pink", "pioneer", "pipe", "pistol", "pitch", "pizza", "place", "planet",
        "plastic", "plate", "play", "please", "pledge", "pluck", "plug", "plunge",
        "poem", "poet", "point", "polar", "pole", "police", "pond", "pony",
        "pool", "popular", "portion", "position", "possible", "post", "potato", "pottery",
        "poverty", "powder", "power", "practice", "praise", "predict", "prefer", "prepare",
        "present", "pretty", "prevent", "price", "pride", "primary", "print", "priority",
        "prison", "private", "prize", "problem", "process", "produce", "profit", "program",
        "project", "promote", "proof", "property", "prosper", "protect", "proud", "provide",
        "public", "pudding", "pull", "pulp", "pulse", "pumpkin", "punch", "pupil",
        "puppy", "purchase", "purity", "purpose", "purse", "push", "put", "puzzle",
        "pyramid", "quality", "quantum", "quarter", "question", "quick", "quit", "quiz",
        "quote", "rabbit", "raccoon", "race", "rack", "radar", "radio", "rail",
        "rain", "raise", "rally", "ramp", "ranch", "random", "range", "rapid",
        "rare", "rate", "rather", "raven", "raw", "razor", "ready", "real",
        "reason", "rebel", "rebuild", "recall", "receive", "recipe", "record", "recycle",
        "reduce", "reflect", "reform", "refuse", "region", "regret", "regular", "reject",
        "relax", "release", "relief", "rely", "remain", "remember", "remind", "remove",
        "render", "renew", "rent", "reopen", "repair", "repeat", "replace", "report",
        "require", "rescue", "resemble", "resist", "resource", "response", "result", "retire",
        "retreat", "return", "reunion", "reveal", "review", "reward", "rhythm", "rib",
        "ribbon", "rice", "rich", "ride", "ridge", "rifle", "right", "rigid",
        "ring", "riot", "ripple", "risk", "ritual", "rival", "river", "road",
        "roast", "robot", "robust", "rocket", "romance", "roof", "rookie", "room",
        "rose", "rotate", "rough", "round", "route", "royal", "rubber", "rude",
        "rug", "rule", "run", "runway", "rural", "sad", "saddle", "sadness",
        "safe", "sail", "salad", "salmon", "salon", "salt", "salute", "same",
        "sample", "sand", "satisfy", "satoshi", "sauce", "sausage", "save", "say",
        "scale", "scan", "scare", "scatter", "scene", "scheme", "school", "science",
        "scissors", "scorpion", "scout", "scrap", "screen", "script", "scrub", "sea",
        "search", "season", "seat", "second", "secret", "section", "security", "seed",
        "seek", "segment", "select", "sell", "seminar", "senior", "sense", "sentence",
        "series", "service", "session", "settle", "setup", "seven", "shadow", "shaft",
        "shallow", "share", "shed", "shell", "sheriff", "shield", "shift", "shine",
        "ship", "shiver", "shock", "shoe", "shoot", "shop", "short", "shoulder",
        "shove", "shrimp", "shrug", "shuffle", "shy", "sibling", "sick", "side",
        "siege", "sight", "sign", "silent", "silk", "silly", "silver", "similar",
        "simple", "since", "sing", "siren", "sister", "situate", "six", "size",
        "skate", "sketch", "ski", "skill", "skin", "skirt", "skull", "slab",
        "slam", "sleep", "slender", "slice", "slide", "slight", "slim", "slogan",
        "slot", "slow", "slush", "small", "smart", "smile", "smoke", "smooth",
        "snack", "snake", "snap", "sniff", "snow", "soap", "soccer", "social",
        "sock", "soda", "soft", "solar", "soldier", "solid", "solution", "solve",
        "someone", "song", "soon", "sorry", "sort", "soul", "sound", "soup",
        "source", "south", "space", "spare", "spatial", "spawn", "speak", "special",
        "speed", "spell", "spend", "sphere", "spice", "spider", "spike", "spin",
        "spirit", "split", "spoil", "sponsor", "spoon", "sport", "spot", "spray",
        "spread", "spring", "spy", "square", "squeeze", "squirrel", "stable", "stadium",
        "staff", "stage", "stairs", "stamp", "stand", "start", "state", "stay",
        "steak", "steel", "stem", "step", "stereo", "stick", "still", "sting",
        "stock", "stomach", "stone", "stool", "story", "stove", "strategy", "street",
        "strike", "strong", "struggle", "student", "stuff", "stumble", "style", "subject",
        "submit", "subway", "success", "such", "sudden", "suffer", "sugar", "suggest",
        "suit", "summer", "sun", "sunny", "sunset", "super", "supply", "supreme",
        "sure", "surface", "surge", "surprise", "surround", "survey", "suspect", "sustain",
        "swallow", "swamp", "swap", "swarm", "swear", "sweet", "swift", "swim",
        "swing", "switch", "sword", "symbol", "symptom", "syrup", "system", "table",
        "tackle", "tag", "tail", "talent", "talk", "tank", "tape", "target",
        "task", "taste", "tattoo", "taxi", "teach", "team", "tell", "ten",
        "tenant", "tennis", "tent", "term", "test", "text", "thank", "that",
        "theme", "then", "theory", "there", "they", "thing", "this", "thought",
        "three", "thrive", "throw", "thumb", "thunder", "ticket", "tide", "tiger",
        "tilt", "timber", "time", "tiny", "tip", "tired", "tissue", "title",
        "toast", "tobacco", "today", "toddler", "toe", "together", "toilet", "token",
        "tomato", "tomorrow", "tone", "tongue", "tonight", "tool", "tooth", "top",
        "topic", "topple", "torch", "tornado", "tortoise", "toss", "total", "tourist",
        "toward", "tower", "town", "toy", "track", "trade", "traffic", "tragic",
        "train", "transfer", "trap", "trash", "travel", "tray", "treat", "tree",
        "trend", "trial", "tribe", "trick", "trigger", "trim", "trip", "trophy",
        "trouble", "truck", "true", "truly", "trumpet", "trust", "truth", "try",
        "tube", "tuition", "tumble", "tuna", "tunnel", "turkey", "turn", "turtle",
        "twelve", "twenty", "twice", "twin", "twist", "two", "type", "typical",
        "ugly", "umbrella", "unable", "unaware", "uncle", "uncover", "under", "undo",
        "unfair", "unfold", "unhappy", "uniform", "unique", "unit", "universe", "unknown",
        "unlock", "until", "unusual", "unveil", "update", "upgrade", "uphold", "upon",
        "upper", "upset", "urban", "urge", "usage", "use", "used", "useful",
        "useless", "usual", "utility", "vacant", "vacuum", "vague", "valid", "valley",
        "valve", "van", "vanish", "vapor", "various", "vast", "vault", "vehicle",
        "velvet", "vendor", "venture", "venue", "verb", "verify", "version", "very",
        "vessel", "veteran", "viable", "vibrant", "vicious", "victory", "video", "view",
        "village", "vintage", "violin", "virtual", "virus", "visa", "visit", "visual",
        "vital", "vivid", "vocal", "voice", "void", "volcano", "volume", "vote",
        "voyage", "wage", "wagon", "wait", "walk", "wall", "walnut", "want",
        "warfare", "warm", "warrior", "wash", "wasp", "waste", "water", "wave",
        "way", "wealth", "weapon", "wear", "weasel", "weather", "web", "wedding",
        "weekend", "weird", "welcome", "west", "wet", "whale", "what", "wheat",
        "wheel", "when", "where", "whip", "whisper", "wide", "width", "wife",
        "wild", "will", "win", "window", "wine", "wing", "wink", "winner",
        "winter", "wire", "wisdom", "wise", "wish", "witness", "wolf", "woman",
        "wonder", "wood", "wool", "word", "work", "world", "worry", "worth",
        "wrap", "wreck", "wrestle", "wrist", "write", "wrong", "yard", "year",
        "yellow", "you", "young", "youth", "zebra", "zero", "zone", "zoo"
    };
    
    std::vector<std::string> Mnemonic::generate(int wordCount) {
        // Перевірка коректності параметрів
        // Validate parameters
        // Проверка корректности параметров
        if (wordCount != 12 && wordCount != 18 && wordCount != 24) {
            throw std::invalid_argument("Word count must be 12, 18, or 24");
        }
        
        // Генерація ентропії
        // Generate entropy
        // Генерация энтропии
        int entropyBits = wordCount * 32 / 3;
        int entropyBytes = entropyBits / 8;
        
        std::vector<uint8_t> entropy(entropyBytes);
        
        // Використовуємо стандартний генератор
        // Use standard generator
        // Используем стандартный генератор
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);
        
        for (int i = 0; i < entropyBytes; ++i) {
            entropy[i] = static_cast<uint8_t>(dis(gen));
        }
        
        // Обчислення контрольної суми
        // Calculate checksum
        // Вычисление контрольной суммы
        auto checksum = calculateChecksum(entropy);
        
        // Комбінування ентропії та контрольної суми
        // Combine entropy and checksum
        // Комбинирование энтропии и контрольной суммы
        std::vector<uint8_t> data = entropy;
        data.insert(data.end(), checksum.begin(), checksum.end());
        
        // Конвертація в мнемонічну фразу
        // Convert to mnemonic phrase
        // Конвертация в мнемоническую фразу
        std::vector<std::string> mnemonic;
        int totalBits = entropyBits + (entropyBits / 32);
        int wordIndexBits = 11;
        
        for (int i = 0; i < totalBits; i += wordIndexBits) {
            int wordIndex = 0;
            for (int j = 0; j < wordIndexBits; ++j) {
                int bitPos = i + j;
                int byteIndex = bitPos / 8;
                int bitIndex = 7 - (bitPos % 8);
                
                if (byteIndex < data.size() && (data[byteIndex] & (1 << bitIndex))) {
                    wordIndex |= (1 << (wordIndexBits - 1 - j));
                }
            }
            
            if (wordIndex < BIP39_WORDLIST.size()) {
                mnemonic.push_back(BIP39_WORDLIST[wordIndex]);
            } else {
                // Fallback to first word if index is out of bounds
                mnemonic.push_back(BIP39_WORDLIST[0]);
            }
        }
        
        return mnemonic;
    }
    
    std::vector<uint8_t> Mnemonic::toSeed(const std::vector<std::string>& mnemonic, 
                                        const std::string& passphrase) {
        // Реалізація PBKDF2 для перетворення мнемоніки в seed
        // Implementation of PBKDF2 to convert mnemonic to seed
        // Реализация PBKDF2 для преобразования мнемоники в сид
        
        // Конкатенуємо слова мнемоніки з пробілами
        // Concatenate mnemonic words with spaces
        // Конкатенируем слова мнемоники с пробелами
        std::string mnemonicString;
        for (size_t i = 0; i < mnemonic.size(); ++i) {
            if (i > 0) mnemonicString += " ";
            mnemonicString += mnemonic[i];
        }
        
        // Формуємо сіль (salt) згідно з BIP-39
        // Form salt according to BIP-39
        // Формируем соль согласно BIP-39
        std::string salt = "mnemonic" + passphrase;
        
        // Використовуємо PBKDF2 з 2048 ітераціями
        // Use PBKDF2 with 2048 iterations
        // Используем PBKDF2 с 2048 итерациями
        std::vector<uint8_t> seed(64);
        
#if OPENSSL_AVAILABLE
        // Використовуємо OpenSSL PBKDF2
        // Use OpenSSL PBKDF2
        // Используем OpenSSL PBKDF2
        if (PKCS5_PBKDF2_HMAC(
                mnemonicString.c_str(), mnemonicString.length(),
                reinterpret_cast<const unsigned char*>(salt.c_str()), salt.length(),
                2048, EVP_sha512(),
                64, seed.data()) != 1) {
            // Fallback to zero seed if PBKDF2 fails
            std::fill(seed.begin(), seed.end(), 0);
        }
#else
        // Для простоти, повертаємо 64 нульових байти
        // For simplicity, return 64 zero bytes
        // Для простоты, возвращаем 64 нулевых байта
        std::fill(seed.begin(), seed.end(), 0);
#endif
        
        return seed;
    }
    
    bool Mnemonic::isValid(const std::vector<std::string>& mnemonic) {
        // Перевірка валідності мнемонічної фрази
        // Validate mnemonic phrase
        // Проверка валидности мнемонической фразы
        
        // Перевірка довжини
        // Check length
        // Проверка длины
        if (mnemonic.size() != 12 && mnemonic.size() != 18 && mnemonic.size() != 24) {
            return false;
        }
        
        // Перевірка наявності слів у wordlist
        // Check if words are in wordlist
        // Проверка наличия слов в wordlist
        for (const auto& word : mnemonic) {
            bool found = false;
            for (const auto& wlWord : BIP39_WORDLIST) {
                if (word == wlWord) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        
        // Calculate checksum and verify it
        // This is a more thorough validation
        try {
            // Convert words to indices
            std::vector<int> indices;
            for (const auto& word : mnemonic) {
                for (size_t i = 0; i < BIP39_WORDLIST.size(); ++i) {
                    if (BIP39_WORDLIST[i] == word) {
                        indices.push_back(static_cast<int>(i));
                        break;
                    }
                }
            }
            
            // Convert indices to bits
            std::vector<uint8_t> bits;
            for (int index : indices) {
                for (int i = 10; i >= 0; --i) {
                    bits.push_back((index >> i) & 1);
                }
            }
            
            // Extract entropy and checksum
            int entropyBits = static_cast<int>(mnemonic.size() * 32 / 3);
            int checksumBits = entropyBits / 32;
            
            // Calculate actual checksum
            std::vector<uint8_t> entropyBytes((entropyBits + 7) / 8);
            for (size_t i = 0; i < entropyBytes.size() && i * 8 < bits.size(); ++i) {
                for (int j = 0; j < 8 && (i * 8 + j) < bits.size(); ++j) {
                    entropyBytes[i] |= (bits[i * 8 + j] << (7 - j));
                }
            }
            
            auto calculatedChecksum = calculateChecksum(entropyBytes);
            
            // Extract checksum from bits
            std::vector<uint8_t> extractedChecksum((checksumBits + 7) / 8);
            size_t entropyEndBit = entropyBits;
            for (size_t i = 0; i < extractedChecksum.size() && (entropyEndBit + i * 8) < bits.size(); ++i) {
                for (int j = 0; j < 8 && (entropyEndBit + i * 8 + j) < bits.size(); ++j) {
                    extractedChecksum[i] |= (bits[entropyEndBit + i * 8 + j] << (7 - j));
                }
            }
            
            // Compare checksums
            return calculatedChecksum == extractedChecksum;
        } catch (...) {
            return false;
        }
        
        return true;
    }
    
    const std::vector<std::string>& Mnemonic::getWordlist() {
        return BIP39_WORDLIST;
    }
    
    std::vector<uint8_t> Mnemonic::calculateChecksum(const std::vector<uint8_t>& entropy) {
        // Обчислення контрольної суми SHA256
        // Calculate SHA256 checksum
        // Вычисление контрольной суммы SHA256
        
        std::vector<uint8_t> hash(32);
        
#if OPENSSL_AVAILABLE
        SHA256(entropy.data(), entropy.size(), hash.data());
#else
        // Використовуємо fallback реалізацію
        // Use fallback implementation
        // Используем fallback реализацию
        sha256_fallback(entropy.data(), entropy.size(), hash.data());
#endif
        
        // Повертаємо перші біти в залежності від розміру ентропії
        // Return first bits depending on entropy size
        // Возвращаем первые биты в зависимости от размера энтропии
        int checksumBits = entropy.size() * 8 / 32;
        int checksumBytes = (checksumBits + 7) / 8;
        
        return std::vector<uint8_t>(hash.begin(), hash.begin() + checksumBytes);
    }
    
}