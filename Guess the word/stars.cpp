//
//  stars.cpp
//  proj5_ruiyan
//
//  Created by Rui Yan on 10/11/2017.
//  Copyright © 2017 Rui Yan. All rights reserved.
//

#include "utilities.h"
#include <iostream>
#include <cctype>
#include <iostream>
#include <streambuf>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <random>
#include <utility>
#include <cstring>
#include <cstdlib>

using namespace std;

const char WORDFILENAME[] = "/Users/ruiyan/Desktop/twowords.txt";
const int MAXWORDS = 9000;
const int MAXCHARS = 99;

double average(int scores[], int numOfScores);
int findmin(int scores[], int numOfScores);
int findmax(int scores[], int numOfScores);
bool contains(const char words[][MAXWORDLEN+1], char w[MAXWORDLEN+1], int NUMWORDS);
bool contains_int(int index[MAXWORDLEN+1], int num, int size);
int xxxrunOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum);
int runOneRound(const char[][7], int, int);

//*************************************
//   main
//*************************************
// The main routine declares an array of c strings that hold words
// from which the secret word will be selected. It determines whether
// to start a round and calls runOneRound numOfRounds time. In addition,
// it writes out the rounf number scores, average, min, max and
// lenOfSecret each round.

int xxxmain()
{
    char words[MAXWORDS][MAXWORDLEN+1];
    // numOfWords is the actual number of words in the array
    int numOfWords = getWords(words, MAXWORDS, WORDFILENAME);
    // write error and terminate if path is invalid or no words in the file
    if (numOfWords < 1){
        cout << "No words were loaded, so I can't play the game." << endl;
        return -1;
    }
    // cin the numOfRounds
    cout << "How many rounds do you want to play? ";
    int numOfRounds;
    cin >> numOfRounds;
    cin.ignore(10000,'\n');
    // write error message and terminate when numOfRounds is not positive
    if (numOfRounds <= 0){
        cout << "The number of rounds must be positive." << endl;
        return -1;
    }
    // int array scores to store the scores in each round
    int *scores = new int[numOfRounds];
    
    // Run one round
    int wordnum;
    for (int i = 1; i <= numOfRounds; i++){
        // Pick the secret word
        wordnum = randInt(0, numOfWords-1);
        // Pick the (wordnum)th word in 'words' array
        char secretWord[MAXWORDLEN+1];
        strcpy(secretWord, words[wordnum]);
        // Output round number and the length of secret word
        cout << '\n' << "Round " << i << endl;
        int lenOfSecret = static_cast<int>(strlen(secretWord));
        cout << "The secret word is " << lenOfSecret << " letters long." << endl;
        // Get the score in ith round
        int score = runOneRound(words, numOfWords, wordnum);
        // Cout the score at the end of each round
        if (score == 1)
            cout << "You got it in 1 try." << endl;
        else cout << "You got it in " << score << " tries." << endl;
        // Store the ith score at (i-1)th index in scores array
        scores[i-1] = score;
        
        double avg;
        cout.setf(ios::fixed); //set the number be rounded to 0.01
        cout.precision(2);
        avg = average(scores, i);    // find the average score
        int min = findmin(scores, i);// find the min score in the first ith round
        int max = findmax(scores, i);// find the max score in the first ith round
        cout << "Average: " << avg << ", minimum: " << min << ", maximum: " << max << endl;
    }
    delete[] scores;
    return 0;
}

//*************************************
//   average
//*************************************
// This function returns the average score of numOfScores
// scores stored in scores array.
double average(int scores[], int numOfScores){
    double addition = 0.00;
    for (int i = 0; i < numOfScores; i++){
        addition += scores[i];
    }
    return addition/numOfScores;
}

//*************************************
//   findmin
// ************************************
// This function returns the min score of numOfScores
// scores stored in scores array.
int findmin(int scores[], int numOfScores){
    int min = scores[0];
    for (int i = 0; i < numOfScores; i++){
        if(scores[i] < min)
            min = scores[i];
    }
    return min;
}

//*************************************
//   findmax
// ************************************
// This function returns the max score of numOfScores
// scores stored in scores array.
int findmax(int scores[], int numOfScores){
    int max = scores[0];
    for (int i = 0; i < numOfScores; i++){
        if(scores[i] > max)
            max = scores[i];
    }
    return max;
}

//*************************************
//   contains
// ************************************
// This function returns the boolean to determine whether
// the cstring array "words" contains the cstring "w".
bool contains(const char words[][MAXWORDLEN+1], char w[MAXWORDLEN+1], int NUMWORDS){
    for (int k = 0; k < NUMWORDS; k++){
        if(strcmp(w, words[k]) == 0)
            return true;
    }
    return false;
}

