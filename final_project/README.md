####Data Stucture and Algorithm Final Project Team 9

##TEAM MEMBER

-National Taiwan University Department of Computer Science and Information Engineering

B03902017	曹峻寧 
B03902071	葉奕廷
B03902104	趙一穎

##ABSTRACT

main.cpp the main function of this program

history.h / history_LL.h provide class incident and essential operation

bank.h / bank_um.h provide class bank and essential operation

bonus.h / bonus.cpp provide bonus feature

the whole program is final project of DSA 2015,which is a bank account managing system
our bonus feature is adding loan system

for more details, you can read our report

##COMPILE
the codes can be compile with Makefile

$make :default setting(recommend)

$make umap: version with std::unordered_map and linked list -- it's default

$make map : version with std::map and vector -- compare with default

$make bonus : version with bonus features, based on umap version

##COMPETITION RESULT

$UMAP Version 

without linklist	350747~357099

with all optimization	385707~425351

$MAP Version

with vector	305178~336443 

##TESTDATA
We have tested speed and correctness of our create operation for score_3 , score_4 , score_6 , score_10.

There are testdatas for score_3 , score_4 , score_6 in create_test file.
score_10 is too big to upload.

##REPORT
the report of this project is in report.pdf
which include some analysis and discussion of the program
