// THIS IS THE PROVIDED CODE FOR PROGRAM #3, DSA 1, FALL 2023

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// A simple class; each object holds four public fields
class Data {
 public:
  string lastName;
  string firstName;
  string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {
  ifstream input(filename);
  if (!input) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // The first line indicates the size
  string line;
  getline(input, line);
  stringstream ss(line);
  int size;
  ss >> size;

  // Load the data
  for (int i = 0; i < size; i++) {
    getline(input, line);
    stringstream ss2(line);
    Data *pData = new Data();
    ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
    l.push_back(pData);
  }

  input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {
  ofstream output(filename);
  if (!output) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // Write the size first
  int size = l.size();
  output << size << "\n";

  // Write the data
  for (auto pData : l) {
    output << pData->lastName << " " << pData->firstName << " " << pData->ssn
           << "\n";
  }

  output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
  string filename;
  cout << "Enter name of input file: ";
  cin >> filename;
  list<Data *> theList;
  loadDataList(theList, filename);

  cout << "Data loaded.\n";

  cout << "Executing sort...\n";
  clock_t t1 = clock();
  sortDataList(theList);
  clock_t t2 = clock();
  double timeDiff = ((double)(t2 - t1)) / CLOCKS_PER_SEC;

  cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

  cout << "Enter name of output file: ";
  cin >> filename;
  writeDataList(theList, filename);

  return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------
#include <functional>
#include <iterator>
#include <unordered_map>

//_________________________________________________________________________________________________________________________________________________________________________
// MNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNM
//*************************************************************************************************************************************************************************
// T3
auto compareData(const Data *p1, const Data *p2) {
  // Compare last names
  int lastNameComparison = p1->lastName.compare(p2->lastName);
  if (lastNameComparison != 0) {
    return lastNameComparison < 0;
  }

  // Compare first names if last names are the same
  int firstNameComparison = p1->firstName.compare(p2->firstName);
  if (firstNameComparison != 0) {
    return firstNameComparison < 0;
  }

  // Compare SSNs if both last and first names are the same
  return p1->ssn < p2->ssn;
};

void insertionSort(list<Data *> &l) {
  auto it = l.begin();
  ++it;

  while (it != l.end()) {
    auto curr = *it;
    auto prevIt = prev(it);

    while (prevIt != l.begin() && compareData(curr, *prevIt)) {
      swap(*prevIt, *it);
      it = prevIt;
      prevIt = prev(prevIt);
    }

    if (prevIt == l.begin() && compareData(curr, *prevIt)) {
      swap(*prevIt, *it);
      it = prevIt;
    }

    ++it;
  }
}

//_________________________________________________________________________________________________________________________________________________________________________
// MNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNM
//*************************************************************************************************************************************************************************
// T4

std::string socialSecurityNumber;
int binIndexFirstPass;
int binIndexSecondPass;
std::list<Data *> firstPassBins[10000];
std::list<Data *> secondPassBins[100000];
void radixSort(std::list<Data *> &dataList) {
    // Assuming 'firstPassBins' and 'secondPassBins' are defined elsewhere
    std::string socialSecurityNumber;
    int binIndexFirstPass, binIndexSecondPass;

    // First pass: Distribute elements into bins based on the last four digits
    for (auto data : dataList) {
        socialSecurityNumber = data->ssn;
        binIndexFirstPass = (socialSecurityNumber[7] - '0') * 1000 +
                            (socialSecurityNumber[8] - '0') * 100 +
                            (socialSecurityNumber[9] - '0') * 10 +
                            (socialSecurityNumber[10] - '0');
        firstPassBins[binIndexFirstPass].push_back(data);
    }

    // Second pass: Distribute elements into bins based on the first three and fifth-sixth digits
    for (auto &bin : firstPassBins) {
        for (auto data : bin) {
            socialSecurityNumber = data->ssn;
            binIndexSecondPass = (socialSecurityNumber[0] - '0') * 10000 +
                                 (socialSecurityNumber[1] - '0') * 1000 +
                                 (socialSecurityNumber[2] - '0') * 100 +
                                 (socialSecurityNumber[4] - '0') * 10 +
                                 (socialSecurityNumber[5] - '0');
            secondPassBins[binIndexSecondPass].push_back(data);
        }
    }

    // Clear the original list and rearrange elements according to the sorted bins
    dataList.clear();
    for (auto &bin : secondPassBins) {
        if (!bin.empty()) {
            dataList.splice(dataList.end(), bin);
        }
    }
}


//_________________________________________________________________________________________________________________________________________________________________________
// MNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNM
//*************************************************************************************************************************************************************************

// T1&T2
// Manually input possible first and last names into hash maps
/* unordered_map<string, unsigned int> firstNamesMap = {
    {"AALIYAH", 0},     {"AARON", 1},       {"ABEL", 2},        {"ABIGAIL", 3},     {"ABRAHAM", 4},
    {"ADALINE", 5},     {"ADALYN", 6},      {"ADALYNN", 7},     {"ADAM", 8},        {"ADDISON", 9},
    {"ADELINE", 10},    {"ADELYN", 11},     {"ADRIAN", 12},     {"ADRIANA", 13},    {"AIDAN", 14},
    {"AIDEN", 15},      {"ALAINA", 16},     {"ALAN", 17},       {"ALANA", 18},      {"ALAYNA", 19},
    {"ALEJANDRO", 20},  {"ALEX", 21},       {"ALEXA", 22},      {"ALEXANDER", 23},  {"ALEXANDRA", 24},
    {"ALEXIS", 25},     {"ALICE", 26},      {"ALINA", 27},      {"ALIVIA", 28},     {"ALIYAH", 29},
    {"ALLISON", 30},    {"ALYSSA", 31},     {"AMARA", 32},      {"AMAYA", 33},      {"AMELIA", 34},
    {"AMIR", 35},       {"AMY", 36},        {"ANA", 37},        {"ANASTASIA", 38},  {"ANDREA", 39},
    {"ANDRES", 40},     {"ANDREW", 41},     {"ANGEL", 42},      {"ANGELA", 43},     {"ANGELINA", 44},
    {"ANNA", 45},       {"ANNABELLE", 46},  {"ANTHONY", 47},    {"ANTONIO", 48},    {"ARABELLA", 49},
    {"ARIA", 50},       {"ARIANA", 51},     {"ARIANNA", 52},    {"ARIEL", 53},      {"ARTHUR", 54},
    {"ARYA", 55},       {"ASHER", 56},      {"ASHLEY", 57},     {"ASHTON", 58},     {"ATHENA", 59},
    {"AUBREE", 60},     {"AUBREY", 61},     {"AUDREY", 62},     {"AUGUST", 63},     {"AURORA", 64},
    {"AUSTIN", 65},     {"AUTUMN", 66},     {"AVA", 67},        {"AVERY", 68},      {"AXEL", 69},
    {"AYDEN", 70},      {"AYLA", 71},       {"BAILEY", 72},     {"BARRETT", 73},    {"BEAU", 74},
    {"BECKETT", 75},    {"BELLA", 76},      {"BENJAMIN", 77},   {"BENNETT", 78},    {"BENTLEY", 79},
    {"BLAKE", 80},      {"BRADLEY", 81},    {"BRADY", 82},      {"BRANDON", 83},    {"BRANTLEY", 84},
    {"BRAXTON", 85},    {"BRAYDEN", 86},    {"BRIAN", 87},      {"BRIANNA", 88},    {"BRIELLE", 89},
    {"BRODY", 90},      {"BROOKE", 91},     {"BROOKLYN", 92},   {"BROOKLYNN", 93},  {"BROOKS", 94},
    {"BRYAN", 95},      {"BRYCE", 96},      {"BRYNLEE", 97},    {"BRYSON", 98},     {"CADEN", 99},
    {"CALEB", 100},     {"CALLIE", 101},    {"CALVIN", 102},    {"CAMDEN", 103},    {"CAMERON", 104},
    {"CAMILA", 105},    {"CARLOS", 106},    {"CAROLINE", 107},  {"CARSON", 108},    {"CARTER", 109},
    {"CATHERINE", 110}, {"CAYDEN", 111},    {"CECILIA", 112},   {"CHARLES", 113},   {"CHARLIE", 114},
    {"CHARLOTTE", 115}, {"CHASE", 116},     {"CHLOE", 117},     {"CHRISTIAN", 118}, {"CHRISTOPHER", 119},
    {"CLAIRE", 120},    {"CLARA", 121},     {"CLAYTON", 122},   {"COLE", 123},      {"COLIN", 124},
    {"COLTON", 125},    {"CONNOR", 126},    {"COOPER", 127},    {"CORA", 128},      {"DAISY", 129},
    {"DAKOTA", 130},    {"DALEYZA", 131},   {"DAMIAN", 132},    {"DANIEL", 133},    {"DANIELA", 134},
    {"DAVID", 135},     {"DAWSON", 136},    {"DEAN", 137},      {"DECLAN", 138},    {"DELANEY", 139},
    {"DELILAH", 140},   {"DEREK", 141},     {"DESTINY", 142},   {"DIANA", 143},     {"DIEGO", 144},
    {"DOMINIC", 145},   {"DYLAN", 146},     {"EASTON", 147},    {"EDEN", 148},      {"EDWARD", 149},
    {"ELEANOR", 150},   {"ELENA", 151},     {"ELI", 152},       {"ELIANA", 153},    {"ELIAS", 154},
    {"ELIJAH", 155},    {"ELISE", 156},     {"ELIZA", 157},     {"ELIZABETH", 158}, {"ELLA", 159},
    {"ELLIANA", 160},   {"ELLIE", 161},     {"ELLIOT", 162},    {"ELLIOTT", 163},   {"ELOISE", 164},
    {"EMERSON", 165},   {"EMERSYN", 166},   {"EMERY", 167},     {"EMILIA", 168},    {"EMILIANO", 169},
    {"EMILY", 170},     {"EMMA", 171},      {"EMMANUEL", 172},  {"EMMETT", 173},    {"ERIC", 174},
    {"ESTHER", 175},    {"ETHAN", 176},     {"EVA", 177},       {"EVAN", 178},      {"EVELYN", 179},
    {"EVERETT", 180},   {"EVERLY", 181},    {"EZEKIEL", 182},   {"EZRA", 183},      {"FAITH", 184},
    {"FELIX", 185},     {"FINLEY", 186},    {"FINN", 187},      {"FIONA", 188},     {"GABRIEL", 189},
    {"GABRIELLA", 190}, {"GAEL", 191},      {"GAVIN", 192},     {"GENESIS", 193},   {"GENEVIEVE", 194},
    {"GEORGE", 195},    {"GEORGIA", 196},   {"GIANNA", 197},    {"GIOVANNI", 198},  {"GRACE", 199},
    {"GRACIE", 200},    {"GRAHAM", 201},    {"GRANT", 202},     {"GRAYSON", 203},   {"GREYSON", 204},
    {"GRIFFIN", 205},   {"HADLEY", 206},    {"HAILEY", 207},    {"HANNAH", 208},    {"HARLEY", 209},
    {"HARMONY", 210},   {"HARPER", 211},    {"HARRISON", 212},  {"HAYDEN", 213},    {"HAZEL", 214},
    {"HENRY", 215},     {"HOLDEN", 216},    {"HUDSON", 217},    {"HUNTER", 218},    {"IAN", 219},
    {"IRIS", 220},      {"ISAAC", 221},     {"ISABEL", 222},    {"ISABELLA", 223},  {"ISABELLE", 224},
    {"ISAIAH", 225},    {"ISLA", 226},      {"ISRAEL", 227},    {"IVAN", 228},      {"IVY", 229},
    {"JACE", 230},      {"JACK", 231},      {"JACKSON", 232},   {"JACOB", 233},     {"JADE", 234},
    {"JADEN", 235},     {"JAKE", 236},      {"JAMES", 237},     {"JAMESON", 238},   {"JASMINE", 239},
    {"JASON", 240},     {"JASPER", 241},    {"JAVIER", 242},    {"JAX", 243},       {"JAXON", 244},
    {"JAXSON", 245},    {"JAYCE", 246},     {"JAYDEN", 247},    {"JAYLA", 248},     {"JEREMIAH", 249},
    {"JEREMY", 250},    {"JESSE", 251},     {"JESSICA", 252},   {"JESUS", 253},     {"JOANNA", 254},
    {"JOCELYN", 255},   {"JOEL", 256},      {"JOHN", 257},      {"JONAH", 258},     {"JONATHAN", 259},
    {"JORDAN", 260},    {"JORDYN", 261},    {"JORGE", 262},     {"JOSE", 263},      {"JOSEPH", 264},
    {"JOSEPHINE", 265}, {"JOSHUA", 266},    {"JOSIAH", 267},    {"JOSIE", 268},     {"JOSUE", 269},
    {"JUAN", 270},      {"JUDAH", 271},     {"JUDE", 272},      {"JULIA", 273},     {"JULIAN", 274},
    {"JULIANA", 275},   {"JULIANNA", 276},  {"JULIET", 277},    {"JULIETTE", 278},  {"JUNE", 279},
    {"JUSTIN", 280},    {"KADEN", 281},     {"KAI", 282},       {"KAIDEN", 283},    {"KALEB", 284},
    {"KARTER", 285},    {"KATHERINE", 286}, {"KAYDEN", 287},    {"KAYLA", 288},     {"KAYLEE", 289},
    {"KENDALL", 290},   {"KENNEDY", 291},   {"KENNETH", 292},   {"KEVIN", 293},     {"KHLOE", 294},
    {"KILLIAN", 295},   {"KIMBERLY", 296},  {"KING", 297},      {"KINGSTON", 298},  {"KINSLEY", 299},
    {"KNOX", 300},      {"KYLE", 301},      {"KYLIE", 302},     {"KYRIE", 303},     {"LAILA", 304},
    {"LANDON", 305},    {"LAUREN", 306},    {"LAYLA", 307},     {"LEAH", 308},      {"LEILA", 309},
    {"LEILANI", 310},   {"LEO", 311},       {"LEON", 312},      {"LEONARDO", 313},  {"LEVI", 314},
    {"LIAM", 315},      {"LILA", 316},      {"LILIANA", 317},   {"LILLIAN", 318},   {"LILLY", 319},
    {"LILY", 320},      {"LINCOLN", 321},   {"LOGAN", 322},     {"LOLA", 323},      {"LONDON", 324},
    {"LONDYN", 325},    {"LORENZO", 326},   {"LUCA", 327},      {"LUCAS", 328},     {"LUCIA", 329},
    {"LUCY", 330},      {"LUIS", 331},      {"LUKAS", 332},     {"LUKE", 333},      {"LUNA", 334},
    {"LYDIA", 335},     {"LYLA", 336},      {"MACKENZIE", 337}, {"MADDOX", 338},    {"MADELINE", 339},
    {"MADELYN", 340},   {"MADISON", 341},   {"MAGGIE", 342},    {"MAKAYLA", 343},   {"MALACHI", 344},
    {"MALIA", 345},     {"MARCUS", 346},    {"MARGARET", 347},  {"MARIA", 348},     {"MARIAH", 349},
    {"MARK", 350},      {"MARLEY", 351},    {"MARY", 352},      {"MASON", 353},     {"MATEO", 354},
    {"MATIAS", 355},    {"MATTEO", 356},    {"MATTHEW", 357},   {"MAVERICK", 358},  {"MAX", 359},
    {"MAXIMUS", 360},   {"MAXWELL", 361},   {"MAYA", 362},      {"MCKENZIE", 363},  {"MELANIE", 364},
    {"MELODY", 365},    {"MESSIAH", 366},   {"MIA", 367},       {"MICAH", 368},     {"MICHAEL", 369},
    {"MICHELLE", 370},  {"MIGUEL", 371},    {"MILA", 372},      {"MILES", 373},     {"MILO", 374},
    {"MOLLY", 375},     {"MORGAN", 376},    {"MYA", 377},       {"MYLES", 378},     {"NAOMI", 379},
    {"NATALIA", 380},   {"NATALIE", 381},   {"NATHAN", 382},    {"NATHANIEL", 383}, {"NEVAEH", 384},
    {"NICHOLAS", 385},  {"NICOLAS", 386},   {"NICOLE", 387},    {"NOAH", 388},      {"NOELLE", 389},
    {"NOLAN", 390},     {"NORA", 391},      {"NORAH", 392},     {"NOVA", 393},      {"OLIVER", 394},
    {"OLIVIA", 395},    {"OMAR", 396},      {"OSCAR", 397},     {"OWEN", 398},      {"PAIGE", 399},
    {"PAISLEY", 400},   {"PARKER", 401},    {"PATRICK", 402},   {"PAUL", 403},      {"PAXTON", 404},
    {"PAYTON", 405},    {"PENELOPE", 406},  {"PETER", 407},     {"PEYTON", 408},    {"PIPER", 409},
    {"PRESLEY", 410},   {"PRESTON", 411},   {"QUINN", 412},     {"RACHEL", 413},    {"RAELYNN", 414},
    {"REAGAN", 415},    {"REBECCA", 416},   {"REESE", 417},     {"REMI", 418},      {"REMINGTON", 419},
    {"RHETT", 420},     {"RICHARD", 421},   {"RILEY", 422},     {"RIVER", 423},     {"ROBERT", 424},
    {"ROMAN", 425},     {"ROSALIE", 426},   {"ROSE", 427},      {"ROWAN", 428},     {"RUBY", 429},
    {"RYAN", 430},      {"RYDER", 431},     {"RYKER", 432},     {"RYLEE", 433},     {"RYLEIGH", 434},
    {"SADIE", 435},     {"SAMANTHA", 436},  {"SAMUEL", 437},    {"SANTIAGO", 438},  {"SARA", 439},
    {"SARAH", 440},     {"SAVANNAH", 441},  {"SAWYER", 442},    {"SCARLETT", 443},  {"SEBASTIAN", 444},
    {"SELENA", 445},    {"SERENITY", 446},  {"SIENNA", 447},    {"SILAS", 448},     {"SKYLAR", 449},
    {"SLOANE", 450},    {"SOFIA", 451},     {"SOPHIA", 452},    {"SOPHIE", 453},    {"STELLA", 454},
    {"STEVEN", 455},    {"SUMMER", 456},    {"SYDNEY", 457},    {"TAYLOR", 458},    {"TEAGAN", 459},
    {"TESSA", 460},     {"THEODORE", 461},  {"THIAGO", 462},    {"THOMAS", 463},    {"TIMOTHY", 464},
    {"TRINITY", 465},   {"TRISTAN", 466},   {"TUCKER", 467},    {"TYLER", 468},     {"VALENTINA", 469},
    {"VALERIA", 470},   {"VALERIE", 471},   {"VANESSA", 472},   {"VICTOR", 473},    {"VICTORIA", 474},
    {"VINCENT", 475},   {"VIOLET", 476},    {"VIVIAN", 477},    {"WAYLON", 478},    {"WESLEY", 479},
    {"WESTON", 480},    {"WILLIAM", 481},   {"WILLOW", 482},    {"WYATT", 483},     {"XANDER", 484},
    {"XAVIER", 485},    {"XIMENA", 486},    {"ZACHARY", 487},   {"ZANDER", 488},    {"ZANE", 489},
    {"ZAYDEN", 490},    {"ZION", 491},      {"ZOE", 492},       {"ZOEY", 493}};
unordered_map<string, unsigned int> lastNamesMap = {
    {"ACOSTA", 0},       {"ADAMS", 1},       {"ADKINS", 2},       {"AGUILAR", 3},      {"AGUIRRE", 4},
    {"ALEXANDER", 5},    {"ALLEN", 6},       {"ALVARADO", 7},     {"ALVAREZ", 8},      {"ANDERSON", 9},
    {"ANDREWS", 10},     {"ARMSTRONG", 11},  {"ARNOLD", 12},      {"AUSTIN", 13},      {"AVILA", 14},
    {"AYALA", 15},       {"BAILEY", 16},     {"BAKER", 17},       {"BALDWIN", 18},     {"BANKS", 19},
    {"BARBER", 20},      {"BARKER", 21},     {"BARNES", 22},      {"BARNETT", 23},     {"BARRETT", 24},
    {"BARTON", 25},      {"BATES", 26},      {"BECK", 27},        {"BECKER", 28},      {"BELL", 29},
    {"BENNETT", 30},     {"BENSON", 31},     {"BERRY", 32},       {"BISHOP", 33},      {"BLACK", 34},
    {"BLAIR", 35},       {"BLAKE", 36},      {"BOWEN", 37},       {"BOWMAN", 38},      {"BOYD", 39},
    {"BRADLEY", 40},     {"BRADY", 41},      {"BREWER", 42},      {"BROOKS", 43},      {"BROWN", 44},
    {"BRYANT", 45},      {"BURGESS", 46},    {"BURKE", 47},       {"BURNS", 48},       {"BURTON", 49},
    {"BUSH", 50},        {"BUTLER", 51},     {"BYRD", 52},        {"CABRERA", 53},     {"CALDERON", 54},
    {"CALDWELL", 55},    {"CAMACHO", 56},    {"CAMPBELL", 57},    {"CAMPOS", 58},      {"CANNON", 59},
    {"CARDENAS", 60},    {"CARLSON", 61},    {"CARPENTER", 62},   {"CARR", 63},        {"CARRILLO", 64},
    {"CARROLL", 65},     {"CARTER", 66},     {"CASTANEDA", 67},   {"CASTILLO", 68},    {"CASTRO", 69},
    {"CERVANTES", 70},   {"CHAMBERS", 71},   {"CHAN", 72},        {"CHANDLER", 73},    {"CHANG", 74},
    {"CHAPMAN", 75},     {"CHAVEZ", 76},     {"CHEN", 77},        {"CHRISTENSEN", 78}, {"CLARK", 79},
    {"CLARKE", 80},      {"COHEN", 81},      {"COLE", 82},        {"COLEMAN", 83},     {"COLLINS", 84},
    {"COLON", 85},       {"CONTRERAS", 86},  {"COOK", 87},        {"COOPER", 88},      {"CORTEZ", 89},
    {"COX", 90},         {"CRAIG", 91},      {"CRAWFORD", 92},    {"CROSS", 93},       {"CRUZ", 94},
    {"CUMMINGS", 95},    {"CUNNINGHAM", 96}, {"CURRY", 97},       {"CURTIS", 98},      {"DANIEL", 99},
    {"DANIELS", 100},    {"DAVIDSON", 101},  {"DAVIS", 102},      {"DAWSON", 103},     {"DAY", 104},
    {"DEAN", 105},       {"DELACRUZ", 106},  {"DELEON", 107},     {"DELGADO", 108},    {"DENNIS", 109},
    {"DIAZ", 110},       {"DIXON", 111},     {"DOMINGUEZ", 112},  {"DOUGLAS", 113},    {"DOYLE", 114},
    {"DUNCAN", 115},     {"DUNN", 116},      {"DURAN", 117},      {"EDWARDS", 118},    {"ELLIOTT", 119},
    {"ELLIS", 120},      {"ERICKSON", 121},  {"ESPINOZA", 122},   {"ESTRADA", 123},    {"EVANS", 124},
    {"FARMER", 125},     {"FERGUSON", 126},  {"FERNANDEZ", 127},  {"FIELDS", 128},     {"FIGUEROA", 129},
    {"FISCHER", 130},    {"FISHER", 131},    {"FITZGERALD", 132}, {"FLEMING", 133},    {"FLETCHER", 134},
    {"FLORES", 135},     {"FORD", 136},      {"FOSTER", 137},     {"FOWLER", 138},     {"FOX", 139},
    {"FRANCIS", 140},    {"FRANCO", 141},    {"FRANK", 142},      {"FRANKLIN", 143},   {"FRAZIER", 144},
    {"FREEMAN", 145},    {"FUENTES", 146},   {"FULLER", 147},     {"GALLAGHER", 148},  {"GALLEGOS", 149},
    {"GARCIA", 150},     {"GARDNER", 151},   {"GARNER", 152},     {"GARRETT", 153},    {"GARZA", 154},
    {"GEORGE", 155},     {"GIBSON", 156},    {"GILBERT", 157},    {"GILL", 158},       {"GOMEZ", 159},
    {"GONZALES", 160},   {"GONZALEZ", 161},  {"GOODMAN", 162},    {"GOODWIN", 163},    {"GORDON", 164},
    {"GRAHAM", 165},     {"GRANT", 166},     {"GRAVES", 167},     {"GRAY", 168},       {"GREEN", 169},
    {"GREENE", 170},     {"GREGORY", 171},   {"GRIFFIN", 172},    {"GRIFFITH", 173},   {"GROSS", 174},
    {"GUERRA", 175},     {"GUERRERO", 176},  {"GUTIERREZ", 177},  {"GUZMAN", 178},     {"HAIL", 179},
    {"HALE", 180},       {"HALL", 181},      {"HAMILTON", 182},   {"HAMMOND", 183},    {"HAMPTON", 184},
    {"HANSEN", 185},     {"HANSON", 186},    {"HARDY", 187},      {"HARMON", 188},     {"HARPER", 189},
    {"HARRINGTON", 190}, {"HARRIS", 191},    {"HARRISON", 192},   {"HART", 193},       {"HARVEY", 194},
    {"HAWKINS", 195},    {"HAYES", 196},     {"HAYNES", 197},     {"HENDERSON", 198},  {"HENRY", 199},
    {"HERNANDEZ", 200},  {"HERRERA", 201},   {"HICKS", 202},      {"HIGGINS", 203},    {"HILL", 204},
    {"HINES", 205},      {"HODGES", 206},    {"HOFFMAN", 207},    {"HOLLAND", 208},    {"HOLMES", 209},
    {"HOLT", 210},       {"HOPKINS", 211},   {"HORTON", 212},     {"HOWARD", 213},     {"HOWELL", 214},
    {"HUANG", 215},      {"HUBBARD", 216},   {"HUDSON", 217},     {"HUGHES", 218},     {"HUNT", 219},
    {"HUNTER", 220},     {"INGRAM", 221},    {"JACKSON", 222},    {"JACOBS", 223},     {"JAMES", 224},
    {"JENKINS", 225},    {"JENNINGS", 226},  {"JENSEN", 227},     {"JIMENEZ", 228},    {"JOHNSON", 229},
    {"JOHNSTON", 230},   {"JONES", 231},     {"JORDAN", 232},     {"JOSEPH", 233},     {"JUAREZ", 234},
    {"KELLER", 235},     {"KELLEY", 236},    {"KELLY", 237},      {"KENNEDY", 238},    {"KHAN", 239},
    {"KIM", 240},        {"KING", 241},      {"KLEIN", 242},      {"KNIGHT", 243},     {"LAMBERT", 244},
    {"LANE", 245},       {"LARA", 246},      {"LARSON", 247},     {"LAWRENCE", 248},   {"LAWSON", 249},
    {"LE", 250},         {"LEE", 251},       {"LEON", 252},       {"LEONARD", 253},    {"LEWIS", 254},
    {"LI", 255},         {"LIN", 256},       {"LITTLE", 257},     {"LIU", 258},        {"LOGAN", 259},
    {"LONG", 260},       {"LOPEZ", 261},     {"LOVE", 262},       {"LOWE", 263},       {"LUCAS", 264},
    {"LUNA", 265},       {"LYNCH", 266},     {"LYONS", 267},      {"MACK", 268},       {"MALDONADO", 269},
    {"MALONE", 270},     {"MANN", 271},      {"MANNING", 272},    {"MARQUEZ", 273},    {"MARSHALL", 274},
    {"MARTIN", 275},     {"MARTINEZ", 276},  {"MASON", 277},      {"MATTHEWS", 278},   {"MAXWELL", 279},
    {"MAY", 280},        {"MCCARTHY", 281},  {"MCCOY", 282},      {"MCDANIEL", 283},   {"MCDONALD", 284},
    {"MCGEE", 285},      {"MCKINNEY", 286},  {"MCLAUGHLIN", 287}, {"MEDINA", 288},     {"MEJIA", 289},
    {"MENDEZ", 290},     {"MENDOZA", 291},   {"MEYER", 292},      {"MILES", 293},      {"MILLER", 294},
    {"MILLS", 295},      {"MIRANDA", 296},   {"MITCHELL", 297},   {"MOLINA", 298},     {"MONTGOMERY", 299},
    {"MONTOYA", 300},    {"MOORE", 301},     {"MORALES", 302},    {"MORAN", 303},      {"MORENO", 304},
    {"MORGAN", 305},     {"MORRIS", 306},    {"MORRISON", 307},   {"MOSS", 308},       {"MULLINS", 309},
    {"MUNOZ", 310},      {"MURPHY", 311},    {"MURRAY", 312},     {"MYERS", 313},      {"NAVARRO", 314},
    {"NEAL", 315},       {"NELSON", 316},    {"NEWMAN", 317},     {"NEWTON", 318},     {"NGUYEN", 319},
    {"NICHOLS", 320},    {"NORMAN", 321},    {"NORRIS", 322},     {"NUNEZ", 323},      {"OBRIEN", 324},
    {"OCHOA", 325},      {"OCONNOR", 326},   {"OLIVER", 327},     {"OLSON", 328},      {"ORTEGA", 329},
    {"ORTIZ", 330},      {"OWENS", 331},     {"PACHECO", 332},    {"PADILLA", 333},    {"PAGE", 334},
    {"PALMER", 335},     {"PARK", 336},      {"PARKER", 337},     {"PARKS", 338},      {"PARSONS", 339},
    {"PATEL", 340},      {"PATTERSON", 341}, {"PAUL", 342},       {"PAYNE", 343},      {"PEARSON", 344},
    {"PENA", 345},       {"PEREZ", 346},     {"PERKINS", 347},    {"PERRY", 348},      {"PERSON", 349},
    {"PETERS", 350},     {"PETERSON", 351},  {"PHAM", 352},       {"PHILLIPS", 353},   {"PIERCE", 354},
    {"PORTER", 355},     {"POTTER", 356},    {"POWELL", 357},     {"POWERS", 358},     {"PRICE", 359},
    {"QUINN", 360},      {"RAMIREZ", 361},   {"RAMOS", 362},      {"RAMSEY", 363},     {"RAY", 364},
    {"REED", 365},       {"REESE", 366},     {"REEVES", 367},     {"REID", 368},       {"REYES", 369},
    {"REYNOLDS", 370},   {"RHODES", 371},    {"RICE", 372},       {"RICHARDS", 373},   {"RICHARDSON", 374},
    {"RILEY", 375},      {"RIOS", 376},      {"RIVAS", 377},      {"RIVERA", 378},     {"ROBBINS", 379},
    {"ROBERTS", 380},    {"ROBERTSON", 381}, {"ROBINSON", 382},   {"ROBLES", 383},     {"RODGERS", 384},
    {"RODRIGUEZ", 385},  {"ROGERS", 386},    {"ROJAS", 387},      {"ROMAN", 388},      {"ROMERO", 389},
    {"ROSALES", 390},    {"ROSE", 391},      {"ROSS", 392},       {"ROWE", 393},       {"RUIZ", 394},
    {"RUSSELL", 395},    {"RYAN", 396},      {"SALAZAR", 397},    {"SALINAS", 398},    {"SANCHEZ", 399},
    {"SANDERS", 400},    {"SANDOVAL", 401},  {"SANTIAGO", 402},   {"SANTOS", 403},     {"SAUNDERS", 404},
    {"SCHMIDT", 405},    {"SCHNEIDER", 406}, {"SCHROEDER", 407},  {"SCHULTZ", 408},    {"SCHWARTZ", 409},
    {"SCOTT", 410},      {"SERRANO", 411},   {"SHARP", 412},      {"SHAW", 413},       {"SHELTON", 414},
    {"SHERMAN", 415},    {"SILVA", 416},     {"SIMMONS", 417},    {"SIMON", 418},      {"SIMPSON", 419},
    {"SIMS", 420},       {"SINGH", 421},     {"SMITH", 422},      {"SNYDER", 423},     {"SOLIS", 424},
    {"SOTO", 425},       {"SPENCER", 426},   {"STANLEY", 427},    {"STEELE", 428},     {"STEPHENS", 429},
    {"STEVENS", 430},    {"STEVENSON", 431}, {"STEWART", 432},    {"STONE", 433},      {"STRICKLAND", 434},
    {"SULLIVAN", 435},   {"SUTTON", 436},    {"SWANSON", 437},    {"TATE", 438},       {"TAYLOR", 439},
    {"TERRY", 440},      {"THOMAS", 441},    {"THOMPSON", 442},   {"THORNTON", 443},   {"TODD", 444},
    {"TORRES", 445},     {"TOWNSEND", 446},  {"TRAN", 447},       {"TRUJILLO", 448},   {"TUCKER", 449},
    {"TURNER", 450},     {"VALDEZ", 451},    {"VALENCIA", 452},   {"VARGAS", 453},     {"VASQUEZ", 454},
    {"VAUGHN", 455},     {"VAZQUEZ", 456},   {"VEGA", 457},       {"VELASQUEZ", 458},  {"WADE", 459},
    {"WAGNER", 460},     {"WALKER", 461},    {"WALLACE", 462},    {"WALSH", 463},      {"WALTERS", 464},
    {"WALTON", 465},     {"WANG", 466},      {"WARD", 467},       {"WARNER", 468},     {"WARREN", 469},
    {"WASHINGTON", 470}, {"WATERS", 471},    {"WATKINS", 472},    {"WATSON", 473},     {"WATTS", 474},
    {"WEAVER", 475},     {"WEBB", 476},      {"WEBER", 477},      {"WEBSTER", 478},    {"WELCH", 479},
    {"WELLS", 480},      {"WEST", 481},      {"WHEELER", 482},    {"WHITE", 483},      {"WILLIAMS", 484},
    {"WILLIAMSON", 485}, {"WILLIS", 486},    {"WILSON", 487},     {"WISE", 488},       {"WOLF", 489},
    {"WOLFE", 490},      {"WONG", 491},      {"WOOD", 492},       {"WOODS", 493},      {"WRIGHT", 494},
    {"WU", 495},         {"YANG", 496},      {"YOUNG", 497},      {"ZHANG", 498},      {"ZIMMERMAN", 499}}; */
unordered_map<string, vector<int*>>  firstNamesMap = {{"AALIYAH", {new int(0)}}, {"AARON", {new int(1)}}, {"ABEL", {new int(2)}}, {"ABIGAIL", {new int(3)}}, {"ABRAHAM", {new int(4)}}, {"ADALINE", {new int(5)}}, {"ADALYN", {new int(6)}}, {"ADALYNN", {new int(7)}}, {"ADAM", {new int(8)}}, {"ADDISON", {new int(9)}}, {"ADELINE", {new int(10)}}, {"ADELYN", {new int(11)}}, {"ADRIAN", {new int(12)}}, {"ADRIANA", {new int(13)}}, {"AIDAN", {new int(14)}}, {"AIDEN", {new int(15)}}, {"ALAINA", {new int(16)}}, {"ALAN", {new int(17)}}, {"ALANA", {new int(18)}}, {"ALAYNA", {new int(19)}}, {"ALEJANDRO", {new int(20)}}, {"ALEX", {new int(21)}}, {"ALEXA", {new int(22)}}, {"ALEXANDER", {new int(23)}}, {"ALEXANDRA", {new int(24)}}, {"ALEXIS", {new int(25)}}, {"ALICE", {new int(26)}}, {"ALINA", {new int(27)}}, {"ALIVIA", {new int(28)}}, {"ALIYAH", {new int(29)}}, {"ALLISON", {new int(30)}}, {"ALYSSA", {new int(31)}}, {"AMARA", {new int(32)}}, {"AMAYA", {new int(33)}}, {"AMELIA", {new int(34)}}, {"AMIR", {new int(35)}}, {"AMY", {new int(36)}}, {"ANA", {new int(37)}}, {"ANASTASIA", {new int(38)}}, {"ANDREA", {new int(39)}}, {"ANDRES", {new int(40)}}, {"ANDREW", {new int(41)}}, {"ANGEL", {new int(42)}}, {"ANGELA", {new int(43)}}, {"ANGELINA", {new int(44)}}, {"ANNA", {new int(45)}}, {"ANNABELLE", {new int(46)}}, {"ANTHONY", {new int(47)}}, {"ANTONIO", {new int(48)}}, {"ARABELLA", {new int(49)}}, {"ARIA", {new int(50)}}, {"ARIANA", {new int(51)}}, {"ARIANNA", {new int(52)}}, {"ARIEL", {new int(53)}}, {"ARTHUR", {new int(54)}}, {"ARYA", {new int(55)}}, {"ASHER", {new int(56)}}, {"ASHLEY", {new int(57)}}, {"ASHTON", {new int(58)}}, {"ATHENA", {new int(59)}}, {"AUBREE", {new int(60)}}, {"AUBREY", {new int(61)}}, {"AUDREY", {new int(62)}}, {"AUGUST", {new int(63)}}, {"AURORA", {new int(64)}}, {"AUSTIN", {new int(65)}}, {"AUTUMN", {new int(66)}}, {"AVA", {new int(67)}}, {"AVERY", {new int(68)}}, {"AXEL", {new int(69)}}, {"AYDEN", {new int(70)}}, {"AYLA", {new int(71)}}, {"BAILEY", {new int(72)}}, {"BARRETT", {new int(73)}}, {"BEAU", {new int(74)}}, {"BECKETT", {new int(75)}}, {"BELLA", {new int(76)}}, {"BENJAMIN", {new int(77)}}, {"BENNETT", {new int(78)}}, {"BENTLEY", {new int(79)}}, {"BLAKE", {new int(80)}}, {"BRADLEY", {new int(81)}}, {"BRADY", {new int(82)}}, {"BRANDON", {new int(83)}}, {"BRANTLEY", {new int(84)}}, {"BRAXTON", {new int(85)}}, {"BRAYDEN", {new int(86)}}, {"BRIAN", {new int(87)}}, {"BRIANNA", {new int(88)}}, {"BRIELLE", {new int(89)}}, {"BRODY", {new int(90)}}, {"BROOKE", {new int(91)}}, {"BROOKLYN", {new int(92)}}, {"BROOKLYNN", {new int(93)}}, {"BROOKS", {new int(94)}}, {"BRYAN", {new int(95)}}, {"BRYCE", {new int(96)}}, {"BRYNLEE", {new int(97)}}, {"BRYSON", {new int(98)}}, {"CADEN", {new int(99)}}, {"CALEB", {new int(100)}}, {"CALLIE", {new int(101)}}, {"CALVIN", {new int(102)}}, {"CAMDEN", {new int(103)}}, {"CAMERON", {new int(104)}}, {"CAMILA", {new int(105)}}, {"CARLOS", {new int(106)}}, {"CAROLINE", {new int(107)}}, {"CARSON", {new int(108)}}, {"CARTER", {new int(109)}}, {"CATHERINE", {new int(110)}}, {"CAYDEN", {new int(111)}}, {"CECILIA", {new int(112)}}, {"CHARLES", {new int(113)}}, {"CHARLIE", {new int(114)}}, {"CHARLOTTE", {new int(115)}}, {"CHASE", {new int(116)}}, {"CHLOE", {new int(117)}}, {"CHRISTIAN", {new int(118)}}, {"CHRISTOPHER", {new int(119)}}, {"CLAIRE", {new int(120)}}, {"CLARA", {new int(121)}}, {"CLAYTON", {new int(122)}}, {"COLE", {new int(123)}}, {"COLIN", {new int(124)}}, {"COLTON", {new int(125)}}, {"CONNOR", {new int(126)}}, {"COOPER", {new int(127)}}, {"CORA", {new int(128)}}, {"DAISY", {new int(129)}}, {"DAKOTA", {new int(130)}}, {"DALEYZA", {new int(131)}}, {"DAMIAN", {new int(132)}}, {"DANIEL", {new int(133)}}, {"DANIELA", {new int(134)}}, {"DAVID", {new int(135)}}, {"DAWSON", {new int(136)}}, {"DEAN", {new int(137)}}, {"DECLAN", {new int(138)}}, {"DELANEY", {new int(139)}}, {"DELILAH", {new int(140)}}, {"DEREK", {new int(141)}}, {"DESTINY", {new int(142)}}, {"DIANA", {new int(143)}}, {"DIEGO", {new int(144)}}, {"DOMINIC", {new int(145)}}, {"DYLAN", {new int(146)}}, {"EASTON", {new int(147)}}, {"EDEN", {new int(148)}}, {"EDWARD", {new int(149)}}, {"ELEANOR", {new int(150)}}, {"ELENA", {new int(151)}}, {"ELI", {new int(152)}}, {"ELIANA", {new int(153)}}, {"ELIAS", {new int(154)}}, {"ELIJAH", {new int(155)}}, {"ELISE", {new int(156)}}, {"ELIZA", {new int(157)}}, {"ELIZABETH", {new int(158)}}, {"ELLA", {new int(159)}}, {"ELLIANA", {new int(160)}}, {"ELLIE", {new int(161)}}, {"ELLIOT", {new int(162)}}, {"ELLIOTT", {new int(163)}}, {"ELOISE", {new int(164)}}, {"EMERSON", {new int(165)}}, {"EMERSYN", {new int(166)}}, {"EMERY", {new int(167)}}, {"EMILIA", {new int(168)}}, {"EMILIANO", {new int(169)}}, {"EMILY", {new int(170)}}, {"EMMA", {new int(171)}}, {"EMMANUEL", {new int(172)}}, {"EMMETT", {new int(173)}}, {"ERIC", {new int(174)}}, {"ESTHER", {new int(175)}}, {"ETHAN", {new int(176)}}, {"EVA", {new int(177)}}, {"EVAN", {new int(178)}}, {"EVELYN", {new int(179)}}, {"EVERETT", {new int(180)}}, {"EVERLY", {new int(181)}}, {"EZEKIEL", {new int(182)}}, {"EZRA", {new int(183)}}, {"FAITH", {new int(184)}}, {"FELIX", {new int(185)}}, {"FINLEY", {new int(186)}}, {"FINN", {new int(187)}}, {"FIONA", {new int(188)}}, {"GABRIEL", {new int(189)}}, {"GABRIELLA", {new int(190)}}, {"GAEL", {new int(191)}}, {"GAVIN", {new int(192)}}, {"GENESIS", {new int(193)}}, {"GENEVIEVE", {new int(194)}}, {"GEORGE", {new int(195)}}, {"GEORGIA", {new int(196)}}, {"GIANNA", {new int(197)}}, {"GIOVANNI", {new int(198)}}, {"GRACE", {new int(199)}}, {"GRACIE", {new int(200)}}, {"GRAHAM", {new int(201)}}, {"GRANT", {new int(202)}}, {"GRAYSON", {new int(203)}}, {"GREYSON", {new int(204)}}, {"GRIFFIN", {new int(205)}}, {"HADLEY", {new int(206)}}, {"HAILEY", {new int(207)}}, {"HANNAH", {new int(208)}}, {"HARLEY", {new int(209)}}, {"HARMONY", {new int(210)}}, {"HARPER", {new int(211)}}, {"HARRISON", {new int(212)}}, {"HAYDEN", {new int(213)}}, {"HAZEL", {new int(214)}}, {"HENRY", {new int(215)}}, {"HOLDEN", {new int(216)}}, {"HUDSON", {new int(217)}}, {"HUNTER", {new int(218)}}, {"IAN", {new int(219)}}, {"IRIS", {new int(220)}}, {"ISAAC", {new int(221)}}, {"ISABEL", {new int(222)}}, {"ISABELLA", {new int(223)}}, {"ISABELLE", {new int(224)}}, {"ISAIAH", {new int(225)}}, {"ISLA", {new int(226)}}, {"ISRAEL", {new int(227)}}, {"IVAN", {new int(228)}}, {"IVY", {new int(229)}}, {"JACE", {new int(230)}}, {"JACK", {new int(231)}}, {"JACKSON", {new int(232)}}, {"JACOB", {new int(233)}}, {"JADE", {new int(234)}}, {"JADEN", {new int(235)}}, {"JAKE", {new int(236)}}, {"JAMES", {new int(237)}}, {"JAMESON", {new int(238)}}, {"JASMINE", {new int(239)}}, {"JASON", {new int(240)}}, {"JASPER", {new int(241)}}, {"JAVIER", {new int(242)}}, {"JAX", {new int(243)}}, {"JAXON", {new int(244)}}, {"JAXSON", {new int(245)}}, {"JAYCE", {new int(246)}}, {"JAYDEN", {new int(247)}}, {"JAYLA", {new int(248)}}, {"JEREMIAH", {new int(249)}}, {"JEREMY", {new int(250)}}, {"JESSE", {new int(251)}}, {"JESSICA", {new int(252)}}, {"JESUS", {new int(253)}}, {"JOANNA", {new int(254)}}, {"JOCELYN", {new int(255)}}, {"JOEL", {new int(256)}}, {"JOHN", {new int(257)}}, {"JONAH", {new int(258)}}, {"JONATHAN", {new int(259)}}, {"JORDAN", {new int(260)}}, {"JORDYN", {new int(261)}}, {"JORGE", {new int(262)}}, {"JOSE", {new int(263)}}, {"JOSEPH", {new int(264)}}, {"JOSEPHINE", {new int(265)}}, {"JOSHUA", {new int(266)}}, {"JOSIAH", {new int(267)}}, {"JOSIE", {new int(268)}}, {"JOSUE", {new int(269)}}, {"JUAN", {new int(270)}}, {"JUDAH", {new int(271)}}, {"JUDE", {new int(272)}}, {"JULIA", {new int(273)}}, {"JULIAN", {new int(274)}}, {"JULIANA", {new int(275)}}, {"JULIANNA", {new int(276)}}, {"JULIET", {new int(277)}}, {"JULIETTE", {new int(278)}}, {"JUNE", {new int(279)}}, {"JUSTIN", {new int(280)}}, {"KADEN", {new int(281)}}, {"KAI", {new int(282)}}, {"KAIDEN", {new int(283)}}, {"KALEB", {new int(284)}}, {"KARTER", {new int(285)}}, {"KATHERINE", {new int(286)}}, {"KAYDEN", {new int(287)}}, {"KAYLA", {new int(288)}}, {"KAYLEE", {new int(289)}}, {"KENDALL", {new int(290)}}, {"KENNEDY", {new int(291)}}, {"KENNETH", {new int(292)}}, {"KEVIN", {new int(293)}}, {"KHLOE", {new int(294)}}, {"KILLIAN", {new int(295)}}, {"KIMBERLY", {new int(296)}}, {"KING", {new int(297)}}, {"KINGSTON", {new int(298)}}, {"KINSLEY", {new int(299)}}, {"KNOX", {new int(300)}}, {"KYLE", {new int(301)}}, {"KYLIE", {new int(302)}}, {"KYRIE", {new int(303)}}, {"LAILA", {new int(304)}}, {"LANDON", {new int(305)}}, {"LAUREN", {new int(306)}}, {"LAYLA", {new int(307)}}, {"LEAH", {new int(308)}}, {"LEILA", {new int(309)}}, {"LEILANI", {new int(310)}}, {"LEO", {new int(311)}}, {"LEON", {new int(312)}}, {"LEONARDO", {new int(313)}}, {"LEVI", {new int(314)}}, {"LIAM", {new int(315)}}, {"LILA", {new int(316)}}, {"LILIANA", {new int(317)}}, {"LILLIAN", {new int(318)}}, {"LILLY", {new int(319)}}, {"LILY", {new int(320)}}, {"LINCOLN", {new int(321)}}, {"LOGAN", {new int(322)}}, {"LOLA", {new int(323)}}, {"LONDON", {new int(324)}}, {"LONDYN", {new int(325)}}, {"LORENZO", {new int(326)}}, {"LUCA", {new int(327)}}, {"LUCAS", {new int(328)}}, {"LUCIA", {new int(329)}}, {"LUCY", {new int(330)}}, {"LUIS", {new int(331)}}, {"LUKAS", {new int(332)}}, {"LUKE", {new int(333)}}, {"LUNA", {new int(334)}}, {"LYDIA", {new int(335)}}, {"LYLA", {new int(336)}}, {"MACKENZIE", {new int(337)}}, {"MADDOX", {new int(338)}}, {"MADELINE", {new int(339)}}, {"MADELYN", {new int(340)}}, {"MADISON", {new int(341)}}, {"MAGGIE", {new int(342)}}, {"MAKAYLA", {new int(343)}}, {"MALACHI", {new int(344)}}, {"MALIA", {new int(345)}}, {"MARCUS", {new int(346)}}, {"MARGARET", {new int(347)}}, {"MARIA", {new int(348)}}, {"MARIAH", {new int(349)}}, {"MARK", {new int(350)}}, {"MARLEY", {new int(351)}}, {"MARY", {new int(352)}}, {"MASON", {new int(353)}}, {"MATEO", {new int(354)}}, {"MATIAS", {new int(355)}}, {"MATTEO", {new int(356)}}, {"MATTHEW", {new int(357)}}, {"MAVERICK", {new int(358)}}, {"MAX", {new int(359)}}, {"MAXIMUS", {new int(360)}}, {"MAXWELL", {new int(361)}}, {"MAYA", {new int(362)}}, {"MCKENZIE", {new int(363)}}, {"MELANIE", {new int(364)}}, {"MELODY", {new int(365)}}, {"MESSIAH", {new int(366)}}, {"MIA", {new int(367)}}, {"MICAH", {new int(368)}}, {"MICHAEL", {new int(369)}}, {"MICHELLE", {new int(370)}}, {"MIGUEL", {new int(371)}}, {"MILA", {new int(372)}}, {"MILES", {new int(373)}}, {"MILO", {new int(374)}}, {"MOLLY", {new int(375)}}, {"MORGAN", {new int(376)}}, {"MYA", {new int(377)}}, {"MYLES", {new int(378)}}, {"NAOMI", {new int(379)}}, {"NATALIA", {new int(380)}}, {"NATALIE", {new int(381)}}, {"NATHAN", {new int(382)}}, {"NATHANIEL", {new int(383)}}, {"NEVAEH", {new int(384)}}, {"NICHOLAS", {new int(385)}}, {"NICOLAS", {new int(386)}}, {"NICOLE", {new int(387)}}, {"NOAH", {new int(388)}}, {"NOELLE", {new int(389)}}, {"NOLAN", {new int(390)}}, {"NORA", {new int(391)}}, {"NORAH", {new int(392)}}, {"NOVA", {new int(393)}}, {"OLIVER", {new int(394)}}, {"OLIVIA", {new int(395)}}, {"OMAR", {new int(396)}}, {"OSCAR", {new int(397)}}, {"OWEN", {new int(398)}}, {"PAIGE", {new int(399)}}, {"PAISLEY", {new int(400)}}, {"PARKER", {new int(401)}}, {"PATRICK", {new int(402)}}, {"PAUL", {new int(403)}}, {"PAXTON", {new int(404)}}, {"PAYTON", {new int(405)}}, {"PENELOPE", {new int(406)}}, {"PETER", {new int(407)}}, {"PEYTON", {new int(408)}}, {"PIPER", {new int(409)}}, {"PRESLEY", {new int(410)}}, {"PRESTON", {new int(411)}}, {"QUINN", {new int(412)}}, {"RACHEL", {new int(413)}}, {"RAELYNN", {new int(414)}}, {"REAGAN", {new int(415)}}, {"REBECCA", {new int(416)}}, {"REESE", {new int(417)}}, {"REMI", {new int(418)}}, {"REMINGTON", {new int(419)}}, {"RHETT", {new int(420)}}, {"RICHARD", {new int(421)}}, {"RILEY", {new int(422)}}, {"RIVER", {new int(423)}}, {"ROBERT", {new int(424)}}, {"ROMAN", {new int(425)}}, {"ROSALIE", {new int(426)}}, {"ROSE", {new int(427)}}, {"ROWAN", {new int(428)}}, {"RUBY", {new int(429)}}, {"RYAN", {new int(430)}}, {"RYDER", {new int(431)}}, {"RYKER", {new int(432)}}, {"RYLEE", {new int(433)}}, {"RYLEIGH", {new int(434)}}, {"SADIE", {new int(435)}}, {"SAMANTHA", {new int(436)}}, {"SAMUEL", {new int(437)}}, {"SANTIAGO", {new int(438)}}, {"SARA", {new int(439)}}, {"SARAH", {new int(440)}}, {"SAVANNAH", {new int(441)}}, {"SAWYER", {new int(442)}}, {"SCARLETT", {new int(443)}}, {"SEBASTIAN", {new int(444)}}, {"SELENA", {new int(445)}}, {"SERENITY", {new int(446)}}, {"SIENNA", {new int(447)}}, {"SILAS", {new int(448)}}, {"SKYLAR", {new int(449)}}, {"SLOANE", {new int(450)}}, {"SOFIA", {new int(451)}}, {"SOPHIA", {new int(452)}}, {"SOPHIE", {new int(453)}}, {"STELLA", {new int(454)}}, {"STEVEN", {new int(455)}}, {"SUMMER", {new int(456)}}, {"SYDNEY", {new int(457)}}, {"TAYLOR", {new int(458)}}, {"TEAGAN", {new int(459)}}, {"TESSA", {new int(460)}}, {"THEODORE", {new int(461)}}, {"THIAGO", {new int(462)}}, {"THOMAS", {new int(463)}}, {"TIMOTHY", {new int(464)}}, {"TRINITY", {new int(465)}}, {"TRISTAN", {new int(466)}}, {"TUCKER", {new int(467)}}, {"TYLER", {new int(468)}}, {"VALENTINA", {new int(469)}}, {"VALERIA", {new int(470)}}, {"VALERIE", {new int(471)}}, {"VANESSA", {new int(472)}}, {"VICTOR", {new int(473)}}, {"VICTORIA", {new int(474)}}, {"VINCENT", {new int(475)}}, {"VIOLET", {new int(476)}}, {"VIVIAN", {new int(477)}}, {"WAYLON", {new int(478)}}, {"WESLEY", {new int(479)}}, {"WESTON", {new int(480)}}, {"WILLIAM", {new int(481)}}, {"WILLOW", {new int(482)}}, {"WYATT", {new int(483)}}, {"XANDER", {new int(484)}}, {"XAVIER", {new int(485)}}, {"XIMENA", {new int(486)}}, {"ZACHARY", {new int(487)}}, {"ZANDER", {new int(488)}}, {"ZANE", {new int(489)}}, {"ZAYDEN", {new int(490)}}, {"ZION", {new int(491)}}, {"ZOE", {new int(492)}}, {"ZOEY", {new int(493)}}};
unordered_map<string, vector<int*>>  lastNamesMap = {{"ACOSTA", {new int(0)}}, {"ADAMS", {new int(1)}}, {"ADKINS", {new int(2)}}, {"AGUILAR", {new int(3)}}, {"AGUIRRE", {new int(4)}}, {"ALEXANDER", {new int(5)}}, {"ALLEN", {new int(6)}}, {"ALVARADO", {new int(7)}}, {"ALVAREZ", {new int(8)}}, {"ANDERSON", {new int(9)}}, {"ANDREWS", {new int(10)}}, {"ARMSTRONG", {new int(11)}}, {"ARNOLD", {new int(12)}}, {"AUSTIN", {new int(13)}}, {"AVILA", {new int(14)}}, {"AYALA", {new int(15)}}, {"BAILEY", {new int(16)}}, {"BAKER", {new int(17)}}, {"BALDWIN", {new int(18)}}, {"BANKS", {new int(19)}}, {"BARBER", {new int(20)}}, {"BARKER", {new int(21)}}, {"BARNES", {new int(22)}}, {"BARNETT", {new int(23)}}, {"BARRETT", {new int(24)}}, {"BARTON", {new int(25)}}, {"BATES", {new int(26)}}, {"BECK", {new int(27)}}, {"BECKER", {new int(28)}}, {"BELL", {new int(29)}}, {"BENNETT", {new int(30)}}, {"BENSON", {new int(31)}}, {"BERRY", {new int(32)}}, {"BISHOP", {new int(33)}}, {"BLACK", {new int(34)}}, {"BLAIR", {new int(35)}}, {"BLAKE", {new int(36)}}, {"BOWEN", {new int(37)}}, {"BOWMAN", {new int(38)}}, {"BOYD", {new int(39)}}, {"BRADLEY", {new int(40)}}, {"BRADY", {new int(41)}}, {"BREWER", {new int(42)}}, {"BROOKS", {new int(43)}}, {"BROWN", {new int(44)}}, {"BRYANT", {new int(45)}}, {"BURGESS", {new int(46)}}, {"BURKE", {new int(47)}}, {"BURNS", {new int(48)}}, {"BURTON", {new int(49)}}, {"BUSH", {new int(50)}}, {"BUTLER", {new int(51)}}, {"BYRD", {new int(52)}}, {"CABRERA", {new int(53)}}, {"CALDERON", {new int(54)}}, {"CALDWELL", {new int(55)}}, {"CAMACHO", {new int(56)}}, {"CAMPBELL", {new int(57)}}, {"CAMPOS", {new int(58)}}, {"CANNON", {new int(59)}}, {"CARDENAS", {new int(60)}}, {"CARLSON", {new int(61)}}, {"CARPENTER", {new int(62)}}, {"CARR", {new int(63)}}, {"CARRILLO", {new int(64)}}, {"CARROLL", {new int(65)}}, {"CARTER", {new int(66)}}, {"CASTANEDA", {new int(67)}}, {"CASTILLO", {new int(68)}}, {"CASTRO", {new int(69)}}, {"CERVANTES", {new int(70)}}, {"CHAMBERS", {new int(71)}}, {"CHAN", {new int(72)}}, {"CHANDLER", {new int(73)}}, {"CHANG", {new int(74)}}, {"CHAPMAN", {new int(75)}}, {"CHAVEZ", {new int(76)}}, {"CHEN", {new int(77)}}, {"CHRISTENSEN", {new int(78)}}, {"CLARK", {new int(79)}}, {"CLARKE", {new int(80)}}, {"COHEN", {new int(81)}}, {"COLE", {new int(82)}}, {"COLEMAN", {new int(83)}}, {"COLLINS", {new int(84)}}, {"COLON", {new int(85)}}, {"CONTRERAS", {new int(86)}}, {"COOK", {new int(87)}}, {"COOPER", {new int(88)}}, {"CORTEZ", {new int(89)}}, {"COX", {new int(90)}}, {"CRAIG", {new int(91)}}, {"CRAWFORD", {new int(92)}}, {"CROSS", {new int(93)}}, {"CRUZ", {new int(94)}}, {"CUMMINGS", {new int(95)}}, {"CUNNINGHAM", {new int(96)}}, {"CURRY", {new int(97)}}, {"CURTIS", {new int(98)}}, {"DANIEL", {new int(99)}}, {"DANIELS", {new int(100)}}, {"DAVIDSON", {new int(101)}}, {"DAVIS", {new int(102)}}, {"DAWSON", {new int(103)}}, {"DAY", {new int(104)}}, {"DEAN", {new int(105)}}, {"DELACRUZ", {new int(106)}}, {"DELEON", {new int(107)}}, {"DELGADO", {new int(108)}}, {"DENNIS", {new int(109)}}, {"DIAZ", {new int(110)}}, {"DIXON", {new int(111)}}, {"DOMINGUEZ", {new int(112)}}, {"DOUGLAS", {new int(113)}}, {"DOYLE", {new int(114)}}, {"DUNCAN", {new int(115)}}, {"DUNN", {new int(116)}}, {"DURAN", {new int(117)}}, {"EDWARDS", {new int(118)}}, {"ELLIOTT", {new int(119)}}, {"ELLIS", {new int(120)}}, {"ERICKSON", {new int(121)}}, {"ESPINOZA", {new int(122)}}, {"ESTRADA", {new int(123)}}, {"EVANS", {new int(124)}}, {"FARMER", {new int(125)}}, {"FERGUSON", {new int(126)}}, {"FERNANDEZ", {new int(127)}}, {"FIELDS", {new int(128)}}, {"FIGUEROA", {new int(129)}}, {"FISCHER", {new int(130)}}, {"FISHER", {new int(131)}}, {"FITZGERALD", {new int(132)}}, {"FLEMING", {new int(133)}}, {"FLETCHER", {new int(134)}}, {"FLORES", {new int(135)}}, {"FORD", {new int(136)}}, {"FOSTER", {new int(137)}}, {"FOWLER", {new int(138)}}, {"FOX", {new int(139)}}, {"FRANCIS", {new int(140)}}, {"FRANCO", {new int(141)}}, {"FRANK", {new int(142)}}, {"FRANKLIN", {new int(143)}}, {"FRAZIER", {new int(144)}}, {"FREEMAN", {new int(145)}}, {"FUENTES", {new int(146)}}, {"FULLER", {new int(147)}}, {"GALLAGHER", {new int(148)}}, {"GALLEGOS", {new int(149)}}, {"GARCIA", {new int(150)}}, {"GARDNER", {new int(151)}}, {"GARNER", {new int(152)}}, {"GARRETT", {new int(153)}}, {"GARZA", {new int(154)}}, {"GEORGE", {new int(155)}}, {"GIBSON", {new int(156)}}, {"GILBERT", {new int(157)}}, {"GILL", {new int(158)}}, {"GOMEZ", {new int(159)}}, {"GONZALES", {new int(160)}}, {"GONZALEZ", {new int(161)}}, {"GOODMAN", {new int(162)}}, {"GOODWIN", {new int(163)}}, {"GORDON", {new int(164)}}, {"GRAHAM", {new int(165)}}, {"GRANT", {new int(166)}}, {"GRAVES", {new int(167)}}, {"GRAY", {new int(168)}}, {"GREEN", {new int(169)}}, {"GREENE", {new int(170)}}, {"GREGORY", {new int(171)}}, {"GRIFFIN", {new int(172)}}, {"GRIFFITH", {new int(173)}}, {"GROSS", {new int(174)}}, {"GUERRA", {new int(175)}}, {"GUERRERO", {new int(176)}}, {"GUTIERREZ", {new int(177)}}, {"GUZMAN", {new int(178)}}, {"HAIL", {new int(179)}}, {"HALE", {new int(180)}}, {"HALL", {new int(181)}}, {"HAMILTON", {new int(182)}}, {"HAMMOND", {new int(183)}}, {"HAMPTON", {new int(184)}}, {"HANSEN", {new int(185)}}, {"HANSON", {new int(186)}}, {"HARDY", {new int(187)}}, {"HARMON", {new int(188)}}, {"HARPER", {new int(189)}}, {"HARRINGTON", {new int(190)}}, {"HARRIS", {new int(191)}}, {"HARRISON", {new int(192)}}, {"HART", {new int(193)}}, {"HARVEY", {new int(194)}}, {"HAWKINS", {new int(195)}}, {"HAYES", {new int(196)}}, {"HAYNES", {new int(197)}}, {"HENDERSON", {new int(198)}}, {"HENRY", {new int(199)}}, {"HERNANDEZ", {new int(200)}}, {"HERRERA", {new int(201)}}, {"HICKS", {new int(202)}}, {"HIGGINS", {new int(203)}}, {"HILL", {new int(204)}}, {"HINES", {new int(205)}}, {"HODGES", {new int(206)}}, {"HOFFMAN", {new int(207)}}, {"HOLLAND", {new int(208)}}, {"HOLMES", {new int(209)}}, {"HOLT", {new int(210)}}, {"HOPKINS", {new int(211)}}, {"HORTON", {new int(212)}}, {"HOWARD", {new int(213)}}, {"HOWELL", {new int(214)}}, {"HUANG", {new int(215)}}, {"HUBBARD", {new int(216)}}, {"HUDSON", {new int(217)}}, {"HUGHES", {new int(218)}}, {"HUNT", {new int(219)}}, {"HUNTER", {new int(220)}}, {"INGRAM", {new int(221)}}, {"JACKSON", {new int(222)}}, {"JACOBS", {new int(223)}}, {"JAMES", {new int(224)}}, {"JENKINS", {new int(225)}}, {"JENNINGS", {new int(226)}}, {"JENSEN", {new int(227)}}, {"JIMENEZ", {new int(228)}}, {"JOHNSON", {new int(229)}}, {"JOHNSTON", {new int(230)}}, {"JONES", {new int(231)}}, {"JORDAN", {new int(232)}}, {"JOSEPH", {new int(233)}}, {"JUAREZ", {new int(234)}}, {"KELLER", {new int(235)}}, {"KELLEY", {new int(236)}}, {"KELLY", {new int(237)}}, {"KENNEDY", {new int(238)}}, {"KHAN", {new int(239)}}, {"KIM", {new int(240)}}, {"KING", {new int(241)}}, {"KLEIN", {new int(242)}}, {"KNIGHT", {new int(243)}}, {"LAMBERT", {new int(244)}}, {"LANE", {new int(245)}}, {"LARA", {new int(246)}}, {"LARSON", {new int(247)}}, {"LAWRENCE", {new int(248)}}, {"LAWSON", {new int(249)}}, {"LE", {new int(250)}}, {"LEE", {new int(251)}}, {"LEON", {new int(252)}}, {"LEONARD", {new int(253)}}, {"LEWIS", {new int(254)}}, {"LI", {new int(255)}}, {"LIN", {new int(256)}}, {"LITTLE", {new int(257)}}, {"LIU", {new int(258)}}, {"LOGAN", {new int(259)}}, {"LONG", {new int(260)}}, {"LOPEZ", {new int(261)}}, {"LOVE", {new int(262)}}, {"LOWE", {new int(263)}}, {"LUCAS", {new int(264)}}, {"LUNA", {new int(265)}}, {"LYNCH", {new int(266)}}, {"LYONS", {new int(267)}}, {"MACK", {new int(268)}}, {"MALDONADO", {new int(269)}}, {"MALONE", {new int(270)}}, {"MANN", {new int(271)}}, {"MANNING", {new int(272)}}, {"MARQUEZ", {new int(273)}}, {"MARSHALL", {new int(274)}}, {"MARTIN", {new int(275)}}, {"MARTINEZ", {new int(276)}}, {"MASON", {new int(277)}}, {"MATTHEWS", {new int(278)}}, {"MAXWELL", {new int(279)}}, {"MAY", {new int(280)}}, {"MCCARTHY", {new int(281)}}, {"MCCOY", {new int(282)}}, {"MCDANIEL", {new int(283)}}, {"MCDONALD", {new int(284)}}, {"MCGEE", {new int(285)}}, {"MCKINNEY", {new int(286)}}, {"MCLAUGHLIN", {new int(287)}}, {"MEDINA", {new int(288)}}, {"MEJIA", {new int(289)}}, {"MENDEZ", {new int(290)}}, {"MENDOZA", {new int(291)}}, {"MEYER", {new int(292)}}, {"MILES", {new int(293)}}, {"MILLER", {new int(294)}}, {"MILLS", {new int(295)}}, {"MIRANDA", {new int(296)}}, {"MITCHELL", {new int(297)}}, {"MOLINA", {new int(298)}}, {"MONTGOMERY", {new int(299)}}, {"MONTOYA", {new int(300)}}, {"MOORE", {new int(301)}}, {"MORALES", {new int(302)}}, {"MORAN", {new int(303)}}, {"MORENO", {new int(304)}}, {"MORGAN", {new int(305)}}, {"MORRIS", {new int(306)}}, {"MORRISON", {new int(307)}}, {"MOSS", {new int(308)}}, {"MULLINS", {new int(309)}}, {"MUNOZ", {new int(310)}}, {"MURPHY", {new int(311)}}, {"MURRAY", {new int(312)}}, {"MYERS", {new int(313)}}, {"NAVARRO", {new int(314)}}, {"NEAL", {new int(315)}}, {"NELSON", {new int(316)}}, {"NEWMAN", {new int(317)}}, {"NEWTON", {new int(318)}}, {"NGUYEN", {new int(319)}}, {"NICHOLS", {new int(320)}}, {"NORMAN", {new int(321)}}, {"NORRIS", {new int(322)}}, {"NUNEZ", {new int(323)}}, {"OBRIEN", {new int(324)}}, {"OCHOA", {new int(325)}}, {"OCONNOR", {new int(326)}}, {"OLIVER", {new int(327)}}, {"OLSON", {new int(328)}}, {"ORTEGA", {new int(329)}}, {"ORTIZ", {new int(330)}}, {"OWENS", {new int(331)}}, {"PACHECO", {new int(332)}}, {"PADILLA", {new int(333)}}, {"PAGE", {new int(334)}}, {"PALMER", {new int(335)}}, {"PARK", {new int(336)}}, {"PARKER", {new int(337)}}, {"PARKS", {new int(338)}}, {"PARSONS", {new int(339)}}, {"PATEL", {new int(340)}}, {"PATTERSON", {new int(341)}}, {"PAUL", {new int(342)}}, {"PAYNE", {new int(343)}}, {"PEARSON", {new int(344)}}, {"PENA", {new int(345)}}, {"PEREZ", {new int(346)}}, {"PERKINS", {new int(347)}}, {"PERRY", {new int(348)}}, {"PERSON", {new int(349)}}, {"PETERS", {new int(350)}}, {"PETERSON", {new int(351)}}, {"PHAM", {new int(352)}}, {"PHILLIPS", {new int(353)}}, {"PIERCE", {new int(354)}}, {"PORTER", {new int(355)}}, {"POTTER", {new int(356)}}, {"POWELL", {new int(357)}}, {"POWERS", {new int(358)}}, {"PRICE", {new int(359)}}, {"QUINN", {new int(360)}}, {"RAMIREZ", {new int(361)}}, {"RAMOS", {new int(362)}}, {"RAMSEY", {new int(363)}}, {"RAY", {new int(364)}}, {"REED", {new int(365)}}, {"REESE", {new int(366)}}, {"REEVES", {new int(367)}}, {"REID", {new int(368)}}, {"REYES", {new int(369)}}, {"REYNOLDS", {new int(370)}}, {"RHODES", {new int(371)}}, {"RICE", {new int(372)}}, {"RICHARDS", {new int(373)}}, {"RICHARDSON", {new int(374)}}, {"RILEY", {new int(375)}}, {"RIOS", {new int(376)}}, {"RIVAS", {new int(377)}}, {"RIVERA", {new int(378)}}, {"ROBBINS", {new int(379)}}, {"ROBERTS", {new int(380)}}, {"ROBERTSON", {new int(381)}}, {"ROBINSON", {new int(382)}}, {"ROBLES", {new int(383)}}, {"RODGERS", {new int(384)}}, {"RODRIGUEZ", {new int(385)}}, {"ROGERS", {new int(386)}}, {"ROJAS", {new int(387)}}, {"ROMAN", {new int(388)}}, {"ROMERO", {new int(389)}}, {"ROSALES", {new int(390)}}, {"ROSE", {new int(391)}}, {"ROSS", {new int(392)}}, {"ROWE", {new int(393)}}, {"RUIZ", {new int(394)}}, {"RUSSELL", {new int(395)}}, {"RYAN", {new int(396)}}, {"SALAZAR", {new int(397)}}, {"SALINAS", {new int(398)}}, {"SANCHEZ", {new int(399)}}, {"SANDERS", {new int(400)}}, {"SANDOVAL", {new int(401)}}, {"SANTIAGO", {new int(402)}}, {"SANTOS", {new int(403)}}, {"SAUNDERS", {new int(404)}}, {"SCHMIDT", {new int(405)}}, {"SCHNEIDER", {new int(406)}}, {"SCHROEDER", {new int(407)}}, {"SCHULTZ", {new int(408)}}, {"SCHWARTZ", {new int(409)}}, {"SCOTT", {new int(410)}}, {"SERRANO", {new int(411)}}, {"SHARP", {new int(412)}}, {"SHAW", {new int(413)}}, {"SHELTON", {new int(414)}}, {"SHERMAN", {new int(415)}}, {"SILVA", {new int(416)}}, {"SIMMONS", {new int(417)}}, {"SIMON", {new int(418)}}, {"SIMPSON", {new int(419)}}, {"SIMS", {new int(420)}}, {"SINGH", {new int(421)}}, {"SMITH", {new int(422)}}, {"SNYDER", {new int(423)}}, {"SOLIS", {new int(424)}}, {"SOTO", {new int(425)}}, {"SPENCER", {new int(426)}}, {"STANLEY", {new int(427)}}, {"STEELE", {new int(428)}}, {"STEPHENS", {new int(429)}}, {"STEVENS", {new int(430)}}, {"STEVENSON", {new int(431)}}, {"STEWART", {new int(432)}}, {"STONE", {new int(433)}}, {"STRICKLAND", {new int(434)}}, {"SULLIVAN", {new int(435)}}, {"SUTTON", {new int(436)}}, {"SWANSON", {new int(437)}}, {"TATE", {new int(438)}}, {"TAYLOR", {new int(439)}}, {"TERRY", {new int(440)}}, {"THOMAS", {new int(441)}}, {"THOMPSON", {new int(442)}}, {"THORNTON", {new int(443)}}, {"TODD", {new int(444)}}, {"TORRES", {new int(445)}}, {"TOWNSEND", {new int(446)}}, {"TRAN", {new int(447)}}, {"TRUJILLO", {new int(448)}}, {"TUCKER", {new int(449)}}, {"TURNER", {new int(450)}}, {"VALDEZ", {new int(451)}}, {"VALENCIA", {new int(452)}}, {"VARGAS", {new int(453)}}, {"VASQUEZ", {new int(454)}}, {"VAUGHN", {new int(455)}}, {"VAZQUEZ", {new int(456)}}, {"VEGA", {new int(457)}}, {"VELASQUEZ", {new int(458)}}, {"WADE", {new int(459)}}, {"WAGNER", {new int(460)}}, {"WALKER", {new int(461)}}, {"WALLACE", {new int(462)}}, {"WALSH", {new int(463)}}, {"WALTERS", {new int(464)}}, {"WALTON", {new int(465)}}, {"WANG", {new int(466)}}, {"WARD", {new int(467)}}, {"WARNER", {new int(468)}}, {"WARREN", {new int(469)}}, {"WASHINGTON", {new int(470)}}, {"WATERS", {new int(471)}}, {"WATKINS", {new int(472)}}, {"WATSON", {new int(473)}}, {"WATTS", {new int(474)}}, {"WEAVER", {new int(475)}}, {"WEBB", {new int(476)}}, {"WEBER", {new int(477)}}, {"WEBSTER", {new int(478)}}, {"WELCH", {new int(479)}}, {"WELLS", {new int(480)}}, {"WEST", {new int(481)}}, {"WHEELER", {new int(482)}}, {"WHITE", {new int(483)}}, {"WILLIAMS", {new int(484)}}, {"WILLIAMSON", {new int(485)}}, {"WILLIS", {new int(486)}}, {"WILSON", {new int(487)}}, {"WISE", {new int(488)}}, {"WOLF", {new int(489)}}, {"WOLFE", {new int(490)}}, {"WONG", {new int(491)}}, {"WOOD", {new int(492)}}, {"WOODS", {new int(493)}}, {"WRIGHT", {new int(494)}}, {"WU", {new int(495)}}, {"YANG", {new int(496)}}, {"YOUNG", {new int(497)}}, {"ZHANG", {new int(498)}}, {"ZIMMERMAN", {new int(499)}}};

list<Data *> sortedByName[500][494];
string last;
string first;
int lastIndex;
int firstIndex;

bool compareSSN(Data *&a, Data *&b) {
  return ((((a)->ssn).compare((b)->ssn)) < 0);
}

void insertionSortSSN(list<Data *> &l) {
  auto it = l.begin();
  ++it;

  while (it != l.end()) {
    auto curr = *it;
    auto prevIt = prev(it);

    while (prevIt != l.begin() && compareSSN(curr, *prevIt)) {
      swap(*prevIt, *it);
      it = prevIt;
      prevIt = prev(prevIt);
    }

    if (prevIt == l.begin() && compareSSN(curr, *prevIt)) {
      swap(*prevIt, *it);
      it = prevIt;
    }

    ++it;
  }
}

void sortByName(std::list<Data*>& l) {
    // Assuming sortedByName is defined as a 2D vector of lists of Data pointers:

    for (auto i : l) {
        std::string last = i->lastName;
        std::string first = i->firstName;
        auto lastIter = lastNamesMap.find(last);
        auto firstIter = firstNamesMap.find(first);
        if (lastIter != lastNamesMap.end() && firstIter != firstNamesMap.end()) {
            int* lastIndex = lastIter->second.at(0); // Assuming the third element is what you need
            int* firstIndex = firstIter->second.at(0); // Assuming the first element is what you need
            // Check boundaries of the array before inserting
            if (*lastIndex >= 0 && *lastIndex < 500 && *firstIndex >= 0 && *firstIndex < 494) {
                sortedByName[*lastIndex][*firstIndex].push_back(i); // Add the element to the list
            }
        }
    }

    l.clear(); // Clear the original list

    // Define the compareSSN function or use a lambda for comparison if needed

    for (int p1 = 0; p1 < 500; p1++) {
        for (int p2 = 0; p2 < 494; p2++) {
            // Sort each sublist
            //sortedByName[p1][p2].sort(compareSSN);
            insertionSortSSN(sortedByName[p1][p2]);
            // Merge the sorted sublist back to the original list
            l.splice(l.end(), sortedByName[p1][p2]);
        }
    }
}



//_________________________________________________________________________________________________________________________________________________________________________
// MNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNMNM
//*************************************************************************************************************************************************************************

void sortDataList(list<Data *> &l) {
  first = l.front()->lastName;
  last = l.back()->lastName;

  if (first[0] == 'A' && last[0] == 'Z') {
    // Assume T3
    insertionSort(l);
  }

  else if (first == last) {
    // Assume T4
    radixSort(l);
  }

  else {
    // Assume T1 or T2
    //radixSort(l);
    sortByName(l);
  }
}