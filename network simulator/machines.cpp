#include "machines.h"
#include "errors.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>

using namespace std;

Node::Node(const std::string& n, IP_address ip)
        : local_ip_(ip), name_(n), incoming_(nullptr),
          node_list_(std::vector<shared_ptr<Node>>()), data_list_(nullptr)
{}

Node::~Node()
{
    if (incoming_ != nullptr) {
        delete incoming_;
        incoming_ = nullptr;
    }
}

void Node::format(ostream& os) const
{
    os << "node {\n";
    os << "    name = \"" << name_ << "\",\n    local_ip = ";
    os << local_ip_;
    os << ",\n    incoming_ = ";
    if (incoming_ == nullptr) os << "nullptr";
    else                      os << *incoming_;
    os << ",\n    node_list = [ ";
    for (auto& node : node_list_) {
        os << node->get_ip();
        os << ' ';
    }
    os << "],\n    data_list = ";
    list_format(os, data_list_);
    os << "\n  }";
}

IP_address Node::get_ip() const
{
    // returns the local ip address of the machine
    return local_ip_;
}

void Node::connect(const shared_ptr<Node>& x)
{
    // appends the new machine in the connected list
    if (x == nullptr) return;
        // if the machine passed is not a nullptr, append x to this node
    else node_list_.push_back(x);
}

void Node::disconnect(const shared_ptr<Node>& x)
{
    //  removes the designated machine from the connected list
    if (node_list_.size() == 0) return;

    // loop through node_list to find x
    for (int i = 0; i < node_list_.size(); i++) {
        // removing the element that matches x
        if (node_list_[i] == x) {
            node_list_[i] = node_list_.back();
            node_list_.pop_back();
            return;
        }
    }
}

size_t Node::send() {
    size_t count = 0;
    List new_list = List();
    List runner = new_list;

    // when node_list_ not empty, send out every Datagram* in data_list_
    //find in node list
    if (node_list_.empty() == false) {
        shared_ptr<Node> best;
        best = node_list_[0];
        shared_ptr<ListNode> dat_send;

        while (data_list_ != nullptr) {
            for (int i = 0; i < node_list_.size(); i++) {

                if (node_list_[i].get()->get_ip()
                    == data_list_.get()->data->get_destination()) {
                    dat_send = data_list_;
                    best = node_list_[i];
                    break;
                }
                    // otherwise update best
                else if (abs(node_list_[i].get()->get_ip().first_octad()
                             - data_list_.get()->data->get_destination().first_octad())
                         <= abs(best.get()->get_ip().first_octad()
                                - data_list_.get()->data->get_destination().first_octad())) {
                    best = node_list_[i];
                    dat_send = data_list_;
                }
            }

            try {
                //send datagram
                best->receive(dat_send.get()->data);
            }

            catch (err_code e) {
                //pushes d to the back of new list
                if (e != err_code::recv_blocked) throw e;
                else {
                    if (runner == nullptr) {
                        //runner =  datagram  (store it at runner)
                        runner.get()->data = dat_send.get()->data;
                    } else {
                        //runner.next = datagram
                        runner.get()->next = dat_send;
                        //runner = runner.next
                        runner = runner.get()->next;
                    }
                }
            }
            count++;

            // set data_list to next
            data_list_ = data_list_->next;
        }
    }
    //assign new list to list
    data_list_ = new_list;
    return count;
}

void Node::receive(Datagram* d) {
    // if the dest of Datagram is current, compare
    if (d->get_destination() == local_ip_) {
        if (incoming_ == nullptr) {
            incoming_ = d;
        } else {
            // if incoming is not nullptr, throw err
            throw err_code::recv_blocked;
        }
    } else {
        push_back(data_list_, d);
    }
}


void Node::allocate_datagram(const IP_address& dst, const string& message)
{
    // allocates a new Datagram in the memory
    // pushes the Datagram* to back of the linked-list data_list_
    Datagram* raw_ptr = new Datagram (get_ip(), dst, message);
    push_back(data_list_, raw_ptr);
}

string Node::release_datagram()
{
    string str = "";

    if (incoming_ != nullptr) {
        str = incoming_->get_msg();
        delete incoming_;
        incoming_ = nullptr;
    }
    return str;
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
    n.format(os);
    return os;
}
