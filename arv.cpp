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

//startup and saving stuff
void LoadConversionRate(float *conver);
void SaveConversionRate(float *conver);

//Main selection
void NewHouse(ElectriCheck *EleCheck, float *conver);
void ExistingHouse();
void CallFunctions();

//for the text output stuff
void header(string txt);
void MenuAlign(string text, int width);

//Configuration and computation
void ConfigureRate(float *conver);
float compute(float *conver, float kwh, int time);

void ShowApplianceUsage(string houseName);
void RemoveHouse(string houseName);

int main(){
    LoadConversionRate(&ConversionRate);
    CallFunctions();
    system("cls||clear");
    header("Program Closed.");
    return 0;
}

void LoadConversionRate(float *conver){
    ifstream RateFile;
    RateFile.open("conversion_rate.txt");

    if(RateFile){
        RateFile >> *conver;
        if(RateFile.fail()){
            *conver = 5;
        }
        RateFile.close();
    }
    else{
        *conver = 5;
        RateFile.close();

        ofstream NewRateFile;
        NewRateFile.open("conversion_rate.txt");
        NewRateFile << *conver;
        NewRateFile.close();
    }
}

void SaveConversionRate(float *conver){
    ofstream RateFile;
    RateFile.open("conversion_rate.txt");

    if(RateFile){
        RateFile << *conver;
        RateFile.close();
    }
}

void CallFunctions(){
    ElectriCheck *EleCheck = new ElectriCheck;
    float *conversion = &ConversionRate;
    int options = 0;

    do{
        system("cls||clear");
        header("ElectriCheck");
        MenuAlign("1. New House", 45);
        MenuAlign("2. Check Existing House", 56);
        MenuAlign("3. Configure Utility Rates", 59);
        MenuAlign("4. Exit", 40);
        cout << "\n" << setw(30) << "Enter Option: ";
        cin >> options;
        cin.ignore();

        if(options == 1){
            NewHouse(EleCheck, conversion);
        }
        else if(options == 2){
            ExistingHouse();
        }
        else if(options == 3){
            ConfigureRate(conversion);
        }
    }while(options != 4);

    delete EleCheck;
    system("cls||clear");
}

void NewHouse(ElectriCheck *EleCheck, float *conver){
    system("cls||clear");
    header("New House");
    cout << "Current Conversion Rate: " << *conver << endl;

    ofstream HouseName;
    HouseName.open("HouseNames.txt", ios::app);

    cin.ignore();
    cout << "Enter House Name: ";
    getline(cin, EleCheck->house);
    HouseName << EleCheck->house << endl;
    HouseName.close();

    string Owner = EleCheck->house + ".txt";
    ofstream CurrentOwner;
    CurrentOwner.open(Owner.c_str());

    int NumAppliances = 0;
    float totalBill = 0;
    cout << "Enter number of Appliances: ";
    cin >> NumAppliances;
    cin.ignore();

    ElectriCheck *Appliances = new ElectriCheck[NumAppliances];
    for(int i = 0; i < NumAppliances; i++){
        cout << "\nAppliance " << i+1 << endl;
        cout << "Enter Appliance Name: ";
        getline(cin, Appliances[i].appliance);
        cout << "Enter kW: ";
        cin >> Appliances[i].kWh;
        cout << "Enter use duration(Minutes): ";
        cin >> Appliances[i].time;
        totalBill += compute(conver, Appliances[i].kWh, Appliances[i].time);
        CurrentOwner << i+1 << ","
                     << Appliances[i].appliance << ","
                     << Appliances[i].kWh << ","
                     << Appliances[i].time << endl;
        cin.ignore();
    }

    system("cls||clear");
    cout << "--------------------------------------------------------" << endl;
    cout << left << setw(10) << "ID" << setw(15) << "Appliance" << setw(10) << "kW" << setw(10) << "Time Usage" << endl;
    cout << "--------------------------------------------------------" << endl;
    for(int i = 0; i < NumAppliances; i++){
        cout << left << setw(10) << i+1 << setw(15) << Appliances[i].appliance << setw(10) << Appliances[i].kWh << setw(10) << Appliances[i].time << endl;
    }
    cout << "\n------------------------------------------------------" << endl;
    cout << "Total Bill: " << fixed << setprecision(2) << totalBill << endl;
    CurrentOwner << fixed << setprecision(2) << totalBill << endl;
    CurrentOwner.close();
    cout << "\nPress Enter to return to menu";
    cin.get();
    delete[] Appliances;
}


