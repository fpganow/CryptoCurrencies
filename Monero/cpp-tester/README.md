# cpp-tester
cpp-tester

This project reads the data from a binary file, (default: log_file.8.bin) and runs CryptoNight Step3 using this data.

The purpose of this project is to have actual data from an actual Monero Mining run so that one can develop an FPGA version and have a way of validating their code.

I copied a lot of the code from the original Monero project and put all that is required to run inside one single C++ file.  I also expanded some #define's to make it easier to make sense of what is happening.  I also created a similar implementation by assembling code that I found on the internet in Python.
