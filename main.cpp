#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
using namespace std;

struct ElectriCheck{
    string house, appliance;
    float cost, kWh;
    int time;
};

//can be any value though
float ConversionRate = 0;


void LoadConversionRate(float *conver);
void SaveConversionRate(float *conver);
void header(string txt);
void MenuAlign(string text, int width);
void ConfigureRate(float *conver);
float compute(float *conver, float kwh, int time);
void NewHouse(ElectriCheck *EleCheck, float *conver);
void ExistingHouse();
void CallFunctions();


int main(){
    LoadConversionRate(&ConversionRate);

    CallFunctions();

    system("cls||clear");
    header("Program Closed.");
    return 0;
}


void LoadConversionRate(float *conver){
    ifstream RateFile("conversion_rate.txt");

    if(RateFile){
        RateFile >> *conver;

        if(RateFile.fail()){
            *conver = 5;
        }

        RateFile.close();
    }
    else{
        *conver = 5;

        ofstream NewRateFile("conversion_rate.txt");
        NewRateFile << *conver;
        NewRateFile.close();
    }
}

void SaveConversionRate(float *conver){
    ofstream RateFile("conversion_rate.txt");

    if(RateFile){
        RateFile << *conver;
        RateFile.close();
    }
}

void header(string txt){
    cout << "======================================================" << endl;
    cout << "-" << setw(26 + txt.length()/2) << txt << setw(27 - txt.length()/2) << "-" << endl;
    cout << "======================================================" << endl;
}

void MenuAlign(string text, int width) {
    cout << setw((width + text.length()) / 2) << right << text << endl;
}

void ConfigureRate(float *conver){
    system("cls||clear");

    header("Configure Utility Rates");

    int options=0;
    cout << "Current Conversion Rate: " << *conver << endl;
    cout << "1. Change Conversion Rate" << endl;
    cout << "2. Exit" << endl;
    cout << "\nEnter Option: ";
    cin >> options;

    if(options==1){
        cout << "Enter New Conversion Rate: ";
        cin >> *conver;

        SaveConversionRate(conver);

        cout << "Conversion rate updated successfully!" << endl;
    }
}

float compute(float *conver, float kwh, int time){
    float cost=0;
    cost=*conver*(kwh*time);
    return cost;
}
void NewHouse(ElectriCheck *EleCheck, float *conver){
    system("cls||clear");
    header("New House");
    cout << "Current Conversion Rate: " << *conver << endl;
    ofstream HouseName("HouseNames.txt", ios::app);
    cin.ignore();
    cout << "Enter House Name: ";
    getline(cin, EleCheck->house);
    HouseName << EleCheck->house << endl;
    string Owner=EleCheck->house+".txt";
    ofstream CurrentOwner(Owner);

    int NumAppliances=0;
    float totalBill=0;
    cout << "Enter number of Appliances: ";
    cin >> NumAppliances;
    cin.ignore();
    ElectriCheck *Appliances=new ElectriCheck[NumAppliances];
    for(int i=0; i<NumAppliances; i++){
        cout << "\nAppliance " << i+1 << endl;
        cout << "Enter Appliance Name: ";
        getline(cin,Appliances[i].appliance);
        cout << "Enter kWh: ";
        cin >> Appliances[i].kWh;
        cout << "Enter use duration(Minutes): ";
        cin >> Appliances[i].time;
        totalBill+=compute(conver, Appliances[i].kWh, Appliances[i].time);
        CurrentOwner<<i+1<<","
                    <<Appliances[i].appliance<<","
                    <<Appliances[i].kWh<<","
                    <<Appliances[i].time << endl;
        cin.ignore();
    }
    cout << "\n------------------------------------------------------" << endl;
    cout << "Total Bill: " << totalBill << endl;
    CurrentOwner<<totalBill<<endl;
    int test;
    cin >> test;
    CurrentOwner.close();
    delete[] Appliances;
}
void ExistingHouse(){
    header("Recorded House's");

}
void CallFunctions(){
    system("cls||clear");
    ElectriCheck *EleCheck=new ElectriCheck;
    float *conversion=&ConversionRate;
    int options=0;
    header("ElectriCheck");
    MenuAlign("1. New House", 45);
    MenuAlign("2. Check Existing House", 56);
    MenuAlign("3. Configure Utility Rates", 59);
    MenuAlign("4. Exit", 40);
    cout << "\n" << setw(30) << "Enter Option: ";
    cin >> options;

    if(options==1){
        NewHouse(EleCheck, conversion);
        CallFunctions();
    }
    else if(options==2){
        system("cls");
        ExistingHouse();
        CallFunctions();
    }
    else if(options==3){
        ConfigureRate(conversion);
        CallFunctions();
    }
    else if(options==4){
        system("cls||clear");
        return;
    }
    else{
        system("cls||clear");
        CallFunctions();
    }
}
