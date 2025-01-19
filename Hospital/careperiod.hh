/* Class CarePeriod
 * ----------
 * COMP.CS.110 SPRING 2024
 * ----------
 * Class for describing a patient's care period in hospital.
 *
 * Note: Students need to implement almost all of this class by themselves.
 * */
#ifndef CAREPERIOD_HH
#define CAREPERIOD_HH

#include "person.hh"
#include "date.hh"
#include <string>
#include <set>

class CarePeriod
{
public:
    /**
      * @brief CarePeriod
      * @param start date given as a string (ddmmyyyy)
      * @param patient whose care period will be created
      */
    CarePeriod(const std::string& start, Person* patient);

    /**
      * @brief CarePeriod
      * @param start date given as a Date object
      * @param patient whose care period will be created
      */
    CarePeriod(const Date& start, Person* patient);


    /**
      * @brief destructor
      */
    ~CarePeriod();


    /**
     * @brief end_care
     * @param end given as a Date object
     * A method to give an end date to a care period
     */
    void end_care(const Date& end);

    /**
     * @brief assign_staff
     * @param staff
     * A Method to add staff ids to assigned_staff_
     * it's a set since there can be multiple staff per care period
     */
    void assign_staff(const std::string& staff);


    /**
     * @brief is_ended
     * @return a bool value. true if there is an end date
     *  false if there is no end date for the care period
     *  A method to check if the care period has ended.
     */
    bool is_ended() const;

    /**
     * @brief print_start
     * A method to print out the start_date
     */
    void print_start() const;
    /**
     * @brief print_end
     * A method to print out the end_date
     */
    void print_end()const;
    /**
     * @brief print_staff
     * A method to print out all assigned staff
     */
    void print_staff() const;

    /**
     * @brief is_assigned
     * @param staff
     * @return a bool value true if the staff is in assigned_staff
     *  false if it isn't
     *  A method to check is a staff id has been assigned to a care period
     */
    bool is_assigned(const std::string& staff) const;

    /**
     * @brief get_id
     * @return the patient_id of the patient_ Person.
     */
    std::string get_id();

private:
    Person* patient_;
    Date start_;
    std::string start_date;
    std::string end_date;
    Date end_;
    std::set<std::string> assigned_staff_;
    bool ended_ = false;

    // More attributes and methods
};

#endif // CAREPERIOD_HH
