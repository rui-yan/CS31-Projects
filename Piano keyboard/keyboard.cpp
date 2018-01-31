//
//  keyboard.cpp
//  proj3_ruiyan
//
//  Created by Rui Yan on 20/10/2017.
//  Copyright © 2017 Rui Yan. All rights reserved.
//

#include <iostream>
#include <cassert>
#include <string>
#include <cctype>
using namespace std;

// function prototypes
bool isNoteLetter(char letter); // check whether it is a valid note letter "ABCDEFG"
bool isANote(string song, int& pos); // check a valid note, 4 syntaxs: X X# X#1 X1
bool isCorrectSlash(string song, int& pos); // check whether a slash is at right position
bool hasCorrectSyntax(string song); // check whether a song string has correct syntax
bool isPlayable(string song); // check whether a song is playable
char encodeNote(int octave, char noteLetter, char accidentalSign); // encode the note
char processNote(string song, int& pos); //process each note and return the encoded note
int encodeSong(string song, string& instructions, int& badBeat); //encode the song

// main function
int main()
{
    assert(hasCorrectSyntax("D5/"));
    assert(!hasCorrectSyntax("D5//Z/"));
    string instrs;
    int badb;
    instrs = "xxx"; badb = -999; // so we can detect whether these get changed
    assert(encodeSong("G3B3DD5//G/A/A3B/C5/B3D5//G//G//CE5//C5/D5/E5/F#5/B3G5//G//G/",
                     instrs, badb) == 0  &&  instrs == "[SFHR] LQ[DW]E[FR] L L [GT] ERT*[FU] L L"  &&  badb == -999);
    assert(encodeSong("D3/F#3/A3/D4//D3F#3A3D4/", instrs, badb) == 0  &&  instrs == ".8DH [.8DH]"  &&  badb == -999);
    assert(encodeSong("D5//B#1/", instrs, badb) == 0  &&  instrs == "R Z"  &&  badb == -999);
    assert(encodeSong("", instrs, badb) == 0  &&  instrs == ""  &&  badb == -999);
    assert(encodeSong("DADDA/", instrs, badb) == 0  &&  instrs == "[HQHHQ]"  &&  badb == -999);
    assert(encodeSong("A3C#E//E//F#3A3D4/A3C#E/", instrs, badb) == 0  &&  instrs == "[D!J] J [8DH][D!J]"  &&  badb == -999);
    assert(encodeSong("/", instrs, badb) == 0  &&  instrs == " "  &&  badb == -999);
    instrs = "xxx"; badb = -999; // so we can detect whether these get changed
    assert(encodeSong("D5//Z/B8///", instrs, badb) == 1  &&  instrs == "xxx"  &&  badb == -999);
    assert(encodeSong("d5//Z/B8///", instrs, badb) == 1  &&  instrs == "xxx"  &&  badb == -999);
    assert(encodeSong(")()(", instrs, badb) == 1  &&  instrs == "xxx"  &&  badb == -999);
    assert(encodeSong("/Cb6//D6/C2/", instrs, badb) == 2  &&  instrs == "xxx"  &&  badb == 4);
   // assert(encodeSong("D5//D8/", instrs, badb) == 2  &&  instrs == "xxx"  &&  badb == 3);
    
    cerr << "All tests succeeded" << endl;
}

//*************************************
//  isNoteLetter
//*************************************
// This function tests whether the passed-in letter
// is a note letter or not.
const string noteLetter = "ABCDEFG"; // valid note letters

bool isNoteLetter(char letter)
{
    for (int k = 0; k <= noteLetter.size(); k++)
    {
        if (letter == noteLetter[k])
            return true;
    }
    return false;
}

//*************************************
//  isANote
//*************************************
// This funciton tests whether the note starts with a note
// letter at song[pos] is a correct note or not. The correct
// note has four syntaxs. Only a note letter; A note letter
// immediately followed by an accidental sign; followed by a digit;
// or followed by an accidental sign immediately followed by a digit.

bool isANote(string song, int& pos)
{
    //Get distance. We know song[pos] is a note letter
    //The first character of a note is a note letter
    pos++;
    
    //We pass the note letter, so check the next character after it.
    //If ends, true
    if (isNoteLetter(song[pos]) || song[pos] == '/')
    {
        return true;
        // Else, if there is a accidental sign after the note letter
    } else if (song[pos] == '#' || song[pos] == 'b')
    {
        pos++; // check the next character
        // if ends, true
        if (isNoteLetter(song[pos]) || song[pos] == '/')
        {
            return true;
        // if digit, true
        } else if (isdigit(song[pos]))
        {
            pos++;
            // check the next character, if ends, true
            if (isNoteLetter(song[pos]) || song[pos] == '/')
                return true;
        }
        // Else, if there is a digit after the note letter
    } else if (isdigit(song[pos]))
    {
        pos++;// check the next character, if ends, true
        if (isNoteLetter(song[pos]) || song[pos] == '/')
            return true;
    }
    // If not true in all above cases, then false
    return false;
}

