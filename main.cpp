/*main.cpp*/

//
// <Caglar Kurtkaya>
// U. of Illinois, Chicago
// CS 341, Fall 2019
// Project #03: GradeUtil UI
// This program is C++11 dependent
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

// includes for gradeutil
#include "gradeutil.h"

College InputGradeData(string filename)
{
    College college;
    ifstream file(filename);
    string line, value;

    if (!file.good())
    {
        cout << "**Error: unable to open input file '" << filename << "'." << endl;
        return college;
    }

    // first line contains semester,year
    getline(file, line);
    stringstream ss(line);

    getline(ss, college.Name, ',');
    getline(ss, college.Semester, ',');
    getline(ss, value);
    college.Year = stoi(value);

    // second line contains column headers --- skip
    getline(file, line);

    //
    // now start inputting and parse course data:
    //

    while (getline(file, line))
    {
        Course c = ParseCourse(line);

        //
        // search for correct dept to ask course to, otherwise create a new dept:
        //
        auto dept_iter = std::find_if(college.Depts.begin(),
                                      college.Depts.end(),
                                      [&](const Dept &d) {
                                          return (d.Name == c.Dept);
                                      });

        if (dept_iter == college.Depts.end())
        {
            //
            // doesn't exist, so we have to create a new dept
            // and insert course:
            //
            Dept d(c.Dept);

            d.Courses.push_back(c);

            college.Depts.push_back(d);
        }
        else
        {
            // dept exists, so insert course into existing dept:
            dept_iter->Courses.push_back(c);
        }

    } //while

    //
    // done:
    //
    return college;
}

// TODO: define your own functions
//
//
//
//


//=========================================================
//Prints the college info
void collegeInfo(const College& college){
  GradeStats gStats = GetGradeDistribution(college);
  int n = 0;
  int dfw = 0;
  
  cout << "** College of " << college.Name << ", " << college.Semester << " " << college.Year << " **" <<  endl;
  cout << "# of courses taught: " << college.NumCourses() << endl;
  cout << "# of students taught: " << college.NumStudents() << endl;
  cout << "grade distribution (A-F): " 
    << gStats.PercentA << "%, "
    << gStats.PercentB << "%, "
    << gStats.PercentC << "%, "
    << gStats.PercentD << "%, "
    << gStats.PercentF << "%" << endl;
  cout <<"DFW rate: " << GetDFWRate(college,dfw,n) << "%" << endl; 
    
}


//=========================================================
//Prints the summary of a dept or all depts in a college

void summaryCmd(const College& college){
  string deptName;
  int n = 0;
  int dfw = 0;
  
  cout << "dept name, or all? " ;
  cin >> deptName;
  
  
  
  if(deptName == "all"){
    
    for(const Dept& d : college.Depts){
      cout << d.Name << ":" << endl;
      cout << " # courses taught: " << d.NumCourses() << endl;
      cout << " # students taught: " << d.NumStudents() << endl;
      GradeStats deptStats = GetGradeDistribution(d);
      cout << " grade distribution (A-F): " 
    << deptStats.PercentA << "%, "
    << deptStats.PercentB << "%, "
    << deptStats.PercentC << "%, "
    << deptStats.PercentD << "%, "
    << deptStats.PercentF << "%" << endl;
      cout <<" DFW rate: " << GetDFWRate(d,dfw,n) << "%" << endl; 
    }
  }
  else{
    
    auto iter = find_if(college.Depts.begin(), college.Depts.end(), [=](const Dept& d1){
      if(d1.Name == deptName){
        return true;
      }
      else{
        return false;
      }
    });
    
    //no dept found, promt error message
    if(iter == college.Depts.end()){
      cout << "**dept not found" << endl;
    }
    //we found the dept
    else{
      cout << iter->Name << ":" << endl;
      cout << " # courses taught: " << iter->NumCourses() << endl;
      cout << " # students taught: " << iter->NumStudents() << endl;
      GradeStats deptStats = GetGradeDistribution(*iter);
      cout << " grade distribution (A-F): " 
    << deptStats.PercentA << "%, "
    << deptStats.PercentB << "%, "
    << deptStats.PercentC << "%, "
    << deptStats.PercentD << "%, "
    << deptStats.PercentF << "%" << endl;
      cout <<" DFW rate: " << GetDFWRate(*iter,dfw,n) << "%" << endl; 
      
    }
    
  }
  
}