void ExistingHouse(){
    system("cls||clear");
    header("Recorded Houses");

    ifstream CountFile;
    CountFile.open("HouseNames.txt");

    if(!CountFile){
        cout << "No recorded houses found." << endl;
        cout << "\nPress Enter to return to menu";
        cin.ignore();
        cin.get();
        return;
    }

    int count = 0;
    string house;

    while(getline(CountFile, house)){
        if(house != ""){
            count++;
        }
    }
    CountFile.close();

    if(count == 0){
        cout << "No recorded houses found." << endl;
        cout << "\nPress Enter to return to menu";
        cin.ignore();
        cin.get();
        return;
    }

    string *houses = new string[count];

    ifstream HouseName;
    HouseName.open("HouseNames.txt");

    int index = 0;
    while(getline(HouseName, house)){
        if(house != ""){
            houses[index] = house;
            index++;
        }
    }
    HouseName.close();

    for(int i = 0; i < count; i++){
        cout << i + 1 << ". " << houses[i] << endl;
    }

    int houseChoice;
    cout << "\nSelect a house number[0 to go back]: ";
    cin >> houseChoice;

    if(houseChoice == 0){
        delete[] houses;
        return;
    }

    if(houseChoice < 1 || houseChoice > count){
        cout << "Invalid house number." << endl;
        delete[] houses;
        cout << "\nPress Enter to return to menu";
        cin.ignore();
        cin.get();
        return;
    }

    string selectedHouse = houses[houseChoice - 1];
    delete[] houses;

    system("cls||clear");
    header(selectedHouse);

    int option;
    cout << "1. Show Appliances usage" << endl;
    cout << "2. Remove house" << endl;
    cout << "3. Back" << endl;
    cout << "\nEnter Option: ";
    cin >> option;

    if(option == 1){
        ShowApplianceUsage(selectedHouse);
    }
    else if(option == 2){
        RemoveHouse(selectedHouse);
    }
    else if(option == 3){
        ExistingHouse();
        return;
    }
    else{
        cout << "Invalid option." << endl;
        cout << "\nPress Enter to return to menu";
        cin.ignore();
        cin.get();
    }
}

void header(string txt){
    int len = txt.length();
    cout << "======================================================" << endl;
    cout << "-" << setw(26 + len/2) << txt << setw(27 - len/2) << "-" << endl;
    cout << "======================================================" << endl;
}

void MenuAlign(string text, int width){
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
    float cost = 0;
    cost = *conver*(kwh*(time/60.0));
    return cost;
}

void ShowApplianceUsage(string houseName){
    system("cls||clear");
    header("Appliance Usage");

    string filename = houseName + ".txt";
    ifstream HouseFile;
    HouseFile.open(filename.c_str());

    if(!HouseFile){
        cout << "Could not open file for " << houseName << "." << endl;
        cout << "\nPress Enter to return to menu";
        cin.ignore();
        cin.get();
        return;
    }

    string line, appName, KWH, Usage;

    cout << "House: " << houseName << endl;
    cout << "------------------------------------------------------" << endl;
    cout << left << setw(10) << "ID" << setw(15) << "Appliance" << setw(10) << "kW" << setw(10) << "Time Usage" << endl;
    cout << "------------------------------------------------------" << endl;

    while(getline(HouseFile, line, ',')){
        //reseter so it does not read the bill
        appName = "";

        getline(HouseFile, appName, ',');
        getline(HouseFile, KWH, ',');
        getline(HouseFile, Usage);

        //check if the appliance name is empty so it can print the bill
        if(appName.empty()){
            cout << "------------------------------------------------------" << endl;
            cout << "Total Bill: " << line << endl;
            break;
        }
        cout << left << setw(10) << line << setw(15) << appName << setw(10) << KWH << setw(10) << Usage << endl;
    }
    HouseFile.close();

    cout << "\nPress Enter to return to menu";
    cin.ignore();
    cin.get();
}

void RemoveHouse(string houseName){
    system("cls||clear");
    header("Remove House");

    ifstream CountFile;
    CountFile.open("HouseNames.txt");

    if(!CountFile){
        cout << "HouseNames.txt not found." << endl;
        cout << "\nPress Enter to return to menu";
        cin.ignore();
        cin.get();
        return;
    }

    int count = 0;
    string house;

    while(getline(CountFile, house)){
        if(house != "" && house != houseName){
            count++;
        }
    }
    CountFile.close();

    string *houses = new string[count];

    ifstream HouseName;
    HouseName.open("HouseNames.txt");

    int index = 0;
    while(getline(HouseName, house)){
        if(house != "" && house != houseName){
            houses[index] = house;
            index++;
        }
    }
    HouseName.close();

    ofstream UpdatedHouseName;
    UpdatedHouseName.open("HouseNames.txt");

    for(int i = 0; i < count; i++){
        UpdatedHouseName << houses[i] << endl;
    }
    UpdatedHouseName.close();

    delete[] houses;

    string filename = houseName + ".txt";
    remove(filename.c_str());

    cout << houseName << " has been removed." << endl;
    cout << "\nPress Enter to return to menu";
    cin.ignore();
    cin.get();
}
