//  fare.cpp
//  Project 2
//  UID: 304952720
//  Created by Rui Yan on 12/10/2017.
//  Copyright © 2017 Rui Yan. All rights reserved.

#include <iostream>
#include <string>
using namespace std;

int main()
{
    const double BASE = 1.35;            //the base of fare is $1.35
    const double PERZONE = 0.55;         //adds $0.55 to fare per zone
    const double SNRBASE = 0.55;         //the base of senior fare is $0.55
    const double SNRPERZONE = 0.25;      //adds $0.25 to senior fare per zone

    const double MNR_STU_SNR = 0.65;     //when zone = 0 or 1, student and minor fare are $0.65;
                                   //when zone = 0, senior fare is $0.65.
    const double SNR_0 = 0.45;           //when zone = 0, senior fare is $0.45.
    
    // Gather input data
    cout << "Age of rider: ";      //print out the prompt for age
    int age;                       //declare the integer age
    cin >> age;                    //read the value of age from keyboard
    cin.ignore(10000, '\n');       
    
    cout << "Student? (y/n): ";    //print out the prompt for student status
    string stu;                    //declare the string stu
    getline(cin, stu);             //read the input from keyboard and store it to stu
    
    cout << "Destination: ";       //print out the prompt for destination
    string dest;                   //declare the string dest
    getline(cin, dest);            //read the input from keyboard and store it to dest

    cout << "Number of zone boundaries crossed: ";
    int zone;
    cin >> zone;
    cin.ignore(10000, '\n');
    
    double amt;                    //declare the double amt (amount)
    cout.setf(ios::fixed);         //set the number be rounded to 0.01
    cout.precision(2);
    
    //compute the amount of fare by cases
    if ((age < 18) || (stu == "y" && age < 65)) {     //consider students or people under 18
        if (zone <= 1 ) {
            amt = MNR_STU_SNR;        //for students or people under 18, when zone = 0 or 1, fare = $0.65
        } else {
            amt = BASE + PERZONE*zone;//for students or people under 18, when zone > 1, fare = $1.35 + $0.55*zone
        }
    }
    else if (age >= 65) {             //consider people 65 or over
        if (zone == 0){
            amt = SNR_0;
        } else if (stu == "y" && zone == 1) {
            amt = MNR_STU_SNR;
        } else {
            amt = SNRBASE + SNRPERZONE*zone;
        }
    }
    else {
        amt = BASE + PERZONE*zone;
    }

    //handle the incorrect input by cases and print result
    if (age < 0) {
        cout << "---" << endl;
        cout << "The age must not be negative" << endl;
        return 1;
    } else if ((stu != "y") && (stu != "n")) {
        cout << "---" << endl;
        cout << "You must enter y or n" << endl;
        return 1;
    } else if (dest == "") {
        cout << "---" << endl;
        cout << "You must enter a destination" << endl;
        return 1;
    } else if (zone < 0) {
        cout << "---" << endl;
        cout << "The number of zone boundaries crossed must not be negative" << endl;
        return 1;
    } else {
        cout << "---" << endl;
        cout << "The fare to " << dest << " is $" << amt << endl;
        return 0;
    }
}