//*************************************
//  isCorrectSlash
//*************************************
// This function tests whether a slash is at right position
// so that the song has correct syntax.

bool isCorrectSlash(string song, int& pos)
{
    pos++;
    if(pos == song.size()) //if slash is the last character, then true
    {
        return true;
    } else if(song[pos] == '/' || isNoteLetter(song[pos])) {
        return true;
        //if after the slash is another slash or a note letter, then true
    }
    return false;
}

//*************************************
//  hasCorrectSyntax
//*************************************
// A song string is a sequence of zero or more beats. Every character
// in a non-empty song string must be part of a beat (so, for example,
// C/G is not a song string because the G is not part of a beat, since
// every beat must end with a slash).

// This function returns true if its parameter is a song string
// (i.e., it meets the definition above), and false otherwise;
bool hasCorrectSyntax(string song)
{
    int k = 0;
    if (song.empty())
        return true;
    
    if (song[song.size()-1] != '/')
        return false;
    
    while (k != song.size())
    {
        if (isNoteLetter(song[k]))
        {
            if(!isANote(song, k))
                return false;
        } else if (song[k] == '/') {
            if(!isCorrectSlash(song, k))
                return false;
        } else return false;
    }
    return true;
}

//*************************************
//  isPlayable
//*************************************
// The playable notes are
// (notes with no digit character. These are considered to be
// in octave 4, so D and D4 are equivalent, as are F# and F#4.)
// (notes with a digit character 2, 3, 4, or 5, except that Cb2
// is not a playable note. These notes are considered to be in
// the indicated octave number.)

// A playable song is a song string that does not contain any notes
// that are not playable.

bool isPlayable(string song) {
    bool playable = true;
    
    if (!hasCorrectSyntax(song))
        return false;
    
    for (int k = 0; k < song.size(); k++) {
        if (song[k] == '0' || song[k] == '1' || song[k] == '6' ||
            song[k] == '7' || song[k] == '8' || song[k] == '9'){
            if (k >= 2 && ((song[k-2] == 'C' && song[k-1] == 'b' && song[k] == '6')||
                           (song[k-2] == 'B' && song[k-1] == '#' && song[k] == '1'))){
                playable = true;
            } else if (k >= 1 && song[k-1] == 'C' && song[k] == '6') {
                playable = true;
            // if there is digit 0,1,6,7,8,9 except Cb6, C6, B#1 in the note, then not playable
            } else
                return false;
        } else if (k >= 2 && (song[k-2] == 'C' && song[k-1] == 'b' && song[k] == '2'))
                return false;
    }
    return playable;
}

//*************************************
//  encodeNote
//*************************************
// Given an octave number, a note letter, and an accidental sign, return
// the character that the indicated note is encoded as if it is playable.
// Return a space character if it is not playable.
//
// First parameter:   the octave number (the integer 4 is the number of the
//                    octave that starts with middle C, for example).
// Second parameter:  an upper case note letter, 'A' through 'G'
// Third parameter:   '#', 'b', or ' ' (meaning no accidental sign)
//
// Examples:  encodeNote(4, 'A', ' ') returns 'Q'
//            encodeNote(4, 'A', '#') returns '%'
//            encodeNote(4, 'H', ' ') returns ' '

char encodeNote(int octave, char noteLetter, char accidentalSign)
{
    // This check is here solely to report a common CS 31 student error.
    if (octave > 9)
    {
        cerr << "********** encodeNote was called with first argument = "
        << octave << endl;
    }
    
    // Convert Cb, C, C#/Db, D, D#/Eb, ..., B, B#
    //      to -1, 0,   1,   2,   3, ...,  11, 12
    
    int note;
    switch (noteLetter)
    {
        case 'C':  note =  0; break;
        case 'D':  note =  2; break;
        case 'E':  note =  4; break;
        case 'F':  note =  5; break;
        case 'G':  note =  7; break;
        case 'A':  note =  9; break;
        case 'B':  note = 11; break;
        default:   return ' ';
    }
    switch (accidentalSign)
    {
        case '#':  note++; break;
        case 'b':  note--; break;
        case ' ':  break;
        default:   return ' ';
    }
    
    // Convert ..., A#1, B1, C2, C#2, D2, ... to
    //         ..., -2,  -1, 0,   1,  2, ...
    
    int sequenceNumber = 12 * (octave - 2) + note;
    
    string keymap = "Z1X2CV3B4N5M,6.7/A8S9D0FG!H@JK#L$Q%WE^R&TY*U(I)OP";
    if (sequenceNumber < 0  ||  sequenceNumber >= keymap.size())
        return ' ';
    return keymap[sequenceNumber];
}