//=========================================================
//Prints info about courses in a vector

void printCourseInfo(const vector<Course>& allCourses){
  int n = 0;
  int dfw = 0;
  for(const Course& c : allCourses){
    cout << c.Dept << " " << c.Number << " (section " << c.Section << ")"<< ": " << c.Instructor << endl;
    cout << " # students: " << c.getNumStudents() << endl;
    if(c.getGradingType() == Course::Letter){
      cout << " course type: letter" << endl;
    }
    else if(c.getGradingType() == Course::Satisfactory){
      cout << " course type: satisfactory" << endl;
    }
    else{
      cout << " course type: unknown" << endl;
    }
    
    GradeStats cGradeStats =  GetGradeDistribution(c);
    cout << " grade distribution (A-F): " 
    << cGradeStats.PercentA << "%, "
    << cGradeStats.PercentB << "%, "
    << cGradeStats.PercentC << "%, "
    << cGradeStats.PercentD << "%, "
    << cGradeStats.PercentF << "%" << endl;
    cout <<" DFW rate: " << GetDFWRate(c,dfw,n) << "%" << endl;
  }
  
}

//=========================================================
//
void printCourseInfo2(const vector<Course>& allCourses){

  for(const Course& c : allCourses){
    cout << c.Dept << " " << c.Number << " (section " << c.Section << ")"<< ": " << c.Instructor << endl;
    cout << " # students: " << c.getNumStudents() << endl;
    if(c.getGradingType() == Course::Letter){
      cout << " course type: letter" << endl;
    }
    else if(c.getGradingType() == Course::Satisfactory){
      cout << " course type: satisfactory" << endl;
    }
    else{
      cout << " course type: unknown" << endl;
    }
  }
  
}
//=========================================================
//
void searchCmd(const College& college){
  string userInputDept;
  string instructorPrefix;
  int courseNum;
  

  
  cout << "dept name, or all? ";
  cin >> userInputDept;
  cout << "course # or instructor prefix? ";
  cin >> instructorPrefix;
  
  stringstream ss(instructorPrefix);
  ss >> courseNum;
  
  if(userInputDept == "all"){
    if(ss.fail()){
      // conversion failed, input is not numeric
      vector<Course> allCourses =  FindCourses(college, instructorPrefix);
      if(allCourses.size() == 0){
        cout <<"**none found" << endl;
      }
      //we found macthing course/s
      else{
        //print out info
        printCourseInfo(allCourses);
      }
    }
    else{
      // conversion worked, courseNum contains numeric value
       vector<Course> allCourses =  FindCourses(college, courseNum);
       if(allCourses.size() == 0){
        cout <<"**none found" << endl;
       }
       //we found macthing course/s
       else{
         //print out info
         printCourseInfo(allCourses);
       } 
     }
    }
  else{
    auto iter = find_if(college.Depts.begin(), college.Depts.end(),
                       [=](const Dept& d){
                         if(d.Name == userInputDept){
                           return true;
                         }
                         else{
                           return false;
                         }                         
                       });
    if(iter == college.Depts.end()){
      cout << "**dept not found" << endl;
    }
    else{
       if(ss.fail()){
          // conversion failed, input is not numeric
          vector<Course> allCourses =  FindCourses(*iter, instructorPrefix);
          if(allCourses.size() == 0){
            cout <<"**none found" << endl;
           }
          //we found macthing course/s
          else{
            //print out info
            printCourseInfo(allCourses);
          }
        }
        else{
          // conversion worked, courseNum contains numeric value
           vector<Course> allCourses =  FindCourses(*iter, courseNum);
           if(allCourses.size() == 0){
            cout <<"**none found" << endl;
           }
           //we found macthing course/s
           else{
             //print out info
             printCourseInfo(allCourses);
           } 
      
      }
    }
  }
}

