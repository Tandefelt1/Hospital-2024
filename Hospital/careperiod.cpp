#include "careperiod.hh"
#include <iostream>

CarePeriod::CarePeriod(const std::string& start, Person* patient):
    patient_(patient), start_(start)
{
}

CarePeriod::CarePeriod(const Date &start, Person* patient):
    patient_(patient), start_(start)
{
}

CarePeriod::~CarePeriod()
{
}

void CarePeriod::end_care(const Date& end){
    end_ = end;
    ended_ = true;
}

void CarePeriod::assign_staff(const std::string& staff) {
    assigned_staff_.insert(staff);
}

void CarePeriod::print_start() const{
    start_.print();
}

void CarePeriod::print_end() const
{
    end_.print();
}

void CarePeriod::print_staff() const
{
    if (assigned_staff_.empty()){
        std::cout << "None";
    }
    else{
        for (const auto& member : assigned_staff_) {
            std::cout << member << " ";
        }
    }
}

bool CarePeriod::is_assigned(const std::string &staff_id) const
{
    for (const auto& member : assigned_staff_){
        if (member == staff_id){
            return true;
        }
    }
    return false;

}

std::string CarePeriod::get_id()
{
    return patient_->get_id();
}

bool CarePeriod::is_ended() const
{
    if (ended_){
        return true;
    }
    else{
        return false;
    }
}
