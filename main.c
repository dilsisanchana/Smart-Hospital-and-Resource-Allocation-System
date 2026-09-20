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
int patientWaitTimes[maxPatients];

int specialtyQueueCount[numOfSpecialties]={0,0,0,0};
double patientSurcharges[maxPatients];
double patientWardCosts[maxPatients];
double patientGrossBills[maxPatients];
double patientDiscounts[maxPatients];
double patientFinalBill[maxPatients];
int patientOrder[maxPatients];

int patientCount = 0;

void initializeBeds();
void displayBeds();
void registerPatient();
void allocateBed(int patientIndex);
void calculateWaitTime(int patientIndex);
void calculateSurcharge(int patientIndex);
void calculateWardCost(int patientIndex);
void calculateGrossBill(int patientIndex);
void calculateFinalBill(int patientIndex);
void sortPatientsByOrder();
void displayPriorityOrder();
void displayPatientBill(int patientIndex);
void displayUrgencyReport();

int main()
{
    int choice;

    initializeBeds();
    do
    {
        registerPatient();
        printf("\nDo you want to register another patient? (1= yes,0= No): ");
        scanf("%d",&choice);

    }while(choice == 1);

    printf("\nSmart Hospital & Resource Allocation System\n");

    sortPatientsByOrder();
    displayPriorityOrder();
    displayUrgencyReport();
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
   do
   {
     printf("\nEnter patient triage Level (1 = Normal, 2 = Urgent, 3 = Critical): ");
     scanf("%d",&patientTriageLevels[patientCount]);

     if (patientTriageLevels[patientCount]<1 || patientTriageLevels[patientCount]>3)
     {
        printf("Invalid triage level.Please enter 1, 2, or 3.\n");
     }
   }while(patientTriageLevels[patientCount]<1 || patientTriageLevels[patientCount]>3);

   do
   {
       printf("\nEnter specialty ID(1 to 4): ");
       scanf("%d",&patientSpecialties[patientCount]);

       if (patientSpecialties[patientCount]<1 || patientSpecialties[patientCount]>4)
       {
           printf("Invalid specialty ID .Please enter 1,2,3,or 4.\n");
       }
   }while (patientSpecialties[patientCount]<1 || patientSpecialties[patientCount]>4);

   calculateWaitTime(patientCount);
   calculateSurcharge(patientCount);

   do
   {

       printf("\nIs the patient admitted to ward?(1 = Yes, 0 = No): ");
       scanf("%d",&patientAdmitted[patientCount]);

       if (patientAdmitted[patientCount] != 0 && patientAdmitted[patientCount]!=1)
       {
           printf("Invalid choice.Please enter 1 or 0.\n");
       }
   }while (patientAdmitted[patientCount] != 0 && patientAdmitted[patientCount]!=1);

   if (patientAdmitted[patientCount]==1)
   {
       do
       {
           printf("\nEnter ward  ID(1 to 4): ");
           scanf("%d",&patientWards[patientCount]);

           if (patientWards[patientCount]<1 || patientWards[patientCount]>4)
           {
               printf("Invalid ward ID.Please enter 1,2,3, or 4.\n");
           }
       }while (patientWards[patientCount]<1 || patientWards[patientCount]>4);

       do
       {
           printf("\nEnter days admitted: ");
           scanf("%d",&patientDaysAdmitted[patientCount]);

           if (patientDaysAdmitted[patientCount]<1)
           {
               printf("Invalid number of days.Please enter at least 1 day.\n");
           }

       }while(patientDaysAdmitted[patientCount]<1);

       calculateWardCost(patientCount);
       allocateBed(patientCount);
   }
   else
   {
       patientDaysAdmitted[patientCount]=0;
       patientWardCosts[patientCount]=0;
   }

   calculateGrossBill(patientCount);
   calculateFinalBill(patientCount);
   displayPatientBill(patientCount);
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

void calculateWaitTime(int patientIndex)
{
    int specialtyIndex;
    specialtyIndex = patientSpecialties[patientIndex]-1;
    patientWaitTimes[patientIndex]=specialtyQueueCount[specialtyIndex]*consultationTime[specialtyIndex];

    printf("\nEstimated waiting time: %d minutes\n",patientWaitTimes[patientIndex]);

    specialtyQueueCount[specialtyIndex]++;
}

void calculateSurcharge(int patientIndex)
{
    int specialtyIndex;
    specialtyIndex=patientSpecialties[patientIndex]-1;

    if (patientTriageLevels[patientIndex]==1)
    {
        patientSurcharges[patientIndex]= 0;
    }
    else if (patientTriageLevels[patientIndex]==2)
    {
        patientSurcharges[patientIndex]=specialtyConsultationFees[specialtyIndex]*0.20;

    }
    else
    {
        patientSurcharges[patientIndex]=specialtyConsultationFees[specialtyIndex]*0.50;
    }

    printf("\nEmergency surcharge: %.2f LKR\n",patientSurcharges[patientIndex]);
}

void calculateWardCost(int patientIndex)
{
    int wardIndex;
    wardIndex=patientWards[patientIndex]-1;
    patientWardCosts[patientIndex]=dailyBedRates[wardIndex]*patientDaysAdmitted[patientIndex];
    printf("\nWard cost: %.2f LKR\n",patientWardCosts[patientIndex]);
}

void calculateGrossBill(int patientIndex)
{
    int specialtyIndex;
    specialtyIndex = patientSpecialties[patientIndex]-1;

    patientGrossBills[patientIndex]=specialtyConsultationFees[specialtyIndex]
                                    +patientSurcharges[patientIndex]
                                    +patientWardCosts[patientIndex];

    printf("\nGross bill: %.2f LKR\n",patientGrossBills[patientIndex]);
}

void calculateFinalBill(int patientIndex)
{
    if (patientAges[patientIndex]<5 || patientAges[patientIndex]>65)
    {
        patientDiscounts[patientIndex]=patientGrossBills[patientIndex]*0.15;
    }
    else
        patientDiscounts[patientIndex]=0.00;

    patientFinalBill[patientIndex]=patientGrossBills[patientIndex]-
                                    patientDiscounts[patientIndex];

    printf("\nFinal Bill: %.2f LKR\n",patientFinalBill[patientIndex]);
}

void sortPatientsByOrder()
{
    int i,j;
    int highestPriority;
    int temp;

    for (i=0; i< patientCount; i++)
    {
        patientOrder[i]=i;
    }
    for (i=0; i< patientCount - 1;i++)
    {
        highestPriority = i;

        for (j=i+1; j<patientCount; j++)
        {
            if (patientTriageLevels[patientOrder[j]] >
                patientTriageLevels[patientOrder[highestPriority]])
            {
                highestPriority = j;
            }
        }

        temp= patientOrder[i];
        patientOrder[i] = patientOrder[highestPriority];
        patientOrder[highestPriority] = temp;
    }
}

void displayPriorityOrder()
{
    int i;
    int patientIndex;

    printf("\nEmergency Priority Order:\n");

    for (i=0; i< patientCount; i++)
    {
        patientIndex = patientOrder[i];

        printf("%d. %s - Triage level %d\n",i+1,patientNames[patientIndex],
               patientTriageLevels[patientIndex]);
    }
}

void displayPatientBill(int patientIndex)
{
    int specialtyIndex;
    specialtyIndex= patientSpecialties[patientIndex]-1;

    printf("\n========== Patient Billing Summary ==========\n");
    printf("Patient Name: %s\n",patientNames[patientIndex]);
    printf("Consultation Fee: %.2f LKR\n",specialtyConsultationFees[specialtyIndex]);
    printf("Emergency Surcharge: %.2f LKR\n",patientSurcharges[patientIndex]);
    printf("Ward Cost: %.2f LKR\n",patientWardCosts[patientIndex]);
    printf("Gross Bill: %.2f LKR\n",patientGrossBills[patientIndex]);
    printf("Age Discount: %.2f LKR\n",patientDiscounts[patientIndex]);
    printf("Final Bill: %.2f LKR\n",patientFinalBill[patientIndex]);
    printf("=============================================\n");
}

void displayUrgencyReport()
{
    int i;
    int normalCount=0;
    int urgentCount=0;
    int criticalCount=0;

    for (i=0; i< patientCount; i++)
    {
        if (patientTriageLevels[i] ==1)
        {
            normalCount++;
        }
        else if (patientTriageLevels[i]== 2)
        {
            urgentCount++;
        }
        else if (patientTriageLevels[i]==3)
        {
            criticalCount++;
        }
    }

    printf("\n========== Patient Urgency Report ==========\n");
    printf("Normal Patients: %d\n",normalCount);
    printf("Urgent patients: %d\n",urgentCount);
    printf("Critical patients: %d\n",criticalCount);
    printf("===========================================\n");

}