//=========================================================
//

void satisfactoryCmd(const College& college){
  string userInputS;
  
  cout << "dept name, or all? ";
  cin >> userInputS;
  
  //vector of Courses to store the ones with grading type satisfactory
  vector<Course> myCourses;
  
  if(userInputS == "all"){
    for(const Dept& d : college.Depts){
      for(const Course& c : d.Courses){
        if(c.getGradingType() == Course::Satisfactory){
          myCourses.push_back(c);
        }        
      }
      if(myCourses.size() == 0){
        cout << "**none found" << endl;
      }
    }  
  }
  else{
    auto iter = find_if(college.Depts.begin(), college.Depts.end(), [=](const Dept& d){
      if(d.Name == userInputS){
        return true;
      }
      else{
        return false;
      }
    });
    
    if(iter == college.Depts.end()){
      cout << "**dept not found" << endl;
    }
    else{
      for(const Course& c1 : iter->Courses){
        if(c1.getGradingType() == Course::Satisfactory){
          myCourses.push_back(c1);
        }
      }
    }
     if(myCourses.size() == 0){
        cout << "**none found" << endl;
      }
  }
  
    sort(myCourses.begin(), myCourses.end(), 
      [](const Course& c1, const Course& c2)
      {
        if (c1.Dept < c2.Dept)
          return true;
        else if (c1.Dept > c2.Dept)
          return false;
        else // same dept, look at course #:
          if (c1.Number < c2.Number)
            return true;
          else if (c1.Number > c2.Number)
            return false;
          else // same course #, look at section #:
            if (c1.Section < c2.Section)
              return true;
            else 
              return false;
      }
    );
  
  
  
  printCourseInfo2(myCourses);
  
}

//=========================================================
void dfwCmd(const College& college){
  string userInputDept;
  string userInputThold;
  double sToD;
  
  int dfw = 0;
  int n = 0;
  double dfwRate = 0.0;
  
  cout << "dept name, or all? ";
  cin >> userInputDept;
  
  cout << "dfw threshold? ";
  cin >> userInputThold;
  
  stringstream ss(userInputThold);
  ss >> sToD;
  
  vector<Course> myCourses;
  
  if(userInputDept == "all"){
    for(const Dept& d : college.Depts){
      for(const Course& c : d.Courses){
        dfwRate = GetDFWRate(c,dfw,n);
        if(dfwRate > sToD){
          myCourses.push_back(c);
        }
      }
    }
    
    if(myCourses.size() == 0){
      cout << "**none found" << endl;
    }   
 
  }
  else{
    auto iter = find_if(college.Depts.begin(),college.Depts.end(),
                       [=](const Dept& d){
                         if(d.Name == userInputDept){
                           return true;
                         }
                         else{
                           return false;
                         }
                       });
    if(iter == college.Depts.end()){
      cout << "**dept not found" << endl;
    }
    else{
      for(const Course& c : iter->Courses){
        dfwRate = GetDFWRate(c,dfw,n);
        if(dfwRate > sToD){
          myCourses.push_back(c);
        }
        
      }
      if(myCourses.size() == 0){
        cout << "**none found" << endl;
      }    
    }
     
  }
  
  
  sort(myCourses.begin(), myCourses.end(), 
      [](const Course& c1, const Course& c2)
      {
        int dfw1 = 0;
        int n1 = 0;
        int dfw2 = 0;
        int n2 = 0;
        double c1DFW = GetDFWRate(c1, dfw1,n1);
        double c2DFW = GetDFWRate(c2, dfw2,n2);
        
        if(c1DFW > c2DFW){
          return true;
        }
        else if(c1DFW < c2DFW){
          return false;
        }
        else if (c1.Dept < c2.Dept)
          return true;
        else if (c1.Dept > c2.Dept)
          return false;
        else // same dept, look at course #:
          if (c1.Number < c2.Number)
            return true;
          else if (c1.Number > c2.Number)
            return false;
          else // same course #, look at section #:
            if (c1.Section < c2.Section)
              return true;
            else 
              return false;
      }
    );
  
  printCourseInfo(myCourses);
}



