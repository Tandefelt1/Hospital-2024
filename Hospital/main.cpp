/* Hospital
 *
 * Desc:
 *    This program implements a hospital reception
 * Each patient or staff member that enters has a Person class object created for them
 * This object contains information on patients and staff. Information on staff members and
 * patients can be accessed with various commands that have been implemented through files cli and utils
 * The commands are as follows:
 * -Recruit 'staff_id':
 * A method to recruit staff members to the hospital. The user inputs "recruit" and a chosen name.
 * Then a person with that id is created and stored in the map staff_
 * -Enter 'patien_id':
 * The same as recruit but for patients. In addition to the person creation, enter creates a CarePeriod
 * for the arriving patient. Unlike in the case of recruit, patients can leave and come back.
 * This means they can have multiple care periods per person. care periods contain information on patient
 * arrival date and assigned staff
 * -Leave 'patient_id':
 * A way for patients to leave the hospital, ending the current care period. Patients can re-enter afterwards
 * to start a new care period
 * -Add_medicine 'medicine' 'dose' 'amount' 'patient_id'
 * prescribe medicine to a patient, specifying the medicine, dose and amount. this stays between care periods
 * -Remove_medicine 'medicine' 'patient_id'
 * Remove the prescribed medicine from the patient
 * -Print_patient_info 'patient_id':
 * prints out relevant information on the patients care periods and medicine
 * -Print_care_periods 'staff_id':
 * Prints out information on patients and their care periods the staff member has been assigned for
 * -Print_all_medicines:
 * Prints out all medicines and the patients who have been assigned them in alphabetical order
 * -Print_all_patients:
 * Essentially the same as print_patient_info but for all patients in the hospital records
 * -Print_current_patients:
 * Essentially the same as print_patient_info but for all CURRENT patients in the hospital records
 * -quit
 * deallocates all used memory, destroying hospital and ending the program
 *
 *
 * Program author:
 * Name: Santeri Kasper
 * Student number: *******
 * UserID: ****
 * E-mail: santeri.kasper@tuni.fi
 *
 * Quick note: I'm aware the data structure patients_ is awful for memory.
 * I was not aware of this at the start of the project. I would change it if I had the time
 * This has caused me long hours of annoyance and probably lowers my point total considerably...
 * I'm kind of proud I managed to salvage this program though, as it was a good learing experience on dynamic memory management...
 *
 * For potential hiring companies, if i made that shit work, i can do anything.
 **/
#include "cli.hh"
#include "hospital.hh"
#include <string>

const std::string PROMPT = "Hosp> ";


int main()
{
    Hospital* hospital = new Hospital();
    Cli cli(hospital, PROMPT);
    while ( cli.exec() ){}

    delete hospital;
    return EXIT_SUCCESS;
}