//*************************************
//   contains_int
// ************************************
// This function returns the boolean to determine whether
// the int array "index" contains the int "num".
bool contains_int(int index[MAXWORDLEN+1], int num, int size){
    for (int k = 0; k < size; k++){
        if(num == index[k]){
            return true;
        }
    }
    return false;
}

//*************************************
//   runOneRound
// ************************************
// Using words[wordnum] as the secret word, this function plays one
// round of the game. It returns the score for that round.
int xxxrunOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum){
    // return -1 if nwords is not positive, or when wordnum < 0 or >= nwords
    if (nWords <= 0 || wordnum < 0 || wordnum >= nWords)
        return -1;
    
    bool win = false; // boolean to decide whether win or not
    int sc = 1; // score counter
    int index_star[MAXWORDLEN+1];  // int array to store the pos of stars in probe and secret word
    int index_planet[MAXWORDLEN+1];// int array to store the pos of planets in probe word
    
    // store the secret word in cstring secret
    char secret[MAXWORDLEN+1] = "";
    strcpy(secret,words[wordnum]);
    
    // while loop to prompt and count stars and planets until win (a round ends)
    while(!win){
        // ASK PROBE WORD
        cout << "Probe word: ";
        char probe[MAXCHARS+1] = "";
        cin.getline(probe, MAXCHARS+1);
        
        // CHECK THE VALIDITY OF PROBE WORD
        int lenOfProbe;
        lenOfProbe = static_cast<int>(strlen(probe));
        bool lower = true;
        if (lenOfProbe < MINWORDLEN || lenOfProbe > MAXWORDLEN){
            cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
            continue;
        } else {
            for (int k = 0; probe[k]!='\0'; k++){
                if(!islower(probe[k])){
                    lower = false;
                }
            }
            if(!lower){
                cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
                continue;
            }
        }
        if(lower && lenOfProbe >=MINWORDLEN && lenOfProbe <= MAXWORDLEN){
            if(!contains(words, probe, nWords)){
                cout << "I don't know that word." << endl;
                continue;
            }
        }
        
        // COUNT THE NUMBER OF STARS AND PLANETS
        int stars = 0;
        int planets = 0;
        int k = 0; // index of array (index_star)
        int n = 0; // index of array (index_planet)
        // process the stars
        for(int i = 0; probe[i] != '\0'; i++){
            if(probe[i] == secret[i]){
                stars += 1;
                index_star[k] = i;
                k += 1;
            }
        }
        // process the planets
        for(int i=0; secret[i] != '\0'; i++){
            for(int j=0; probe[j] != '\0'; j++){
                if((!contains_int(index_star, j, k)) && (!contains_int(index_star, i, k))
                   && (!contains_int(index_planet, j, n))){
                    if(secret[i] == probe[j] && i != j){
                        planets += 1;
                        index_planet[n] = j;
                        n += 1;
                        break;
                   }
                }
            }
        }
        // DETERMINE WIN OR NOT
        int lenOfSecret;
        lenOfSecret = static_cast<int>(strlen(secret));
        if (stars == lenOfSecret && planets == 0){
            win = true; // if win, return sc
            return sc;
        }
        // PRINT THE NUMBER OF STARS AND PLANETS
        cout << "Stars: " << stars << ", Planets: " << planets << endl;
        // INCREMENT SC BY 1
        sc += 1;
    }
    return sc;
}


class TooMuchOutput {};

class limitoutputstringbuf : public stringbuf
{
public:
    limitoutputstringbuf(streamsize max)
    : m_left(max)
    {}
    int_type overflow(int_type c = traits_type::eof())
    {
        if (m_left == 0)
            throw TooMuchOutput();
        m_left--;
        return stringbuf::overflow(c);
    }
private:
    streamsize m_left;
};

class streambuf_switcher
{
public:
    streambuf_switcher(ios& dest, ios& src)
    : streambuf_switcher(dest, src.rdbuf())
    {}
    streambuf_switcher(ios& dest, streambuf* src)
    : dest_stream(dest), saved_streambuf(dest.rdbuf(src))
    {}
    ~streambuf_switcher()
    {
        dest_stream.rdbuf(saved_streambuf);
    }
private:
    ios& dest_stream;
    streambuf* saved_streambuf;
};

int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static mt19937 generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

int xxxrunOneRound(const char [][7], int, int);

using RORtype = function<int(const char[][7], int, int)>;
RORtype fffrunOneRound = xxxrunOneRound;
const initializer_list<const char*>* initWords;

int getWords(char words[][7], int maxWords, const char [])
{
    if (maxWords < 1)
        return maxWords;
    int num = 0;
    for (auto w : *initWords)
    {
        if (num == maxWords)
            break;
        strcpy(words[num], w);
        num++;
    }
    return num;
}

