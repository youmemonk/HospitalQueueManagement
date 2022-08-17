#include <iostream>
#include <vector>
#include <queue>

#define cutoffTime 5
#define numOfDocs 5

using namespace std;

class Doctor {
public:
	long id;
	string name;
  string type;
  
  long availableTime;
  
  Doctor(long id){
  	this->availableTime = 0;
  }
};

class Patient {
public:
	long id;
    string name;
    string type;
  
    long incomingTime;
    
    Patient(long id, long incomingTime, string type){
        this->incomingTime = incomingTime;
        this->type = type;
    }
};

struct compare{
    bool operator()(Doctor* a,Doctor* b){
        return a->availableTime < b->availableTime;
    }
};


queue<Patient*> paidQueue;
queue<Patient*> freeQueue;
priority_queue<Doctor*, vector<Doctor*>,compare> doctorHeap;

void initiateDoctors(priority_queue<Doctor*, vector<Doctor*>, compare> &doctorHeap){
    long incrementalId = 1;
    int availDocs = numOfDocs;
    while(availDocs--){
        Doctor* doc = new Doctor(incrementalId++);
        doctorHeap.push(doc);
    }
}

void assignDoctor(long currTime){
    while(!doctorHeap.empty() && doctorHeap.top()->availableTime <= currTime){
        Doctor* doctor = doctorHeap.top(); doctorHeap.pop();
        if(!paidQueue.empty()){
            paidQueue.pop();
        }else{
            freeQueue.pop();
        }

        //engage doctor & push back to heap
        doctor->availableTime = currTime + 15;
        doctorHeap.push(doctor);
    }
}

void checkForRefunds(long time){
    while(!paidQueue.empty() && time - paidQueue.front()->incomingTime >= cutoffTime){
        Patient* patient = paidQueue.front();
        cout << "Refund given to patient: " << patient->id << endl;
        paidQueue.pop(); //removing the patient
    }
}

void visitingPatient(long id, long time, string type){
	Patient* patient = new Patient(id, time, type);
	
    if(type == "free"){
        freeQueue.push(patient);
    }else{
        paidQueue.push(patient);
    }

    assignDoctor(time);
    checkForRefunds(time);
}

int main() {
    initiateDoctors(doctorHeap);
    visitingPatient(1, 2, "free");
    visitingPatient(2, 3, "paid");
    visitingPatient(3, 3, "paid");
    visitingPatient(4, 3, "paid");
    visitingPatient(5, 3, "paid");
    visitingPatient(6, 3, "paid");
    visitingPatient(7, 3, "paid");
    visitingPatient(8, 3, "paid");
    visitingPatient(9, 3, "paid");

    return 0;
}