//*************************************
//  processNote
//*************************************
// This function start at the beginning of a note and return the
// encode of that note.

char processNote(string song, int& pos){
    // Assume the note is playable
    char ins = ' '; // initialize char ins as ' '
    pos++; // if song[pos] is a note, process the next character
    // if ends, then encode this note. This note only has a note letter
    if(song[pos] == '/' || isNoteLetter(song[pos])){
        ins = encodeNote(4, song[pos-1], ' ');
    // else if a digit, then encode this note. This note has a digit and a note letter
    } else if(isdigit(song[pos])){
        ins = encodeNote(song[pos] - '0',song[pos-1],' ');
        pos++; // move to the next character
    // else if an accidental sign, then check the next character
    } else if(song[pos] == '#' || song[pos] == 'b'){
        pos++;
        // if a digit, then encode. This note has a note letter,
        // a digit and an accidental sign
        if(isdigit(song[pos])){
            ins = encodeNote(song[pos] - '0', song[pos-2], song[pos-1]);
            pos++; // move to the next character
        // if ends, then encode. This note has a note letter and an accidental sign
        } else if(song[pos] == '/' || isNoteLetter(song[pos])){
            ins = encodeNote(4, song[pos-2], song[pos-1]);
        }
    }
    return ins; //return the encode char of the note start at song[pos]
}

//*************************************
//  encodeSong
//*************************************
// If the parameter song is a playable song, the function sets instructions
// to the encoding of the song, leaves badBeat unchanged, and returns 0.
// In all other cases, the function leaves instructions unchanged. If song
// is not a song string, the function leaves badBeat unchanged and returns 1.
// If song is a song string but is not playable, badBeat is set to the
// number of the beat with the first unplayable note (where the first beat
// of the whole song is number 1, the second is number 2, etc.), and the
// function returns 2. You must not assume that instructions and badBeat
// have any particular values at the time this function is entered.

int encodeSong(string song, string& instructions, int& badBeat) {
    
    int numOfBeats = 1; // count the number of beats
    int numOfNotes_beforeSlash = 0; // count the number of notes in each beat
    
    // return 1 if not a song string, leaves instructions and badBeat unchanged
    if (!hasCorrectSyntax(song)) {
        return 1;
    // return 2 is not playble, leaves instructions unchanged and set badBeat as
    // the first unplayble note
    } else if (!isPlayable(song)) {
        for(int k = 0; k!=song.size(); k++) {
            if (song[k]=='/')
                numOfBeats += 1;
            if (song[k] == '0' || song[k] == '1' || song[k] == '6' ||
                song[k] == '7' || song[k] == '8' || song[k] == '9') {
                if (k >= 2 && ((song[k-2] == 'C' && song[k-1] == 'b' && song[k] == '6')||
                               (song[k-2] == 'B' && song[k-1] == '#' && song[k] == '1'))){
                    badBeat = badBeat;
                } else if (k >= 1 && song[k-1] == 'C' && song[k] == '6') {
                    badBeat = badBeat;
                } else{
                    badBeat = numOfBeats;
                    return 2;
                }
            } else if(k >=2 && song[k-2] == 'C' && song[k-1] == 'b' && song[k] == '2') {
                // make sure not out of bound
                    badBeat = numOfBeats;
                    return 2;
            }
        }
        return 2;
    }
    
    int i = 0;
    //return 0 if playble, set instructions as encoded song and leaves the badBeat unchanged
    if (isPlayable(song)){
        instructions = ""; // initialize the instructions to an empty string
        string temp = "";  // creat a temporary string to store the encoded note
        while (i != song.size()){      // iterate through the song
            if(isNoteLetter(song[i])){ // if song[i] is a note letter
                numOfNotes_beforeSlash += 1;
                temp += processNote(song, i);
            } else if(song[i] == '/'){
                //if '/' is at the first position, then ' '
                if (i == 0) {
                    instructions += " ";
                // else if '/' is after a '/', then ' '
                } else if (song[i-1] == '/') {
                    instructions += " ";
                // else if one note before '/', then encode the note
                } else if (numOfNotes_beforeSlash == 1) {
                    instructions += temp;
                // else if more than one note before '/', then add []
                } else if (numOfNotes_beforeSlash > 1) {
                    instructions = instructions + "[" + temp + "]";
                }
                i++;
                numOfNotes_beforeSlash = 0;// after processing each beat, set the number of
                // notes in the beat as zero
                temp = ""; // set the encoded note in the beat as empty string
            }
        }
    }
    return 0;
}

