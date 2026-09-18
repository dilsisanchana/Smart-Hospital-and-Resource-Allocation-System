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

//Bed Occupancy Matrix
//0 = Available , 1 = Occupied
int bedOccupancy[numOfWards][maxBeds];

//patient data
char patientNames[maxPatients][50];
int patientAges[maxPatients];
int patientTriageLevels[maxPatients];
int patientSpecialties[maxPatients];
int patientAdmitted[maxPatients];
int patientWards[maxPatients];
int patientDaysAdmitted[maxPatients];
int patientBedNumber[maxPatients];

int patientCount = 0;

void initializeBeds();
void displayBeds();
void registerPatient();
void allocateBed(int patientIndex);

int main()
{
    initializeBeds();
    registerPatient();

    printf("\nSmart Hospital & Resource Allocation System\n");

    displayBeds();
    return 0;
}

void initializeBeds()
{
    int i,j;
    for (i=0; i<numOfWards; i++)
    {
        for (j=0; j<maxBeds; j++)
        {
            bedOccupancy[i][j]=0;
        }
    }
}

void displayBeds()
{
    int i,j;
    for (i=0; i<numOfWards;i++)
    {
        printf("\n%s: ",wardNames[i]);
        for (j=0; j<wardBedCapacities[i]; j++)
        {
            printf("%d ",bedOccupancy[i][j]);
        }
        printf("\n");
    }
}

void registerPatient()
{
   printf("\nEnter patient name: ");
   scanf(" %[^\n]",patientNames[patientCount]);

   printf("\nEnter patient age: ");
   scanf("%d",&patientAges[patientCount]);

   printf("\nEnter patient triage Level (1 = Normal, 2 = Urgent, 3 = Critical): ");
   scanf("%d",&patientTriageLevels[patientCount]);

   printf("\nEnter specialty ID(1 to 4): ");
   scanf("%d",&patientSpecialties[patientCount]);

   printf("\nIs the patient admitted to ward?(1 = Yes, 0 = No): ");
   scanf("%d",&patientAdmitted[patientCount]);

   if (patientAdmitted[patientCount]==1)
   {
       printf("\nEnter ward  ID(1 to 4): ");
       scanf("%d",&patientWards[patientCount]);

       printf("\nEnter days admitted: ");
       scanf("%d",&patientDaysAdmitted[patientCount]);

       allocateBed(patientCount);
   }
   else
   {
       patientDaysAdmitted[patientCount]=0;
   }
   patientCount++;
}

void allocateBed(int patientIndex)
{
    int wardIndex;
    int j;

    wardIndex =patientWards[patientIndex]-1;
    for (j=0; j<wardBedCapacities[wardIndex]; j++)
    {
        if (bedOccupancy[wardIndex][j]== 0)
        {
            bedOccupancy[wardIndex][j]=1;
            patientBedNumber[patientIndex] = j+1;

            printf("\nBed allocated: %d\n",patientBedNumber[patientIndex]);
            return;

        }
    }
    printf("\nNo available bed in %s",wardNames[wardIndex]);
}
