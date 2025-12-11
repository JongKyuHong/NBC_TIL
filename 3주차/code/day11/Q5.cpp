// Copyright 2025 JongKyu
#include <iostream>
#include <string>

using namespace std; // NOLINT

class Pilot {
 private:
     string name;
     int licenseID;
 public:
     Pilot() : name(""), licenseID(0) {}
     Pilot(string name, int id) : name(name), licenseID(id) {}
     void printPilotInfo() const {
         cout << "ID : " << licenseID << " Name : " << name;
     }
};

class Aircraft {
 private:
     string modelName;
     Pilot crew[2];
 public:
     Aircraft(string name, const Pilot& captain, const Pilot& coPilot)
              : modelName(name), crew{ captain, coPilot } {}
     void displayAircraftInfo() const {
         cout << "비행기 정보 출력\n" << "기종 : " << modelName << "\n";
     }
     void displayCrewInfo() const {
         cout << "조종사 정보 출력\n" << "기장 : ";
         crew[0].printPilotInfo();
         cout << "\n부기장 : ";
         crew[1].printPilotInfo();
     }
};

int main() {
    Pilot Captain("종규", 1);
    Pilot CoPilot("규종", 2);

    string ModelName = "배틀크루져";
    Aircraft AirCraftInstance(ModelName, Captain, CoPilot);

    AirCraftInstance.displayAircraftInfo();
    AirCraftInstance.displayCrewInfo();

    return 0;
}
