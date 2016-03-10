This is an example of a printer server with multiple clients written in C. Assignment2 for Comp 310
To Test:
First enter "make" OR "gcc -o server server.c -lrt -Pthread " and "gcc -o client client.c -lr -Pthread" 

then to start the server type: "./server &"

then to start a client and job type "./client 2 3" where 2 is the client number and 3 is the number of seconds the job should take. 

Example output: 

asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ make
gcc -o server server.c -lrt -pthread
gcc -o client client.c -lrt -pthread
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./server &
[1] 22359
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./client 5 5
Client 5 has a 5 second job to print. Puts request in Buffer[0].
Printer 1 has started printing job from Client: 5, from Buffer[0]. The job will take 5 seconds. 
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./client 4 4
Client 4 has a 4 second job to print. Puts request in Buffer[1].
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ Printer 1 has finished its job from Buffer[0], the job took 5 seconds 
Printer 1 has started printing job from Client: 4, from Buffer[1]. The job will take 4 seconds. 
Printer 1 has finished its job from Buffer[1], the job took 4 seconds 
./client 9 5
Client 9 has a 5 second job to print. Puts request in Buffer[2].
Printer 1 has started printing job from Client: 9, from Buffer[2]. The job will take 5 seconds. 
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./client 4 7
Client 4 has a 7 second job to print. Puts request in Buffer[3].
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./client Printer 1 has finished its job from Buffer[2], the job took 5 seconds 
Printer 1 has started printing job from Client: 4, from Buffer[3]. The job will take 7 seconds. 
3 3
Client 3 has a 3 second job to print. Puts request in Buffer[4].
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ Printer 1 has finished its job from Buffer[3], the job took 7 seconds 
Printer 1 has started printing job from Client: 3, from Buffer[4]. The job will take 3 seconds. 
Printer 1 has finished its job from Buffer[4], the job took 3 seconds 


asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./client 5 5
Client 5 has a 5 second job to print. Puts request in Buffer[0].
Printer 1 has started printing job from Client: 5, from Buffer[0]. The job will take 5 seconds. 
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./client 4 4
Client 4 has a 4 second job to print. Puts request in Buffer[1].
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ Printer 1 has finished its job from Buffer[0], the job took 5 seconds 
Printer 1 has started printing job from Client: 4, from Buffer[1]. The job will take 4 seconds. 
Printer 1 has finished its job from Buffer[1], the job took 4 seconds 
./client 9 5
Client 9 has a 5 second job to print. Puts request in Buffer[2].
Printer 1 has started printing job from Client: 9, from Buffer[2]. The job will take 5 seconds. 
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./client 4 7
Client 4 has a 7 second job to print. Puts request in Buffer[3].
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./client Printer 1 has finished its job from Buffer[2], the job took 5 seconds 
Printer 1 has started printing job from Client: 4, from Buffer[3]. The job will take 7 seconds. 
3 3
Client 3 has a 3 second job to print. Puts request in Buffer[4].
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ Printer 1 has finished its job from Buffer[3], the job took 7 seconds 
Printer 1 has started printing job from Client: 3, from Buffer[4]. The job will take 3 seconds. 
Printer 1 has finished its job from Buffer[4], the job took 3 seconds 

asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./client 5 5
Client 5 has a 5 second job to print. Puts request in Buffer[0].
Printer 1 has started printing job from Client: 5, from Buffer[0]. The job will take 5 seconds. 
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./client 4 4
Client 4 has a 4 second job to print. Puts request in Buffer[1].
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ Printer 1 has finished its job from Buffer[0], the job took 5 seconds 
Printer 1 has started printing job from Client: 4, from Buffer[1]. The job will take 4 seconds. 
Printer 1 has finished its job from Buffer[1], the job took 4 seconds 
./client 9 5
Client 9 has a 5 second job to print. Puts request in Buffer[2].
Printer 1 has started printing job from Client: 9, from Buffer[2]. The job will take 5 seconds. 
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./client 4 7
Client 4 has a 7 second job to print. Puts request in Buffer[3].
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ ./client Printer 1 has finished its job from Buffer[2], the job took 5 seconds 
Printer 1 has started printing job from Client: 4, from Buffer[3]. The job will take 7 seconds. 
3 3
Client 3 has a 3 second job to print. Puts request in Buffer[4].
asa@APC ~/Desktop/Classes/Comp 310/assignment2 $ Printer 1 has finished its job from Buffer[3], the job took 7 seconds 
Printer 1 has started printing job from Client: 3, from Buffer[4]. The job will take 3 seconds. 
Printer 1 has finished its job from Buffer[4], the job took 3 seconds 


