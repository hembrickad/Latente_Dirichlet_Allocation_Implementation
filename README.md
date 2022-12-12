# 2022-603-Project-Hembrickad

A general test of different implementations of the Latent Dirichlet Allocation Algorithm in an effort to increase speed of set up

## Utils.cpp & Utils.h

- Parses the CSV files contents into individual words per doument
- Input the CSV files for formatting

## SingleLDA.cpp & Housekeeping.cpp

- Single threaded version of the Latent Dirichlet Allocation algorithm and Pre-processing

## POSIXLIDA.cpp & ThreadedHousekeeping.cpp

- Multi-threaded version of the Latent Dirichlet Allocation algorithm and Setup with the use of mutex locks soley on the pre-processing methods

## MultiThreadingLDA.cpp & MultiThreadingHousekeeping.cpp

- Multi-threaded version of the Latent Dirichlet Allocation algorithm and Setup without the use of mutex locks on the pre-processing methods