void testmain(const initializer_list<const char*>& words, const char* input, const RORtype& mor)
{
    initWords = &words;
    istringstream iss(input);
    streambuf_switcher ssi(cin, iss);
    fffrunOneRound = mor;
    xxxmain();
}

int runOneRound(const char words[][7], int num, int wordnum)
{
    return fffrunOneRound(words, num, wordnum);
}

void testrunOneRound(const initializer_list<const char*>& words,
                     const char* input, int wordnum, int& score, string& output,
                     bool testBadWordnum = false)
{
    initWords = &words;
    istringstream iss(input);
    streambuf_switcher ssi(cin, iss);
    limitoutputstringbuf losb(1000);
    streambuf_switcher sso(cout, &losb);
    char wordList[9000][7];
    int nWords = getWords(wordList, 10, "");
    if (testBadWordnum)
        nWords = wordnum;
    score = xxxrunOneRound(wordList, nWords, wordnum);
    output = losb.str();
}

void testone(int n)
{
    int score;
    string output;
    
    switch (n)
    {
        default: {
            return;
        } break; case 1: {      // 0 rounds
            testmain({ "tiger", "heron" }, "0\n1\nheron\n",
                     [](const char [][7], int, int)->int
                     { throw 1; });
        } break; case 2: {      // got it in 1 try
            testmain({ "tiger", "heron" }, "1\n",
                     [](const char [][7], int, int)->int
                     { return 1; });
        } break; case 3: {      // stats for one game
            testmain({ "tiger", "heron" }, "1\n",
                     [](const char [][7], int, int)->int
                     { return 42; });
        } break; case 4: {      // stats for several games
            testmain({ "tiger", "heron" }, "4\n",
                     [](const char [][7], int, int)->int
                     {
                         static const size_t NUM_SCORES = 4;
                         static int scores[NUM_SCORES] = { 4, 6, 3, 8 };
                         static int r = NUM_SCORES - 1;
                         if (++r == NUM_SCORES)
                             r = 0;
                         return scores[r];
                     });
        } break; case 5: {      // random numbers used
            static int counts[10] = { 0 };
            static int errors = 0;
            {
                limitoutputstringbuf losb(1000);
                streambuf_switcher sso(cout, &losb);
                testmain({ "tiger", "heron", "okapi", "ocelot", "myna",
                    "hyrax", "bison", "squid", "kudu", "pika" },
                         "1000\n", [](const char [][7], int, int wordnum)->int
                         {
                             if (wordnum >= 0  &&  wordnum < 10)
                                 counts[wordnum]++;
                             else
                                 errors++;
                             return 1;
                         });
            }
            if (errors > 0)
                cout << "ERRORS: " << errors << endl;
            if (none_of(counts, counts+10,
                        [](int k) { return k-100 < -33  ||  k-100 > 33; }))
                cout << "CORRECT" << endl;
            else
            {
                for (int k = 0; k < 10; k++)
                    cout << counts[k] << " ";
            }
        } break; case 6: {      // bad arg to runOneRound
            testrunOneRound({ "tiger", "heron", "heron" }, "heron\n", 2,
                            score, output, true);
            cout << (score == -1 && output.empty() ? "CORRECT" : "WRONG") << endl;
        } break; case 7: {      // last word OK
            testrunOneRound({ "tiger", "heron" }, "heron\n", 1,
                            score, output);
            cout << (score == 1 ? "CORRECT" : "WRONG") << endl;
        } break; case 8: {      // guessed in one try
            testrunOneRound({ "tiger", "heron", "myna" }, "heron\n", 1, score, output);
            cout << (score == 1 ? "CORRECT" : "WRONG") << endl;
        } break; case 9: {      // guessed in two tries
            testrunOneRound({ "tiger", "heron", "myna" }, "tiger\nheron\n", 1, score, output);
            cout << (score == 2 ? "CORRECT" : "WRONG") << endl;
        } break; case 10: {     // errors don't count as guesses
            testrunOneRound({ "tiger", "heron", "myna" }, "he@on\nokapi\nheron\n", 1, score, output);
            cout << (score == 1 ? "CORRECT" : "WRONG") << endl;
        } break; case 11: {     // invalid characters in probe word
            testrunOneRound({ "tiger", "heron", "myna" }, "he@on\nheron\n", 1, score, output);
            cout << output;
        } break; case 12: {     // space character in probe word
            testrunOneRound({ "tiger", "heron", "myna", "he", "ron" }, "he ron\nheron\n", 1, score, output);
            cout << output;
        } break; case 13: {     // too few characters in probe word
            testrunOneRound({ "tiger", "heron", "myna" }, "yak\nheron\n", 1, score, output);
            cout << output;
        } break; case 14: {     // too many characters in probe word
            testrunOneRound({ "tiger", "heron", "myna" }, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\nheron\n", 1, score, output);
            cout << output;
        } break; case 15: {     // probe word not in list
            testrunOneRound({ "tiger", "heron", "myna" }, "okapi\nheron\n", 1, score, output);
            cout << output;
        } break; case 16: {     // stars/planets: 0/0
            testrunOneRound({ "hyrax", "bison", "myna" }, "hyrax\nbison\n", 1, score, output);
            cout << output;
        } break; case 17: {     // 1/0
            testrunOneRound({ "tiger", "bison", "myna" }, "tiger\nbison\n", 1, score, output);
            cout << output;
        } break; case 18: {     // 4/0
            testrunOneRound({ "mouse", "louse", "myna" }, "mouse\nlouse\n", 1, score, output);
            cout << output;
        } break; case 19: {     // 0/1
            testrunOneRound({ "tiger", "mouse", "myna" }, "tiger\nmouse\n", 1, score, output);
            cout << output;
        } break; case 20: {     // 0/4
            testrunOneRound({ "rhino", "minor", "myna" }, "rhino\nminor\n", 1, score, output);
            cout << output;
        } break; case 21: {     // 1/3
            testrunOneRound({ "horse", "shrew", "myna" }, "horse\nshrew\n", 1, score, output);
            cout << output;
        } break; case 22: {     // 0/5 not a win for 5 letter
            testrunOneRound({ "stare", "aster", "myna" }, "stare\naster\n", 1, score, output);
            cout << output;
        } break; case 23: {     // 1/2 more of a letter in secret word than probe
            testrunOneRound({ "sheep", "geese", "myna" }, "sheep\ngeese\n", 1, score, output);
            cout << output;
        } break; case 24: {     // 1/2 more of a letter in probe than secret word
            testrunOneRound({ "geese", "sheep", "myna" }, "geese\nsheep\n", 1, score, output);
            cout << output;
        } break; case 25: {     // 0/5 multiple letters
            testrunOneRound({ "emerge", "redeem", "myna" }, "emerge\nredeem\n", 1, score, output);
            cout << output;
        } break; case 26: {     // 1/2 probe 4, secret 5
            testrunOneRound({ "myna", "hyena", "pika" }, "myna\nhyena\n", 1, score, output);
            cout << output;
        } break; case 27: {     // 1/2 probe 5, secret 4
            testrunOneRound({ "hyena", "myna", "pika" }, "hyena\nmyna\n", 1, score, output);
            cout << output;
        } break; case 28: {     // 1/3 probe 4, secret 6
            testrunOneRound({ "bear", "badger", "myna" }, "bear\nbadger\n", 1, score, output);
            cout << output;
        } break; case 29: {     // 1/3 probe 6, secret 4
            testrunOneRound({ "badger", "bear", "myna" }, "badger\nbear\n", 1, score, output);
            cout << output;
        } break; case 30: {     // 5/0 probe 5, secret 6
            testrunOneRound({ "tiger", "tigers", "myna" }, "tiger\ntigers\n", 1, score, output);
            cout << output;
        } break; case 31: {     // 5/0 probe 6, secret 5
            testrunOneRound({ "tigers", "tiger", "myna" }, "tigers\ntiger\n", 1, score, output);
            cout << output;
        } break; case 32: {     // 2/2 probe 6, secret 4, repeats
            testrunOneRound({ "veer", "vervet", "myna" }, "veer\nvervet\n", 1, score, output);
            cout << output;
        } break; case 33: {     // 2/2 probe 4, secret 6, repeats
            testrunOneRound({ "vervet", "veer", "myna" }, "vervet\nveer\n", 1, score, output);
            cout << output;
        } break; case 34: {     // checking doesn't corrupt word
            int score2;
            string output2;
            testrunOneRound({ "llama", "halal", "myna" }, "llama\nllama\nhalal\n", 1, score, output);
            testrunOneRound({ "llama", "halal", "myna" }, "halal\nhalal\nllama\n", 0, score2, output2);
            cout << (score == 3 && score2 == 3 && output == output2 ? "CORRECT" : "WRONG");
        } break;
    }
}

int main()
{
    cout << "Enter test number (1-34): ";
    int n;
    cin >> n;
    string s;
    {
        limitoutputstringbuf losb(1000);
        streambuf_switcher sso(cout, &losb);
        
        try
        {
            testone(n);
            cout << "DONE" << endl;
        }
        catch(...)
        {
        }
        
        s = losb.str();
    }
    cout << s;
    if (!s.empty()  &&  s.back() != '\n')
        cout << endl;
}
