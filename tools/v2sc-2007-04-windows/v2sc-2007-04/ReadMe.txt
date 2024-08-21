To run V2SC in command line:

$v2sc input_verilog_file.v [-p] [-n] [-d] [-r] [-m mName]

There are some commandline options for obtaining best output, depending on 
the nature of the input Verilog code. Each of the above options are 
interpreted by V2SC as follows:


V2SC Options:

-p: Each module in Verilog is translated into a SC_MODULE SystemC class. If "-p" option 
    is present, if there were any parameters declared in the Verilog module, a templated 
    SC_MODULE class would be generated through which the parameters are passed into the 
    class. At the default case, i.e., while no "-p" option is present, parameters are all 
    interpreted as static constants in the corresponding SC_MODULE class.

-n: While present, each nonblocking assignments is mapped into an individual seperate
    process in the target SystemC file. At the default case, i.e., while no "-n" option
    is present, all the nonblocking assignments are mapped into a single process in the 
    target SystemC file. 

-d: While present, delays and events are ignored. At the default case, i.e., while no 
    "-d" option is present, delays and events are supported.

-r: While present, this product is registered in your computer. It's needed only at the first run,
    once v2sc is registered in your computer, it will run at the next time with no trouble.

-m: While present, module named "mName" will be assumed as the top level module. A verilog_file_name_main.cpp
    file will be generated in which module mName will be instantiated and portmapped with the 
    appropriate declared signals. At the default case, i.e., while no "-m" option is present, 
    no verilog_file_name_main.cpp file is generated. 
