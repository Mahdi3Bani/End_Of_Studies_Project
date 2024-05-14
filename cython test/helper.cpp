#include "helper.h"

/*
int length_of_Syracuse_flight(const int &candidate){
  if (candidate==1){return 0;}
  if (candidate%2==0){
    return length_of_Syracuse_flight( candidate/2 ) +1; }
  
  return length_of_Syracuse_flight( candidate*3+1 ) +1 ;
}

void max_length_of_Syracuse_flight(const int &max_candidate, int &best_candidate, int &max_flight_length){
  max_flight_length=0;
  best_candidate=1;
  
  int new_length=0;
  
  for (int i=1; i<=max_candidate; i++){
    new_length = length_of_Syracuse_flight(i);
    if ( new_length > max_flight_length ){
      max_flight_length = new_length;
      best_candidate    = i;
    }
  }

  return ;
}
*/

unsigned int length_of_Syracuse_flight(const unsigned int &candidate){
  
  if (candidate==1){return 0;}
  if (candidate%2==0){
    return length_of_Syracuse_flight( candidate/2 ) +1; }
  if (candidate<=1431655765){
    return length_of_Syracuse_flight( candidate*3+1 ) +1 ;}
  else{
    std::cout<<"unsigned int size exceeded : 3 * " << candidate << " + 1 > 2^32 - 1\n";
    return -1;
  }
}

void max_length_of_Syracuse_flight(const unsigned int &max_candidate, unsigned int &best_candidate, unsigned int &max_flight_length){
  max_flight_length=0;
  best_candidate=1;
  
  unsigned int new_length=0;
  
  for (unsigned int i=1; i<=max_candidate; i++){
    new_length = length_of_Syracuse_flight(i);
    if ( new_length > max_flight_length ){
      max_flight_length = new_length;
      best_candidate    = i;
    }
  }

  return ;
}


int fibCpp(int a){
  std::cout << "Message from the .cpp file : we're computing fibCpp("<<a<<")\n";
  int x=0;
  int y=1;
  int z=0;
  for (int i=0; i<a;i++){
    z=y;
    y=x+y;
    x=z;
  }
  return y;
}
