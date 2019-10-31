#include "company.hpp"
#include <iostream>
#include <cassert>

using namespace std;

CompanyTracker::CompanyTracker (int n)
  // initializes the tracker with n students and their 1-person companies
{
  numCompanies = n;
  companies = new Company* [numCompanies];
  for (int i = 0; i < numCompanies; ++i)
      companies[i] = new Company ();
}

CompanyTracker::~CompanyTracker ()
  // deallocates all dynamically allocated memory
{
  //deallocate all companies and their parents using Company dtor
  for (int i = 0; i < numCompanies; i++){
    delete companies[i];
  }
  delete[] companies; //deallocate companies
}

//find the biggest company that the student is a part of 
Company* CompanyTracker::findRoot(int student_number){
  Company* c = companies[student_number];
  //iterate through until
  while (c->parent){
    c = c->parent;
  }
  return c;
}

void CompanyTracker::merge (int i, int j)
  /* Merges the largest companies containing students i and j,
     according to the rules described above.
     That is, it generates a new Company object which will
     become the parent company of the largest companies currently
     containing students i and j.
     If i and j already belong to the same company (or are the same),
     merge doesn't do anything.
     If either i or j are out of range, merge doesn't do anything. */
{
  // your implementation goes here
  // if students are in the same company already
  if (!inSameCompany(i,j)){
    Company* parent = new Company(findRoot(i),findRoot(j));
    findRoot(i)->parent = parent;
    findRoot(j)->parent = parent;
  }
}

void CompanyTracker::split (int i)
  /* Splits the largest company that student i belongs to,
     according to the rules described above.
     That is, it deletes that Company object, and makes sure that
     the two subcompanies have no parent afterwards.
     If i's largest company is a 1-person company, split doesn't do anything.
     If i is out of range, split doesn't do anything. */
{
  // your implementation goes here
  // if there is a parent company, then split
  if(companies[i]->parent != nullptr){
    Company* root = findRoot(i); 
    //reassign parents
    root->merge1->parent = nullptr;
    root->merge2->parent = nullptr;
    delete root;
  }
}

bool CompanyTracker::inSameCompany (int i, int j)
  /* Returns whether students i and j are currently in the same company.
     Returns true if i==j.
     Returns false if i or j (or both) is out of range. */
{
  //out of bound error
  if (i >= numCompanies || j >= numCompanies|| j < 0 || i < 0){
    return false;
  }
  //check if the root company is the same
  else if (findRoot(i) == findRoot(j)){
    return true;
  } 
  else{
    return false;
  }
    
}

int main(){
  
}

