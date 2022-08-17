#include <iostream>
#include <vector>
#include <queue>

#define cutoffTime 5
#define numOfDocs 5
#define avgPatientTime 15

using namespace std;

class Doctor {
public:
	long id;
	string name;
    string type;
  
    long availableTime;
  
    Doctor(long id){
        this->id = id;
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
        this->id = id;
        this->incomingTime = incomingTime;
        this->type = type;
    }
};

struct compare{
    bool operator()(Doctor* a,Doctor* b){
        return a->availableTime > b->availableTime;
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
    // assign doctor if one is available
    while(!doctorHeap.empty() && doctorHeap.top()->availableTime <= currTime && !paidQueue.empty() || !freeQueue.empty()){
        Doctor* doctor = doctorHeap.top();
        doctorHeap.pop();
        cout << "Found available doctor: " << doctor->id << endl;
        if(!paidQueue.empty()){
            // priority given to the Paid patient
            cout << "Paid Patient: " << paidQueue.front()->id << " has been assigned doctor: " << doctor->id << endl;
            paidQueue.pop();
        }else{
            cout << "Free Patient: " << freeQueue.front()->id << " has been assigned doctor: " << doctor->id << endl;
            freeQueue.pop();
        }

        //engage doctor & push back to heap
        doctor->availableTime = currTime + avgPatientTime;
        doctorHeap.push(doctor);
    }
}

vector<Doctor*> getDoctors(){
    //for iterating through the doctorHeap
    vector<Doctor*> doctors;
    while(!doctorHeap.empty()){
        doctors.push_back(doctorHeap.top());
        doctorHeap.pop();
    }
    // repush
    for(Doctor* doc : doctors){
        doctorHeap.push(doc);
    }
    return doctors;
}

bool isRefundRequired(long time){
    //check if refund is required for the next coming patient
    cout << "Checking if new patient might need refund\n" << endl;
    vector<Doctor* > doctors = getDoctors();
    
    int freeDocCount = 0;
    for(Doctor* doc : doctors){
        if(time + cutoffTime >= doc->availableTime){
            freeDocCount++;
        }
    }

    if(freeDocCount <= paidQueue.size()){
        cout << "Refund will be required. Forwarding patient to free queue" << endl;
        return true;
    }
    return false;
}

void giveRefunds(long time){
    // provide refund if patient goes beyond cutoff time
    while(!paidQueue.empty() && time - paidQueue.front()->incomingTime >= cutoffTime){
        Patient* patient = paidQueue.front();
        cout << "Refund given to patient: " << patient->id << endl;
        paidQueue.pop(); //removing the patient
    }
}

void visitingPatient(long id, long incomingTime, string type){
    //incomingTime == incomingTime of the patient
	Patient* patient = new Patient(id, incomingTime, type);
	
    // if patient is from free queue, or the next person coming might require refund
    if(type == "free" || isRefundRequired(incomingTime)){
        cout << "Adding patient: " << id << " in Free queue" << endl;
        freeQueue.push(patient);
    }else{
        cout << "Adding patient: " << id << " in Paid queue" << endl;
        paidQueue.push(patient);
    }

    assignDoctor(incomingTime);
    //give any refunds remaining
    giveRefunds(incomingTime);
}

int main() {
    //initiate all the doctors in priority queue (min)
    initiateDoctors(doctorHeap);

    // incoming patient queries
    visitingPatient(1, 2, "free");
    visitingPatient(2, 3, "paid");
    visitingPatient(3, 4, "paid");
    visitingPatient(4, 5, "paid");
    visitingPatient(5, 6, "paid");
    visitingPatient(6, 7, "free");
    visitingPatient(7, 8, "paid");
    visitingPatient(8, 9, "paid");
    visitingPatient(9, 10, "paid");
    visitingPatient(30, 100, "paid");
    visitingPatient(40, 100, "paid");


    giveRefunds(INT_MAX);
    
    return 0;
}