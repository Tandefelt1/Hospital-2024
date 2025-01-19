#include "hospital.hh"
#include "utils.hh"
#include <iostream>
#include <set>
#include "date.hh"
Hospital::Hospital()
{
}

// The destructor of Hospital.
// The biggest problem of the project. Deallocating patients_ is tricky
Hospital::~Hospital()
{
    // Deallocating staff
    for( std::map<std::string, Person*>::iterator
             iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        delete iter->second;
    }

    // Next all Persons and CarePeriods are deallocated
    // Quick note: This is the only way i have found to deallocate
    // without copious memory leaks. The choice of 'pair' in my
    // patients_ data structure was needlessly complicated
    // altough i have made it work since i have no time to start over

    // Iterate over each patient in the patients_ map
    for(auto& patient : patients_) {
        // Iterate through each pair in the vector
        for(auto& pair : patient.second) {
            // Delete the CarePeriod object
            delete pair.second;
            // Pop back the vector element to stop memory leaks
            patient.second.pop_back();
        }

    }

    // Iterate through the patient_list and deallocate all Persons.
    for (auto& person : patient_list){
        delete person;
    }

}
// The ready made set date method.
void Hospital::set_date(Params params)
{
    std::string day = params.at(0);
    std::string month = params.at(1);
    std::string year = params.at(2);
    if( not utils::is_numeric(day, false) or
        not utils::is_numeric(month, false) or
        not utils::is_numeric(year, false) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.set(stoi(day), stoi(month), stoi(year));
    std::cout << "Date has been set to ";
    utils::today.print();
    std::cout << std::endl;
}

// The method for advancing the date
void Hospital::advance_date(Params params)
{
    std::string amount = params.at(0);
    if( not utils::is_numeric(amount, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.advance(stoi(amount));
    std::cout << "New date is ";
    utils::today.print();
    std::cout << std::endl;
}

// A method to recruit staff
void Hospital::recruit(Params params)
{
    std::string specialist_id = params.at(0);
    if( staff_.find(specialist_id) != staff_.end() )
    {
        std::cout << ALREADY_EXISTS << specialist_id << std::endl;
        return;
    }
    Person* new_specialist = new Person(specialist_id);
    staff_.insert({specialist_id, new_specialist});
    std::cout << STAFF_RECRUITED << std::endl;
}

// A merhod to print all staff members in alphabetical order
void Hospital::print_all_staff(Params)
{
    if( staff_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }
    for( std::map<std::string, Person*>::const_iterator iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        std::cout << iter->first << std::endl;
    }
}

// A method to add medicine for patients.
void Hospital::add_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string strength = params.at(1);
    std::string dosage = params.at(2);
    std::string patient = params.at(3);
    if( not utils::is_numeric(strength, true) or
        not utils::is_numeric(dosage, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    std::map<std::string, Person*>::const_iterator
        patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }

    Person* patient_obj = patient_iter->second;
    if (!patient_obj) {
        std::cout << "Error: Invalid patient object." << std::endl;
        return;
    }

    patient_iter->second->add_medicine(medicine, stoi(strength), stoi(dosage));
    std::cout << MEDICINE_ADDED << patient << std::endl;
}

// A method to remove medicine from patients
void Hospital::remove_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string patient = params.at(1);
    std::map<std::string, Person*>::const_iterator
        patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->remove_medicine(medicine);
    std::cout << MEDICINE_REMOVED << patient << std::endl;
}

// A method to enter patients into the hospital
// the input is checked for current patients
// Next the input is checked for existing patients overall
// If the patient is new, a new person and care period are created
// and inserted to patients_, care_periods and patient_list
// If the patient exists, the patient and care period are inserted
// to patients_ and the care period to care_periods
void Hospital::enter(Params params) {
    std::string patient_id = params.at(0);

    // Check if the patient already exists in the current_patients_ map
    if (current_patients_.find(patient_id) != current_patients_.end()) {
        std::cout << ALREADY_EXISTS << patient_id << std::endl;
        return;
    }

    // Check if the patient already exists in the patients_ map
    auto it = patients_.find(patient_id);
    if (it == patients_.end()) {
        // If the patient is new, create a new Person object and a new care period
        Person* new_patient = new Person(patient_id);
        CarePeriod* new_period = new CarePeriod(utils::today, new_patient);
        current_patients_.insert({patient_id, new_patient});
        patients_.insert({patient_id, {{new_patient, new_period}}});
        patient_list.insert(new_patient);
        care_periods_.push_back(new_period);
    } else {
        // If the patient already exists, use the existing Person object and insert a new care period
        Person* existing_patient = it->second.back().first;
        current_patients_.insert({patient_id, existing_patient});
        CarePeriod* new_period = new CarePeriod(utils::today, existing_patient);
        patients_[patient_id].emplace_back(existing_patient, new_period); // Add new care period to existing patient
        care_periods_.push_back(new_period);
    }
    std::cout << PATIENT_ENTERED << std::endl;
}

// A method for patients to leave the hospital
// The input is checked for current patients
// the patient_id is removed from current_patients
// the last CarePeriods end date is added
void Hospital::leave(Params params) {
    std::string patient_id = params.at(0);

    auto it = current_patients_.find(patient_id);
    if (it == current_patients_.end()) {
        std:: cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    // Remove the patient from the current_patients_ map
    current_patients_.erase(patient_id);

    // Mark the end date of the current care period for the patient
    auto patient_info = patients_.find(patient_id);
    if (patient_info != patients_.end() && !patient_info->second.empty()) {
        auto& patient_care_period_vec = patient_info->second;
        auto& last_care_period = patient_care_period_vec.back();
        last_care_period.second->end_care(utils::today);
        std::cout << PATIENT_LEFT << std::endl;
    }
}

// A method to assign staff to the current care period of a patient
// staff_id and patient_id are checked for the corresponding maps
// The patients last and thus current care period is found
// the staff id assigned to the particular care period
// the patient id is added to assigned_patients to keep chronologiacl order
// (for print_care_periods)
void Hospital::assign_staff(Params params)
{

    std::string staff_id = params.at(0);
    std::string patient_id = params.at(1);

    // Find the patient in the current_patients_ map
    auto patient_it = current_patients_.find(patient_id);
    if (patient_it == current_patients_.end()) {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    // Find the staff member in the staff_ map
    auto staff_it = staff_.find(staff_id);
    if (staff_it == staff_.end()) {
        std::cout << CANT_FIND << staff_id << std::endl;
        return;
    }

    // Get the patient's active care periods from the patients_ map
    auto patient_info = patients_.find(patient_id);
    if (patient_info == patients_.end() || patient_info->second.empty()) {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    // Get the patient's active care period
    auto& patient_care_periods = patient_info->second;
    auto& last_care_period = patient_care_periods.back();

    // Assign the staff member to the patient's active care period
    last_care_period.second->assign_staff(staff_id);



    // add the patient to id to assigned_patients (to keep chronological order)
    assigned_patients.push_back(patient_id);
    std::cout << STAFF_ASSIGNED << patient_id << std::endl;
}
// A method to print out information on a particular patient
// patient_id is checked to find a patient in patients_.
// Information on patient care periods in chronological order
// and prescribed medicine is printed out
void Hospital::print_patient_info(Params params)
{
    std::string patient_id = params.at(0);

    // Check if the patient exists in patients_
    auto patient_it = patients_.find(patient_id);
    if (patient_it == patients_.end()) {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }


    // Print details of each care period for the patient in chronological order
    const auto& person_info = patient_it->second;
    for (const auto& care_period_pair : person_info) {
        CarePeriod* care_period_ptr = care_period_pair.second;
        if (care_period_ptr) {
            const CarePeriod& care_period = *care_period_ptr;
            std::cout << "* Care period: ";
            care_period.print_start();
            std::cout << " - ";
            if (!care_period.is_ended()) {
                std::cout << std::endl;
            } else {
                care_period.print_end();
                std::cout << std::endl;
            }

            std::cout << "  - Staff: ";
            care_period.print_staff();
            std::cout << std::endl;
        }
    }


    std::cout << "* Medicines:";
    const auto& person_ptr = person_info.front().first;
    const Person& person = *person_ptr;
    person.print_medicines("  - ");


}

// A method to print out all assigned patients and care periods of a staff member
// staff_id is found from staff_
// care_periods is iterated through to find all patient_ids that have
// been assigned to the staff member
// they are printed through chronologically
// alphabetical_patients exists because it contains unique names
// the name is erased from the set  after printing its info out
// so no duplicate information is printed out.
void Hospital::print_care_periods(Params params)
{
    std::set<std::string> alphabetical_patients;
    std::string staff_id = params.at(0);
    if (staff_.find(staff_id) == staff_.end()){
        // staff_id not found in the map
        std::cout << CANT_FIND << staff_id << std::endl;
        return;
    }

    for (auto& care_period : care_periods_) {
        if (care_period->is_assigned(staff_id)) {
            // staff_id is assigned to this care period, insert patient ID into alphabetical_patients
            alphabetical_patients.insert(care_period->get_id());

        }
    }
    if (alphabetical_patients.empty()){
        std::cout << "None" << std::endl;
        return;
    }
    for (const std::string& patient_id : assigned_patients) {
        auto test = alphabetical_patients.find(patient_id);
        if (test != alphabetical_patients.end()){
            auto patient_it = patients_.find(patient_id);
            if (patient_it == patients_.end()) {
                std::cout << CANT_FIND << patient_id << std::endl;
                return;
            }


            // Print details of each care period for the patient in chronological order
            const auto& person_info = patient_it->second;
            for (const auto& care_period_pair : person_info) {
                CarePeriod* care_period_ptr = care_period_pair.second;
                if (care_period_ptr->is_assigned(staff_id)) {
                    const CarePeriod& care_period = *care_period_ptr;

                    care_period.print_start();
                    std::cout << " - ";
                    if (!care_period.is_ended()) {
                        std::cout << std::endl;
                    } else {
                        care_period.print_end();
                        std::cout << std::endl;
                    }
                }

            }
            std::cout << "* Patient: " << patient_id << std::endl;
            alphabetical_patients.erase(patient_id);
        }


    }
}

// A method to print out all medicines and their recipient names.
// a set all_medicines is created. all unique medicines are inserted to it.
// Per medicine all recipients are collected to set people.
// Medicine name and patient ids are printed out for all medicines.
void Hospital::print_all_medicines(Params)
{
    std::set<std::string>all_medicines;
    for (const auto& person : patient_list){
        std::vector<std::string> personal_medicines = person->get_medicines();
        for (const auto& medicine : personal_medicines){
            all_medicines.insert(medicine);
        }
    }
    if (all_medicines.empty()){
        std::cout << "None" << std::endl;
        return;
    }
    for (const auto& medicine : all_medicines){
        std::set<std::string>people;
        for (const auto& person : patient_list){
            std::vector<std::string>personal_medicine = person->get_medicines();
            for (size_t i=0; i < personal_medicine.size(); ++i){
                if (personal_medicine[i]==medicine){
                    people.insert(person->get_id());
                }
            }
        }
        std::cout << medicine << " prescribed for" << std::endl;
        for (const auto& patient_id : people){
            std::cout << "* " << patient_id << std::endl;
        }

    }
}
// print_all_patients prints the relevant information on all patients
// Essenctially does the exact same thing as print_patient_information
// but does it in alphabetical order for all patients.
void Hospital::print_all_patients(Params)
{
    std::set<std::string> alphabetical_patients;
    std::map<std::string, std::vector<std::pair<Person*, CarePeriod*>>>::iterator it;
    for (it = patients_.begin(); it != patients_.end(); ++it) {
        const std::string& patient_id = it->first;
        alphabetical_patients.insert(patient_id);
    }
    if (alphabetical_patients.begin() == alphabetical_patients.end()){
        std::cout << "None" << std::endl;
        return;
    }
    for (const std::string& patient_id : alphabetical_patients) {
        std::cout << patient_id << std::endl;
        auto patient_it = patients_.find(patient_id);

        // Print details of each care period for the patient in chronological order
        const auto& person_info = patient_it->second;
        for (const auto& care_period_pair : person_info) {
            CarePeriod* care_period_ptr = care_period_pair.second;
            if (care_period_ptr) {
                const CarePeriod& care_period = *care_period_ptr;
                std::cout << "* Care period: ";
                care_period.print_start();
                std::cout << " - ";
                if (!care_period.is_ended()) {
                    std::cout << std::endl;
                } else {
                    care_period.print_end();
                    std::cout << std::endl;
                }

                std::cout << "  - Staff: ";
                care_period.print_staff();
                std::cout << std::endl;
            }
        }
        std::cout << "* Medicines:";
        const auto& person_ptr = person_info.front().first;
        const Person& person = *person_ptr;
        person.print_medicines("  - ");
    }
}

// print_all_patients prints the relevant information on all patients
// Essenctially does the exact same thing as print_patient_information
// but does it in alphabetical order for all CURRENT patients.
void Hospital::print_current_patients(Params) {
    if (current_patients_.empty()) {
        std::cout << "None" << std::endl;
        return;
    }

    std::set<std::string> alphabetical_patients;
    std::map<std::string, Person*>::iterator it;
    for (it = current_patients_.begin(); it != current_patients_.end(); ++it) {
        const std::string& patient_id = it->first;
        alphabetical_patients.insert(patient_id);
    }
    if (alphabetical_patients.begin() == alphabetical_patients.end()){
        std::cout << "None" << std::endl;
        return;
    }
    for (const std::string& patient_id : alphabetical_patients) {
        std::cout << patient_id << std::endl;
        auto patient_it = patients_.find(patient_id);

        // Print details of each care period for the patient in chronological order
        const auto& person_info = patient_it->second;
        for (const auto& care_period_pair : person_info) {
            CarePeriod* care_period_ptr = care_period_pair.second;
            if (care_period_ptr) {
                const CarePeriod& care_period = *care_period_ptr;
                std::cout << "* Care period: ";
                care_period.print_start();
                std::cout << " - ";
                if (!care_period.is_ended()) {
                    std::cout << std::endl;
                } else {
                    care_period.print_end();
                    std::cout << std::endl;
                }

                std::cout << "  - Staff: ";
                care_period.print_staff();
                std::cout << std::endl;
            }
        }
        std::cout << "* Medicines:";
        const auto& person_ptr = person_info.front().first;
        const Person& person = *person_ptr;
        person.print_medicines("  - ");
    }
}