//=========================================================

void letterBcmd(const College& college){
  string userInput;
  string userInputThold;
  double threshold;
  GradeStats cGradeStats;
  vector<Course> myCourses;
    
  cout << "dept name, or all? ";
  cin >> userInput;
  
  cout << "letter B threshold? ";
  cin >> userInputThold;
  
  stringstream ss(userInputThold);
  ss >> threshold;
  
  
  if(userInput == "all"){
    for(const Dept& d : college.Depts){
        for(const Course& c :  d.Courses){
          cGradeStats =  GetGradeDistribution(c);
          if(cGradeStats.PercentB > threshold){
            myCourses.push_back(c);
          }          
        }
    }
    
    if(myCourses.size() == 0){
       cout << "**none found" << endl;
    }
  }
  else{
    auto iter = find_if(college.Depts.begin(), college.Depts.end(),
                       [=](const Dept& d){
                         if(d.Name == userInput){
                           return true;
                         }
                         else{
                           return false;
                         }
                       });
    
    if(iter == college.Depts.end()){
      cout << "**dept not found" << endl;
    }
    else{
      for(const Course& c : iter->Courses){
         cGradeStats =  GetGradeDistribution(c);
          if(cGradeStats.PercentB > threshold){
            myCourses.push_back(c);
          }   
      }
      if(myCourses.size() == 0){
        cout << "**none found" << endl;
      }
    }  
    
  }
  
  
  sort(myCourses.begin(), myCourses.end(), 
      [](const Course& c1, const Course& c2)
      {
        GradeStats c1Gstats = GetGradeDistribution(c1);
        GradeStats c2Gstats = GetGradeDistribution(c2);
        
        
        if(c1Gstats.PercentB > c2Gstats.PercentB){
          return true;
        }
        else if(c1Gstats.PercentB < c2Gstats.PercentB){
          return false;
        }
        else if (c1.Dept < c2.Dept)
          return true;
        else if (c1.Dept > c2.Dept)
          return false;
        else // same dept, look at course #:
          if (c1.Number < c2.Number)
            return true;
          else if (c1.Number > c2.Number)
            return false;
          else // same course #, look at section #:
            if (c1.Section < c2.Section)
              return true;
            else 
              return false;
      }
    );
  
  printCourseInfo(myCourses);
  
}

//=========================================================

void printDeptGPA(const vector<Dept>& myDepts){
  GradeStats deptGstats;
  double dGPA = 0.00;
  double dcGPA = 0.00;
  int count = 0;
  map<double, string> mymap;
  
  
  for(const Dept& d : myDepts){
    for(const Course& c : d.Courses){
      if(c.getGradingType() == Course::Letter){
        deptGstats = GetGradeDistribution(c);  
        dcGPA =  ((deptGstats.NumA * 4.00) + (deptGstats.NumB *3.00) + (deptGstats.NumC * 2.00) + (deptGstats.NumD * 1.00) )  / (deptGstats.N);
        dGPA = dGPA + dcGPA;
        count++;
      }
      
     
    }
    
    
    mymap.insert(pair<double,string>(dGPA/count, d.Name));
    count = 0;
    dGPA = 0;
    count = 0;
  }
  
  //print out in reverse order
 for(auto rit = mymap.rbegin(); rit != mymap.rend(); ++rit)
{
   cout << "Overall GPA for " << rit->second<< " : " << rit->first << endl;
   
}
 
  
}

