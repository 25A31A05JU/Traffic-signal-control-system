#include <iostream>
#include <string>
using namespace std;

// ---------------- VEHICLE ----------------
class Vehicle {
public:
    int id;
    string type;
    int priority;

    Vehicle(int id, string type) {
        this->id = id;
        this->type = type;

        if (type == "ambulance") priority = 4;
        else if (type == "fire") priority = 3;
        else if (type == "police") priority = 2;
        else priority = 1;
    }
};

// ---------------- NODE ----------------
class Node {
public:
    Vehicle data;
    Node* next;

    Node(Vehicle v) : data(v), next(NULL) {}
};

// ---------------- LANE ----------------
class Lane {
public:
    Node* head;
    Node* tail;
    string name;

    Lane(string name) {
        this->name = name;
        head = tail = NULL;
    }

    // Add vehicle (Queue behavior)
    void addVehicle(Vehicle v) {
        Node* newNode = new Node(v);
        if (!head) head = tail = newNode;
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Remove vehicle (Dequeue)
    void removeVehicle() {
        if (!head) {
            cout << name << " is empty!\n";
            return;
        }

        Node* temp = head;
        cout << "Passed: [" << temp->data.type << "-" << temp->data.id << "]\n";
        head = head->next;
        delete temp;

        if (!head) tail = NULL;
    }

    // Display
    void display() {
        cout << name << ": ";
        if (!head) {
            cout << "Empty";
        }
        Node* temp = head;
        while (temp) {
            cout << "[" << temp->data.type << "-" << temp->data.id << "] ";
            temp = temp->next;
        }
        cout << endl;
    }

    // Size
    int getSize() {
        int count = 0;
        Node* temp = head;
        while (temp) {
            count++;
            temp = temp->next;
        }
        return count;
    }

    // Check ambulance (from your first code idea)
    bool hasAmbulance() {
        Node* temp = head;
        while (temp) {
            if (temp->data.type == "ambulance")
                return true;
            temp = temp->next;
        }
        return false;
    }

    // Check any emergency
    bool hasEmergency() {
        Node* temp = head;
        while (temp) {
            if (temp->data.priority > 1)
                return true;
            temp = temp->next;
        }
        return false;
    }

    // Move ambulance to front (your queue logic adapted)
    void moveAmbulanceToFront() {
        if (!head || !head->next) return;

        Node* temp = head;
        Node* prev = NULL;

        while (temp) {
            if (temp->data.type == "ambulance") break;
            prev = temp;
            temp = temp->next;
        }

        if (!temp || temp == head) return;

        // Remove from middle
        prev->next = temp->next;

        if (temp == tail) tail = prev;

        // Insert at front
        temp->next = head;
        head = temp;
    }

    // General priority handling
    void prioritizeEmergency() {
        if (!head || !head->next) return;

        Node* temp = head;
        Node* prev = NULL;

        Node* maxNode = head;
        Node* maxPrev = NULL;

        while (temp) {
            if (temp->data.priority > maxNode->data.priority) {
                maxNode = temp;
                maxPrev = prev;
            }
            prev = temp;
            temp = temp->next;
        }

        if (maxNode == head) return;

        if (maxPrev) maxPrev->next = maxNode->next;

        if (maxNode == tail) tail = maxPrev;

        maxNode->next = head;
        head = maxNode;
    }
};

// ---------------- SIGNAL ----------------
class TrafficSignal {
public:
    Lane* lanes[4];

    TrafficSignal(Lane* l1, Lane* l2, Lane* l3, Lane* l4) {
        lanes[0] = l1;
        lanes[1] = l2;
        lanes[2] = l3;
        lanes[3] = l4;
    }

    int getNextLane() {
        // ?? Priority 1: Ambulance (from first code)
        for (int i = 0; i < 4; i++) {
            if (lanes[i]->hasAmbulance()) {
                lanes[i]->moveAmbulanceToFront();
                return i;
            }
        }

        // ?? Priority 2: Any emergency
        for (int i = 0; i < 4; i++) {
            if (lanes[i]->hasEmergency()) {
                lanes[i]->prioritizeEmergency();
                return i;
            }
        }

        // ?? Priority 3: Largest queue
        int maxIndex = 0;
        int maxSize = lanes[0]->getSize();

        for (int i = 1; i < 4; i++) {
            if (lanes[i]->getSize() > maxSize) {
                maxSize = lanes[i]->getSize();
                maxIndex = i;
            }
        }

        return maxIndex;
    }

    void runSignal() {
        int index = getNextLane();

        cout << "\n?? GREEN SIGNAL ? " << lanes[index]->name << endl;
        lanes[index]->removeVehicle();

        displayAll();
    }

    void displayAll() {
        cout << "\n--- Current Traffic ?????? ---\n";
        for (int i = 0; i < 4; i++) {
            lanes[i]->display();
        }
        cout << "--------------------------------\n";
    }
};

// ---------------- MAIN ----------------
int main() {
    Lane l1("Lane A"), l2("Lane B"), l3("Lane C"), l4("Lane D");
    TrafficSignal signal(&l1, &l2, &l3, &l4);

    int choice, laneChoice, id = 1;
    string type;

    while (true) {
        cout << "\n===== TRAFFIC MENU =====\n";
        cout << "1. Add Vehicle\n";
        cout << "2. Run Signal Cycle\n";
        cout << "3. Display Traffic\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Select Lane (1-A, 2-B, 3-C, 4-D): ";
            cin >> laneChoice;

            cout << "Enter vehicle type: ";
            cin >> type;

            if (laneChoice == 1) l1.addVehicle(Vehicle(id++, type));
            else if (laneChoice == 2) l2.addVehicle(Vehicle(id++, type));
            else if (laneChoice == 3) l3.addVehicle(Vehicle(id++, type));
            else if (laneChoice == 4) l4.addVehicle(Vehicle(id++, type));
            else cout << "Invalid lane!\n";

            break;

        case 2:
            signal.runSignal();
            break;

        case 3:
            signal.displayAll();
            break;

        case 4:
            cout << "Exiting...\n";
            return 0;

        default:
            cout << "Invalid choice!\n";
        }
    }
}