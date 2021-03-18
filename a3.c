#include "a3.h"

time_t currTime;
struct tm *time_to_display;
char buffer [80];
long count;
long num;
int when_to_print_time;
long when_to_alarm_time;
int signum = 1;
bool alarm_sig = false;
bool stop_sig = false;
bool printInitialTime = true; 

// this function prints an alarm message when the user-specified value is reached
void *start_alarm(void *i){
    while(!alarm_sig);
    printf("ALARM\n");
    pthread_exit(NULL);
}
// this function sends a signal to end the program
void *timerManager(void *i){  
    time(&currTime);
    time_to_display = localtime(&currTime);
    
    sleep(when_to_alarm_time);
    signum++;   

    sleep(count - when_to_alarm_time);
    signum++;
    
    pthread_exit(NULL);
} 
// this function prints the time of day and the countdown timer
// when the countdown hits 0
void *clockManager(void *i){
    int counter = when_to_print_time;
    time(&currTime);
    time_to_display = localtime(&currTime);
    
    while(!stop_sig) {
        counter--;
        time(&currTime);
        time_to_display = localtime(&currTime);
        if (counter == 0 || printInitialTime) {
        	strftime (buffer,80,"Time of day: %T",time_to_display);
        	puts (buffer);
        	counter = when_to_print_time;
        	printInitialTime = false;
        }   
        num--;
        if (num == 0) 
        printf("Countdown timer = 0 seconds\n");
        sleep(1); 
    }    
    pthread_exit(NULL);
}

void signalManager() { // this function handles all the signals
    while(signum != 2); // alarm signal
    alarm_sig = true;
    
    while(signum != 3); // exit signal
    stop_sig = true;
    pthread_exit(NULL);
}
// Main function creates 4 threads which call other functions listed above.
// If no argument is entered the default values are 25 1 17.
int main(int argc, char* argv[]){
    pthread_t signalHandler, timer, printTime, alarm;
    signal(SIGINT, *signalManager);
        
    time(&currTime);
    time_to_display = localtime(&currTime);
        
     if(argc == 1){
        count = DEFAULT_COUNT;
        num = count;
        when_to_print_time = DEFAULT_PRINT;
        when_to_alarm_time = DEFAULT_ALARM;
    } 
    else if (argc > 1){
		count = strtol(argv[1], NULL, 10);
        when_to_print_time = strtol(argv[2], NULL, 10);
        when_to_alarm_time = strtol(argv[3], NULL, 10);
		// this checks to see if the values entered are valid
        if(count == 0 || (when_to_print_time != 1 && when_to_print_time != 60) || (when_to_alarm_time == 0 || when_to_alarm_time >= count)) {
            printf("\nInvalid input.\n");
            return 0;
        } 
        num = count;
    } 
	// creates four threads that call functions
    pthread_create(&signalHandler, NULL, (void*) &signalManager, NULL);
    pthread_create(&timer, NULL, &timerManager, NULL);
    pthread_create(&printTime, NULL, &clockManager, NULL);
    pthread_create(&alarm, NULL, &start_alarm, NULL);
        
    pthread_join(signalHandler, NULL);
    pthread_join(timer, NULL);
    pthread_join(printTime, NULL);
    pthread_join(alarm, NULL);
    // end of program
    printf("\nProgram has finished.\n");
    return 0;
}