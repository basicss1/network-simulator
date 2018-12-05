#include "system.h"
#include "machines.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

System::System()
        : tick_(0) {}

void System::format(std::ostream& os, const vector<IP_address>& ips0) const
{
    vector<IP_address> ips = ips0;
    if (ips.empty()) {
        for (size_t i = 0; i < network_.size(); ++i)
            if (network_[i] != nullptr)
                ips.push_back(network_[i]->get_ip());
    }
    os << "system {\n  tick = " << tick_ << ",\n";
    for (IP_address& ip : ips) {
        os << "  ";
        os << ip;
        os << " = ";
        int idx = -1;
        for (int i = 0; i < MAX_MACHINES; ++i) {
            if (network_[i]->get_ip() == ip) {
                idx = i;
                break;
            }
        }
        if (idx < 0)
            throw err_code::no_such_machine;
        os << *network_[idx];
        os << '\n';
    }
    os << "}\n";
}

void System::create_machine(const string& type,
                            const string& name,
                            const IP_address& ip)
{
    // check for type, if not one of three throw error code
    if(type != "laptop" && type != "server" && type != "wan") {
        throw err_code::unknown_machine_type;
    }

    // finds the first empty (nullptr) slot in the network_ array
    for (int i = 0; i < network_.size(); i++) {
        if (network_[i] == nullptr) {
            //creates a new machine by calling make_shared<Node>(name, ip).
            shared_ptr<Node> machine = make_shared<Node>(name, ip);
            network_[i] = machine;

            cout << "System::create_machine: " << type << ' ' << name << ' ';
            cout << ip;
            cout << '\n';
            return;
        }
    }

    // throw an err_code::network_full exception if the network_ array is full
    throw err_code::network_full;
}

void System::delete_machine(const IP_address& ip)
{
    // find the machine with the given ip address
    for (int i = 0; i < network_.size(); i++) {

        if (network_[i] != nullptr && (network_[i])->get_ip() == ip) {
            shared_ptr<Node> machine = network_[i];

            //disconnect between this machine and all other machines in network_ to disconnect
            for (int j = 0; j < network_.size(); j++) {
                if (network_[j] != machine)
                    machine->disconnect(network_[j]);
            }

            // after disconnecting the machine from others in network, set it to nullpointer
            network_[i] = nullptr;

            cout << "System::delete_machine: ";
            cout << ip;
            cout << '\n';
            return;
        }
    }

    // if did not find machine in network
    throw err_code::no_such_machine;
}

void System::connect_machine(const IP_address& ip1, const IP_address& ip2)
{
    // call connect to connect these two machines to each other
    if (ip1==ip2) return;

    //create share pointer machine 1 and 2
    shared_ptr<Node> machine1, machine2;

    //looping through the network of the machine
    for (int i = 0; i < network_.size(); i++) {

        // if the match for ip1 and ip2 are found, set machine1 and 2 to that element
        if (network_[i] != nullptr && network_[i]->get_ip()==ip1) {
            machine1 = network_[i];
        }
        else if (network_[i] != nullptr && network_[i]->get_ip() == ip2) {
            machine2 = network_[i];
        }
    }

    //if both machine is found, connect the machines
    if (machine1 != nullptr && machine2 != nullptr) {
        machine1->connect(machine2);
        machine2->connect(machine1);
    }
        // throw error code if one or both machine not found
    else {
        throw err_code::no_such_machine;
    }

    cout << "System::connect_machine: ";
    cout << ip1;
    cout << ", ";
    cout << ip2;
    cout << '\n';
}

void System::allocate_datagram(const IP_address& ip1,
                               const IP_address& ip2,
                               const string& message)
{
    shared_ptr<Node> machine1;

    for (int i = 0; i < network_.size(); i++) {

        // if the match for ip1 is found, set machine1 to that element
        if (network_[i] != nullptr && network_[i]->get_ip()==ip1) {
            machine1 = network_[i];
            machine1.get()->allocate_datagram(ip2,message);
            return;
        }
    }

    //throw error if machine not found
    throw err_code::no_such_machine;

    cout << "System::allocate_datagram: ";
    cout << ip1;
    cout << ", ";
    cout << ip2;
    cout << ", \"" << message << "\"\n";
}

void System::release_datagram(const IP_address& ip)
{
    //process the data and end the data transmission
    shared_ptr<Node> machine1;

    for (int i = 0; i < network_.size(); i++) {

        // if the match for ip1 is found, set machine1 to that element
        if (network_[i] != nullptr && network_[i]->get_ip()==ip) {
            machine1 = network_[i];
            //release datagram of given machine
            machine1.get()->release_datagram();
            return;
        }
    }

    //throw err if not found
    throw err_code::no_such_machine;


    return;

    cout << "System::release_datagram: ";
    cout << ip;
    cout << '\n';
}

void System::time_click()
{
    for (shared_ptr<Node> machine: network_) {
        machine.get()->send();
    }
    ++tick_;
    cout << "System::time_click\n";
}

std::ostream& operator<<(std::ostream& os, const System& sys)
{
    sys.format(os, {});
    return os;
}

