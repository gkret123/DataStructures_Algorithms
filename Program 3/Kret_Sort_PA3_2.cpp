// THIS IS THE PROVIDED CODE FOR PROGRAM #3, DSA 1, FALL 2023

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds four public fields
class Data
{
public:
    string lastName;
    string firstName;
    string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename)
{

    ifstream input(filename);
    if (!input)
    {
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
    for (int i = 0; i < size; i++)
    {
        getline(input, line);
        stringstream ss2(line);
        Data *pData = new Data();
        ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
        l.push_back(pData);
    }

    input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename)
{

    ofstream output(filename);
    if (!output)
    {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    // Write the size first
    int size = l.size();
    output << size << "\n";

    // Write the data
    for (auto pData : l)
    {
        output << pData->lastName << " "
               << pData->firstName << " "
               << pData->ssn << "\n";
    }

    output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main()
{
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
// //Written By: Gabriel Kret
// ECE-264: DSA I
// Professor Sable

// Additional libraries needed not included in presupplied code template
#include <array>
#include <functional>
#include <map>
#include <unordered_map>

// This is a comparator function used to relate SSN's only when the names are identical (when there are two identical names after one another in T1, T2, or T3). This can also be used for T4, although it is not in my code
bool CompSSNs(const Data *person1, const Data *person2)
{
    if (person1->ssn > person2->ssn)
        return false;
    else
        return true;
}
// Full comparator used exclusively for T3.
//  The choice was made to use a "full comparator" for T3 becuase the data is already mostly sorted and the sort will be finished using the builtin l.sort
auto compAll = [](const Data *person1, const Data *person2)
{
    int cped = person1->lastName.compare(person2->lastName);
    if (cped != 0)
    {
        return cped < 0;
    }
    cped = person1->firstName.compare(person2->firstName);
    if (cped != 0)
    {
        return cped < 0;
    }
    // If both the last names and first names are identical, compare the social security numbers (SSNs)
    return person1->ssn < person2->ssn;
};

// Here are the array of names generated and formatted outside this code file. These arrays are used to generate the Unordered Maps mapping each name to a vector holding a pointer to a data object.
//  There exist 500 unique last names in the dataset
const string LNames[500] = {"ACOSTA", "ADAMS", "ADKINS", "AGUILAR", "AGUIRRE", "ALEXANDER", "ALLEN", "ALVARADO", "ALVAREZ", "ANDERSON", "ANDREWS", "ARMSTRONG", "ARNOLD", "AUSTIN", "AVILA", "AYALA", "BAILEY", "BAKER", "BALDWIN", "BANKS", "BARBER", "BARKER", "BARNES", "BARNETT", "BARRETT", "BARTON", "BATES", "BECK", "BECKER", "BELL", "BENNETT", "BENSON", "BERRY", "BISHOP", "BLACK", "BLAIR", "BLAKE", "BOWEN", "BOWMAN", "BOYD", "BRADLEY", "BRADY", "BREWER", "BROOKS", "BROWN", "BRYANT", "BURGESS", "BURKE", "BURNS", "BURTON", "BUSH", "BUTLER", "BYRD", "CABRERA", "CALDERON", "CALDWELL", "CAMACHO", "CAMPBELL", "CAMPOS", "CANNON", "CARDENAS", "CARLSON", "CARPENTER", "CARR", "CARRILLO", "CARROLL", "CARTER", "CASTANEDA", "CASTILLO", "CASTRO", "CERVANTES", "CHAMBERS", "CHAN", "CHANDLER", "CHANG", "CHAPMAN", "CHAVEZ", "CHEN", "CHRISTENSEN", "CLARK", "CLARKE", "COHEN", "COLE", "COLEMAN", "COLLINS", "COLON", "CONTRERAS", "COOK", "COOPER", "CORTEZ", "COX", "CRAIG", "CRAWFORD", "CROSS", "CRUZ", "CUMMINGS", "CUNNINGHAM", "CURRY", "CURTIS", "DANIEL", "DANIELS", "DAVIDSON", "DAVIS", "DAWSON", "DAY", "DEAN", "DELACRUZ", "DELEON", "DELGADO", "DENNIS", "DIAZ", "DIXON", "DOMINGUEZ", "DOUGLAS", "DOYLE", "DUNCAN", "DUNN", "DURAN", "EDWARDS", "ELLIOTT", "ELLIS", "ERICKSON", "ESPINOZA", "ESTRADA", "EVANS", "FARMER", "FERGUSON", "FERNANDEZ", "FIELDS", "FIGUEROA", "FISCHER", "FISHER", "FITZGERALD", "FLEMING", "FLETCHER", "FLORES", "FORD", "FOSTER", "FOWLER", "FOX", "FRANCIS", "FRANCO", "FRANK", "FRANKLIN", "FRAZIER", "FREEMAN", "FUENTES", "FULLER", "GALLAGHER", "GALLEGOS", "GARCIA", "GARDNER", "GARNER", "GARRETT", "GARZA", "GEORGE", "GIBSON", "GILBERT", "GILL", "GOMEZ", "GONZALES", "GONZALEZ", "GOODMAN", "GOODWIN", "GORDON", "GRAHAM", "GRANT", "GRAVES", "GRAY", "GREEN", "GREENE", "GREGORY", "GRIFFIN", "GRIFFITH", "GROSS", "GUERRA", "GUERRERO", "GUTIERREZ", "GUZMAN", "HAIL", "HALE", "HALL", "HAMILTON", "HAMMOND", "HAMPTON", "HANSEN", "HANSON", "HARDY", "HARMON", "HARPER", "HARRINGTON", "HARRIS", "HARRISON", "HART", "HARVEY", "HAWKINS", "HAYES", "HAYNES", "HENDERSON", "HENRY", "HERNANDEZ", "HERRERA", "HICKS", "HIGGINS", "HILL", "HINES", "HODGES", "HOFFMAN", "HOLLAND", "HOLMES", "HOLT", "HOPKINS", "HORTON", "HOWARD", "HOWELL", "HUANG", "HUBBARD", "HUDSON", "HUGHES", "HUNT", "HUNTER", "INGRAM", "JACKSON", "JACOBS", "JAMES", "JENKINS", "JENNINGS", "JENSEN", "JIMENEZ", "JOHNSON", "JOHNSTON", "JONES", "JORDAN", "JOSEPH", "JUAREZ", "KELLER", "KELLEY", "KELLY", "KENNEDY", "KHAN", "KIM", "KING", "KLEIN", "KNIGHT", "LAMBERT", "LANE", "LARA", "LARSON", "LAWRENCE", "LAWSON", "LE", "LEE", "LEON", "LEONARD", "LEWIS", "LI", "LIN", "LITTLE", "LIU", "LOGAN", "LONG", "LOPEZ", "LOVE", "LOWE", "LUCAS", "LUNA", "LYNCH", "LYONS", "MACK", "MALDONADO", "MALONE", "MANN", "MANNING", "MARQUEZ", "MARSHALL", "MARTIN", "MARTINEZ", "MASON", "MATTHEWS", "MAXWELL", "MAY", "MCCARTHY", "MCCOY", "MCDANIEL", "MCDONALD", "MCGEE", "MCKINNEY", "MCLAUGHLIN", "MEDINA", "MEJIA", "MENDEZ", "MENDOZA", "MEYER", "MILES", "MILLER", "MILLS", "MIRANDA", "MITCHELL", "MOLINA", "MONTGOMERY", "MONTOYA", "MOORE", "MORALES", "MORAN", "MORENO", "MORGAN", "MORRIS", "MORRISON", "MOSS", "MULLINS", "MUNOZ", "MURPHY", "MURRAY", "MYERS", "NAVARRO", "NEAL", "NELSON", "NEWMAN", "NEWTON", "NGUYEN", "NICHOLS", "NORMAN", "NORRIS", "NUNEZ", "OBRIEN", "OCHOA", "OCONNOR", "OLIVER", "OLSON", "ORTEGA", "ORTIZ", "OWENS", "PACHECO", "PADILLA", "PAGE", "PALMER", "PARK", "PARKER", "PARKS", "PARSONS", "PATEL", "PATTERSON", "PAUL", "PAYNE", "PEARSON", "PENA", "PEREZ", "PERKINS", "PERRY", "PERSON", "PETERS", "PETERSON", "PHAM", "PHILLIPS", "PIERCE", "PORTER", "POTTER", "POWELL", "POWERS", "PRICE", "QUINN", "RAMIREZ", "RAMOS", "RAMSEY", "RAY", "REED", "REESE", "REEVES", "REID", "REYES", "REYNOLDS", "RHODES", "RICE", "RICHARDS", "RICHARDSON", "RILEY", "RIOS", "RIVAS", "RIVERA", "ROBBINS", "ROBERTS", "ROBERTSON", "ROBINSON", "ROBLES", "RODGERS", "RODRIGUEZ", "ROGERS", "ROJAS", "ROMAN", "ROMERO", "ROSALES", "ROSE", "ROSS", "ROWE", "RUIZ", "RUSSELL", "RYAN", "SALAZAR", "SALINAS", "SANCHEZ", "SANDERS", "SANDOVAL", "SANTIAGO", "SANTOS", "SAUNDERS", "SCHMIDT", "SCHNEIDER", "SCHROEDER", "SCHULTZ", "SCHWARTZ", "SCOTT", "SERRANO", "SHARP", "SHAW", "SHELTON", "SHERMAN", "SILVA", "SIMMONS", "SIMON", "SIMPSON", "SIMS", "SINGH", "SMITH", "SNYDER", "SOLIS", "SOTO", "SPENCER", "STANLEY", "STEELE", "STEPHENS", "STEVENS", "STEVENSON", "STEWART", "STONE", "STRICKLAND", "SULLIVAN", "SUTTON", "SWANSON", "TATE", "TAYLOR", "TERRY", "THOMAS", "THOMPSON", "THORNTON", "TODD", "TORRES", "TOWNSEND", "TRAN", "TRUJILLO", "TUCKER", "TURNER", "VALDEZ", "VALENCIA", "VARGAS", "VASQUEZ", "VAUGHN", "VAZQUEZ", "VEGA", "VELASQUEZ", "WADE", "WAGNER", "WALKER", "WALLACE", "WALSH", "WALTERS", "WALTON", "WANG", "WARD", "WARNER", "WARREN", "WASHINGTON", "WATERS", "WATKINS", "WATSON", "WATTS", "WEAVER", "WEBB", "WEBER", "WEBSTER", "WELCH", "WELLS", "WEST", "WHEELER", "WHITE", "WILLIAMS", "WILLIAMSON", "WILLIS", "WILSON", "WISE", "WOLF", "WOLFE", "WONG", "WOOD", "WOODS", "WRIGHT", "WU", "YANG", "YOUNG", "ZHANG", "ZIMMERMAN"};
// There given dataset included 500 first names. However, these duplicates were removed outside the scope of this code, prior to inputting them to the array. There are 494 unique first names as a result.
const string FNames[500] = {"AALIYAH", "AARON", "ABEL", "ABIGAIL", "ABRAHAM", "ADALINE", "ADALYN", "ADALYNN", "ADAM", "ADDISON", "ADELINE", "ADELYN", "ADRIAN", "ADRIANA", "AIDAN", "AIDEN", "ALAINA", "ALAN", "ALANA", "ALAYNA", "ALEJANDRO", "ALEX", "ALEXA", "ALEXANDER", "ALEXANDRA", "ALEXIS", "ALICE", "ALINA", "ALIVIA", "ALIYAH", "ALLISON", "ALYSSA", "AMARA", "AMAYA", "AMELIA", "AMIR", "AMY", "ANA", "ANASTASIA", "ANDREA", "ANDRES", "ANDREW", "ANGEL", "ANGELA", "ANGELINA", "ANNA", "ANNABELLE", "ANTHONY", "ANTONIO", "ARABELLA", "ARIA", "ARIANA", "ARIANNA", "ARIEL", "ARTHUR", "ARYA", "ASHER", "ASHLEY", "ASHTON", "ATHENA", "AUBREE", "AUBREY", "AUDREY", "AUGUST", "AURORA", "AUSTIN", "AUTUMN", "AVA", "AVERY", "AXEL", "AYDEN", "AYLA", "BAILEY", "BARRETT", "BEAU", "BECKETT", "BELLA", "BENJAMIN", "BENNETT", "BENTLEY", "BLAKE", "BRADLEY", "BRADY", "BRANDON", "BRANTLEY", "BRAXTON", "BRAYDEN", "BRIAN", "BRIANNA", "BRIELLE", "BRODY", "BROOKE", "BROOKLYN", "BROOKLYNN", "BROOKS", "BRYAN", "BRYCE", "BRYNLEE", "BRYSON", "CADEN", "CALEB", "CALLIE", "CALVIN", "CAMDEN", "CAMERON", "CAMILA", "CARLOS", "CAROLINE", "CARSON", "CARTER", "CATHERINE", "CAYDEN", "CECILIA", "CHARLES", "CHARLIE", "CHARLOTTE", "CHASE", "CHLOE", "CHRISTIAN", "CHRISTOPHER", "CLAIRE", "CLARA", "CLAYTON", "COLE", "COLIN", "COLTON", "CONNOR", "COOPER", "CORA", "DAISY", "DAKOTA", "DALEYZA", "DAMIAN", "DANIEL", "DANIELA", "DAVID", "DAWSON", "DEAN", "DECLAN", "DELANEY", "DELILAH", "DEREK", "DESTINY", "DIANA", "DIEGO", "DOMINIC", "DYLAN", "EASTON", "EDEN", "EDWARD", "ELEANOR", "ELENA", "ELI", "ELIANA", "ELIAS", "ELIJAH", "ELISE", "ELIZA", "ELIZABETH", "ELLA", "ELLIANA", "ELLIE", "ELLIOT", "ELLIOTT", "ELOISE", "EMERSON", "EMERSYN", "EMERY", "EMILIA", "EMILIANO", "EMILY", "EMMA", "EMMANUEL", "EMMETT", "ERIC", "ESTHER", "ETHAN", "EVA", "EVAN", "EVELYN", "EVERETT", "EVERLY", "EZEKIEL", "EZRA", "FAITH", "FELIX", "FINLEY", "FINN", "FIONA", "GABRIEL", "GABRIELLA", "GAEL", "GAVIN", "GENESIS", "GENEVIEVE", "GEORGE", "GEORGIA", "GIANNA", "GIOVANNI", "GRACE", "GRACIE", "GRAHAM", "GRANT", "GRAYSON", "GREYSON", "GRIFFIN", "HADLEY", "HAILEY", "HANNAH", "HARLEY", "HARMONY", "HARPER", "HARRISON", "HAYDEN", "HAZEL", "HENRY", "HOLDEN", "HUDSON", "HUNTER", "IAN", "IRIS", "ISAAC", "ISABEL", "ISABELLA", "ISABELLE", "ISAIAH", "ISLA", "ISRAEL", "IVAN", "IVY", "JACE", "JACK", "JACKSON", "JACOB", "JADE", "JADEN", "JAKE", "JAMES", "JAMESON", "JASMINE", "JASON", "JASPER", "JAVIER", "JAX", "JAXON", "JAXSON", "JAYCE", "JAYDEN", "JAYLA", "JEREMIAH", "JEREMY", "JESSE", "JESSICA", "JESUS", "JOANNA", "JOCELYN", "JOEL", "JOHN", "JONAH", "JONATHAN", "JORDAN", "JORDYN", "JORGE", "JOSE", "JOSEPH", "JOSEPHINE", "JOSHUA", "JOSIAH", "JOSIE", "JOSUE", "JUAN", "JUDAH", "JUDE", "JULIA", "JULIAN", "JULIANA", "JULIANNA", "JULIET", "JULIETTE", "JUNE", "JUSTIN", "KADEN", "KAI", "KAIDEN", "KALEB", "KARTER", "KATHERINE", "KAYDEN", "KAYLA", "KAYLEE", "KENDALL", "KENNEDY", "KENNETH", "KEVIN", "KHLOE", "KILLIAN", "KIMBERLY", "KING", "KINGSTON", "KINSLEY", "KNOX", "KYLE", "KYLIE", "KYRIE", "LAILA", "LANDON", "LAUREN", "LAYLA", "LEAH", "LEILA", "LEILANI", "LEO", "LEON", "LEONARDO", "LEVI", "LIAM", "LILA", "LILIANA", "LILLIAN", "LILLY", "LILY", "LINCOLN", "LOGAN", "LOLA", "LONDON", "LONDYN", "LORENZO", "LUCA", "LUCAS", "LUCIA", "LUCY", "LUIS", "LUKAS", "LUKE", "LUNA", "LYDIA", "LYLA", "MACKENZIE", "MADDOX", "MADELINE", "MADELYN", "MADISON", "MAGGIE", "MAKAYLA", "MALACHI", "MALIA", "MARCUS", "MARGARET", "MARIA", "MARIAH", "MARK", "MARLEY", "MARY", "MASON", "MATEO", "MATIAS", "MATTEO", "MATTHEW", "MAVERICK", "MAX", "MAXIMUS", "MAXWELL", "MAYA", "MCKENZIE", "MELANIE", "MELODY", "MESSIAH", "MIA", "MICAH", "MICHAEL", "MICHELLE", "MIGUEL", "MILA", "MILES", "MILO", "MOLLY", "MORGAN", "MYA", "MYLES", "NAOMI", "NATALIA", "NATALIE", "NATHAN", "NATHANIEL", "NEVAEH", "NICHOLAS", "NICOLAS", "NICOLE", "NOAH", "NOELLE", "NOLAN", "NORA", "NORAH", "NOVA", "OLIVER", "OLIVIA", "OMAR", "OSCAR", "OWEN", "PAIGE", "PAISLEY", "PARKER", "PATRICK", "PAUL", "PAXTON", "PAYTON", "PENELOPE", "PETER", "PEYTON", "PIPER", "PRESLEY", "PRESTON", "QUINN", "RACHEL", "RAELYNN", "REAGAN", "REBECCA", "REESE", "REMI", "REMINGTON", "RHETT", "RICHARD", "RILEY", "RIVER", "ROBERT", "ROMAN", "ROSALIE", "ROSE", "ROWAN", "RUBY", "RYAN", "RYDER", "RYKER", "RYLEE", "RYLEIGH", "SADIE", "SAMANTHA", "SAMUEL", "SANTIAGO", "SARA", "SARAH", "SAVANNAH", "SAWYER", "SCARLETT", "SEBASTIAN", "SELENA", "SERENITY", "SIENNA", "SILAS", "SKYLAR", "SLOANE", "SOFIA", "SOPHIA", "SOPHIE", "STELLA", "STEVEN", "SUMMER", "SYDNEY", "TAYLOR", "TEAGAN", "TESSA", "THEODORE", "THIAGO", "THOMAS", "TIMOTHY", "TRINITY", "TRISTAN", "TUCKER", "TYLER", "VALENTINA", "VALERIA", "VALERIE", "VANESSA", "VICTOR", "VICTORIA", "VINCENT", "VIOLET", "VIVIAN", "WAYLON", "WESLEY", "WESTON", "WILLIAM", "WILLOW", "WYATT", "XANDER", "XAVIER", "XIMENA", "ZACHARY", "ZANDER", "ZANE", "ZAYDEN", "ZION", "ZOE", "ZOEY"};

// This array will hold data relating to when two people have the same first and last name. If two or more people have the same first and last name, they will "receive" an additional sort by SSN using the CompSSN function
Data EqualFirstLastName[500];

// Function to perform insertion sort on a list of Data pointers. Even though an insertion sort is worst-case quadratic, it is good to use for T3 since the dataset is mostly sorted
void insertionSort(list<Data *> &l)
{
    // Start from the second element (as the first element is considered sorted)
    for (auto i = ++l.begin(); i != l.end(); ++i)
    {
        // Store the current element
        auto current = *i;
        // Initialize the comparison pointer to the element before the current element
        auto j = i;
        j--;

        // While we haven't reached the beginning of the list and the current element is smaller than the previous
        while (j != l.begin() && compAll(current, *j))
        {
            // Swap the current and previous elements
            auto temp = *j;
            *j = *i;
            *i = temp;
            // Move the pointers one step back
            j--;
            i--;
        }

        // If we've reached the beginning of the list and the current element is smaller than the first
        if (j == l.begin() && compAll(current, *j))
        {
            // Swap the current and first elements
            auto temp = *j;
            *j = *i;
            *i = temp;
        }
    }
}

void sortDataList(list<Data *> &l)
{ // Initialization of the iterators neede to determine if the Dataset is T1, T2, T3, or T4
    list<Data *>::iterator itBack;
    list<Data *>::iterator itNext;

    itNext = l.begin();
    itNext++;

    itBack = l.end();
    itBack--;

    // Sort T4
    if ((*l.begin())->lastName == (*itBack)->lastName)
    { // This is a sort for T4 that uses a simple comparator of SSNs. Since all the names are identical, they can be ignored. The choice was made to use l.sort even though it may not be as fast as a Radix or Bucket sort.
        
        l.sort([](const Data *person1, const Data *person2){ return person1->ssn < person2->ssn; });
        return;
    }
    // Sort T3
    else if ((*l.begin())->lastName == (*itNext)->lastName)
    { // This is an insertion sort used for T3. An insertion sort was chosen becuase the data is already mostly sorted and simply needs to reorganize the SSN's
        insertionSort(l);
        return;
    }

    // Sort T1, T2
    // The decision was made to use Hash Maps to sort T1 and T2
    else
    { // Unordered map declaration for Hash Maps
        unordered_map<string, vector<Data *>> hashMapFirstNames = {};
        unordered_map<string, vector<Data *>> hashMapLastNames = {};

        // for loop assigns an empty vector to each name and adds the name and vector to the respective Hash Maps
        for (int k = 0; k < 500; k++)
        {
            if (k < 494)
            { // emplace function extends the container by inserting a new element at the position
                hashMapFirstNames.emplace(FNames[k], vector<Data *>());
            }
            hashMapLastNames.emplace(LNames[k], vector<Data *>());
        }
        // The next portion of code "finalizes" the Hash Map so the data can be sorted
        //  iterates over a list of pointers to objects and stores them in a Hash Map according to their first name attribute.
        for (auto i = l.begin(); i != l.end(); i++)
        {
            hashMapFirstNames[(*i)->firstName].push_back(*i);
        }

        // iterates over a list of pointers to objects and stores them in the second Hash Map according to their last name attribute.
        for (auto i = 0; i < 500; i++)
        {
            auto VectorMapFirstN = hashMapFirstNames[FNames[i]];
            for (auto j = VectorMapFirstN.begin(); j != VectorMapFirstN.end(); j++)
            {
                hashMapLastNames[(*j)->lastName].push_back(*j);
            }
        }
        auto L_Increment = l.begin();
        // Iterating through all the Last Names in the Map of Last Names
        for (int i = 0; i < 500; i++)
        {
            auto VectorMapLastN = hashMapLastNames[LNames[i]];

            int z = VectorMapLastN.size() - 1;
            for (int j = 0; j <= z; j++)
            {
                // Loop until j reaches z or the next Data object has a different first name than the current one
                if (j != z && VectorMapLastN[j]->firstName == VectorMapLastN[j + 1]->firstName)
                {
                    Data *EqualFirstLastName[500];
                    int loc = 0;
                    int k = j;
                    // Store the pointer to the next Data object in the array
                    EqualFirstLastName[loc] = VectorMapLastN[j];
                    loc++;
                    // This while loop stores the pointers to the Data objects that have the same first and last name as the current one in the array EqualFirstLastName.
                    // It also updates the variables j and location accordingly.
                    while (j < z && VectorMapLastN[j + 1]->firstName == VectorMapLastN[k]->firstName)
                    {
                        j++;
                        EqualFirstLastName[loc] = VectorMapLastN[j];
                        loc++;
                    }
                    // Sort the array of pointers by their social security numbers using the CompSSNs function
                    sort(EqualFirstLastName, &EqualFirstLastName[loc], CompSSNs);
                    // This for loop adds the sorted Data objects with the same first and last name to the list l.
                    for (int w = 0; w < loc; w++)
                    {
                        // Dereference the L_Increment pointer and assign it the value of the w-th element in the array
                        *L_Increment = EqualFirstLastName[w];
                        L_Increment++;
                    }
                }
                // If the first name of the current Data object is not the same as the next Data object

                // Dereference the L_Increment pointer and assign it the value of the current Data object
                else
                {
                    *L_Increment = VectorMapLastN[j];
                    L_Increment++;
                }
            }
        }
    }
}