//=========================================================
bool compare(const pair<double, Course>&i, const pair<double, Course>&j)
{
  if(i.first > j.first)
    return true;
  else if(i.first < j.first)
    return false;
  else if (i.second.Dept < j.second.Dept)
    return true;
  else if (i.second.Dept > j.second.Dept)
    return false;
  else // same dept, look at course 
    if (i.second.Number < j.second.Number)
      return true;
    else if (i.second.Number > j.second.Number)
      return false;
    else // same course #, look at section #:
      if (i.second.Section < j.second.Section)
         return true;
      else 
         return false;  
  
}
//=========================================================

void printDeptGPA2(const vector<Course>& myCourses){
  GradeStats deptGstats;
  double dcGPA = 0.00;
  vector< pair <double,Course> > myVect; 
  
  for(const Course& c : myCourses){
    deptGstats = GetGradeDistribution(c);  
    dcGPA =  ((deptGstats.NumA * 4.00) + (deptGstats.NumB *3.00) + (deptGstats.NumC * 2.00) + (deptGstats.NumD * 1.00) )  / (deptGstats.N);
    myVect.push_back(pair<double,Course>(dcGPA,c));
    
  }
  
  sort(myVect.begin(),myVect.end(),compare);
  
  for(auto c : myVect){
    cout << "Overall GPA for " << c.second.Dept << " " << c.second.Number << "("<<c.second.Section << ")" << " : " << c.first << endl;
  }
  
}

//=========================================================

void averageCmd(const College& college){
  string userInput;
  vector<Dept> myDepts;
  vector<Course> myCourses;
  Course all;

  cout << "dept name, or all? ";
  cin >> userInput;


  if(userInput == "all"){
    for(const Dept& d : college.Depts){
      
      myDepts.push_back(d);

      }
    
    
    printDeptGPA(myDepts);

    }
  else{
    auto iter = find_if(college.Depts.begin(), college.Depts.end(),
                       [=](const Dept& d){
                         if(d.Name == userInput){
                           return true;
                         }
                         else{
                           return false;
                         }
                       });
    if(iter == college.Depts.end()){
      cout << "not found" << endl;
    }
    else{
      for(const Course& c : iter->Courses){
         if(c.getGradingType() == Course::Letter){
          myCourses.push_back(c);
        }
      }
    }
    printDeptGPA2(myCourses);
  } 
}

//=========================================================

int main()
{
    string filename;
    string userInput;

    cout << std::fixed;
    cout << std::setprecision(2);

    //
    // 1. Input the filename and then the grade data:
    //
    cin >> filename;
    // filename = "fall-2018.csv";

    College college = InputGradeData(filename);
  
    //sorting depts in college by Name
    sort(college.Depts.begin(), college.Depts.end(),[](const Dept& d1, const Dept& d2){
      if(d1.Name < d2.Name){
        return true;
      }
      else{
        return false;
      }
    });

    // 2. TODO: print out summary of the college
    // DEFINE your own functions
    collegeInfo(college);
    cout << endl;

    //
    // 3. TODO: Start executing commands from the user:
    // DEFINE your own functions
    
    cout << "Enter a command> ";
    cin >> userInput;
  
    while(userInput != "#"){
      
      if(userInput == "summary"){
        summaryCmd(college);
        
      }
      else if(userInput == "search"){
        searchCmd(college);
      }
      else if(userInput == "satisfactory"){
        satisfactoryCmd(college);
      }
      else if(userInput == "dfw"){
        dfwCmd(college);
      }
      else if(userInput == "letterB"){
        letterBcmd(college);
      }
      else if(userInput == "average"){
         averageCmd(college);
      }
      else{
         cout << "**unknown command" << endl;
      }
      
      
      cout << "Enter a command> ";
      cin >> userInput;
    }

    //
    // done:
    //
    return 0;
}