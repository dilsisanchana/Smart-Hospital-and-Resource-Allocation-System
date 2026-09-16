#include <stdio.h>

#define maxPatients 100
#define numOfSpecialties 4
#define numOfWards 4
#define maxBeds 20

//Doctor Specialties Lookup data
const char *specialtyNames[numOfSpecialties]={"General Practice(OPD)",
"Paediatrics","Cardiology","Neurology"};

const double specialtyConsultationFees[numOfSpecialties]={1500.00,
2500.00,4500.00,5000.00};

const int consultationTime[numOfSpecialties]={15,20,30,30};

const int dailyPatientCapacity[numOfSpecialties]={30,20,12,10};

//Hospital Ward Lookup data

const char *wardNames[numOfWards]={"General Ward","Paediatric Ward",
"Surgical Ward","ICU (Intensive Care Unit)"};

const double dailyBedRates[numOfWards]={3000.00,6000.00,12000.00,
25000.00};

const int wardBedCapacities[numOfWards]={20,10,10,5};


int main()
{
    printf("Smart Hospital & Allocation System\n");
    return 0;
}
