# cython: c_string_type=unicode, c_string_encoding=utf8
from libc.stdint cimport uint64_t

cdef extern from "helper.h":
    void max_length_of_Syracuse_flight(const unsigned int &max_candidate, unsigned int &best_candidate, unsigned int &max_flight_length)

    
def w_max_length_of_Syracuse_flight( const unsigned int &max_candidate, unsigned int &best_candidate, unsigned int &max_flight_length ):
    max_length_of_Syracuse_flight( max_candidate, best_candidate, max_flight_length )
    return best_candidate, max_flight_length

cdef extern from "helper.h":
    int fibCpp(int a)


def w_fib(a) :
    print(f"message from the .pyx file : we get the  values a={a} ")
    print(f"we now invoke fibCpp(a)")
    answer = fibCpp(a);
    print(f"message from the .pyx file : the C function returned {answer}")
    return answer
    
