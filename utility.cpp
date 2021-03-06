#include "stdafx.h"
#include "utility.h"
#include <random>
#include <sstream>

std::random_device utility::rand;
std::mt19937 utility::rgen(utility::rand());
std::uniform_real_distribution<> utility::dist(0, 100);

const int utility::Window_width  = 480,
          utility::Window_height = 320;

int utility::True_width = 480,
    utility::True_height = 320;


float utility::R_Rand() { return dist(rgen); }


void utility::Output(std::string text) {
  theWorld.GetConsole()->WriteToOutput(text);
}


template <typename T>
T utility::R_Max(T x, T y) { return x > y ? x : y; }
  
template <typename T>
T utility::R_Min(T x, T y) { return x < y ? x : y; }
  
  
bool utility::Find_Substr(std::string& out, std::string in) {
  if ( in == "" ) return false;
  int it = 0, hit_count = 0;
  for ( auto i : out ) {
    if ( in[it++] == i ) ++hit_count;
    else { it = 0; hit_count = 0; }
    if ( hit_count == in.size() ) break;
  }
  if ( hit_count == in.size() ) return true;
    
  return false;
}
  
template <typename T>
T utility::Str_To_Type(std::string x) {
  std::stringstream x;
  x >> o;
  T i;
  x << i;
  return i;
}

int utility::GL_To_Angel(int x) {
  //return MathUtil::ScreenToWorld(
  return 0.0;
}


/*
void Do_Contact(b2Contact* contact) {


}

void utility::Contact_Listener::BeginContact(b2Contact* contact) {
  void* contact_data1 = contact->GetFixtureA()->GetBody()->GetUserData();
  void* contact_data2 = contact->GetFixtureB()->GetBody()->GetUserData();

  /*
  if ( contact_data1 != nullptr && contact_data2 != nullptr ) {// relevant contact made
   reinterpret_cast<Collision_Entity*>(contact_data1)->Set_Contact_Type(
    reinterpret_cast<Collision_Entity*>(contact_data1)->R_Entity_Type());
  }*//*
};

void utility::Contact_Listener::EndContact(b2Contact* contact) {
  Do_Contact(contact);
}*/