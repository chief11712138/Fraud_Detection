#include "Fraud_Detection_System.h"

Fraud_Detection_System::Fraud_Detection_System()
{
    Initialize();
}

void Fraud_Detection_System::Initialize()
{
    ReadKnownData();
    ReadUnknownData();
    InitializeBP();

    cout << "Data initialize finish." << endl;
}

void Fraud_Detection_System::update()
{
    Initialize();

    cout << "Everything up to date." << endl;
}
