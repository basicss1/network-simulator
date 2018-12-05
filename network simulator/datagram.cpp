#include "datagram.h"
#include "parsing.h"
#include "errors.h"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

IP_address::IP_address(string s)
{
    int i = 0;
    this->ip_[i] = 0;
    bool read_num = false;

    // throw error if the string is empty
    if (s.empty()) throw err_code::bad_ip_address;

    // loop through each char in string
    for(char each:s) {
        // initialize the result vector and check for dots after each number
        if(each == '.' && read_num == true) {
            i++;
            this->ip_[i] = 0;
            read_num = false;
        }
            // check if the string is valid and store the IP address in result vector
        else if(each-'0'>=0 && each-'0'<=9) {
            int a = each-'0';
            this->ip_[i] = this->ip_[i]*10 + a;
            read_num = true;
        }
            //throw error code otherwise
        else {
            throw err_code::bad_ip_address;
        }
    }

    // check the range of numbers in IP Address
    for (int each: this->ip_) {
        if(each<0 || each>255) {
            throw err_code::bad_ip_address;
        }
    }

    // check the quantity of numbers in IP Address (should equal 4)
    if (i != 3 || read_num == false) {
        throw err_code::bad_ip_address;
    }
    else return;
}

bool IP_address::operator==(const IP_address& that) const
{
    //loop through both IP_address
    for(int i=0; i<4; i++) {
        //check if each number equals, if not, return false
        if(this->ip_[i]!= that.ip_[i])  {
            return false;
        }
    }

    //return true if no discrepency is found
    return true;
}

int IP_address::first_octad() const
{
    //return the first number in an IP address
    return this->ip_[0];
}

std::ostream& operator<<(std::ostream& os, const IP_address& addr)
{
    return os << '"'
              << addr.ip_[0] << '.'
              << addr.ip_[1] << '.'
              << addr.ip_[2] << '.'
              << addr.ip_[3] << '"';
}

Datagram::Datagram(const IP_address& s, const IP_address& d, const string& m)
        : src_(s), dst_(d), msg_(m)
{};

IP_address Datagram::get_destination() const
{
    // returns the destination IP address of the datagram
    return this->dst_;
}

string Datagram::get_msg() const
{
    return this->msg_;
}

std::ostream& operator<<(std::ostream& os, const Datagram& dg)
{
    os << "datagram {";
    os << "src=" << dg.src_;
    os << ", dst=" << dg.dst_;
    os << ", length=" << dg.length_;
    os << ", msg=\"" << dg.msg_ << "\"";
    os << "}";
    return os;
}
